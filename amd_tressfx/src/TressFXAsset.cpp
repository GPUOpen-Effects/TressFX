//--------------------------------------------------------------------------------------
// File: TressFXAsset.cpp
//
//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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
//--------------------------------------------------------------------------------------

#include "AMD_Types.h"
#include "Math\\Vector3D.h"
#include "Math\\Transform.h"
#include "Util.h"
#include "TressFXAsset.h"
#include "TressFXFileFormat.h"
#include <string>
#include <sstream>

using namespace std;
using namespace DirectX;

extern int g_TressFXNumVerticesPerStrand;

namespace AMD
{

//--------------------------------------------------------------------------------------
//
// Constructor
//
//--------------------------------------------------------------------------------------
TressFXAssetLoader::TressFXAssetLoader(void)
{
    m_HairAsset.m_pHairStrandType = NULL;
    m_HairAsset.m_pRefVectors = NULL;
    m_HairAsset.m_pTriangleVertices = NULL;
    m_HairAsset.m_pGlobalRotations = NULL;
    m_HairAsset.m_pLocalRotations = NULL;
    m_HairAsset.m_pVertices = NULL;
    m_HairAsset.m_pTangents = NULL;
    m_HairAsset.m_pThicknessCoeffs = NULL;
    m_HairAsset.m_pRestLengths = NULL;
    m_HairAsset.m_pFollowRootOffset = NULL;
    m_HairAsset.m_NumFollowHairsPerGuideHair = 4;
    m_HairAsset.m_NumGuideHairStrands = 0;
    m_HairAsset.m_NumOfVerticesInStrand = 0;
    m_HairAsset.m_TipSeparationFactor = 0;

    m_maxRadiusAroundGuideHair = 0.5;
    m_usingPerStrandTexCoords = false;
    m_usingPerVertexColors = false;
    m_usingPerVertexTexCoords = false;
}

//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
TressFXAssetLoader::~TressFXAssetLoader(void)
{
    Clear();
}

//--------------------------------------------------------------------------------------
//
// DestroyAll
//
// Deletes member variables
//
//--------------------------------------------------------------------------------------
void TressFXAssetLoader::DestroyAll()
{
    if ( m_HairAsset.m_pVertices )
    {
        delete [] m_HairAsset.m_pVertices;
        m_HairAsset.m_pVertices = NULL;
    }

    if ( m_HairAsset.m_pTangents )
    {
        delete [] m_HairAsset.m_pTangents;
        m_HairAsset.m_pTangents = NULL;
    }

    if ( m_HairAsset.m_pHairStrandType )
    {
        delete [] m_HairAsset.m_pHairStrandType;
        m_HairAsset.m_pHairStrandType = NULL;
    }

    if ( m_HairAsset.m_pStrandTexCoords )
    {
        delete [] m_HairAsset.m_pStrandTexCoords;
        m_HairAsset.m_pStrandTexCoords = NULL;
    }

    if ( m_HairAsset.m_pLocalRotations )
    {
        delete [] m_HairAsset.m_pLocalRotations;
        m_HairAsset.m_pLocalRotations = NULL;
    }

    if ( m_HairAsset.m_pGlobalRotations )
    {
        delete [] m_HairAsset.m_pGlobalRotations;
        m_HairAsset.m_pGlobalRotations = NULL;
    }

    if ( m_HairAsset.m_pRefVectors )
    {
        delete [] m_HairAsset.m_pRefVectors;
        m_HairAsset.m_pRefVectors = NULL;
    }

    if ( m_HairAsset.m_pTriangleVertices )
    {
        delete [] m_HairAsset.m_pTriangleVertices;
        m_HairAsset.m_pTriangleVertices = NULL;
    }

    if ( m_HairAsset.m_pThicknessCoeffs )
    {
        delete [] m_HairAsset.m_pThicknessCoeffs;
        m_HairAsset.m_pThicknessCoeffs = NULL;
    }

    if ( m_HairAsset.m_pRestLengths )
    {
        delete [] m_HairAsset.m_pRestLengths;
        m_HairAsset.m_pRestLengths = NULL;
    }

    if ( m_HairAsset.m_pFollowRootOffset )
    {
        delete [] m_HairAsset.m_pFollowRootOffset;
        m_HairAsset.m_pFollowRootOffset = NULL;
    }

    if (m_HairAsset.m_pMapping )
    {
        delete [] m_HairAsset.m_pMapping;
        m_HairAsset.m_pMapping = NULL;
    }
}

void TressFXAssetLoader::ConstructTransforms()
{
    // construct local and global transforms for all hair strands
    for ( int iStrand = 0; iStrand < m_HairAsset.m_NumTotalHairStrands; ++iStrand )
    {
        int indexRootVertMaster = iStrand * m_HairAsset.m_NumOfVerticesInStrand;

        // vertex 0
        {
            TressFXHairVertex& vert_i = m_Vertices[indexRootVertMaster];
            TressFXHairVertex& vert_i_plus_1 = m_Vertices[indexRootVertMaster + 1];

            const tressfx_vec3 vec = vert_i_plus_1.position - vert_i.position;
            tressfx_vec3 vecX = vec.NormalizeOther();

            tressfx_vec3 vecZ = vecX.Cross(tressfx_vec3(1.0, 0, 0));

            if ( vecZ.LengthSqr() < 0.0001 )
            {
                vecZ = vecX.Cross(tressfx_vec3(0, 1.0f, 0));
            }

            vecZ.Normalize();
            tressfx_vec3 vecY = vecZ.Cross(vecX).Normalize();

            tressfx_mat33 rotL2W;

            rotL2W(0, 0) = vecX.x;  rotL2W(0, 1) = vecY.x;      rotL2W(0, 2) = vecZ.x;
            rotL2W(1, 0) = vecX.y;  rotL2W(1, 1) = vecY.y;      rotL2W(1, 2) = vecZ.y;
            rotL2W(2, 0) = vecX.z;  rotL2W(2, 1) = vecY.z;      rotL2W(2, 2) = vecZ.z;

            vert_i.localTransform.GetRotation() = rotL2W;
            vert_i.localTransform.GetTranslation() = vert_i.position;
            vert_i.globalTransform = vert_i.localTransform; // For vertex 0, local and global transforms are the same.
        }

        // vertex 1 through n-1
        for ( int i = 1; i < (int)m_HairAsset.m_NumOfVerticesInStrand; i++ )
        {
            TressFXHairVertex& vert_i_minus_1 = m_Vertices[indexRootVertMaster + i - 1];
            TressFXHairVertex& vert_i = m_Vertices[indexRootVertMaster + i];

            tressfx_vec3 vec = vert_i.position - vert_i_minus_1.position;
            vec = vert_i_minus_1.globalTransform.GetRotation().InverseOther() * vec;

            tressfx_vec3 vecX = vec.NormalizeOther();

            tressfx_vec3 X = tressfx_vec3(1.0f, 0, 0);
            tressfx_vec3 rotAxis = X.Cross(vecX);
            float angle = acos(X.Dot(vecX));

            if ( abs(angle) < 0.001 || rotAxis.LengthSqr() < 0.001 )
            {
                vert_i.localTransform.GetRotation().SetIdentity();
            }
            else
            {
                rotAxis.Normalize();
                tressfx_quat rot = tressfx_quat(rotAxis, angle);
                vert_i.localTransform.GetRotation() = rot;
            }

            vert_i.localTransform.GetTranslation() = vec;
            vert_i.globalTransform = vert_i_minus_1.globalTransform * vert_i.localTransform;
            vert_i.referenceVector = vert_i.localTransform.GetTranslation();
        }
    }
}

//--------------------------------------------------------------------------------------
//
// GetTangentVectors
//
// Create two arbitrary tangent vectors (t0 and t1) perpendicular to the input normal vector (n).
//
//--------------------------------------------------------------------------------------
void GetTangentVectors(const tressfx_vec3& n, tressfx_vec3& t0, tressfx_vec3& t1)
{
    if ( fabsf(n[2]) > 0.707f )
    {
        float a = n[1]*n[1] + n[2]*n[2];
        float k = 1.0f/sqrtf(a);
        t0[0] = 0;
        t0[1] = -n[2]*k;
        t0[2] = n[1]*k;

        t1[0] = a*k;
        t1[1] = -n[0]*t0[2];
        t1[2] = n[0]*t0[1];
    }
    else
    {
        float a = n[0]*n[0] + n[1]*n[1];
        float k = 1.0f/sqrtf(a);
        t0[0] = -n[1]*k;
        t0[1] = n[0]*k;
        t0[2] = 0;

        t1[0] = -n[2]*t0[1];
        t1[1] = n[2]*t0[0];
        t1[2] = a*k;
    }
}

// Reads in a file of hair data and appends it to the list of hair strands
bool TressFXAssetLoader::LoadAppend(TressFX_HairBlob *pRawHairBlob, const TressFX_GuideFollowParams& guideFollowParams, int groupId)
{
    m_maxRadiusAroundGuideHair = guideFollowParams.radiusForFollowHair;
    m_HairAsset.m_TipSeparationFactor = guideFollowParams.tipSeparationFactor;
    m_HairAsset.m_NumFollowHairsPerGuideHair = guideFollowParams.numFollowHairsPerGuideHair;

    TressFXFileObject tfxFileObj;
    memcpy((void *)&tfxFileObj, pRawHairBlob->pHair, sizeof(TressFXFileObject));

    g_TressFXNumVerticesPerStrand = tfxFileObj.numVerticesPerStrand;

    bool bothEndsImmovable = tfxFileObj.bothEndsImmovable != 0;
    m_HairAsset.m_NumOfVerticesInStrand = tfxFileObj.numVerticesPerStrand;

    // Make sure number of vertices per strand is greater than two and less than or equal to thread group size (64). Also thread group size should be a multiple of number of vertices per strand.
    // So possible number is 4, 8, 16, 32 and 64.
    assert(m_HairAsset.m_NumOfVerticesInStrand > 2 && m_HairAsset.m_NumOfVerticesInStrand <= THREAD_GROUP_SIZE && THREAD_GROUP_SIZE % m_HairAsset.m_NumOfVerticesInStrand == 0);

    // number of strands to load from tfx file.
    unsigned numStrandsInFile = tfxFileObj.numGuideHairStrands;
    char *pHairSrc = (char *)pRawHairBlob->pHair;

    FLOAT *pVerts = (FLOAT *)(pHairSrc + tfxFileObj.verticesOffset);
    FLOAT *pTexCoords = NULL;

    if (tfxFileObj.perStrandTexCoordOffset != 0)
    {
        pTexCoords = (FLOAT *)(pHairSrc + tfxFileObj.perStrandTexCoordOffset);
        m_usingPerStrandTexCoords = true;
    }
    else
    {
        m_usingPerStrandTexCoords = false;
    }

    // Compute the total number of strands considering slave hairs and thread size.
    // Make the number of total strands a multiple of thread group size to avoid branching in compute shader.
    int numGuideStrands_old = m_HairAsset.m_NumGuideHairStrands;
    int numStrandsToLoad = numStrandsInFile - numStrandsInFile % THREAD_GROUP_SIZE;
    int numGuideStrands = numGuideStrands_old + numStrandsToLoad;
    int numTotalStrands = numGuideStrands * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1);
    int numTotalVertices = numTotalStrands * m_HairAsset.m_NumOfVerticesInStrand;

