//--------------------------------------------------------------------------------------
// File: TressFXShortCut.cpp
//
// TressFX ShortCut method.
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
#include "TressFXShortCut.h"
#include "TressFXPrecompiledShaders.h"


#ifndef AMD_V_RETURN
#define AMD_V_RETURN(x)    { hr = (x); if ( FAILED(hr) ) { return hr; } }
#endif

// unreferenced formal parameter
#pragma warning(disable: 4100)

using namespace DirectX;

// Constants must match in TressFXRender.hlsl

// Clear value for depths resource
#define SHORTCUT_INITIAL_DEPTH 0x3f800000

// Number of depth layers to use.  2 or 3 supported.
#define SHORTCUT_NUM_DEPTHS 3

// Compute source color as weighted average of front fragments, vs blending in order.
#define SHORTCUT_WEIGHTED_AVERAGE 1

// Output color deterministically when fragments have the same depth.  Requires additional clear of colors resource.
#define SHORTCUT_DETERMINISTIC 1


namespace AMD
{

    HRESULT GPUOnlyStructuredBuffer::Create(ID3D11Device* pd3dDevice, DWORD structSize, DWORD structCount)
    {
        HRESULT hr;

        // Per-pixel Linked List (PPLL) buffer
        D3D11_BUFFER_DESC BufferDesc;
        BufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
        BufferDesc.ByteWidth = (DWORD)(structCount * structSize);
        BufferDesc.CPUAccessFlags = 0;
        BufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
        BufferDesc.Usage = D3D11_USAGE_DEFAULT;
        BufferDesc.StructureByteStride = structSize;
        AMD_V_RETURN(pd3dDevice->CreateBuffer(&BufferDesc, NULL, &m_pBuffer));

        // Create UAV view of Fragment and Link Buffer
        D3D11_UNORDERED_ACCESS_VIEW_DESC UAVDesc;
        UAVDesc.Format = DXGI_FORMAT_UNKNOWN;
        UAVDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;
        UAVDesc.Buffer.FirstElement = 0;
        UAVDesc.Buffer.NumElements = (DWORD)(structCount);
        UAVDesc.Buffer.Flags = D3D11_BUFFER_UAV_FLAG_COUNTER;
        AMD_V_RETURN(pd3dDevice->CreateUnorderedAccessView(m_pBuffer, &UAVDesc, &m_pUAV));

        // Create SRV view of Fragment and Link Buffer
        D3D11_SHADER_RESOURCE_VIEW_DESC SRVDesc;
        SRVDesc.Format = DXGI_FORMAT_UNKNOWN;
        SRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
        SRVDesc.Buffer.ElementOffset = 0;
        SRVDesc.Buffer.ElementWidth = (DWORD)(structCount);
        AMD_V_RETURN(pd3dDevice->CreateShaderResourceView(m_pBuffer, &SRVDesc, &m_pSRV));

        return S_OK;
    }

    void GPUOnlyStructuredBuffer::Destroy()
    {
        AMD_SAFE_RELEASE(m_pSRV);
        AMD_SAFE_RELEASE(m_pUAV);
        AMD_SAFE_RELEASE(m_pBuffer);
    }

    HRESULT TressFXShortCut::CreateScreenSizedItems(ID3D11Device* pd3dDevice, int winWidth, int winHeight)
    {
        D3D11_TEXTURE2D_DESC tex2D_desc;
        tex2D_desc.Width = winWidth;
        tex2D_desc.Height = winHeight;
        tex2D_desc.ArraySize = 1;
        tex2D_desc.Format = DXGI_FORMAT_R16_TYPELESS;
        tex2D_desc.Usage = D3D11_USAGE_DEFAULT;
        tex2D_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        tex2D_desc.CPUAccessFlags = 0;
        tex2D_desc.MipLevels = 0;
        tex2D_desc.MiscFlags = 0;
        tex2D_desc.SampleDesc.Count = 1;
        tex2D_desc.SampleDesc.Quality = 0;

        HRESULT hr;
        AMD_V_RETURN(pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pAccumInvAlpha));

