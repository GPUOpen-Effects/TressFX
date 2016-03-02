//--------------------------------------------------------------------------------------
// File: SceneRender.cpp
//
// Main scene rendering code
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

#include "DXUT.h"
#include "AMD_LIB.h"
#include "AMD_SDK.h"
#include "SceneRender.h"

// unreferenced formal parameter
#pragma warning(disable: 4100)

using namespace AMD;
using namespace DirectX;

struct CB_PER_FRAME_SCENE
{
    XMMATRIX        m_mWorld;
    XMMATRIX        m_mViewProj;
    XMMATRIX        m_mInvViewProj;
    XMMATRIX        m_mViewProjLight;
    XMFLOAT4        m_vEye;
    XMFLOAT4        m_PointLightPos;
    XMFLOAT4        m_PointLightColor;
    XMFLOAT4        m_AmbientLightColor;
};

struct CB_PER_MATERIAL
{
    XMVECTOR        m_MatAmbient;
    XMVECTOR        m_MatDiffuse;  // alpha value: whether texture is used (yes: 1; no: -1)
    XMVECTOR        m_MatSpecular; // alpha value: Specular Exponent
    XMVECTOR        m_ScalpColor;
};

struct CB_BLUR
{
    float       width;
    float       height;
    float       invWidth;
    float       invHeight;
};

#define MAX_BONE_MATRICES 255

#define IDSVR_SCENE_SHADOW_TEXTURE          12
#define IDSRV_SCENE_BONES                   13
#define IDSRV_SCENE_NORMAL_MAP              14
#define IDSRV_SCENE_SPEC_MASK               15
#define IDSRV_SCENE_DIFFUSE                 16

//--------------------------------------------------------------------------------------
//
// CalcSkinnedMeshBoundingBox
//
// Calculate bounding box of the mesh
//
//--------------------------------------------------------------------------------------
void CSceneRender::CalculateMeshBoundingBox()
{
    XMFLOAT3 lower(FLT_MAX, FLT_MAX, FLT_MAX);
    XMFLOAT3 upper(FLT_MIN, FLT_MIN, FLT_MIN);

    for (unsigned int i = 0; i < m_SkinnedMesh.GetNumMeshes(); ++i)
    {
        if (m_bAnimation)
        {
            DirectX::XMMATRIX mIdentity = DirectX::XMMatrixIdentity();
            m_SkinnedMesh.TransformMesh( mIdentity, 0.0f );
            XMMATRIX matrix = m_SkinnedMesh.GetInfluenceMatrix(0);
            //XMMATRIX matrix  = m_SkinnedMesh.m_pBindPoseFrameMatrices[0]
        }

        XMFLOAT3 bBoxCenter, bBoxHalf;
        XMStoreFloat3(&bBoxCenter, m_SkinnedMesh.GetMeshBBoxCenter(i));
        XMStoreFloat3(&bBoxHalf, m_SkinnedMesh.GetMeshBBoxExtents(i));

        if ((bBoxCenter.x - bBoxHalf.x) < lower.x)
        {
            lower.x = (bBoxCenter.x - bBoxHalf.x);
        }
        if ((bBoxCenter.y - bBoxHalf.y) < lower.y)
        {
            lower.y = (bBoxCenter.y - bBoxHalf.y);
        }
        if ((bBoxCenter.z - bBoxHalf.z) < lower.z)
        {
            lower.z = (bBoxCenter.z - bBoxHalf.z);
        }
        if ((bBoxCenter.x + bBoxHalf.x) > upper.x)
        {
            upper.x = (bBoxCenter.x + bBoxHalf.x);
        }
        if ((bBoxCenter.y + bBoxHalf.y) > upper.y)
        {
            upper.y = (bBoxCenter.y + bBoxHalf.y);
        }
        if ((bBoxCenter.z + bBoxHalf.z) > upper.z)
        {
            upper.z = (bBoxCenter.z + bBoxHalf.z);
        }
    }

    m_MeshBBoxCenter.x = ((lower.x + upper.x) / 2.0f);
    m_MeshBBoxCenter.y = ((lower.y + upper.y) / 2.0f);
    m_MeshBBoxCenter.z = ((lower.z + upper.z) / 2.0f);
    m_MeshBBoxExtent.x = ((upper.x - lower.x) / 2.0f);
    m_MeshBBoxExtent.y = ((upper.y - lower.y) / 2.0f);
    m_MeshBBoxExtent.z = ((upper.z - lower.z) / 2.0f);
}

//--------------------------------------------------------------------------------------
//
// CalculateMeshAmbient
//
// Calculate luminance value corresponding to the ambient values of the materials
// averaged together. The intent is to give us a decent guess at an ambient value
// to use during hair rendering.
//
//--------------------------------------------------------------------------------------
void CSceneRender::CalculateMeshAmbient()
{
    float fCounter = 0.0f;
    XMVECTOR AmbientAccumulator = XMVectorSet(0, 0, 0, 0);

    // Render the skinned mesh
    for ( UINT m = 0; m < m_SkinnedMesh.GetNumMeshes(); m++ )
    {
        // Set materials
        SDKMESH_SUBSET* pSubset = NULL;
        SDKMESH_MATERIAL* pMat = NULL;

        for ( UINT subset = 0; subset < m_SkinnedMesh.GetNumSubsets( m ); subset++ )
        {
            pSubset = m_SkinnedMesh.GetSubset( m, subset );

            pMat = m_SkinnedMesh.GetMaterial( pSubset->MaterialID );
            if ( pMat )
            {
                AmbientAccumulator += XMVectorSet(pMat->Ambient.x, pMat->Ambient.y, pMat->Ambient.z, 0);
                fCounter += 1.0f;
            }
        }
    }
    AmbientAccumulator /= fCounter;
    XMFLOAT3 f3AmbientAccumulator;
    XMStoreFloat3(&f3AmbientAccumulator, AmbientAccumulator);
    m_MeshAmbient = 0.299f*f3AmbientAccumulator.x + 0.587f*f3AmbientAccumulator.y + 0.114f*f3AmbientAccumulator.z;
}

