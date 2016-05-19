//--------------------------------------------------------------------------------------
// File: TressFXMesh.cpp
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

#include "AMD_Types.h"
#include "TressFXMesh.h"

#include <string>
#include <sstream>

#ifndef AMD_V_RETURN
#define AMD_V_RETURN(x)    { hr = (x); if ( FAILED(hr) ) { return hr; } }
#endif

using namespace std;
using namespace DirectX;

namespace AMD
{

//--------------------------------------------------------------------------------------
//
// Constructor
//
// Used for initializing member variables to default values.
//--------------------------------------------------------------------------------------
TressFXMesh::TressFXMesh(void) :
    m_pIndexBuffer(NULL)
  , m_pTriangleIndexBuffer(NULL)
  , m_pThicknessCoeffsBuffer(NULL)
  , m_pTriangleIndexBufferSRV(NULL)
  , m_pThicknessCoeffsSRV(NULL)
{
    m_HairVertexPositionsUAB = NULL;
    m_HairVertexPositionsSRV = NULL;
    m_HairVertexPositionsPrevUAB = NULL;

    m_HairStrandTypeBuffer = NULL;
    m_HairStrandTypeSRV = NULL;

    m_HairVertexPositionsUAV = NULL;
    m_HairVertexPositionsPrevUAV = NULL;

    m_HairLengthBuffer = NULL;
    m_HairRestLengthSRV = NULL;

    m_InitialHairPositionsBuffer = NULL;
    m_InitialHairPositionsSRV = NULL;
    m_InitialHairPositionsUAV = NULL;

    m_HairVertexTangentsUAB = NULL;
    m_HairVertexTangentsSRV = NULL;
    m_HairVertexTangentsUAV = NULL;

    m_HairRefVectorsBuffer = NULL;
    m_HairRefVecsInLocalFrameSRV = NULL;

    m_FollowHairRootOffsetBuffer = NULL;
    m_FollowHairRootOffsetSRV = NULL;

    m_GlobalRotationsUAB = NULL;
    m_GlobalRotationsUAV = NULL;

    m_LocalRotationsUAB = NULL;
    m_LocalRotationsUAV = NULL;

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
    m_HairAsset.m_NumOfVerticesInStrand = 2;

    m_HairSkinMappingBuffer = NULL;
    m_HairTransformsBuffer = NULL;
    m_HairSkinMappingSRV = NULL;
    m_HairTransformsUAV = NULL;
    m_HairTransformsSRV = NULL;
    m_pStrandTexCoordBuffer = NULL;
    m_pStrandTexCoordSRV = NULL;
}

//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
TressFXMesh::~TressFXMesh(void)
{
    OnDestroy();
}


//--------------------------------------------------------------------------------------
//
// OnCreate
//
//  Called to create a hair mesh. This function creates the D3D resources
//  and stores the hair data into these resources.
//
//--------------------------------------------------------------------------------------
HRESULT TressFXMesh::OnCreate(ID3D11Device* pd3dDevice, TressFX_HairBlob *pHairBlob, TressFX_SceneMesh *sceneMesh, ID3D11ShaderResourceView *pTexture)
{
    HRESULT hr;

    // load the binary file
    if (!Deserialize(pHairBlob))
    {
        return E_FAIL;
    }

    //thickness coeff buffer
    {
        D3D11_BUFFER_DESC bd;
        ZeroMemory(&bd, sizeof(bd));

        bd.Usage     = D3D11_USAGE_IMMUTABLE;
        bd.ByteWidth = sizeof(float) * m_HairAsset.m_NumTotalHairVertices;
        bd.BindFlags = D3D11_BIND_SHADER_RESOURCE;
        bd.StructureByteStride = sizeof(float);

        D3D11_SUBRESOURCE_DATA srData;
        ZeroMemory(&srData, sizeof(srData));
        srData.pSysMem = m_HairAsset.m_pThicknessCoeffs;

        AMD_V_RETURN( pd3dDevice->CreateBuffer( &bd, &srData, &m_pThicknessCoeffsBuffer) );
    }

    //thickness coeff buffer srv
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        ZeroMemory(&SRVDesc, sizeof(SRVDesc));

        SRVDesc.Format                = DXGI_FORMAT_R32_FLOAT;
        SRVDesc.ViewDimension         = D3D11_SRV_DIMENSION_BUFFER;
        SRVDesc.Buffer.FirstElement  = 0;
        SRVDesc.Buffer.NumElements   = m_HairAsset.m_NumTotalHairVertices;

        AMD_V_RETURN( pd3dDevice->CreateShaderResourceView(m_pThicknessCoeffsBuffer, &SRVDesc, &m_pThicknessCoeffsSRV) );
    }


