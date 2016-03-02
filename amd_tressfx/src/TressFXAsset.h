//--------------------------------------------------------------------------------------
// File: TresssFXAsset.h
//
// Hair asset structure
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
#ifndef AMD_TRESSFX_ASSET_H
#define AMD_TRESSFX_ASSET_H

#include <DirectXMath.h>
#include "AMD_TressFX.h"
#include "TressFXSkinFormat.h"
#include "Math\\Transform.h"
#include "Util.h"
#include <vector>

#define THREAD_GROUP_SIZE 64        // This needs to match THREAD_GROUP_SIZE in TressFXSimulation.hlsl

namespace AMD
{

struct TressFXAsset
{
    int*                   m_pHairStrandType;
    DirectX::XMFLOAT4*     m_pRefVectors;
    DirectX::XMFLOAT4*     m_pGlobalRotations;
    DirectX::XMFLOAT4*     m_pLocalRotations;
    DirectX::XMFLOAT4*     m_pVertices;
    DirectX::XMFLOAT4*     m_pTangents;
    DirectX::XMFLOAT2*     m_pStrandTexCoords;
    DirectX::XMFLOAT2*     m_pVertexTexCoords;
    DirectX::XMFLOAT4*     m_pVertexColors;
    DirectX::XMFLOAT4*     m_pFollowRootOffset;
    HairToTriangleMapping* m_pMapping;
    StrandVertex*          m_pTriangleVertices;
    float*                 m_pThicknessCoeffs;
    float*                 m_pRestLengths;
    std::vector<int>       m_LineIndices;
    std::vector<int>       m_TriangleIndices;
    BSphere                m_bSphere;
    int                    m_NumTotalHairStrands;
    int                    m_NumTotalHairVertices;
    int                    m_NumOfVerticesInStrand;
    int                    m_NumGuideHairStrands;
    int                    m_NumGuideHairVertices;
    int                    m_NumFollowHairsPerGuideHair;
    int                    m_NumPerStrandTexCoords;
    int                    m_NumPerVertexColors;
    int                    m_NumPerVertexTexCoords;
    int                    m_NumHairToTriangleMappings;
    float                  m_TipSeparationFactor;
};

struct TressFXHairVertex
{
    tressfx_vec3 position;
    float invMass; // In case mass is infinite, inverse mass is zero

    tressfx_transform globalTransform;
    tressfx_transform localTransform;

    tressfx_vec3 referenceVector;
    tressfx_vec3 tangent;
    tressfx_vec3 texcoord;
};

struct TressFXStrand
{
    bool m_bGuideHair;
    int m_GroupID;
    DirectX::XMFLOAT2 m_texCoord;
};

class TressFXAssetLoader
{
public:
    TressFXAssetLoader(void);
    ~TressFXAssetLoader(void);

    TressFXAsset m_HairAsset;

private:
    std::vector<TressFXStrand> m_HairStrands;
    std::vector<TressFXHairVertex> m_Vertices;
    std::vector<tressfx_vec3> m_SlaveOffset;

    float m_maxRadiusAroundGuideHair;
    bool m_usingPerStrandTexCoords;
    bool m_usingPerVertexColors;
    bool m_usingPerVertexTexCoords;

    std::vector<HairToTriangleMapping> mapping;

    // prepare internal data
    void ConstructTransforms();
    void ComputeDistanceToRoot();
    void ComputeStrandTangent();

    void DestroyAll();

public:
    void Clear();
    bool LoadAppend(TressFX_HairBlob *pRawHairBlob, const TressFX_GuideFollowParams& guideFollowParams, int groupId);
    void ProcessVertices();
    void GenerateFollowHairs();
    bool Serialize(TressFX_HairBlob *pTressFXHairBlob);
    void ProcessVertices(float scale, DirectX::XMFLOAT3 rotation, DirectX::XMFLOAT3 translation);
};

} // namespace AMD

#endif
