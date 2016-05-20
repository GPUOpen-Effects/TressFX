#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#define AMD_V_RETURN(x)                                                                  \
    {                                                                                    \
        vr = (x);                                                                        \
        if (vr != VK_SUCCESS)                                                            \
        {                                                                                \
            return vr;                                                                   \
        }                                                                                \
    }

namespace AMD
{
struct ShaderModule
{
    VkShaderModule m_shaderModule;

    ShaderModule(VkDevice dev, const std::vector<uint32_t> &code);
    ~ShaderModule();

  private:
    VkDevice m_pvkDevice;
};

VkDeviceMemory allocBufferMemory(VkDevice dev, VkBuffer buffer,
                                 uint32_t texture_memory_index);
VkDeviceMemory allocImageMemory(VkDevice dev, VkImage image,
                                uint32_t texture_memory_index);

VkWriteDescriptorSet getWriteDescriptor(VkDescriptorSet dstSet, uint32_t dstBinding,
                                        VkDescriptorType descriptorType,
                                        const VkBufferView *texelBufferView);

VkWriteDescriptorSet getWriteDescriptor(VkDescriptorSet dstSet, uint32_t dstBinding,
                                        VkDescriptorType descriptorType,
                                        const VkDescriptorBufferInfo *Bufferdescriptor);

VkWriteDescriptorSet getWriteDescriptor(VkDescriptorSet dstSet, uint32_t dstBinding,
                                        VkDescriptorType descriptorType,
                                        const VkDescriptorImageInfo *descriptor);

void fillInitialData(VkCommandBuffer commandBuffer, VkBuffer scratchBuffer,
                     void *pScratchBuffer, void *pDataToUpload, VkBuffer destBuffer,
                     size_t &offsetInScratchBuffer, VkDeviceSize size);

VkResult getDescriptorLayout(VkDevice pvkDevice, const VkDescriptorSetLayoutBinding *ptr,
                             size_t count, VkDescriptorSetLayout &result);
VkBufferMemoryBarrier getBufferBarrier(VkBuffer buffer, VkAccessFlags srcAccess,
                                       VkAccessFlags dstAccess, size_t offset = 0,
                                       size_t size = VK_WHOLE_SIZE);

VkPipelineShaderStageCreateInfo getShaderStageCreateInfo(VkShaderModule module,
                                                         VkShaderStageFlagBits stage,
                                                         const char *shaderName);

VkImageCreateInfo getImageCreateInfo(VkFormat format, uint32_t width, uint32_t height,
                                     VkImageUsageFlags usage, uint32_t layers = 1);

struct CommonPipelineState
{
    // No tesselation
    static const VkPipelineTessellationStateCreateInfo tessellationState;
    // One viewport and scissor
    static const VkDynamicState dynamicStates[2];
    static const VkPipelineDynamicStateCreateInfo dynamicState;
    // One sample
    static const VkPipelineMultisampleStateCreateInfo multisampleState;
    // One viewport
    static const VkPipelineViewportStateCreateInfo viewportState;
    static const VkPipelineRasterizationStateCreateInfo rasterizationCreateInfo;

    static const VkPipelineVertexInputStateCreateInfo m_pLayoutHair;
    // Full screen quad layout structure
    static const VkPipelineVertexInputStateCreateInfo m_pLayoutQuad;

    // Triangles list
    static const VkPipelineInputAssemblyStateCreateInfo inputAssemblyTriangle;
    // Lines list
    static const VkPipelineInputAssemblyStateCreateInfo inputAssemblyLine;

    static VkGraphicsPipelineCreateInfo getBasePipelineCreateInfo(
        const VkPipelineVertexInputStateCreateInfo *vertex_input_state,
        const VkPipelineDepthStencilStateCreateInfo *depth_stencil_state,
        const VkPipelineColorBlendStateCreateInfo *color_blend_state,
        const VkPipelineInputAssemblyStateCreateInfo *input_assembly,
        const VkPipelineShaderStageCreateInfo *stages, VkPipelineLayout layout,
        VkRenderPass render_pass, uint32_t subpass);

    static const VkPipelineDepthStencilStateCreateInfo DepthTestEnabledDesc;
    // Enable depth test to use early z, disable depth write to make sure required layers
    // won't be clipped out in early z
    static const VkPipelineDepthStencilStateCreateInfo
        DepthTestEnabledNoDepthWritesStencilWriteIncrementDesc;
    static const VkPipelineDepthStencilStateCreateInfo
        DepthTestDisabledStencilTestLessDSS;
    static const VkPipelineDepthStencilStateCreateInfo
        m_pDepthWriteEnabledStencilTestLess_DSS;

    // disable color write if there is no need for fragments counting
    static const VkPipelineColorBlendStateCreateInfo ColorWritesOff;
    static const VkPipelineColorBlendStateCreateInfo BlendStateBlendToBg;
    static const VkPipelineColorBlendStateCreateInfo m_pDepthWritesToColor_BS;
    static const VkPipelineColorBlendStateCreateInfo m_pResolveColor_BS;
    static const VkPipelineColorBlendStateCreateInfo m_pSum_BS;
};

VkImageMemoryBarrier
getImageMemoryBarrier(VkImage image, VkAccessFlags srcMask, VkAccessFlags dstMask,
                      VkImageLayout oldLayout, VkImageLayout newLayout,
                      VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT);
}