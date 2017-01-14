//--------------------------------------------------------------------------------------
// File: TressFXMesh.cpp
//
// Hair mesh code
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

#include "TressFXMeshVulkan.h"
#include "AMD_Types.h"
#include "UtilVulkan.h"

#include <sstream>
#include <string>

using namespace std;
using namespace DirectX;

namespace AMD
{

//--------------------------------------------------------------------------------------
//
// Constructor
//
// Used for initializing member variables to default values.
//--------------------------------------------------------------------------------------
TressFXMesh::TressFXMesh(void)
    : m_pIndexBuffer(NULL), m_pTriangleIndexBuffer(NULL), m_pThicknessCoeffsBuffer(NULL), m_pThicknessIndexTriangleIndexMemory(NULL),
      m_pThicknessCoeffsView(NULL)
{
    m_HairVertexPositionsBuffer = NULL;
    m_HairVertexPositionsView = NULL;
    m_HairVertexPositionsPrevBuffer = NULL;

    m_HairStrandTypeBuffer = NULL;
    m_HairStrandTypeView = NULL;

    m_HairVertexPositionsView = NULL;
    m_HairVertexPositionsPrevView = NULL;

    m_HairLengthBuffer = NULL;
    m_HairRestLengthSRV = NULL;

    m_InitialHairPositionsBuffer = NULL;
    m_InitialHairPositionsView = NULL;

    m_HairVertexTangentsBuffer = NULL;
    m_HairVertexTangentsView = NULL;

    m_HairRefVectorsBuffer = NULL;
    m_HairRefVecsInLocalFrameView = NULL;

    m_FollowHairRootOffsetBuffer = NULL;
    m_FollowHairRootOffsetView = NULL;

    m_GlobalRotationsBuffer = NULL;
    m_GlobalRotationsView = NULL;

    m_LocalRotationsBuffer = NULL;
    m_LocalRotationsView = NULL;

    m_HairAsset.m_pHairStrandType = NULL;
    m_HairAsset.m_pRefVectors = NULL;
    m_HairAsset.m_pTriangleVertices = NULL;
    m_HairAsset.m_pGlobalRotations = NULL;
    m_HairAsset.m_pLocalRotations = NULL;
    m_HairAsset.m_pVertices = NULL;
    m_HairAsset.m_pTangents = NULL;
    m_HairAsset.m_pThicknessCoeffs = NULL;
    m_HairAsset.m_pRestLengths = NULL;
    m_HairAsset.m_pFollowRootOffset = NULL;

    m_HairAsset.m_NumFollowHairsPerGuideHair = 4;
    m_HairAsset.m_NumGuideHairStrands = 0;
    m_HairAsset.m_NumOfVerticesInStrand = 2;

    m_HairSkinMappingBuffer = NULL;
    m_HairTransformsBuffer = NULL;
    m_HairSkinMappingView = NULL;
    m_HairTransformsView = NULL;
    m_pStrandTexCoordBuffer = NULL;
    m_pStrandTexCoordView = NULL;
}

//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
TressFXMesh::~TressFXMesh(void) { OnDestroy(); }

//--------------------------------------------------------------------------------------
//
// OnCreate
//
//  Called to create a hair mesh. This function creates the VK resources
//  and stores the hair data into these resources.
//
//--------------------------------------------------------------------------------------
VkResult TressFXMesh::OnCreate(VkDevice pvkDevice,
 TressFX_HairBlob *pHairBlob,
                               TressFX_SceneMesh *sceneMesh,
 VkImageView pTexture,
                               VkPhysicalDeviceMemoryProperties memProperties,
                               VkCommandBuffer upload_cmd_buffer,
 VkBuffer scratchBuffer,
                               VkDeviceMemory scratchMemory,
                               VkDescriptorSetLayout GlobalConstraintsSetLayout,
                               VkDescriptorSetLayout LocalConstraintsSetLayout,
                               VkDescriptorSetLayout LenghtWindCollisionSetLayout,
                               VkDescriptorSetLayout PrepareFollowHairSetLayout,
                               VkDescriptorSetLayout UpdateFollowHaitSetLayout,
                               VkDescriptorSetLayout computeTangentSetLayout,
                               VkDescriptorSetLayout Pass1SetLayout,
                               VkDescriptorSetLayout ShadowSetLayout, const DebugMarkerPointer& markerCallbacks)
{
    VkResult vr;
    size_t offsetInUploadBuffer = 0;

    // load the binary file
    if (!Deserialize(pHairBlob))
    {
        return VK_ERROR_INITIALIZATION_FAILED;
    }

    // Create Buffers
    {
        // thickness coeff buffer
        VkBufferCreateInfo bd{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bd.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        bd.size = sizeof(float) * m_HairAsset.m_NumTotalHairVertices;
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bd, nullptr, &m_pThicknessCoeffsBuffer));

        //-----------------------------------
        // Index buffer (lines and triangles)
        //-----------------------------------

        // Line index buffer
        m_TotalIndexCount = (int)m_HairAsset.m_LineIndices.size();
        bd.size = (UINT)(sizeof(unsigned int) * m_TotalIndexCount);
        bd.usage = VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bd, nullptr, &m_pIndexBuffer));

        // Triangle index buffer
        m_TotalTriangleIndexCount = (int)m_HairAsset.m_TriangleIndices.size();
        bd.size = (UINT)(sizeof(unsigned int) * m_TotalTriangleIndexCount);
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bd, nullptr, &m_pTriangleIndexBuffer));

        VkMemoryRequirements thicknessBufferMemReq;
        vkGetBufferMemoryRequirements(pvkDevice, m_pThicknessCoeffsBuffer, &thicknessBufferMemReq);
        VkMemoryRequirements indexBufferMemReq;
        vkGetBufferMemoryRequirements(pvkDevice, m_pIndexBuffer, &indexBufferMemReq);
        VkMemoryRequirements triangleIndexBufferMemReq;
        vkGetBufferMemoryRequirements(pvkDevice, m_pTriangleIndexBuffer, &triangleIndexBufferMemReq);

        uint32_t memoryType = getMemoryTypeIndex(indexBufferMemReq.memoryTypeBits & triangleIndexBufferMemReq.memoryTypeBits & thicknessBufferMemReq.memoryTypeBits,
            memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

        if (memoryType == -1) return VK_ERROR_INITIALIZATION_FAILED;

        VkDeviceSize indexBufferOffset = align(thicknessBufferMemReq.size, indexBufferMemReq.alignment);
        VkDeviceSize triangleIndexBufferOffset = align(indexBufferOffset + indexBufferMemReq.size, triangleIndexBufferMemReq.alignment);
        VkDeviceSize totalSize = triangleIndexBufferOffset + triangleIndexBufferMemReq.size;

        VkMemoryAllocateInfo allocateInfo{VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO};
        allocateInfo.allocationSize = totalSize;
        allocateInfo.memoryTypeIndex = memoryType;
        vkAllocateMemory(pvkDevice, &allocateInfo, nullptr, &m_pThicknessIndexTriangleIndexMemory);

        vkBindBufferMemory(pvkDevice, m_pThicknessCoeffsBuffer, m_pThicknessIndexTriangleIndexMemory, 0);
        vkBindBufferMemory(pvkDevice, m_pIndexBuffer, m_pThicknessIndexTriangleIndexMemory, indexBufferOffset);
        vkBindBufferMemory(pvkDevice, m_pTriangleIndexBuffer, m_pThicknessIndexTriangleIndexMemory, triangleIndexBufferOffset);

        // Fill them
        void *uploadBuffer;
        vkMapMemory(pvkDevice, scratchMemory, 0, totalSize, 0, &uploadBuffer);

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pThicknessCoeffs, m_pThicknessCoeffsBuffer,
                        offsetInUploadBuffer, sizeof(float) * m_HairAsset.m_NumTotalHairVertices);

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        &m_HairAsset.m_LineIndices[0], m_pIndexBuffer, offsetInUploadBuffer,
                        sizeof(unsigned int) * m_TotalIndexCount);

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        &m_HairAsset.m_TriangleIndices[0], m_pTriangleIndexBuffer,
                        offsetInUploadBuffer, sizeof(unsigned int) * m_TotalTriangleIndexCount);
        vkUnmapMemory(pvkDevice, scratchMemory);
    }

    // thickness coeff buffer srv
    {
        VkBufferViewCreateInfo SRVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        SRVDesc.format = VK_FORMAT_R32_SFLOAT;
        SRVDesc.range = m_HairAsset.m_NumTotalHairVertices * sizeof(float);
        SRVDesc.buffer = m_pThicknessCoeffsBuffer;

        AMD_CHECKED_VULKAN_CALL(
            vkCreateBufferView(pvkDevice, &SRVDesc, nullptr, &m_pThicknessCoeffsView));
    }

    m_pHairTextureSRV = pTexture;

    AMD_CHECKED_VULKAN_CALL(CreateBufferAndViews(pvkDevice, sceneMesh,
 memProperties,
                              upload_cmd_buffer, scratchBuffer,
 scratchMemory,
                              offsetInUploadBuffer, markerCallbacks ));
    AMD_CHECKED_VULKAN_CALL(AllocateDescriptorsSets(pvkDevice, GlobalConstraintsSetLayout,
                                LocalConstraintsSetLayout, LenghtWindCollisionSetLayout,
                                PrepareFollowHairSetLayout, UpdateFollowHaitSetLayout,
                                computeTangentSetLayout, Pass1SetLayout, ShadowSetLayout));

    VkBufferMemoryBarrier bufferBarrier[] = {
        getBufferBarrier(m_pThicknessCoeffsBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_pIndexBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_INDEX_READ_BIT),
        getBufferBarrier(m_pTriangleIndexBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_INDEX_READ_BIT),
        getBufferBarrier(m_HairStrandTypeBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_InitialHairPositionsBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_HairLengthBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_HairRefVectorsBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_FollowHairRootOffsetBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_GlobalRotationsBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_LocalRotationsBuffer, VK_ACCESS_TRANSFER_WRITE_BIT,
                         VK_ACCESS_SHADER_READ_BIT),
    };

    vkCmdPipelineBarrier(upload_cmd_buffer, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
                         VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 0, 0, nullptr,
                         AMD_ARRAY_SIZE(bufferBarrier), bufferBarrier, 0, nullptr);

    DestroyAsset();

    return vr;
}

