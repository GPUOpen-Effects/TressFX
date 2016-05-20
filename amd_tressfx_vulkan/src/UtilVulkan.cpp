#include "UtilVulkan.h"

namespace AMD
{
ShaderModule::ShaderModule(VkDevice dev, const std::vector<uint32_t> &code)
    : m_pvkDevice(dev)
{
    VkShaderModuleCreateInfo moduleInfo{VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
                                        nullptr, 0, code.size() * sizeof(uint32_t),
                                        code.data()};

    vkCreateShaderModule(m_pvkDevice, &moduleInfo, nullptr, &m_shaderModule);
}

ShaderModule::~ShaderModule()
{
    vkDestroyShaderModule(m_pvkDevice, m_shaderModule, nullptr);
}

VkDeviceMemory allocBufferMemory(VkDevice dev, VkBuffer buffer,
                                 uint32_t texture_memory_index)
{
    VkMemoryRequirements memReqs;
    vkGetBufferMemoryRequirements(dev, buffer, &memReqs);

    VkMemoryAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocateInfo.allocationSize = memReqs.size;
    allocateInfo.memoryTypeIndex = texture_memory_index;
    VkDeviceMemory result;
    vkAllocateMemory(dev, &allocateInfo, nullptr, &result);
    vkBindBufferMemory(dev, buffer, result, 0);
    return result;
}

VkDeviceMemory allocImageMemory(VkDevice dev, VkImage image,
                                uint32_t texture_memory_index)
{
    VkMemoryRequirements memReqs;
    vkGetImageMemoryRequirements(dev, image, &memReqs);

    VkMemoryAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
    allocateInfo.allocationSize = memReqs.size;
    allocateInfo.memoryTypeIndex = texture_memory_index;
    VkDeviceMemory result;
    vkAllocateMemory(dev, &allocateInfo, nullptr, &result);
    vkBindImageMemory(dev, image, result, 0);
    return result;
}

VkWriteDescriptorSet getWriteDescriptor(VkDescriptorSet dstSet, uint32_t dstBinding,
                                        VkDescriptorType descriptorType,
                                        const VkBufferView *texelBufferView)
{
    VkWriteDescriptorSet result{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    result.descriptorCount = 1;
    result.dstSet = dstSet;
    result.dstBinding = dstBinding;
    result.descriptorType = descriptorType;
    result.pTexelBufferView = texelBufferView;
    return result;
}

VkWriteDescriptorSet getWriteDescriptor(VkDescriptorSet dstSet, uint32_t dstBinding,
                                        VkDescriptorType descriptorType,
                                        const VkDescriptorBufferInfo *Bufferdescriptor)
{
    VkWriteDescriptorSet result{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    result.descriptorCount = 1;
    result.dstSet = dstSet;
    result.dstBinding = dstBinding;
    result.descriptorType = descriptorType;
    result.pBufferInfo = Bufferdescriptor;
    return result;
}

VkWriteDescriptorSet getWriteDescriptor(VkDescriptorSet dstSet, uint32_t dstBinding,
                                        VkDescriptorType descriptorType,
                                        const VkDescriptorImageInfo *descriptor)
{
    VkWriteDescriptorSet result{VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET};
    result.descriptorCount = 1;
    result.dstSet = dstSet;
    result.dstBinding = dstBinding;
    result.descriptorType = descriptorType;
    result.pImageInfo = descriptor;
    return result;
}

void fillInitialData(VkCommandBuffer commandBuffer, VkBuffer scratchBuffer,
                     void *pScratchBuffer, void *pDataToUpload, VkBuffer destBuffer,
                     size_t &offsetInScratchBuffer, VkDeviceSize size)
{
    memcpy(reinterpret_cast<char *>(pScratchBuffer) + offsetInScratchBuffer,
           pDataToUpload, size);
    VkBufferCopy copyInfo{offsetInScratchBuffer, 0, static_cast<uint32_t>(size)};
    vkCmdCopyBuffer(commandBuffer, scratchBuffer, destBuffer, 1, &copyInfo);
    offsetInScratchBuffer += size;
}

VkResult getDescriptorLayout(VkDevice pvkDevice, const VkDescriptorSetLayoutBinding *ptr,
                             size_t count, VkDescriptorSetLayout &result)
{
    VkDescriptorSetLayoutCreateInfo info{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO};
    info.bindingCount = static_cast<uint32_t>(count);
    info.pBindings = ptr;

    VkResult vr;
    AMD_V_RETURN(vkCreateDescriptorSetLayout(pvkDevice, &info, nullptr, &result));
    return VK_SUCCESS;
}

VkBufferMemoryBarrier getBufferBarrier(VkBuffer buffer, VkAccessFlags srcAccess,
                                       VkAccessFlags dstAccess, size_t offset,
                                       size_t size)
{
    VkBufferMemoryBarrier result{VK_STRUCTURE_TYPE_BUFFER_MEMORY_BARRIER};
    result.buffer = buffer;
    result.srcAccessMask = srcAccess;
    result.dstAccessMask = dstAccess;
    result.offset = offset;
    result.size = size;
    return result;
}

VkPipelineShaderStageCreateInfo getShaderStageCreateInfo(VkShaderModule module,
                                                         VkShaderStageFlagBits stage,
                                                         const char *shaderName)
{
    VkPipelineShaderStageCreateInfo shaderStageInfo{
        VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO};
    shaderStageInfo.module = module;
    shaderStageInfo.pName = shaderName;
    shaderStageInfo.stage = stage;
    return shaderStageInfo;
}

// No tesselation
const VkPipelineTessellationStateCreateInfo CommonPipelineState::tessellationState{
    VK_STRUCTURE_TYPE_PIPELINE_TESSELLATION_STATE_CREATE_INFO};
const VkDynamicState CommonPipelineState::dynamicStates[] = {VK_DYNAMIC_STATE_VIEWPORT,
                                                             VK_DYNAMIC_STATE_SCISSOR};
const VkPipelineDynamicStateCreateInfo CommonPipelineState::dynamicState{
    VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO, nullptr, 0, 2, dynamicStates};
// One sample
const VkPipelineMultisampleStateCreateInfo CommonPipelineState::multisampleState{
    VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO, nullptr, 0,
    VK_SAMPLE_COUNT_1_BIT};
// One viewport and scissor
const VkPipelineViewportStateCreateInfo CommonPipelineState::viewportState{
    VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
    nullptr,
    0,
    1,
    nullptr,
    1,
    nullptr};
const VkPipelineRasterizationStateCreateInfo
    CommonPipelineState::rasterizationCreateInfo = {
        VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
        nullptr,
        0,
        VK_FALSE,
        VK_FALSE,
        VK_POLYGON_MODE_FILL,
        VK_CULL_MODE_NONE,
        VK_FRONT_FACE_CLOCKWISE,
        VK_FALSE,
        0.,
        0.,
        0.,
        1.};

const VkPipelineInputAssemblyStateCreateInfo CommonPipelineState::inputAssemblyTriangle =
    {VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0,
     VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST, false};

const VkPipelineInputAssemblyStateCreateInfo CommonPipelineState::inputAssemblyLine = {
    VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO, nullptr, 0,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST, false};

const VkPipelineVertexInputStateCreateInfo CommonPipelineState::m_pLayoutHair{
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO};

VkGraphicsPipelineCreateInfo CommonPipelineState::getBasePipelineCreateInfo(
    const VkPipelineVertexInputStateCreateInfo *vertex_input_state,
    const VkPipelineDepthStencilStateCreateInfo *depth_stencil_state,
    const VkPipelineColorBlendStateCreateInfo *color_blend_state,
    const VkPipelineInputAssemblyStateCreateInfo *input_assembly,
    const VkPipelineShaderStageCreateInfo *stages, VkPipelineLayout layout,
    VkRenderPass render_pass, uint32_t subpass)
{
    VkGraphicsPipelineCreateInfo pipelineCreateInfo{
        VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO};
    pipelineCreateInfo.pTessellationState = &tessellationState;
    pipelineCreateInfo.pMultisampleState = &multisampleState;
    pipelineCreateInfo.pVertexInputState = vertex_input_state;
    pipelineCreateInfo.pViewportState = &viewportState;
    pipelineCreateInfo.pDynamicState = &dynamicState;
    pipelineCreateInfo.pDepthStencilState = depth_stencil_state;
    pipelineCreateInfo.pColorBlendState = color_blend_state;
    pipelineCreateInfo.stageCount = 2;
    pipelineCreateInfo.basePipelineHandle = VK_NULL_HANDLE;
    pipelineCreateInfo.basePipelineIndex = -1;
    pipelineCreateInfo.pRasterizationState = &rasterizationCreateInfo;
    pipelineCreateInfo.pInputAssemblyState = input_assembly;
    pipelineCreateInfo.pStages = stages;
    pipelineCreateInfo.layout = layout;
    pipelineCreateInfo.renderPass = render_pass;
    pipelineCreateInfo.subpass = subpass;
    return pipelineCreateInfo;
}

namespace
{
VkPipelineDepthStencilStateCreateInfo
getSpecializedDSS(bool depth_test_enable, bool depth_write_enable,
                  bool stencil_test_enable, VkCompareOp stencil_op, VkStencilOp pass_op,
                  uint32_t write_mask)
{
    VkPipelineDepthStencilStateCreateInfo DSS{
        VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO};
    DSS.depthTestEnable = depth_test_enable;
    DSS.depthCompareOp = VK_COMPARE_OP_LESS_OR_EQUAL;
    DSS.depthWriteEnable = depth_write_enable;
    DSS.stencilTestEnable = stencil_test_enable;
    DSS.front.compareMask = 0xff;
    DSS.front.writeMask = write_mask;
    DSS.front.failOp = VK_STENCIL_OP_KEEP;
    DSS.front.depthFailOp = VK_STENCIL_OP_KEEP;
    DSS.front.passOp = pass_op;
    DSS.front.compareOp = stencil_op;
    DSS.back.compareMask = 0xff;
    DSS.back.writeMask = write_mask;
    DSS.back.failOp = VK_STENCIL_OP_KEEP;
    DSS.back.depthFailOp = VK_STENCIL_OP_KEEP;
    DSS.back.passOp = pass_op;
    DSS.back.compareOp = stencil_op;
    return DSS;
}

const VkPipelineColorBlendAttachmentState BlendStateBlendToBg_render_target_0{
    true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_OP_ADD,
    VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_A_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_G_BIT};

const VkPipelineColorBlendAttachmentState m_pDepthWritesToColor_BS_render_target_0{
    true,
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_OP_ADD,
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_OP_ADD,
    VK_COLOR_COMPONENT_R_BIT};

const VkPipelineColorBlendAttachmentState m_pResolveColor_BS_BS_render_target_0{
    true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_SRC_ALPHA, VK_BLEND_OP_ADD,
    VK_BLEND_FACTOR_ZERO, VK_BLEND_FACTOR_ZERO, VK_BLEND_OP_ADD,
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT};

const VkPipelineColorBlendAttachmentState m_pSum_BS_render_target_0{
    true, VK_BLEND_FACTOR_ONE, VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD, VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_ONE, VK_BLEND_OP_ADD,
    VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT |
        VK_COLOR_COMPONENT_A_BIT};

// Full screen quad layout structure
const VkVertexInputBindingDescription layout_quad_bindings[1] = {
    {0, 32, VK_VERTEX_INPUT_RATE_VERTEX}};
const VkVertexInputAttributeDescription layout_quad[3] = {
    {0, 0, VK_FORMAT_R32G32B32_SFLOAT, 0},  // POSITION
    {1, 0, VK_FORMAT_R32G32B32_SFLOAT, 12}, // TANGENT
    {2, 0, VK_FORMAT_R32G32_SFLOAT, 24},    // TEXCOORD
};
}

const VkPipelineDepthStencilStateCreateInfo CommonPipelineState::DepthTestEnabledDesc =
    getSpecializedDSS(true, true, false, VK_COMPARE_OP_NEVER, VK_STENCIL_OP_KEEP, 0xff);
const VkPipelineDepthStencilStateCreateInfo
    CommonPipelineState::DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc =
        getSpecializedDSS(true, false, true, VK_COMPARE_OP_ALWAYS,
                          VK_STENCIL_OP_INCREMENT_AND_WRAP, 0xff);
const VkPipelineDepthStencilStateCreateInfo
    CommonPipelineState::DepthTestDisabledStencilTestLessDSS = getSpecializedDSS(
        false, false, true, VK_COMPARE_OP_LESS, VK_STENCIL_OP_KEEP, 0x00);
const VkPipelineDepthStencilStateCreateInfo
    CommonPipelineState::m_pDepthWriteEnabledStencilTestLess_DSS =
        getSpecializedDSS(true, true, true, VK_COMPARE_OP_LESS, VK_STENCIL_OP_KEEP, 0x0);

// disable color write if there is no need for fragments counting
const VkPipelineColorBlendStateCreateInfo CommonPipelineState::ColorWritesOff{
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO};

const VkPipelineColorBlendStateCreateInfo CommonPipelineState::BlendStateBlendToBg{
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_FALSE,
    VK_LOGIC_OP_NO_OP,
    1,
    &BlendStateBlendToBg_render_target_0};

const VkPipelineColorBlendStateCreateInfo CommonPipelineState::m_pDepthWritesToColor_BS{
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_FALSE,
    VK_LOGIC_OP_NO_OP,
    1,
    &m_pDepthWritesToColor_BS_render_target_0};

const VkPipelineColorBlendStateCreateInfo CommonPipelineState::m_pResolveColor_BS{
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_FALSE,
    VK_LOGIC_OP_NO_OP,
    1,
    &m_pResolveColor_BS_BS_render_target_0};

const VkPipelineColorBlendStateCreateInfo CommonPipelineState::m_pSum_BS{
    VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
    nullptr,
    0,
    VK_FALSE,
    VK_LOGIC_OP_NO_OP,
    1,
    &m_pSum_BS_render_target_0};

const VkPipelineVertexInputStateCreateInfo CommonPipelineState::m_pLayoutQuad{
    VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
    nullptr,
    0,
    1,
    layout_quad_bindings,
    3,
    layout_quad};

VkImageMemoryBarrier getImageMemoryBarrier(VkImage image, VkAccessFlags srcMask,
                                           VkAccessFlags dstMask, VkImageLayout oldLayout,
                                           VkImageLayout newLayout,
                                           VkImageAspectFlags aspect)
{
    VkImageMemoryBarrier memoryBarrier{VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER};
    memoryBarrier.srcAccessMask = srcMask;
    memoryBarrier.dstAccessMask = dstMask;
    memoryBarrier.oldLayout = oldLayout;
    memoryBarrier.newLayout = newLayout;
    memoryBarrier.image = image;
    memoryBarrier.subresourceRange.aspectMask = aspect;
    memoryBarrier.subresourceRange.levelCount =
        memoryBarrier.subresourceRange.layerCount = 1;
    return memoryBarrier;
}

VkImageCreateInfo getImageCreateInfo(VkFormat format, uint32_t width, uint32_t height,
                                     VkImageUsageFlags usage, uint32_t layers)
{
    VkImageCreateInfo tex2D_desc{VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO};
    tex2D_desc.extent.width = width;
    tex2D_desc.extent.height = height;
    tex2D_desc.extent.depth = 1;
    tex2D_desc.arrayLayers = layers;
    tex2D_desc.mipLevels = 1;
    tex2D_desc.format = format;
    tex2D_desc.usage = usage;
    tex2D_desc.samples = VK_SAMPLE_COUNT_1_BIT;
    tex2D_desc.imageType = VK_IMAGE_TYPE_2D;
    tex2D_desc.tiling = VK_IMAGE_TILING_OPTIMAL;
    return tex2D_desc;
}
}
