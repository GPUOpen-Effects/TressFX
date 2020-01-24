//---------------------------------------------------------------------------------------
// Loads and processes TressFX files.
// Inputs are binary files/streams/blobs
// Outputs are raw data that will mostly end up on the GPU.
//-------------------------------------------------------------------------------------
//
// Copyright (c) 2019 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#include "Math/Transform.h"
#include "Math/Vector3D.h"
#include "TressFXAsset.h"
#include "TressFXFileFormat.h"
#include "EngineInterface.h"

#include <vector>
#include <string>

static void GetTangentVectors(const Vector3& n, Vector3& t0, Vector3& t1)
{
    if (fabsf(n[2]) > 0.707f)
    {
        float a = n[1] * n[1] + n[2] * n[2];
        float k = 1.0f / sqrtf(a);
        t0[0]   = 0;
        t0[1]   = -n[2] * k;
        t0[2]   = n[1] * k;

        t1[0] = a * k;
        t1[1] = -n[0] * t0[2];
        t1[2] = n[0] * t0[1];
    }
    else
    {
        float a = n[0] * n[0] + n[1] * n[1];
        float k = 1.0f / sqrtf(a);
        t0[0]   = -n[1] * k;
        t0[1]   = n[0] * k;
        t0[2]   = 0;

        t1[0] = -n[2] * t0[1];
        t1[1] = n[2] * t0[0];
        t1[2] = a * k;
    }
}

