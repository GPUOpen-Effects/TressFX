//--------------------------------------------------------------------------------------
// File: TressFXRenderer.cpp
//
// Main hair rendering code
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
#include "TressFXOpaque.h"
#include "TressFXRenderer.h"
#include "TressFXPrecompiledShaders.h"

#ifndef AMD_V_RETURN
#define AMD_V_RETURN(x)    { hr = (x); if ( FAILED(hr) ) { return hr; } }
#endif

// unreferenced formal parameter
#pragma warning(disable: 4100)

using namespace DirectX;

extern int g_TressFXNumVerticesPerStrand;

struct CB_PER_FRAME
{
    XMMATRIX    m_mWorld;
    XMMATRIX    m_mViewProj;
    XMMATRIX    m_mInvViewProj;
    XMMATRIX    m_mViewProjLight;

    XMFLOAT3    m_vEye;
    float       m_fvFOV;

    XMFLOAT4    m_AmbientLightColor;
    XMFLOAT4    m_PointLightColor;
    XMFLOAT4    m_PointLightPos;
    XMFLOAT4    m_MatBaseColor;
    XMFLOAT4    m_MatKValue;

    float       m_FiberAlpha;
    float       m_HairSMAlpha;
    float       m_bExpandPixels;
    float       m_FiberRadius;

    XMFLOAT4    m_WinSize;

    float       m_FiberSpacing;
    float       m_bThinTip;
    float       m_fNearLight;
    float       m_fFarLight;

    int         m_iTechSM;
    int         m_bUseCoverage;
    int         m_iStrandCopies;
    int         m_iMaxFragments;

    float       m_alphaThreshold;
    float       m_fHairKs2;
    float       m_fHairEx2;
    unsigned    m_optionalSRVs;

    XMMATRIX    m_mInvViewProjViewport;

    int         m_mNumVerticesPerStrand;
    int         m_mNumFollowHairsPerGuideHair;
    int         m_bSingleHeadTransform;
    int         padding0;
};

// Optional SRVs
#define PER_STRAND_TEX_COORDS   0x0001
#define PER_VERTEX_TEX_COORDS   0x0002
#define PER_VERTEX_COLORS       0x0004

struct PER_PIXEL_LINKED_LIST_STRUCT
{
    DWORD           TangentAndCoverage;
    DWORD           depth;
    DWORD           strandColor;
    DWORD           dwNext;
};

struct PPLL_BUFFERS
{
    // Buffers for the head of the per-pixel linked lists (PPLL)
    ID3D11Texture2D*            pHeadPPLL_Buffer;
    ID3D11ShaderResourceView*   pHeadPPLL_SRV;
    ID3D11UnorderedAccessView*  pHeadPPLL_UAV;

    // Buffers for the per-pixel linked list (PPLL) data
    ID3D11Buffer*               pPPLL_Buffer;
    ID3D11UnorderedAccessView*  pPPLL_UAV;
    ID3D11ShaderResourceView*   pPPLL_SRV;

    int                         width;
    int                         height;
    int                         refCount;    // reference count - delete buffers when 0
};

PPLL_BUFFERS g_PPLBuffers = {NULL, NULL, NULL, NULL, NULL, NULL, 0, 0, 0};


const static UINT           g_HairTotalLayers = 32;