        D3D11_SHADER_RESOURCE_VIEW_DESC srDesc;
        srDesc.Format = DXGI_FORMAT_R16_FLOAT;
        srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srDesc.Texture2D.MostDetailedMip = 0;
        srDesc.Texture2D.MipLevels = 1;
        pd3dDevice->CreateShaderResourceView(m_pAccumInvAlpha, &srDesc, &m_pAccumInvAlpha_SRV);

        D3D11_RENDER_TARGET_VIEW_DESC rtDesc;
        rtDesc.Format = DXGI_FORMAT_R16_FLOAT;
        rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtDesc.Texture2D.MipSlice = 0;
        pd3dDevice->CreateRenderTargetView(m_pAccumInvAlpha, &rtDesc, &m_pAccumInvAlpha_RTV);

        tex2D_desc.Width = winWidth;
        tex2D_desc.Height = winHeight;
        tex2D_desc.ArraySize = SHORTCUT_NUM_DEPTHS;
        tex2D_desc.Format = DXGI_FORMAT_R32_TYPELESS;
        tex2D_desc.Usage = D3D11_USAGE_DEFAULT;
        tex2D_desc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
        tex2D_desc.CPUAccessFlags = 0;
        tex2D_desc.MipLevels = 0;
        tex2D_desc.MiscFlags = 0;
        tex2D_desc.SampleDesc.Count = 1;
        tex2D_desc.SampleDesc.Quality = 0;
        AMD_V_RETURN(pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pFragmentDepthsTexture));

        srDesc.Format = DXGI_FORMAT_R32_UINT;
        srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
        srDesc.Texture2DArray.ArraySize = SHORTCUT_NUM_DEPTHS;
        srDesc.Texture2DArray.FirstArraySlice = 0;
        srDesc.Texture2DArray.MipLevels = 1;
        srDesc.Texture2DArray.FirstArraySlice = 0;
        pd3dDevice->CreateShaderResourceView(m_pFragmentDepthsTexture, &srDesc, &m_pFragmentDepthsTexture_SRV);

        D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
        uavDesc.Format = DXGI_FORMAT_R32_UINT;
        uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2DARRAY;
        uavDesc.Texture2DArray.ArraySize = SHORTCUT_NUM_DEPTHS;
        uavDesc.Texture2DArray.FirstArraySlice = 0;
        uavDesc.Texture2DArray.MipSlice = 0;
        pd3dDevice->CreateUnorderedAccessView(m_pFragmentDepthsTexture, &uavDesc, &m_pFragmentDepthsTexture_UAV);

#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        tex2D_desc.Width = winWidth;
        tex2D_desc.Height = winHeight;
        tex2D_desc.ArraySize = 1;
        tex2D_desc.Format = DXGI_FORMAT_R16G16B16A16_TYPELESS;
        tex2D_desc.Usage = D3D11_USAGE_DEFAULT;
        tex2D_desc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
        tex2D_desc.CPUAccessFlags = 0;
        tex2D_desc.MipLevels = 0;
        tex2D_desc.MiscFlags = 0;
        tex2D_desc.SampleDesc.Count = 1;
        tex2D_desc.SampleDesc.Quality = 0;
        AMD_V_RETURN(pd3dDevice->CreateTexture2D(&tex2D_desc, 0, &m_pFragmentColorsTexture));

        rtDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        rtDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
        rtDesc.Texture2D.MipSlice = 0;
        pd3dDevice->CreateRenderTargetView(m_pFragmentColorsTexture, &rtDesc, &m_pFragmentColorsTexture_RTV);

        srDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
        srDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        srDesc.Texture2D.MostDetailedMip = 0;
        srDesc.Texture2D.MipLevels = 1;
        pd3dDevice->CreateShaderResourceView(m_pFragmentColorsTexture, &srDesc, &m_pFragmentColorsTexture_SRV);