//--------------------------------------------------------------------------------------
//
// CreateShaderAndLayout
//
// Creates the shaders and layouts for scene rendering
//
//--------------------------------------------------------------------------------------
void CSceneRender::CreateShaderAndLayout( ID3D11Device* pd3dDevice, AMD::ShaderCache *pShaderCache )
{

    // skinned mesh layout
    const D3D11_INPUT_ELEMENT_DESC skinnedlayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "WEIGHTS",  0, DXGI_FORMAT_R8G8B8A8_UNORM,     0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BONES",    0, DXGI_FORMAT_R8G8B8A8_UINT,      0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT,       0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    const D3D11_INPUT_ELEMENT_DESC scenelayout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R16G16_FLOAT,       0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "BINORMAL", 0, DXGI_FORMAT_R16G16B16A16_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    // screen quad layout
    const D3D11_INPUT_ELEMENT_DESC layout_mesh[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };


    // Add shaders to the cache (do this only once)

    // vertex shaders
    // Shader creation might be deferred in shader cache, so we have to assgin ID3D11InputLayout* pointer
    // for each VS shader to track the created ID3D11InputLayout object.
    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSStreamOutVerts, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"StreamOutVS", L"SceneRender.hlsl", 0, NULL, &m_pStreamOutLayout,
        (D3D11_INPUT_ELEMENT_DESC*)skinnedlayout, ARRAYSIZE( skinnedlayout ) );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSStreamOutSkinned, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"SkinStreamOutVS", L"SceneRender.hlsl", 0, NULL, &m_pStreamOutLayoutSkinned,
        (D3D11_INPUT_ELEMENT_DESC*)skinnedlayout, ARRAYSIZE( skinnedlayout ) );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSScene, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"SceneVS", L"SceneRender.hlsl", 0, NULL, &m_pScenelayout,
        (D3D11_INPUT_ELEMENT_DESC*)scenelayout, ARRAYSIZE( scenelayout ) );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSGenerateSceneSM, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"VS_GenerateSceneSM", L"SceneRender.hlsl", 0, NULL, &m_pSceneSMLayout,
        (D3D11_INPUT_ELEMENT_DESC*)scenelayout, ARRAYSIZE( scenelayout ) );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSSkinning, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"SkinningVS", L"SceneRender.hlsl", 0, NULL, &m_pSkinningLayout,
        (D3D11_INPUT_ELEMENT_DESC*)skinnedlayout, ARRAYSIZE( skinnedlayout ) );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSGenerateSkinnedSM, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"VS_GenerateSkinnedSM", L"SceneRender.hlsl", 0, NULL, &m_pLayoutSceneSM,
        (D3D11_INPUT_ELEMENT_DESC*)skinnedlayout, ARRAYSIZE( skinnedlayout ) );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVSScreenQuad, AMD::ShaderCache::SHADER_TYPE_VERTEX,
        L"vs_5_0", L"VS_ScreenQuad", L"SceneRender.hlsl", 0, NULL, &m_pQuadLayout,
        (D3D11_INPUT_ELEMENT_DESC*)layout_mesh, ARRAYSIZE( layout_mesh ) );

    // pixel shaders
    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pPSRenderScene, AMD::ShaderCache::SHADER_TYPE_PIXEL, L"ps_5_0", L"PS_RenderScene",
        L"SceneRender.hlsl", 0, NULL, NULL, NULL, 0 );

    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pPSRenderSceneShadow, AMD::ShaderCache::SHADER_TYPE_PIXEL, L"ps_5_0", L"PS_RenderSceneShadow",
        L"SceneRender.hlsl", 0, NULL, NULL, NULL, 0 );

    AMD::ShaderCache::Macro Macros[4];
    wcscpy_s( Macros[0].m_wsName, AMD::ShaderCache::m_uMACRO_MAX_LENGTH, L"GAUSSIAN_FILTER" );
    Macros[0].m_iValue = 1;
    wcscpy_s( Macros[1].m_wsName, AMD::ShaderCache::m_uMACRO_MAX_LENGTH, L"KERNEL_RADIUS" );
    Macros[1].m_iValue = SHADOW_BLUR_RADIUS;
    wcscpy_s( Macros[2].m_wsName, AMD::ShaderCache::m_uMACRO_MAX_LENGTH, L"USE_APPROXIMATE_FILTER" );
    Macros[2].m_iValue = SHADOW_BLUR_PRECISION;
    wcscpy_s( Macros[3].m_wsName, AMD::ShaderCache::m_uMACRO_MAX_LENGTH, L"HORIZ" );
    Macros[3].m_iValue = 1;
    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pHorizontalBlurPS, AMD::ShaderCache::SHADER_TYPE_PIXEL, L"ps_5_0", L"PSFilterX",
        L"GaussianFilter.hlsl", 4, Macros, NULL, NULL, 0 );
    wcscpy_s( Macros[3].m_wsName, AMD::ShaderCache::m_uMACRO_MAX_LENGTH, L"VERT" );
    Macros[3].m_iValue = 1;
    pShaderCache->AddShader( (ID3D11DeviceChild**)&m_pVerticalBlurPS, AMD::ShaderCache::SHADER_TYPE_PIXEL, L"ps_5_0", L"PSFilterY",
        L"GaussianFilter.hlsl", 4, Macros, NULL, NULL, 0 );

    // Create the stream-out geometry shader from the stream-out vertex shaders
    ID3DBlob *pStreamOutVS = NULL, *pErrors = NULL;
    HRESULT hr;
    hr = D3DCompileFromFile(L"..\\src\\Shaders\\SceneRender.hlsl", NULL, NULL, "SkinStreamOutVS", "vs_5_0", 0, 0, &pStreamOutVS, &pErrors);
    if (pErrors)
    {
        wchar_t *errorString;
        char *msg = (char *)pErrors->GetBufferPointer();
        errorString = new wchar_t[(int)pErrors->GetBufferSize()];
        for (int i = 0; i < (int)pErrors->GetBufferSize(); i++)
        {
            errorString[i] = wchar_t(msg[i]);
        }
        OutputDebugString(errorString);
        delete []errorString;
    }

    const D3D11_SO_DECLARATION_ENTRY SODeclarationEntry[] =
    {
        {0, "POSITION", 0, 0, 4, 0 },
    };
    UINT bufferStrides[] = { sizeof(float) * 4 };

    // create the geometry shader for skinning
    hr = pd3dDevice->CreateGeometryShaderWithStreamOutput(pStreamOutVS->GetBufferPointer(), pStreamOutVS->GetBufferSize(),
        SODeclarationEntry, 1, bufferStrides, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, &m_pGSSkinStreamOut);

    // create the geometry shader for streaming out untransformed vertices
    hr = D3DCompileFromFile(L"..\\src\\Shaders\\SceneRender.hlsl", NULL, NULL, "StreamOutVS", "vs_5_0", 0, 0, &pStreamOutVS, &pErrors);
    if (pErrors)
    {
        wchar_t *errorString;
        char *msg = (char *)pErrors->GetBufferPointer();
        errorString = new wchar_t[(int)pErrors->GetBufferSize()];
        for (int i = 0; i < (int)pErrors->GetBufferSize(); i++)
        {
            errorString[i] = wchar_t(msg[i]);
        }
        OutputDebugString(errorString);
        delete []errorString;
    }
    hr = pd3dDevice->CreateGeometryShaderWithStreamOutput(pStreamOutVS->GetBufferPointer(), pStreamOutVS->GetBufferSize(),
        SODeclarationEntry, 1, bufferStrides, 1, D3D11_SO_NO_RASTERIZED_STREAM, NULL, &m_pGSStreamOut);
}

