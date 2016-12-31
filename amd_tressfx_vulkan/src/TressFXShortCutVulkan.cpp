//--------------------------------------------------------------------------------------
// File: TressFXShortCut.cpp
//
// TressFX ShortCut method.
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

#include "TressFXShortCutVulkan.h"
#include "AMD_Types.h"
#include "TressFXPrecompiledShadersVulkan.h"
#include "UtilVulkan.h"
#include <vector>

// unreferenced formal parameter
#pragma warning(disable : 4100)

// Constants must match in TressFXRender.hlsl

// Clear value for depths resource
#define SHORTCUT_INITIAL_DEPTH 0x3f800000

// Number of depth layers to use.  2 or 3 supported.
#define SHORTCUT_NUM_DEPTHS 3

// Compute source color as weighted average of front fragments, vs blending in order.
#define SHORTCUT_WEIGHTED_AVERAGE 1

// Output color deterministically when fragments have the same depth.  Requires additional
// clear of colors resource.
#define SHORTCUT_DETERMINISTIC 1

namespace AMD
{

VkResult GPUOnlyStructuredBuffer::Create(VkDevice pvkDevice, uint32_t structSize,
                                         uint32_t structCount)
{
    VkResult vr;

    // Per-pixel Linked List (PPLL) buffer
    VkBufferCreateInfo BufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
    BufferDesc.size = structCount * structSize;
    BufferDesc.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &BufferDesc, NULL, &m_pBuffer));
    m_pvkDevice = pvkDevice;

    return VK_SUCCESS;
}

void GPUOnlyStructuredBuffer::Destroy()
{
    if (m_pBuffer)
        vkDestroyBuffer(m_pvkDevice, m_pBuffer, nullptr);
}

VkResult TressFXShortCut::CreateScreenSizedItems(VkDevice pvkDevice, int winWidth,
                                                 int winHeight,
                                                 VkPhysicalDeviceMemoryProperties memProperties)
{
    m_pvkDevice = pvkDevice;
    VkResult vr;
    {
        VkImageCreateInfo accumInvAlphaInfo = getImageCreateInfo(
            VK_FORMAT_R16_SFLOAT, winWidth, winHeight,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
        AMD_CHECKED_VULKAN_CALL(vkCreateImage(pvkDevice, &accumInvAlphaInfo, nullptr,
                                   &m_pAccumInvAlphaTexture));
        m_pAccumInvAlphaMemory =
            allocImageMemory(pvkDevice, m_pAccumInvAlphaTexture, memProperties);

        VkImageViewCreateInfo srDesc{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        srDesc.format = VK_FORMAT_R16_SFLOAT;
        srDesc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srDesc.subresourceRange.levelCount = 1;
        srDesc.subresourceRange.layerCount = 1;
        srDesc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        srDesc.image = m_pAccumInvAlphaTexture;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateImageView(pvkDevice, &srDesc, nullptr, &m_pAccumInvAlphaView));
    }

    {
        VkImageCreateInfo fragmentDepthInfo =
            getImageCreateInfo(VK_FORMAT_R32_UINT, winWidth, winHeight,
                               VK_IMAGE_USAGE_STORAGE_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                                   VK_IMAGE_USAGE_TRANSFER_DST_BIT,
                               SHORTCUT_NUM_DEPTHS);
        AMD_CHECKED_VULKAN_CALL(vkCreateImage(pvkDevice, &fragmentDepthInfo, nullptr,
                                   &m_pFragmentDepthsTexture));

        m_pFragmentDepthsMemory =
            allocImageMemory(pvkDevice, m_pFragmentDepthsTexture, memProperties);

        VkImageViewCreateInfo srDesc{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        srDesc.format = VK_FORMAT_R32_UINT;
        srDesc.viewType = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
        srDesc.subresourceRange.levelCount = 1;
        srDesc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        srDesc.subresourceRange.layerCount = SHORTCUT_NUM_DEPTHS;
        srDesc.image = m_pFragmentDepthsTexture;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateImageView(pvkDevice, &srDesc, nullptr, &m_pFragmentDepthsView));
    }

#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
    {
        VkImageCreateInfo fragmentColorInfo = getImageCreateInfo(
            VK_FORMAT_R16G16B16A16_SFLOAT, winWidth, winHeight,
            VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT |
                VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT);
        AMD_CHECKED_VULKAN_CALL(vkCreateImage(pvkDevice, &fragmentColorInfo, nullptr,
                                   &m_pFragmentColorsTexture));

        m_pFragmentColorsMemory =
            allocImageMemory(pvkDevice, m_pFragmentColorsTexture, memProperties);

        VkImageViewCreateInfo srDesc{VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO};
        srDesc.format = VK_FORMAT_R16G16B16A16_SFLOAT;
        srDesc.viewType = VK_IMAGE_VIEW_TYPE_2D;
        srDesc.subresourceRange.levelCount = 1;
        srDesc.subresourceRange.layerCount = 1;
        srDesc.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        srDesc.image = m_pFragmentColorsTexture;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateImageView(pvkDevice, &srDesc, nullptr, &m_pFragmentColorsView));
    }
#else
    m_FragmentColors.Create(pd3dDevice, 4 * SHORTCUT_NUM_DEPTHS, winWidth * winHeight);
#endif

    return VK_SUCCESS;
}