#else
        m_FragmentColors.Create(pd3dDevice, 4 * SHORTCUT_NUM_DEPTHS, winWidth * winHeight);
#endif

        return S_OK;
    }

    void TressFXShortCut::DestroyScreenSizedItems()
    {
        AMD_SAFE_RELEASE(m_pAccumInvAlpha_SRV);
        AMD_SAFE_RELEASE(m_pAccumInvAlpha_RTV);
        AMD_SAFE_RELEASE(m_pAccumInvAlpha);

        AMD_SAFE_RELEASE(m_pFragmentDepthsTexture_SRV);
        AMD_SAFE_RELEASE(m_pFragmentDepthsTexture_UAV);
        AMD_SAFE_RELEASE(m_pFragmentDepthsTexture);

        AMD_SAFE_RELEASE(m_pFragmentColorsTexture_RTV);
        AMD_SAFE_RELEASE(m_pFragmentColorsTexture_SRV);
        AMD_SAFE_RELEASE(m_pFragmentColorsTexture);
        m_FragmentColors.Destroy();

    }

    //--------------------------------------------------------------------------------------
    //
    // CreateRenderStateObjects
    //
    // Creates the render state objects for hair rendering
    //
    //--------------------------------------------------------------------------------------
    HRESULT TressFXShortCut::CreateRenderStateObjects(ID3D11Device* pd3dDevice)
    {
        HRESULT hr;

        // Create depth stencil states
        D3D11_DEPTH_STENCIL_DESC DSDesc;
        DSDesc.DepthEnable = TRUE;
        DSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DSDesc.StencilEnable = FALSE;
        DSDesc.StencilReadMask = 0xff;
        DSDesc.StencilWriteMask = 0xff;
        DSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        DSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        AMD_V_RETURN(pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestEnabled_DSS));

        DSDesc.DepthEnable = TRUE;
        DSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        DSDesc.StencilEnable = TRUE;
        DSDesc.StencilReadMask = 0xFF;
        DSDesc.StencilWriteMask = 0xFF;
        DSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
        DSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        DSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_INCR_SAT;
        DSDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
        AMD_V_RETURN(hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS));

        DSDesc.DepthEnable = FALSE;
        DSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
        DSDesc.StencilEnable = TRUE;
        DSDesc.StencilReadMask = 0xFF;
        DSDesc.StencilWriteMask = 0x00;
        DSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
        DSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
        AMD_V_RETURN(hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthTestDisabledStencilTestLess_DSS));

        DSDesc.DepthEnable = TRUE;
        DSDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
        DSDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
        DSDesc.StencilEnable = TRUE;
        DSDesc.StencilReadMask = 0xFF;
        DSDesc.StencilWriteMask = 0x00;
        DSDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS;
        DSDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
        DSDesc.BackFace.StencilFunc = D3D11_COMPARISON_LESS;
        AMD_V_RETURN(hr = pd3dDevice->CreateDepthStencilState(&DSDesc, &m_pDepthWriteEnabledStencilTestLess_DSS));

        // Create blend state objects
        D3D11_BLEND_DESC blendDesc;
        ZeroMemory(&blendDesc, sizeof(D3D11_BLEND_DESC));

        blendDesc.IndependentBlendEnable = false;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_RED;
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_COLOR;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        AMD_V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &m_pDepthWritesToColor_BS));


        blendDesc.IndependentBlendEnable = false;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_SRC_ALPHA;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        AMD_V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &m_pResolveColor_BS));

        // Create a blend state to disable color writes
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = 0;
        AMD_V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &m_pNoWrites_BS));

        // Create a blend state to sum into all channels
        blendDesc.IndependentBlendEnable = false;
        blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
        blendDesc.AlphaToCoverageEnable = false;
        blendDesc.RenderTarget[0].BlendEnable = true;
        blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
        blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
        blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
        AMD_V_RETURN(pd3dDevice->CreateBlendState(&blendDesc, &m_pSum_BS));
        
        return S_OK;
    }

    HRESULT TressFXShortCut::OnCreateDevice(ID3D11Device* pd3dDevice, int winWidth, int winHeight)
    {
        HRESULT hr;

        // pixel shaders  STAND INS.
        AMD_V_RETURN(pd3dDevice->CreatePixelShader(PS_Depth_Hair_Data, sizeof(PS_Depth_Hair_Data), NULL, &m_pPSDepthsAlpha));
        AMD_V_RETURN(pd3dDevice->CreatePixelShader(PS_FillColors_Hair_Data, sizeof(PS_FillColors_Hair_Data), NULL, &m_pPSFillColors));
        AMD_V_RETURN(pd3dDevice->CreatePixelShader(PS_ResolveDepth_Hair_Data, sizeof(PS_ResolveDepth_Hair_Data), NULL, &m_pPSResolveDepth));
        AMD_V_RETURN(pd3dDevice->CreatePixelShader(PS_ResolveColor_Hair_Data, sizeof(PS_ResolveColor_Hair_Data), NULL, &m_PSResolveColor));

        AMD_V_RETURN(CreateScreenSizedItems(pd3dDevice, winWidth, winHeight));
        AMD_V_RETURN(CreateRenderStateObjects(pd3dDevice));

        return S_OK;
    }

    HRESULT TressFXShortCut::OnResizedSwapChain(ID3D11Device* pd3dDevice, int winWidth, int winHeight)
    {
        DestroyScreenSizedItems();

        HRESULT hr;
        AMD_V_RETURN(CreateScreenSizedItems(pd3dDevice, winWidth, winHeight));
        return S_OK;
    }


    void TressFXShortCut::OnDestroy(bool destroyShaders)
    {
        AMD_SAFE_RELEASE(m_pDepthTestEnabled_DSS);
        AMD_SAFE_RELEASE(m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS);
        AMD_SAFE_RELEASE(m_pDepthTestDisabledStencilTestLess_DSS);
        AMD_SAFE_RELEASE(m_pDepthWriteEnabledStencilTestLess_DSS);

        AMD_SAFE_RELEASE(m_pDepthWritesToColor_BS);
        AMD_SAFE_RELEASE(m_pNoWrites_BS);
        AMD_SAFE_RELEASE(m_pResolveColor_BS);
        AMD_SAFE_RELEASE(m_pSum_BS);

        DestroyScreenSizedItems();

        if (destroyShaders)
        {
            // vs/ps for rendering hair from the eye camera.
            AMD_SAFE_RELEASE(m_pPSDepthsAlpha);
            AMD_SAFE_RELEASE(m_pPSResolveDepth);
            AMD_SAFE_RELEASE(m_pPSFillColors);
            AMD_SAFE_RELEASE(m_PSResolveColor);
        }

    }

    ID3D11PixelShader* TressFXShortCut::SetupDepthPass(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
    {
        const UINT dwDepthClearMax[] = { SHORTCUT_INITIAL_DEPTH };
        pd3dContext->ClearUnorderedAccessViewUint(m_pFragmentDepthsTexture_UAV, dwDepthClearMax);

        //// Clear stencil buffer to mask the rendering area
        //// Keep depth buffer for correct depth and early z
        pd3dContext->ClearDepthStencilView(pDSV, D3D10_CLEAR_STENCIL, 1.0, 0);

        float clearColor[4] = { 1, 1, 1, 1 };
        pd3dContext->ClearRenderTargetView(m_pAccumInvAlpha_RTV, clearColor);

        ID3D11UnorderedAccessView*  pUAV[] = { m_pFragmentDepthsTexture_UAV, NULL, NULL, NULL, NULL, NULL, NULL };
        UINT    pUAVCounters[] = { 0, 0, 0, 0, 0, 0, 0 };
        pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &m_pAccumInvAlpha_RTV, pDSV, 1, 7, pUAV, pUAVCounters);

        pd3dContext->OMSetBlendState(m_pDepthWritesToColor_BS, 0, 0xffffffff);
        
        pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, 0x00);

        return m_pPSDepthsAlpha;
    }


    ID3D11PixelShader* TressFXShortCut::SetupResolveDepth(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
    {
        ID3D11UnorderedAccessView*  pUAV[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        UINT    pUAVCounters[] = { 0, 0, 0, 0, 0, 0, 0 };
        pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(0, NULL, pDSV, 0, 7, pUAV, pUAVCounters);
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_DEPTHS, 1, &m_pFragmentDepthsTexture_SRV);

        pd3dContext->OMSetBlendState(m_pNoWrites_BS, 0, 0xffffffff);

        pd3dContext->OMSetDepthStencilState(m_pDepthWriteEnabledStencilTestLess_DSS, 0x00);

        return m_pPSResolveDepth;
    }


    ID3D11PixelShader* TressFXShortCut::SetupShadePass(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
    {
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        float clearColor[4] = { 0, 0, 0, 0 };
        pd3dContext->ClearRenderTargetView(m_pFragmentColorsTexture_RTV, clearColor);

        ID3D11UnorderedAccessView*  pUAV[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        UINT    pUAVCounters[] = { 0, 0, 0, 0, 0, 0, 0 };
        pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &m_pFragmentColorsTexture_RTV, pDSV, 1, 7, pUAV, pUAVCounters);

        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_DEPTHS, 1, &m_pFragmentDepthsTexture_SRV);

        pd3dContext->OMSetBlendState(m_pSum_BS, 0, 0xffffffff);

        pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, 0x00);
#else
#if SHORTCUT_DETERMINISTIC
        const UINT dwZero[] = { 0 };
        pd3dContext->ClearUnorderedAccessViewUint(m_FragmentColors.m_pUAV, dwZero);
#endif

        ID3D11UnorderedAccessView*  pUAV[] = { m_FragmentColors.m_pUAV, NULL, NULL, NULL, NULL, NULL, NULL };
        UINT    pUAVCounters[] = { 0, 0, 0, 0, 0, 0, 0 };
        pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(0, NULL, pDSV, 0, 7, pUAV, pUAVCounters);
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_DEPTHS, 1, &m_pFragmentDepthsTexture_SRV);

        pd3dContext->OMSetBlendState(m_pNoWrites_BS, 0, 0xffffffff);

        pd3dContext->OMSetDepthStencilState(m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, 0x00);
