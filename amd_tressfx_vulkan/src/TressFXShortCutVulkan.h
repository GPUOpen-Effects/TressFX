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

namespace AMD
{
struct GPUOnlyStructuredBuffer
{
    GPUOnlyStructuredBuffer() : m_pBuffer(nullptr), m_pvkDevice(nullptr) {}
    VkResult Create(VkDevice pd3dDevice, uint32_t structCount, uint32_t structSize);
    void Destroy();

    VkBuffer m_pBuffer;

  private:
    VkDevice m_pvkDevice;
};

class TressFXShortCut
{
  private:
    VkDevice m_pvkDevice;
    VkDescriptorPool m_pDPShortcutPool;

    VkDescriptorSetLayout m_pSLDepthAlpha;
    VkDescriptorSetLayout m_pSLColors;

    VkRenderPass m_pRPRenderHair;
    VkFramebuffer m_pFBRenderHair;

  public:
    VkPipelineLayout m_depthPassPipelineLayout;
    VkPipelineLayout m_colorPassPipelineLayout;
    VkDescriptorSet m_depthPassSet;
    VkDescriptorSet m_colorPassSet;
    // Pipelines for depth alpha
    VkPipeline m_pPLRenderHairDepthsAlpha;
    VkPipeline m_pPLRenderHairStrandCopiesDepthsAlpha;
    VkPipeline m_pPLRenderHairAADepthsAlpha;
    VkPipeline m_pPLRenderHairAAStrandCopiesDepthsAlpha;
    // Pipeline for depth resolve
    VkPipeline m_pPLDepthResolve;
    // Pipelines for color filling
    VkPipeline m_pPLRenderHairFillColors;
    VkPipeline m_pPLRenderHairAAStrandCopiesFillColors;
    VkPipeline m_pPLRenderHairAAFillColors;
    VkPipeline m_pPLRenderHairStrandCopiesFillColors;
    // Pipeline for color resolve
    VkPipeline m_pPLColorResolve;

    VkImage m_pFragmentColorsTexture;
    VkDeviceMemory m_pFragmentColorsMemory;
    VkImageView m_pFragmentColorsView;

    VkImage m_pFragmentDepthsTexture;
    VkDeviceMemory m_pFragmentDepthsMemory;
    VkImageView m_pFragmentDepthsView;

    VkImage m_pAccumInvAlphaTexture;
    VkDeviceMemory m_pAccumInvAlphaMemory;
    VkImageView m_pAccumInvAlphaView;

  private:
    GPUOnlyStructuredBuffer m_FragmentColors;

    VkResult CreateScreenSizedItems(VkDevice pvkDevice, int winWidth, int winHeight,
                                    uint32_t MemoryIndexGPU);
    void DestroyScreenSizedItems();
    VkResult CreateRenderStateObjects(VkDevice pvkDevice);
    VkResult CreateLayouts(VkDevice pvkDevice, VkDescriptorSetLayout SLMesh,
                           VkSampler noiseSamplerRef, VkSampler shadowSamplerRef);
    VkResult CreateFramebuffer(VkDevice pvkDevice, VkImageView depthStencilView,
                               VkImageView colorView, uint32_t width, uint32_t height);
    VkResult AllocateAndPopulateSets(VkDevice pvkDevice, VkBuffer configBuffer,
                                     uint64_t configBufferSize, VkImageView noiseMap,
                                     VkImageView hairShadowMap);

  public:
    TressFXShortCut(void)
        : m_pFragmentColorsTexture(nullptr), m_pFragmentColorsMemory(nullptr),
          m_pFragmentColorsView(nullptr), m_pFragmentDepthsTexture(nullptr),
          m_pFragmentDepthsMemory(nullptr), m_pFragmentDepthsView(nullptr),
          m_pAccumInvAlphaTexture(nullptr), m_pAccumInvAlphaMemory(nullptr),
          m_pAccumInvAlphaView(nullptr)
    {
    }
    ~TressFXShortCut(void){};

    VkResult OnCreateDevice(VkDevice pvkDevice, int winWidth, int winHeight,
                            VkDescriptorSetLayout mesh_layout, VkSampler noiseSamplerRef,
                            VkSampler shadowSamplerRef, VkImageView depthStencilView,
                            VkImageView colorView, VkBuffer configBuffer,
                            uint64_t configBufferSize, VkImageView noiseMap,
                            VkImageView hairShadowMap, uint32_t deviceLocalMemoryIndex,
                            uint32_t width, uint32_t height);
    VkResult OnResizedSwapChain(VkDevice pvkDevice, int winWidth, int WinHeight,
                                uint32_t deviceLocalMemoryIndex);

    // Individual render pass setups.
    // Each basically sets up render state, UAVs, SRVs, and returns pixel
    // shader to use.
    void SetupDepthPass(VkDevice pvkDevice, VkCommandBuffer commandBuffer, uint32_t width,
                        uint32_t height);
    void SetupShadePass(VkDevice pvkDevice, VkCommandBuffer commandBuffer, uint32_t width,
                        uint32_t height);
    void SetupResolveDepth(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                           uint32_t width, uint32_t height);
    void SetupResolveColor(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                           uint32_t width, uint32_t height);

    void OnDestroy(bool destroyShaders);
};

} // namespace AMD