void TressFXShortCut::DestroyScreenSizedItems()
{
    AMD_SAFE_RELEASE(m_pFBRenderHair, vkDestroyFramebuffer, m_pvkDevice);

    AMD_SAFE_RELEASE(m_pFragmentColorsView, vkDestroyImageView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pFragmentDepthsView, vkDestroyImageView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pAccumInvAlphaView, vkDestroyImageView, m_pvkDevice);

    AMD_SAFE_RELEASE(m_pFragmentColorsMemory, vkFreeMemory, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pFragmentDepthsMemory, vkFreeMemory, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pAccumInvAlphaMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_pFragmentColorsTexture, vkDestroyImage, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pFragmentDepthsTexture, vkDestroyImage, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pAccumInvAlphaTexture, vkDestroyImage, m_pvkDevice);
    m_FragmentColors.Destroy();
}

namespace
{
VkAttachmentDescription getAttachmentDescription(
    VkFormat format, VkAttachmentLoadOp loadOp, VkAttachmentStoreOp storeOp,
    VkImageLayout inoutLayout,
    VkAttachmentLoadOp stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
    VkAttachmentStoreOp stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE)
{
    return {0,          format,        VK_SAMPLE_COUNT_1_BIT, loadOp,
            storeOp,    stencilLoadOp, stencilStoreOp,        inoutLayout,
            inoutLayout};
}

VkRenderPass createRenderPass(VkDevice pvkDevice, VkFormat depthStencilFormat, VkFormat colorFormat)
{
    VkRenderPassCreateInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO};