    //-----------------------------------
    // Index buffer (lines and triangles)
    //-----------------------------------

    // Line index buffer
    m_TotalIndexCount = (int)m_HairAsset.m_LineIndices.size();

    D3D11_BUFFER_DESC bd;
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)(sizeof( unsigned int ) * m_HairAsset.m_LineIndices.size());
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA InitData;
    InitData.pSysMem = &(m_HairAsset.m_LineIndices[0]);
    hr = pd3dDevice->CreateBuffer( &bd, &InitData, &m_pIndexBuffer );

    if ( FAILED(hr) )
    {
        return hr;
    }

    // Triangle index buffer
    m_TotalTriangleIndexCount = (int)m_HairAsset.m_TriangleIndices.size();
    bd.Usage = D3D11_USAGE_DEFAULT;
    bd.ByteWidth = (UINT)(sizeof( unsigned int ) * m_HairAsset.m_TriangleIndices.size());
    bd.BindFlags = D3D11_BIND_INDEX_BUFFER | D3D11_BIND_SHADER_RESOURCE;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags = 0;

    InitData.pSysMem = &(m_HairAsset.m_TriangleIndices[0]);
    hr = pd3dDevice->CreateBuffer( &bd, &InitData, &m_pTriangleIndexBuffer );

    if ( FAILED(hr) )
    {
        return hr;
    }

    // triangle index buffer srv
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc;
        sbSRVDesc.Buffer.FirstElement           = 0;
        sbSRVDesc.Buffer.NumElements            = (UINT)m_HairAsset.m_TriangleIndices.size();
        sbSRVDesc.Format                        = DXGI_FORMAT_R32_UINT;
        sbSRVDesc.ViewDimension                 = D3D11_SRV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_pTriangleIndexBufferSRV);
        AMD_V_RETURN( pd3dDevice->CreateShaderResourceView(m_pTriangleIndexBuffer, &sbSRVDesc, &m_pTriangleIndexBufferSRV) );
    }

    m_pHairTextureSRV = pTexture;

    hr = CreateBufferAndViews(pd3dDevice, sceneMesh);

    DestroyAsset();

    return hr;
}