namespace AMD
{

//--------------------------------------------------------------------------------------
//
// CreateShaderAndLayout
//
// Creates the shaders and layouts for hair rendering
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::CreateShaderAndLayout(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    // Hair vertex layout
    const D3D11_INPUT_ELEMENT_DESC layout_hair[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,    0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT,    0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // Full screen quad layout structure
    const D3D11_INPUT_ELEMENT_DESC layout_quad[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
        { "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    // create the input layouts
    AMD_V_RETURN(pd3dDevice->CreateInputLayout(layout_hair, ARRAYSIZE(layout_hair), VS_RenderHair_Data, sizeof(VS_RenderHair_Data), &m_pLayoutHair));
    AMD_V_RETURN(pd3dDevice->CreateInputLayout(layout_quad, ARRAYSIZE(layout_quad), VS_ScreenQuad_Data, sizeof(VS_ScreenQuad_Data), &m_pLayoutQuad));

    // vertex shaders
    AMD_V_RETURN(pd3dDevice->CreateVertexShader(VS_RenderHair_Data, sizeof(VS_RenderHair_Data), NULL, &m_pVSRenderHair));
    AMD_V_RETURN(pd3dDevice->CreateVertexShader(VS_RenderHair_AA_Data, sizeof(VS_RenderHair_AA_Data), NULL, &m_pVSRenderHairAA));
    AMD_V_RETURN(pd3dDevice->CreateVertexShader(VS_GenerateHairSM_Data, sizeof(VS_GenerateHairSM_Data), NULL, &m_pVSGenerateHairSM));
    AMD_V_RETURN(pd3dDevice->CreateVertexShader(VS_RenderHair_StrandCopies_Data, sizeof(VS_RenderHair_StrandCopies_Data), NULL, &m_pVSRenderHairStrandCopies));
    AMD_V_RETURN(pd3dDevice->CreateVertexShader(VS_RenderHair_AA_StrandCopies_Data, sizeof(VS_RenderHair_AA_StrandCopies_Data), NULL, &m_pVSRenderHairAAStrandCopies));
    AMD_V_RETURN(pd3dDevice->CreateVertexShader(VS_ScreenQuad_Data, sizeof(VS_ScreenQuad_Data), NULL, &m_pVSScreenQuad));

    // pixel shaders
    AMD_V_RETURN(pd3dDevice->CreatePixelShader(PS_ABuffer_Hair_Data, sizeof(PS_ABuffer_Hair_Data), NULL, &m_pPSABuffer_Hair));
    AMD_V_RETURN(pd3dDevice->CreatePixelShader(PS_KBuffer_Hair_Data, sizeof(PS_KBuffer_Hair_Data), NULL, &m_pPSKBuffer_Hair));

    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// CreateTextureAndViews
//
// Creates the textures and corresponding views for hair rendering
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::CreateTextureAndViews( ID3D11Device* pd3dDevice )
{
    HRESULT hr;

    // Create SM DSVs for hair
    D3D11_TEXTURE2D_DESC tex2D_desc;
    tex2D_desc.Width =  SM_HAIR_WIDTH;
    tex2D_desc.Height = SM_HAIR_WIDTH;
    tex2D_desc.ArraySize = 1;
    tex2D_desc.Format = DXGI_FORMAT_R32_TYPELESS;
    tex2D_desc.Usage = D3D11_USAGE_DEFAULT;
    tex2D_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL|D3D11_BIND_SHADER_RESOURCE;
    tex2D_desc.CPUAccessFlags = 0;
    tex2D_desc.MipLevels = 0;
    tex2D_desc.MiscFlags = 0;
    tex2D_desc.SampleDesc.Count = 1;
    tex2D_desc.SampleDesc.Quality = 0;

    AMD_V_RETURN(pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pSMHairTx));

    D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc;
    dsvDesc.Flags = 0;
    dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
    dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    dsvDesc.Texture2D.MipSlice = 0;
    AMD_V_RETURN(pd3dDevice->CreateDepthStencilView(m_pSMHairTx, &dsvDesc, &m_pSMHairDSV));

    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
    srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srvDesc.Texture2D.MipLevels = 1;
    srvDesc.Texture2D.MostDetailedMip = 0;
    AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_pSMHairTx, &srvDesc, &m_pSMHairSRV));

    // Noise texture
    D3D11_TEXTURE2D_DESC texDesc;
    texDesc.Width = 512;
    texDesc.Height = 512;
    texDesc.MipLevels = 1;
    texDesc.ArraySize = 1;
    texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    texDesc.Usage = D3D11_USAGE_DEFAULT;
    texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    texDesc.MiscFlags = 0;
    texDesc.CPUAccessFlags = 0;
    texDesc.SampleDesc.Count=1;
    texDesc.SampleDesc.Quality=0;

    XMFLOAT4* noiseArray = new XMFLOAT4[texDesc.Width*texDesc.Height];
    for (UINT i = 0; i < texDesc.Width*texDesc.Height; i++)
    {
        noiseArray[i].x = rand()/(float)RAND_MAX;
        noiseArray[i].y = rand()/(float)RAND_MAX;
        noiseArray[i].z = rand()/(float)RAND_MAX;
        noiseArray[i].w = rand()/(float)RAND_MAX;
    }

    D3D11_SUBRESOURCE_DATA initData0;
    initData0.pSysMem = noiseArray;
    initData0.SysMemPitch = texDesc.Width*sizeof(XMFLOAT4);
    initData0.SysMemSlicePitch = 0;

    hr = pd3dDevice->CreateTexture2D(&texDesc, &initData0, &m_pNoiseTexture2D);
    AMD_SAFE_DELETE_ARRAY(noiseArray);
    AMD_V_RETURN(hr);

    D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
    srDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
    srDesc.Texture2D.MipLevels = (UINT)(-1);
    srDesc.Texture2D.MostDetailedMip = 0;

    AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_pNoiseTexture2D, &srDesc, &m_pNoiseSRV));

    return S_OK;
}


//--------------------------------------------------------------------------------------
//
// CreateVertexBuffers
//
// Creates the vertex buffers for hair rendering
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::CreateVertexBuffers(ID3D11Device* pd3dDevice)
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
    AMD_V_RETURN(pd3dDevice->CreateBuffer(&bd, &initData, &m_pScreenQuadVB));

    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// CreateConstantBuffers
