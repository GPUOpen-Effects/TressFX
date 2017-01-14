//--------------------------------------------------------------------------------------
// File: TressFXRenderer.cpp
//
// Main hair rendering code
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

#include "TressFXRendererVulkan.h"
#include "AMD_Types.h"
#include "TressFXOpaqueVulkan.h"
#include "TressFXPrecompiledShadersVulkan.h"
#include "UtilVulkan.h"

// Number of depth layers to use.  2 or 3 supported.
#define SHORTCUT_NUM_DEPTHS 3

// unreferenced formal parameter
#pragma warning(disable : 4100)

using namespace DirectX;

extern int g_TressFXNumVerticesPerStrand;

struct CB_PER_FRAME
{
    XMMATRIX m_mWorld;
    XMMATRIX m_mViewProj;
    XMMATRIX m_mInvViewProj;
    XMMATRIX m_mViewProjLight;

    XMFLOAT3 m_vEye;
    float m_fvFOV;

    XMFLOAT4 m_AmbientLightColor;
    XMFLOAT4 m_PointLightColor;
    XMFLOAT4 m_PointLightPos;
    XMFLOAT4 m_MatBaseColor;
    XMFLOAT4 m_MatKValue;

    float m_FiberAlpha;
    float m_HairSMAlpha;
    float m_bExpandPixels;
    float m_FiberRadius;

    XMFLOAT4 m_WinSize;

    float m_FiberSpacing;
    float m_bThinTip;
    float m_fNearLight;
    float m_fFarLight;

    int m_iTechSM;
    int m_bUseCoverage;
    int m_iStrandCopies;
    int m_iMaxFragments;

    float m_alphaThreshold;
    float m_fHairKs2;
    float m_fHairEx2;
    unsigned m_optionalSRVs;

    XMMATRIX m_mInvViewProjViewport;

    int m_mNumVerticesPerStrand;
    int m_mNumFollowHairsPerGuideHair;
    int m_bSingleHeadTransform;
    int padding0;
};

// Optional SRVs
#define PER_STRAND_TEX_COORDS 0x0001
#define PER_VERTEX_TEX_COORDS 0x0002
#define PER_VERTEX_COLORS 0x0004

struct PER_PIXEL_LINKED_LIST_STRUCT
{
    DWORD TangentAndCoverage;
    DWORD depth;
    DWORD strandColor;
    DWORD dwNext;
};

struct PPLL_BUFFERS
{
    // Buffers for the head of the per-pixel linked lists (PPLL)
    VkImage pHeadPPLL_Buffer;
    VkDeviceMemory pHeadPPLL_Memory;
    VkImageView pHeadPPLL_View;

    // Buffers for the per-pixel linked list (PPLL) data
    VkBuffer pPPLL_Buffer;
    VkDeviceMemory pPPLL_Memory;

    // Buffer for the atomic counter
    VkBuffer pAtomicCounterPLL_Buffer;
    VkDeviceMemory pAtomicCounterPLL_Memory;

    // If possible PLLL head, PPLL and atomic counter use same memory block
    VkDeviceMemory pSharedMemory;

    int width;
    int height;
    int refCount; // reference count - delete buffers when 0
};

PPLL_BUFFERS g_PPLBuffers = {VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, VK_NULL_HANDLE, 0, 0, 0};

const static UINT g_HairTotalLayers = 32;