    m_HairAsset.m_NumGuideHairStrands = numGuideStrands;
    m_HairAsset.m_NumTotalHairStrands = numTotalStrands;
    m_HairAsset.m_NumGuideHairVertices = numGuideStrands * m_HairAsset.m_NumOfVerticesInStrand;
    m_HairAsset.m_NumTotalHairVertices = numTotalVertices;

    // reading strand and vertex data from tfx buffer.
    for ( int i = 0; i < numStrandsToLoad; i++ )
    {
        TressFXStrand guideStrand;
        guideStrand.m_bGuideHair = true;
        guideStrand.m_GroupID = groupId;

        for ( int j = 0; j < m_HairAsset.m_NumOfVerticesInStrand; j++ )
        {
            TressFXHairVertex v;
            m_Vertices.push_back(v);
            TressFXHairVertex& vert = m_Vertices[m_Vertices.size() - 1];

            vert.position.x = *pVerts++;
            vert.position.y = *pVerts++;
            vert.position.z = *pVerts++;

            // first two vertices are always immovable by having zero inverse mass.
            if ( j == 0 || j == 1 )
            {
                vert.invMass = 0;
            }
            else
            {
                vert.invMass = 1.0f;
            }

            // In some cases, two end vertices in both ends of strand are needed to be immovable.
            if ( bothEndsImmovable )
            {
                if ( bothEndsImmovable && (j == m_HairAsset.m_NumOfVerticesInStrand - 1 || j == m_HairAsset.m_NumOfVerticesInStrand - 2) )
                {
                    vert.invMass = 0;
                }
            }
        }

        if (m_usingPerStrandTexCoords)
        {
            assert(pTexCoords);
            guideStrand.m_texCoord.x = *pTexCoords++;
            guideStrand.m_texCoord.y = *pTexCoords++;
        }
        else
        {
            guideStrand.m_texCoord.x = 0;
            guideStrand.m_texCoord.y = 0;
        }

        m_HairStrands.push_back(guideStrand);

        // adding follow hair strands as placeholder.
        for ( int j = 0; j < m_HairAsset.m_NumFollowHairsPerGuideHair; j++ )
        {
            TressFXStrand followStrand;
            followStrand.m_bGuideHair = false;
            followStrand.m_GroupID = groupId;
            followStrand.m_texCoord.x = guideStrand.m_texCoord.x;
            followStrand.m_texCoord.y = guideStrand.m_texCoord.y;

            m_HairStrands.push_back(followStrand);

            for ( int k = 0; k < m_HairAsset.m_NumOfVerticesInStrand; k++ )
            {
                TressFXHairVertex v;
                m_Vertices.push_back(v);
                TressFXHairVertex& followVert = m_Vertices[m_Vertices.size() - 1];

                followVert.invMass = 0;
                followVert.position.Set(0, 0, 0);
            }
        }
    }