//--------------------------------------------------------------------------------------
//
// CreateTextureAndViews
//
// Creates the textures and corrisponding views for scene rendering
//
//--------------------------------------------------------------------------------------
void CSceneRender::CreateTextureAndViews( ID3D11Device* pd3dDevice )
{
    HRESULT hr;

    // Create SM DSVs for scene
    D3D11_TEXTURE2D_DESC tex2D_desc;
    tex2D_desc.Width =  SM_SCENE_WIDTH;
    tex2D_desc.Height = SM_SCENE_HEIGHT;
    tex2D_desc.ArraySize = 1;
    tex2D_desc.Format = DXGI_FORMAT_R32_TYPELESS;
    tex2D_desc.Usage = D3D11_USAGE_DEFAULT;
    tex2D_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL|D3D11_BIND_SHADER_RESOURCE;
    tex2D_desc.CPUAccessFlags = 0;
    tex2D_desc.MipLevels = 0;
    tex2D_desc.MiscFlags = 0;
    tex2D_desc.SampleDesc.Count = 1;
    tex2D_desc.SampleDesc.Quality = 0;

    hr = pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pSMSceneTx);
    if (FAILED(hr))
    {
        ::MessageBoxW(0, L"fail to create m_pSMSceneTx", L"d3d Error", 0);
    }

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = 0;
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    if (FAILED(pd3dDevice->CreateDepthStencilView(m_pSMSceneTx, &dsvDesc, &m_pSMSceneDSV)))
    {
        ::MessageBoxW(0, L"fail to create m_pSMSceneDSV", L"d3d Error", 0);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    if (FAILED(pd3dDevice->CreateShaderResourceView(m_pSMSceneTx, &srvDesc, &m_pSMSceneSRV)))
    {
        ::MessageBoxW(0, L"fail to create m_pMSceneSRV", L"d3d Error", 0);
    }

    // Shadow texture for scene rendering
    tex2D_desc.Width =  SM_SCENE_WIDTH;
    tex2D_desc.Height = SM_SCENE_HEIGHT;
    tex2D_desc.MipLevels = 0;
    tex2D_desc.ArraySize = 1;
    tex2D_desc.Format = DXGI_FORMAT_R8_UNORM;
    tex2D_desc.SampleDesc.Count = 1;
    tex2D_desc.SampleDesc.Quality = 0;
    tex2D_desc.Usage = D3D11_USAGE_DEFAULT;
    tex2D_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
    tex2D_desc.CPUAccessFlags = 0;
    tex2D_desc.MiscFlags = 0;

    hr = pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pSceneShadowTexture);
    if (FAILED(hr))
    {
        ::MessageBoxW(0, L"fail to create m_pSceneShadowTexture", L"d3d Error", 0);
    }

    hr = pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pSceneShadowTexture_temp);
    if (FAILED(hr))
    {
        ::MessageBoxW(0, L"fail to create m_pSceneShadowTexture_temp", L"d3d Error", 0);
    }

    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
    srDesc.Format = DXGI_FORMAT_R8_UNORM;
    srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture1D.MostDetailedMip = 0;
    srDesc.Texture1D.MipLevels = 0xffffffff;
    pd3dDevice->CreateShaderResourceView(m_pSceneShadowTexture, &srDesc, &m_pSceneShadowSRV);
    pd3dDevice->CreateShaderResourceView(m_pSceneShadowTexture_temp, &srDesc, &m_pSceneShadowSRV_temp);

    D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
    rtDesc.Format = DXGI_FORMAT_R8_UNORM;
    rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
    rtDesc.Texture2D.MipSlice = 0;
    pd3dDevice->CreateRenderTargetView(m_pSceneShadowTexture, &rtDesc, &m_pSceneShadowRTV);
    pd3dDevice->CreateRenderTargetView(m_pSceneShadowTexture_temp, &rtDesc, &m_pSceneShadowRTV_temp);

    // Create a bone matrix buffer
    D3D11_BUFFER_DESC vbdesc =
    {
        MAX_BONE_MATRICES * sizeof( DirectX::XMMATRIX ),
        D3D11_USAGE_DYNAMIC,
        D3D11_BIND_SHADER_RESOURCE,
        D3D11_CPU_ACCESS_WRITE,
        0
    };
    pd3dDevice->CreateBuffer( &vbdesc, NULL, &m_pBoneBuffer );

    D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
    ZeroMemory( &SRVDesc, sizeof( SRVDesc ) );
    SRVDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    SRVDesc.Buffer.ElementOffset = 0;
    SRVDesc.Buffer.ElementWidth = MAX_BONE_MATRICES * 4;
    pd3dDevice->CreateShaderResourceView( m_pBoneBuffer, &SRVDesc, &m_pBoneBufferRV );

}

