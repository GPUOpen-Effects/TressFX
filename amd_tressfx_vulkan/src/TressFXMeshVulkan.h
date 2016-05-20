//--------------------------------------------------------------------------------------
// File: TressFXMesh.h
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

#pragma once
#include "TressFXAsset.h"
#include "Util.h"

namespace AMD
{

class TressFXMesh
{
  private:
    VkDevice m_pvkDevice;
    // private member function
    VkResult CreateBufferAndViews(VkDevice pvkDevice, TressFX_SceneMesh *sceneMesh,
                                  uint32_t texture_buffer_memory_index,
                                  VkCommandBuffer upload_cmd_buffer,
                                  VkBuffer scratchBuffer, VkDeviceMemory scratchMemory,
                                  size_t &offsetInUploadBuffer);

    VkResult AllocateDescriptorsSets(VkDevice pvkDevice,
                                     VkDescriptorSetLayout GlobalConstraintsSetLayout,
                                     VkDescriptorSetLayout LocalConstraintsSetLayout,
                                     VkDescriptorSetLayout LenghtWindTangentSetLayout,
                                     VkDescriptorSetLayout PrepareFollowHairSetLayout,
                                     VkDescriptorSetLayout UpdateFollowHaitSetLayout,
                                     VkDescriptorSetLayout computeTangentSetLayout,
                                     VkDescriptorSetLayout Pass1SetLayout,
                                     VkDescriptorSetLayout ShadowSetLayout);

    VkDescriptorPool m_descriptorPool;

  public:
    VkBuffer m_pIndexBuffer;
    VkDeviceMemory m_pIndexMemory;
    VkBuffer m_pTriangleIndexBuffer;
    VkDeviceMemory m_pTriangleIndexMemory;
    VkBuffer m_pThicknessCoeffsBuffer;
    VkDeviceMemory m_pThicknessCoeffsMemory;
    VkBufferView m_pThicknessCoeffsView;
    int m_TotalIndexCount;
    int m_TotalTriangleIndexCount;
    TressFXAsset m_HairAsset;

    // Simulation Descriptor Sets
    VkDescriptorSet m_GlobalConstraintsSet;
    VkDescriptorSet m_LocalConstraintsSet;
    VkDescriptorSet m_LenghtWindCollisionSet;
    VkDescriptorSet m_ComputeTangentSet;
    VkDescriptorSet m_PrepareFollowHairSet;
    VkDescriptorSet m_UpdateFollowHairSet;

    // Rendering Descriptor Sets
    VkDescriptorSet m_pass1_set;
    VkDescriptorSet m_shadow_pass_set;

    // vertex positions
    VkBuffer m_HairVertexPositionsBuffer;
    VkDeviceMemory m_HairVertexPositionsMemory;
    VkBufferView m_HairVertexPositionsView;

    VkBuffer m_HairVertexPositionsPrevBuffer;
    VkDeviceMemory m_HairVertexPositionsPrevMemory;
    VkBufferView m_HairVertexPositionsPrevView;

    VkBuffer m_InitialHairPositionsBuffer;
    VkDeviceMemory m_InitialHairPositionsMemory;
    VkBufferView m_InitialHairPositionsView;

    // strand type
    VkBuffer m_HairStrandTypeBuffer;
    VkDeviceMemory m_HairStrandTypeMemory;
    VkBufferView m_HairStrandTypeView;

    // rest length
    VkBuffer m_HairLengthBuffer;
    VkDeviceMemory m_HairLengthMemory;
    VkBufferView m_HairRestLengthSRV;

    // reference vectors
    VkBuffer m_HairRefVectorsBuffer;
    VkDeviceMemory m_HairRefVectorsMemory;
    VkBufferView m_HairRefVecsInLocalFrameView;

    // follow hair root offset
    VkBuffer m_FollowHairRootOffsetBuffer;
    VkDeviceMemory m_FollowHairRootOffsetMemory;
    VkBufferView m_FollowHairRootOffsetView;

    // global and local transforms for each vertex
    VkBuffer m_GlobalRotationsBuffer;
    VkDeviceMemory m_GlobalRotationsMemory;
    VkBufferView m_GlobalRotationsView;

    VkBuffer m_LocalRotationsBuffer;
    VkDeviceMemory m_LocalRotationsMemory;
    VkBufferView m_LocalRotationsView;

    // vertex tangents
    VkBuffer m_HairVertexTangentsBuffer;
    VkDeviceMemory m_HairVertexTangentsMemory;
    VkBufferView m_HairVertexTangentsView;

    // mapping from hair to skinned mesh
    VkBuffer m_HairSkinMappingBuffer;
    VkDeviceMemory m_HairSkinMappingMemory;
    VkBufferView m_HairSkinMappingView;

    // per-strand transformations for skinning
    VkBuffer m_HairTransformsBuffer;
    VkDeviceMemory m_HairTransformsMemory;
    VkBufferView m_HairTransformsView;

    // per-strand texture coordinates
    VkBuffer m_pStrandTexCoordBuffer;
    VkDeviceMemory m_pStrandTexCoordMemory;
    VkBufferView m_pStrandTexCoordView;

    // hair texture
    VkImageView m_pHairTextureSRV;

    // public member functions
    TressFXMesh(void);
    ~TressFXMesh(void);

    VkResult OnCreate(VkDevice pvkDevice, TressFX_HairBlob *pHairBlob,
                      TressFX_SceneMesh *sceneMesh, VkImageView pTexture,
                      uint32_t MemoryIndexGPU, VkCommandBuffer upload_cmd_buffer,
                      VkBuffer scratchBuffer, VkDeviceMemory scratchMemory,
                      VkDescriptorSetLayout GlobalConstraintsSetLayout,
                      VkDescriptorSetLayout LocalConstraintsSetLayout,
                      VkDescriptorSetLayout LenghtWindCollisiontSetLayout,
                      VkDescriptorSetLayout PrepareFollowHairSetLayout,
                      VkDescriptorSetLayout UpdateFollowHaitSetLayout,
                      VkDescriptorSetLayout ComputeTangetSetLayout,
                      VkDescriptorSetLayout Pass1SetLayout,
                      VkDescriptorSetLayout ShadowSetLayout);
    void OnDestroy();

    void DestroyAsset();

    bool Deserialize(TressFX_HairBlob *pHairBlob);
};

} // namespace AMD