//--------------------------------------------------------------------------------------
//
// CreateBufferAndViews
//
// Called by the OnCreate function to create the VK resources
//
//--------------------------------------------------------------------------------------
VkResult TressFXMesh::CreateBufferAndViews(
    VkDevice pvkDevice,
 TressFX_SceneMesh *sceneMesh,
    VkPhysicalDeviceMemoryProperties memProp,
 VkCommandBuffer upload_cmd_buffer,
    VkBuffer scratchBuffer, VkDeviceMemory scratchMemory, size_t &offsetInUploadBuffer, const DebugMarkerPointer& markerCallbacks)
{
    VkResult vr;
    m_pvkDevice = pvkDevice;
    size_t sizeToUpload =
        // m_HairStrandTypeSRV
        m_HairAsset.m_NumTotalHairStrands * sizeof(int) +
        // m_InitialHairPositionsSRV
        m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4) +
        // m_HairVertexPositionsUAB
        2 * m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4) +
        // m_HairVertexTangentsUAB
        m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4) +
        // m_HairRestLengthSRV
        m_HairAsset.m_NumTotalHairVertices * sizeof(float) +
        // m_HairRefVecsInLocalFrameSRV
        m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4) +
        // m_FollowHairRootOffsetSRV
        m_HairAsset.m_NumTotalHairStrands * sizeof(XMFLOAT4) +
        // m_GlobalRotationsUAB
        m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4) +
        // m_LocalRotationsUAB
        m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4) +
        // m_pStrandTexCoordBuffer
        m_HairAsset.m_NumTotalHairStrands * sizeof(float) * 2;

    void *uploadBuffer;
    vkMapMemory(pvkDevice, scratchMemory, 0, sizeToUpload, 0, &uploadBuffer);

    //--------------------------------
    // m_HairStrandTypeSRV
    //--------------------------------
    {
        VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDesc.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        bufferDesc.size = m_HairAsset.m_NumTotalHairStrands * sizeof(int);
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBuffer(pvkDevice, &bufferDesc, nullptr, &m_HairStrandTypeBuffer));
		markerCallbacks.nameObject(m_HairStrandTypeBuffer, "HairStrandTypeBuffer");

        m_HairStrandTypeMemory = allocBufferMemory(pvkDevice, m_HairStrandTypeBuffer,
                                                   memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_HairStrandTypeMemory, "HairStrandTypeMemory");

        VkBufferViewCreateInfo SRVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        SRVDesc.format = VK_FORMAT_R32_SINT;
        SRVDesc.range = m_HairAsset.m_NumTotalHairStrands * sizeof(int32_t);
        SRVDesc.buffer = m_HairStrandTypeBuffer;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBufferView(pvkDevice, &SRVDesc, nullptr, &m_HairStrandTypeView));
		markerCallbacks.nameObject(m_HairStrandTypeView, "HairStrandTypeView");
        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pHairStrandType, m_HairStrandTypeBuffer,
                        offsetInUploadBuffer, bufferDesc.size);
    }

    //---------------------------------
    // m_InitialHairPositionsSRV
    // m_InitialHairPositionsUAV
    //---------------------------------
    {
        VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDesc.size = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDesc.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDesc, nullptr,
                                    &m_InitialHairPositionsBuffer));
		markerCallbacks.nameObject(m_InitialHairPositionsBuffer, "InitialHairPositionBuffer");
        m_InitialHairPositionsMemory = allocBufferMemory(
            pvkDevice, m_InitialHairPositionsBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_InitialHairPositionsMemory, "InitialHairPositionsMemory");

        VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        desc.range = m_HairAsset.m_NumTotalHairVertices * 4 * sizeof(float);
        desc.buffer = m_InitialHairPositionsBuffer;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBufferView(pvkDevice, &desc, nullptr, &m_InitialHairPositionsView));
		markerCallbacks.nameObject(m_InitialHairPositionsView, "InitialHairPositionsView");
        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pVertices, m_InitialHairPositionsBuffer,
                        offsetInUploadBuffer, bufferDesc.size);
    }

    // ------------------------
    // m_HairVertexPositionsUAB
    //-------------------------
    {
        VkBufferCreateInfo bufferDescUA{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDescUA.size = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT |
                             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                             VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDescUA, nullptr,
                                    &m_HairVertexPositionsBuffer));
		markerCallbacks.nameObject(m_HairVertexPositionsBuffer, "HairVertexPositionsBuffer");
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDescUA, nullptr,
                                    &m_HairVertexPositionsPrevBuffer));
		markerCallbacks.nameObject(m_HairVertexPositionsPrevBuffer, "HairVertexPositionsPrevBuffer");

        m_HairVertexPositionsMemory = allocBufferMemory(
            pvkDevice, m_HairVertexPositionsBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_HairVertexPositionsMemory, "HairVertexPositionsMemory");
        m_HairVertexPositionsPrevMemory = allocBufferMemory(
            pvkDevice, m_HairVertexPositionsPrevBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_HairVertexPositionsPrevMemory, "HairVertexPositionsPrevMemory");
        // TODO: We could write only once to uploadBuffer and share data between
        // the
        // 3 buffers
        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pVertices, m_HairVertexPositionsBuffer,
                        offsetInUploadBuffer, bufferDescUA.size);
        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pVertices, m_HairVertexPositionsPrevBuffer,
                        offsetInUploadBuffer, bufferDescUA.size);
    }

    // -----------------------------
    // m_HairVertexTangentsUAB
    //------------------------------
    {
        VkBufferCreateInfo bufferDescUA{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDescUA.size = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT |
                             VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
                             VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDescUA, nullptr,
                                    &m_HairVertexTangentsBuffer));
		markerCallbacks.nameObject(m_HairVertexTangentsBuffer, "HairVertexTangentsBuffer");
        m_HairVertexTangentsMemory = allocBufferMemory(
            pvkDevice, m_HairVertexTangentsBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_HairVertexTangentsMemory, "HairVertexTangentsMemory");
        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pTangents, m_HairVertexTangentsBuffer,
                        offsetInUploadBuffer, bufferDescUA.size);
    }

    //-----------------------
    // m_HairVertexPositionsView
    //-----------------------
    {
        VkBufferViewCreateInfo sbSRVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        sbSRVDesc.range = m_HairAsset.m_NumTotalHairVertices * 4 * sizeof(float);
        sbSRVDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        sbSRVDesc.buffer = m_HairVertexPositionsBuffer;
        AMD_CHECKED_VULKAN_CALL(vkCreateBufferView(pvkDevice, &sbSRVDesc, nullptr,
                                        &m_HairVertexPositionsView));
		markerCallbacks.nameObject(m_HairVertexPositionsView, "HairVertexPositionsView");
    }

    //-----------------------
    // m_HairVertexTangentsView
    //-----------------------
    {
        VkBufferViewCreateInfo sbSRVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        sbSRVDesc.range = m_HairAsset.m_NumTotalHairVertices * 4 * sizeof(float);
        sbSRVDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        sbSRVDesc.buffer = m_HairVertexTangentsBuffer;
        AMD_CHECKED_VULKAN_CALL(vkCreateBufferView(pvkDevice, &sbSRVDesc, nullptr,
                                        &m_HairVertexTangentsView));
		markerCallbacks.nameObject(m_HairVertexTangentsView, "HairVertexTangentsView");
    }

    //-----------------------
    // m_HairVertexPositionsView
    //-----------------------
    {
        VkBufferViewCreateInfo sbUAVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        sbUAVDesc.range = m_HairAsset.m_NumTotalHairVertices * 4 * sizeof(float);
        sbUAVDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        sbUAVDesc.buffer = m_HairVertexPositionsPrevBuffer;
        AMD_CHECKED_VULKAN_CALL(vkCreateBufferView(pvkDevice, &sbUAVDesc, nullptr,
                                        &m_HairVertexPositionsPrevView));
		markerCallbacks.nameObject(m_HairVertexPositionsPrevView, "HairVertexPositionsPrevView");
    }

    //---------------------------
    // m_HairRestLengthSRV
    //---------------------------
    {
        VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDesc.size = m_HairAsset.m_NumTotalHairVertices * sizeof(float);
        bufferDesc.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        //        AMD_SAFE_RELEASE(m_HairLengthBuffer);
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBuffer(pvkDevice, &bufferDesc, nullptr, &m_HairLengthBuffer));
		markerCallbacks.nameObject(m_HairLengthBuffer, "HairLengthBuffer");
        m_HairLengthMemory =
            allocBufferMemory(pvkDevice, m_HairLengthBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_HairLengthMemory, "HairLengthMemory");

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pRestLengths, m_HairLengthBuffer,
                        offsetInUploadBuffer, bufferDesc.size);

        VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        desc.format = VK_FORMAT_R32_SFLOAT;
        desc.range = m_HairAsset.m_NumTotalHairVertices * sizeof(float);
        desc.buffer = m_HairLengthBuffer;
        AMD_CHECKED_VULKAN_CALL(vkCreateBufferView(pvkDevice, &desc, nullptr, &m_HairRestLengthSRV));
		markerCallbacks.nameObject(m_HairRestLengthSRV, "HairRestLengthSRV");
    }

    //-----------------------------------
    // m_HairRefVecsInLocalFrameSRV
    //-----------------------------------
    {

        VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDesc.size = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDesc.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBuffer(pvkDevice, &bufferDesc, nullptr, &m_HairRefVectorsBuffer));
		markerCallbacks.nameObject(m_HairRefVectorsBuffer, "HairRefVectorsBuffer");
        m_HairRefVectorsMemory = allocBufferMemory(pvkDevice, m_HairRefVectorsBuffer,
                                                   memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_HairRefVectorsMemory, "HairRefVectorsMemory");

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pRefVectors, m_HairRefVectorsBuffer,
                        offsetInUploadBuffer, bufferDesc.size);

        VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        desc.range = m_HairAsset.m_NumTotalHairVertices * 4 * sizeof(float);
        desc.buffer = m_HairRefVectorsBuffer;
        AMD_CHECKED_VULKAN_CALL(vkCreateBufferView(pvkDevice, &desc, nullptr,
                                        &m_HairRefVecsInLocalFrameView));
		markerCallbacks.nameObject(m_HairRefVecsInLocalFrameView, "HairRefVecsInLocalFrameView");
    }

    //-----------------------------------
    // m_FollowHairRootOffsetSRV
    //-----------------------------------
    {
        VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDesc.size = m_HairAsset.m_NumTotalHairStrands * sizeof(XMFLOAT4);
        bufferDesc.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        //        AMD_SAFE_RELEASE(m_FollowHairRootOffsetBuffer);
        AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDesc, nullptr,
                                    &m_FollowHairRootOffsetBuffer));
		markerCallbacks.nameObject(m_FollowHairRootOffsetBuffer, "FollowHairRootOffsetBuffer");
        m_FollowHairRootOffsetMemory = allocBufferMemory(
            pvkDevice, m_FollowHairRootOffsetBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_FollowHairRootOffsetMemory, "FollowHaitRootOffsetMemory");

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pFollowRootOffset, m_FollowHairRootOffsetBuffer,
                        offsetInUploadBuffer, bufferDesc.size);

        VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        desc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        desc.range = m_HairAsset.m_NumTotalHairStrands * 4 * sizeof(float);
        desc.buffer = m_FollowHairRootOffsetBuffer;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBufferView(pvkDevice, &desc, nullptr, &m_FollowHairRootOffsetView));
		markerCallbacks.nameObject(m_FollowHairRootOffsetView, "FollowHairRootOffsetView");
    }

    // --------------------------------------------
    // m_GlobalRotationsUAB & m_GlobalRotationsUAV
    //---------------------------------------------
    {
        VkBufferCreateInfo bufferDescUA{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDescUA.size = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.usage =
            VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBuffer(pvkDevice, &bufferDescUA, nullptr, &m_GlobalRotationsBuffer));
		markerCallbacks.nameObject(m_GlobalRotationsBuffer, "GlobalRotationsBuffer");
        m_GlobalRotationsMemory = allocBufferMemory(pvkDevice, m_GlobalRotationsBuffer,
                                                    memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_GlobalRotationsMemory, "GlobalRotationsMemory");

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pGlobalRotations, m_GlobalRotationsBuffer,
                        offsetInUploadBuffer, bufferDescUA.size);

        VkBufferViewCreateInfo sbUAVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        sbUAVDesc.range = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        sbUAVDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        sbUAVDesc.buffer = m_GlobalRotationsBuffer;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBufferView(pvkDevice, &sbUAVDesc, nullptr, &m_GlobalRotationsView));
		markerCallbacks.nameObject(m_GlobalRotationsView, "GlobalRotationsView");
    }

    // --------------------------------------------
    // m_LocalRotationsUAB & m_LocalRotationsUAV
    //---------------------------------------------
    {
        VkBufferCreateInfo bufferDescUA{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
        bufferDescUA.size = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        bufferDescUA.usage =
            VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
        AMD_CHECKED_VULKAN_CALL(
            vkCreateBuffer(pvkDevice, &bufferDescUA, nullptr, &m_LocalRotationsBuffer));
		markerCallbacks.nameObject(m_LocalRotationsBuffer, "LocalROtationsBuffer");
        m_LocalRotationsMemory = allocBufferMemory(pvkDevice, m_LocalRotationsBuffer,
                                                   memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		markerCallbacks.nameObject(m_LocalRotationsMemory, "LocalRotationMemory");

        fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                        m_HairAsset.m_pLocalRotations, m_LocalRotationsBuffer,
                        offsetInUploadBuffer, bufferDescUA.size);

        VkBufferViewCreateInfo sbUAVDesc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
        sbUAVDesc.range = m_HairAsset.m_NumTotalHairVertices * sizeof(XMFLOAT4);
        sbUAVDesc.format = VK_FORMAT_R32G32B32A32_SFLOAT;
        sbUAVDesc.buffer = m_LocalRotationsBuffer;
        //        AMD_SAFE_RELEASE(m_LocalRotationsUAV);
        //        AMD_CHECKED_VULKAN_CALL(
        //            vkCreateBufferView(pvkDevice, &sbUAVDesc, nullptr,
        //            &m_LocalRotationsView));
    }

    if (m_HairAsset.m_pMapping != NULL)
    {
        // -----------------------------------------------
        // m_HairSkinMappingBuffer & m_HairSkinMappingSRV
        //------------------------------------------------
        {
            //            D3D11_SUBRESOURCE_DATA initialData;

            // "flatten" the mapping for one big vertex array
            for (int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++)
            {
                m_HairAsset.m_pMapping[i].triangle +=
                    sceneMesh->meshOffsets[m_HairAsset.m_pMapping[i].mesh];
            }

            //            initialData.pSysMem = m_HairAsset.m_pMapping;
            VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
            bufferDesc.size =
                m_HairAsset.m_NumTotalHairStrands * sizeof(HairToTriangleMapping);
            bufferDesc.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDesc, nullptr,
                                        &m_HairSkinMappingBuffer));
			markerCallbacks.nameObject(m_HairSkinMappingBuffer, "HairSkinMappingBuffer");

            VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
            desc.format = VK_FORMAT_UNDEFINED;
            desc.range =
                m_HairAsset.m_NumTotalHairStrands * sizeof(HairToTriangleMapping);
            desc.buffer = m_HairSkinMappingBuffer;
            AMD_CHECKED_VULKAN_CALL(
                vkCreateBufferView(pvkDevice, &desc, nullptr, &m_HairSkinMappingView));
			markerCallbacks.nameObject(m_HairSkinMappingView, "HairSkinMappingView");
        }

        // --------------------------------------------
        // m_HairTransformsBuffer & m_HairTransformsUAV
        //---------------------------------------------
        {
            TressFX_HairTransform *pTransforms =
                new TressFX_HairTransform[m_HairAsset.m_NumTotalHairStrands];
            for (int i = 0; i < m_HairAsset.m_NumTotalHairStrands; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    for (int k = 0; k < 4; k++)
                    {
                        if (j == k)
                        {
                            pTransforms[i].matrix[j][k] = 1.0f;
                        }
                        else
                        {
                            pTransforms[i].matrix[j][k] = 0.0f;
                        }
                    }
                    if (j == 3)
                    {
                        pTransforms[i].quaternion[j] = 1.0f;
                    }
                    else
                    {
                        pTransforms[i].quaternion[j] = 0.0f;
                    }
                }
            }

            VkBufferCreateInfo bufferDescUA{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
            bufferDescUA.size =
                m_HairAsset.m_NumTotalHairStrands * sizeof(TressFX_HairTransform);
            bufferDescUA.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
            AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDescUA, nullptr,
                                        &m_HairTransformsBuffer));
			markerCallbacks.nameObject(m_HairTransformsBuffer, "HairTransformsBuffer");

            VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
            desc.range =
                m_HairAsset.m_NumTotalHairStrands * sizeof(TressFX_HairTransform);
            desc.format = VK_FORMAT_UNDEFINED;
            desc.buffer = m_HairTransformsBuffer;
            AMD_CHECKED_VULKAN_CALL(
                vkCreateBufferView(pvkDevice, &desc, nullptr, &m_HairTransformsView));
			markerCallbacks.nameObject(m_HairTransformsView, "HairTransformView");

            AMD_SAFE_DELETE_ARRAY(pTransforms);
        }
    }

    if (m_HairAsset.m_pStrandTexCoords != NULL)
    {
        // -----------------------------------------------
        // m_pStrandTexCoordBuffer & m_pStrandTexCoordSRV
        //------------------------------------------------
        {
            VkBufferCreateInfo bufferDesc{VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO};
            bufferDesc.size = m_HairAsset.m_NumTotalHairStrands * sizeof(float) * 2;
            bufferDesc.usage = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT |
                               VK_BUFFER_USAGE_TRANSFER_DST_BIT;
            AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &bufferDesc, nullptr,
                                        &m_pStrandTexCoordBuffer));
			markerCallbacks.nameObject(m_pStrandTexCoordBuffer, "StrandTexCoordBuffer");
            m_pStrandTexCoordMemory = allocBufferMemory(
                pvkDevice, m_pStrandTexCoordBuffer, memProp, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
			markerCallbacks.nameObject(m_pStrandTexCoordMemory, "StrandTexCoordMemory");

            fillInitialData(upload_cmd_buffer, scratchBuffer, uploadBuffer,
                            m_HairAsset.m_pStrandTexCoords, m_pStrandTexCoordBuffer,
                            offsetInUploadBuffer, bufferDesc.size);

            VkBufferViewCreateInfo desc{VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO};
            desc.format = VK_FORMAT_R32G32_SFLOAT;
            desc.range = m_HairAsset.m_NumTotalHairStrands * sizeof(float) * 2;
            desc.buffer = m_pStrandTexCoordBuffer;
            //            AMD_SAFE_RELEASE(m_pStrandTexCoordSRV);
            AMD_CHECKED_VULKAN_CALL(
                vkCreateBufferView(pvkDevice, &desc, nullptr, &m_pStrandTexCoordView));
			markerCallbacks.nameObject(m_pStrandTexCoordView, "StrandTexCoordView");
        }
    }
    vkUnmapMemory(pvkDevice, scratchMemory);
    return VK_SUCCESS;
}