//
// Creates the constant buffers for hair rendering
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::CreateConstantBuffer(ID3D11Device* pd3dDevice)
{
    HRESULT hr;
    D3D11_BUFFER_DESC cbDesc;
    ZeroMemory( &cbDesc, sizeof(cbDesc) );
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

    cbDesc.ByteWidth = sizeof( CB_PER_FRAME );
    AMD_V_RETURN(pd3dDevice->CreateBuffer(&cbDesc, NULL, &m_pcbPerFrame));

    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// CreateRenderStateObjects
//
// Creates the render state objects for hair rendering
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::CreateRenderStateObjects(ID3D11Device* pd3dDevice)
{
    HRESULT hr;

    // Create depth stencil states
    D3D11_DEPTH_STENCIL_DESC DSDesc;
    DSDesc.DepthEnable                  = TRUE;
    DSDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
    DSDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ALL;
    DSDesc.StencilEnable                = FALSE;
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
    AMD_V_RETURN(pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestEnabledDSS));

    DSDesc.DepthEnable                  = TRUE;
    DSDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
    DSDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
    DSDesc.StencilEnable                = TRUE;
    DSDesc.StencilReadMask              = 0xFF;
    DSDesc.StencilWriteMask             = 0xFF;
    DSDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_INCR_SAT;
    DSDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;
    DSDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_INCR_SAT;
    DSDesc.BackFace.StencilFunc         = D3D11_COMPARISON_ALWAYS;
    AMD_V_RETURN(hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestEnabledNoDepthWritesStencilWriteIncrementDSS));

    DSDesc.DepthEnable                  = FALSE;
    DSDesc.DepthFunc                    = D3D11_COMPARISON_LESS_EQUAL;
    DSDesc.DepthWriteMask               = D3D11_DEPTH_WRITE_MASK_ZERO;
    DSDesc.StencilEnable                = TRUE;
    DSDesc.StencilReadMask              = 0xFF;
    DSDesc.StencilWriteMask             = 0x00;
    DSDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;
    DSDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_LESS;
    DSDesc.BackFace.StencilFailOp       = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilDepthFailOp  = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilPassOp       = D3D11_STENCIL_OP_KEEP;
    DSDesc.BackFace.StencilFunc         = D3D11_COMPARISON_LESS;
    AMD_V_RETURN(hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestDisabledStencilTestLessDSS));

    // Create sampler state objects
    D3D11_SAMPLER_DESC samDesc;
    ZeroMemory( &samDesc, sizeof(samDesc) );
    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samDesc.AddressU = samDesc.AddressV = samDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samDesc.MaxAnisotropy = 16;
    samDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    samDesc.MinLOD = 0;
    samDesc.MaxLOD = D3D11_FLOAT32_MAX;
    AMD_V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &m_pSamplerStateLinearWrap));

    samDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
    AMD_V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &m_pSamplerStatePointClamp));

    samDesc.Filter = D3D11_FILTER_COMPARISON_MIN_MAG_MIP_LINEAR;
    samDesc.ComparisonFunc = D3D11_COMPARISON_LESS;
    samDesc.BorderColor[0] = samDesc.BorderColor[1] =
        samDesc.BorderColor[2] = samDesc.BorderColor[3] = 1;
    samDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
    samDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
    samDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
    AMD_V_RETURN(pd3dDevice->CreateSamplerState(&samDesc, &m_pSamplerStateCmpLess));

    // Create blend state objects
    D3D11_BLEND_DESC blendDesc;
    ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));
    blendDesc.IndependentBlendEnable = false;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
    blendDesc.AlphaToCoverageEnable = false;
    blendDesc.RenderTarget[0].BlendEnable = true;
    blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
    blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO; //D3D11_BLEND_SRC_ALPHA;
    blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
    AMD_V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &m_pBlendStateBlendToBg));

    // Create a blend state to disable color writes
    blendDesc.RenderTarget[0].SrcBlend  = D3D11_BLEND_ONE;
    blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
    blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
    AMD_V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &m_pColorWritesOff));

    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// OnCreateDevice
//
// Called when the device is created to create resources for hair rendering
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::OnCreateDevice(ID3D11Device* pd3dDevice, int winWidth, int winHeight, bool bShortCutOn)
{
    HRESULT hr;

    AMD_V_RETURN(CreateShaderAndLayout(pd3dDevice));
    AMD_V_RETURN(CreateTextureAndViews(pd3dDevice));
    AMD_V_RETURN(CreateConstantBuffer(pd3dDevice));
    AMD_V_RETURN(CreateVertexBuffers(pd3dDevice));
    AMD_V_RETURN(CreateRenderStateObjects(pd3dDevice));

    if (bShortCutOn)
    {
        m_ShortCut.OnCreateDevice(pd3dDevice, winWidth, winHeight);
    }
    else
    {
        AMD_V_RETURN(CreatePPLL(pd3dDevice, winWidth, winHeight, false));
    }

    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// CreatePPLL
//
// Creates the per pixel linked list buffers and views. To save space, the PPLL is
// shared between multiple TressFXRenderer objects. The ref count gets incremented
// when the PPLL is shared with a new TressFXRenderer object.
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::CreatePPLL(ID3D11Device* pd3dDevice, int winWidth, int winHeight, bool resize)
{
    HRESULT hr;

    // see if the buffer needs to be resized or if refCount is 0
    if ((winWidth != g_PPLBuffers.width) || (winHeight != g_PPLBuffers.height) || (g_PPLBuffers.refCount == 0))
    {
        // Release any previously allocated buffers
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_Buffer);
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_SRV);
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_UAV);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_Buffer);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_UAV);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_SRV);

        // linked list head texture
        D3D11_TEXTURE2D_DESC texDesc;
        texDesc.Width = winWidth;
        texDesc.Height = winHeight;
        texDesc.MipLevels = 1;
        texDesc.ArraySize = 1;
        texDesc.Format = DXGI_FORMAT_R32_UINT;
        texDesc.Usage = D3D11_USAGE_DEFAULT;
        texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        texDesc.MiscFlags = 0;
        texDesc.CPUAccessFlags = 0;
        texDesc.SampleDesc.Count=1;
        texDesc.SampleDesc.Quality=0;
        AMD_V_RETURN(pd3dDevice->CreateTexture2D(&texDesc, NULL, &g_PPLBuffers.pHeadPPLL_Buffer));

        // SRV for linked list head
        D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
        srDesc.Format = DXGI_FORMAT_R32_UINT;
        srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srDesc.Texture2D.MipLevels = 1;
        srDesc.Texture2D.MostDetailedMip = 0;
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(g_PPLBuffers.pHeadPPLL_Buffer, &srDesc, &g_PPLBuffers.pHeadPPLL_SRV));

        // Create UAV view of the buffer containing the heads of the per-pixel linked lists
        D3D11_UNORDERED_ACCESS_VIEW_DESC UAVHeadPPLLBufferDesc;
        UAVHeadPPLLBufferDesc.Format              = DXGI_FORMAT_R32_UINT;
        UAVHeadPPLLBufferDesc.ViewDimension       = D3D11_UAV_DIMENSION_TEXTURE2D;
        UAVHeadPPLLBufferDesc.Texture2D.MipSlice  = 0;
        AMD_V_RETURN(pd3dDevice->CreateUnorderedAccessView(g_PPLBuffers.pHeadPPLL_Buffer, &UAVHeadPPLLBufferDesc, &g_PPLBuffers.pHeadPPLL_UAV));

        // Per-pixel Linked List (PPLL) buffer
        D3D11_BUFFER_DESC BufferDesc;
        BufferDesc.BindFlags               = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        BufferDesc.ByteWidth               = (DWORD)(g_HairTotalLayers * winWidth * winHeight * sizeof(PER_PIXEL_LINKED_LIST_STRUCT) );
        BufferDesc.CPUAccessFlags          = 0;
        BufferDesc.MiscFlags               = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        BufferDesc.Usage                   = D3D11_USAGE_DEFAULT;
        BufferDesc.StructureByteStride     = sizeof(PER_PIXEL_LINKED_LIST_STRUCT);
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&BufferDesc, NULL, &g_PPLBuffers.pPPLL_Buffer));

        // Create UAV view of Fragment and Link Buffer
        D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
        UAVDesc.Format                = DXGI_FORMAT_UNKNOWN;
        UAVDesc.ViewDimension         = D3D11_UAV_DIMENSION_BUFFER;
        UAVDesc.Buffer.FirstElement   = 0;
        UAVDesc.Buffer.NumElements    = (DWORD)(g_HairTotalLayers * winWidth * winHeight);
        UAVDesc.Buffer.Flags          = D3D11_BUFFER_UAV_FLAG_COUNTER;
        AMD_V_RETURN(pd3dDevice->CreateUnorderedAccessView(g_PPLBuffers.pPPLL_Buffer, &UAVDesc, &g_PPLBuffers.pPPLL_UAV));

        // Create SRV view of Fragment and Link Buffer
        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        SRVDesc.Format                = DXGI_FORMAT_UNKNOWN;
        SRVDesc.ViewDimension         = D3D11_SRV_DIMENSION_BUFFER;
        SRVDesc.Buffer.ElementOffset  = 0;
        SRVDesc.Buffer.ElementWidth   = (DWORD)(g_HairTotalLayers * winWidth * winHeight);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(g_PPLBuffers.pPPLL_Buffer, &SRVDesc, &g_PPLBuffers.pPPLL_SRV));

        // update the width and height
        g_PPLBuffers.width = winWidth;
        g_PPLBuffers.height = winHeight;

        // if the refCount is non-zero, then we're just resizing the buffers
        if (g_PPLBuffers.refCount == 0)
        {
            g_PPLBuffers.refCount++;
        }
    }
    else
    {
        if (!resize)
        {
            g_PPLBuffers.refCount++;
        }
    }

    m_pHeadPPLL_Buffer = g_PPLBuffers.pHeadPPLL_Buffer;
    m_pHeadPPLL_SRV = g_PPLBuffers.pHeadPPLL_SRV;
    m_pHeadPPLL_UAV = g_PPLBuffers.pHeadPPLL_UAV;
    m_pPPLL_Buffer = g_PPLBuffers.pPPLL_Buffer;
    m_pPPLL_UAV = g_PPLBuffers.pPPLL_UAV;
    m_pPPLL_SRV = g_PPLBuffers.pPPLL_SRV;

    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// DeletePPLL