    // parse the hair skinning data
    if (pRawHairBlob->animationSize != 0)
    {
        TressFXSkinFileObject tfxSkinObj;
        memcpy((void *)&tfxSkinObj, pRawHairBlob->pAnimation, sizeof(TressFXSkinFileObject));
        HairToTriangleMapping *pMapping = (HairToTriangleMapping *)((char *)pRawHairBlob->pAnimation + tfxSkinObj.hairToMeshMap_Offset);
        m_HairAsset.m_pMapping = new HairToTriangleMapping[m_HairAsset.m_NumGuideHairStrands * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1)];
        memset(m_HairAsset.m_pMapping, 0, m_HairAsset.m_NumGuideHairStrands * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1) * sizeof(HairToTriangleMapping));
        mapping.resize(m_HairAsset.m_NumGuideHairStrands);

        // mesh mapping.
        for ( int i = 0; i < m_HairAsset.m_NumGuideHairStrands; i++ )
        {
            mapping[i].mesh = 0;
            mapping[i].triangle = pMapping[i].triangle;
            mapping[i].barycentricCoord[0] = pMapping[i].barycentricCoord[0];
            mapping[i].barycentricCoord[1] = pMapping[i].barycentricCoord[1];
            mapping[i].barycentricCoord[2] = pMapping[i].barycentricCoord[2];
        }

        float* pUVWCoord = (float*)((char *)pRawHairBlob->pAnimation + tfxSkinObj.perStrandUVCoordniate_Offset);

        for ( int i = 0; i < m_HairAsset.m_NumGuideHairStrands; i++ )
        {
            float u = pUVWCoord[3 * i];
            float v = pUVWCoord[3 * i + 1];

            int indexGuideStrand = i * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1);

            m_HairStrands[indexGuideStrand].m_texCoord.x = u;
            m_HairStrands[indexGuideStrand].m_texCoord.y = v;
        }

        m_usingPerStrandTexCoords = true;
    }
    else
    {
        m_HairAsset.m_pMapping = NULL;
    }

    return true;
}

