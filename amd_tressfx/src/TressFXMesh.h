//--------------------------------------------------------------------------------------
// File: TressFXMesh.h
//
// Hair mesh code
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

#pragma once
#include "TressFXAsset.h"
#include "Util.h"

namespace AMD
{

class TressFXMesh
{
private:
    // private member function
    HRESULT CreateBufferAndViews(ID3D11Device* pd3dDevice, TressFX_SceneMesh *sceneMesh);

public:
    ID3D11Buffer*               m_pIndexBuffer;
    ID3D11Buffer*               m_pTriangleIndexBuffer;
    ID3D11ShaderResourceView*   m_pTriangleIndexBufferSRV;
    ID3D11Buffer*               m_pThicknessCoeffsBuffer;
    ID3D11ShaderResourceView*   m_pThicknessCoeffsSRV;
    int                         m_TotalIndexCount;
    int                         m_TotalTriangleIndexCount;
    TressFXAsset                m_HairAsset;

    // vertex positions
    ID3D11Buffer* m_HairVertexPositionsUAB;
    ID3D11UnorderedAccessView* m_HairVertexPositionsUAV;
    ID3D11ShaderResourceView* m_HairVertexPositionsSRV;

    ID3D11Buffer* m_HairVertexPositionsPrevUAB;
    ID3D11UnorderedAccessView* m_HairVertexPositionsPrevUAV;

    ID3D11Buffer* m_InitialHairPositionsBuffer;
    ID3D11ShaderResourceView* m_InitialHairPositionsSRV;
    ID3D11UnorderedAccessView* m_InitialHairPositionsUAV;

    // strand type
    ID3D11Buffer* m_HairStrandTypeBuffer;
    ID3D11ShaderResourceView* m_HairStrandTypeSRV;

    // rest length
    ID3D11Buffer* m_HairLengthBuffer;
    ID3D11ShaderResourceView* m_HairRestLengthSRV;

    // reference vectors
    ID3D11Buffer* m_HairRefVectorsBuffer;
    ID3D11ShaderResourceView* m_HairRefVecsInLocalFrameSRV;

    // follow hair root offset
    ID3D11Buffer* m_FollowHairRootOffsetBuffer;
    ID3D11ShaderResourceView* m_FollowHairRootOffsetSRV;

    // global and local transforms for each vertex
    ID3D11Buffer* m_GlobalRotationsUAB;
    ID3D11UnorderedAccessView* m_GlobalRotationsUAV;

    ID3D11Buffer* m_LocalRotationsUAB;
    ID3D11UnorderedAccessView* m_LocalRotationsUAV;

    // vertex tangents
    ID3D11Buffer* m_HairVertexTangentsUAB;
    ID3D11UnorderedAccessView* m_HairVertexTangentsUAV;
    ID3D11ShaderResourceView* m_HairVertexTangentsSRV;

    // mapping from hair to skinned mesh
    ID3D11Buffer* m_HairSkinMappingBuffer;
    ID3D11ShaderResourceView* m_HairSkinMappingSRV;

    // per-strand transformations for skinning
    ID3D11Buffer* m_HairTransformsBuffer;
    ID3D11UnorderedAccessView* m_HairTransformsUAV;
    ID3D11ShaderResourceView* m_HairTransformsSRV;

    // per-strand texture coordinates
    ID3D11Buffer* m_pStrandTexCoordBuffer;
    ID3D11ShaderResourceView* m_pStrandTexCoordSRV;

    // hair texture
    ID3D11ShaderResourceView* m_pHairTextureSRV;

    // public member functions
    TressFXMesh(void);
    ~TressFXMesh(void);

    HRESULT OnCreate(ID3D11Device* pd3dDevice, TressFX_HairBlob *pHairBlob, TressFX_SceneMesh *sceneMesh, ID3D11ShaderResourceView *pTexture);
    void OnDestroy();

    void DestroyAsset();

    bool Deserialize(TressFX_HairBlob *pHairBlob);

};

} // namespace AMD