//
// Deletes the PPLL buffers when the refCount goes to 0
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::DeletePPLL()
{
    if (g_PPLBuffers.refCount == 0)
    {
        return;
    }

    g_PPLBuffers.refCount--;

    m_pHeadPPLL_Buffer = NULL;
    m_pHeadPPLL_SRV = NULL;
    m_pHeadPPLL_UAV = NULL;
    m_pPPLL_Buffer = NULL;
    m_pPPLL_UAV = NULL;
    m_pPPLL_SRV = NULL;

    if (g_PPLBuffers.refCount == 0)
    {
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_Buffer);
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_SRV);
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_UAV);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_Buffer);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_UAV);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_SRV);

        g_PPLBuffers.width = 0;
        g_PPLBuffers.height = 0;
    }
}

//--------------------------------------------------------------------------------------
//
// OnResizedSwapChain
//
// Called when the swap chain is being resized. Allocate resources that need to be
// created at this time because they are dependent on the size of the frame buffer.
//
//--------------------------------------------------------------------------------------
HRESULT TressFXRenderer::OnResizedSwapChain( ID3D11Device* pd3dDevice, int width, int height, bool bShortCutOn )
{
    HRESULT hr;
    if (bShortCutOn)
    {
        AMD_V_RETURN(m_ShortCut.OnResizedSwapChain(pd3dDevice, width, height));
    }
    else
    {
        AMD_V_RETURN(CreatePPLL(pd3dDevice, width, height, true));
    }
    return S_OK;
}