#endif

        return m_pPSFillColors;
    }


    ID3D11PixelShader* TressFXShortCut::SetupResolveColor(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV)
    {
        ID3D11UnorderedAccessView*  pUAV[] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL };
        UINT    pUAVCounters[] = { 0, 0, 0, 0, 0, 0, 0 };
        pd3dContext->OMSetRenderTargetsAndUnorderedAccessViews(1, &pRTV, pDSV, 1, 7, pUAV, pUAVCounters);


        pd3dContext->PSSetShaderResources(IDSRV_HAIR_ACCUM_INV_ALPHA, 1, &m_pAccumInvAlpha_SRV);
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_DEPTHS, 1, &m_pFragmentDepthsTexture_SRV);
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_COLORS, 1, &m_pFragmentColorsTexture_SRV);
#else
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_COLORS, 1, &m_FragmentColors.m_pSRV);
#endif

        pd3dContext->OMSetBlendState(m_pResolveColor_BS, 0, 0xffffffff);

        pd3dContext->OMSetDepthStencilState(m_pDepthTestDisabledStencilTestLess_DSS, 0x00);

        return m_PSResolveColor;
    }

    void TressFXShortCut::PostResolveColor(ID3D11DeviceContext* pd3dContext)
    {
        ID3D11ShaderResourceView* pNULLShader = NULL;
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_ACCUM_INV_ALPHA, 1, &pNULLShader);
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_DEPTHS, 1, &pNULLShader);
        pd3dContext->PSSetShaderResources(IDSRV_HAIR_FRAGMENT_COLORS, 1, &pNULLShader);
    }

}