//--------------------------------------------------------------------------------------
//
// CreateVertexBuffers
//
// Creates the buffers for scene rendering
//
//--------------------------------------------------------------------------------------
void CSceneRender::CreateVertexBuffers(ID3D11Device* pd3dDevice)
{
    HRESULT hr;
    // Create the screen quad vertex buffer(use StandardVertex for simplicity)
    const StandardVertex screenQuad[6] =
    {
        { XMFLOAT3(-1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 1.0f) }, //0
        { XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, //1
        { XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //2
        { XMFLOAT3( 1.0f, -1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 1.0f) }, //2
        { XMFLOAT3(-1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f) }, //1
        { XMFLOAT3( 1.0f,  1.0f, 0.0f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f) }  //3
    };

    D3D11_BUFFER_DESC bd;
    bd.Usage =          D3D11_USAGE_DEFAULT;
    bd.ByteWidth =      sizeof( StandardVertex ) * 6;
    bd.BindFlags =      D3D11_BIND_VERTEX_BUFFER;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags =      0;

    D3D11_SUBRESOURCE_DATA initData;
    initData.pSysMem =  screenQuad;
    initData.SysMemPitch = 0;
    initData.SysMemSlicePitch = 0;
    hr = pd3dDevice->CreateBuffer( &bd, &initData, &m_pScreenQuadVB );
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "Fail to create screen quad vertex buffer", "d3d error", 0);
    }

    m_numMeshes = m_SkinnedMesh.GetNumMeshes();
    m_pMeshIndexCount = new unsigned[m_numMeshes];
    unsigned totalIndexCount = 0;

    for ( UINT m = 0; m < m_SkinnedMesh.GetNumMeshes(); m++ )
    {
        m_pMeshIndexCount[m] = 0;
        for ( UINT subset = 0; subset < m_SkinnedMesh.GetNumSubsets( m ); subset++ )
        {
            SDKMESH_SUBSET *pSubset = m_SkinnedMesh.GetSubset( m, subset );
            m_pMeshIndexCount[m] += (unsigned)pSubset->IndexCount;
        }
        totalIndexCount += m_pMeshIndexCount[m];
    }

    // Stream Out Buffers
    bd.Usage =          D3D11_USAGE_DEFAULT;
    bd.ByteWidth =      sizeof(float) * 4 * totalIndexCount;
    bd.BindFlags =      D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_STREAM_OUTPUT;
    bd.CPUAccessFlags = 0;
    bd.MiscFlags =      0;

   // Create a buffer that will hold all of the un-transformed vertices
    hr = pd3dDevice->CreateBuffer( &bd, NULL, &m_pMeshVertices );
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "Failed to allocate untransformed vertex buffers", "d3d error", 0);
    }

    // create an SRV for the un-transformed vertices
    D3D11_SHADER_RESOURCE_VIEW_DESC descSRV;
    descSRV.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    descSRV.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    descSRV.Buffer.ElementOffset = 0;
    descSRV.Buffer.ElementWidth = totalIndexCount;
    hr = pd3dDevice->CreateShaderResourceView(m_pMeshVertices, &descSRV, &m_pMeshVerticesSRV);
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "Fail to create transformed vertex buffer SRV", "d3d error", 0);
    }

    // Create a buffer that will hold all of the transformed vertices
    hr = pd3dDevice->CreateBuffer( &bd, NULL, &m_pTransformedVerts );
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "Failed to allocate transformed vertex buffers", "d3d error", 0);
    }

    // create an SRV for the transformed vertices
    hr = pd3dDevice->CreateShaderResourceView(m_pTransformedVerts, &descSRV, &m_pTransformedSRV);
    if ( FAILED( hr ) )
    {
        ::MessageBoxA(0, "Fail to create transformed vertex buffer SRV", "d3d error", 0);
    }
}

//--------------------------------------------------------------------------------------
//
// CreateConstantBuffers
//
// Creates the constant buffers for scene rendering
//
//--------------------------------------------------------------------------------------
void CSceneRender::CreateConstantBuffer( ID3D11Device* pd3dDevice )
{
    HRESULT hr;
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory( &cbDesc, sizeof(cbDesc) );
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.ByteWidth = sizeof( CB_PER_MATERIAL );
    hr = pd3dDevice->CreateBuffer( &cbDesc, NULL, &m_pcbPerMaterial );
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create constant buffer for material", "d3d error", 0);
    }

    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.ByteWidth = sizeof( CB_PER_FRAME_SCENE );
    hr = pd3dDevice->CreateBuffer( &cbDesc, NULL, &m_pcbPerFrame );
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create constant buffer for frame", "d3d error", 0);
    }

    cbDesc.ByteWidth = sizeof( CB_BLUR );
    cbDesc.Usage = D3D11_USAGE_IMMUTABLE;
    cbDesc.CPUAccessFlags = 0;
    CB_BLUR initData;
    initData.width = (float)SM_SCENE_WIDTH;
    initData.height = (float)SM_SCENE_HEIGHT;
    initData.invWidth = 1.0f / (float)SM_SCENE_WIDTH;
    initData.invHeight = 1.0f / (float)SM_SCENE_HEIGHT;

    D3D11_SUBRESOURCE_DATA subResourceData;
    subResourceData.pSysMem = (void *)&initData;
    hr = pd3dDevice->CreateBuffer( &cbDesc, &subResourceData, &m_pcbBlur );
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create constant buffer for shadow blur", "d3d error", 0);
    }
}

//--------------------------------------------------------------------------------------
//
// CreateRenderStateObjects
//
// Creates the constant buffers for scene rendering
//
//--------------------------------------------------------------------------------------
void CSceneRender::CreateRenderStateObjects( ID3D11Device* pd3dDevice )
{
    HRESULT hr;

    // Create depth stencil states
    D3D11_DEPTH_STENCIL_DESC DSDesc;
    DSDesc.StencilReadMask              = 0xff;
    DSDesc.StencilWriteMask             = 0xff;
    DSDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    DSDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
    DSDesc.DepthEnable                  = TRUE;
    DSDesc.StencilEnable                = FALSE;
    DSDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
    DSDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
    hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestEnabledDSS);

    DSDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
    hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestEnabledNoWriteDSS);

    DSDesc.DepthEnable                  = FALSE;
    hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthStencilDisableState);

    // Create sampler state objects
    D3D11_SAMPLER_DESC samDesc;
    ZeroMemory( &samDesc, sizeof(samDesc) );
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.MaxAnisotropy = 16;
    samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = D3D11_FLOAT32_MAX;
    hr = pd3dDevice->CreateSamplerState( &samDesc, &m_pSamplerStateLinearWrap );
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create linear wrap sampler state", "D3D Error", 0);
    }

    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    hr = pd3dDevice->CreateSamplerState( &samDesc, &m_pSamplerStatePointClamp );
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create point sampler state", "D3D Error", 0);
    }

    ZeroMemory( &samDesc, sizeof(samDesc) );
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
    samDesc.MaxAnisotropy = 1;
    samDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samDesc.MaxLOD = D3D11_FLOAT32_MAX;
    if (FAILED(pd3dDevice->CreateSamplerState(&samDesc, &m_pLinearClampSampler)))
    {
        ::MessageBoxA(0, "Fail to create linear clamp sampler state", "D3D error", 0);
    }
    // Point sampler
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    if (FAILED(pd3dDevice->CreateSamplerState(&samDesc, &m_pPointSampler)))
    {
        ::MessageBoxA(0, "Fail to create linear point sampler state", "D3D error", 0);
    }

     // Create rasterizer state for scene rendering
    D3D11_RASTERIZER_DESC rasterizerDesc =
    {
        D3D11_FILL_SOLID,   //D3D11_FILL_MODE FillMode;
        //D3D11_FILL_WIREFRAME,//D3D11_FILL_MODE FillMode;
        D3D11_CULL_BACK,    //D3D11_CULL_MODE CullMode;
        FALSE,              //BOOL FrontCounterClockwise
        0,                  //INT DepthBias;
        0.0f,               //FLOAT DepthBiasClamp;
        0,                  //FLOAT SlopeScaledDepthBias;
        TRUE,               //BOOL DepthClipEnable;
        FALSE,              //BOOL ScissorEnable;
        FALSE,              //BOOL MultisampleEnable;
        FALSE               //BOOL AntialiasedLineEnable;
    };
    hr = pd3dDevice->CreateRasterizerState(&rasterizerDesc, &m_pRasterizerStateScene);
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create rasterizer state: sceneShadowmap", "D3D Error", 0);
    }

    // Create rasterizer state for shadow map rendering
    D3D11_RASTERIZER_DESC drd =
    {
        D3D11_FILL_SOLID,   //D3D11_FILL_MODE FillMode;
        D3D11_CULL_NONE,    //D3D11_CULL_MODE CullMode;
        FALSE,              //BOOL FrontCounterClockwise;
        256,                //INT DepthBias;
        0.0f,               //FLOAT DepthBiasClamp;
        2.f,                //FLOAT SlopeScaledDepthBias;
        FALSE,              //BOOL DepthClipEnable;
        FALSE,              //BOOL ScissorEnable;
        FALSE,              //BOOL MultisampleEnable;
        FALSE               //BOOL AntialiasedLineEnable;
    };
    hr = pd3dDevice->CreateRasterizerState(&drd, &m_pRasterizerStateShadowmap);
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to create rasterizer state: sceneShadowmap", "D3D Error", 0);
    }
}

