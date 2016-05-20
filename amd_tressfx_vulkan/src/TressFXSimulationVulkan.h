//--------------------------------------------------------------------------------------
// File: TressFXSimulation.h
//
// Hair physics simulation header
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
#include "Math\vector3D.h"
#include "TressFXMeshVulkan.h"
#include "Util.h"
#include <map>
#include <string>

typedef DirectX::XMFLOAT4 float4;
typedef DirectX::XMFLOAT3 float3;

namespace AMD
{

class TressFXSimulation
{
  public:
    TressFXSimulation(void);
    ~TressFXSimulation(void);

    VkResult OnCreateDevice(VkDevice pvkDevice, TressFX_CollisionCapsule *pCollision,
                            uint32_t maxUniformCount, uint32_t cpu_memory_index,
                            uint32_t gpu_memory_index);
    VkResult Simulate(VkDevice pvkContext, VkCommandBuffer commandBuffer,
                      float fElapsedTime, float density, tressfx_vec3 &windDir,
                      float windMag, DirectX::XMMATRIX *pModelTransformForHead,
                      ID3D11UnorderedAccessView *pSkinningTransforms,
                      float targetFrameRate, bool singleHeadTransform, bool warp,
                      uint32_t uniformBufferIndex);
    VkResult GenerateTransforms(VkDevice pvkContext, TressFX_SceneMesh sceneMesh,
                                ID3D11UnorderedAccessView **ppSkinningTransformationsUAV,
                                DirectX::XMMATRIX *pModelTransformForHead);
    VkResult ApplyTransformGlobally(VkDevice pvkContext,
                                    ID3D11UnorderedAccessView *pSkinningTransforms,
                                    float density, bool singleHeadTransform,
                                    DirectX::XMMATRIX *pModelTransformForHead);
    void OnDestroy(VkDevice pvkDevice);
    TressFXMesh *m_pTressFXMesh;
    float m_elapsedTimeSinceLastSim;

    // Descriptors set layout
    VkDescriptorSetLayout m_GlobalConstraintsSetLayout;
    VkDescriptorSetLayout m_LocalConstraintsSetLayout;
    VkDescriptorSetLayout m_LenghtWindTangentSetLayout;
    VkDescriptorSetLayout m_PrepareFollowHairSetLayout;
    VkDescriptorSetLayout m_UpdateFollowHaitSetLayout;
    VkDescriptorSetLayout m_ComputeTangentSetLayout;

  private:
    bool m_bGuideFollowHairPrev;

    // hair simulation params
    TressFX_SimulationParams m_simParams;

    VkDescriptorPool m_descriptorPool;
    VkDescriptorSetLayout m_configSetLayout;
    VkDescriptorSet m_configSet;

    // Compute Shader
    // data for compute shaders
    VkPipelineLayout m_CSIntegrationAndGlobalShapeConstraintsLayout;
    VkPipeline m_CSIntegrationAndGlobalShapeConstraints;
    VkPipelineLayout m_CSApplyHairTransformGloballyLayout;
    VkPipeline m_CSApplyHairTransformGlobally;
    VkPipelineLayout m_CSComputeTangentsLayout;
    VkPipeline m_CSComputeTangents;
    VkPipelineLayout m_CSLocalShapeConstraintsLayout;
    VkPipeline m_CSLocalShapeConstraints;
    VkPipelineLayout m_CSLocalShapeConstraintsSingleDispatchLayout;
    VkPipeline m_CSLocalShapeConstraintsSingleDispatch;
    VkPipelineLayout m_CSLengthConstriantsWindAndCollisionLayout;
    VkPipeline m_CSLengthConstriantsWindAndCollision;
    VkPipelineLayout m_CSUpdateFollowHairVerticesLayout;
    VkPipeline m_CSUpdateFollowHairVertices;
    VkPipelineLayout m_CSPrepareFollowHairBeforeTurningIntoGuideLayout;
    VkPipeline m_CSPrepareFollowHairBeforeTurningIntoGuide;
    VkPipelineLayout m_CSGenerateTransformsLayout;
    VkPipeline m_CSGenerateTransforms;

    // const buffers for CS
    VkBuffer m_pCBCSPerFrame;
    VkDeviceMemory m_pCBCSPerFrameMemory;
    VkBuffer m_pCBCSCollisionCapsule;
    VkDeviceMemory m_pCBCSCollisionCapsuleMemory;
    VkBuffer m_pCBGenerateTransforms;
    VkDeviceMemory m_pCBGenerateTransformsMemory;
    VkBuffer m_pCBHeadTransforms;
    VkDeviceMemory m_pCBHeadTransformsMemory;

    VkResult CreateComputeShaderConstantBuffers(VkDevice pvkDevice,
                                                TressFX_CollisionCapsule *pCollision,
                                                uint32_t maxUniformBufferCount,
                                                uint32_t memoryIndexCPU,
                                                uint32_t memoryIndexGPU);
    VkResult CreateDescriptorSet(VkDevice pvkDevice);

  public:
    void SetSimulationParams(const TressFX_SimulationParams &simParams)
    {
        m_simParams = simParams;
    }
};

} // namespace AMD