//--------------------------------------------------------------------------------------
//
// BeginHairFrame
//
// Start of hair rendering.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::BeginHairFrame(ID3D11DeviceContext* pd3dContext,
                                    DirectX::XMVECTOR eyePoint, DirectX::XMVECTOR lightPosition,
                                    DirectX::XMMATRIX *pModelTransformForHead,  DirectX::XMMATRIX *pViewProj,  DirectX::XMMATRIX *pViewProjLightOut,
                                    float screenWidth, float screenHeight, bool singleHeadTransform)
{
    SetSamplerStates(pd3dContext);

    // Set up camera parameters for when the camera is at the position of the light for rendering the shadow map
    XMMATRIX mViewLight, mProjLight;
    XMVECTOR modelCenter = XMVector3TransformCoord(XMLoadFloat3(&m_pTressFXMesh->m_HairAsset.m_bSphere.center), *pModelTransformForHead);
    XMVECTOR vLightAt = modelCenter;
    XMVECTOR vUp = XMVectorSet(0, 1, 0, 0);
    mViewLight = XMMatrixLookAtLH(lightPosition, vLightAt, vUp);

    XMVECTOR vLightToObject = XMVectorSubtract(lightPosition, modelCenter);
    float dis = XMVectorGetX(XMVector3Length(vLightToObject));

    float min_dis = max(0.001f, dis - m_pTressFXMesh->m_HairAsset.m_bSphere.radius);
    float max_dis = dis + m_pTressFXMesh->m_HairAsset.m_bSphere.radius;

    float halfAngle = 1.5f*asin(m_pTressFXMesh->m_HairAsset.m_bSphere.radius/dis);
    float FOV = 2*halfAngle;

    float ratio = 1;

    mProjLight = XMMatrixPerspectiveFovLH( FOV, ratio, min_dis, max_dis );
    *pViewProjLightOut = mViewLight * mProjLight;

    // Map the per-frame constant buffer
    D3D11_MAPPED_SUBRESOURCE MappedResource;
    pd3dContext->Map( m_pcbPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource );
    CB_PER_FRAME* pcbPerFrame = ( CB_PER_FRAME* )MappedResource.pData;

    // camera parameters
    XMMATRIX mViewProj = *pViewProj;
    XMMATRIX mInvViewProj = XMMatrixInverse(0, mViewProj);

    float fRenderWidth =  screenWidth;
    float fRenderHeight = screenHeight;

    // Inverse of viewprojection matrix with viewport mapping
    XMMATRIX mViewport  ( 2.0f/fRenderWidth,  0.0f,                   0.0f, 0.0f,
                            0.0f,               -2.0f/fRenderHeight,    0.0f, 0.0f,
                            0.0f,               0.0f,                   1.0f, 0.0f,
                            -1.0f,              1.0f,                   0.0f, 1.0f );

    XMMATRIX mInvViewProjViewport = mViewport * mInvViewProj;

    pcbPerFrame->m_mViewProj = XMMatrixTranspose(mViewProj);
    pcbPerFrame->m_mInvViewProj = XMMatrixTranspose(mInvViewProj);

    pcbPerFrame->m_mInvViewProjViewport = XMMatrixTranspose(mInvViewProjViewport);

    pcbPerFrame->m_mWorld = XMMatrixTranspose(*pModelTransformForHead);

    XMStoreFloat3(&pcbPerFrame->m_vEye, eyePoint);

    pcbPerFrame->m_fvFOV = XM_PI/4;

    // Light camera parameters
    pcbPerFrame->m_mViewProjLight  = XMMatrixTranspose(*pViewProjLightOut);
    pcbPerFrame->m_fNearLight = min_dis;
    pcbPerFrame->m_fFarLight = max_dis;

    XMStoreFloat4(&pcbPerFrame->m_PointLightPos, lightPosition);
    pcbPerFrame->m_PointLightPos.w = 1;

    // scene light color
    pcbPerFrame->m_AmbientLightColor = m_hairParams.ambientLightColor;
    pcbPerFrame->m_PointLightColor = m_hairParams.pointLightColor;

    // hair material
    pcbPerFrame->m_MatBaseColor =  XMFLOAT4(m_hairParams.color.x, m_hairParams.color.y, m_hairParams.color.z, 1);
    pcbPerFrame->m_MatKValue = XMFLOAT4(m_hairParams.Ka, m_hairParams.Kd, m_hairParams.Ks1, m_hairParams.Ex1);
    pcbPerFrame->m_fHairKs2 = m_hairParams.Ks2;
    pcbPerFrame->m_fHairEx2 = m_hairParams.Ex2;

    pcbPerFrame->m_FiberAlpha = m_hairParams.alpha;
    pcbPerFrame->m_HairSMAlpha = m_hairParams.shadowMapAlpha;

    pcbPerFrame->m_FiberRadius = m_hairParams.thickness;

    pcbPerFrame->m_FiberSpacing = m_hairParams.duplicateStrandSpacing;

    pcbPerFrame->m_bThinTip = (m_hairParams.bThinTip ? 1.f : -1.f);
    pcbPerFrame->m_bExpandPixels = 1;

    pcbPerFrame->m_WinSize = XMFLOAT4((float)screenWidth, (float)screenHeight, 1.0f/(float)screenWidth, 1.0f/(float)screenHeight);

    pcbPerFrame->m_iMaxFragments = m_hairParams.maxFragments;

    pcbPerFrame->m_alphaThreshold = m_hairParams.alphaThreshold;

    pcbPerFrame->m_iTechSM = m_hairParams.shadowTechnique;
    pcbPerFrame->m_bUseCoverage = m_hairParams.bAntialias ? 1 : 0;
    pcbPerFrame->m_iStrandCopies = m_hairParams.strandCopies;

    pcbPerFrame->m_mNumVerticesPerStrand = g_TressFXNumVerticesPerStrand;
    pcbPerFrame->m_mNumFollowHairsPerGuideHair = m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair;
    pcbPerFrame->m_bSingleHeadTransform = singleHeadTransform;

    unsigned optionalSRVs = 0;
    if ((m_pTressFXMesh->m_pStrandTexCoordSRV) && (m_pTressFXMesh->m_pHairTextureSRV))
    {
        optionalSRVs |= PER_STRAND_TEX_COORDS;
        // ignore the material base color when getting hair color from the texture
        pcbPerFrame->m_MatBaseColor =  XMFLOAT4(1, 1, 1, 1);
    }
    pcbPerFrame->m_optionalSRVs = optionalSRVs;

    pd3dContext->Unmap( m_pcbPerFrame, 0 );

    // Set constant buffer for vertex and pixel shader
    pd3dContext->VSSetConstantBuffers( 1, 1, &m_pcbPerFrame );
    pd3dContext->PSSetConstantBuffers( 1, 1, &m_pcbPerFrame );
}