//--------------------------------------------------------------------------------------
//
// OnCreateDevice
//
// Called when the device is created. Allocates resources used by the scene rendering.
//
//--------------------------------------------------------------------------------------
void CSceneRender::OnCreateDevice( ID3D11Device* pd3dDevice, const WCHAR *bodyFile, const WCHAR *animationFile, AMD::ShaderCache *pShaderCache, bool createShaders)
{
    m_SkinnedMesh.Create(pd3dDevice, bodyFile, NULL);
    if (animationFile != NULL)
    {
        m_SkinnedMesh.LoadAnimation(animationFile);
        m_bAnimation = true;
    }
    else
    {
        m_bAnimation = false;
    }

    DirectX::XMMATRIX mIdentity = DirectX::XMMatrixIdentity();
    m_SkinnedMesh.TransformBindPose( mIdentity );

    CalculateMeshBoundingBox();
    CalculateMeshAmbient();

    if (createShaders)
    {
        CreateShaderAndLayout(pd3dDevice, pShaderCache);
    }

    CreateTextureAndViews(pd3dDevice);
    CreateVertexBuffers(pd3dDevice);
    CreateConstantBuffer(pd3dDevice);
    CreateRenderStateObjects(pd3dDevice);
}

//--------------------------------------------------------------------------------------
//
// RenderScreenQuad
//
// Renders a full screen quad
//
//--------------------------------------------------------------------------------------
void CSceneRender::RenderScreenQuad(ID3D11DeviceContext* pd3dContext,
    ID3D11VertexShader* pVS, ID3D11PixelShader* pPS)
{
    // set shader
    pd3dContext->VSSetShader( pVS, NULL, 0 );
    pd3dContext->PSSetShader( pPS, NULL, 0 );

    UINT strides = sizeof( StandardVertex );
    UINT offsets = 0;
    pd3dContext->IASetInputLayout( m_pQuadLayout );
    pd3dContext->IASetVertexBuffers( 0, 1, &m_pScreenQuadVB, &strides, &offsets );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw full screen quad
    pd3dContext->Draw( 6, 0);
}

//--------------------------------------------------------------------------------------
//  OnFrameMove
//
// Handle updates to the scene.  This is called regardless of which D3D API is used
//--------------------------------------------------------------------------------------
void CSceneRender::OnFrameMove( double fTime)
{
    DirectX::XMMATRIX mIdentity = DirectX::XMMatrixIdentity();
    m_SkinnedMesh.TransformMesh( mIdentity, fTime );
}


