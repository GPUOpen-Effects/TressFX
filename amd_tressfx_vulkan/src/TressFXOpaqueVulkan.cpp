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

#include "TressFXOpaqueVulkan.h"

namespace AMD
{
void TressFX_OpaqueDesc::Initialize(TressFX_Desc &desc,
                                    VkImageView depthTexture,
                                    VkImageView colorTexture,
                                    VkCommandBuffer commandBuffer,
                                    VkDeviceMemory scratchMemory,
                                    VkBuffer scratchBuffer,
                                    size_t &offsetInScratchBuffer,
                                    VkPhysicalDeviceMemoryProperties memProperties)
{
    refCount = 0;
    markerCallbacks.init(desc.pvkDevice);
    tressFXSimulation.OnCreateDevice(desc.pvkDevice,
                                        &desc.collisionCapsule,
                                        desc.maxConstantBuffers, memProperties,
                                        markerCallbacks);
    tressFXRenderer.OnCreateDevice(
        desc.pvkDevice, desc.backBufferWidth, desc.backBufferHeight, desc.bShortCutOn,
        desc.maxConstantBuffers, memProperties,
        depthTexture, colorTexture, commandBuffer, scratchMemory, scratchBuffer,
        offsetInScratchBuffer, desc.depthStencilFormat, desc.colorFormat);
    refCount++;
}

void TressFX_OpaqueDesc::Release(VkDevice pvkDevice)
{
    refCount--;
    if (refCount <= 0)
    {
        tressFXRenderer.OnDestroy();
        tressFXSimulation.OnDestroy(pvkDevice);
        refCount = 0;
    }
}

bool TressFX_OpaqueDesc::LoadAppendAsset(
    TressFX_HairBlob *pRawHairBlob, const TressFX_GuideFollowParams &guideFollowParams,
    int groupId)
{
    return tressFXAssetLoader.LoadAppend(pRawHairBlob, guideFollowParams, groupId);
}

bool TressFX_OpaqueDesc::CreateProcessedAsset(
    TressFX_Desc &desc, TressFX_HairBlob **ppHairBlob, TressFX_SceneMesh *sceneMesh,
    VkImageView hairTexture, VkPhysicalDeviceMemoryProperties memProperties,
    VkCommandBuffer uploadCmdBuffer, VkBuffer scratchBuffer, VkDeviceMemory scratchMemory)
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
    pTressFXMesh->OnCreate(desc.pvkDevice, &desc.tressFXHair, sceneMesh,
                           hairTexture,
                           memProperties, uploadCmdBuffer,
                           scratchBuffer,
                           scratchMemory,
                           tressFXSimulation.m_GlobalConstraintsSetLayout,
                           tressFXSimulation.m_LocalConstraintsSetLayout,
                           tressFXSimulation.m_LenghtWindTangentSetLayout,
                           tressFXSimulation.m_PrepareFollowHairSetLayout,
                           tressFXSimulation.m_UpdateFollowHaitSetLayout,
                           desc.pOpaque->tressFXSimulation.m_ComputeTangentSetLayout,
                           tressFXRenderer.m_pass1_hair_set_layout,
                           tressFXRenderer.m_shadow_pass_hair_set_layout,
                           desc.pOpaque->markerCallbacks);

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

bool TressFX_OpaqueDesc::Simulate(TressFX_Desc &desc, VkCommandBuffer commandBuffer,
                                  float elapsedTime, uint32_t uniformBufferIndex)
{
    tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
    desc.numTotalHairStrands =
        tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
    desc.numTotalHairVertices =
        tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
    tressFXSimulation.SetSimulationParams(desc.simulationParams);
    tressfx_vec3 windDir;
    windDir.x = desc.simulationParams.windDir.x;
    windDir.y = desc.simulationParams.windDir.y;
    windDir.z = desc.simulationParams.windDir.z;
    VkResult vr = tressFXSimulation.Simulate(
        desc.pvkDevice, commandBuffer, elapsedTime, desc.hairParams.density, windDir,
        desc.simulationParams.windMag, &desc.modelTransformForHead,
        desc.targetFrameRate, desc.bSingleHeadTransform, false, uniformBufferIndex,
        markerCallbacks);

    return vr == VK_SUCCESS;
}

bool TressFX_OpaqueDesc::Begin(TressFX_Desc &desc, uint32_t uniformBufferIndex)
{
    tressFXRenderer.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
    tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
    desc.numTotalHairStrands =
        tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
    desc.numTotalHairVertices =
        tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
    tressFXRenderer.m_hairParams = desc.hairParams;
    tressFXRenderer.BeginHairFrame(
        desc.pvkDevice, desc.eyePoint, desc.lightPosition, &desc.modelTransformForHead,
        &desc.mViewProj, &desc.mViewProjLightFromLibrary, (float)desc.backBufferWidth,
        (float)desc.backBufferHeight, desc.bSingleHeadTransform, uniformBufferIndex);
    return true;
}

bool TressFX_OpaqueDesc::RenderShadowMap(TressFX_Desc &desc,
                                         VkCommandBuffer commandBuffer,
                                         uint32_t uniformBufferIndex)
{
    tressFXRenderer.GenerateShadowMap(desc.pvkDevice,
                                      commandBuffer,
                                      desc.hairParams.density, uniformBufferIndex,
                                      markerCallbacks);
    desc.pHairShadowMapSRV = tressFXRenderer.GetShadowMapSRV();
    return true;
}

bool TressFX_OpaqueDesc::RenderHair(TressFX_Desc &desc, VkCommandBuffer commandBuffer,
                                    uint32_t uniformBufferIndex)
{
    if (desc.bShortCutOn)
    {
        tressFXRenderer.RenderHairShortcut(desc.pvkDevice,
                                           commandBuffer,
                                           desc.backBufferWidth,
                                           desc.backBufferHeight,
                                           uniformBufferIndex, markerCallbacks);
    }
    else
    {
        tressFXRenderer.RenderHair(desc.pvkDevice, commandBuffer,
                                   desc.backBufferWidth,
                                   desc.backBufferHeight, uniformBufferIndex,
                                   markerCallbacks);
    }
    return true;
}

bool TressFX_OpaqueDesc::End(TressFX_Desc &desc)
{
    tressFXRenderer.EndHairFrame(desc.pvkDevice);
    return true;
}

bool TressFX_OpaqueDesc::Resize(TressFX_Desc &desc, VkPhysicalDeviceMemoryProperties memProperties)
{
    VkResult vr = tressFXRenderer.OnResizedSwapChain(
        desc.pvkDevice, desc.backBufferWidth, desc.backBufferHeight, desc.bShortCutOn,
		memProperties);
    return (vr == VK_SUCCESS);
}

bool TressFX_OpaqueDesc::GenerateTransforms(TressFX_Desc &desc,
                                            TressFX_SceneMesh &sceneMesh)
{
    (void)desc;
    (void)sceneMesh;
    /*        tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
            desc.numTotalHairStrands =
       tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
            desc.numTotalHairVertices =
       tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
            HRESULT hr = tressFXSimulation.GenerateTransforms(desc.pd3dDeviceContext,
       sceneMesh,
                &desc.pSkinningTransformationsUAV, &desc.modelTransformForHead);
            return (hr == S_OK);*/
    return true;
}

bool TressFX_OpaqueDesc::ApplyRigidTransforms(TressFX_Desc &desc)
{
    (void)desc;
    /*        tressFXSimulation.m_pTressFXMesh = (TressFXMesh *)desc.pTressFXMesh;
            desc.numTotalHairStrands =
       tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands;
            desc.numTotalHairVertices =
       tressFXSimulation.m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices;
                    HRESULT hr =
       tressFXSimulation.ApplyTransformGlobally(desc.pd3dDeviceContext,
       desc.pSkinningTransformationsUAV, desc.hairParams.density,
       desc.bSingleHeadTransform, &desc.modelTransformForHead);
            return (hr == S_OK);*/
    return true;
}

} // namespace AMD