//--------------------------------------------------------------------------------------
//
// CreateBufferAndViews
//
// Called by the OnCreate function to create the D3D resources
//
//--------------------------------------------------------------------------------------
HRESULT TressFXMesh::CreateBufferAndViews(ID3D11Device* pd3dDevice, TressFX_SceneMesh *sceneMesh)
{
    HRESULT hr;

    //--------------------------------
    // m_HairStrandTypeSRV
    //--------------------------------
    {
        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pHairStrandType;

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;
        bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairStrands * sizeof(int);
        bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;

        AMD_SAFE_RELEASE(m_HairStrandTypeBuffer);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_HairStrandTypeBuffer));

        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        SRVDesc.Format = DXGI_FORMAT_R32_SINT;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        SRVDesc.Buffer.FirstElement = 0;
        SRVDesc.Buffer.NumElements = m_HairAsset.m_NumTotalHairStrands;
        AMD_SAFE_RELEASE(m_HairStrandTypeSRV);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_HairStrandTypeBuffer, &SRVDesc, &m_HairStrandTypeSRV));
    }

    //---------------------------------
    // m_InitialHairPositionsSRV
    // m_InitialHairPositionsUAV
    //---------------------------------
    {
        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pVertices;

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bufferDesc.Usage = D3D11_USAGE_DEFAULT; //D3D11_USAGE_IMMUTABLE;
        bufferDesc.StructureByteStride  = sizeof(XMFLOAT4);
        bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;

        AMD_SAFE_RELEASE(m_InitialHairPositionsBuffer);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_InitialHairPositionsBuffer));

        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = m_HairAsset.m_NumTotalHairVertices;
        desc.Format = DXGI_FORMAT_UNKNOWN;
        desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_InitialHairPositionsSRV);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_InitialHairPositionsBuffer, &desc, &m_InitialHairPositionsSRV));

        D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
        sbUAVDesc.Buffer.FirstElement       = 0;
        sbUAVDesc.Buffer.Flags              = 0;
        sbUAVDesc.Buffer.NumElements        = m_HairAsset.m_NumTotalHairVertices;
        sbUAVDesc.Format                    = DXGI_FORMAT_UNKNOWN;
        sbUAVDesc.ViewDimension             = D3D11_UAV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_InitialHairPositionsUAV);
        AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_InitialHairPositionsBuffer, &sbUAVDesc, &m_InitialHairPositionsUAV) );
    }

    // ------------------------
    // m_HairVertexPositionsUAB
    //-------------------------
    {
        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pVertices;

        D3D11_BUFFER_DESC bufferDescUA;
        bufferDescUA.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.Usage = D3D11_USAGE_DEFAULT;
        bufferDescUA.BindFlags =  D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        bufferDescUA.CPUAccessFlags = 0;
        bufferDescUA.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bufferDescUA.StructureByteStride  = sizeof(XMFLOAT4);
        AMD_SAFE_RELEASE(m_HairVertexPositionsUAB);
        AMD_SAFE_RELEASE(m_HairVertexPositionsPrevUAB);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDescUA, &initialData, &m_HairVertexPositionsUAB));
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDescUA, &initialData, &m_HairVertexPositionsPrevUAB));
    }

    // -----------------------------
    // m_HairVertexTangentsUAB
    //------------------------------
    {
        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pTangents;

        D3D11_BUFFER_DESC bufferDescUA;
        bufferDescUA.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.Usage = D3D11_USAGE_DEFAULT;
        bufferDescUA.BindFlags =  D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        bufferDescUA.CPUAccessFlags = 0;
        bufferDescUA.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bufferDescUA.StructureByteStride  = sizeof(XMFLOAT4);
        AMD_SAFE_RELEASE(m_HairVertexTangentsUAB);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDescUA, &initialData, &m_HairVertexTangentsUAB));
    }

    //-----------------------
    // m_HairVertexPositionsSRV
    //-----------------------
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc;
        sbSRVDesc.Buffer.FirstElement = 0;
        sbSRVDesc.Buffer.NumElements = m_HairAsset.m_NumTotalHairVertices;
        sbSRVDesc.Format = DXGI_FORMAT_UNKNOWN; //DXGI_FORMAT_R32G32B32A32_FLOAT
        sbSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_HairVertexPositionsSRV);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_HairVertexPositionsUAB, &sbSRVDesc, &m_HairVertexPositionsSRV));
    }

    //-----------------------
    // m_HairVertexTangentsSRV
    //-----------------------
    {
        D3D11_SHADER_RESOURCE_VIEW_DESC sbSRVDesc;
        sbSRVDesc.Buffer.FirstElement           = 0;
        sbSRVDesc.Buffer.NumElements            = m_HairAsset.m_NumTotalHairVertices;
        sbSRVDesc.Format                        = DXGI_FORMAT_UNKNOWN; //DXGI_FORMAT_R32G32B32A32_FLOAT
        sbSRVDesc.ViewDimension                 = D3D11_SRV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_HairVertexTangentsSRV);
        AMD_V_RETURN( pd3dDevice->CreateShaderResourceView(m_HairVertexTangentsUAB, &sbSRVDesc, &m_HairVertexTangentsSRV) );
    }

    //-----------------------
    // m_HairVertexPositionsUAV
    //-----------------------
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
        sbUAVDesc.Buffer.FirstElement       = 0;
        sbUAVDesc.Buffer.Flags              = 0;
        sbUAVDesc.Buffer.NumElements        = m_HairAsset.m_NumTotalHairVertices;
        sbUAVDesc.Format                    = DXGI_FORMAT_UNKNOWN; //DXGI_FORMAT_R32G32B32A32_FLOAT
        sbUAVDesc.ViewDimension             = D3D11_UAV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_HairVertexPositionsUAV);
        AMD_SAFE_RELEASE(m_HairVertexPositionsPrevUAV);
        AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_HairVertexPositionsUAB, &sbUAVDesc, &m_HairVertexPositionsUAV) );
        AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_HairVertexPositionsPrevUAB, &sbUAVDesc, &m_HairVertexPositionsPrevUAV) );
    }

    //------------------------------
    // m_HairVertexTangentsUAV
    //------------------------------
    {
        D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
        sbUAVDesc.Buffer.FirstElement       = 0;
        sbUAVDesc.Buffer.Flags              = 0;
        sbUAVDesc.Buffer.NumElements        = m_HairAsset.m_NumTotalHairVertices;
        sbUAVDesc.Format                    = DXGI_FORMAT_UNKNOWN;
        sbUAVDesc.ViewDimension             = D3D11_UAV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_HairVertexTangentsUAV);
        AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_HairVertexTangentsUAB, &sbUAVDesc, &m_HairVertexTangentsUAV) );
    }

    //---------------------------
    // m_HairRestLengthSRV
    //---------------------------
    {
        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(float);
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags =  D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pRestLengths;

        AMD_SAFE_RELEASE(m_HairLengthBuffer);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_HairLengthBuffer));

        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        desc.Format = DXGI_FORMAT_R32_FLOAT;
        desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = m_HairAsset.m_NumTotalHairVertices;
        AMD_SAFE_RELEASE(m_HairRestLengthSRV);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_HairLengthBuffer, &desc, &m_HairRestLengthSRV));
    }

    //-----------------------------------
    // m_HairRefVecsInLocalFrameSRV
    //-----------------------------------
    {

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags =  D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pRefVectors;

        AMD_SAFE_RELEASE(m_HairRefVectorsBuffer);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_HairRefVectorsBuffer));

        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = m_HairAsset.m_NumTotalHairVertices;
        AMD_SAFE_RELEASE(m_HairRefVecsInLocalFrameSRV);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_HairRefVectorsBuffer, &desc, &m_HairRefVecsInLocalFrameSRV));
    }

    //-----------------------------------
    // m_FollowHairRootOffsetSRV
    //-----------------------------------
    {

        D3D11_BUFFER_DESC bufferDesc;
        bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairStrands * sizeof(XMFLOAT4);
        bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
        bufferDesc.BindFlags =  D3D11_BIND_SHADER_RESOURCE;
        bufferDesc.CPUAccessFlags = 0;
        bufferDesc.MiscFlags = 0;

        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pFollowRootOffset;

        AMD_SAFE_RELEASE(m_FollowHairRootOffsetBuffer);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_FollowHairRootOffsetBuffer));

        D3D11_SHADER_RESOURCE_VIEW_DESC desc;
        desc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
        desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        desc.Buffer.FirstElement = 0;
        desc.Buffer.NumElements = m_HairAsset.m_NumTotalHairStrands;
        AMD_SAFE_RELEASE(m_FollowHairRootOffsetSRV);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_FollowHairRootOffsetBuffer, &desc, &m_FollowHairRootOffsetSRV));
    }


    // --------------------------------------------
    // m_GlobalRotationsUAB & m_GlobalRotationsUAV
    //---------------------------------------------
    {

        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pGlobalRotations;

        D3D11_BUFFER_DESC bufferDescUA;
        bufferDescUA.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.Usage = D3D11_USAGE_DEFAULT;
        bufferDescUA.BindFlags =  D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        bufferDescUA.CPUAccessFlags = 0;
        bufferDescUA.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bufferDescUA.StructureByteStride  = sizeof(XMFLOAT4);
        AMD_SAFE_RELEASE(m_GlobalRotationsUAB);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDescUA, &initialData, &m_GlobalRotationsUAB));

        D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
        sbUAVDesc.Buffer.FirstElement       = 0;
        sbUAVDesc.Buffer.Flags              = 0;
        sbUAVDesc.Buffer.NumElements        = m_HairAsset.m_NumTotalHairVertices;
        sbUAVDesc.Format                    = DXGI_FORMAT_UNKNOWN;
        sbUAVDesc.ViewDimension             = D3D11_UAV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_GlobalRotationsUAV);
        AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_GlobalRotationsUAB, &sbUAVDesc, &m_GlobalRotationsUAV) );

    }

    // --------------------------------------------
    // m_LocalRotationsUAB & m_LocalRotationsUAV
    //---------------------------------------------
    {
        D3D11_SUBRESOURCE_DATA initialData;
        initialData.pSysMem = m_HairAsset.m_pLocalRotations;

        D3D11_BUFFER_DESC bufferDescUA;
        bufferDescUA.ByteWidth = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.Usage = D3D11_USAGE_DEFAULT;
        bufferDescUA.BindFlags =  D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        bufferDescUA.CPUAccessFlags = 0;
        bufferDescUA.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        bufferDescUA.StructureByteStride  = sizeof(XMFLOAT4);
        AMD_SAFE_RELEASE(m_LocalRotationsUAB);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDescUA, &initialData, &m_LocalRotationsUAB));

        D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
        sbUAVDesc.Buffer.FirstElement       = 0;
        sbUAVDesc.Buffer.Flags              = 0;
        sbUAVDesc.Buffer.NumElements        = m_HairAsset.m_NumTotalHairVertices;
        sbUAVDesc.Format                    = DXGI_FORMAT_UNKNOWN;
        sbUAVDesc.ViewDimension             = D3D11_UAV_DIMENSION_BUFFER;
        AMD_SAFE_RELEASE(m_LocalRotationsUAV);
        AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_LocalRotationsUAB, &sbUAVDesc, &m_LocalRotationsUAV) );
    }

    if (m_HairAsset.m_pMapping != NULL)
    {
        // -----------------------------------------------
        // m_HairSkinMappingBuffer & m_HairSkinMappingSRV
        //------------------------------------------------
        {
            D3D11_SUBRESOURCE_DATA initialData;

            // "flatten" the mapping for one big vertex array
            for (int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++)
            {
                m_HairAsset.m_pMapping[i].triangle += sceneMesh->meshOffsets[m_HairAsset.m_pMapping[i].mesh];
            }

            initialData.pSysMem = m_HairAsset.m_pMapping;
            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairStrands * sizeof(HairToTriangleMapping);
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            bufferDesc.StructureByteStride  = sizeof(HairToTriangleMapping);
            AMD_SAFE_RELEASE(m_HairSkinMappingBuffer);
            AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_HairSkinMappingBuffer));

            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            desc.Buffer.ElementOffset = 0;
            desc.Buffer.ElementWidth = m_HairAsset.m_NumTotalHairStrands;
            AMD_SAFE_RELEASE(m_HairSkinMappingSRV);
            AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_HairSkinMappingBuffer, &desc, &m_HairSkinMappingSRV));
        }

        // --------------------------------------------
        // m_HairTransformsBuffer & m_HairTransformsUAV
        //---------------------------------------------
        {
            TressFX_HairTransform *pTransforms = new TressFX_HairTransform[m_HairAsset.m_NumTotalHairStrands];
            for (int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        if (j == k)
                        {
                            pTransforms[i].matrix[j][k] = 1.0f;
                        }
                        else
                        {
                            pTransforms[i].matrix[j][k] = 0.0f;
                        }
                    }
                    if (j == 3)
                    {
                        pTransforms[i].quaternion[j] = 1.0f;
                    }
                    else
                    {
                        pTransforms[i].quaternion[j] = 0.0f;
                    }
                }
            }

            D3D11_SUBRESOURCE_DATA initialData;
            initialData.pSysMem = (void *)pTransforms;

            D3D11_BUFFER_DESC bufferDescUA;
            bufferDescUA.ByteWidth = m_HairAsset.m_NumTotalHairStrands * sizeof(TressFX_HairTransform);
            bufferDescUA.Usage = D3D11_USAGE_DEFAULT;
            bufferDescUA.BindFlags =  D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
            bufferDescUA.CPUAccessFlags = 0; //D3D11_CPU_ACCESS_READ;
            bufferDescUA.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            bufferDescUA.StructureByteStride  = sizeof(TressFX_HairTransform);
            AMD_SAFE_RELEASE(m_HairTransformsBuffer);
            AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDescUA, &initialData, &m_HairTransformsBuffer));

            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            desc.Buffer.FirstElement = 0;
            desc.Buffer.NumElements = m_HairAsset.m_NumTotalHairStrands;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            AMD_SAFE_RELEASE(m_HairTransformsSRV);
            AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_HairTransformsBuffer, &desc, &m_HairTransformsSRV));

            D3D11_UNORDERED_ACCESS_VIEW_DESC sbUAVDesc;
            sbUAVDesc.Buffer.FirstElement       = 0;
            sbUAVDesc.Buffer.Flags              = 0;
            sbUAVDesc.Buffer.NumElements        = m_HairAsset.m_NumTotalHairStrands;
            sbUAVDesc.Format                    = DXGI_FORMAT_UNKNOWN;
            sbUAVDesc.ViewDimension             = D3D11_UAV_DIMENSION_BUFFER;
            AMD_SAFE_RELEASE(m_HairTransformsUAV);
            AMD_V_RETURN( pd3dDevice->CreateUnorderedAccessView(m_HairTransformsBuffer, &sbUAVDesc, &m_HairTransformsUAV) );

            AMD_SAFE_DELETE_ARRAY(pTransforms);
        }
    }

    if (m_HairAsset.m_pStrandTexCoords != NULL)
    {
        // -----------------------------------------------
        // m_pStrandTexCoordBuffer & m_pStrandTexCoordSRV
        //------------------------------------------------
        {
            D3D11_SUBRESOURCE_DATA initialData;
            initialData.pSysMem = m_HairAsset.m_pStrandTexCoords;
            D3D11_BUFFER_DESC bufferDesc;
            bufferDesc.ByteWidth = m_HairAsset.m_NumTotalHairStrands * sizeof(float) * 2;
            bufferDesc.Usage = D3D11_USAGE_DEFAULT;
            bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
            bufferDesc.CPUAccessFlags = 0;
            bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
            bufferDesc.StructureByteStride  = sizeof(float) * 2;
            AMD_SAFE_RELEASE(m_pStrandTexCoordBuffer);
            AMD_V_RETURN(pd3dDevice->CreateBuffer(&bufferDesc, &initialData, &m_pStrandTexCoordBuffer));

            D3D11_SHADER_RESOURCE_VIEW_DESC desc;
            desc.Format = DXGI_FORMAT_UNKNOWN;
            desc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
            desc.Buffer.ElementOffset = 0;
            desc.Buffer.ElementWidth = m_HairAsset.m_NumTotalHairStrands;
            AMD_SAFE_RELEASE(m_pStrandTexCoordSRV);
            AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_pStrandTexCoordBuffer, &desc, &m_pStrandTexCoordSRV));
        }
    }

    return S_OK;
}


