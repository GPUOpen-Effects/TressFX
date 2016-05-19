//--------------------------------------------------------------------------------------
// File: TressFXRenderer.h
//
// Header file for the main hair rendering code
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
#include "TressFXMesh.h"
#include "TressFXShortCut.h"
#include "AMD_TressFX.h"
#include "Math\Vector3D.h"

#define SM_HAIR_WIDTH   640
#define SM_HAIR_HEIGHT  640

// forward declarations
namespace AMD
{
    class ShaderCache;
}

namespace AMD
{

class TressFXRenderer
{
private:
    // shaders

    // VS for rendering the hair
    ID3D11VertexShader*         m_pVSRenderHair;
    ID3D11VertexShader*         m_pVSRenderHairAA;

    // VS for rendering the hair with copies of the strands
    ID3D11VertexShader*         m_pVSRenderHairStrandCopies;
    ID3D11VertexShader*         m_pVSRenderHairAAStrandCopies;

    // VS for generating the hair shadow map
    ID3D11VertexShader*         m_pVSGenerateHairSM;

    // PS for rendering the hair to the A buffer
    ID3D11PixelShader*          m_pPSABuffer_Hair;

    // PS for final rendering using a k-buffer for sorting the k nearest fragments
    ID3D11PixelShader*          m_pPSKBuffer_Hair;

    ID3D11VertexShader*         m_pVSScreenQuad;

    // constant buffer
    ID3D11Buffer*               m_pcbPerFrame;

    // input layouts for hair
    ID3D11InputLayout*          m_pLayoutHair;
    ID3D11InputLayout*          m_pLayoutQuad;


    // Buffers for the head of the per-pixel linked lists (PPLL)
    ID3D11Texture2D*            m_pHeadPPLL_Buffer;
    ID3D11ShaderResourceView*   m_pHeadPPLL_SRV;
    ID3D11UnorderedAccessView*  m_pHeadPPLL_UAV;

    // Buffers for the per-pixel linked list (PPLL) data
    ID3D11Buffer*               m_pPPLL_Buffer;
    ID3D11UnorderedAccessView*  m_pPPLL_UAV;
    ID3D11ShaderResourceView*   m_pPPLL_SRV;

    // textures and views
    ID3D11Texture2D*            m_pNoiseTexture2D;
    ID3D11ShaderResourceView*   m_pNoiseSRV;

    // Hair shadow map depth stencil buffer
    ID3D11Texture2D*            m_pSMHairTx;
    ID3D11DepthStencilView*     m_pSMHairDSV;
    ID3D11ShaderResourceView*   m_pSMHairSRV;

    // vertex buffer for full screen quad
    ID3D11Buffer*               m_pScreenQuadVB;

    // Render States
    ID3D11DepthStencilState     *m_pDepthTestEnabledDSS;
    ID3D11DepthStencilState     *m_pDepthTestEnabledNoDepthWritesStencilWriteIncrementDSS;
    ID3D11DepthStencilState     *m_pDepthTestDisabledStencilTestLessDSS;
    ID3D11SamplerState          *m_pSamplerStateLinearWrap;
    ID3D11SamplerState          *m_pSamplerStatePointClamp;
    ID3D11SamplerState          *m_pSamplerStateCmpLess;
    ID3D11BlendState            *m_pBlendStateBlendToBg;
    ID3D11BlendState            *m_pColorWritesOff;

    TressFXShortCut              m_ShortCut;

public:
    // hair rendering params
    TressFX_HairParams          m_hairParams;

    // hair geometry
    TressFXMesh*               m_pTressFXMesh;

private:
    HRESULT CreateShaderAndLayout(ID3D11Device* pd3dDevice);
    HRESULT CreateTextureAndViews(ID3D11Device* pd3dDevice);
    HRESULT CreateConstantBuffer(ID3D11Device* pd3dDevice);
    HRESULT CreateVertexBuffers(ID3D11Device* pd3dDevice);
    HRESULT CreateRenderStateObjects(ID3D11Device* pd3dDevice);
    HRESULT CreatePPLL(ID3D11Device* pd3dDevice, int winWidth, int winHeight, bool resize);
    void DeletePPLL();
    HRESULT CreateShortCutLayers(ID3D11Device* pd3dDevice, int winWidth, int winHeight, bool resize);

    void SetConstantBuffer(ID3D11DeviceContext* pd3dContext,
        DirectX::XMVECTOR eyePoint, DirectX::XMVECTOR lightPosition,
        DirectX::XMMATRIX *pModelTransformForHead,
        float screenWidth, float screenHeight);

    void SetSamplerStates(ID3D11DeviceContext* pd3dContext);

    void RenderHairGeometry(ID3D11DeviceContext* pd3dContext,
                        ID3D11VertexShader* pVS,
                        ID3D11PixelShader* pPS,
                        float density,
                        bool useLinePrimitives = true,
                        int iStrandCopies = 1);

    void RenderScreenQuad(ID3D11DeviceContext* pd3dContext,
                    ID3D11VertexShader* pVS, ID3D11PixelShader* pPS);
public:
    TressFXRenderer(void) {};
    ~TressFXRenderer(void) {};

    ID3D11ShaderResourceView* GetShadowMapSRV() { return m_pSMHairSRV;};
    HRESULT OnCreateDevice(ID3D11Device* pd3dDevice, int winWidth, int winHeight, bool bShortCutOn);
    HRESULT OnResizedSwapChain(ID3D11Device* pd3dDevice, int winWidth, int WinHeight, bool bShortCutOn);
    void BeginHairFrame(ID3D11DeviceContext* pd3dContext, DirectX::XMVECTOR eyePoint,
        DirectX::XMVECTOR lightPosition, DirectX::XMMATRIX *pModelTrasnsformForHead, DirectX::XMMATRIX *pViewProj, DirectX::XMMATRIX *pViewProjLightOut,
        float screenWidth, float screenHeight, bool singleHeadTransform);
    void GenerateShadowMap(ID3D11DeviceContext* pd3dContext, float density, float screenWidth, float screenHeight);
    void RenderHair(ID3D11DeviceContext* pd3dContext);
    void RenderHairShortcut(ID3D11DeviceContext* pd3dContext);
    void EndHairFrame(ID3D11DeviceContext* pd3dContext);
    void OnDestroy(bool destroyShaders);
};

} // namespace AMD