// random number generator
float GetRandom(float Min, float Max)
{
    return ((float(rand()) / float(RAND_MAX)) * (Max - Min)) + Min;
}

int myrandom(int i) { (void)i; return 0; }

// Generates the follow hairs
void TressFXAssetLoader::GenerateFollowHairs()
{
    assert(m_HairAsset.m_NumFollowHairsPerGuideHair >= 0);

    m_SlaveOffset.resize(m_HairAsset.m_NumTotalHairStrands, tressfx_vec3(0, 0, 0));

    // Shuffle hair strands for LOD and density. Without shuffling, hair can disappear unevenly.
    srand(0);

    // Shuffle elements by randomly exchanging each other.
    for ( int i = 0; i < m_HairAsset.m_NumGuideHairStrands; i++ )
    {
        const int indexRand = rand() % m_HairAsset.m_NumGuideHairStrands;
        const int indexGuideStrand = i * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1);
        const int indexGuideStrandRand = indexRand * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1);

        // shuffle strands
        for ( int j = 0; j < m_HairAsset.m_NumFollowHairsPerGuideHair + 1; j++ )
        {
            TressFXStrand strandTemp = m_HairStrands[indexGuideStrand + j];
            m_HairStrands[indexGuideStrand + j] = m_HairStrands[indexGuideStrandRand + j];
            m_HairStrands[indexGuideStrandRand + j] = strandTemp;
        }

        // shuffle vertices
        int indexGuideVertex = indexGuideStrand * m_HairAsset.m_NumOfVerticesInStrand;
        int indexGuideVertexRand = indexGuideStrandRand * m_HairAsset.m_NumOfVerticesInStrand;

        for ( int j = 0; j < m_HairAsset.m_NumOfVerticesInStrand; j++ )
        {
            TressFXHairVertex vertexTemp = m_Vertices[indexGuideVertex + j];
            m_Vertices[indexGuideVertex + j] = m_Vertices[indexGuideVertexRand + j];
            m_Vertices[indexGuideVertexRand + j] = vertexTemp;
        }

        // shuffle mapping
        if ( m_HairAsset.m_pMapping )
        {
            HairToTriangleMapping map = mapping[i];
            mapping[i] = mapping[indexRand];
            mapping[indexRand] = map;
        }
    }

    if ( m_HairAsset.m_pMapping )
    {
        for ( int i = 0; i < m_HairAsset.m_NumGuideHairStrands; i++ )
        {
            int index = i * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1);
            m_HairAsset.m_pMapping[index].mesh = 0;
            m_HairAsset.m_pMapping[index].triangle = mapping[i].triangle;
            m_HairAsset.m_pMapping[index].barycentricCoord[0] = mapping[i].barycentricCoord[0];
            m_HairAsset.m_pMapping[index].barycentricCoord[1] = mapping[i].barycentricCoord[1];
            m_HairAsset.m_pMapping[index].barycentricCoord[2] = mapping[i].barycentricCoord[2];

            for ( int j = 1; j <= m_HairAsset.m_NumFollowHairsPerGuideHair; j++ )
            {
                m_HairAsset.m_pMapping[index + j].mesh = 0;
                m_HairAsset.m_pMapping[index + j].triangle = mapping[i].triangle;
                m_HairAsset.m_pMapping[index + j].barycentricCoord[0] = mapping[i].barycentricCoord[0];
                m_HairAsset.m_pMapping[index + j].barycentricCoord[1] = mapping[i].barycentricCoord[1];
                m_HairAsset.m_pMapping[index + j].barycentricCoord[2] = mapping[i].barycentricCoord[2];
            }
        }

        mapping.clear();
    }

    if ( m_HairAsset.m_NumFollowHairsPerGuideHair == 0 )
    {
        return;
    }

    // Generate slave hairs from guide hairs
    for ( int i = 0; i < m_HairAsset.m_NumGuideHairStrands; i++ )
    {
        int indexGuideStrand = i*(m_HairAsset.m_NumFollowHairsPerGuideHair + 1);
        const TressFXStrand& guideStrand = m_HairStrands[indexGuideStrand];
        int indexRootVertMaster = indexGuideStrand * m_HairAsset.m_NumOfVerticesInStrand;

        m_SlaveOffset[indexGuideStrand].Set(0, 0, 0);

        tressfx_vec3 v01 = m_Vertices[indexRootVertMaster + 1].position - m_Vertices[indexRootVertMaster].position;
        v01.Normalize();

        // Find two orthogonal unit tangent vectors to v01
        tressfx_vec3 t0, t1;
        GetTangentVectors(v01, t0, t1);

        for ( int j = 0; j < m_HairAsset.m_NumFollowHairsPerGuideHair; j++ )
        {
            int indexStrandFollow = indexGuideStrand + j + 1;
            TressFXStrand& followStrand = m_HairStrands[indexStrandFollow];
            followStrand.m_bGuideHair = false;
            followStrand.m_GroupID = guideStrand.m_GroupID;
            followStrand.m_texCoord = guideStrand.m_texCoord;

            int indexRootVertSlave = indexStrandFollow * m_HairAsset.m_NumOfVerticesInStrand;

            // offset vector from the guide strand's root vertex position
            tressfx_vec3 offset = GetRandom(-m_maxRadiusAroundGuideHair, m_maxRadiusAroundGuideHair)*t0 + GetRandom(-m_maxRadiusAroundGuideHair, m_maxRadiusAroundGuideHair)*t1;
            m_SlaveOffset[indexStrandFollow] = offset;

            for ( int k = 0; k < m_HairAsset.m_NumOfVerticesInStrand; k++ )
            {
                const TressFXHairVertex& guideVert = m_Vertices[indexRootVertMaster + k];
                TressFXHairVertex& followVert = m_Vertices[indexRootVertSlave + k];

                float factor = m_HairAsset.m_TipSeparationFactor*((float)k / ((float)m_HairAsset.m_NumOfVerticesInStrand)) + 1.0f;
                followVert.position = guideVert.position + offset*factor;
                followVert.invMass = guideVert.invMass;
            }
        }
    }
}