//--------------------------------------------------------------------------------------
//
// SetSamplerStates
//
// Sets the sampler states for hair rendering
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::SetSamplerStates( ID3D11DeviceContext* pd3dContext )
{
    // Set texture sampler states
    ID3D11SamplerState* pSS[3];
    pSS[0] = m_pSamplerStateLinearWrap;
    pSS[1] = m_pSamplerStatePointClamp;
    pSS[2] = m_pSamplerStateCmpLess;
    pd3dContext->PSSetSamplers(0, 3, pSS);

    // Set vertex texture sampler states
    // If PER_STRAND_TEX_COORDS has not been specified, the linear wrap sampler will
    // not be actually used in vertex shader. However, the sampler slot will still
    // exist in the vertex shader, so we always set the linear wrap sampler to avoid
    // SAMPLER_NOT_SET warning from d3d debug layer.
    ID3D11SamplerState* vSS[1];
    vSS[0] = m_pSamplerStateLinearWrap;
    pd3dContext->VSSetSamplers(0, 1, vSS);
}

//--------------------------------------------------------------------------------------
//
// RenderHairGeometry
//
// Renders the hair geometry as individual strands of hair
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::RenderHairGeometry( ID3D11DeviceContext* pd3dContext,
                                ID3D11VertexShader* pVS,
                                ID3D11PixelShader* pPS,
                                float density,
                                bool useLinePrimitives /*= true*/,
                                int iStrandCopies /*= 1*/)
{

    pd3dContext->VSSetShader( pVS, NULL, 0 );
    pd3dContext->PSSetShader( pPS, NULL, 0 );

    pd3dContext->PSSetShaderResources( IDSRV_NOISEMAP, 1, &m_pNoiseSRV );
    pd3dContext->VSSetShaderResources( IDSRV_NOISEMAP, 1, &m_pNoiseSRV );
    pd3dContext->VSSetShaderResources( IDSRV_HAIR_VERTEX_POSITIONS, 1, &m_pTressFXMesh->m_HairVertexPositionsSRV );
    pd3dContext->VSSetShaderResources( IDSRV_HAIR_TRANSFORMS, 1, &m_pTressFXMesh->m_HairTransformsSRV );
    pd3dContext->VSSetShaderResources( IDSRV_HAIR_TANGENTS, 1, &m_pTressFXMesh->m_HairVertexTangentsSRV );

    if (useLinePrimitives)
    {
        // render line primitives for the shadow map
        pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_LINELIST );

        UINT stride = 0;
        UINT offset = 0;
        pd3dContext->IASetInputLayout( 0 );
        ID3D11Buffer* buf[]={0};
        pd3dContext->IASetVertexBuffers( 0, 1, buf, &stride, &offset );
        pd3dContext->IASetIndexBuffer( m_pTressFXMesh->m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

        pd3dContext->DrawIndexed(UINT( density * m_pTressFXMesh->m_TotalIndexCount), 0, 0 );
    }
    else
    {
        // triangle rendering
        pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

        UINT stride = 0;
        UINT offset = 0;
        pd3dContext->IASetInputLayout( 0 );
        ID3D11Buffer* buf[]={0};
        pd3dContext->IASetVertexBuffers( 0, 1, buf, &stride, &offset );
        pd3dContext->IASetIndexBuffer( m_pTressFXMesh->m_pTriangleIndexBuffer, DXGI_FORMAT_R32_UINT, 0 );

        pd3dContext->VSSetShaderResources( IDSRV_HAIR_THICKNESSES, 1, &m_pTressFXMesh->m_pThicknessCoeffsSRV);
        if (m_pTressFXMesh->m_pStrandTexCoordSRV != NULL)
        {
            pd3dContext->VSSetShaderResources( IDSRV_HAIR_STRAND_TEX_COORDS, 1, &m_pTressFXMesh->m_pStrandTexCoordSRV);
            pd3dContext->VSSetShaderResources( IDSRV_HAIR_COLOR_TEXTURE, 1, &m_pTressFXMesh->m_pHairTextureSRV);
        }

        pd3dContext->DrawIndexedInstanced(UINT( density * m_pTressFXMesh->m_TotalTriangleIndexCount), iStrandCopies, 0, 0, 0);  // If iStrandCopies>1 a different VS is set
    }

    ID3D11ShaderResourceView* nullViews[] = { NULL };
    pd3dContext->VSSetShaderResources( IDSRV_HAIR_VERTEX_POSITIONS, 1, nullViews );
    pd3dContext->VSSetShaderResources( IDSRV_HAIR_TRANSFORMS, 1, nullViews );
    pd3dContext->VSSetShaderResources( IDSRV_HAIR_TANGENTS, 1, nullViews );
}

//--------------------------------------------------------------------------------------
//
// RenderScreenQuad
//
// Renders a full screen quad
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::RenderScreenQuad(ID3D11DeviceContext* pd3dContext,
    ID3D11VertexShader* pVS, ID3D11PixelShader* pPS)
{
    // set shader
    pd3dContext->VSSetShader( pVS, NULL, 0 );
    pd3dContext->PSSetShader( pPS, NULL, 0 );

    UINT strides = sizeof( StandardVertex );
    UINT offsets = 0;
    pd3dContext->IASetInputLayout( m_pLayoutQuad );
    pd3dContext->IASetVertexBuffers( 0, 1, &m_pScreenQuadVB, &strides, &offsets );
    pd3dContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Draw full screen quad
    pd3dContext->Draw( 6, 0);
}


