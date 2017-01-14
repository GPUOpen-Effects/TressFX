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

#ifndef AMD_TRESSFX_OPAQUE_H
#define AMD_TRESSFX_OPAQUE_H

#include "TressFXAsset.h"
#include "TressFXMeshVulkan.h"
#include "TressFXRendererVulkan.h"
#include "TressFXSimulationVulkan.h"

namespace AMD
{

struct TressFX_OpaqueDesc
{
  public:
    TressFX_OpaqueDesc() : refCount(0) {}
    void Initialize(TressFX_Desc &desc,
                    VkImageView depthTexture,
                    VkImageView colorTexture,
                    VkCommandBuffer commandBuffer,
                    VkDeviceMemory scratchMemory,
                    VkBuffer scratchBuffer,
                    size_t &offsetInScratchBuffer, VkPhysicalDeviceMemoryProperties memProperties);
    void Release(VkDevice pvkDevice);

    bool LoadAppendAsset(TressFX_HairBlob *pRawHairBlob,
                         const TressFX_GuideFollowParams &guideFollowParams, int groupId);
    bool CreateProcessedAsset(TressFX_Desc &desc, TressFX_HairBlob **ppHairBlob,
                              TressFX_SceneMesh *sceneMesh, VkImageView hairTexture,
                              VkPhysicalDeviceMemoryProperties memProperties,
                              VkCommandBuffer uploadCmdBuffer, VkBuffer scratchBuffer,
                              VkDeviceMemory scratchMemor);

    bool Begin(TressFX_Desc &desc, uint32_t uniformBufferIndex);
    bool End(TressFX_Desc &desc);
    bool RenderShadowMap(TressFX_Desc &desc, VkCommandBuffer commandBuffer,
                         uint32_t uniformBufferIndex);
    bool RenderHair(TressFX_Desc &desc, VkCommandBuffer commandBuffer,
                    uint32_t uniformBufferIndex);
    bool Simulate(TressFX_Desc &desc, VkCommandBuffer commandBuffer, float elapsedTime,
                  uint32_t uniformBufferIndex);
    bool GenerateTransforms(TressFX_Desc &desc, TressFX_SceneMesh &sceneMesh);
    bool ApplyRigidTransforms(TressFX_Desc &desc);
    bool Resize(TressFX_Desc &desc, VkPhysicalDeviceMemoryProperties memProperties);

    TressFXSimulation tressFXSimulation; // Hair simulation class
    TressFXRenderer tressFXRenderer;     // Hair rendering class
    DebugMarkerPointer markerCallbacks;
  private:
    TressFXAssetLoader tressFXAssetLoader; // Hair asset loading class
    int refCount;                          // reference count - delete allocations when 0
    TressFX_OpaqueDesc(const TressFX_OpaqueDesc&) {}
    TressFX_OpaqueDesc& operator=(const TressFX_OpaqueDesc&) {}
};

} // namespace AMD

#endif