//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when the D3D device is being destroyed.
//
//--------------------------------------------------------------------------------------
void TressFXMesh::OnDestroy()
{
    AMD_SAFE_RELEASE(m_pTriangleIndexBuffer);
    AMD_SAFE_RELEASE(m_pTriangleIndexBufferSRV);

    AMD_SAFE_RELEASE(m_pIndexBuffer);

    AMD_SAFE_RELEASE(m_pThicknessCoeffsBuffer);
    AMD_SAFE_RELEASE(m_pThicknessCoeffsSRV);

    //compute shader variables
    AMD_SAFE_RELEASE(m_HairVertexPositionsUAB);
    AMD_SAFE_RELEASE(m_HairVertexPositionsSRV);
    AMD_SAFE_RELEASE(m_HairVertexPositionsPrevUAB);
    AMD_SAFE_RELEASE(m_HairVertexTangentsUAB);
    AMD_SAFE_RELEASE(m_HairVertexTangentsSRV);
    AMD_SAFE_RELEASE(m_HairVertexPositionsUAV);
    AMD_SAFE_RELEASE(m_HairVertexPositionsPrevUAV);
    AMD_SAFE_RELEASE(m_HairVertexTangentsUAV);

    AMD_SAFE_RELEASE(m_HairStrandTypeBuffer);
    AMD_SAFE_RELEASE(m_HairStrandTypeSRV);

    //vertex buffers
    AMD_SAFE_RELEASE(m_HairLengthBuffer);
    AMD_SAFE_RELEASE(m_HairRestLengthSRV);

    AMD_SAFE_RELEASE(m_InitialHairPositionsBuffer);
    AMD_SAFE_RELEASE(m_InitialHairPositionsSRV);
    AMD_SAFE_RELEASE(m_InitialHairPositionsUAV);

    AMD_SAFE_RELEASE(m_HairRefVectorsBuffer);
    AMD_SAFE_RELEASE(m_HairRefVecsInLocalFrameSRV);

    AMD_SAFE_RELEASE(m_FollowHairRootOffsetBuffer);
    AMD_SAFE_RELEASE(m_FollowHairRootOffsetSRV);

    AMD_SAFE_RELEASE(m_GlobalRotationsUAB);
    AMD_SAFE_RELEASE(m_GlobalRotationsUAV);

    AMD_SAFE_RELEASE(m_LocalRotationsUAB);
    AMD_SAFE_RELEASE(m_LocalRotationsUAV);

    AMD_SAFE_RELEASE(m_HairSkinMappingBuffer);
    AMD_SAFE_RELEASE(m_HairSkinMappingSRV);
    AMD_SAFE_RELEASE(m_HairTransformsBuffer);
    AMD_SAFE_RELEASE(m_HairTransformsUAV);
    AMD_SAFE_RELEASE(m_HairTransformsSRV);
    AMD_SAFE_RELEASE(m_pStrandTexCoordBuffer);
    AMD_SAFE_RELEASE(m_pStrandTexCoordSRV);

    AMD_SAFE_RELEASE(m_pHairTextureSRV);

    DestroyAsset();
}


