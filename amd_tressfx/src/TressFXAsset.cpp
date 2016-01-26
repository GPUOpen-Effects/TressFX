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
// ConstructAsset
//
// Initializes the local, global transformations and reference vector for the hair strand
//
//--------------------------------------------------------------------------------------
void TressFXStrand::ConstructAsset()
{
    // vertex 0
    {
        HairVertex& vert_i = m_VertexArray[0];
        HairVertex& vert_i_plus_1 = m_VertexArray[1];

        const CVector3D vec = vert_i_plus_1.position - vert_i.position;
        CVector3D vecX = vec.NormalizeOther();

        CVector3D vecZ = vecX.Cross(CVector3D(1.0, 0, 0));

        if ( vecZ.LengthSqr() < 0.0001 )
        {
            vecZ = vecX.Cross(CVector3D(0, 1.0f, 0));
        }

        vecZ.Normalize();
        CVector3D vecY = vecZ.Cross(vecX).Normalize();

        CMatrix33 rotL2W;

        rotL2W(0, 0) = vecX.m_X;    rotL2W(0, 1) = vecY.m_X;        rotL2W(0, 2) = vecZ.m_X;
        rotL2W(1, 0) = vecX.m_Y;    rotL2W(1, 1) = vecY.m_Y;        rotL2W(1, 2) = vecZ.m_Y;
        rotL2W(2, 0) = vecX.m_Z;    rotL2W(2, 1) = vecY.m_Z;        rotL2W(2, 2) = vecZ.m_Z;

        vert_i.localTransform.GetRotation() = rotL2W;
        vert_i.localTransform.GetTranslation() = vert_i.position;
        vert_i.globalTransform = vert_i.localTransform; // For vertex 0, local and global transforms are the same.
    }

    // vertex 1 through n-1
    for (int i = 1 ; i < (int)m_VertexArray.size(); i++ )
    {
        HairVertex& vert_i_minus_1 = m_VertexArray[i-1];
        HairVertex& vert_i = m_VertexArray[i];

        CVector3D vec = vert_i.position - vert_i_minus_1.position;
        vec = vert_i_minus_1.globalTransform.GetRotation().InverseOther() * vec;

        CVector3D vecX = vec.NormalizeOther();

        CVector3D X = CVector3D(1.0f, 0, 0);
        CVector3D rotAxis = X.Cross(vecX);
        float angle = acos(X.Dot(vecX));

        if ( abs(angle) < 0.001 || rotAxis.LengthSqr() < 0.001 )
        {
            vert_i.localTransform.GetRotation().SetIdentity();
        }
        else
        {
            rotAxis.Normalize();
            CQuaternion rot = CQuaternion(rotAxis, angle);
            vert_i.localTransform.GetRotation() = rot;
        }

        vert_i.localTransform.GetTranslation() = vec;
        vert_i.globalTransform = vert_i_minus_1.globalTransform * vert_i.localTransform;
        vert_i.referenceVector = vert_i.localTransform.GetTranslation();
    }
}

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

//--------------------------------------------------------------------------------------
//
// GetTangentVectors
//
// Create two arbitrary tangent vectors (t0 and t1) perpendicular to the input normal vector (n).
//
//--------------------------------------------------------------------------------------
void GetTangentVectors(const CVector3D& n, CVector3D& t0, CVector3D& t1)
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


struct OldHairToTriangleMapping
{
    UINT    triangle;                       // index to triangle in the skinned mesh that contains this hair
    FLOAT   barycentricCoord[3];            // barycentric coordinate of the hair within the triangle
};