//--------------------------------------------------------------------------------------
//  SetBoneMatrices
//
//  This loads the matrices into a buffer that is bound to the shader.
//--------------------------------------------------------------------------------------
void CSceneRender::SetBoneMatrices(ID3D11DeviceContext* pd3dImmediateContext, UINT iMesh )
{
    if (!m_bAnimation)
    {
        return;
    }

    DirectX::XMMATRIX* pMatrices;
    D3D11_MAPPED_SUBRESOURCE MappedResource;

    pd3dImmediateContext->Map( m_pBoneBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
    pMatrices = (DirectX::XMMATRIX *)MappedResource.pData;
    for ( UINT i = 0; i < m_SkinnedMesh.GetNumInfluences( iMesh ); i++ )
    {
        pMatrices[i] = m_SkinnedMesh.GetMeshInfluenceMatrix( iMesh, i );
    }
    pd3dImmediateContext->Unmap(m_pBoneBuffer, 0);
    pd3dImmediateContext->VSSetShaderResources(IDSRV_SCENE_BONES, 1, &m_pBoneBufferRV);
}

//--------------------------------------------------------------------------------------
//
// GenerateShadowMap
//
// Renders the scene geometry from the point of view of the light into a shadow map.
//
//--------------------------------------------------------------------------------------
void CSceneRender::GenerateShadowMap(TressFX_Desc & desc)
{
    // Get original render target and depth stencil view
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();
    ID3D11RasterizerState* pRSTemp;
    ID3D11DeviceContext* pd3dContext = desc.pd3dDeviceContext;
    pd3dContext->RSGetState( &pRSTemp );

    // Set viewport value
    D3D11_VIEWPORT viewportSMScene = {0, 0, SM_SCENE_WIDTH, SM_SCENE_HEIGHT, 0.0f, 1.0f};
    pd3dContext->RSSetViewports( 1, &viewportSMScene );
    // clear depth for early z
    pd3dContext->ClearDepthStencilView(m_pSMSceneDSV, D3D11_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0, 0);
    // make suure the scene shadow map texture is unbound
    ID3D11ShaderResourceView *pNull = NULL;
    pd3dContext->PSSetShaderResources(IDSRV_SCENESM, 1, &pNull);
    // set render target to shadow map texture
    pd3dContext->OMSetRenderTargets(0, 0, m_pSMSceneDSV);
    pd3dContext->RSSetState( m_pRasterizerStateShadowmap );

    if (m_bAnimation)
    {
        RenderSceneGeometry(desc, m_pVSGenerateSkinnedSM, NULL);
    }
    else
    {
        RenderSceneGeometry(desc, m_pVSGenerateSceneSM, NULL);
    }

    // reset view port
    D3D11_VIEWPORT viewportWin = {0, 0, (float)g_ScreenWidth, (float)g_ScreenHeight, 0.0f, 1.0f};
    pd3dContext->RSSetViewports(1, &viewportWin);

    // reset render targets
    pd3dContext->OMSetRenderTargets(1, &pRTV, pDSV);
    pd3dContext->RSSetState( pRSTemp );
    pRSTemp->Release();
}

//--------------------------------------------------------------------------------------
//
// SkinSDKMesh
//
// Skins the SDKMesh object and stores the transformed vertices into a buffer.
//
//--------------------------------------------------------------------------------------
void CSceneRender::StreamOutVertices(ID3D11DeviceContext* pd3dContext, bool doSkinning)
{

    // skin the mesh
    ID3D11Buffer* pVertexBuffers[1];
    ID3D11Buffer* pSOBuffers[1];
    UINT stride[1];
    UINT offsetVB[1];
    UINT offsetSO[1];

    //
    // Skin the vertices and stream them out
    //
    pd3dContext->IASetInputLayout( m_pSkinningLayout );
    if (doSkinning)
    {
        pd3dContext->VSSetShader( m_pVSStreamOutSkinned, NULL, 0 );
        pd3dContext->GSSetShader( m_pGSSkinStreamOut, NULL, 0 );
        pSOBuffers[0] = m_pTransformedVerts;
    }
    else
    {
        pd3dContext->VSSetShader( m_pVSStreamOutVerts, NULL, 0 );
        pd3dContext->GSSetShader( m_pGSStreamOut, NULL, 0 );
        pSOBuffers[0] = m_pMeshVertices;
    }
    pd3dContext->PSSetShader( NULL, NULL, 0 );
    pd3dContext->OMSetDepthStencilState(m_pDepthStencilDisableState, 0);

    UINT vertexOffset = 0;

    for ( UINT m = 0; m < m_numMeshes; m++ )
    {
        // Turn on stream out
        offsetSO[0] = vertexOffset;
        pd3dContext->SOSetTargets( 1, pSOBuffers, offsetSO );

        // Set source vertex buffer
        pVertexBuffers[0] = m_SkinnedMesh.GetVB11( m, 0 );
        stride[0] = m_SkinnedMesh.GetVertexStride( m, 0 );
        offsetVB[0] = 0;
        pd3dContext->IASetVertexBuffers( 0, 1, pVertexBuffers, stride, offsetVB );
        pd3dContext->IASetIndexBuffer( m_SkinnedMesh.GetIB11( m ), m_SkinnedMesh.GetIBFormat11( m ), 0 );
        pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        // Set the bone matrices
        if (doSkinning)
        {
            SetBoneMatrices(pd3dContext, m);
        }

        SDKMESH_SUBSET* pSubset = NULL;
        for ( UINT subset = 0; subset < m_SkinnedMesh.GetNumSubsets( m ); subset++ )
        {
            pSubset = m_SkinnedMesh.GetSubset( m, subset );
            pd3dContext->DrawIndexed( ( UINT )pSubset->IndexCount, ( UINT )pSubset->IndexStart,
                                        ( UINT )pSubset->VertexStart );
            vertexOffset += (UINT)pSubset->IndexCount * sizeof(float) * 4;
        }
        // unbind the buffers
        pSOBuffers[0] = NULL;
        pd3dContext->SOSetTargets( 1, pSOBuffers, offsetSO );
        pd3dContext->GSSetShader( NULL, NULL, 0 );
    }

}
//--------------------------------------------------------------------------------------
//
// UpdateSceneCB
//
// Updates the scene's per-frame constant buffer.
//
//--------------------------------------------------------------------------------------
void CSceneRender::UpdateSceneCB(TressFX_Desc & desc)
{
    // set per-frame CB
    D3D11_MAPPED_SUBRESOURCE MappedResource;
    HRESULT hr = desc.pd3dDeviceContext->Map( m_pcbPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
    if (FAILED(hr))
    {
        ::MessageBoxA(0, "Fail to map constant buffer frame", "d3d error", 0);
    }

    CB_PER_FRAME_SCENE* pcbPerFrame = ( CB_PER_FRAME_SCENE* )MappedResource.pData;
    pcbPerFrame->m_mWorld = XMMatrixTranspose(desc.modelTransformForHead);
    pcbPerFrame->m_mViewProj = XMMatrixTranspose(desc.mViewProj);
    pcbPerFrame->m_mInvViewProj = XMMatrixTranspose(desc.mInvViewProj);
    pcbPerFrame->m_mViewProjLight = XMMatrixTranspose(desc.mViewProjLightFromLibrary);
    XMStoreFloat4(&pcbPerFrame->m_vEye, desc.eyePoint);
    XMStoreFloat4(&pcbPerFrame->m_PointLightPos, desc.lightPosition);
    pcbPerFrame->m_AmbientLightColor = desc.hairParams.ambientLightColor;
    pcbPerFrame->m_PointLightColor = desc.hairParams.pointLightColor;
    desc.pd3dDeviceContext->Unmap( m_pcbPerFrame, 0 );

    desc.pd3dDeviceContext->VSSetConstantBuffers( 5, 1, &m_pcbPerFrame );
    desc.pd3dDeviceContext->PSSetConstantBuffers( 5, 1, &m_pcbPerFrame );
}

//--------------------------------------------------------------------------------------
//
// RenderSceneGeometry
//
// Renders the scene geometry.
//
//--------------------------------------------------------------------------------------
void CSceneRender::RenderSceneGeometry(TressFX_Desc & desc, ID3D11VertexShader* pVS, ID3D11PixelShader* pPS)
{
    HRESULT hr;
    ID3D11Buffer* pBuffers[1];
    UINT stride[1];
    UINT offset[1] = { 0 };

    // set shaders
    ID3D11DeviceContext* pd3dContext = desc.pd3dDeviceContext;

    // Set vertex Layout
    if (m_bAnimation)
    {
        pd3dContext->IASetInputLayout(m_pSkinningLayout);
    }
    else
    {
        pd3dContext->IASetInputLayout(m_pSceneSMLayout);
    }


    // silence truncation and pointer truncation warnings
#pragma warning(push)
#pragma warning(disable : 4302)
#pragma warning(disable : 4311)
    // Render the skinned mesh
    for ( UINT m = 0; m < m_SkinnedMesh.GetNumMeshes(); m++ )
    {

        // Set IA parameters
        pBuffers[0] = m_SkinnedMesh.GetVB11( m, 0 );
        stride[0] = ( UINT )m_SkinnedMesh.GetVertexStride( m, 0 );
        offset[0] = 0;
        pd3dContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
        pd3dContext->IASetIndexBuffer( m_SkinnedMesh.GetIB11( m ), m_SkinnedMesh.GetIBFormat11( m ), 0 );
        pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        // Set shaders
        pd3dContext->VSSetShader( pVS, NULL, 0 );
        pd3dContext->GSSetShader( NULL, NULL, 0 );
        pd3dContext->PSSetShader( pPS, NULL, 0 );

        // Set the bone matrices
        SetBoneMatrices( pd3dContext, m );

        // set the sampler state
        pd3dContext->PSSetSamplers( 0, 1, &m_pLinearClampSampler );

        // enable the depth test
        pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledDSS, 0);

        // Set materials
        SDKMESH_SUBSET* pSubset = NULL;
        SDKMESH_MATERIAL* pMat = NULL;

        for ( UINT subset = 0; subset < m_SkinnedMesh.GetNumSubsets( m ); subset++ )
        {
            pSubset = m_SkinnedMesh.GetSubset( m, subset );

            pMat = m_SkinnedMesh.GetMaterial( pSubset->MaterialID );
            if ( pMat )
            {
                // Set material value for each subset
                D3D11_MAPPED_SUBRESOURCE MappedResource;
                hr = pd3dContext->Map( m_pcbPerMaterial, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
                if (FAILED(hr))
                {
                    ::MessageBoxA(0, "Fail to map constant buffer material", "d3d error", 0);
                }

                CB_PER_MATERIAL* pcbPerMaterial = ( CB_PER_MATERIAL* )MappedResource.pData;

                pcbPerMaterial->m_MatAmbient = XMVectorSet(pMat->Ambient.x, pMat->Ambient.y, pMat->Ambient.z, 1);
                pcbPerMaterial->m_MatDiffuse = XMVectorSet(pMat->Diffuse.x, pMat->Diffuse.y, pMat->Diffuse.z, -1);
                pcbPerMaterial->m_MatSpecular = XMVectorSet(pMat->Specular.x, pMat->Specular.y, pMat->Specular.z, pMat->Power);
                pcbPerMaterial->m_ScalpColor = XMVectorSet(desc.hairParams.color.x, desc.hairParams.color.y, desc.hairParams.color.z, 1.0);

                // Set shader resource views
                    if ( pMat->pDiffuseRV11 && (int)pMat->pDiffuseRV11 != ERROR_RESOURCE_VALUE)
                    {
                        // change pcbPerMaterial->m_MatDiffuse.w to 1
                        //pcbPerMaterial->m_MatDiffuse = XMVectorSet(XMVectorGetX(pcbPerMaterial->m_MatDiffuse),
                        //    XMVectorGetY(pcbPerMaterial->m_MatDiffuse), XMVectorGetZ(pcbPerMaterial->m_MatDiffuse), 1);
                        pcbPerMaterial->m_MatDiffuse = XMVectorSet(1, 1, 1, 1);
                        pd3dContext->PSSetShaderResources(IDSRV_SCENE_DIFFUSE, 1, &pMat->pDiffuseRV11);
                    }
                    else
                    {
                        ID3D11ShaderResourceView* pNULL = NULL;
                        pd3dContext->PSSetShaderResources(IDSRV_SCENE_DIFFUSE, 1, &pNULL);
                    }
                    if ( pMat->pNormalRV11 && (int)pMat->pNormalRV11 != ERROR_RESOURCE_VALUE)
                    {
                        pd3dContext->PSSetShaderResources(IDSRV_SCENE_NORMAL_MAP, 1, &pMat->pNormalRV11);
                    }
                    else
                    {
                        ID3D11ShaderResourceView* pNULL = NULL;
                        pd3dContext->PSSetShaderResources(IDSRV_SCENE_NORMAL_MAP, 1, &pNULL);
                    }
                    if ( pMat->pSpecularRV11 && (int)pMat->pSpecularRV11 != ERROR_RESOURCE_VALUE)
                    {
                        pcbPerMaterial->m_MatSpecular = XMVectorSet(0.5, 0.5, 0.5, 0.5);
                        pd3dContext->PSSetShaderResources(IDSRV_SCENE_SPEC_MASK, 1, &pMat->pSpecularRV11);
                    }
                    else
                    {
                        ID3D11ShaderResourceView* pNULL = NULL;
                        pd3dContext->PSSetShaderResources(IDSRV_SCENE_SPEC_MASK, 1, &pNULL);
                    }

                pd3dContext->Unmap(m_pcbPerMaterial, 0);
                pd3dContext->VSSetConstantBuffers( 2, 1, &m_pcbPerMaterial );
                pd3dContext->PSSetConstantBuffers( 2, 1, &m_pcbPerMaterial );
            }
            pd3dContext->DrawIndexed( ( UINT )pSubset->IndexCount, ( UINT )pSubset->IndexStart,
                                        ( UINT )pSubset->VertexStart );
        }
    }
#pragma warning(pop)

    //clear out the vb bindings for the next pass
    pBuffers[0] = NULL;
    pd3dContext->IASetVertexBuffers( 0, 1, pBuffers, stride, offset );
}

//--------------------------------------------------------------------------------------
//
// RenderScene
//
// Renders the scene and generates the hair shadow for the scene model.
//
//--------------------------------------------------------------------------------------
void CSceneRender::RenderScene(TressFX_Desc & desc, bool shadow)
{

    ID3D11DeviceContext* pd3dContext = desc.pd3dDeviceContext;

    // render the scene shadow
    D3D11_VIEWPORT shadowViewport = {0, 0, (float)SM_SCENE_WIDTH, (float)SM_SCENE_HEIGHT, 0.0f, 1.0f};
    pd3dContext->RSSetViewports(1, &shadowViewport);
    ID3D11ShaderResourceView* pNull = 0;
    pd3dContext->PSSetShaderResources(IDSVR_SCENE_SHADOW_TEXTURE, 1, &pNull);
    ID3D11RenderTargetView* pRTV = DXUTGetD3D11RenderTargetView();
    ID3D11DepthStencilView* pDSV = DXUTGetD3D11DepthStencilView();

    float clearColor[4] = { 1, 1, 1, 1 };
    pd3dContext->ClearRenderTargetView(m_pSceneShadowRTV, clearColor);

    // Create a hair shadow by rendering the hair shadow map into a buffer which will get blurred and projected on the scene
    if (shadow)
    {
        pd3dContext->PSSetShaderResources(IDSRV_SCENESM, 1, &pNull);
        pd3dContext->OMSetRenderTargets(1, &m_pSceneShadowRTV, m_pSMSceneDSV);
        ID3D11ShaderResourceView *pSMHairSRV = desc.pHairShadowMapSRV;
        pd3dContext->PSSetShaderResources(IDSVR_SCENE_SHADOW_TEXTURE, 1, &pSMHairSRV);
        pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledNoWriteDSS, 0x00);
        // render the hair shadow map into the shadow texture
        RenderScreenQuad(pd3dContext, m_pVSScreenQuad, m_pPSRenderSceneShadow);
        pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledDSS, 0x00);

        // blur the shadow texture
        pd3dContext->PSSetSamplers( 0, 1, &m_pPointSampler );
        pd3dContext->PSSetSamplers( 1, 1, &m_pLinearClampSampler );
        pd3dContext->PSSetConstantBuffers( 0, 1, &m_pcbBlur );
        // PS Horizontal filter pass
        pd3dContext->OMSetRenderTargets( 1, &m_pSceneShadowRTV_temp, NULL );
        pd3dContext->PSSetShaderResources(0, 1, &m_pSceneShadowSRV);
        RenderScreenQuad(pd3dContext, m_pVSScreenQuad, m_pHorizontalBlurPS);
        // PS Vertical filter pass
        ID3D11ShaderResourceView *pNullResource = NULL;
        pd3dContext->PSSetShaderResources(0, 1, &pNullResource);
        pd3dContext->OMSetRenderTargets( 1, &m_pSceneShadowRTV, NULL );
        pd3dContext->PSSetShaderResources(0, 1, &m_pSceneShadowSRV_temp);
        RenderScreenQuad(pd3dContext, m_pVSScreenQuad, m_pVerticalBlurPS);
        // restore the render target
        pd3dContext->OMSetRenderTargets(1, &pRTV, pDSV);
        pd3dContext->ClearDepthStencilView( pDSV, D3D11_CLEAR_DEPTH, 1.0f, 0 );
        // restore the samplers
        pd3dContext->PSSetSamplers( 0, 1, &m_pSamplerStateLinearWrap );
        pd3dContext->PSSetSamplers( 1, 1, &m_pSamplerStatePointClamp );
        pd3dContext->PSSetShaderResources(IDSRV_SCENESM, 1, &m_pSMSceneSRV);
    }

    // render scene
    D3D11_VIEWPORT sceneViewport = {0, 0, (float)g_ScreenWidth, (float)g_ScreenHeight, 0.0f, 1.0f};
    pd3dContext->RSSetViewports(1, &sceneViewport);
    pd3dContext->PSSetShaderResources(IDSVR_SCENE_SHADOW_TEXTURE, 1, &m_pSceneShadowSRV);
    ID3D11RasterizerState* pRSTemp;
    pd3dContext->RSGetState( &pRSTemp );
    pd3dContext->RSSetState( m_pRasterizerStateScene );
    if (m_bAnimation)
    {
        RenderSceneGeometry(desc, m_pVSSkinning, m_pPSRenderScene);
    }
    else
    {
        RenderSceneGeometry(desc, m_pVSScene, m_pPSRenderScene);
    }
    pd3dContext->RSSetState( pRSTemp );
    pRSTemp->Release();
}