// Clear the array of hair strands
void TressFXAssetLoader::Clear()
{
    m_HairStrands.clear();
    m_Vertices.clear();

    m_HairAsset.m_LineIndices.clear();
    m_HairAsset.m_TriangleIndices.clear();

    DestroyAll();

    m_HairAsset.m_pHairStrandType = NULL;
    m_HairAsset.m_pRefVectors = NULL;
    m_HairAsset.m_pTriangleVertices = NULL;
    m_HairAsset.m_pGlobalRotations = NULL;
    m_HairAsset.m_pLocalRotations = NULL;
    m_HairAsset.m_pVertices = NULL;
    m_HairAsset.m_pTangents = NULL;
    m_HairAsset.m_pThicknessCoeffs = NULL;
    m_HairAsset.m_pRestLengths = NULL;
    m_HairAsset.m_pFollowRootOffset = NULL;
    m_HairAsset.m_NumFollowHairsPerGuideHair = 4;
    m_HairAsset.m_NumGuideHairStrands = 0;
    m_HairAsset.m_NumOfVerticesInStrand = 0;
    m_HairAsset.m_TipSeparationFactor = 0;

    m_maxRadiusAroundGuideHair = 0.5;
}

// Calculates the tangent value for each vertices of the strand
void TressFXAssetLoader::ComputeStrandTangent()
{
    for ( int iStrand = 0; iStrand < m_HairAsset.m_NumTotalHairStrands; ++iStrand )
    {
        //const TressFXStrand& strand = m_HairStrands[iStrand];
        int indexRootVertMaster = iStrand * m_HairAsset.m_NumOfVerticesInStrand;

        // vertex 0
        {
            TressFXHairVertex& vert_0 = m_Vertices[indexRootVertMaster];
            TressFXHairVertex& vert_1 = m_Vertices[indexRootVertMaster + 1];

            tressfx_vec3 tangent = vert_1.position - vert_0.position;
            tangent.Normalize();
            vert_0.tangent = tangent;
        }

        // vertex 1 through n-1
        for ( int i = 1; i < (int)m_HairAsset.m_NumOfVerticesInStrand - 1; i++ )
        {
            TressFXHairVertex& vert_i_minus_1 = m_Vertices[indexRootVertMaster + i - 1];
            TressFXHairVertex& vert_i = m_Vertices[indexRootVertMaster + i];
            TressFXHairVertex& vert_i_plus_1 = m_Vertices[indexRootVertMaster + i + 1];

            tressfx_vec3 tangent_pre = vert_i.position - vert_i_minus_1.position;
            tangent_pre.Normalize();

            tressfx_vec3 tangent_next = vert_i_plus_1.position - vert_i.position;
            tangent_next.Normalize();

            tressfx_vec3 tangent = tangent_pre + tangent_next;
            tangent = tangent.Normalize();

            vert_i.tangent = tangent;
        }
    }
}

