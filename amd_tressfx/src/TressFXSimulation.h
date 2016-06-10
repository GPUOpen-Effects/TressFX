//--------------------------------------------------------------------------------------
// File: TressFXSimulation.h
//
// Hair physics simulation header
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

#include <string>
#include "AMD_TressFX.h"
#include "TressFXMesh.h"
#include "Util.h"
#include "Math\vector3D.h"
#include <map>

typedef DirectX::XMFLOAT4 float4;
typedef DirectX::XMFLOAT3 float3;

// forward declarations
namespace AMD
{
    class ShaderCache;
}

namespace AMD
{

class TressFXSimulation
{
public:
    TressFXSimulation(void);
    ~TressFXSimulation(void);

    HRESULT OnCreateDevice(ID3D11Device* pd3dDevice, TressFX_CollisionCapsule *pCollision);
    HRESULT Simulate(ID3D11DeviceContext* pd3dContext, float fElapsedTime, float density,
                     tressfx_vec3 &windDir, float windMag, DirectX::XMMATRIX *pModelTransformForHead,
                     ID3D11UnorderedAccessView *pSkinningTransforms,
                     float targetFrameRate = 1.0f / 60.0f, bool singleHeadTransform = true, bool bAlwaysSimulate = false);
    HRESULT GenerateTransforms(ID3D11DeviceContext* pd3dContext, TressFX_SceneMesh sceneMesh,
                               ID3D11UnorderedAccessView** ppSkinningTransformationsUAV,
                               DirectX::XMMATRIX *pModelTransformForHead);
    HRESULT ApplyTransformGlobally(ID3D11DeviceContext* pd3dContext, ID3D11UnorderedAccessView* pSkinningTransforms,
                                   float density, bool singleHeadTransform, DirectX::XMMATRIX *pModelTransformForHead);
    void OnDestroy(bool destroyShaders);
    TressFXMesh*                m_pTressFXMesh;
    float                       m_elapsedTimeSinceLastSim;

private:
    bool                        m_bGuideFollowHairPrev;

    // hair simulation params
    TressFX_SimulationParams m_simParams;

    // Compute Shader
    //data for compute shaders
    ID3D11ComputeShader*        m_CSIntegrationAndGlobalShapeConstraints;
    ID3D11ComputeShader*        m_CSApplyHairTransformGlobally;
    ID3D11ComputeShader*        m_CSComputeTangents;
    ID3D11ComputeShader*        m_CSLocalShapeConstraints;
    ID3D11ComputeShader*        m_CSLocalShapeConstraintsSingleDispatch;
    ID3D11ComputeShader*        m_CSLengthConstraintsWindAndCollision;
    ID3D11ComputeShader*        m_CSUpdateFollowHairVertices;
    ID3D11ComputeShader*        m_CSPrepareFollowHairBeforeTurningIntoGuide;
    ID3D11ComputeShader*        m_CSGenerateTransforms;

    // const buffers for CS
    ID3D11Buffer*               m_pCBCSPerFrame;
    ID3D11Buffer*               m_pCBCSCollisionCapsule;
    ID3D11Buffer*               m_pCBGenerateTransforms;
    ID3D11Buffer*               m_pCBHeadTransforms;

    HRESULT CreateComputeShaderConstantBuffers(ID3D11Device* pd3dDevice, TressFX_CollisionCapsule *pCollision);

public:
    void SetSimulationParams(const TressFX_SimulationParams& simParams) { m_simParams = simParams; }
};

} // namespace AMD
