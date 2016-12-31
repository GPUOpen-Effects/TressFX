#pragma once

#include <vector>
#include <vulkan\vulkan.h>

#define AMD_CHECKED_VULKAN_CALL(x)                                                                  \
    {                                                                                    \
        vr = (x);                                                                        \
        if (vr != VK_SUCCESS)                                                            \
        {                                                                                \
            return vr;                                                                   \
        }                                                                                \
    }

#undef AMD_SAFE_RELEASE
#define AMD_SAFE_RELEASE(object, releaseFunction, device) if (object != VK_NULL_HANDLE) releaseFunction(device, object, nullptr);

namespace AMD
{
    struct DebugMarkerPointer
    {
        DebugMarkerPointer() {}

        void init(VkDevice device)
        {
            dev = device;
            pfnDebugMarkerSetObjectTag = (PFN_vkDebugMarkerSetObjectTagEXT)vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectTagEXT");
            pfnDebugMarkerSetObjectName = (PFN_vkDebugMarkerSetObjectNameEXT)vkGetDeviceProcAddr(device, "vkDebugMarkerSetObjectNameEXT");
            pfnCmdDebugMarkerBegin = (PFN_vkCmdDebugMarkerBeginEXT)vkGetDeviceProcAddr(device, "vkCmdDebugMarkerBeginEXT");
            pfnCmdDebugMarkerEnd = (PFN_vkCmdDebugMarkerEndEXT)vkGetDeviceProcAddr(device, "vkCmdDebugMarkerEndEXT");
            pfnCmdDebugMarkerInsert = (PFN_vkCmdDebugMarkerInsertEXT)vkGetDeviceProcAddr(device, "vkCmdDebugMarkerInsertEXT");
        }

        void nameObject(VkBuffer object, const char* name) const
        {
            nameObject_impl(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_EXT, reinterpret_cast<uint64_t>(object), name);
        }

        void nameObject(VkBufferView object, const char* name) const
        {
            nameObject_impl(VK_DEBUG_REPORT_OBJECT_TYPE_BUFFER_VIEW_EXT, reinterpret_cast<uint64_t>(object), name);
        }

        void nameObject(VkDeviceMemory object, const char* name) const
        {
            nameObject_impl(VK_DEBUG_REPORT_OBJECT_TYPE_DEVICE_MEMORY_EXT, reinterpret_cast<uint64_t>(object), name);
        }

        void nameObject(VkPipeline object, const char* name) const
        {

            nameObject_impl(VK_DEBUG_REPORT_OBJECT_TYPE_PIPELINE_EXT, reinterpret_cast<uint64_t>(object), name);
        }

        void markBeginRegion(VkCommandBuffer cmdBuffer, const char* name) const
        {
            if (!pfnCmdDebugMarkerBegin)
                return;
            VkDebugMarkerMarkerInfoEXT info{ VK_STRUCTURE_TYPE_DEBUG_MARKER_MARKER_INFO_EXT };
            info.pMarkerName = name;
            info.color[0] = 1.f;
            pfnCmdDebugMarkerBegin(cmdBuffer, &info);
        }

        void markEndRegion(VkCommandBuffer cmdBuffer) const
        {
            if (!pfnCmdDebugMarkerEnd)
                return;
            pfnCmdDebugMarkerEnd(cmdBuffer);
        }

    private:
        PFN_vkDebugMarkerSetObjectTagEXT pfnDebugMarkerSetObjectTag = NULL;
        PFN_vkDebugMarkerSetObjectNameEXT pfnDebugMarkerSetObjectName = NULL;
        PFN_vkCmdDebugMarkerBeginEXT pfnCmdDebugMarkerBegin = NULL;
        PFN_vkCmdDebugMarkerEndEXT pfnCmdDebugMarkerEnd = NULL;
        PFN_vkCmdDebugMarkerInsertEXT pfnCmdDebugMarkerInsert = NULL;

        VkDevice dev;

        void nameObject_impl(VkDebugReportObjectTypeEXT type, uint64_t object, const char* name) const
        {
            if (!pfnDebugMarkerSetObjectName)
                return;
            VkDebugMarkerObjectNameInfoEXT info{ VK_STRUCTURE_TYPE_DEBUG_MARKER_OBJECT_NAME_INFO_EXT };
            info.objectType = type;
            info.object = object;
            info.pObjectName = name;
            pfnDebugMarkerSetObjectName(dev, &info);
        }
    };


    struct ShaderModule
    {
        VkShaderModule m_shaderModule;

        ShaderModule(VkDevice dev, const std::vector<uint32_t> &code);
        ~ShaderModule();
    private:
        VkDevice m_pvkDevice;

        ShaderModule(const ShaderModule&) {};
        ShaderModule& operator=(const ShaderModule&) {};
    };

    uint32_t getMemoryTypeIndex(uint32_t typeBits, const VkPhysicalDeviceMemoryProperties &memprops, VkMemoryPropertyFlags properties);
    VkDeviceSize align(VkDeviceSize offset, VkDeviceSize alignment);
    VkDeviceMemory allocBufferMemory(VkDevice dev, VkBuffer buffer,
        const VkPhysicalDeviceMemoryProperties &memprops, VkMemoryPropertyFlags properties);
    VkDeviceMemory allocImageMemory(VkDevice dev, VkImage image,
        const VkPhysicalDeviceMemoryProperties &memprops);

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
        VkAccessFlags dstAccess, size_t offset = 0u,
        uint64_t size = VK_WHOLE_SIZE);

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
            uint32_t layerCount,
            VkImageAspectFlags aspect);
}