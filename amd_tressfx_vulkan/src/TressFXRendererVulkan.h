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
#include "AMD_TressFX.h"
#include "Math\Vector3D.h"
#include "TressFXMeshVulkan.h"
#include "TressFXShortCutVulkan.h"

#define SM_HAIR_WIDTH 640
#define SM_HAIR_HEIGHT 640

namespace AMD
{

class TressFXRenderer
{
  private:
    VkDevice m_pvkDevice;
    // We need 2 pass because buffer need to be flushed inbetween
    VkRenderPass m_pRPHairRendering;
    VkRenderPass m_RPHairShadow;

    // Layout
    VkDescriptorSetLayout m_pass1_config_set_layout;
    VkPipelineLayout m_pass1_layout;
    VkDescriptorSetLayout m_pass2_set_layout;
    VkPipelineLayout m_pass2_layout;
    VkDescriptorSetLayout m_shadow_pass_config_set_layout;
    VkPipelineLayout m_shadow_pass_layout;
    VkDescriptorPool m_descriptorStorage;
    VkDescriptorSet m_pass1_config_set;
    VkDescriptorSet m_pass2_set;
    VkDescriptorSet m_shadow_pass_set;

    // Pass 1 pipeline
    VkPipeline m_pPLRenderHair;
    VkPipeline m_pPLRenderHairStrandCopies;
    VkPipeline m_pPLRenderHairAA;
    VkPipeline m_pPLRenderHairAAStrandCopies;
    // final rendering using a k-buffer for sorting the k nearest fragments
    VkPipeline m_pPLResolveKBuffer;
    // pipeline for generating the hair shadow map
    VkPipeline m_pPLGenerateHairSM;

    // samplers
    VkSampler m_pSamplerStateLinearWrap;
    VkSampler m_pSamplerStatePointClamp;
    VkSampler m_pSamplerStateCmpLess;

    // constant buffer
    VkBuffer m_pcbPerFrame;
    VkDeviceMemory m_pcbPerFrameMemory;

    // Buffers for the head of the per-pixel linked lists (PPLL)
    VkImage m_pHeadPPLLTexture;
    VkImageView m_pHeadPPLLView;

    // Buffers for the per-pixel linked list (PPLL) data
    VkBuffer m_pPPLLBuffer;

    // Buffer for the atomic counter
    VkBuffer m_pAtomicCounterPPLLBuffer;

    // textures and views
    VkImage m_pNoiseTexture;
    VkDeviceMemory m_pNoiseMemory;
    VkImageView m_pNoiseView;

    // Hair shadow map depth stencil buffer
    VkImage m_pSMHairTexture;
    VkDeviceMemory m_pSMHairMemory;
    VkImageView m_pSMHairView;

    // vertex buffer for full screen quad
    VkBuffer m_pScreenQuadVB;
    VkDeviceMemory m_pScreenQuadMemory;

    TressFXShortCut m_ShortCut;

    VkImageView m_depthBuffer;
    VkImageView m_colorTexture;
    VkFramebuffer m_renderHairFramebuffer;
    VkFramebuffer m_shadowFrameBuffer;

  public:
    VkDescriptorSetLayout m_pass1_hair_set_layout;
    VkDescriptorSetLayout m_shadow_pass_hair_set_layout;

    // hair rendering params
    TressFX_HairParams m_hairParams;

    // hair geometry
    TressFXMesh *m_pTressFXMesh;

  private:
    VkResult CreateFrameBuffer(VkDevice pvkDevice, VkImageView depthTextureView,
                               VkImageView colorTextureView, uint32_t width,
                               uint32_t height);
    VkResult CreateTextureAndViews(VkDevice pvkDevice, uint32_t MemoryIndexGPU,
                                   VkCommandBuffer commandBuffer,
                                   VkDeviceMemory scratchMemory, VkBuffer scratchBuffer,
                                   size_t &offsetInScratchBuffer);
    VkResult CreateConstantBuffer(VkDevice pvkDevice, uint32_t maxUniformBuffer,
                                  uint32_t MemoryIndexCPU);
    VkResult CreateVertexBuffers(VkDevice pvkDevice, uint32_t MemoryIndexGPU,
                                 VkCommandBuffer commandBuffer,
                                 VkDeviceMemory scratchMemory, VkBuffer scratchBuffer,
                                 size_t &offsetInScratchBuffer);
    VkResult CreateSamplers(VkDevice pvkDevice);
    VkResult CreateRenderStateObjects(VkDevice pvkDevice);
    VkResult CreatePPLL(VkDevice pvkDevice, int winWidth, int winHeight, bool resize,
                        uint32_t MemoryIndexGPU);
    VkResult CreateLayouts(VkDevice pvkDevice);
    VkResult AllocateAndPopulateSets(VkDevice pvkDevice, bool isShortcut);
    void DeletePPLL(VkDevice pvkDevice);
    void RenderScreenQuad(VkCommandBuffer pcmdbuffer, VkPipeline pPipeline);

  public:
    TressFXRenderer(void){};
    ~TressFXRenderer(void){};

    VkImageView GetShadowMapSRV() { return m_pSMHairView; };
    VkResult OnCreateDevice(VkDevice pvkDevice, int winWidth, int winHeight,
                            bool bShortCutOn, uint32_t maxUniformBuffer,
                            uint32_t MemoryIndexCPU, uint32_t MemoryIndexGPU,
                            VkImageView depthTexture, VkImageView colorTexture,
                            VkCommandBuffer commandBuffer, VkDeviceMemory scratchMemory,
                            VkBuffer scratchBuffer, size_t &offsetInScratchBuffer);
    VkResult OnResizedSwapChain(VkDevice pvkDevice, int winWidth, int WinHeight,
                                bool bShortCutOn, uint32_t MemoryIndexGPU);
    void BeginHairFrame(VkDevice pvkDevice, DirectX::XMVECTOR eyePoint,
                        DirectX::XMVECTOR lightPosition,
                        DirectX::XMMATRIX *pModelTrasnsformForHead,
                        DirectX::XMMATRIX *pViewProj,
                        DirectX::XMMATRIX *pViewProjLightOut, float screenWidth,
                        float screenHeight, bool singleHeadTransform,
                        uint32_t uniformBufferIndex);
    void GenerateShadowMap(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                           float density, uint32_t uniformBufferIndex);
    void RenderHair(VkDevice pvkDevice, VkCommandBuffer commandBuffer, uint32_t width,
                    uint32_t height, uint32_t uniformBufferIndex);
    void RenderHairShortcut(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                            uint32_t width, uint32_t height, uint32_t uniformBufferIndex);
    void EndHairFrame(VkDevice pvkDevice);
    void OnDestroy();
};

} // namespace AMD