VkResult TressFXMesh::AllocateDescriptorsSets(
    VkDevice pvkDevice, VkDescriptorSetLayout GlobalConstraintsSetLayout,
    VkDescriptorSetLayout LocalConstraintsSetLayout,
    VkDescriptorSetLayout LenghtWindTangentSetLayout,
    VkDescriptorSetLayout PrepareFollowHairSetLayout,
    VkDescriptorSetLayout UpdateFollowHaitSetLayout,
    VkDescriptorSetLayout computeTangentSetLayout, VkDescriptorSetLayout Pass1SetLayout,
    VkDescriptorSetLayout ShadowSetLayout)
{
    VkDescriptorPoolSize poolSizes[] = {{VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 17},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 11}};
    VkDescriptorPoolCreateInfo info{VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO};
    info.maxSets = 8;
    info.poolSizeCount = AMD_ARRAY_SIZE(poolSizes);
    info.pPoolSizes = poolSizes;
    VkResult vr;
    AMD_CHECKED_VULKAN_CALL(vkCreateDescriptorPool(pvkDevice, &info, nullptr, &m_descriptorPool));

    const VkDescriptorSetLayout setLayouts[] = {GlobalConstraintsSetLayout,
                                                LocalConstraintsSetLayout,
                                                LenghtWindTangentSetLayout,
                                                PrepareFollowHairSetLayout,
                                                UpdateFollowHaitSetLayout,
                                                computeTangentSetLayout,
                                                Pass1SetLayout,
                                                ShadowSetLayout};
    VkDescriptorSetAllocateInfo allocateInfo{
        VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO};
    allocateInfo.descriptorPool = m_descriptorPool;
    allocateInfo.descriptorSetCount = AMD_ARRAY_SIZE(setLayouts);
    allocateInfo.pSetLayouts = setLayouts;

    VkDescriptorSet sets[AMD_ARRAY_SIZE(setLayouts)]{};

    AMD_CHECKED_VULKAN_CALL(vkAllocateDescriptorSets(pvkDevice, &allocateInfo, sets));
    m_GlobalConstraintsSet = sets[0];
    m_LocalConstraintsSet = sets[1];
    m_LenghtWindCollisionSet = sets[2];
    m_PrepareFollowHairSet = sets[3];
    m_UpdateFollowHairSet = sets[4];
    m_ComputeTangentSet = sets[5];
    m_pass1_set = sets[6];
    m_shadow_pass_set = sets[7];

    VkDescriptorBufferInfo positionInfo{m_HairVertexPositionsBuffer, 0,
                                        m_HairAsset.m_NumTotalHairVertices * 4 *
                                            sizeof(float)};
    VkDescriptorBufferInfo prevPositionInfo{m_HairVertexPositionsPrevBuffer, 0,
                                            m_HairAsset.m_NumTotalHairVertices * 4 *
                                                sizeof(float)};
    VkDescriptorBufferInfo tangentInfo{m_HairVertexTangentsBuffer, 0,
                                       m_HairAsset.m_NumTotalHairVertices * 4 *
                                           sizeof(float)};

    VkWriteDescriptorSet writeDescriptorSets[] = {
        getWriteDescriptor(m_GlobalConstraintsSet, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &positionInfo),
        getWriteDescriptor(m_GlobalConstraintsSet, IDSRV_HAIR_PREVIOUS_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &prevPositionInfo),
        getWriteDescriptor(m_GlobalConstraintsSet, IDSRV_HAIR_VERTEX_INITIAL_POSITIONS,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_InitialHairPositionsView),
        getWriteDescriptor(m_GlobalConstraintsSet, IDSRV_HAIR_STRAND_TYPE,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairStrandTypeView),

        getWriteDescriptor(m_LocalConstraintsSet, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &positionInfo),
        getWriteDescriptor(m_LocalConstraintsSet, IDSRV_HAIR_STRAND_TYPE,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairStrandTypeView),
        getWriteDescriptor(m_LocalConstraintsSet, IDSRV_HAIR_GLOBAL_ROTATION,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_GlobalRotationsView),
        getWriteDescriptor(m_LocalConstraintsSet, IDSRV_HAIR_LOCAL_REF_VEC,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairRefVecsInLocalFrameView),

        getWriteDescriptor(m_ComputeTangentSet, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairVertexPositionsView),
        getWriteDescriptor(m_ComputeTangentSet, IDSRV_HAIR_STRAND_TYPE,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairStrandTypeView),
        getWriteDescriptor(m_ComputeTangentSet, IDSRV_HAIR_TANGENTS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &tangentInfo),

        getWriteDescriptor(m_LenghtWindCollisionSet, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &positionInfo),
        getWriteDescriptor(m_LenghtWindCollisionSet, IDSRV_HAIR_STRAND_TYPE,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairStrandTypeView),
        getWriteDescriptor(m_LenghtWindCollisionSet, IDSRV_HAIR_LENGTH,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, &m_HairRestLengthSRV),

        getWriteDescriptor(m_PrepareFollowHairSet, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairVertexPositionsView),
        getWriteDescriptor(m_PrepareFollowHairSet, IDSRV_HAIR_PREVIOUS_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &prevPositionInfo),
        getWriteDescriptor(m_PrepareFollowHairSet, IDSRV_HAIR_STRAND_TYPE,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairStrandTypeView),

        getWriteDescriptor(m_UpdateFollowHairSet, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &positionInfo),
        getWriteDescriptor(m_UpdateFollowHairSet, IDSRV_HAIR_STRAND_TYPE,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairStrandTypeView),
        getWriteDescriptor(m_UpdateFollowHairSet, IDSRV_HAIR_ROOT_OFFSET,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_FollowHairRootOffsetView),

        getWriteDescriptor(m_pass1_set, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairVertexPositionsView),
        getWriteDescriptor(m_pass1_set, IDSRV_HAIR_TANGENTS,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairVertexTangentsView),
        getWriteDescriptor(m_pass1_set, IDSRV_HAIR_THICKNESSES,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_pThicknessCoeffsView),

        getWriteDescriptor(m_shadow_pass_set, IDSRV_HAIR_VERTEX_POSITIONS,
                           VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
                           &m_HairVertexPositionsView),
    };

    vkUpdateDescriptorSets(pvkDevice, AMD_ARRAY_SIZE(writeDescriptorSets),
                           writeDescriptorSets, 0, nullptr);

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when the D3D device is being destroyed.
//
//--------------------------------------------------------------------------------------
void TressFXMesh::OnDestroy()
{
    AMD_SAFE_RELEASE(m_pTriangleIndexBuffer, vkDestroyBuffer, m_pvkDevice);

    AMD_SAFE_RELEASE(m_pIndexBuffer, vkDestroyBuffer, m_pvkDevice);

    AMD_SAFE_RELEASE(m_pThicknessCoeffsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pThicknessCoeffsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pThicknessIndexTriangleIndexMemory, vkFreeMemory, m_pvkDevice);

    // compute shader variables
    AMD_SAFE_RELEASE(m_HairVertexPositionsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexPositionsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexPositionsMemory, vkFreeMemory, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexPositionsPrevView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexPositionsPrevBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexPositionsPrevMemory, vkFreeMemory, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexTangentsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexTangentsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairVertexTangentsMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_HairStrandTypeView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairStrandTypeBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairStrandTypeMemory, vkFreeMemory, m_pvkDevice);

    // vertex buffers
    AMD_SAFE_RELEASE(m_HairRestLengthSRV, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairLengthBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairLengthMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_InitialHairPositionsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_InitialHairPositionsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_InitialHairPositionsMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_HairRefVecsInLocalFrameView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairRefVectorsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairRefVectorsMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_FollowHairRootOffsetView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_FollowHairRootOffsetBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_FollowHairRootOffsetMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_GlobalRotationsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_GlobalRotationsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_GlobalRotationsMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_LocalRotationsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_LocalRotationsBuffer, vkDestroyBuffer, m_pvkDevice);
    AMD_SAFE_RELEASE(m_LocalRotationsMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_HairSkinMappingView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairSkinMappingBuffer, vkDestroyBuffer, m_pvkDevice);
    //    AMD_SAFE_RELEASE(m_HairSkinMappingMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_HairTransformsView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_HairTransformsBuffer, vkDestroyBuffer, m_pvkDevice);
    //    AMD_SAFE_RELEASE(m_HairTransformsMemory, vkFreeMemory, m_pvkDevice);

    AMD_SAFE_RELEASE(m_pStrandTexCoordView, vkDestroyBufferView, m_pvkDevice);
    AMD_SAFE_RELEASE(m_pStrandTexCoordBuffer, vkDestroyBuffer, m_pvkDevice);
    //    AMD_SAFE_RELEASE(m_pStrandTexCoordMemory, vkFreeMemory, m_pvkDevice);

    //    AMD_SAFE_RELEASE(m_pHairTextureSRV);
    AMD_SAFE_RELEASE(m_descriptorPool, vkDestroyDescriptorPool, m_pvkDevice);

    DestroyAsset();
}