//--------------------------------------------------------------------------------------
//
// Deserialize
//
// Converts the hair blob from a serialized stream to a TressFXAsset structure.
//
//--------------------------------------------------------------------------------------
bool TressFXMesh::Deserialize(TressFX_HairBlob *pHairBlob)
{
    string str;
    str.assign((char *)pHairBlob->pHair, pHairBlob->size);
    stringbuf buffer;
    buffer.str(str);

    buffer.sgetn((char *)&m_HairAsset.m_NumTotalHairVertices, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumTotalHairStrands, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumOfVerticesInStrand, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumGuideHairVertices, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumGuideHairStrands, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumFollowHairsPerGuideHair, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_TipSeparationFactor, sizeof(float));
    buffer.sgetn((char *)&m_HairAsset.m_NumPerStrandTexCoords, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumPerVertexColors, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumPerVertexTexCoords, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumHairToTriangleMappings, sizeof(int));

    m_HairAsset.m_pHairStrandType = new int[m_HairAsset.m_NumTotalHairStrands];
    buffer.sgetn((char *)m_HairAsset.m_pHairStrandType, m_HairAsset.m_NumTotalHairStrands * sizeof(int));

    m_HairAsset.m_pRefVectors = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pRefVectors, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pGlobalRotations = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pGlobalRotations, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pLocalRotations = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pLocalRotations, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pVertices = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pVertices, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pTangents = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pTangents, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pTriangleVertices = new StrandVertex[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pTriangleVertices, m_HairAsset.m_NumTotalHairVertices * sizeof(StrandVertex));

    m_HairAsset.m_pThicknessCoeffs = new float[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pThicknessCoeffs, m_HairAsset.m_NumTotalHairVertices * sizeof(float));

    m_HairAsset.m_pFollowRootOffset = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairStrands];
    buffer.sgetn((char *)m_HairAsset.m_pFollowRootOffset, m_HairAsset.m_NumTotalHairStrands * sizeof(XMFLOAT4));

    m_HairAsset.m_pRestLengths = new float[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pRestLengths, m_HairAsset.m_NumTotalHairVertices * sizeof(float));

    buffer.sgetn((char *)&m_HairAsset.m_bSphere, sizeof(BSphere));

    m_HairAsset.m_TriangleIndices.clear();

    int size;
    buffer.sgetn((char *)&size, sizeof(int));
    int *pIndices = new int[size];
    buffer.sgetn((char *)pIndices, size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        m_HairAsset.m_TriangleIndices.push_back(pIndices[i]);
    }
    AMD_SAFE_DELETE_ARRAY(pIndices);

    m_HairAsset.m_LineIndices.clear();
    buffer.sgetn((char *)&size, sizeof(int));
    pIndices = new int[size];
    buffer.sgetn((char *)pIndices, size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        m_HairAsset.m_LineIndices.push_back(pIndices[i]);
    }
    AMD_SAFE_DELETE_ARRAY(pIndices);

    if (m_HairAsset.m_NumPerStrandTexCoords)
    {
        m_HairAsset.m_pStrandTexCoords = new DirectX::XMFLOAT2[m_HairAsset.m_NumTotalHairStrands];
        buffer.sgetn((char *)m_HairAsset.m_pStrandTexCoords, m_HairAsset.m_NumTotalHairStrands * sizeof(DirectX::XMFLOAT2));
    }
    else
    {
        m_HairAsset.m_pStrandTexCoords = NULL;
    }

    if (m_HairAsset.m_NumPerVertexColors)
    {
        m_HairAsset.m_pVertexColors = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
        buffer.sgetn((char *)m_HairAsset.m_pVertexColors, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    }
    else
    {
        m_HairAsset.m_pVertexColors = NULL;
    }

    if (m_HairAsset.m_NumPerVertexTexCoords)
    {
        m_HairAsset.m_pVertexTexCoords = new DirectX::XMFLOAT2[m_HairAsset.m_NumTotalHairVertices];
        buffer.sgetn((char *)m_HairAsset.m_pVertexTexCoords, m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT2));
    }
    else
    {
        m_HairAsset.m_pVertexTexCoords = NULL;
    }

    if (m_HairAsset.m_NumHairToTriangleMappings > 0)
    {
        m_HairAsset.m_pMapping = new HairToTriangleMapping[m_HairAsset.m_NumTotalHairStrands];
        buffer.sgetn((char *)m_HairAsset.m_pMapping, m_HairAsset.m_NumTotalHairStrands * sizeof(HairToTriangleMapping));
    }
    else
    {
        m_HairAsset.m_pMapping = NULL;
    }

    return true;
}


//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
void TressFXMesh::DestroyAsset(void)
{
    m_HairAsset.m_TriangleIndices.clear();
    m_HairAsset.m_LineIndices.clear();

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

    if ( m_HairAsset.m_pStrandTexCoords )
    {
        delete [] m_HairAsset.m_pStrandTexCoords;
        m_HairAsset.m_pStrandTexCoords = NULL;
    }

    if ( m_HairAsset.m_pHairStrandType )
    {
        delete [] m_HairAsset.m_pHairStrandType;
        m_HairAsset.m_pHairStrandType = NULL;
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

    if ( m_HairAsset.m_pMapping )
    {
        delete [] m_HairAsset.m_pMapping;
        m_HairAsset.m_pMapping = NULL;
    }
}

} // namespace AMD