namespace
{
//----------------------------------------------------------------------------------
//
// CreateRenderPass
//
// Create the render pass used for the non shortcut algorithm
//
//----------------------------------------------------------------------------------
VkRenderPass CreateRenderPass(VkDevice dev, VkFormat depthStencilFormat, VkFormat colorFormat)
{
    const VkAttachmentDescription attachments[] = {
        // Depth Buffer
        {0, depthStencilFormat, VK_SAMPLE_COUNT_1_BIT,
         VK_ATTACHMENT_LOAD_OP_LOAD, VK_ATTACHMENT_STORE_OP_STORE,
         VK_ATTACHMENT_LOAD_OP_CLEAR, VK_ATTACHMENT_STORE_OP_DONT_CARE,
         VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
         VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL},
        // color texture
        {0, colorFormat, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_LOAD,
         VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
         VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
         VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL}};

    const VkAttachmentReference depthStencilReference = {
        0, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
    const VkAttachmentReference colorReference = {
        1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

    const VkSubpassDescription subpasses[] = {
        {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 0, nullptr, nullptr,
         &depthStencilReference, 0, nullptr},
        {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 1, &colorReference, nullptr,
         &depthStencilReference, 0, nullptr}};

    const VkSubpassDependency dependencies[] = {
        {0, 1, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT,
         VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, 0}};

    VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                                          nullptr,
                                          0,
                                          AMD_ARRAY_SIZE(attachments),
                                          attachments,
                                          AMD_ARRAY_SIZE(subpasses),
                                          subpasses,
                                          AMD_ARRAY_SIZE(dependencies),
                                          dependencies};
    VkRenderPass result;
    vkCreateRenderPass(dev, &renderPassInfo, nullptr, &result);
    return result;
}

VkRenderPass CreateShadowRenderPass(VkDevice dev)
{
    const VkAttachmentDescription attachments[] = {
        // Depth Buffer
        {0, VK_FORMAT_D32_SFLOAT, VK_SAMPLE_COUNT_1_BIT, VK_ATTACHMENT_LOAD_OP_CLEAR,
         VK_ATTACHMENT_STORE_OP_STORE, VK_ATTACHMENT_LOAD_OP_DONT_CARE,
         VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
         VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL},
    };

    const VkAttachmentReference depthReference = {
        0, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};

    const VkSubpassDescription subpasses[] = {{0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0,
                                               nullptr, 0, nullptr, nullptr,
                                               &depthReference, 0, nullptr}};

    VkRenderPassCreateInfo renderPassInfo{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
                                          nullptr,
                                          0,
                                          AMD_ARRAY_SIZE(attachments),
                                          attachments,
                                          AMD_ARRAY_SIZE(subpasses),
                                          subpasses,
                                          0,
                                          nullptr};
    VkRenderPass result;
    vkCreateRenderPass(dev, &renderPassInfo, nullptr, &result);
    return result;
}

} // End anonymous namespace

namespace AMD
{
//--------------------------------------------------------------------------------------
//
// CreateTextureAndViews
//
// Creates the textures and corresponding views for hair rendering
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::CreateTextureAndViews(
    VkDevice pvkDevice, VkPhysicalDeviceMemoryProperties memProperties, VkCommandBuffer commandBuffer,
    VkDeviceMemory scratchMemory, VkBuffer scratchBuffer, size_t &offsetInScratchBuffer)
{
    VkResult vr;

    // Create SM DSVs for hair
    {
        VkImageCreateInfo hairShadowMapInfo = getImageCreateInfo(
            VK_FORMAT_D32_SFLOAT, SM_HAIR_WIDTH, SM_HAIR_HEIGHT,
            VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);
        AMD_CHECKED_VULKAN_CALL(
            vkCreateImage(pvkDevice, &hairShadowMapInfo, nullptr, &m_pSMHairTexture));

        m_pSMHairMemory = allocImageMemory(pvkDevice, m_pSMHairTexture, memProperties);

        VkImageViewCreateInfo dsvDesc{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        dsvDesc.format = VK_FORMAT_D32_SFLOAT;
        dsvDesc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        dsvDesc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
        dsvDesc.subresourceRange.layerCount = 1;
        dsvDesc.subresourceRange.levelCount = 1;
        dsvDesc.image = m_pSMHairTexture;
        AMD_CHECKED_VULKAN_CALL(vkCreateImageView(pvkDevice, &dsvDesc, nullptr, &m_pSMHairView));

        VkImageMemoryBarrier defineHairSMLayoutBarrier[] = {getImageMemoryBarrier(
            m_pSMHairTexture, 0, VK_ACCESS_SHADER_READ_BIT, VK_IMAGE_LAYOUT_UNDEFINED,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, VK_IMAGE_ASPECT_DEPTH_BIT)};

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0,
                             nullptr, AMD_ARRAY_SIZE(defineHairSMLayoutBarrier),
                             defineHairSMLayoutBarrier);
    }

    // Noise texture
    {
        VkImageCreateInfo noiseTextureInfo = getImageCreateInfo(
            VK_FORMAT_R32G32B32A32_SFLOAT, 512, 512,
            VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        AMD_CHECKED_VULKAN_CALL(
            vkCreateImage(pvkDevice, &noiseTextureInfo, nullptr, &m_pNoiseTexture));
        m_pNoiseMemory = allocImageMemory(pvkDevice, m_pNoiseTexture, memProperties);

        XMFLOAT4 *noiseArray = new XMFLOAT4[512 * 512];
        for (UINT i = 0; i < 512 * 512; i++)
        {
            noiseArray[i].x = rand() / (float)RAND_MAX;
            noiseArray[i].y = rand() / (float)RAND_MAX;
            noiseArray[i].z = rand() / (float)RAND_MAX;
            noiseArray[i].w = rand() / (float)RAND_MAX;
        }

        void *memoryPointer;
        vkMapMemory(pvkDevice, scratchMemory, offsetInScratchBuffer,
                    512 * 512 * sizeof(XMFLOAT4), 0, &memoryPointer);
        memcpy(memoryPointer, noiseArray, 512 * 512 * sizeof(XMFLOAT4));
        vkUnmapMemory(pvkDevice, scratchMemory);

        VkImageMemoryBarrier makeNoiseTextureTransferableBarrier[] = {
            getImageMemoryBarrier(m_pNoiseTexture, 0, VK_ACCESS_TRANSFER_WRITE_BIT,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, VK_IMAGE_ASPECT_COLOR_BIT)};

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0,
                             nullptr, AMD_ARRAY_SIZE(makeNoiseTextureTransferableBarrier),
                             makeNoiseTextureTransferableBarrier);

        VkBufferImageCopy copyInfo{offsetInScratchBuffer, 512, 512};
        copyInfo.imageExtent.width = 512;
        copyInfo.imageExtent.height = 512;
        copyInfo.imageExtent.depth = 1;
        copyInfo.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        copyInfo.imageSubresource.layerCount = 1;
        vkCmdCopyBufferToImage(commandBuffer, scratchBuffer, m_pNoiseTexture,
                               VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyInfo);
        offsetInScratchBuffer += 512 * 512 * sizeof(XMFLOAT4);

        VkImageMemoryBarrier makeNoiseTextureSampleableBarrier[] = {getImageMemoryBarrier(
            m_pNoiseTexture, VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT,
            VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, 1, VK_IMAGE_ASPECT_COLOR_BIT)};

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
                             VK_PIPELINE_STAGE_ALL_COMMANDS_BIT, 0, 0, nullptr, 0,
                             nullptr, AMD_ARRAY_SIZE(makeNoiseTextureSampleableBarrier),
                             makeNoiseTextureSampleableBarrier);

        delete[] noiseArray;

        VkImageViewCreateInfo srDesc{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        srDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        srDesc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srDesc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        srDesc.subresourceRange.layerCount = 1;
        srDesc.subresourceRange.levelCount = 1;
        srDesc.image = m_pNoiseTexture;

        AMD_CHECKED_VULKAN_CALL(vkCreateImageView(pvkDevice, &srDesc, nullptr, &m_pNoiseView));
    }

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// CreateConstantBuffers
//
// Creates the constant buffers for hair rendering
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::CreateConstantBuffer(VkDevice pvkDevice,
                                               uint32_t maxUniformBuffer,
                                               VkPhysicalDeviceMemoryProperties memProperties)
{
    VkResult vr;
    VkBufferCreateInfo cbDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    cbDesc.usage = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
    cbDesc.size = maxUniformBuffer * sizeof(CB_PER_FRAME);
    AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &cbDesc, nullptr, &m_pcbPerFrame));
    m_pcbPerFrameMemory = allocBufferMemory(pvkDevice, m_pcbPerFrame, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    return VK_SUCCESS;
}

VkResult TressFXRenderer::CreateFrameBuffer(VkDevice pvkDevice,
                                            VkImageView depthTextureView,
                                            VkImageView colorTexture, uint32_t width,
                                            uint32_t height)
{
    m_depthBuffer = depthTextureView;
    m_colorTexture = colorTexture;
    VkResult vr;

    {
        VkImageView attachements[] = {m_depthBuffer, m_colorTexture};
        VkFramebufferCreateInfo info{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        info.renderPass = m_pRPHairRendering;
        info.attachmentCount = AMD_ARRAY_SIZE(attachements);
        info.pAttachments = attachements;
        info.width = width;
        info.height = height;
        info.layers = 1;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateFramebuffer(pvkDevice, &info, nullptr, &m_renderHairFramebuffer));
    }

    VkImageView attachements[1] = {m_pSMHairView};
    VkFramebufferCreateInfo info{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
    info.renderPass = m_RPHairShadow;
    info.attachmentCount = AMD_ARRAY_SIZE(attachements);
    info.pAttachments = attachements;
    info.width = SM_HAIR_WIDTH;
    info.height = SM_HAIR_HEIGHT;
    info.layers = 1;
    AMD_CHECKED_VULKAN_CALL(vkCreateFramebuffer(pvkDevice, &info, nullptr, &m_shadowFrameBuffer));
    return VK_SUCCESS;
}

VkResult TressFXRenderer::CreateSamplers(VkDevice pvkDevice)
{
    VkResult vr;

    // Create sampler state objects
    VkSamplerCreateInfo samDesc{VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO};
    samDesc.minFilter = VK_FILTER_LINEAR;
    samDesc.magFilter = VK_FILTER_LINEAR;
    samDesc.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samDesc.addressModeU = samDesc.addressModeV = samDesc.addressModeW =
        VK_SAMPLER_ADDRESS_MODE_REPEAT;
    samDesc.maxAnisotropy = 16.f;
    samDesc.compareOp = VK_COMPARE_OP_NEVER;
    samDesc.minLod = 0.f;
    samDesc.maxLod = 1000.f;
    AMD_CHECKED_VULKAN_CALL(
        vkCreateSampler(pvkDevice, &samDesc, nullptr, &m_pSamplerStateLinearWrap));

    samDesc.minFilter = VK_FILTER_NEAREST;
    samDesc.magFilter = VK_FILTER_NEAREST;
    samDesc.mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST;
    AMD_CHECKED_VULKAN_CALL(
        vkCreateSampler(pvkDevice, &samDesc, nullptr, &m_pSamplerStatePointClamp));

    samDesc.minFilter = VK_FILTER_LINEAR;
    samDesc.magFilter = VK_FILTER_LINEAR;
    samDesc.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samDesc.compareEnable = true, samDesc.compareOp = VK_COMPARE_OP_LESS;
    samDesc.borderColor = VK_BORDER_COLOR_INT_OPAQUE_WHITE;
    samDesc.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samDesc.addressModeV = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    samDesc.addressModeW = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
    AMD_CHECKED_VULKAN_CALL(vkCreateSampler(pvkDevice, &samDesc, nullptr, &m_pSamplerStateCmpLess));

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// CreateRenderStateObjects
//
// Creates the render state objects for hair rendering
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::CreateRenderStateObjects(VkDevice pvkDevice, VkFormat depthStencilFormat, VkFormat colorFormat)
{
    VkResult vr;

    m_pRPHairRendering = CreateRenderPass(pvkDevice, depthStencilFormat, colorFormat);
    ShaderModule fragmentShadersModule(pvkDevice, pass1_fragment);

    ShaderModule vertexShadersModule(pvkDevice, render_hair_aa_strand_copies_vertex);
    VkPipelineShaderStageCreateInfo renderHairAAStrandCopiesStage[2] = {
        getShaderStageCreateInfo(vertexShadersModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(fragmentShadersModule.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };
    VkGraphicsPipelineCreateInfo renderHairAAStrandCopiesDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::ColorWritesOff,
            &CommonPipelineState::inputAssemblyTriangle, renderHairAAStrandCopiesStage,
            m_pass1_layout, m_pRPHairRendering, 0);

    ShaderModule RenderHairAAVertexShader(pvkDevice, render_hair_aa_vertex);
    VkPipelineShaderStageCreateInfo renderHairAAStage[2] = {
        getShaderStageCreateInfo(RenderHairAAVertexShader.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(fragmentShadersModule.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };
    VkGraphicsPipelineCreateInfo renderHairAADesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::ColorWritesOff,
            &CommonPipelineState::inputAssemblyTriangle, renderHairAAStage,
            m_pass1_layout, m_pRPHairRendering, 0);

    ShaderModule renderHairStrandCopiesVertexModule(pvkDevice,
                                                    render_hair_strand_copies_vertex);
    VkPipelineShaderStageCreateInfo renderHairStrandCopiesStage[2] = {
        getShaderStageCreateInfo(renderHairStrandCopiesVertexModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(fragmentShadersModule.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };
    VkGraphicsPipelineCreateInfo renderHairStrandCopiesDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::ColorWritesOff,
            &CommonPipelineState::inputAssemblyTriangle, renderHairStrandCopiesStage,
            m_pass1_layout, m_pRPHairRendering, 0);

    ShaderModule renderHairVertexModule(pvkDevice, render_hair_vertex);
    VkPipelineShaderStageCreateInfo renderHairStage[2] = {
        getShaderStageCreateInfo(renderHairVertexModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(fragmentShadersModule.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };
    VkGraphicsPipelineCreateInfo renderHairDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::ColorWritesOff,
            &CommonPipelineState::inputAssemblyTriangle, renderHairStage, m_pass1_layout,
            m_pRPHairRendering, 0);

    ShaderModule pass2VSModule(pvkDevice, pass2_vertex);
    ShaderModule pass2FSModule(pvkDevice, pass2_fragment);
    VkPipelineShaderStageCreateInfo renderKBufferStage[2] = {
        getShaderStageCreateInfo(pass2VSModule.m_shaderModule, VK_SHADER_STAGE_VERTEX_BIT,
                                 "main"),
        getShaderStageCreateInfo(pass2FSModule.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    VkGraphicsPipelineCreateInfo renderKBufferDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutQuad,
            &CommonPipelineState::DepthTestDisabledStencilTestLessDSS,
            &CommonPipelineState::BlendStateBlendToBg,
            &CommonPipelineState::inputAssemblyTriangle, renderKBufferStage,
            m_pass2_layout, m_pRPHairRendering, 1);

    m_RPHairShadow = CreateShadowRenderPass(pvkDevice);

    ShaderModule shadowVSModule(pvkDevice, hair_shadow_vertex);
    ShaderModule shadowFSModule(pvkDevice, hair_shadow_fragment);
    VkPipelineShaderStageCreateInfo renderHairShadow[2] = {
        getShaderStageCreateInfo(shadowVSModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(shadowFSModule.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    VkGraphicsPipelineCreateInfo renderHairShadowDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledDesc,
            &CommonPipelineState::ColorWritesOff, &CommonPipelineState::inputAssemblyLine,
            renderHairShadow, m_shadow_pass_layout, m_RPHairShadow, 0);

    VkGraphicsPipelineCreateInfo pipelineInfoArray[] = {renderHairAAStrandCopiesDesc,
                                                        renderHairAADesc,
                                                        renderHairStrandCopiesDesc,
                                                        renderHairDesc,
                                                        renderKBufferDesc,
                                                        renderHairShadowDesc};

    VkPipeline pipelineArray[6] = {};

    AMD_CHECKED_VULKAN_CALL(vkCreateGraphicsPipelines(pvkDevice, VK_NULL_HANDLE,
                                           AMD_ARRAY_SIZE(pipelineInfoArray),
                                           pipelineInfoArray, nullptr, pipelineArray));

    m_pPLRenderHairAAStrandCopies = pipelineArray[0];
    m_pPLRenderHairAA = pipelineArray[1];
    m_pPLRenderHairStrandCopies = pipelineArray[2];
    m_pPLRenderHair = pipelineArray[3];
    m_pPLResolveKBuffer = pipelineArray[4];
    m_pPLGenerateHairSM = pipelineArray[5];

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// OnCreateDevice
//
// Called when the device is created to create resources for hair rendering
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::OnCreateDevice(
    VkDevice pvkDevice, int winWidth, int winHeight, bool bShortCutOn,
    uint32_t maxUniformBuffer, VkPhysicalDeviceMemoryProperties memProperties,
    VkImageView depthTexture, VkImageView colorTexture, VkCommandBuffer commandBuffer,
    VkDeviceMemory scratchMemory, VkBuffer scratchBuffer, size_t &offsetInScratchBuffer,
    VkFormat depthStencilFormat, VkFormat colorFormat)
{
    m_pvkDevice = pvkDevice;
    VkResult vr;

    AMD_CHECKED_VULKAN_CALL(CreateTextureAndViews(pvkDevice, memProperties, commandBuffer,
                                       scratchMemory, scratchBuffer,
                                       offsetInScratchBuffer));
    AMD_CHECKED_VULKAN_CALL(CreateConstantBuffer(pvkDevice, maxUniformBuffer, memProperties));
    AMD_CHECKED_VULKAN_CALL(CreateSamplers(pvkDevice));
    AMD_CHECKED_VULKAN_CALL(CreateLayouts(pvkDevice));
    if (!bShortCutOn)
    {
        AMD_CHECKED_VULKAN_CALL(
            CreatePPLL(pvkDevice, winWidth, winHeight, false, memProperties));
        VkImageMemoryBarrier defineHeadPPLLBarrier[] = {getImageMemoryBarrier(
            m_pHeadPPLLTexture, 0, VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
            VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL, 1, VK_IMAGE_ASPECT_COLOR_BIT)};
        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0,
                             nullptr, AMD_ARRAY_SIZE(defineHeadPPLLBarrier),
                             defineHeadPPLLBarrier);
    }
    else
    {
        m_ShortCut.OnCreateDevice(pvkDevice, winWidth, winHeight, m_pass1_hair_set_layout,
                                  m_pSamplerStateLinearWrap, m_pSamplerStatePointClamp,
                                  depthTexture, colorTexture, m_pcbPerFrame,
                                  sizeof(CB_PER_FRAME), m_pNoiseView, m_pSMHairView,
                                  memProperties, winWidth, winHeight, depthStencilFormat, colorFormat);

        VkImageMemoryBarrier barriers[] = {
            getImageMemoryBarrier(m_ShortCut.m_pAccumInvAlphaTexture, 0,
                                  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                      VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, VK_IMAGE_ASPECT_COLOR_BIT),
            getImageMemoryBarrier(m_ShortCut.m_pFragmentDepthsTexture, 0,
                                  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                      VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
                                  VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL,
                                  SHORTCUT_NUM_DEPTHS, VK_IMAGE_ASPECT_COLOR_BIT),
            getImageMemoryBarrier(m_ShortCut.m_pFragmentColorsTexture, 0,
                                  VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT |
                                      VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
                                  VK_IMAGE_LAYOUT_UNDEFINED,
                                  VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, 1, VK_IMAGE_ASPECT_COLOR_BIT)};
        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                             VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr, 0,
                             nullptr, AMD_ARRAY_SIZE(barriers), barriers);
    }

    AMD_CHECKED_VULKAN_CALL(CreateRenderStateObjects(pvkDevice, depthStencilFormat, colorFormat));
    AMD_CHECKED_VULKAN_CALL(AllocateAndPopulateSets(pvkDevice, bShortCutOn));
    AMD_CHECKED_VULKAN_CALL(
        CreateFrameBuffer(pvkDevice, depthTexture, colorTexture, winWidth, winHeight));

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// CreateRenderStateObjects
//
// Creates the pipelines for hair rendering
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::CreateLayouts(VkDevice pvkDevice)
{
    VkResult vr;

    // First pass model dependent set layout
    {
        const VkDescriptorSetLayoutBinding pass1_hair_bindings[] = {
            // Hair Pos
            {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
            // Hair Tangent
            {IDSRV_HAIR_TANGENTS, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
            // Hair Thickness
            {IDSRV_HAIR_THICKNESSES, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
        };
        AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, pass1_hair_bindings,
                                         AMD_ARRAY_SIZE(pass1_hair_bindings),
                                         m_pass1_hair_set_layout));
    }

    // First pass non model dependent set layout
    {
        VkDescriptorSetLayoutBinding pass1_config_bindings[] = {
            // TressFX parameters
            {IDSRV_CONSTANTS_BUFFER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1,
             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
            // Atomic counter
            {IDSRV_ATOMIC_COUNTER_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // PPLL Head
            {IDSRV_HEAD_PPLL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // PPLL List
            {IDSRV_PPLL, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // Sampler for noise texture in VS
            {IDSRV_NOISE_SAMPLER, VK_DESCRIPTOR_TYPE_SAMPLER, 1,
             VK_SHADER_STAGE_VERTEX_BIT, &m_pSamplerStateLinearWrap},
            // Noise texture
            {IDSRV_NOISEMAP, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
        };
        AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, pass1_config_bindings,
                                         AMD_ARRAY_SIZE(pass1_config_bindings),
                                         m_pass1_config_set_layout));
    }

    VkDescriptorSetLayout pass1_descriptor_set_layouts[] = {m_pass1_config_set_layout,
                                                            m_pass1_hair_set_layout};
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, 0,
        AMD_ARRAY_SIZE(pass1_descriptor_set_layouts), pass1_descriptor_set_layouts};
    AMD_CHECKED_VULKAN_CALL(
        vkCreatePipelineLayout(pvkDevice, &pipelineLayoutInfo, nullptr, &m_pass1_layout));

    // Pass 2 set layout
    {
        VkDescriptorSetLayoutBinding pass2_bindings[] = {
            // TressFX parameters
            {IDSRV_CONSTANTS_BUFFER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1,
             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
            // PPLL Head
            {IDSRV_HEAD_PPLL, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // PPLL List
            {IDSRV_PPLL, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // Shadow texture
            {IDSRV_HAIRSM, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // Sampler for hair shadow texture
            {IDSRV_SHADOW_SAMPLER, VK_DESCRIPTOR_TYPE_SAMPLER, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT, &m_pSamplerStatePointClamp},
        };
        AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, pass2_bindings,
                                         AMD_ARRAY_SIZE(pass2_bindings),
                                         m_pass2_set_layout));
    }

    VkPipelineLayoutCreateInfo pass2PipelineLayoutInfo{
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, 0, 1,
        &m_pass2_set_layout};
    AMD_CHECKED_VULKAN_CALL(vkCreatePipelineLayout(pvkDevice, &pass2PipelineLayoutInfo, nullptr,
                                        &m_pass2_layout));

    // Shadow pass model dependent set layout
    {
        VkDescriptorSetLayoutBinding shadow_pass_hair_bindings[] = {
            // TressFX parameters
            {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
        };
        AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, shadow_pass_hair_bindings,
                                         AMD_ARRAY_SIZE(shadow_pass_hair_bindings),
                                         m_shadow_pass_hair_set_layout));
    }

    // Shadow pass non model dependent set layout
    {
        VkDescriptorSetLayoutBinding shadow_pass_config_bindings[] = {
            // TressFX parameters
            {IDSRV_CONSTANTS_BUFFER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1,
             VK_SHADER_STAGE_VERTEX_BIT}};
        AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, shadow_pass_config_bindings,
                                         AMD_ARRAY_SIZE(shadow_pass_config_bindings),
                                         m_shadow_pass_config_set_layout));
    }

    VkDescriptorSetLayout shadow_pass_descriptor_set_layouts[] = {
        m_shadow_pass_config_set_layout, m_shadow_pass_hair_set_layout};
    VkPipelineLayoutCreateInfo shadowPassPipelineLayoutInfo{
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO, nullptr, 0,
        AMD_ARRAY_SIZE(shadow_pass_descriptor_set_layouts),
        shadow_pass_descriptor_set_layouts};
    AMD_CHECKED_VULKAN_CALL(vkCreatePipelineLayout(pvkDevice, &shadowPassPipelineLayoutInfo, nullptr,
                                        &m_shadow_pass_layout));

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// AllocateAndPopulateSets
//
// Allocate descriptor set and fills them
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::AllocateAndPopulateSets(VkDevice pvkDevice, bool isShortcut)
{
    VkResult vr;
    const VkDescriptorPoolSize descriptorPoolSize[] = {
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 3},
        {VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 3},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 3},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 2},
        {VK_DESCRIPTOR_TYPE_SAMPLER, 3},
        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 2}};

    VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    descriptorPoolCreateInfo.maxSets = 3;
    descriptorPoolCreateInfo.poolSizeCount = AMD_ARRAY_SIZE(descriptorPoolSize);
    descriptorPoolCreateInfo.pPoolSizes = descriptorPoolSize;
    AMD_CHECKED_VULKAN_CALL(vkCreateDescriptorPool(pvkDevice, &descriptorPoolCreateInfo, nullptr,
                                        &m_descriptorStorage));

    VkDescriptorBufferInfo bufferDescriptor{m_pcbPerFrame, 0, sizeof(CB_PER_FRAME)};
    VkDescriptorSetAllocateInfo allocateInfo{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    allocateInfo.descriptorPool = m_descriptorStorage;
    allocateInfo.descriptorSetCount = 1;
    allocateInfo.pSetLayouts = &m_shadow_pass_config_set_layout;
    AMD_CHECKED_VULKAN_CALL(
        vkAllocateDescriptorSets(pvkDevice, &allocateInfo, &m_shadow_pass_set));
    if (!isShortcut)
    {
        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &m_pass1_config_set_layout;
        AMD_CHECKED_VULKAN_CALL(
            vkAllocateDescriptorSets(pvkDevice, &allocateInfo, &m_pass1_config_set));

        allocateInfo.descriptorSetCount = 1;
        allocateInfo.pSetLayouts = &m_pass2_set_layout;
        AMD_CHECKED_VULKAN_CALL(vkAllocateDescriptorSets(pvkDevice, &allocateInfo, &m_pass2_set));

        VkDescriptorBufferInfo atomicCounterDescriptor{m_pAtomicCounterPPLLBuffer, 0,
                                                       sizeof(unsigned int)};
        VkDescriptorBufferInfo PPLL_Buffer_Descriptor{
            m_pPPLLBuffer, 0,
            g_HairTotalLayers * 1024 * 1024 * sizeof(PER_PIXEL_LINKED_LIST_STRUCT)};
        VkDescriptorImageInfo headPLLDescriptor{VK_NULL_HANDLE, m_pHeadPPLLView,
                                                VK_IMAGE_LAYOUT_GENERAL};
        VkDescriptorImageInfo noiseDescriptor{VK_NULL_HANDLE, m_pNoiseView,
                                              VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
        VkDescriptorImageInfo shadowDescriptor{VK_NULL_HANDLE, m_pSMHairView,
                                               VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

        VkWriteDescriptorSet writeDescriptorSets[] = {
            getWriteDescriptor(m_pass1_config_set, IDSRV_CONSTANTS_BUFFER,
                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                               &bufferDescriptor),
            getWriteDescriptor(m_pass1_config_set, IDSRV_ATOMIC_COUNTER_BUFFER,
                               VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                               &atomicCounterDescriptor),
            getWriteDescriptor(m_pass1_config_set, IDSRV_HEAD_PPLL,
                               VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, &headPLLDescriptor),
            getWriteDescriptor(m_pass1_config_set, IDSRV_PPLL,
                               VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                               &PPLL_Buffer_Descriptor),
            getWriteDescriptor(m_pass1_config_set, IDSRV_NOISEMAP,
                               VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, &noiseDescriptor),
            getWriteDescriptor(m_pass2_set, IDSRV_CONSTANTS_BUFFER,
                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                               &bufferDescriptor),
            getWriteDescriptor(m_pass2_set, IDSRV_HEAD_PPLL,
                               VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, &headPLLDescriptor),
            getWriteDescriptor(m_pass2_set, IDSRV_PPLL, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
                               &PPLL_Buffer_Descriptor),
            getWriteDescriptor(m_pass2_set, IDSRV_HAIRSM,
                               VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, &shadowDescriptor),
            getWriteDescriptor(m_shadow_pass_set, IDSRV_CONSTANTS_BUFFER,
                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                               &bufferDescriptor),
        };

        vkUpdateDescriptorSets(pvkDevice, AMD_ARRAY_SIZE(writeDescriptorSets),
                               writeDescriptorSets, 0, nullptr);
    }
    else
    {
        VkWriteDescriptorSet writeDescriptorSets[] = {
            getWriteDescriptor(m_shadow_pass_set, IDSRV_CONSTANTS_BUFFER,
                               VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
                               &bufferDescriptor),
        };

        vkUpdateDescriptorSets(pvkDevice, AMD_ARRAY_SIZE(writeDescriptorSets),
                               writeDescriptorSets, 0, nullptr);
    }

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// CreatePPLL
//
// Creates the per pixel linked list buffers and views. To save space, the PPLL is
// shared between multiple TressFXRenderer objects. The ref count gets incremented
// when the PPLL is shared with a new TressFXRenderer object.
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::CreatePPLL(VkDevice pvkDevice, int winWidth, int winHeight,
                                     bool resize, VkPhysicalDeviceMemoryProperties memProperties)
{
    VkResult vr;

    // see if the buffer needs to be resized or if refCount is 0
    if ((winWidth != g_PPLBuffers.width) || (winHeight != g_PPLBuffers.height) ||
        (g_PPLBuffers.refCount == 0))
    {
        // linked list head texture
        VkImageCreateInfo headPPLLInfo =
            getImageCreateInfo(VK_FORMAT_R32_UINT, winWidth, winHeight,
                               VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT);
        AMD_CHECKED_VULKAN_CALL(vkCreateImage(pvkDevice, &headPPLLInfo, nullptr,
                                   &g_PPLBuffers.pHeadPPLL_Buffer));

        // Per-pixel Linked List (PPLL) buffer
        VkBufferCreateInfo BufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        BufferDesc.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
        BufferDesc.size = (DWORD)(g_HairTotalLayers * winWidth * winHeight *
                                  sizeof(PER_PIXEL_LINKED_LIST_STRUCT));
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBuffer(pvkDevice, &BufferDesc, nullptr, &g_PPLBuffers.pPPLL_Buffer));

        // Atomic counter buffer
        BufferDesc.usage =
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        BufferDesc.size = sizeof(unsigned int);
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &BufferDesc, nullptr,
                                    &g_PPLBuffers.pAtomicCounterPLL_Buffer));

        VkMemoryRequirements headPPLLMemReq;
        vkGetImageMemoryRequirements(pvkDevice, g_PPLBuffers.pHeadPPLL_Buffer, &headPPLLMemReq);
        VkMemoryRequirements PPLBufferMemReq;
        vkGetBufferMemoryRequirements(pvkDevice, g_PPLBuffers.pPPLL_Buffer, &PPLBufferMemReq);
        VkMemoryRequirements atomicBufferMemReq;
        vkGetBufferMemoryRequirements(pvkDevice, g_PPLBuffers.pAtomicCounterPLL_Buffer, &atomicBufferMemReq);

        uint32_t typeBits = headPPLLMemReq.memoryTypeBits & PPLBufferMemReq.memoryTypeBits & atomicBufferMemReq.memoryTypeBits;
		uint32_t memoryType = getMemoryTypeIndex(typeBits, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        if (memoryType == -1)
        {
			// Split in 3
            g_PPLBuffers.pHeadPPLL_Memory = allocImageMemory(
                pvkDevice, g_PPLBuffers.pHeadPPLL_Buffer, memProperties);

            g_PPLBuffers.pPPLL_Memory =
                allocBufferMemory(pvkDevice, g_PPLBuffers.pPPLL_Buffer, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

            g_PPLBuffers.pAtomicCounterPLL_Memory = allocBufferMemory(
                pvkDevice, g_PPLBuffers.pAtomicCounterPLL_Buffer, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        }
        else
        {
			VkDeviceSize PPLBufferOffset = align(headPPLLMemReq.size, PPLBufferMemReq.alignment);
			VkDeviceSize atomicBufferOffset = align(PPLBufferOffset + PPLBufferMemReq.size, atomicBufferMemReq.alignment);
			VkDeviceSize totalSize = atomicBufferOffset + atomicBufferMemReq.size;

			VkMemoryAllocateInfo allocate{ VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO };
			allocate.allocationSize = totalSize;
			allocate.memoryTypeIndex = memoryType;
			AMD_CHECKED_VULKAN_CALL(vkAllocateMemory(pvkDevice, &allocate, NULL, &g_PPLBuffers.pSharedMemory));

			AMD_CHECKED_VULKAN_CALL(vkBindImageMemory(pvkDevice, g_PPLBuffers.pHeadPPLL_Buffer, g_PPLBuffers.pSharedMemory, 0));
			AMD_CHECKED_VULKAN_CALL(vkBindBufferMemory(pvkDevice, g_PPLBuffers.pPPLL_Buffer, g_PPLBuffers.pSharedMemory, PPLBufferOffset));
			AMD_CHECKED_VULKAN_CALL(vkBindBufferMemory(pvkDevice, g_PPLBuffers.pAtomicCounterPLL_Buffer, g_PPLBuffers.pSharedMemory, atomicBufferOffset));
		}


        // View for linked list head
        VkImageViewCreateInfo srDesc{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        srDesc.format = VK_FORMAT_R32_UINT;
        srDesc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srDesc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        srDesc.subresourceRange.layerCount = 1;
        srDesc.subresourceRange.levelCount = 1;
        srDesc.image = g_PPLBuffers.pHeadPPLL_Buffer;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateImageView(pvkDevice, &srDesc, nullptr, &g_PPLBuffers.pHeadPPLL_View));



        // update the width and height
        g_PPLBuffers.width = winWidth;
        g_PPLBuffers.height = winHeight;

        // if the refCount is non-zero, then we're just resizing the buffers
        if (g_PPLBuffers.refCount == 0)
        {
            g_PPLBuffers.refCount++;
        }
    }
    else
    {
        if (!resize)
        {
            g_PPLBuffers.refCount++;
        }
    }

    m_pHeadPPLLTexture = g_PPLBuffers.pHeadPPLL_Buffer;
    m_pHeadPPLLView = g_PPLBuffers.pHeadPPLL_View;
    m_pPPLLBuffer = g_PPLBuffers.pPPLL_Buffer;
    m_pAtomicCounterPPLLBuffer = g_PPLBuffers.pAtomicCounterPLL_Buffer;

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// DeletePPLL
//
// Deletes the PPLL buffers when the refCount goes to 0
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::DeletePPLL(VkDevice pvkDevice)
{
    if (g_PPLBuffers.refCount == 0)
    {
        return;
    }

    g_PPLBuffers.refCount--;

    m_pHeadPPLLTexture = NULL;
    m_pHeadPPLLView = NULL;
    m_pPPLLBuffer = NULL;

    if (g_PPLBuffers.refCount == 0)
    {
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_View, vkDestroyImageView, pvkDevice);
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_Buffer, vkDestroyImage, pvkDevice);
        AMD_SAFE_RELEASE(g_PPLBuffers.pHeadPPLL_Memory, vkFreeMemory, pvkDevice);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_Buffer, vkDestroyBuffer, pvkDevice);
        AMD_SAFE_RELEASE(g_PPLBuffers.pPPLL_Memory, vkFreeMemory, pvkDevice);
        AMD_SAFE_RELEASE(g_PPLBuffers.pAtomicCounterPLL_Buffer, vkDestroyBuffer,
                         pvkDevice);
        AMD_SAFE_RELEASE(g_PPLBuffers.pAtomicCounterPLL_Memory, vkFreeMemory, pvkDevice);
		AMD_SAFE_RELEASE(g_PPLBuffers.pSharedMemory, vkFreeMemory, pvkDevice);

        g_PPLBuffers.width = 0;
        g_PPLBuffers.height = 0;
    }
}

//--------------------------------------------------------------------------------------
//
// OnResizedSwapChain
//
// Called when the swap chain is being resized. Allocate resources that need to be
// created at this time because they are dependent on the size of the frame buffer.
//
//--------------------------------------------------------------------------------------
VkResult TressFXRenderer::OnResizedSwapChain(VkDevice pvkDevice, int width, int height,
                                             bool bShortCutOn,
                                             VkPhysicalDeviceMemoryProperties memProperties)
{
    VkResult vr;
    if (bShortCutOn)
    {
        // AMD_CHECKED_VULKAN_CALL(m_ShortCut.OnResizedSwapChain(pd3dDevice, width, height));
    }
    else
    {
        AMD_CHECKED_VULKAN_CALL(CreatePPLL(pvkDevice, width, height, true, memProperties));
    }
    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// BeginHairFrame
//
// Start of hair rendering.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::BeginHairFrame(
    VkDevice pvkDevice, DirectX::XMVECTOR eyePoint, DirectX::XMVECTOR lightPosition,
    DirectX::XMMATRIX *pModelTransformForHead, DirectX::XMMATRIX *pViewProj,
    DirectX::XMMATRIX *pViewProjLightOut, float screenWidth, float screenHeight,
    bool singleHeadTransform, uint32_t uniformBufferIndex)
{
    // Set up camera parameters for when the camera is at the position of the light for
    // rendering the shadow map
    XMMATRIX mViewLight, mProjLight;
    XMVECTOR modelCenter = XMVector3TransformCoord(
        XMLoadFloat3(&m_pTressFXMesh->m_HairAsset.m_bSphere.center),
        *pModelTransformForHead);
    XMVECTOR vLightAt = modelCenter;
    XMVECTOR vUp = XMVectorSet(0, 1, 0, 0);
    mViewLight = XMMatrixLookAtLH(lightPosition, vLightAt, vUp);

    XMVECTOR vLightToObject = XMVectorSubtract(lightPosition, modelCenter);
    float dis = XMVectorGetX(XMVector3Length(vLightToObject));

    float min_dis = max(0.001f, dis - m_pTressFXMesh->m_HairAsset.m_bSphere.radius);
    float max_dis = dis + m_pTressFXMesh->m_HairAsset.m_bSphere.radius;

    float halfAngle = 1.5f * asin(m_pTressFXMesh->m_HairAsset.m_bSphere.radius / dis);
    float FOV = 2 * halfAngle;

    float ratio = 1;

    mProjLight = XMMatrixPerspectiveFovLH(FOV, ratio, min_dis, max_dis);
    *pViewProjLightOut = mViewLight * mProjLight;

    // Map the per-frame constant buffer
    CB_PER_FRAME *pcbPerFrame;
    vkMapMemory(pvkDevice, m_pcbPerFrameMemory, uniformBufferIndex * sizeof(CB_PER_FRAME),
                sizeof(CB_PER_FRAME), 0, reinterpret_cast<void **>(&pcbPerFrame));

    // camera parameters
    XMMATRIX mViewProj = *pViewProj;
    XMMATRIX mInvViewProj = XMMatrixInverse(0, mViewProj);

    float fRenderWidth = screenWidth;
    float fRenderHeight = screenHeight;

    // Inverse of viewprojection matrix with viewport mapping
    XMMATRIX mViewport(2.0f / fRenderWidth, 0.0f, 0.0f, 0.0f, 0.0f, -2.0f / fRenderHeight,
                       0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 1.0f);

    XMMATRIX mInvViewProjViewport = mInvViewProj;

    pcbPerFrame->m_mViewProj = XMMatrixTranspose(mViewProj);
    pcbPerFrame->m_mInvViewProj = XMMatrixTranspose(mInvViewProj);

    pcbPerFrame->m_mInvViewProjViewport = XMMatrixTranspose(mInvViewProjViewport);

    pcbPerFrame->m_mWorld = XMMatrixTranspose(*pModelTransformForHead);

    XMStoreFloat3(&pcbPerFrame->m_vEye, eyePoint);

    pcbPerFrame->m_fvFOV = XM_PI / 4;

    // Light camera parameters
    pcbPerFrame->m_mViewProjLight = *pViewProjLightOut;
    pcbPerFrame->m_fNearLight = min_dis;
    pcbPerFrame->m_fFarLight = max_dis;

    XMStoreFloat4(&pcbPerFrame->m_PointLightPos, lightPosition);
    pcbPerFrame->m_PointLightPos.w = 1;

    // scene light color
    pcbPerFrame->m_AmbientLightColor = m_hairParams.ambientLightColor;
    pcbPerFrame->m_PointLightColor = m_hairParams.pointLightColor;

    // hair material
    pcbPerFrame->m_MatBaseColor =
        XMFLOAT4(m_hairParams.color.x, m_hairParams.color.y, m_hairParams.color.z, 1);
    pcbPerFrame->m_MatKValue =
        XMFLOAT4(m_hairParams.Ka, m_hairParams.Kd, m_hairParams.Ks1, m_hairParams.Ex1);
    pcbPerFrame->m_fHairKs2 = m_hairParams.Ks2;
    pcbPerFrame->m_fHairEx2 = m_hairParams.Ex2;

    pcbPerFrame->m_FiberAlpha = m_hairParams.alpha;
    pcbPerFrame->m_HairSMAlpha = m_hairParams.shadowMapAlpha;

    pcbPerFrame->m_FiberRadius = m_hairParams.thickness;

    pcbPerFrame->m_FiberSpacing = m_hairParams.duplicateStrandSpacing;

    pcbPerFrame->m_bThinTip = (m_hairParams.bThinTip ? 1.f : -1.f);
    pcbPerFrame->m_bExpandPixels = 1;

    pcbPerFrame->m_WinSize =
        XMFLOAT4((float)screenWidth, (float)screenHeight, 1.0f / (float)screenWidth,
                 1.0f / (float)screenHeight);

    pcbPerFrame->m_iMaxFragments = m_hairParams.maxFragments;

    pcbPerFrame->m_alphaThreshold = m_hairParams.alphaThreshold;

    pcbPerFrame->m_iTechSM = m_hairParams.shadowTechnique;
    pcbPerFrame->m_bUseCoverage = m_hairParams.bAntialias ? 1 : 0;
    pcbPerFrame->m_iStrandCopies = m_hairParams.strandCopies;

    pcbPerFrame->m_mNumVerticesPerStrand = g_TressFXNumVerticesPerStrand;
    pcbPerFrame->m_mNumFollowHairsPerGuideHair =
        m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair;
    pcbPerFrame->m_bSingleHeadTransform = singleHeadTransform;

    unsigned optionalSRVs = 0;
    if ((m_pTressFXMesh->m_pStrandTexCoordView) && (m_pTressFXMesh->m_pHairTextureSRV))
    {
        optionalSRVs |= PER_STRAND_TEX_COORDS;
        // ignore the material base color when getting hair color from the texture
        pcbPerFrame->m_MatBaseColor = XMFLOAT4(1, 1, 1, 1);
    }
    pcbPerFrame->m_optionalSRVs = optionalSRVs;

    vkUnmapMemory(pvkDevice, m_pcbPerFrameMemory);
}

//--------------------------------------------------------------------------------------
//
// RenderScreenQuad
//
// Renders a triangle big enough to cover whole screen
//
////--------------------------------------------------------------------------------------
void TressFXRenderer::RenderScreenQuad(VkCommandBuffer cmdbuffer, VkPipeline pPipeline)
{
    // set shader
    vkCmdBindPipeline(cmdbuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pPipeline);

    // Draw full screen triangle
    vkCmdDraw(cmdbuffer, 3, 1, 0, 0);
}

//--------------------------------------------------------------------------------------
//
// GenerateShadowMap
//
// Renders the hair from the point of view of the light into a shadow map
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::GenerateShadowMap(VkDevice pvkDevice,
                                        VkCommandBuffer commandBuffer,
                                        float density, uint32_t uniformBufferIndex,
                                        const DebugMarkerPointer& markerCallbacks)
{
    VkViewport viewport{0.f, 0.f, SM_HAIR_WIDTH, SM_HAIR_HEIGHT, 0.f, 1.f};
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);
    VkRect2D scissor{0, 0, SM_HAIR_WIDTH, SM_HAIR_HEIGHT};
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
    // clear depth for early z
    VkRenderPassBeginInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
    info.renderPass = m_RPHairShadow;
    info.framebuffer = m_shadowFrameBuffer;
    info.clearValueCount = 1;
    VkClearValue depthClear{};
    depthClear.depthStencil.depth = 1.f;
    info.pClearValues = &depthClear;
    info.renderArea.extent = {SM_HAIR_WIDTH, SM_HAIR_HEIGHT};
    markerCallbacks.markBeginRegion(commandBuffer, "Shadowmap");
    vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                      m_pPLGenerateHairSM);
    VkDescriptorSet sets[] = {m_shadow_pass_set, m_pTressFXMesh->m_shadow_pass_set};
    uint32_t descriptorOffsets[] = {uniformBufferIndex * sizeof(CB_PER_FRAME)};
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_shadow_pass_layout, 0, AMD_ARRAY_SIZE(sets), sets,
                            AMD_ARRAY_SIZE(descriptorOffsets), descriptorOffsets);
    vkCmdBindIndexBuffer(commandBuffer, m_pTressFXMesh->m_pIndexBuffer, 0,
                         VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, UINT(density * m_pTressFXMesh->m_TotalIndexCount), 1,
                     0, 0, 0);
    vkCmdEndRenderPass(commandBuffer);
    markerCallbacks.markEndRegion(commandBuffer);
}

//--------------------------------------------------------------------------------------
//
// RenderHair
//
// Renders the hair in two passes. The first pass fills an A-buffer by rendering the
// hair geometry into a per-pixel linked list which keeps all of the overlapping
// fragments.
// The second pass renders a full screen quad (using a stencil mask set in the first pass
// to avoid unnecessary pixels) which reads fragments from the per-pixel linked list
// and blends the nearest k fragments (K-buffer) in back to front order.
//
////--------------------------------------------------------------------------------------
void TressFXRenderer::RenderHair(VkDevice pvkDevice,
                                 VkCommandBuffer commandBuffer,
                                 uint32_t width,
                                 uint32_t height,
                                 uint32_t uniformBufferIndex, const DebugMarkerPointer& markerCallbacks)
{
    // Caller must rebind original render target and depth stencil view after call

    // render hair
    markerCallbacks.markBeginRegion(commandBuffer, "RenderHair");

    // Clear HeadPPLL buffer as it's not an attachment
    VkClearColorValue dwClearDataMinusOne{};
    dwClearDataMinusOne.uint32[0] = 0xFFFFFFFF;
    VkImageSubresourceRange range{VK_IMAGE_ASPECT_COLOR_BIT, 0, 1, 0, 1};
    vkCmdClearColorImage(commandBuffer, m_pHeadPPLLTexture, VK_IMAGE_LAYOUT_GENERAL,
                         &dwClearDataMinusOne, 1, &range);
    vkCmdFillBuffer(commandBuffer, m_pAtomicCounterPPLLBuffer, 0, sizeof(unsigned int),
                    0);

    VkBufferMemoryBarrier flushAtomicCounterBarrier[] = {
        getBufferBarrier(m_pAtomicCounterPPLLBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT)};

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_TRANSFER_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr,
                         AMD_ARRAY_SIZE(flushAtomicCounterBarrier),
                         flushAtomicCounterBarrier, 0, nullptr);

    uint32_t descriptorOffsets[] = {uniformBufferIndex * sizeof(CB_PER_FRAME)};

    {
        VkClearValue clearStencil{};
        clearStencil.depthStencil.stencil = 0;
        VkRenderPassBeginInfo renderPassBeginInfo{
            VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
        renderPassBeginInfo.renderPass = m_pRPHairRendering;
        renderPassBeginInfo.framebuffer = m_renderHairFramebuffer;
        renderPassBeginInfo.renderArea.extent.width = width;
        renderPassBeginInfo.renderArea.extent.height = height;
        renderPassBeginInfo.clearValueCount = 1;
        renderPassBeginInfo.pClearValues = &clearStencil;
        vkCmdBeginRenderPass(commandBuffer, &renderPassBeginInfo,
                             VK_SUBPASS_CONTENTS_INLINE);
    }

    // Pass 1: A-Buffer pass
    if (m_hairParams.bAntialias)
    {
        if (m_hairParams.strandCopies > 1)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_pPLRenderHairAAStrandCopies);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_pPLRenderHairAA);
        }
    }
    else
    {
        if (m_hairParams.strandCopies > 1)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_pPLRenderHairStrandCopies);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_pPLRenderHair);
        }
    }
    VkDescriptorSet descriptorSets[] = {m_pass1_config_set, m_pTressFXMesh->m_pass1_set};
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pass1_layout, 0, AMD_ARRAY_SIZE(descriptorSets),
                            descriptorSets, AMD_ARRAY_SIZE(descriptorOffsets),
                            descriptorOffsets);

    vkCmdBindIndexBuffer(commandBuffer, m_pTressFXMesh->m_pTriangleIndexBuffer, 0,
                         VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, UINT(m_hairParams.density *
                                         m_pTressFXMesh->m_TotalTriangleIndexCount),
                     m_hairParams.strandCopies, 0, 0, 0);

    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);