//--------------------------------------------------------------------------------------
//
// Deserialize
//
// Converts the hair blob from a serialized stream to a TressFXAsset structure.
//
//--------------------------------------------------------------------------------------
bool TressFXMesh::Deserialize(TressFX_HairBlob *pHairBlob)
{
    string str;
    str.assign((char *)pHairBlob->pHair, pHairBlob->size);
    stringbuf buffer;
    buffer.str(str);

    buffer.sgetn((char *)&m_HairAsset.m_NumTotalHairVertices, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumTotalHairStrands, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumOfVerticesInStrand, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumGuideHairVertices, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumGuideHairStrands, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumFollowHairsPerGuideHair, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_TipSeparationFactor, sizeof(float));
    buffer.sgetn((char *)&m_HairAsset.m_NumPerStrandTexCoords, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumPerVertexColors, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumPerVertexTexCoords, sizeof(int));
    buffer.sgetn((char *)&m_HairAsset.m_NumHairToTriangleMappings, sizeof(int));

    m_HairAsset.m_pHairStrandType = new int[m_HairAsset.m_NumTotalHairStrands];
    buffer.sgetn((char *)m_HairAsset.m_pHairStrandType,
                 m_HairAsset.m_NumTotalHairStrands * sizeof(int));

    m_HairAsset.m_pRefVectors = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pRefVectors,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pGlobalRotations =
        new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pGlobalRotations,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pLocalRotations =
        new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pLocalRotations,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pVertices = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pVertices,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pTangents = new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pTangents,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));

    m_HairAsset.m_pTriangleVertices =
        new StrandVertex[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pTriangleVertices,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(StrandVertex));

    m_HairAsset.m_pThicknessCoeffs = new float[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pThicknessCoeffs,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(float));

    m_HairAsset.m_pFollowRootOffset =
        new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairStrands];
    buffer.sgetn((char *)m_HairAsset.m_pFollowRootOffset,
                 m_HairAsset.m_NumTotalHairStrands * sizeof(XMFLOAT4));

    m_HairAsset.m_pRestLengths = new float[m_HairAsset.m_NumTotalHairVertices];
    buffer.sgetn((char *)m_HairAsset.m_pRestLengths,
                 m_HairAsset.m_NumTotalHairVertices * sizeof(float));

    buffer.sgetn((char *)&m_HairAsset.m_bSphere, sizeof(BSphere));

    m_HairAsset.m_TriangleIndices.clear();

    int size;
    buffer.sgetn((char *)&size, sizeof(int));
    int *pIndices = new int[size];
    buffer.sgetn((char *)pIndices, size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        m_HairAsset.m_TriangleIndices.push_back(pIndices[i]);
    }
    AMD_SAFE_DELETE_ARRAY(pIndices);

    m_HairAsset.m_LineIndices.clear();
    buffer.sgetn((char *)&size, sizeof(int));
    pIndices = new int[size];
    buffer.sgetn((char *)pIndices, size * sizeof(int));
    for (int i = 0; i < size; i++)
    {
        m_HairAsset.m_LineIndices.push_back(pIndices[i]);
    }
    AMD_SAFE_DELETE_ARRAY(pIndices);

    if (m_HairAsset.m_NumPerStrandTexCoords)
    {
        m_HairAsset.m_pStrandTexCoords =
            new DirectX::XMFLOAT2[m_HairAsset.m_NumTotalHairStrands];
        buffer.sgetn((char *)m_HairAsset.m_pStrandTexCoords,
                     m_HairAsset.m_NumTotalHairStrands * sizeof(DirectX::XMFLOAT2));
    }
    else
    {
        m_HairAsset.m_pStrandTexCoords = NULL;
    }

    if (m_HairAsset.m_NumPerVertexColors)
    {
        m_HairAsset.m_pVertexColors =
            new DirectX::XMFLOAT4[m_HairAsset.m_NumTotalHairVertices];
        buffer.sgetn((char *)m_HairAsset.m_pVertexColors,
                     m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT4));
    }
    else
    {
        m_HairAsset.m_pVertexColors = NULL;
    }

    if (m_HairAsset.m_NumPerVertexTexCoords)
    {
        m_HairAsset.m_pVertexTexCoords =
            new DirectX::XMFLOAT2[m_HairAsset.m_NumTotalHairVertices];
        buffer.sgetn((char *)m_HairAsset.m_pVertexTexCoords,
                     m_HairAsset.m_NumTotalHairVertices * sizeof(DirectX::XMFLOAT2));
    }
    else
    {
        m_HairAsset.m_pVertexTexCoords = NULL;
    }

    if (m_HairAsset.m_NumHairToTriangleMappings > 0)
    {
        m_HairAsset.m_pMapping =
            new HairToTriangleMapping[m_HairAsset.m_NumTotalHairStrands];
        buffer.sgetn((char *)m_HairAsset.m_pMapping,
                     m_HairAsset.m_NumTotalHairStrands * sizeof(HairToTriangleMapping));
    }
    else
    {
        m_HairAsset.m_pMapping = NULL;
    }

    return true;
}