void TressFXAssetLoader::ComputeDistanceToRoot()
{
    for ( int iStrand = 0; iStrand < m_HairAsset.m_NumTotalHairStrands; ++iStrand )
    {
        int indexRootVertMaster = iStrand * m_HairAsset.m_NumOfVerticesInStrand;

        float strandLength = 0;
        m_Vertices[indexRootVertMaster].texcoord.z = 0;

        // vertex 1 through n
        for ( int i = 1; i < (int)m_HairAsset.m_NumOfVerticesInStrand; ++i )
        {
            TressFXHairVertex& vert_i_minus_1 = m_Vertices[indexRootVertMaster + i - 1];
            TressFXHairVertex& vert_i = m_Vertices[indexRootVertMaster + i];

            tressfx_vec3 vec = vert_i.position - vert_i_minus_1.position;
            float disSeg = vec.Length();

            vert_i.texcoord.z = vert_i_minus_1.texcoord.z + disSeg;
            strandLength += disSeg;
        }

        for ( int i = 0; i< (int)m_HairAsset.m_NumOfVerticesInStrand; ++i )
        {
            TressFXHairVertex& vert_i = m_Vertices[indexRootVertMaster + i];
            vert_i.texcoord.z /= strandLength;
        }
    }
}

//--------------------------------------------------------------------------------------
//
// ProcessVertices
//
// After all of the vertices have been loaded ProcessVertices is called to create the
// associated data with the hair vertices, which includes attributes like tangents,strand
// length, and transformations. Also the hair type is stored with each vertex which
// allows different simulation parameters for different sections of the hair.
//
//--------------------------------------------------------------------------------------
void TressFXAssetLoader::ProcessVertices()
{
    // construct local and global transforms for each hair strand.
    ConstructTransforms();

    // compute tangent vectors
    ComputeStrandTangent();

    // compute distances to root vertices
    ComputeDistanceToRoot();

    m_HairAsset.m_pVertices = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pHairStrandType = new int[m_HairAsset.m_NumTotalHairStrands];

    if (m_usingPerStrandTexCoords)
    {
        m_HairAsset.m_pStrandTexCoords = new XMFLOAT2[m_HairAsset.m_NumTotalHairStrands];
    }
    else
    {
        m_HairAsset.m_pStrandTexCoords = NULL;
    }

    m_HairAsset.m_pTangents = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];

    // Initialize the hair strands and compute tangents
    for ( int i = 0; i < m_HairAsset.m_NumTotalHairVertices; i++ )
    {
        m_HairAsset.m_pTangents[i].x = m_Vertices[i].tangent.x;
        m_HairAsset.m_pTangents[i].y = m_Vertices[i].tangent.y;
        m_HairAsset.m_pTangents[i].z = m_Vertices[i].tangent.z;
    }

    m_HairAsset.m_pRestLengths = new float[m_HairAsset.m_NumTotalHairVertices];
    int index = 0;

    // Calculate rest lengths
    for ( int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++ )
    {
        int indexRootVert = i * m_HairAsset.m_NumOfVerticesInStrand;

        for ( int j = 0; j < m_HairAsset.m_NumOfVerticesInStrand - 1; j++ )
        {
            m_HairAsset.m_pRestLengths[index++] = (m_Vertices[indexRootVert + j].position - m_Vertices[indexRootVert + j + 1].position).Length();
        }

        // Since number of edges are one less than number of vertices in hair strand, below line acts as a placeholder.
        m_HairAsset.m_pRestLengths[index++] = 0;
    }

    assert(index == m_HairAsset.m_NumTotalHairVertices);

    m_HairAsset.m_pRefVectors = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pGlobalRotations = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pLocalRotations = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pTriangleVertices = new StrandVertex[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pThicknessCoeffs = new float[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pFollowRootOffset = new XMFLOAT4[m_HairAsset.m_NumTotalHairStrands];
    m_HairAsset.m_LineIndices.reserve(m_HairAsset.m_NumTotalHairVertices * 2);
    m_HairAsset.m_TriangleIndices.reserve(m_HairAsset.m_NumTotalHairVertices * 6);
    int id=0;
    index = 0;

    TressFXStrand* pGuideHair = NULL;
    int indexGuideHairStrand = -1;

    // initialize the rest of the hair data
    for ( int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++ )
    {
        int indexRootVert = i * m_HairAsset.m_NumOfVerticesInStrand;

        for ( int j = 0; j < m_HairAsset.m_NumOfVerticesInStrand - 1; j++ )
        {
            // line indices
            m_HairAsset.m_LineIndices.push_back(id);
            m_HairAsset.m_LineIndices.push_back(id + 1);

            // triangle indices
            m_HairAsset.m_TriangleIndices.push_back(2 * id);
            m_HairAsset.m_TriangleIndices.push_back(2 * id + 1);
            m_HairAsset.m_TriangleIndices.push_back(2 * id + 2);
            m_HairAsset.m_TriangleIndices.push_back(2 * id + 2);
            m_HairAsset.m_TriangleIndices.push_back(2 * id + 1);
            m_HairAsset.m_TriangleIndices.push_back(2 * id + 3);
            id++;
        }

        id++;

        TressFXHairVertex* vertices = &m_Vertices[indexRootVert];

        for ( int j = 0; j < m_HairAsset.m_NumOfVerticesInStrand; j++ )
        {
            // triangle vertices
            StrandVertex strandVertex;
            strandVertex.position = XMFLOAT3(vertices[j].position.x, vertices[j].position.y, vertices[j].position.z);
            strandVertex.tangent = XMFLOAT3(vertices[j].tangent.x, vertices[j].tangent.y, vertices[j].tangent.z);
            strandVertex.texcoord = XMFLOAT4(vertices[j].texcoord.x, vertices[j].texcoord.y, vertices[j].texcoord.z, 0);
            m_HairAsset.m_pTriangleVertices[index] = strandVertex;

            float tVal = m_HairAsset.m_pTriangleVertices[index].texcoord.z;
            m_HairAsset.m_pThicknessCoeffs[index] = sqrt(1.f - tVal * tVal);

            XMFLOAT4 v;

            // temp vertices
            v.x = vertices[j].position.x;
            v.y = vertices[j].position.y;
            v.z = vertices[j].position.z;
            v.w = vertices[j].invMass;
            m_HairAsset.m_pVertices[index] = v;

            // global rotations
            v.x = vertices[j].globalTransform.GetRotation().x;
            v.y = vertices[j].globalTransform.GetRotation().y;
            v.z = vertices[j].globalTransform.GetRotation().z;
            v.w = vertices[j].globalTransform.GetRotation().w;
            m_HairAsset.m_pGlobalRotations[index] = v;

            // local rotations
            v.x = vertices[j].localTransform.GetRotation().x;
            v.y = vertices[j].localTransform.GetRotation().y;
            v.z = vertices[j].localTransform.GetRotation().z;
            v.w = vertices[j].localTransform.GetRotation().w;
            m_HairAsset.m_pLocalRotations[index] = v;

            // ref vectors
            v.x = vertices[j].referenceVector.x;
            v.y = vertices[j].referenceVector.y;
            v.z = vertices[j].referenceVector.z;
            m_HairAsset.m_pRefVectors[index].x = v.x;
            m_HairAsset.m_pRefVectors[index].y = v.y;
            m_HairAsset.m_pRefVectors[index].z = v.z;

            index++;
        }

        int groupId = m_HairStrands[i].m_GroupID;
        m_HairAsset.m_pHairStrandType[i] = groupId;

        if ( m_usingPerStrandTexCoords )
        {
            m_HairAsset.m_pStrandTexCoords[i] = m_HairStrands[i].m_texCoord;
        }

        if ( m_HairStrands[i].m_bGuideHair )
        {
            indexGuideHairStrand = i;
            pGuideHair = &m_HairStrands[i];
            m_HairAsset.m_pFollowRootOffset[i] = XMFLOAT4(0, 0, 0, (float)indexGuideHairStrand); // forth component is an index to the guide hair strand. For guide hair, it points itself.
        }
        else
        {
            assert(pGuideHair);
            tressfx_vec3& offset = m_SlaveOffset[i];
            m_HairAsset.m_pFollowRootOffset[i] = XMFLOAT4(offset.x, offset.y, offset.z, (float)indexGuideHairStrand);
        }
    }

    // Find the bounding sphere
    BBox bBox;

    for ( int i = 0; i < (int)m_Vertices.size(); ++i )
    {
        const TressFXHairVertex& vertex = m_Vertices[i];
        bBox = Union(bBox, Float3(vertex.position.x, vertex.position.y, vertex.position.z));
    }

    Float3 c; float radius;
    bBox.BoundingSphere(&c, &radius);
    m_HairAsset.m_bSphere.center = XMFLOAT3(c.x, c.y, c.z);
    m_HairAsset.m_bSphere.radius = radius;
}

//--------------------------------------------------------------------------------------
//
// Serialize
//
// Serializes the hair asset into a TressFX_HairBlob structure which can be saved to disk
// for faster loading.
//
//--------------------------------------------------------------------------------------
bool TressFXAssetLoader::Serialize(TressFX_HairBlob* pTressFXHairBlob)
{
    stringbuf buffer;

    buffer.sputn((char *)&m_HairAsset.m_NumTotalHairVertices, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumTotalHairStrands, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumOfVerticesInStrand, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumGuideHairVertices, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumGuideHairStrands, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumFollowHairsPerGuideHair, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_TipSeparationFactor, sizeof(float));

    if (m_usingPerStrandTexCoords)
    {
        m_HairAsset.m_NumPerStrandTexCoords = m_HairAsset.m_NumTotalHairStrands;
    }
    else
    {
        m_HairAsset.m_NumPerStrandTexCoords = 0;
    }

    if (m_usingPerVertexColors)
    {
        m_HairAsset.m_NumPerVertexColors = m_HairAsset.m_NumTotalHairVertices;
    }
    else
    {
        m_HairAsset.m_NumPerVertexColors = 0;
    }

    if (m_usingPerVertexTexCoords)
    {
        m_HairAsset.m_NumPerVertexTexCoords = m_HairAsset.m_NumTotalHairVertices;
    }
    else
    {
        m_HairAsset.m_NumPerVertexTexCoords = 0;
    }

    if (m_HairAsset.m_pMapping != NULL)
    {
        m_HairAsset.m_NumHairToTriangleMappings = m_HairAsset.m_NumTotalHairStrands;
    }
    else
    {
        m_HairAsset.m_NumHairToTriangleMappings = 0;
    }

    buffer.sputn((char *)&m_HairAsset.m_NumPerStrandTexCoords, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumPerVertexColors, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumPerVertexTexCoords, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_NumHairToTriangleMappings, sizeof(int));

    buffer.sputn((char *)m_HairAsset.m_pHairStrandType, m_HairAsset.m_NumTotalHairStrands * sizeof(int));
    buffer.sputn((char *)m_HairAsset.m_pRefVectors, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    buffer.sputn((char *)m_HairAsset.m_pGlobalRotations, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    buffer.sputn((char *)m_HairAsset.m_pLocalRotations, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    buffer.sputn((char *)m_HairAsset.m_pVertices, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    buffer.sputn((char *)m_HairAsset.m_pTangents, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    buffer.sputn((char *)m_HairAsset.m_pTriangleVertices, m_HairAsset.m_NumTotalHairVertices * sizeof(StrandVertex));
    buffer.sputn((char *)m_HairAsset.m_pThicknessCoeffs, m_HairAsset.m_NumTotalHairVertices * sizeof(float));
    buffer.sputn((char *)m_HairAsset.m_pFollowRootOffset, m_HairAsset.m_NumTotalHairStrands * sizeof(XMFLOAT4));
    buffer.sputn((char *)m_HairAsset.m_pRestLengths, m_HairAsset.m_NumTotalHairVertices * sizeof(float));
    buffer.sputn((char *)&m_HairAsset.m_bSphere, sizeof(BSphere));
    int size = (int)m_HairAsset.m_TriangleIndices.size();
    buffer.sputn((char *)&size, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_TriangleIndices[0], m_HairAsset.m_TriangleIndices.size() * sizeof(int));
    size = (int)m_HairAsset.m_LineIndices.size();
    buffer.sputn((char *)&size, sizeof(int));
    buffer.sputn((char *)&m_HairAsset.m_LineIndices[0], m_HairAsset.m_LineIndices.size() * sizeof(int));

    if (m_usingPerStrandTexCoords)
    {
        buffer.sputn((char *)m_HairAsset.m_pStrandTexCoords, m_HairAsset.m_NumTotalHairStrands * sizeof(DirectX::XMFLOAT2));
    }

    // ToDo: put the per-vertex colors, per-vertex texture coordinates, and guide hair arrays here

    // hair to skinned mesh mapping
    if (m_HairAsset.m_pMapping != NULL)
    {
        buffer.sputn((char *)m_HairAsset.m_pMapping, m_HairAsset.m_NumTotalHairStrands * sizeof(HairToTriangleMapping));
    }

    string str = buffer.str();
    pTressFXHairBlob->size = (unsigned)str.size();
    pTressFXHairBlob->pHair = (void *) new char[pTressFXHairBlob->size];
    str.copy((char *)pTressFXHairBlob->pHair, pTressFXHairBlob->size);

    return true;
}

} // namespace AMD