//--------------------------------------------------------------------------------------
//
// GenerateShadowMap
//
// Renders the hair from the point of view of the light into a shadow map
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::GenerateShadowMap(ID3D11DeviceContext* pd3dContext, float density,
                        float screenWidth, float screenHeight)
{
    // Get original render target and depth stencil view
    ID3D11RenderTargetView* pRTV = NULL;
    ID3D11DepthStencilView* pDSV = NULL;
    pd3dContext->OMGetRenderTargets( 1, &pRTV, &pDSV );

    D3D11_VIEWPORT viewportSMHair = {0, 0, SM_HAIR_WIDTH, SM_HAIR_HEIGHT, 0.0f, 1.0f};
    pd3dContext->RSSetViewports( 1, &viewportSMHair );
    // clear depth for early z
    pd3dContext->ClearDepthStencilView(m_pSMHairDSV, D3D11_CLEAR_DEPTH|D3D10_CLEAR_STENCIL, 1.0, 0);
    // set render target to shadow map texture
    pd3dContext->OMSetRenderTargets(0, 0, m_pSMHairDSV);
    RenderHairGeometry(pd3dContext, m_pVSGenerateHairSM, NULL, density);

    // reset view port
    D3D11_VIEWPORT viewportWin = {0, 0, (float)screenWidth, (float)screenHeight, 0.0f, 1.0f};
    pd3dContext->RSSetViewports(1, &viewportWin);

    // reset render targets
    pd3dContext->OMSetRenderTargets(1, &pRTV, pDSV);
    AMD_SAFE_RELEASE( pRTV );
    AMD_SAFE_RELEASE( pDSV );

    // set the hair shadow map shader resource view
    pd3dContext->PSSetShaderResources(IDSRV_HAIRSM, 1, &m_pSMHairSRV);
}


//--------------------------------------------------------------------------------------
//
// RenderHair
//
// Renders the hair in two passes. The first pass fills an A-buffer by rendering the
// hair geometry into a per-pixel linked list which keeps all of the overlapping fragments.
// The second pass renders a full screen quad (using a stencil mask set in the first pass
// to avoid unnecessary pixels) which reads fragments from the per-pixel linked list
// and blends the nearest k fragments (K-buffer) in back to front order.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::RenderHair(ID3D11DeviceContext* pd3dContext)
{
    // Get original render target and depth stencil view
    ID3D11RenderTargetView* pRTV = NULL;
    ID3D11DepthStencilView* pDSV = NULL;
    pd3dContext->OMGetRenderTargets( 1, &pRTV, &pDSV );

    // render hair

    const UINT dwClearDataMinusOne[1] = {0xFFFFFFFF};
    pd3dContext->ClearUnorderedAccessViewUint(m_pHeadPPLL_UAV, dwClearDataMinusOne);

    // Clear stencil buffer to mask the rendering area
    // Keep depth buffer for correct depth and early z
    pd3dContext->ClearDepthStencilView(pDSV, D3D10_CLEAR_STENCIL, 1.0, 0);

    ID3D11UnorderedAccessView*  pUAV[] = {m_pHeadPPLL_UAV, m_pPPLL_UAV, NULL, NULL, NULL, NULL, NULL};
    UINT    pUAVCounters[] = { 0, 0, 0, 0, 0, 0, 0 };
    pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &pRTV, pDSV, 1, 7, pUAV, pUAVCounters);

    // disable color write if there is no need for fragments counting
    pd3dContext->OMSetBlendState(m_pColorWritesOff, 0, 0xffffffff);

    // Enable depth test to use early z, disable depth write to make sure required layers won't be clipped out in early z
    pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledNoDepthWritesStencilWriteIncrementDSS, 0x00);

    // Pass 1: A-Buffer pass
    if (m_hairParams.bAntialias)
    {
        if (m_hairParams.strandCopies > 1)
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairAAStrandCopies, m_pPSABuffer_Hair, m_hairParams.density, false, m_hairParams.strandCopies);
        }
        else
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairAA, m_pPSABuffer_Hair, m_hairParams.density, false, 1);
        }
    }
    else
    {
        if (m_hairParams.strandCopies > 1)
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairStrandCopies, m_pPSABuffer_Hair, m_hairParams.density, false, m_hairParams.strandCopies);
        }
        else
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHair, m_pPSABuffer_Hair, m_hairParams.density, false, 1);
        }
    }

    // Pass 2: K-Buffer pass
    pd3dContext->OMSetBlendState(m_pBlendStateBlendToBg, 0, 0xffffffff);
    pd3dContext->OMSetDepthStencilState(m_pDepthTestDisabledStencilTestLessDSS, 0x00);

    pUAV[0] = pUAV[1] = pUAV[2] = pUAV[3] = pUAV[4] = pUAV[5] = pUAV[6] = 0;
    pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &pRTV, pDSV, 1, 7, pUAV, pUAVCounters);

    pd3dContext->PSSetShaderResources(IDSRV_HEAD_PPLL, 1, &m_pHeadPPLL_SRV);
    pd3dContext->PSSetShaderResources(IDSRV_PPLL, 1, &m_pPPLL_SRV);

    RenderScreenQuad(pd3dContext, m_pVSScreenQuad, m_pPSKBuffer_Hair);

    ID3D11ShaderResourceView* pNULL = NULL;
    pd3dContext->PSSetShaderResources(IDSRV_HEAD_PPLL, 1, &pNULL);
    pd3dContext->PSSetShaderResources(IDSRV_PPLL, 1, &pNULL);

    pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledDSS, 0x00);
    pd3dContext->OMSetRenderTargets(1, &pRTV, pDSV);
    pd3dContext->OMSetBlendState(NULL, 0, 0xffffffff);
    AMD_SAFE_RELEASE( pRTV );
    AMD_SAFE_RELEASE( pDSV );
}