    const VkAttachmentDescription attachments[] = {
        // DS
        getAttachmentDescription(depthStencilFormat, VK_ATTACHMENT_LOAD_OP_LOAD,
                                 VK_ATTACHMENT_STORE_OP_STORE,
                                 VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
                                 VK_ATTACHMENT_LOAD_OP_CLEAR,
                                 VK_ATTACHMENT_STORE_OP_DONT_CARE),
        // accumInvAlpha
        getAttachmentDescription(VK_FORMAT_R16_SFLOAT, VK_ATTACHMENT_LOAD_OP_CLEAR,
                                 VK_ATTACHMENT_STORE_OP_DONT_CARE,
                                 VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        // FragmentColor
        getAttachmentDescription(
            VK_FORMAT_R16G16B16A16_SFLOAT, VK_ATTACHMENT_LOAD_OP_CLEAR,
            VK_ATTACHMENT_STORE_OP_DONT_CARE, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
#else
// TODO
#endif
        // Result
        getAttachmentDescription(colorFormat, VK_ATTACHMENT_LOAD_OP_LOAD,
                                 VK_ATTACHMENT_STORE_OP_STORE,
                                 VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),

    };

    VkAttachmentReference depthStencil{0,
                                       VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL};
    VkAttachmentReference accumInvAlpha{1, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
    VkAttachmentReference FragmentColor{2, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};
#else
// TODO
#endif
    VkAttachmentReference colorAttachment{3, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL};

    VkAttachmentReference colorResolveInputAttachments[] = {{1, VK_IMAGE_LAYOUT_GENERAL},
                                                            {2, VK_IMAGE_LAYOUT_GENERAL}};

    uint32_t untouchedAttachmentDepthResolve[] = {1};
    uint32_t untouchedAttachmentColorFill[] = {1};

    const VkSubpassDescription subpasses[] = {
        // Depth alpha
        {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 1, &accumInvAlpha, nullptr,
         &depthStencil, 0, nullptr},
        // Depth resolve
        {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 0, nullptr, nullptr,
         &depthStencil, AMD_ARRAY_SIZE(untouchedAttachmentDepthResolve),
         untouchedAttachmentDepthResolve},
        // Color fill
        {0, VK_PIPELINE_BIND_POINT_GRAPHICS, 0, nullptr, 1, &FragmentColor, nullptr,
         &depthStencil, AMD_ARRAY_SIZE(untouchedAttachmentColorFill),
         untouchedAttachmentColorFill},
        // Color resolve
        {0, VK_PIPELINE_BIND_POINT_GRAPHICS, AMD_ARRAY_SIZE(colorResolveInputAttachments),
         colorResolveInputAttachments, 1, &colorAttachment, nullptr, &depthStencil, 0,
         nullptr},
    };

    const VkSubpassDependency dependencies[] = {
        // Depth alpha modifies fragment depth image while depth resolve reads it
        {0, 1, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
         VK_ACCESS_SHADER_READ_BIT, VK_DEPENDENCY_BY_REGION_BIT},
        {0, 2, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, VK_DEPENDENCY_BY_REGION_BIT},
        {0, 3, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
         VK_ACCESS_INPUT_ATTACHMENT_READ_BIT, VK_DEPENDENCY_BY_REGION_BIT},
        // Depth resolve update depth attachement while color fill reads it
        {1, 2, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
         VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT, VK_DEPENDENCY_BY_REGION_BIT},
        {1, 3, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, VK_DEPENDENCY_BY_REGION_BIT},
        // color resolve consumes output from color fill
        {2, 3, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
         VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
         VK_ACCESS_INPUT_ATTACHMENT_READ_BIT, VK_DEPENDENCY_BY_REGION_BIT},
    };

    info.attachmentCount = AMD_ARRAY_SIZE(attachments);
    info.pAttachments = attachments;
    info.subpassCount = AMD_ARRAY_SIZE(subpasses);
    info.pSubpasses = subpasses;
    info.dependencyCount = AMD_ARRAY_SIZE(dependencies);
    info.pDependencies = dependencies;

    VkRenderPass result;
    vkCreateRenderPass(pvkDevice, &info, nullptr, &result);
    return result;
}
}

//--------------------------------------------------------------------------------------
//
// CreateRenderStateObjects
//
// Creates the pipelines for hair rendering
//
//--------------------------------------------------------------------------------------
VkResult TressFXShortCut::CreateRenderStateObjects(VkDevice pvkDevice, VkFormat depthStencilFormat, VkFormat colorFormat)
{
    m_pRPRenderHair = createRenderPass(pvkDevice, depthStencilFormat, colorFormat);

    ShaderModule m_pPSDepthsAlpha(pvkDevice, depth_hair_data);
    ShaderModule m_pPSFillColors(pvkDevice, fillcolors_hair_data);

    ShaderModule render_hair_aa_strand_copiesModule(pvkDevice,
                                                    render_hair_aa_strand_copies_vertex);
    const VkPipelineShaderStageCreateInfo renderHairAAStrandCopiesDepthsAlphaStage[2] = {
        getShaderStageCreateInfo(render_hair_aa_strand_copiesModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSDepthsAlpha.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairAAStrandCopiesDepthsAlphaDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pDepthWritesToColor_BS,
            &CommonPipelineState::inputAssemblyTriangle,
            renderHairAAStrandCopiesDepthsAlphaStage, m_depthPassPipelineLayout,
            m_pRPRenderHair, 0);

    const VkPipelineShaderStageCreateInfo renderHairAAStrandCopiesFillColorsStage[2] = {
        getShaderStageCreateInfo(render_hair_aa_strand_copiesModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSFillColors.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairAAStrandCopiesFillColorsDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pSum_BS,
#else
            &m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, &m_pNoWrites_BS,
#endif
            &CommonPipelineState::inputAssemblyTriangle,
            renderHairAAStrandCopiesFillColorsStage, m_colorPassPipelineLayout,
            m_pRPRenderHair, 2);

    ShaderModule RenderHairAAVertexShader(pvkDevice, render_hair_aa_vertex);
    const VkPipelineShaderStageCreateInfo renderHairAADepthsAlphaStage[2] = {
        getShaderStageCreateInfo(RenderHairAAVertexShader.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSDepthsAlpha.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairAADepthsAlphaDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pDepthWritesToColor_BS,
            &CommonPipelineState::inputAssemblyTriangle, renderHairAADepthsAlphaStage,
            m_depthPassPipelineLayout, m_pRPRenderHair, 0);

    const VkPipelineShaderStageCreateInfo renderHairAAFillColorsStage[2] = {
        getShaderStageCreateInfo(RenderHairAAVertexShader.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSFillColors.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairAAFillColorsDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pSum_BS,
#else
            &m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, &m_pNoWrites_BS,
#endif
            &CommonPipelineState::inputAssemblyTriangle, renderHairAAFillColorsStage,
            m_colorPassPipelineLayout, m_pRPRenderHair, 2);

    ShaderModule renderHairStrandCopiesVertexModule(pvkDevice,
                                                    render_hair_strand_copies_vertex);
    const VkPipelineShaderStageCreateInfo renderHairStrandCopiesDepthsAlphaStage[2] = {
        getShaderStageCreateInfo(renderHairStrandCopiesVertexModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSDepthsAlpha.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairStrandCopiesDepthsAlphaDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pDepthWritesToColor_BS,
            &CommonPipelineState::inputAssemblyTriangle,
            renderHairStrandCopiesDepthsAlphaStage, m_depthPassPipelineLayout,
            m_pRPRenderHair, 0);

    const VkPipelineShaderStageCreateInfo renderHairStrandCopiesFillColorsStage[2] = {
        getShaderStageCreateInfo(renderHairStrandCopiesVertexModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSFillColors.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairStrandCopiesFillColorsDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pSum_BS,
#else
            &m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, &m_pNoWrites_BS,
#endif
            &CommonPipelineState::inputAssemblyTriangle,
            renderHairStrandCopiesFillColorsStage, m_colorPassPipelineLayout,
            m_pRPRenderHair, 2);

    ShaderModule renderHairVertexModule(pvkDevice, render_hair_vertex);
    const VkPipelineShaderStageCreateInfo renderHairDepthsAlphaStage[2] = {
        getShaderStageCreateInfo(renderHairVertexModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSDepthsAlpha.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairDepthsAlphaDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pDepthWritesToColor_BS,
            &CommonPipelineState::inputAssemblyTriangle, renderHairDepthsAlphaStage,
            m_depthPassPipelineLayout, m_pRPRenderHair, 0);

    const VkPipelineShaderStageCreateInfo renderHairFillColorsStage[2] = {
        getShaderStageCreateInfo(renderHairVertexModule.m_shaderModule,
                                 VK_SHADER_STAGE_VERTEX_BIT, "main"),
        getShaderStageCreateInfo(m_pPSFillColors.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo renderHairFillColorsDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutHair,
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
            &CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc,
            &CommonPipelineState::m_pSum_BS,
#else
            &m_pDepthTestEnabledNoDepthWritesStencilWriteIncrement_DSS, &m_pNoWrites_BS,
#endif
            &CommonPipelineState::inputAssemblyTriangle, renderHairFillColorsStage,
            m_colorPassPipelineLayout, m_pRPRenderHair, 2);

    // Resolve depth
    ShaderModule renderQuadVS(pvkDevice, pass2_vertex);
    ShaderModule depth_resolve(pvkDevice, resolve_depth);
    const VkPipelineShaderStageCreateInfo depthResolveStage[2] = {
        getShaderStageCreateInfo(renderQuadVS.m_shaderModule, VK_SHADER_STAGE_VERTEX_BIT,
                                 "main"),
        getShaderStageCreateInfo(depth_resolve.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo depthResolveDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutQuad,
            &CommonPipelineState::m_pDepthWriteEnabledStencilTestLess_DSS,
            &CommonPipelineState::ColorWritesOff,
            &CommonPipelineState::inputAssemblyTriangle, depthResolveStage,
            m_depthPassPipelineLayout, m_pRPRenderHair, 1);

    // Resolve Color
    ShaderModule color_resolve(pvkDevice, resolvecolors);
    const VkPipelineShaderStageCreateInfo colorResolveStage[2] = {
        getShaderStageCreateInfo(renderQuadVS.m_shaderModule, VK_SHADER_STAGE_VERTEX_BIT,
                                 "main"),
        getShaderStageCreateInfo(color_resolve.m_shaderModule,
                                 VK_SHADER_STAGE_FRAGMENT_BIT, "main"),
    };

    const VkGraphicsPipelineCreateInfo colorResolveDesc =
        CommonPipelineState::getBasePipelineCreateInfo(
            &CommonPipelineState::m_pLayoutQuad,
            &CommonPipelineState::DepthTestDisabledStencilTestLessDSS,
            &CommonPipelineState::m_pResolveColor_BS,
            &CommonPipelineState::inputAssemblyTriangle, colorResolveStage,
            m_colorPassPipelineLayout, m_pRPRenderHair, 3);

    VkGraphicsPipelineCreateInfo pipelinesDesc[] = {
        renderHairAAStrandCopiesDepthsAlphaDesc,
        renderHairAAStrandCopiesFillColorsDesc,
        renderHairAADepthsAlphaDesc,
        renderHairAAFillColorsDesc,
        renderHairStrandCopiesDepthsAlphaDesc,
        renderHairStrandCopiesFillColorsDesc,
        renderHairDepthsAlphaDesc,
        renderHairFillColorsDesc,
        depthResolveDesc,
        colorResolveDesc};

    VkPipeline pipelines[AMD_ARRAY_SIZE(pipelinesDesc)];

    VkResult vr;
    AMD_CHECKED_VULKAN_CALL(vkCreateGraphicsPipelines(pvkDevice, VK_NULL_HANDLE,
                                           AMD_ARRAY_SIZE(pipelinesDesc), pipelinesDesc,
                                           nullptr, pipelines));

    m_pPLRenderHairAAStrandCopiesDepthsAlpha = pipelines[0];
    m_pPLRenderHairAAStrandCopiesFillColors = pipelines[1];
    m_pPLRenderHairAADepthsAlpha = pipelines[2];
    m_pPLRenderHairAAFillColors = pipelines[3];
    m_pPLRenderHairStrandCopiesDepthsAlpha = pipelines[4];
    m_pPLRenderHairStrandCopiesFillColors = pipelines[5];
    m_pPLRenderHairDepthsAlpha = pipelines[6];
    m_pPLRenderHairFillColors = pipelines[7];
    m_pPLDepthResolve = pipelines[8];
    m_pPLColorResolve = pipelines[9];

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// CreateLayouts
//
// Creates the descriptors set and pipeline layouts.
//
//--------------------------------------------------------------------------------------
VkResult TressFXShortCut::CreateLayouts(VkDevice pvkDevice,
                                        VkDescriptorSetLayout mesh_layout,
                                        VkSampler noiseSamplerRef,
                                        VkSampler shadowSamplerRef)
{
    VkResult vr;

    {
        VkDescriptorSetLayoutCreateInfo info{
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        VkDescriptorSetLayoutBinding bindings[] = {
            // TressFX parameters
            {IDSRV_CONSTANTS_BUFFER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1,
             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
            // Fragment depth
            {IDSRV_HAIR_FRAGMENT_DEPTHS, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // Sampler for noise texture in VS
            {IDSRV_NOISE_SAMPLER, VK_DESCRIPTOR_TYPE_SAMPLER, 1,
             VK_SHADER_STAGE_VERTEX_BIT, &noiseSamplerRef},
            // Noise texture
            {IDSRV_NOISEMAP, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
        };
        info.bindingCount = AMD_ARRAY_SIZE(bindings);
        info.pBindings = bindings;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateDescriptorSetLayout(pvkDevice, &info, nullptr, &m_pSLDepthAlpha));
    }

    {
        VkDescriptorSetLayoutCreateInfo info{
            VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
        VkDescriptorSetLayoutBinding bindings[] = {
            // TressFX parameters
            {IDSRV_CONSTANTS_BUFFER, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1,
             VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT},
            // Fragment depth
            {IDSRV_HAIR_FRAGMENT_DEPTHS, VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // Sampler for noise texture in VS
            {IDSRV_NOISE_SAMPLER, VK_DESCRIPTOR_TYPE_SAMPLER, 1,
             VK_SHADER_STAGE_VERTEX_BIT, &noiseSamplerRef},
            // Noise texture
            {IDSRV_NOISEMAP, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1,
             VK_SHADER_STAGE_VERTEX_BIT},
            // Hair shadow
            {IDSRV_HAIRSM, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
            // Hair shadow sampler
            {IDSRV_SHADOW_SAMPLER, VK_DESCRIPTOR_TYPE_SAMPLER, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT, &shadowSamplerRef},
// Fragment color
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
            {IDSRV_HAIR_FRAGMENT_COLORS, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
#else
// TODO
#endif
            // inv alpha accum
            {IDSRV_HAIR_ACCUM_INV_ALPHA, VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1,
             VK_SHADER_STAGE_FRAGMENT_BIT},
        };
        info.bindingCount = AMD_ARRAY_SIZE(bindings);
        info.pBindings = bindings;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateDescriptorSetLayout(pvkDevice, &info, nullptr, &m_pSLColors));
    }

    {
        VkPipelineLayoutCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        VkDescriptorSetLayout set_layout[] = {m_pSLDepthAlpha, mesh_layout};

        info.setLayoutCount = AMD_ARRAY_SIZE(set_layout);
        info.pSetLayouts = set_layout;
        AMD_CHECKED_VULKAN_CALL(vkCreatePipelineLayout(pvkDevice, &info, nullptr,
                                            &m_depthPassPipelineLayout));
    }

    {
        VkPipelineLayoutCreateInfo info{VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO};
        VkDescriptorSetLayout set_layout[] = {m_pSLColors, mesh_layout};

        info.setLayoutCount = AMD_ARRAY_SIZE(set_layout);
        info.pSetLayouts = set_layout;
        AMD_CHECKED_VULKAN_CALL(vkCreatePipelineLayout(pvkDevice, &info, nullptr,
                                            &m_colorPassPipelineLayout));
    }

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// CreateFramebuffer
//
// Creates the framebuffer for every pass of the shortcut algorithm
//
//--------------------------------------------------------------------------------------
VkResult TressFXShortCut::CreateFramebuffer(VkDevice pvkDevice,
                                            VkImageView depthStencilView,
                                            VkImageView colorView, uint32_t width,
                                            uint32_t height)
{
    VkResult vr;
    {
        VkFramebufferCreateInfo info{VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO};
        info.renderPass = m_pRPRenderHair;
        info.height = height;
        info.width = width;
        info.layers = 1;
#if SHORTCUT_DETERMINISTIC && SHORTCUT_WEIGHTED_AVERAGE
        VkImageView attachments[] = {depthStencilView, m_pAccumInvAlphaView,
                                     m_pFragmentColorsView, colorView};
        info.attachmentCount = AMD_ARRAY_SIZE(attachments);
        info.pAttachments = attachments;
#else
// TODO
#endif
        AMD_CHECKED_VULKAN_CALL(vkCreateFramebuffer(pvkDevice, &info, nullptr, &m_pFBRenderHair));
    }
    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// AllocateAndPopulateSets
//
// Allocate descriptor set and fills them
//
//--------------------------------------------------------------------------------------
VkResult TressFXShortCut::AllocateAndPopulateSets(VkDevice pvkDevice,
                                                  VkBuffer configBuffer,
                                                  uint64_t configBufferSize,
                                                  VkImageView noiseMap,
                                                  VkImageView hairShadowMap)
{
    VkResult vr;
    {
        VkDescriptorPoolCreateInfo info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
        VkDescriptorPoolSize sizes[] = {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 2},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 5},
                                        {VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 5},
                                        {VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 2},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1},
                                        {VK_DESCRIPTOR_TYPE_SAMPLER, 5}};
        info.maxSets = 2;
        info.poolSizeCount = AMD_ARRAY_SIZE(sizes);
        info.pPoolSizes = sizes;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateDescriptorPool(pvkDevice, &info, nullptr, &m_pDPShortcutPool));
    }

    {
        VkDescriptorSetLayout setLayout[] = {m_pSLDepthAlpha, m_pSLColors};
        VkDescriptorSetAllocateInfo info{VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
        info.descriptorPool = m_pDPShortcutPool;
        info.descriptorSetCount = AMD_ARRAY_SIZE(setLayout);
        info.pSetLayouts = setLayout;
        VkDescriptorSet sets[AMD_ARRAY_SIZE(setLayout)];
        AMD_CHECKED_VULKAN_CALL(vkAllocateDescriptorSets(pvkDevice, &info, sets));
        m_depthPassSet = sets[0];
        m_colorPassSet = sets[1];
    }

    VkDescriptorBufferInfo bufferdesc{configBuffer, 0, configBufferSize};
    VkDescriptorImageInfo noise_descriptor{VK_NULL_HANDLE, noiseMap,
                                           VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    VkDescriptorImageInfo hairSM_descriptor{VK_NULL_HANDLE, hairShadowMap,
                                            VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    VkDescriptorImageInfo hairFragmentDepthsDescriptor{
        VK_NULL_HANDLE, m_pFragmentDepthsView, VK_IMAGE_LAYOUT_GENERAL};
    VkDescriptorImageInfo hairFragmentColorDescriptor{
        VK_NULL_HANDLE, m_pFragmentColorsView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};
    VkDescriptorImageInfo hairAccumInvAlphaDescriptor{
        VK_NULL_HANDLE, m_pAccumInvAlphaView, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL};

    VkWriteDescriptorSet writes[] = {
        getWriteDescriptor(m_depthPassSet, IDSRV_CONSTANTS_BUFFER,
                           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &bufferdesc),
        getWriteDescriptor(m_depthPassSet, IDSRV_HAIR_FRAGMENT_DEPTHS,
                           VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                           &hairFragmentDepthsDescriptor),
        getWriteDescriptor(m_depthPassSet, IDSRV_NOISEMAP,
                           VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, &noise_descriptor),

        getWriteDescriptor(m_colorPassSet, IDSRV_CONSTANTS_BUFFER,
                           VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, &bufferdesc),
        getWriteDescriptor(m_colorPassSet, IDSRV_HAIR_FRAGMENT_DEPTHS,
                           VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
                           &hairFragmentDepthsDescriptor),
        getWriteDescriptor(m_colorPassSet, IDSRV_NOISEMAP,
                           VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, &noise_descriptor),
        getWriteDescriptor(m_colorPassSet, IDSRV_HAIRSM, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
                           &hairSM_descriptor),
        getWriteDescriptor(m_colorPassSet, IDSRV_HAIR_FRAGMENT_COLORS,
                           VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
                           &hairFragmentColorDescriptor),
        getWriteDescriptor(m_colorPassSet, IDSRV_HAIR_ACCUM_INV_ALPHA,
                           VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
                           &hairAccumInvAlphaDescriptor),
    };

    vkUpdateDescriptorSets(pvkDevice, AMD_ARRAY_SIZE(writes), writes, 0, nullptr);

    return VK_SUCCESS;
}

VkResult TressFXShortCut::OnCreateDevice(
    VkDevice pd3dDevice, int winWidth, int winHeight, VkDescriptorSetLayout mesh_layout,
    VkSampler noiseSamplerRef, VkSampler shadowSamplerRef, VkImageView depthStencilView,
    VkImageView colorView, VkBuffer configBuffer, uint64_t configBufferSize,
    VkImageView noiseMap, VkImageView hairShadowMap, VkPhysicalDeviceMemoryProperties memProperties,
    uint32_t width, uint32_t height, VkFormat depthStencilFormat, VkFormat colorFormat)
{
    VkResult vr;

    AMD_CHECKED_VULKAN_CALL(
        CreateScreenSizedItems(pd3dDevice, winWidth, winHeight, memProperties));
    AMD_CHECKED_VULKAN_CALL(
        CreateLayouts(pd3dDevice, mesh_layout, noiseSamplerRef, shadowSamplerRef));
    AMD_CHECKED_VULKAN_CALL(CreateRenderStateObjects(pd3dDevice, depthStencilFormat, colorFormat));
    AMD_CHECKED_VULKAN_CALL(
        CreateFramebuffer(pd3dDevice, depthStencilView, colorView, width, height));
    AMD_CHECKED_VULKAN_CALL(AllocateAndPopulateSets(pd3dDevice, configBuffer, configBufferSize,
                                         noiseMap, hairShadowMap))

    return VK_SUCCESS;
}

VkResult TressFXShortCut::OnResizedSwapChain(VkDevice pd3dDevice, int winWidth,
                                             int winHeight,
                                             VkPhysicalDeviceMemoryProperties memProperties)
{
    DestroyScreenSizedItems();

    VkResult vr;
    AMD_CHECKED_VULKAN_CALL(
        CreateScreenSizedItems(pd3dDevice, winWidth, winHeight, memProperties));
    return VK_SUCCESS;
}

void TressFXShortCut::OnDestroy(bool destroyShaders)
{
    DestroyScreenSizedItems();

    if (destroyShaders)
    {
        AMD_SAFE_RELEASE(m_pPLRenderHairAAStrandCopiesDepthsAlpha, vkDestroyPipeline,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairAAStrandCopiesFillColors, vkDestroyPipeline,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairAADepthsAlpha, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairAAFillColors, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairStrandCopiesDepthsAlpha, vkDestroyPipeline,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairStrandCopiesFillColors, vkDestroyPipeline,
                         m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairDepthsAlpha, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLRenderHairFillColors, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLDepthResolve, vkDestroyPipeline, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pPLColorResolve, vkDestroyPipeline, m_pvkDevice);

        AMD_SAFE_RELEASE(m_pRPRenderHair, vkDestroyRenderPass, m_pvkDevice);

        AMD_SAFE_RELEASE(m_depthPassPipelineLayout, vkDestroyPipelineLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_colorPassPipelineLayout, vkDestroyPipelineLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pSLDepthAlpha, vkDestroyDescriptorSetLayout, m_pvkDevice);
        AMD_SAFE_RELEASE(m_pSLColors, vkDestroyDescriptorSetLayout, m_pvkDevice);
    }
    AMD_SAFE_RELEASE(m_pDPShortcutPool, vkDestroyDescriptorPool, m_pvkDevice);
}

void TressFXShortCut::SetupDepthPass(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                                     uint32_t width, uint32_t height)
{
    VkClearColorValue dwDepthClearMax{};
    dwDepthClearMax.uint32[0] = SHORTCUT_INITIAL_DEPTH;
    dwDepthClearMax.uint32[1] = SHORTCUT_INITIAL_DEPTH;
    dwDepthClearMax.uint32[2] = SHORTCUT_INITIAL_DEPTH;
    dwDepthClearMax.uint32[3] = SHORTCUT_INITIAL_DEPTH;
    VkImageSubresourceRange range{};
    range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    range.layerCount = SHORTCUT_NUM_DEPTHS;
    range.levelCount = 1;
    vkCmdClearColorImage(commandBuffer, m_pFragmentDepthsTexture, VK_IMAGE_LAYOUT_GENERAL,
                         &dwDepthClearMax, 1, &range);

    {
        VkRenderPassBeginInfo info{VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO};
        info.framebuffer = m_pFBRenderHair;
        info.renderPass = m_pRPRenderHair;
        info.renderArea.extent.height = height;
        info.renderArea.extent.width = width;
        VkClearValue clearValue[3];
        clearValue[0].depthStencil.stencil = 0;
        // alpha
        clearValue[1].color.float32[0] = 1;
        clearValue[1].color.float32[1] = 1;
        clearValue[1].color.float32[2] = 1;
        clearValue[1].color.float32[3] = 1;
        // color
        clearValue[2].color.float32[0] = 0.;
        clearValue[2].color.float32[1] = 0.;
        clearValue[2].color.float32[2] = 0.;
        clearValue[2].color.float32[3] = 0.;
        info.clearValueCount = 3;
        info.pClearValues = clearValue;
        vkCmdBeginRenderPass(commandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);
    }
}

void TressFXShortCut::SetupResolveDepth(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                                        uint32_t width, uint32_t height)
{
    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
}

void TressFXShortCut::SetupShadePass(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                                     uint32_t width, uint32_t height)
{
    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
}

void TressFXShortCut::SetupResolveColor(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
                                        uint32_t width, uint32_t height)
{
    vkCmdNextSubpass(commandBuffer, VK_SUBPASS_CONTENTS_INLINE);
}
}