    // Pass 2: K-Buffer pass
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                            m_pass2_layout, 0, 1, &m_pass2_set,
                            AMD_ARRAY_SIZE(descriptorOffsets), descriptorOffsets);
    RenderScreenQuad(commandBuffer, m_pPLResolveKBuffer);
    vkCmdEndRenderPass(commandBuffer);
    markerCallbacks.markEndRegion(commandBuffer);
}

void TressFXRenderer::RenderHairShortcut(VkDevice pvkDevice,

                                         VkCommandBuffer commandBuffer,
 uint32_t width,
                                         uint32_t height, uint32_t uniformBufferIndex, const DebugMarkerPointer& markerCallbacks)
{
    m_ShortCut.SetupDepthPass(pvkDevice, commandBuffer, width, height);

    uint32_t descriptorOffsets[] = {uniformBufferIndex * sizeof(CB_PER_FRAME)};

    // DEPTH FILL
    if (m_hairParams.bAntialias)
    {
        if (m_hairParams.strandCopies > 1)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairAAStrandCopiesDepthsAlpha);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairAADepthsAlpha);
        }
    }
    else
    {
        if (m_hairParams.strandCopies > 1)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairStrandCopiesDepthsAlpha);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairDepthsAlpha);
        }
    }

    {
        VkDescriptorSet descriptorSets[] = {m_ShortCut.m_depthPassSet,
                                            m_pTressFXMesh->m_pass1_set};
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_ShortCut.m_depthPassPipelineLayout, 0,
                                AMD_ARRAY_SIZE(descriptorSets), descriptorSets,
                                AMD_ARRAY_SIZE(descriptorOffsets), descriptorOffsets);
    }

    vkCmdBindIndexBuffer(commandBuffer, m_pTressFXMesh->m_pTriangleIndexBuffer, 0,
                         VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, UINT(m_hairParams.density *
                                         m_pTressFXMesh->m_TotalTriangleIndexCount),
                     m_hairParams.strandCopies, 0, 0, 0);

    // DEPTH RESOLVE
    m_ShortCut.SetupResolveDepth(pvkDevice, commandBuffer, width, height);
    RenderScreenQuad(commandBuffer, m_ShortCut.m_pPLDepthResolve);

    // COLOR FILL
    m_ShortCut.SetupShadePass(pvkDevice, commandBuffer, width, height);

    if (m_hairParams.bAntialias)
    {
        if (m_hairParams.strandCopies > 1)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairAAStrandCopiesFillColors);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairAAFillColors);
        }
    }
    else
    {
        if (m_hairParams.strandCopies > 1)
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairStrandCopiesFillColors);
        }
        else
        {
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                              m_ShortCut.m_pPLRenderHairFillColors);
        }
    }

    {
        VkDescriptorSet descriptorSets[] = {m_ShortCut.m_colorPassSet,
                                            m_pTressFXMesh->m_pass1_set};
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS,
                                m_ShortCut.m_colorPassPipelineLayout, 0,
                                AMD_ARRAY_SIZE(descriptorSets), descriptorSets,
                                AMD_ARRAY_SIZE(descriptorOffsets), descriptorOffsets);
    }

    vkCmdBindIndexBuffer(commandBuffer, m_pTressFXMesh->m_pTriangleIndexBuffer, 0,
                         VK_INDEX_TYPE_UINT32);
    vkCmdDrawIndexed(commandBuffer, UINT(m_hairParams.density *
                                         m_pTressFXMesh->m_TotalTriangleIndexCount),
                     m_hairParams.strandCopies, 0, 0, 0);

    // COLOR RESOLVE
    m_ShortCut.SetupResolveColor(pvkDevice, commandBuffer, width, height);
    RenderScreenQuad(commandBuffer, m_ShortCut.m_pPLColorResolve);
    vkCmdEndRenderPass(commandBuffer);
}