void TressFXRenderer::RenderHairShortcut(ID3D11DeviceContext* pd3dContext)
{
    // Get original render target and depth stencil view
    ID3D11RenderTargetView* pRTV = NULL;
    ID3D11DepthStencilView* pDSV = NULL;
    pd3dContext->OMGetRenderTargets(1, &pRTV, &pDSV);

    ID3D11PixelShader* pPS = m_ShortCut.SetupDepthPass(pd3dContext, pRTV, pDSV);

    // DEPTH FILL
    if (m_hairParams.bAntialias)
    {
        if (m_hairParams.strandCopies > 1)
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairAAStrandCopies, pPS, m_hairParams.density, false, m_hairParams.strandCopies);
        }
        else
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairAA, pPS, m_hairParams.density, false, 1);
        }
    }
    else
    {
        if (m_hairParams.strandCopies > 1)
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairStrandCopies, pPS, m_hairParams.density, false, m_hairParams.strandCopies);
        }
        else
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHair, pPS, m_hairParams.density, false, 1);
        }
    }

    // DEPTH RESOLVE
    pPS = m_ShortCut.SetupResolveDepth(pd3dContext, pRTV, pDSV);
    RenderScreenQuad(pd3dContext, m_pVSScreenQuad, pPS);


    // COLOR FILL
    pPS = m_ShortCut.SetupShadePass(pd3dContext, pRTV, pDSV);

    if (m_hairParams.bAntialias)
    {
        if (m_hairParams.strandCopies > 1)
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairAAStrandCopies, pPS, m_hairParams.density, false, m_hairParams.strandCopies);
        }
        else
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairAA, pPS, m_hairParams.density, false, 1);
        }
    }
    else
    {
        if (m_hairParams.strandCopies > 1)
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHairStrandCopies, pPS, m_hairParams.density, false, m_hairParams.strandCopies);
        }
        else
        {
            RenderHairGeometry(pd3dContext, m_pVSRenderHair, pPS, m_hairParams.density, false, 1);
        }
    }

    // COLOR RESOLVE
    pPS = m_ShortCut.SetupResolveColor(pd3dContext, pRTV, pDSV);
    RenderScreenQuad(pd3dContext, m_pVSScreenQuad, pPS);

    m_ShortCut.PostResolveColor(pd3dContext);

    pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledDSS, 0x00);
    pd3dContext->OMSetRenderTargets(1, &pRTV, pDSV);
    pd3dContext->OMSetBlendState(NULL, 0, 0xffffffff);
    AMD_SAFE_RELEASE(pRTV);
    AMD_SAFE_RELEASE(pDSV);
}

//--------------------------------------------------------------------------------------
//
// EndHairFrame
//
// Start of hair rendering.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::EndHairFrame(ID3D11DeviceContext* pd3dContext)
{
    ID3D11ShaderResourceView* pNull = 0;
    pd3dContext->PSSetShaderResources(IDSRV_HAIRSM, 1, &pNull);
}

//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when device is destroyed.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::OnDestroy(bool destroyShaders)
{
    m_ShortCut.OnDestroy(destroyShaders);

    AMD_SAFE_RELEASE(m_pScreenQuadVB);

    if (destroyShaders)
    {
        // vs/ps for rendering hair from the eye camera.
        AMD_SAFE_RELEASE(m_pVSRenderHair);
        AMD_SAFE_RELEASE(m_pVSRenderHairAA);

        AMD_SAFE_RELEASE(m_pVSRenderHairStrandCopies);
        AMD_SAFE_RELEASE(m_pVSRenderHairAAStrandCopies);

        // vs to generate hair SM from the light camera.
        AMD_SAFE_RELEASE(m_pVSGenerateHairSM);

        // A-buffer
        AMD_SAFE_RELEASE(m_pPSABuffer_Hair);
        // PS composite nearest k hair fragments
        AMD_SAFE_RELEASE(m_pPSKBuffer_Hair);

        AMD_SAFE_RELEASE(m_pVSScreenQuad);

        // vertex layouts for hair
        AMD_SAFE_RELEASE( m_pLayoutHair );
        AMD_SAFE_RELEASE( m_pLayoutQuad );
    }

    // constant buffer
    AMD_SAFE_RELEASE(m_pcbPerFrame);

    // PPLL buffers
    DeletePPLL();

    // textures and views
    AMD_SAFE_RELEASE(m_pNoiseTexture2D);
    AMD_SAFE_RELEASE(m_pNoiseSRV);

    // Hair shadow map depth stencil buffer
    AMD_SAFE_RELEASE(m_pSMHairTx);
    AMD_SAFE_RELEASE(m_pSMHairDSV);
    AMD_SAFE_RELEASE(m_pSMHairSRV);

    // render states
    AMD_SAFE_RELEASE(m_pBlendStateBlendToBg);
    AMD_SAFE_RELEASE(m_pColorWritesOff );
    AMD_SAFE_RELEASE( m_pDepthTestEnabledDSS );
    AMD_SAFE_RELEASE( m_pDepthTestEnabledNoDepthWritesStencilWriteIncrementDSS );
    AMD_SAFE_RELEASE( m_pDepthTestDisabledStencilTestLessDSS );
    AMD_SAFE_RELEASE(m_pSamplerStateLinearWrap );
    AMD_SAFE_RELEASE(m_pSamplerStatePointClamp );
    AMD_SAFE_RELEASE(m_pSamplerStateCmpLess);
}

} // namespace AMD