//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
void TressFXMesh::DestroyAsset(void)
{
    m_HairAsset.m_TriangleIndices.clear();
    m_HairAsset.m_LineIndices.clear();

    if (m_HairAsset.m_pVertices)
    {
        delete[] m_HairAsset.m_pVertices;
        m_HairAsset.m_pVertices = NULL;
    }

    if (m_HairAsset.m_pTangents)
    {
        delete[] m_HairAsset.m_pTangents;
        m_HairAsset.m_pTangents = NULL;
    }

    if (m_HairAsset.m_pStrandTexCoords)
    {
        delete[] m_HairAsset.m_pStrandTexCoords;
        m_HairAsset.m_pStrandTexCoords = NULL;
    }

    if (m_HairAsset.m_pHairStrandType)
    {
        delete[] m_HairAsset.m_pHairStrandType;
        m_HairAsset.m_pHairStrandType = NULL;
    }

    if (m_HairAsset.m_pLocalRotations)
    {
        delete[] m_HairAsset.m_pLocalRotations;
        m_HairAsset.m_pLocalRotations = NULL;
    }

    if (m_HairAsset.m_pGlobalRotations)
    {
        delete[] m_HairAsset.m_pGlobalRotations;
        m_HairAsset.m_pGlobalRotations = NULL;
    }

    if (m_HairAsset.m_pRefVectors)
    {
        delete[] m_HairAsset.m_pRefVectors;
        m_HairAsset.m_pRefVectors = NULL;
    }

    if (m_HairAsset.m_pTriangleVertices)
    {
        delete[] m_HairAsset.m_pTriangleVertices;
        m_HairAsset.m_pTriangleVertices = NULL;
    }

    if (m_HairAsset.m_pThicknessCoeffs)
    {
        delete[] m_HairAsset.m_pThicknessCoeffs;
        m_HairAsset.m_pThicknessCoeffs = NULL;
    }

    if (m_HairAsset.m_pRestLengths)
    {
        delete[] m_HairAsset.m_pRestLengths;
        m_HairAsset.m_pRestLengths = NULL;
    }

    if (m_HairAsset.m_pFollowRootOffset)
    {
        delete[] m_HairAsset.m_pFollowRootOffset;
        m_HairAsset.m_pFollowRootOffset = NULL;
    }

    if (m_HairAsset.m_pMapping)
    {
        delete[] m_HairAsset.m_pMapping;
        m_HairAsset.m_pMapping = NULL;
    }
}

} // namespace AMD
