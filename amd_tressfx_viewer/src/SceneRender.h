//--------------------------------------------------------------------------------------
// File: SceneRender.h
//
// Header file for the scene rendering code
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
#include "DXUT.h"
#include "DX11Mesh.h"
#include "AMD_TressFX.h"

extern int g_ScreenWidth;
extern int g_ScreenHeight;

#define SM_SCENE_WIDTH  640
#define SM_SCENE_HEIGHT 640
#define SHADOW_BLUR_RADIUS      6
#define SHADOW_BLUR_PRECISION   1

namespace AMD
{
    class ShaderCache;
}

class CSceneRender
{
private:
    // Mesh body
    CDXUTSDKMesh                m_SkinnedMesh;          // The skinned mesh

    // VS for rendering the scene
    ID3D11VertexShader*         m_pVSSkinning;
    ID3D11VertexShader*         m_pVSScene;
    // VS for generating un-transformed vertices
    ID3D11VertexShader*         m_pVSStreamOutVerts;
    // VS for generating transformed vertices
    ID3D11VertexShader*         m_pVSStreamOutSkinned;
    // VS for generating the scene shadow map
    ID3D11VertexShader*         m_pVSGenerateSceneSM;
    ID3D11VertexShader*         m_pVSGenerateSkinnedSM;
    ID3D11VertexShader*         m_pVSScreenQuad;

    // GS for stream out
    ID3D11GeometryShader*       m_pGSStreamOut;

    // GS for stream out with skinning
    ID3D11GeometryShader*       m_pGSSkinStreamOut;

    // PS for rendering the scene
    ID3D11PixelShader*          m_pPSRenderScene;

    // pixel shaders for generating the scene shadow
    ID3D11PixelShader*          m_pPSRenderSceneShadow;
    ID3D11PixelShader*          m_pHorizontalBlurPS;
    ID3D11PixelShader*          m_pVerticalBlurPS;

    // constant buffers
    ID3D11Buffer*               m_pcbBlur;
    ID3D11Buffer*               m_pcbPerMaterial;
    ID3D11Buffer*               m_pcbPerFrame;

    // vertex buffer
    ID3D11Buffer*               m_pScreenQuadVB;

    // bone matrix buffer
    ID3D11Buffer*               m_pBoneBuffer;
    ID3D11ShaderResourceView*   m_pBoneBufferRV;

    // input layouts
    ID3D11InputLayout*          m_pStreamOutLayout;
    ID3D11InputLayout*          m_pStreamOutLayoutSkinned;
    ID3D11InputLayout*          m_pSkinningLayout;
    ID3D11InputLayout*          m_pScenelayout;
    ID3D11InputLayout*          m_pSceneSMLayout;
    ID3D11InputLayout*          m_pLayoutSceneSM;
    ID3D11InputLayout*          m_pQuadLayout;

    // Scene shadow map depth stencil buffer
    ID3D11Texture2D*            m_pSMSceneTx;
    ID3D11DepthStencilView*     m_pSMSceneDSV;
    ID3D11ShaderResourceView*   m_pSMSceneSRV;

    // Scene Shadow Textures
    ID3D11Texture2D*            m_pSceneShadowTexture;
    ID3D11ShaderResourceView*   m_pSceneShadowSRV;
    ID3D11RenderTargetView*     m_pSceneShadowRTV;
    ID3D11Texture2D*            m_pSceneShadowTexture_temp;
    ID3D11ShaderResourceView*   m_pSceneShadowSRV_temp;
    ID3D11RenderTargetView*     m_pSceneShadowRTV_temp;

    // Render States
    ID3D11DepthStencilState     *m_pDepthTestEnabledDSS;
    ID3D11DepthStencilState     *m_pDepthTestEnabledNoWriteDSS;
    ID3D11DepthStencilState     *m_pDepthStencilDisableState;
    ID3D11SamplerState          *m_pSamplerStateLinearWrap;
    ID3D11SamplerState          *m_pSamplerStatePointClamp;
    ID3D11SamplerState          *m_pLinearClampSampler;
    ID3D11SamplerState          *m_pPointSampler;
    ID3D11RasterizerState       *m_pRasterizerStateShadowmap;
    ID3D11RasterizerState       *m_pRasterizerStateScene;

    ID3D11DeviceContext         *m_pd3dContext;

    // animation status flag based on skinning animation file existance
    bool m_bAnimation;

public:
    unsigned                    m_numMeshes;            // number of meshes in the model
    unsigned*                   m_pMeshIndexCount;      // number of vertices in each mesh
    ID3D11Buffer*               m_pMeshVertices;        // un-transformed non-indexed vertices (position only, flattened)
    ID3D11ShaderResourceView*   m_pMeshVerticesSRV;     // un-transformed vertext SRV
    ID3D11Buffer*               m_pTransformedVerts;    // transformed non-indexed vertices (position only, flattened)
    ID3D11ShaderResourceView*   m_pTransformedSRV;      // transformed vertext shader resource view
    DirectX::XMFLOAT3           m_MeshBBoxCenter;       // Bounding box center of the skinned mesh
    DirectX::XMFLOAT3           m_MeshBBoxExtent;       // Bounding box extent of the skinned mesh.
                                                        // Actually half of the bounding box edge,
                                                        // which is consistent to SDKMESH_MESH
    float                       m_MeshAmbient;          // estimate of the overall ambient value from the mesh materials

private:
    void CalculateMeshBoundingBox();
    void CalculateMeshAmbient();
    void CreateShaderAndLayout( ID3D11Device* pd3dDevice, AMD::ShaderCache *pShaderCache);
    void CreateTextureAndViews(ID3D11Device* pd3dDevice);
    void CreateConstantBuffer(ID3D11Device* pd3dDevice);
    void CreateVertexBuffers(ID3D11Device* pd3dDevice);
    void CreateRenderStateObjects(ID3D11Device* pd3dDevice);

    void SetBoneMatrices(ID3D11DeviceContext* pd3dImmediateContext, UINT iMesh );
    void RenderSceneGeometry(AMD::TressFX_Desc & desc,
                    ID3D11VertexShader* pVS, ID3D11PixelShader* pPS);

    void RenderScreenQuad(ID3D11DeviceContext* pd3dContext,
                    ID3D11VertexShader* pVS, ID3D11PixelShader* pPS);

public:
    CSceneRender(void) : m_bAnimation(false) {};
    ~CSceneRender(void) {};

    void OnCreateDevice(ID3D11Device* pd3dDevice, const WCHAR *bodyFile, const WCHAR *animationFile, AMD::ShaderCache *pShaderCache, bool createShaders);
    void OnFrameMove( double fTime );
    void UpdateSceneCB(AMD::TressFX_Desc & desc);
    void StreamOutVertices(ID3D11DeviceContext* pd3dImmediateContext, bool doSkinning);
    void GenerateShadowMap(AMD::TressFX_Desc & desc);
    void RenderScene(AMD::TressFX_Desc & desc, bool shadow);
    void OnDestroy(bool destroyShaders);
};

