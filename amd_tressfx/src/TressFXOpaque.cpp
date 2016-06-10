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

#include "TressFXOpaque.h"

namespace AMD
{
    void TressFX_OpaqueDesc::Initialize(TressFX_Desc & desc)
    {
        if (initialized == false)
        {
            refCount = 0;
            tressFXSimulation.OnCreateDevice(desc.pd3dDevice, &desc.collisionCapsule);
            tressFXRenderer.OnCreateDevice(desc.pd3dDevice, desc.backBufferWidth, desc.backBufferHeight, desc.bShortCutOn);
            initialized = true;
        }
        refCount++;
    }

    void TressFX_OpaqueDesc::Release()
    {
        refCount--;
        if (refCount <= 0)
        {
            initialized = false;
            tressFXRenderer.OnDestroy(true);
            tressFXSimulation.OnDestroy(true);
            refCount = 0;
        }
    }

    bool TressFX_OpaqueDesc::LoadAppendAsset(TressFX_HairBlob *pRawHairBlob, const TressFX_GuideFollowParams& guideFollowParams, int groupId)
    {
        return tressFXAssetLoader.LoadAppend(pRawHairBlob, guideFollowParams, groupId);
    }

    bool TressFX_OpaqueDesc::CreateProcessedAsset(TressFX_Desc &desc, TressFX_HairBlob **ppHairBlob, TressFX_SceneMesh *sceneMesh, ID3D11ShaderResourceView *hairTexture)
    {
        tressFXAssetLoader.GenerateFollowHairs();
        tressFXAssetLoader.ProcessVertices();

        if (desc.tressFXHair.pHair != NULL)
        {
            delete desc.tressFXHair.pHair;
            desc.tressFXHair.pHair = NULL;
            desc.tressFXHair.size = 0;
        }

        tressFXAssetLoader.Serialize(&desc.tressFXHair);

        TressFXMesh *pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
        if (pTressFXMesh != NULL)
        {
            delete pTressFXMesh;
        }
        pTressFXMesh = new TressFXMesh();
        pTressFXMesh->OnCreate(desc.pd3dDevice, &desc.tressFXHair, sceneMesh, hairTexture);

        desc.numTotalHairStrands = pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
        desc.numTotalHairVertices = pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
        desc.pTressFXMesh = (void *)pTressFXMesh;

        if (ppHairBlob != NULL)
        {
            *ppHairBlob = &desc.tressFXHair;
        }

        tressFXAssetLoader.Clear();

        return true;
    }

    bool TressFX_OpaqueDesc::Simulate(TressFX_Desc & desc, float elapsedTime)
    {
        tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
        desc.numTotalHairStrands = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
        desc.numTotalHairVertices = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
        tressFXSimulation.SetSimulationParams(desc.simulationParams);
        tressfx_vec3 windDir;
        windDir.x = desc.simulationParams.windDir.x;
        windDir.y = desc.simulationParams.windDir.y;
        windDir.z = desc.simulationParams.windDir.z;
        HRESULT hr = tressFXSimulation.Simulate(desc.pd3dDeviceContext, elapsedTime,
            desc.hairParams.density, windDir, desc.simulationParams.windMag, &desc.modelTransformForHead,
            desc.pSkinningTransformationsUAV, desc.targetFrameRate, desc.bSingleHeadTransform, false);

        return (hr == S_OK);
    }

    bool TressFX_OpaqueDesc::Begin(TressFX_Desc & desc)
    {
        tressFXRenderer.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
        tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
        desc.numTotalHairStrands = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
        desc.numTotalHairVertices = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
        tressFXRenderer.m_hairParams = desc.hairParams;
        tressFXRenderer.BeginHairFrame(desc.pd3dDeviceContext,
            desc.eyePoint, desc.lightPosition, &desc.modelTransformForHead, &desc.mViewProj, &desc.mViewProjLightFromLibrary,
            (float)desc.backBufferWidth, (float)desc.backBufferHeight, desc.bSingleHeadTransform);
        return true;
    }

    bool TressFX_OpaqueDesc::RenderShadowMap(TressFX_Desc & desc)
    {
        tressFXRenderer.GenerateShadowMap(desc.pd3dDeviceContext, desc.hairParams.density,
            (float)desc.backBufferWidth, (float)desc.backBufferHeight);
        desc.pHairShadowMapSRV = tressFXRenderer.GetShadowMapSRV();
        return true;
    }

    bool TressFX_OpaqueDesc::RenderHair(TressFX_Desc & desc)
    {
        if (desc.bShortCutOn)
        {
            tressFXRenderer.RenderHairShortcut(desc.pd3dDeviceContext);
        }
        else
        {
            tressFXRenderer.RenderHair(desc.pd3dDeviceContext);
        }
        return true;
    }

    bool TressFX_OpaqueDesc::End(TressFX_Desc & desc)
    {
        tressFXRenderer.EndHairFrame(desc.pd3dDeviceContext);
        return true;
    }

    bool TressFX_OpaqueDesc::Resize(TressFX_Desc & desc)
    {
        HRESULT hr = tressFXRenderer.OnResizedSwapChain(desc.pd3dDevice, desc.backBufferWidth, desc.backBufferHeight, desc.bShortCutOn);
        return (hr == S_OK);
    }

    bool TressFX_OpaqueDesc::GenerateTransforms(TressFX_Desc & desc, TressFX_SceneMesh &sceneMesh)
    {
        tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
        desc.numTotalHairStrands = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
        desc.numTotalHairVertices = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
        HRESULT hr = tressFXSimulation.GenerateTransforms(desc.pd3dDeviceContext, sceneMesh,
            &desc.pSkinningTransformationsUAV, &desc.modelTransformForHead);
        return (hr == S_OK);
    }

    bool TressFX_OpaqueDesc::ApplyRigidTransforms(TressFX_Desc & desc)
    {
        tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
        desc.numTotalHairStrands = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
        desc.numTotalHairVertices = tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
        HRESULT hr = tressFXSimulation.ApplyTransformGlobally(desc.pd3dDeviceContext, desc.pSkinningTransformationsUAV,
            desc.hairParams.density, desc.bSingleHeadTransform, &desc.modelTransformForHead);
        return (hr == S_OK);
    }

} // namespace AMD
