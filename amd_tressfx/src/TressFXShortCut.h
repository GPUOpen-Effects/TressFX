//--------------------------------------------------------------------------------------
// File: TressFXShortCut.h
//
// Header file for TressFX ShortCut method.
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
#include "AMD_TressFX.h"
#include "Math\Vector3D.h"

// forward declarations
namespace AMD
{
    class ShaderCache;
}

namespace AMD
{
    struct GPUOnlyStructuredBuffer
    {
        GPUOnlyStructuredBuffer() : m_pBuffer(nullptr), m_pUAV(nullptr), m_pSRV(nullptr) {}
        HRESULT Create(ID3D11Device* pd3dDevice, DWORD structCount, DWORD structSize);
        void Destroy();

        ID3D11Buffer*               m_pBuffer;
        ID3D11UnorderedAccessView*  m_pUAV;
        ID3D11ShaderResourceView*   m_pSRV;
    };

    class TressFXShortCut
    {
    private:
        // shaders


        ID3D11PixelShader*          m_pPSDepthsAlpha;
        ID3D11PixelShader*          m_pPSResolveDepth;
        ID3D11PixelShader*          m_pPSFillColors;
        ID3D11PixelShader*          m_PSResolveColor;

        GPUOnlyStructuredBuffer     m_FragmentColors;

        ID3D11Texture2D*            m_pFragmentColorsTexture;
        ID3D11RenderTargetView*     m_pFragmentColorsTexture_RTV;
        ID3D11ShaderResourceView*   m_pFragmentColorsTexture_SRV;

        ID3D11Texture2D*            m_pFragmentDepthsTexture;
        ID3D11UnorderedAccessView*  m_pFragmentDepthsTexture_UAV;
        ID3D11ShaderResourceView*   m_pFragmentDepthsTexture_SRV;

        ID3D11Texture2D*            m_pAccumInvAlpha;
        ID3D11RenderTargetView*     m_pAccumInvAlpha_RTV;
        ID3D11ShaderResourceView*   m_pAccumInvAlpha_SRV;

        // Render States
        ID3D11DepthStencilState     *m_pDepthTestEnabled_DSS;
        ID3D11DepthStencilState     *m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS;
        ID3D11DepthStencilState     *m_pDepthTestDisabledStencilTestLess_DSS;
        ID3D11DepthStencilState     *m_pDepthWriteEnabledStencilTestLess_DSS;

        ID3D11BlendState            *m_pDepthWritesToColor_BS;
        ID3D11BlendState            *m_pNoWrites_BS;
        ID3D11BlendState            *m_pResolveColor_BS;
        ID3D11BlendState            *m_pSum_BS;


        HRESULT CreateScreenSizedItems(ID3D11Device* pd3dDevice, int winWidth, int winHeight);
        void DestroyScreenSizedItems();
        HRESULT CreateRenderStateObjects(ID3D11Device* pd3dDevice);

    public:
        TressFXShortCut(void) :
            m_pPSDepthsAlpha(nullptr), m_pPSResolveDepth(nullptr), m_pPSFillColors(nullptr), m_PSResolveColor(nullptr),
            m_pFragmentColorsTexture(nullptr), m_pFragmentColorsTexture_RTV(nullptr), m_pFragmentColorsTexture_SRV(nullptr),
            m_pFragmentDepthsTexture(nullptr), m_pFragmentDepthsTexture_UAV(nullptr), m_pFragmentDepthsTexture_SRV(nullptr),
            m_pAccumInvAlpha(nullptr), m_pAccumInvAlpha_RTV(nullptr), m_pAccumInvAlpha_SRV(nullptr),
            m_pDepthTestEnabled_DSS(nullptr), m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS(nullptr),
            m_pDepthTestDisabledStencilTestLess_DSS(nullptr), m_pDepthWriteEnabledStencilTestLess_DSS(nullptr),
            m_pDepthWritesToColor_BS(nullptr), m_pNoWrites_BS(nullptr), m_pResolveColor_BS(nullptr), m_pSum_BS(nullptr) {}
        ~TressFXShortCut(void) {};

        HRESULT OnCreateDevice(ID3D11Device* pd3dDevice, int winWidth, int winHeight);
        HRESULT OnResizedSwapChain(ID3D11Device* pd3dDevice, int winWidth, int WinHeight);


        // Individual render pass setups.
        // Each basically sets up render state, UAVs, SRVs, and returns pixel
        // shader to use.
        ID3D11PixelShader* SetupDepthPass(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
        ID3D11PixelShader* SetupShadePass(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
        ID3D11PixelShader* SetupResolveDepth(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
        ID3D11PixelShader* SetupResolveColor(ID3D11DeviceContext* pd3dContext, ID3D11RenderTargetView* pRTV, ID3D11DepthStencilView* pDSV);
        void PostResolveColor(ID3D11DeviceContext* pd3dContext);

        void OnDestroy(bool destroyShaders);
    };

} // namespace AMD