static float GetRandom(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

TressFXAsset::TressFXAsset()
    : m_positions(NULL)
    , m_strandUV(NULL)
    , m_tangents(NULL)
    , m_followRootOffsets(NULL)
    , m_strandTypes(NULL)
    , m_thicknessCoeffs(NULL)
    , m_restLengths(NULL)
    , m_triangleIndices(NULL)
    , m_boneSkinningData(NULL)
    , m_numTotalStrands(0)
    , m_numTotalVertices(0)
    , m_numVerticesPerStrand(0)
    , m_numGuideStrands(0)
    , m_numGuideVertices(0)
    , m_numFollowStrandsPerGuide(0)

{
}

TressFXAsset::~TressFXAsset()
{
}

bool TressFXAsset::LoadHairData(FILE * ioObject)
{
    TressFXTFXFileHeader header = {};

    // read the header
    EI_Seek(ioObject, 0); // make sure the stream pos is at the beginning. 
    EI_Read((void*)&header, sizeof(TressFXTFXFileHeader), ioObject);

    // If the tfx version is lower than the current major version, exit. 
    if (header.version < AMD_TRESSFX_VERSION_MAJOR)
    {
        return false;
    }

    unsigned int numStrandsInFile = header.numHairStrands;

    // We make the number of strands be multiple of TRESSFX_SIM_THREAD_GROUP_SIZE. 
    m_numGuideStrands = (numStrandsInFile - numStrandsInFile % TRESSFX_SIM_THREAD_GROUP_SIZE) + TRESSFX_SIM_THREAD_GROUP_SIZE;

    m_numVerticesPerStrand = header.numVerticesPerStrand;

    // Make sure number of vertices per strand is greater than two and less than or equal to
    // thread group size (64). Also thread group size should be a mulitple of number of
    // vertices per strand. So possible number is 4, 8, 16, 32 and 64.
    TRESSFX_ASSERT(m_numVerticesPerStrand > 2 && m_numVerticesPerStrand <= TRESSFX_SIM_THREAD_GROUP_SIZE && TRESSFX_SIM_THREAD_GROUP_SIZE % m_numVerticesPerStrand == 0);

    m_numFollowStrandsPerGuide = 0;
    m_numTotalStrands = m_numGuideStrands; // Until we call GenerateFollowHairs, the number of total strands is equal to the number of guide strands. 
    m_numGuideVertices = m_numGuideStrands * m_numVerticesPerStrand;
    m_numTotalVertices = m_numGuideVertices; // Again, the total number of vertices is equal to the number of guide vertices here. 

    TRESSFX_ASSERT(m_numTotalVertices % TRESSFX_SIM_THREAD_GROUP_SIZE == 0); // number of total vertices should be multiple of thread group size. 
                                                                            // This assert is actually redundant because we already made m_numGuideStrands
                                                                            // and m_numTotalStrands are multiple of thread group size. 
                                                                            // Just demonstrating the requirement for number of vertices here in case 
                                                                            // you are to make your own loader. 

    m_positions.resize(m_numTotalVertices); // size of m_positions = number of total vertices * sizeo of each position vector. 

    // Read position data from the io stream. 
    EI_Seek(ioObject, header.offsetVertexPosition);
    EI_Read((void*)m_positions.data(), numStrandsInFile * m_numVerticesPerStrand * sizeof(AMD::float4), ioObject); // note that the position data in io stream contains only guide hairs. If we call GenerateFollowHairs
                                                                                                    // to generate follow hairs, m_positions will be re-allocated. 

                                                                                                    // We need to make up some strands to fill up the buffer because the number of strands from stream is not necessarily multile of thread size. 
    AMD::int32 numStrandsToMakeUp = m_numGuideStrands - numStrandsInFile;

    for (AMD::int32 i = 0; i < numStrandsToMakeUp; ++i)
    {
        for (AMD::int32 j = 0; j < m_numVerticesPerStrand; ++j)
        {
            AMD::int32 indexLastVertex = (numStrandsInFile - 1) * m_numVerticesPerStrand + j;
            AMD::int32 indexVertex = (numStrandsInFile + i) * m_numVerticesPerStrand + j;
            m_positions[indexVertex] = m_positions[indexLastVertex];
        }
    }

    // Read strand UVs
    EI_Seek(ioObject, header.offsetStrandUV);
    m_strandUV.resize(m_numTotalStrands); // If we call GenerateFollowHairs to generate follow hairs, 
                                                                                // m_strandUV will be re-allocated. 

    EI_Read((void*)m_strandUV.data(), numStrandsInFile * sizeof(AMD::float2), ioObject);

    // Fill up the last empty space
    AMD::int32 indexLastStrand = (numStrandsInFile - 1);

    for (int i = 0; i < numStrandsToMakeUp; ++i)
    {
        AMD::int32 indexStrand = (numStrandsInFile + i);
        m_strandUV[indexStrand] = m_strandUV[indexLastStrand];
    }

    m_followRootOffsets.resize(m_numTotalStrands);

    // Fill m_followRootOffsets with zeros
    memset(m_followRootOffsets.data(), 0, m_numTotalStrands * sizeof(AMD::float4));

    return true;
}

// This generates follow hairs around loaded guide hairs procedually with random distribution within the max radius input. 
// Calling this is optional. 
bool TressFXAsset::GenerateFollowHairs(int numFollowHairsPerGuideHair, float tipSeparationFactor, float maxRadiusAroundGuideHair)
{
    TRESSFX_ASSERT(numFollowHairsPerGuideHair >= 0);

    m_numFollowStrandsPerGuide = numFollowHairsPerGuideHair;

    // Nothing to do, just exit. 
    if (numFollowHairsPerGuideHair == 0)
        return false;

    // Recompute total number of hair strands and vertices with considering number of follow hairs per a guide hair. 
    m_numTotalStrands = m_numGuideStrands * (m_numFollowStrandsPerGuide + 1);
    m_numTotalVertices = m_numTotalStrands * m_numVerticesPerStrand;

    // keep the old buffers until the end of this function. 
    std::vector<Vector3> positionsGuide = m_positions;
    std::vector<AMD::float2> strandUVGuide = m_strandUV;

    // re-allocate all buffers
    m_positions.resize(m_numTotalVertices);
    m_strandUV.resize(m_numTotalStrands);
        
    m_followRootOffsets.resize(m_numTotalStrands);

    // type-cast to Vector3 to handle data easily. 
    Vector3* pos = m_positions.data();
    Vector3* followOffset = m_followRootOffsets.data();

    // Generate follow hairs
    for (int i = 0; i < m_numGuideStrands; i++)
    {
        int indexGuideStrand = i * (m_numFollowStrandsPerGuide + 1);
        int indexRootVertMaster = indexGuideStrand * m_numVerticesPerStrand;

        memcpy(&pos[indexRootVertMaster], &positionsGuide[i*m_numVerticesPerStrand], sizeof(Vector3)*m_numVerticesPerStrand);
        m_strandUV[indexGuideStrand] = strandUVGuide[i];

        followOffset[indexGuideStrand].Set(0, 0, 0);
        followOffset[indexGuideStrand].w = (float)indexGuideStrand;
        Vector3 v01 = pos[indexRootVertMaster + 1] - pos[indexRootVertMaster];
        v01.Normalize();

        // Find two orthogonal unit tangent vectors to v01
        Vector3 t0, t1;
        GetTangentVectors(v01, t0, t1);

        for (int j = 0; j < m_numFollowStrandsPerGuide; j++)
        {
            int indexStrandFollow = indexGuideStrand + j + 1;
            int indexRootVertFollow = indexStrandFollow * m_numVerticesPerStrand;

            m_strandUV[indexStrandFollow] = m_strandUV[indexGuideStrand];

            // offset vector from the guide strand's root vertex position
            Vector3 offset = GetRandom(-maxRadiusAroundGuideHair, maxRadiusAroundGuideHair) * t0 + GetRandom(-maxRadiusAroundGuideHair, maxRadiusAroundGuideHair) * t1;
            followOffset[indexStrandFollow] = offset;
            followOffset[indexStrandFollow].w = (float)indexGuideStrand;

            for (int k = 0; k < m_numVerticesPerStrand; k++)
            {
                const Vector3* guideVert = &pos[indexRootVertMaster + k];
                Vector3* followVert = &pos[indexRootVertFollow + k];

                float factor = tipSeparationFactor * ((float)k / ((float)m_numVerticesPerStrand)) + 1.0f;
                *followVert = *guideVert + offset * factor;
                (*followVert).w = guideVert->w;
            }
        }
    }

    return true;
}

bool TressFXAsset::ProcessAsset()
{
    m_strandTypes.resize(m_numTotalStrands);
    m_tangents.resize(m_numTotalVertices);
    m_restLengths.resize(m_numTotalVertices);
    m_thicknessCoeffs.resize(m_numTotalVertices);
    m_triangleIndices.resize(GetNumHairTriangleIndices());

    // compute tangent vectors
    ComputeStrandTangent();

    // compute thickness coefficients
    ComputeThicknessCoeffs();

    // compute rest lengths
    ComputeRestLengths();

    // triangle index
    FillTriangleIndexArray();

    for (int i = 0; i < m_numTotalStrands; i++)
        m_strandTypes[i] = 0;

    return true;
}

void TressFXAsset::FillTriangleIndexArray()
{
    TRESSFX_ASSERT(m_numTotalVertices == m_numTotalStrands * m_numVerticesPerStrand);
    TRESSFX_ASSERT(m_triangleIndices.size() != 0);

    AMD::int32 id = 0;
    int iCount = 0;

    for (int i = 0; i < m_numTotalStrands; i++)
    {
        for (int j = 0; j < m_numVerticesPerStrand - 1; j++)
        {
            m_triangleIndices[iCount++] = 2 * id;
            m_triangleIndices[iCount++] = 2 * id + 1;
            m_triangleIndices[iCount++] = 2 * id + 2;
            m_triangleIndices[iCount++] = 2 * id + 2;
            m_triangleIndices[iCount++] = 2 * id + 1;
            m_triangleIndices[iCount++] = 2 * id + 3;

            id++;
        }

        id++;
    }

    TRESSFX_ASSERT(iCount == 6 * m_numTotalStrands * (m_numVerticesPerStrand - 1)); // iCount == GetNumHairTriangleIndices()
}

void TressFXAsset::ComputeStrandTangent()
{
    Vector3* pos = (Vector3*)m_positions.data();
    Vector3* tan  = (Vector3*)m_tangents.data();

    for (int iStrand = 0; iStrand < m_numTotalStrands; ++iStrand)
    {
        int indexRootVertMaster = iStrand * m_numVerticesPerStrand;

        // vertex 0
        {
            Vector3& vert_0 = pos[indexRootVertMaster];
            Vector3& vert_1 = pos[indexRootVertMaster + 1];

            Vector3 tangent = vert_1 - vert_0;
            tangent.Normalize();
            tan[indexRootVertMaster] = tangent;
        }

        // vertex 1 through n-1
        for (int i = 1; i < (int)m_numVerticesPerStrand - 1; i++)
        {
            Vector3& vert_i_minus_1 = pos[indexRootVertMaster + i - 1];
            Vector3& vert_i         = pos[indexRootVertMaster + i];
            Vector3& vert_i_plus_1  = pos[indexRootVertMaster + i + 1];

            Vector3 tangent_pre = vert_i - vert_i_minus_1;
            tangent_pre.Normalize();

            Vector3 tangent_next = vert_i_plus_1 - vert_i;
            tangent_next.Normalize();

            Vector3 tangent = tangent_pre + tangent_next;
            tangent              = tangent.Normalize();

            tan[indexRootVertMaster + i] = tangent;
        }
    }
}

void TressFXAsset::ComputeThicknessCoeffs()
{
    Vector3* pos = (Vector3*)m_positions.data();

    int index = 0;

    for (int iStrand = 0; iStrand < m_numTotalStrands; ++iStrand)
    {
        int   indexRootVertMaster = iStrand * m_numVerticesPerStrand;
        float strandLength        = 0;
        float tVal                = 0;

        // vertex 1 through n
        for (int i = 1; i < (int)m_numVerticesPerStrand; ++i)
        {
            Vector3& vert_i_minus_1 = pos[indexRootVertMaster + i - 1];
            Vector3& vert_i         = pos[indexRootVertMaster + i];

            Vector3 vec    = vert_i - vert_i_minus_1;
            float        disSeg = vec.Length();

            tVal += disSeg;
            strandLength += disSeg;
        }

        for (int i = 0; i < (int)m_numVerticesPerStrand; ++i)
        {
            tVal /= strandLength;
            m_thicknessCoeffs[index++] = sqrt(1.f - tVal * tVal);
        }
    }
}

void TressFXAsset::ComputeRestLengths()
{
    Vector3* pos = (Vector3*)m_positions.data();
    float*        restLen = (float*)m_restLengths.data();

    int index = 0;

    // Calculate rest lengths
    for (int i = 0; i < m_numTotalStrands; i++)
    {
        int indexRootVert = i * m_numVerticesPerStrand;

        for (int j = 0; j < m_numVerticesPerStrand - 1; j++)
        {
            restLen[index++] =
                (pos[indexRootVert + j] - pos[indexRootVert + j + 1]).Length();
        }

        // Since number of edges are one less than number of vertices in hair strand, below
        // line acts as a placeholder.
        restLen[index++] = 0;
    }
}

bool TressFXAsset::LoadBoneData(FILE * ioObject, int skinNumber, EI_Scene * scene)
{
    m_boneSkinningData.resize(0);

    AMD::int32 numOfBones = 0;
    EI_Seek(ioObject, 0);
    EI_Read((void*)&numOfBones, sizeof(AMD::int32), ioObject);

    size_t startOfBoneNames = (sizeof(AMD::int32) * numOfBones);
    size_t currentNamePosition = startOfBoneNames;
    size_t currentOffSetPosition = 0;

    std::vector<std::string> boneNames(numOfBones);
    std::vector<int> boneEngineIds(numOfBones);
    for (int i = 0; i < numOfBones; i++)
    {
        int boneIndex;
        EI_Read((char*)&boneIndex, sizeof(AMD::int32), ioObject);

        AMD::int32 charLen = 0;
        EI_Read((char*)&charLen, sizeof(AMD::int32), ioObject); // character length includes null termination already.

        char boneName[128];
        EI_Read(boneName, sizeof(char) * charLen, ioObject);
        boneNames[i] = boneName;
    }

    for (int i = 0; i < numOfBones; ++i)
    {
        boneEngineIds[i] = scene->GetBoneIdByName(skinNumber, boneNames[i].c_str());
    }

    // Reading the number of strands 
    AMD::int32 numOfStrandsInStream = 0;
    EI_Read((char*)&numOfStrandsInStream, sizeof(AMD::int32), ioObject);

    //If the number of strands from the input stream (tfxbone) is bigger than what we already know from tfx, something is wrong.
    if (m_numGuideStrands < numOfStrandsInStream)
        return 0;

    AMD::int32 boneSkinningMemSize = m_numTotalStrands * sizeof(TressFXBoneSkinningData);
    m_boneSkinningData.resize(m_numTotalStrands);

    TressFXBoneSkinningData skinData;
    for (int i = 0; i < numOfStrandsInStream; ++i)
    {
        AMD::int32 index = 0; // Well, we don't really use this here. 
        EI_Read((char*)&index, sizeof(AMD::int32), ioObject);

        for (AMD::int32 j = 0; j < TRESSFX_MAX_INFLUENTIAL_BONE_COUNT; ++j)
        {
            AMD::int32 boneIndex;
            EI_Read((char*)&boneIndex, sizeof(AMD::int32), ioObject);
            assert(boneIndex >= 0);
            skinData.boneIndex[j] = (float)boneEngineIds[boneIndex]; // Change the joint index to be what the engine wants
            EI_Read((char*)&skinData.weight[j], sizeof(AMD::real32), ioObject);
        }

        float weightSum = skinData.weight[0] + skinData.weight[1] + skinData.weight[2] + skinData.weight[3];

        assert(weightSum > 0.0f);
        assert(skinData.weight[0] != 0.0f);

        // If bone index is -1, then it means that there is no bone associated to this. In this case we simply replace it with zero.
        // This is safe because the corresonding weight should be zero anyway.
        skinData.boneIndex[0] = skinData.boneIndex[0] == -1.f ? 0 : skinData.boneIndex[0];
        skinData.boneIndex[1] = skinData.boneIndex[1] == -1.f ? 0 : skinData.boneIndex[1];
        skinData.boneIndex[2] = skinData.boneIndex[2] == -1.f ? 0 : skinData.boneIndex[2];
        skinData.boneIndex[3] = skinData.boneIndex[3] == -1.f ? 0 : skinData.boneIndex[3];

        m_boneSkinningData[i * (m_numFollowStrandsPerGuide + 1)] = skinData;
    }
    // fill skinning info for markup hair
    for (int i = numOfStrandsInStream; i < m_numGuideStrands; ++i)
    {
        m_boneSkinningData[i * (m_numFollowStrandsPerGuide + 1)] = skinData;
    }

    return true;
}