//--------------------------------------------------------------------------------------
//
// EndHairFrame
//
// Start of hair rendering.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::EndHairFrame(VkDevice pvkDevice) {}

//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when device is destroyed.
//
//--------------------------------------------------------------------------------------
void TressFXRenderer::OnDestroy()
{
    m_ShortCut.OnDestroy(true);

    {
        AMD_SAFE_RELEASE(m_pPLRenderHairAAStrandCopies, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairAA, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairStrandCopies, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHair, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLResolveKBuffer, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLGenerateHairSM, vkDestroyPipeline, m_pvkDevice);

        AMD_SAFE_RELEASE(m_renderHairFramebuffer, vkDestroyFramebuffer, m_pvkDevice);
        AMD_SAFE_RELEASE(m_shadowFrameBuffer, vkDestroyFramebuffer, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pRPHairRendering, vkDestroyRenderPass, m_pvkDevice);
        AMD_SAFE_RELEASE(m_RPHairShadow, vkDestroyRenderPass, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pass1_layout, vkDestroyPipelineLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pass2_layout, vkDestroyPipelineLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_shadow_pass_layout, vkDestroyPipelineLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pass1_config_set_layout, vkDestroyDescriptorSetLayout,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_pass1_hair_set_layout, vkDestroyDescriptorSetLayout,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_pass2_set_layout, vkDestroyDescriptorSetLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_shadow_pass_config_set_layout, vkDestroyDescriptorSetLayout,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_shadow_pass_hair_set_layout, vkDestroyDescriptorSetLayout,
                         m_pvkDevice);
    }

    AMD_SAFE_RELEASE(m_pSamplerStateLinearWrap, vkDestroySampler, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pSamplerStatePointClamp, vkDestroySampler, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pSamplerStateCmpLess, vkDestroySampler, m_pvkDevice);

    AMD_SAFE_RELEASE(m_descriptorStorage, vkDestroyDescriptorPool, m_pvkDevice);

    // constant buffer
    AMD_SAFE_RELEASE(m_pcbPerFrame, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pcbPerFrameMemory, vkFreeMemory, m_pvkDevice);

    // PPLL buffers
    DeletePPLL(m_pvkDevice);

    // textures and views
    AMD_SAFE_RELEASE(m_pNoiseView, vkDestroyImageView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pNoiseTexture, vkDestroyImage, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pNoiseMemory, vkFreeMemory, m_pvkDevice);

    // Hair shadow map depth stencil buffer
    AMD_SAFE_RELEASE(m_pSMHairView, vkDestroyImageView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pSMHairTexture, vkDestroyImage, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pSMHairMemory, vkFreeMemory, m_pvkDevice);
}

} // namespace AMD