// Reads in a file of hair data and appends it to the list of hair strands
bool TressFXAssetLoader::LoadAppend(TressFX_HairBlob *pRawHairBlob, const TressFX_GuideFollowParams& guideFollowParams, int groupId)
{
    m_maxRadiusAroundGuideHair = guideFollowParams.radiusForFollowHair;
    m_HairAsset.m_TipSeparationFactor = guideFollowParams.tipSeparationFactor;
    m_HairAsset.m_NumFollowHairsPerGuideHair = guideFollowParams.numFollowHairsPerGuideHair;

    TressFXFileObject tfxFileObj;
    memcpy((void *)&tfxFileObj, pRawHairBlob->pHair, sizeof(TressFXFileObject));

    g_TressFXNumVerticesPerStrand = tfxFileObj.numVerticesPerStrand;

    // TODO: Remove this temporary fix up code when the tfxskin file strand count matches the .tfx file
    if (pRawHairBlob->animationSize != 0)
    {
        TressFXSkinFileObject tfxSkinObj;
        memcpy((void *)&tfxSkinObj, pRawHairBlob->pAnimation, sizeof(TressFXSkinFileObject));
        //tfxFileObj.numGuideHairStrands = tfxSkinObj.numHairs;
    }
    // end fixup

    bool bothEndsImmovable = tfxFileObj.bothEndsImmovable != 0;
    m_HairAsset.m_NumOfVerticesInStrand = tfxFileObj.numVerticesPerStrand;

    unsigned numStrands = tfxFileObj.numGuideHairStrands;
    std::vector<TressFXStrand*> tempHairs;
    char *pHairSrc = (char *)pRawHairBlob->pHair;

    unsigned numVerts = 3 * numStrands * tfxFileObj.numVerticesPerStrand;
    FLOAT *pVerts = (FLOAT *)(pHairSrc + tfxFileObj.verticesOffset);

    FLOAT *pTexCoords = NULL;

    if (tfxFileObj.perStrandTexCoordOffset != 0)
    {
        pTexCoords = (FLOAT *)(pHairSrc + tfxFileObj.perStrandTexCoordOffset);
        m_usingPerStrandTexCoords = true;
    }
    else
        m_usingPerStrandTexCoords = false;

    for ( unsigned strand = 0; strand < numStrands; strand++ )
    {
        // TODO: allocating a small memory per strand is not a good idea.
        TressFXStrand* pHair = new TressFXStrand();
        pHair->m_bGuideHair = true;

        for ( unsigned vertex = 0; vertex < (unsigned)m_HairAsset.m_NumOfVerticesInStrand; vertex++ )
        {
            CVector3D pnt;

            // x
            pnt.m_X = *pVerts++;

            // y
            pnt.m_Y = *pVerts++;

            // z
            pnt.m_Z = *pVerts++;

            HairVertex vert;
            vert.position.Set(pnt.m_X, pnt.m_Y, pnt.m_Z);

            // In some cases, two end vertices in both ends of strand are needed to be immovable.
            if ( bothEndsImmovable )
            {
                if ( vertex == 0 || vertex == 1 || vertex == numVerts -1 || vertex == numVerts - 2 )
                    vert.invMass = 0;
                else
                    vert.invMass = 1.0f;
            }
            else
            {
                if ( vertex == 0 || vertex == 1 )
                    vert.invMass = 0;
                else
                    vert.invMass = 1.0f;
            }

            pHair->m_VertexArray.push_back(vert);
        }

        pHair->m_GroupID = groupId;

        if (m_usingPerStrandTexCoords)
        {
            assert(pTexCoords);
            pHair->m_texCoord.x = *pTexCoords++;
            pHair->m_texCoord.y = *pTexCoords++;
        }
        else
        {
            pHair->m_texCoord.x = 0;
            pHair->m_texCoord.y = 0;
        }

        // add the new hair into tempHairs
        if ( pHair->m_VertexArray.size() > 2 )
        {
            tempHairs.push_back(pHair);
        }
        else
            delete pHair;
    }

    // Make the number of strands a multiple of thread group size.
    // This is an easy way to avoid branching in compute shader.
    unsigned int loadedNumStrands = (unsigned int)tempHairs.size();
    unsigned int numOfDelete = loadedNumStrands % THREAD_GROUP_SIZE;

    for ( unsigned int i = 0; i < numOfDelete; i++ )
    {
        std::vector<TressFXStrand*>::iterator iter = --tempHairs.end();

        if ( (*iter) )
            delete (*iter);

        tempHairs.pop_back();
    }

    m_HairAsset.m_NumGuideHairStrands += (int)tempHairs.size();

    for ( size_t i = 0; i < tempHairs.size(); i++ )
    {
        m_HairStrands.push_back(tempHairs[i]);

        for ( int j = 0; j < m_HairAsset.m_NumFollowHairsPerGuideHair; j++ )
            m_HairStrands.push_back(NULL); // add placeholder for follow hair
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

            int index = i * (m_HairAsset.m_NumFollowHairsPerGuideHair + 1);

            m_HairStrands[index]->m_texCoord.x = u;
            m_HairStrands[index]->m_texCoord.y = v;
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

    if ( m_HairAsset.m_NumFollowHairsPerGuideHair == 0 )
    {
        // TODO: implement LOD support for skinned hair
        // (until then, don't shuffle the strands for skinned hair)
        if (m_HairAsset.m_pMapping == NULL)
        {
            // randomize the hair strands for LOD density
            random_shuffle(m_HairStrands.begin(), m_HairStrands.end());
        }
        return;
    }

    std::vector<TressFXStrand*> tempHairs;
    tempHairs.reserve(m_HairAsset.m_NumGuideHairStrands);

    for ( int i = 0; i < (int)m_HairStrands.size(); i++ )
    {
        if ( m_HairStrands[i] && m_HairStrands[i]->m_bGuideHair )
            tempHairs.push_back(m_HairStrands[i]);
    }

    m_HairStrands.clear();
    m_HairStrands.resize(m_HairAsset.m_NumGuideHairStrands + m_HairAsset.m_NumGuideHairStrands * (m_HairAsset.m_NumFollowHairsPerGuideHair));

    // randomize the hair strands for LOD density
    // make sure hair strands and skin mapping data maintain their relative order after randomization.
    std::srand(0);
    random_shuffle(tempHairs.begin(), tempHairs.end());

    if ( m_HairAsset.m_pMapping )
    {
        std::srand(0);
        random_shuffle(mapping.begin(), mapping.end());

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

    for ( int i = 0; i < (int)tempHairs.size(); i++ )
    {
        m_HairStrands[i*(m_HairAsset.m_NumFollowHairsPerGuideHair +1)] = tempHairs[i];
    }

    // Generate follow hairs from guide hairs
    for ( int i = 0; i < m_HairAsset.m_NumGuideHairStrands; i++ )
    {
        const TressFXStrand* pGuideHair = m_HairStrands[i*(m_HairAsset.m_NumFollowHairsPerGuideHair +1)];

        for ( int j = 0; j < m_HairAsset.m_NumFollowHairsPerGuideHair; j++ )
        {
            TressFXStrand* pNewFollowHair = new TressFXStrand();
            pNewFollowHair->m_bGuideHair = false;
            pNewFollowHair->m_GroupID = pGuideHair->m_GroupID;
            CVector3D v01 = pGuideHair->m_VertexArray[1].position - pGuideHair->m_VertexArray[0].position;
            v01.Normalize();

            // Find two orthogonal unit tangent vectors to v01
            CVector3D t0, t1;
            GetTangentVectors(v01, t0, t1);
            CVector3D offset = GetRandom(-m_maxRadiusAroundGuideHair, m_maxRadiusAroundGuideHair)*t0 + GetRandom(-m_maxRadiusAroundGuideHair, m_maxRadiusAroundGuideHair)*t1;

            for ( int k = 0; k < (int)pGuideHair->m_VertexArray.size(); k++ )
            {
                HairVertex vert;
                float factor = m_HairAsset.m_TipSeparationFactor*((float)k / (float)pGuideHair->m_VertexArray.size()) + 1.0f;
                vert.position =  pGuideHair->m_VertexArray[k].position + offset*factor;
                vert.invMass = pGuideHair->m_VertexArray[k].invMass;
                pNewFollowHair->m_VertexArray.push_back(vert);
            }

            // copy texture coords from the guide hair
            // TODO: recompute texture coords by considering offset
            pNewFollowHair->m_texCoord.x = pGuideHair->m_texCoord.x;
            pNewFollowHair->m_texCoord.y = pGuideHair->m_texCoord.y;

            m_HairStrands[i*(m_HairAsset.m_NumFollowHairsPerGuideHair +1)+j+1] = pNewFollowHair;
        }
    }
}

// Clear the array of hair strands
void TressFXAssetLoader::Clear()
{
    for ( int i = 0; i < (int)m_HairStrands.size(); i++ )
    {
        if ( m_HairStrands[i] )
            delete m_HairStrands[i];
    }

    m_HairStrands.clear();

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
void TressFXAssetLoader::ComputeStrandTangent(std::vector<HairVertex>& vertices)
{
    int numVertices = (int)vertices.size();

    // Calculate the tangent value for the first vertex of the strand
    CVector3D tangent = vertices[1].position - vertices[0].position;
    tangent.Normalize();
    vertices[0].tangent = tangent;

    // from 1 to n-1
    for (int vertex = 1; vertex < numVertices-1; vertex++)
    {
        CVector3D tangent_pre = vertices[vertex].position - vertices[vertex-1].position;
        tangent_pre.Normalize();

        CVector3D tangent_next = vertices[vertex+1].position - vertices[vertex].position;
        tangent_next.Normalize();

        tangent = tangent_pre + tangent_next;
        tangent = tangent.Normalize();

        vertices[vertex].tangent = tangent;
    }

    // For the last vertex of the strand
    tangent = vertices[numVertices-1].position - vertices[numVertices-2].position;
    tangent = tangent.Normalize();
    vertices[numVertices-1].tangent = tangent;
}

void TressFXAssetLoader::ComputeDistanceToRoot(std::vector<HairVertex>& vertices)
{
    int numVertices = (int)vertices.size();
    float strandLength = 0;
    vertices[0].texcoord.m_Z = 0;

    for (int i=1; i<numVertices; i++)
    {
        CVector3D vec = vertices[i].position - vertices[i-1].position;
        float disSeg = vec.Length();

        vertices[i].texcoord.m_Z = vertices[i-1].texcoord.m_Z + disSeg;
        strandLength += disSeg;
    }

    for (int i=0; i<numVertices; i++)
    {
        vertices[i].texcoord.m_Z /= strandLength;
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
    m_HairAsset.m_NumTotalHairVertices = 0;
    m_HairAsset.m_NumGuideHairVertices = 0;

    // count vertices
    for ( int i = 0; i < (int)m_HairStrands.size(); i++ )
    {
        m_HairAsset.m_NumTotalHairVertices += (int)m_HairStrands[i]->m_VertexArray.size();

        if ( m_HairStrands[i]->m_bGuideHair )
            m_HairAsset.m_NumGuideHairVertices += (int)m_HairStrands[i]->m_VertexArray.size();
    }

    // construct reference vectors and reference frames
    for ( int i = 0; i < (int)m_HairStrands.size(); i++ )
    {
        m_HairStrands[i]->ConstructAsset();
    }

    m_HairAsset.m_NumTotalHairStrands = (int)m_HairStrands.size();

    m_HairAsset.m_pVertices = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    m_HairAsset.m_pHairStrandType = new int[m_HairAsset.m_NumTotalHairStrands];
    if (m_usingPerStrandTexCoords)
        m_HairAsset.m_pStrandTexCoords = new XMFLOAT2[m_HairAsset.m_NumTotalHairStrands];
    else
        m_HairAsset.m_pStrandTexCoords = NULL;
    m_HairAsset.m_pTangents = new XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    int indexTang = 0;

    // Initialize the hair strands and compute tangents
    for ( int i=0; i < m_HairAsset.m_NumTotalHairStrands; i++ )
    {
        TressFXStrand* strand = m_HairStrands[i];
        std::vector<HairVertex>& vertice = strand->m_VertexArray;

        int numVerts = int(vertice.size());
        ComputeStrandTangent(vertice);
        ComputeDistanceToRoot(vertice);

        for( int v=0; v < numVerts; v++ )
        {
            m_HairAsset.m_pTangents[indexTang].x = vertice[v].tangent.m_X;
            m_HairAsset.m_pTangents[indexTang].y = vertice[v].tangent.m_Y;
            m_HairAsset.m_pTangents[indexTang].z = vertice[v].tangent.m_Z;
            indexTang++;
        }
    }

    m_HairAsset.m_pRestLengths = new float[m_HairAsset.m_NumTotalHairVertices];
    int index = 0;

    // Calculate rest lengths
    for ( int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++ )
    {
        TressFXStrand* strand = m_HairStrands[i];
        std::vector<HairVertex>& vertices = strand->m_VertexArray;

        for ( int j = 0; j < (int)vertices.size()-1; j++ )
            m_HairAsset.m_pRestLengths[index++] = (vertices[j].position - vertices[j+1].position).Length();

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

    // initialize the remainder of the hair data
    for ( int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++ )
    {
        TressFXStrand* strand = m_HairStrands[i];
        std::vector<HairVertex>& vertices = strand->m_VertexArray;

        int vertCount = (int)vertices.size();

        for ( int j = 0; j < vertCount - 1; j++ )
        {
            // line indices
            m_HairAsset.m_LineIndices.push_back(id);
            m_HairAsset.m_LineIndices.push_back(id+1);

            // triangle indices
            m_HairAsset.m_TriangleIndices.push_back(2*id);
            m_HairAsset.m_TriangleIndices.push_back(2*id+1);
            m_HairAsset.m_TriangleIndices.push_back(2*id+2);
            m_HairAsset.m_TriangleIndices.push_back(2*id+2);
            m_HairAsset.m_TriangleIndices.push_back(2*id+1);
            m_HairAsset.m_TriangleIndices.push_back(2*id+3);
            id++;
        }

        id++;

        for ( int j = 0; j < vertCount; j++ )
        {
            // triangle vertices
            StrandVertex strandVertex;
            strandVertex.position = XMFLOAT3(vertices[j].position.m_X, vertices[j].position.m_Y, vertices[j].position.m_Z);
            strandVertex.tangent = XMFLOAT3(vertices[j].tangent.m_X, vertices[j].tangent.m_Y, vertices[j].tangent.m_Z);
            strandVertex.texcoord = XMFLOAT4(vertices[j].texcoord.m_X, vertices[j].texcoord.m_Y, vertices[j].texcoord.m_Z, 0);
            m_HairAsset.m_pTriangleVertices[index] = strandVertex;

            float tVal = m_HairAsset.m_pTriangleVertices[index].texcoord.z;
            m_HairAsset.m_pThicknessCoeffs[index] = sqrt(1.f - tVal * tVal);

            XMFLOAT4 v;

            // temp vertices
            v.x = vertices[j].position.m_X;
            v.y = vertices[j].position.m_Y;
            v.z = vertices[j].position.m_Z;
            v.w = vertices[j].invMass;
            m_HairAsset.m_pVertices[index] = v;

            // global rotations
            v.x = vertices[j].globalTransform.GetRotation().m_X;
            v.y = vertices[j].globalTransform.GetRotation().m_Y;
            v.z = vertices[j].globalTransform.GetRotation().m_Z;
            v.w = vertices[j].globalTransform.GetRotation().m_W;
            m_HairAsset.m_pGlobalRotations[index] = v;

            // local rotations
            v.x = vertices[j].localTransform.GetRotation().m_X;
            v.y = vertices[j].localTransform.GetRotation().m_Y;
            v.z = vertices[j].localTransform.GetRotation().m_Z;
            v.w = vertices[j].localTransform.GetRotation().m_W;
            m_HairAsset.m_pLocalRotations[index] = v;

                // ref vectors
            v.x = vertices[j].referenceVector.m_X;
            v.y = vertices[j].referenceVector.m_Y;
            v.z = vertices[j].referenceVector.m_Z;
            m_HairAsset.m_pRefVectors[index].x = v.x;
            m_HairAsset.m_pRefVectors[index].y = v.y;
            m_HairAsset.m_pRefVectors[index].z = v.z;

            index++;
        }

        int groupId = m_HairStrands[i]->m_GroupID;
        m_HairAsset.m_pHairStrandType[i] = groupId;

        if (m_usingPerStrandTexCoords)
            m_HairAsset.m_pStrandTexCoords[i] = m_HairStrands[i]->m_texCoord;

        if ( m_HairStrands[i]->m_bGuideHair )
        {
            indexGuideHairStrand = i;
            pGuideHair = m_HairStrands[i];
            m_HairAsset.m_pFollowRootOffset[i] = XMFLOAT4(0, 0, 0, (float)indexGuideHairStrand); // forth component is an index to the guide hair strand. For guide hair, it points itself.
        }
        else
        {
            assert(pGuideHair);
            CVector3D offset = m_HairStrands[i]->m_VertexArray[0].position - pGuideHair->m_VertexArray[0].position;
            m_HairAsset.m_pFollowRootOffset[i] = XMFLOAT4(offset.m_X, offset.m_Y, offset.m_Z, (float)indexGuideHairStrand); // forth component is an index to the guide hair strand.
        }
    }

    // Find the bounding sphere
    BBox bBox;

    for(int i=0; i < int(m_HairStrands.size()); i++)
    {
        std::vector<HairVertex>& vertices = m_HairStrands[i]->m_VertexArray;

        for ( int j = 0; j < (int)vertices.size(); j++ )
        {
            HairVertex& vertex = vertices[j];
            bBox = Union(bBox, Float3(vertex.position.m_X, vertex.position.m_Y, vertex.position.m_Z));
        }
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
        m_HairAsset.m_NumPerStrandTexCoords = m_HairAsset.m_NumTotalHairStrands;
    else
         m_HairAsset.m_NumPerStrandTexCoords = 0;

    if (m_usingPerVertexColors)
        m_HairAsset.m_NumPerVertexColors = m_HairAsset.m_NumTotalHairVertices;
    else
        m_HairAsset.m_NumPerVertexColors = 0;

    if (m_usingPerVertexTexCoords)
        m_HairAsset.m_NumPerVertexTexCoords = m_HairAsset.m_NumTotalHairVertices;
    else
        m_HairAsset.m_NumPerVertexTexCoords = 0;

    if (m_HairAsset.m_pMapping != NULL)
        m_HairAsset.m_NumHairToTriangleMappings = m_HairAsset.m_NumTotalHairStrands;
    else
        m_HairAsset.m_NumHairToTriangleMappings = 0;

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
        buffer.sputn((char *)m_HairAsset.m_pStrandTexCoords, m_HairAsset.m_NumTotalHairStrands * sizeof(DirectX::XMFLOAT2));

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

