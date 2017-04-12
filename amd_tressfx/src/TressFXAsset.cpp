//---------------------------------------------------------------------------------------
// Loads and processes TressFX files.
// Inputs are binary files/streams/blobs
// Outputs are raw data that will mostly end up on the GPU.
//-------------------------------------------------------------------------------------
//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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
#include "TressFXGPUInterface.h"
#include "TressFXEngineInterface.h"

#include <vector>
#include <string>

using namespace AMD::TRESSFX;

namespace AMD
{
    static void GetTangentVectors(const tressfx_vec3& n, tressfx_vec3& t0, tressfx_vec3& t1)
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
        , m_refVectors(NULL)
        , m_globalRotations(NULL)
        , m_localRotations(NULL)
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
        Clear();
    }

    void TressFXAsset::Clear()
    {
        m_numTotalStrands = 0;
        m_numTotalVertices = 0;
        m_numVerticesPerStrand = 0;
        m_numGuideStrands = 0;
        m_numGuideVertices = 0;
        m_numFollowStrandsPerGuide = 0;

        EI_Safe_Free(m_positions);
        EI_Safe_Free(m_strandUV);
        EI_Safe_Free(m_refVectors);
        EI_Safe_Free(m_globalRotations);
        EI_Safe_Free(m_localRotations);
        EI_Safe_Free(m_tangents);
        EI_Safe_Free(m_followRootOffsets);
        EI_Safe_Free(m_strandTypes);
        EI_Safe_Free(m_thicknessCoeffs);
        EI_Safe_Free(m_restLengths);
        EI_Safe_Free(m_triangleIndices);
        EI_Safe_Free(m_boneSkinningData);
    }

    bool TressFXAsset::LoadHairData(EI_StreamRef ioObject)
    {
        // Clear all data before loading an asset. 
        Clear();

        TressFXTFXFileHeader header;

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

        m_positions = (TRESSFX::float4*)EI_Malloc(m_numTotalVertices * sizeof(float4)); // size of m_positions = number of total vertices * sizeo of each position vector. 

        if (!m_positions)
        {
            Clear();
            return false;
        }

        // Read position data from the io stream. 
        EI_Seek(ioObject, header.offsetVertexPosition);
        EI_Read((void*)m_positions, numStrandsInFile * m_numVerticesPerStrand * sizeof(float4), ioObject); // note that the position data in io stream contains only guide hairs. If we call GenerateFollowHairs
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
        m_strandUV = (TRESSFX::float2*)EI_Malloc(m_numTotalStrands * sizeof(float2)); // If we call GenerateFollowHairs to generate follow hairs, 
                                                                                  // m_strandUV will be re-allocated. 

        if (!m_strandUV)
        {
            // If we have failed to allocate memory, then clear all buffers and exit.
            Clear();
            return false;
        }

        EI_Read((void*)m_strandUV, numStrandsInFile * sizeof(float2), ioObject);

        // Fill up the last empty space
        AMD::int32 indexLastStrand = (numStrandsInFile - 1);

        for (AMD::int32 i = 0; i < numStrandsToMakeUp; ++i)
        {
            AMD::int32 indexStrand = (numStrandsInFile + i);
            m_strandUV[indexStrand] = m_strandUV[indexLastStrand];
        }

        m_followRootOffsets = (TRESSFX::float4*)EI_Malloc(m_numTotalStrands * sizeof(float4));

        // Fill m_followRootOffsets with zeros
        memset(m_followRootOffsets, 0, m_numTotalStrands * sizeof(float4));

        // If we have failed to allocate buffers, then clear the allocated ones and exit. 
        if (!m_followRootOffsets)
        {
            Clear();
            return false;
        }

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
        TRESSFX::float4* positionsGuide = m_positions;
        TRESSFX::float2* strandUVGuide = m_strandUV;

        // re-allocate all buffers
        m_positions = (TRESSFX::float4*)EI_Malloc(m_numTotalVertices * sizeof(float4));
        m_strandUV = (TRESSFX::float2*)EI_Malloc(m_numTotalStrands * sizeof(float2));
        
        EI_Safe_Free(m_followRootOffsets);
        m_followRootOffsets = (TRESSFX::float4*)EI_Malloc(m_numTotalStrands * sizeof(float4));

        // If we have failed to allocate buffers, then clear the allocated ones and exit. 
        if (!m_positions || !m_strandUV || !m_followRootOffsets)
        {
            Clear();
            return false;
        }

        // type-cast to tressfx_vec3 to handle data easily. 
        TRESSFX_ASSERT(sizeof(tressfx_vec3) == sizeof(TRESSFX::float4)); // sizeof(tressfx_vec3) is 4*sizeof(float)  
        tressfx_vec3* pos = static_cast<tressfx_vec3*>((void*)m_positions);
        tressfx_vec3* followOffset = static_cast<tressfx_vec3*>((void*)m_followRootOffsets);

        // Generate follow hairs
        for (int i = 0; i < m_numGuideStrands; i++)
        {
            int indexGuideStrand = i * (m_numFollowStrandsPerGuide + 1);
            int indexRootVertMaster = indexGuideStrand * m_numVerticesPerStrand;

            memcpy(&pos[indexRootVertMaster], &positionsGuide[i*m_numVerticesPerStrand], sizeof(tressfx_vec3)*m_numVerticesPerStrand);
            m_strandUV[indexGuideStrand] = strandUVGuide[i];

            followOffset[indexGuideStrand].Set(0, 0, 0);
            followOffset[indexGuideStrand].w = (float)indexGuideStrand;
            tressfx_vec3 v01 = pos[indexRootVertMaster + 1] - pos[indexRootVertMaster];
            v01.Normalize();

            // Find two orthogonal unit tangent vectors to v01
            tressfx_vec3 t0, t1;
            GetTangentVectors(v01, t0, t1);

            for (int j = 0; j < m_numFollowStrandsPerGuide; j++)
            {
                int indexStrandFollow = indexGuideStrand + j + 1;
                int indexRootVertFollow = indexStrandFollow * m_numVerticesPerStrand;

                m_strandUV[indexStrandFollow] = m_strandUV[indexGuideStrand];

                // offset vector from the guide strand's root vertex position
                tressfx_vec3 offset = GetRandom(-maxRadiusAroundGuideHair, maxRadiusAroundGuideHair) * t0 + GetRandom(-maxRadiusAroundGuideHair, maxRadiusAroundGuideHair) * t1;
                followOffset[indexStrandFollow] = offset;
                followOffset[indexStrandFollow].w = (float)indexGuideStrand;

                for (int k = 0; k < m_numVerticesPerStrand; k++)
                {
                    const tressfx_vec3* guideVert = &pos[indexRootVertMaster + k];
                    tressfx_vec3* followVert = &pos[indexRootVertFollow + 1];

                    float factor = tipSeparationFactor * ((float)k / ((float)m_numVerticesPerStrand)) + 1.0f;
                    *followVert = *guideVert + offset * factor;
                    (*followVert).w = guideVert->w;
                }
            }
        }

        EI_Safe_Free(positionsGuide);
        EI_Safe_Free(strandUVGuide);

        return true;
    }

    bool TressFXAsset::ProcessAsset()
    {
        EI_Safe_Free(m_strandTypes);
        m_strandTypes = (AMD::int32*)EI_Malloc(m_numTotalStrands * sizeof(int));

        EI_Safe_Free(m_tangents);
        m_tangents = (TRESSFX::float4*)EI_Malloc(m_numTotalVertices * sizeof(float4));

        EI_Safe_Free(m_restLengths);
        m_restLengths = (AMD::real32*)EI_Malloc(m_numTotalVertices * sizeof(float));

        EI_Safe_Free(m_refVectors);
        m_refVectors = (TRESSFX::float4*)EI_Malloc(m_numTotalVertices * sizeof(float4));

        EI_Safe_Free(m_globalRotations);
        m_globalRotations = (TRESSFX::float4*)EI_Malloc(m_numTotalVertices * sizeof(float4));

        EI_Safe_Free(m_localRotations);
        m_localRotations = (TRESSFX::float4*)EI_Malloc(m_numTotalVertices * sizeof(float4));

        EI_Safe_Free(m_thicknessCoeffs);
        m_thicknessCoeffs = (AMD::real32*)EI_Malloc(m_numTotalVertices * sizeof(float));

        EI_Safe_Free(m_triangleIndices);
        m_triangleIndices = (AMD::int32*)EI_Malloc(GetNumHairTriangleIndices() * sizeof(int32));

        // If we have failed to allocate buffers, then clear the allocated ones and exit. 
        if (!m_strandTypes || !m_tangents || !m_restLengths || !m_refVectors || !m_globalRotations ||
            !m_localRotations || !m_thicknessCoeffs || !m_triangleIndices)
        {
            Clear();
            return false;
        }

        // construct local and global transforms for each hair strand.
        ComputeTransforms();

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
        TRESSFX_ASSERT(m_triangleIndices != nullptr);

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
        tressfx_vec3* pos = (tressfx_vec3*)m_positions;
        tressfx_vec3* tan  = (tressfx_vec3*)m_tangents;

        for (int iStrand = 0; iStrand < m_numTotalStrands; ++iStrand)
        {
            int indexRootVertMaster = iStrand * m_numVerticesPerStrand;

            // vertex 0
            {
                tressfx_vec3& vert_0 = pos[indexRootVertMaster];
                tressfx_vec3& vert_1 = pos[indexRootVertMaster + 1];

                tressfx_vec3 tangent = vert_1 - vert_0;
                tangent.Normalize();
                tan[indexRootVertMaster] = tangent;
            }

            // vertex 1 through n-1
            for (int i = 1; i < (int)m_numVerticesPerStrand - 1; i++)
            {
                tressfx_vec3& vert_i_minus_1 = pos[indexRootVertMaster + i - 1];
                tressfx_vec3& vert_i         = pos[indexRootVertMaster + i];
                tressfx_vec3& vert_i_plus_1  = pos[indexRootVertMaster + i + 1];

                tressfx_vec3 tangent_pre = vert_i - vert_i_minus_1;
                tangent_pre.Normalize();

                tressfx_vec3 tangent_next = vert_i_plus_1 - vert_i;
                tangent_next.Normalize();

                tressfx_vec3 tangent = tangent_pre + tangent_next;
                tangent              = tangent.Normalize();

                tan[indexRootVertMaster + i] = tangent;
            }
        }
    }

    void TressFXAsset::ComputeThicknessCoeffs()
    {
        tressfx_vec3* pos = (tressfx_vec3*)m_positions;

        int index = 0;

        for (int iStrand = 0; iStrand < m_numTotalStrands; ++iStrand)
        {
            int   indexRootVertMaster = iStrand * m_numVerticesPerStrand;
            float strandLength        = 0;
            float tVal                = 0;

            // vertex 1 through n
            for (int i = 1; i < (int)m_numVerticesPerStrand; ++i)
            {
                tressfx_vec3& vert_i_minus_1 = pos[indexRootVertMaster + i - 1];
                tressfx_vec3& vert_i         = pos[indexRootVertMaster + i];

                tressfx_vec3 vec    = vert_i - vert_i_minus_1;
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
        tressfx_vec3* pos = (tressfx_vec3*)m_positions;
        float*        restLen = (float*)m_restLengths;

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

    void TressFXAsset::ComputeTransforms()
    {
        tressfx_vec3* pos = (tressfx_vec3*)m_positions;
        tressfx_quat* globalRot = (tressfx_quat*)m_globalRotations;
        tressfx_quat* localRot  = (tressfx_quat*)m_localRotations;
        tressfx_vec3* ref      = (tressfx_vec3*)m_refVectors;

        // construct local and global transforms for all hair strands
        for (int iStrand = 0; iStrand < m_numTotalStrands; ++iStrand)
        {
            int indexRootVertMaster = iStrand * m_numVerticesPerStrand;

            // vertex 0
            {
                tressfx_vec3& vert_i = pos[indexRootVertMaster];
                tressfx_vec3& vert_i_plus_1 = pos[indexRootVertMaster + 1];

                const tressfx_vec3 vec  = vert_i_plus_1 - vert_i;
                tressfx_vec3       vecX = vec.NormalizeOther();

                tressfx_vec3 vecZ = vecX.Cross(tressfx_vec3(1.0, 0, 0));

                if (vecZ.LengthSqr() < 0.0001)
                {
                    vecZ = vecX.Cross(tressfx_vec3(0, 1.0f, 0));
                }

                vecZ.Normalize();
                tressfx_vec3 vecY = vecZ.Cross(vecX).Normalize();

                tressfx_mat33 rotL2W;

                rotL2W(0, 0) = vecX.x;
                rotL2W(0, 1) = vecY.x;
                rotL2W(0, 2) = vecZ.x;
                rotL2W(1, 0) = vecX.y;
                rotL2W(1, 1) = vecY.y;
                rotL2W(1, 2) = vecZ.y;
                rotL2W(2, 0) = vecX.z;
                rotL2W(2, 1) = vecY.z;
                rotL2W(2, 2) = vecZ.z;

                tressfx_quat rot = rotL2W;
                localRot[indexRootVertMaster] = globalRot[indexRootVertMaster] = rot;  // For vertex 0, local and global transforms are the same.
            }

            // vertex 1 through n-1
            for (int i = 1; i < (int)m_numVerticesPerStrand; i++)
            {
                tressfx_vec3& vert_i_minus_1 = pos[indexRootVertMaster + i - 1];
                tressfx_vec3& vert_i = pos[indexRootVertMaster + i];
                tressfx_vec3  vec = vert_i - vert_i_minus_1;
                vec = globalRot[indexRootVertMaster + i - 1].InverseOther() * vec;

                tressfx_vec3 vecX = vec.NormalizeOther();
                tressfx_vec3 X = tressfx_vec3(1.0f, 0, 0);
                tressfx_vec3 rotAxis = X.Cross(vecX);
                float angle = acos(X.Dot(vecX));

                if (abs(angle) < 0.001 || rotAxis.LengthSqr() < 0.001)
                {
                    localRot[indexRootVertMaster + i].SetIdentity();
                }
                else
                {
                    rotAxis.Normalize();
                    tressfx_quat rot = tressfx_quat(rotAxis, angle);
                    localRot[indexRootVertMaster + i] = rot;
                }

                globalRot[indexRootVertMaster + i] = globalRot[indexRootVertMaster + i - 1] * localRot[indexRootVertMaster + i];
                ref[indexRootVertMaster + i] = vec;
            }
        }
    }

    bool TressFXAsset::LoadBoneData(const TressFXSkeletonInterface& skeletonData, EI_StreamRef ioObject)
    {
        EI_Safe_Free(m_boneSkinningData);

        AMD::int32 numOfBones = 0;
        EI_Seek(ioObject, 0);
        EI_Read((void*)&numOfBones, sizeof(AMD::int32), ioObject);

        size_t startOfBoneNames = (sizeof(AMD::int32) * numOfBones);
        size_t currentNamePosition = startOfBoneNames;
        size_t currentOffSetPosition = 0;

        std::vector<std::string> boneNames;
        boneNames.resize(numOfBones);

        for (int i = 0; i < numOfBones; i++)
        {
            AMD::int32 index = 0;
            EI_Read((char*)&index, sizeof(AMD::int32), ioObject);

            AMD::int32 charLen = 0;
            EI_Read((char*)&charLen, sizeof(AMD::int32), ioObject); // character length includes null termination already.

            boneNames[i].resize(charLen);
            EI_Read(&boneNames[i][0], sizeof(char) * charLen, ioObject);
        }

        // Reading the number of strands 
        AMD::int32 numOfStrandsInStream = 0;
        EI_Read((char*)&numOfStrandsInStream, sizeof(AMD::int32), ioObject);

        // If the number of strands from the input stream (tfxbone) is bigger than what we already know from tfx, something is wrong. 
        //if (m_numGuideStrands < numOfStrandsInStream)
        //    return 0;

        AMD::int32 boneSkinningMemSize = m_numTotalStrands * sizeof(TressFXBoneSkinningData);
        m_boneSkinningData = (TressFXBoneSkinningData*)EI_Malloc(boneSkinningMemSize);

        // We failed to allocate memory. 
        if (!m_boneSkinningData)
        {
            EI_Safe_Free(m_boneSkinningData);
            return false;
        }

        for (AMD::int32 i = 0; i < m_numGuideStrands; ++i)
        {
            AMD::int32 index = 0; // Well, we don't really use this here. 
            EI_Read((char*)&index, sizeof(AMD::int32), ioObject);

            TressFXBoneSkinningData skinData;

            for (AMD::int32 j = 0; j < TRESSFX_MAX_INFLUENTIAL_BONE_COUNT; ++j)
            {
                AMD::int32 boneIndex;
                EI_Read((char*)&boneIndex, sizeof(AMD::int32), ioObject);
                skinData.boneIndex[j] = (float)boneIndex;
                skinData.boneIndex[j] = (float)skeletonData.GetBoneIndexByName(boneNames[(int)skinData.boneIndex[j]].c_str()); // Change the joint index to be what the engine wants
                EI_Read((char*)&skinData.weight[j], sizeof(AMD::real32), ioObject);
            }

            // If bone index is -1, then it means that there is no bone associated to this. In this case we simply replace it with zero.
            // This is safe because the corresonding weight should be zero anyway.
            skinData.boneIndex[0] = skinData.boneIndex[0] == -1.f ? 0 : skinData.boneIndex[0];
            skinData.boneIndex[1] = skinData.boneIndex[1] == -1.f ? 0 : skinData.boneIndex[1];
            skinData.boneIndex[2] = skinData.boneIndex[2] == -1.f ? 0 : skinData.boneIndex[2];
            skinData.boneIndex[3] = skinData.boneIndex[3] == -1.f ? 0 : skinData.boneIndex[3];

            m_boneSkinningData[i * (m_numFollowStrandsPerGuide + 1)] = skinData;
        }

        return true;
    }
}  // Namespace AMD