//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when device is destroyed.
//
//--------------------------------------------------------------------------------------
void CSceneRender::OnDestroy(bool destroyShaders)
{
    // Mesh body
    m_SkinnedMesh.Destroy();

    if (destroyShaders)
    {
        // vs for rendering scene
        SAFE_RELEASE(m_pVSStreamOutVerts);
        SAFE_RELEASE(m_pVSStreamOutSkinned);
        SAFE_RELEASE(m_pVSSkinning);
        SAFE_RELEASE(m_pVSScene);
        // vs/ps for generate hair SM from the light camera.
        SAFE_RELEASE(m_pVSGenerateSceneSM);
        SAFE_RELEASE(m_pVSGenerateSkinnedSM);
        SAFE_RELEASE(m_pPSRenderScene);

        SAFE_RELEASE(m_pVSScreenQuad);

        // PS for generating scene shadow
        SAFE_RELEASE(m_pPSRenderSceneShadow);
        SAFE_RELEASE(m_pHorizontalBlurPS);
        SAFE_RELEASE(m_pVerticalBlurPS);

        // GS for stream out
        SAFE_RELEASE(m_pGSStreamOut);
        SAFE_RELEASE(m_pGSSkinStreamOut);

        // vertex layout for scene
        SAFE_RELEASE(m_pStreamOutLayout);
        SAFE_RELEASE(m_pStreamOutLayoutSkinned);
        SAFE_RELEASE(m_pSkinningLayout);
        SAFE_RELEASE(m_pScenelayout);
        SAFE_RELEASE(m_pSceneSMLayout);
        SAFE_RELEASE(m_pLayoutSceneSM);
        SAFE_RELEASE(m_pQuadLayout);
    }

    // delete CPU side memory
    SAFE_DELETE_ARRAY(m_pMeshIndexCount);

    // vertex buffer
    SAFE_RELEASE(m_pScreenQuadVB);

    // bone matrix buffer
    SAFE_RELEASE(m_pBoneBuffer);
    SAFE_RELEASE(m_pBoneBufferRV);

    // constant buffers
    SAFE_RELEASE(m_pcbBlur);
    SAFE_RELEASE(m_pcbPerMaterial);
    SAFE_RELEASE(m_pcbPerFrame);

    // Scene shadow map depth stencil buffer
    SAFE_RELEASE(m_pSMSceneTx);
    SAFE_RELEASE(m_pSMSceneDSV);
    SAFE_RELEASE(m_pSMSceneSRV);

    // scene shadow
    SAFE_RELEASE(m_pSceneShadowTexture);
    SAFE_RELEASE(m_pSceneShadowRTV);
    SAFE_RELEASE(m_pSceneShadowSRV);
    SAFE_RELEASE(m_pSceneShadowTexture_temp);
    SAFE_RELEASE(m_pSceneShadowRTV_temp);
    SAFE_RELEASE(m_pSceneShadowSRV_temp);

    // render states
    SAFE_RELEASE(m_pRasterizerStateShadowmap);
    SAFE_RELEASE(m_pRasterizerStateScene);
    SAFE_RELEASE( m_pDepthTestEnabledDSS );
    SAFE_RELEASE( m_pDepthTestEnabledNoWriteDSS );
    SAFE_RELEASE( m_pDepthStencilDisableState );
    SAFE_RELEASE(m_pSamplerStateLinearWrap );
    SAFE_RELEASE(m_pSamplerStatePointClamp );
    SAFE_RELEASE(m_pLinearClampSampler);
    SAFE_RELEASE(m_pPointSampler);

    SAFE_RELEASE(m_pMeshVertices);
    SAFE_RELEASE(m_pMeshVerticesSRV);
    SAFE_RELEASE(m_pTransformedVerts);
    SAFE_RELEASE(m_pTransformedSRV);
}
