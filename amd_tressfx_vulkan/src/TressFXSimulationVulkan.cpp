//--------------------------------------------------------------------------------------
// File: TressFXSimulation.cpp
//
// Hair physics simulation using DirectCompute
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

#include "TressFXSimulationVulkan.h"
#include "AMD_Types.h"
#include "Math\\Transform.h"
#include "TressFXMeshVulkan.h"
#include "TressFXPrecompiledShadersVulkan.h"
#include "UtilVulkan.h"

#include <algorithm>

using namespace DirectX;

extern int g_TressFXNumVerticesPerStrand;

struct TransformConstantBuffer
{
    DirectX::XMMATRIX ModelTransformForHead;
};

struct ConstBufferCS_Per_Frame
{
    float4 Wind;
    float4 Wind1;
    float4 Wind2;
    float4 Wind3;

    int NumLengthConstraintIterations;
    int bCollision;

    float GravityMagnitude;
    float timeStep;

    float Damping0;
    float StiffnessForLocalShapeMatching0;
    float StiffnessForGlobalShapeMatching0;
    float GlobalShapeMatchingEffectiveRange0;

    float Damping1;
    float StiffnessForLocalShapeMatching1;
    float StiffnessForGlobalShapeMatching1;
    float GlobalShapeMatchingEffectiveRange1;

    float Damping2;
    float StiffnessForLocalShapeMatching2;
    float StiffnessForGlobalShapeMatching2;
    float GlobalShapeMatchingEffectiveRange2;

    float Damping3;
    float StiffnessForLocalShapeMatching3;
    float StiffnessForGlobalShapeMatching3;
    float GlobalShapeMatchingEffectiveRange3;

    unsigned int NumOfStrandsPerThreadGroup;
    unsigned int NumFollowHairsPerGuideHair;
    float TipSeparationFactor;

    int bWarp;
    int NumLocalShapeMatchingIterations;

    int NumVerticesPerStrand; // should be 2^n (n is integer and greater and 3) and less
                              // than or equal to THREAD_GROUP_SIZE. i.e. 8, 16, 32 or 64
    float pad[2];
};

struct ConstBufferCS_HeadTransform
{
    DirectX::XMMATRIX ModelTransformForHead;
    XMVECTOR ModelRotateForHead; // quaternion
    int bSingleHeadTransform;
    float padding[3];
};

namespace AMD
{
//--------------------------------------------------------------------------------------
//
// Constructor
//
//--------------------------------------------------------------------------------------
TressFXSimulation::TressFXSimulation(void)
{
    m_CSIntegrationAndGlobalShapeConstraints = NULL;
    m_CSApplyHairTransformGlobally = NULL;
    m_CSComputeTangents = NULL;
    m_CSLocalShapeConstraints = NULL;
    m_CSLengthConstriantsWindAndCollision = NULL;
    m_CSUpdateFollowHairVertices = NULL;
    m_CSPrepareFollowHairBeforeTurningIntoGuide = NULL;
    m_pCBCSPerFrame = NULL;
    m_pCBCSCollisionCapsule = NULL;
    m_pCBHeadTransforms = NULL;

    m_simParams.numLengthConstraintIterations = 2;
    m_simParams.numLocalShapeMatchingIterations = 1;
    m_simParams.gravityMagnitude = 9.82f;

    m_simParams.bCollision = true;
    m_simParams.bGuideFollowSimulation = true;
    m_bGuideFollowHairPrev = m_simParams.bGuideFollowSimulation;

    m_elapsedTimeSinceLastSim = 0;
}

//--------------------------------------------------------------------------------------
//
// Destructor
//
//--------------------------------------------------------------------------------------
TressFXSimulation::~TressFXSimulation(void) {}

namespace
{
VkComputePipelineCreateInfo getComputePipelineInfo(VkPipelineLayout layout,
    VkShaderModule computeShader)
{
    VkComputePipelineCreateInfo result{ VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO };
    result.layout = layout;
    result.stage.module = computeShader;
    result.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    result.stage.pName = "main";
    result.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    return result;
}
}

namespace
{
VkResult getPipelineLayout(VkDevice pvkDevice, VkDescriptorSetLayout configSetLayout,
    VkDescriptorSetLayout secondSetLayout,
    VkPipelineLayout &result)
{
    VkPipelineLayoutCreateInfo pipelineInfo{
        VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO };
    VkDescriptorSetLayout setLayouts[] = { configSetLayout, secondSetLayout };
    pipelineInfo.setLayoutCount = AMD_ARRAY_SIZE(setLayouts);
    pipelineInfo.pSetLayouts = setLayouts;
    VkResult vr;
    AMD_CHECKED_VULKAN_CALL(vkCreatePipelineLayout(pvkDevice, &pipelineInfo, nullptr, &result));
    return VK_SUCCESS;
}
}

VkResult TressFXSimulation::CreateDescriptorSet(VkDevice pvkDevice)
{
    VkResult vr;

    const VkDescriptorSetLayoutBinding global_constraints_bindings[] = {
        {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_PREVIOUS_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_VERTEX_INITIAL_POSITIONS, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_STRAND_TYPE, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
    };

    const VkDescriptorSetLayoutBinding local_constraints_bindings[] = {
        {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_STRAND_TYPE, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_GLOBAL_ROTATION, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_LOCAL_REF_VEC, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
    };

    const VkDescriptorSetLayoutBinding length_wind_collision_bindings[] = {
        {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_STRAND_TYPE, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_LENGTH, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
    };

    const VkDescriptorSetLayoutBinding prepare_follow_hair_bindings[] = {
        {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_PREVIOUS_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_STRAND_TYPE, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
    };

    const VkDescriptorSetLayoutBinding update_follow_hair_bindings[] = {
        {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_STRAND_TYPE, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_ROOT_OFFSET, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
    };

    const VkDescriptorSetLayoutBinding compute_tangent_bindings[] = {
        {IDSRV_HAIR_VERTEX_POSITIONS, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_STRAND_TYPE, VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HAIR_TANGENTS, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
    };

    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, global_constraints_bindings,
        AMD_ARRAY_SIZE(global_constraints_bindings),
        m_GlobalConstraintsSetLayout));
    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, local_constraints_bindings,
        AMD_ARRAY_SIZE(local_constraints_bindings),
        m_LocalConstraintsSetLayout));
    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, length_wind_collision_bindings,
        AMD_ARRAY_SIZE(length_wind_collision_bindings),
        m_LenghtWindTangentSetLayout));
    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, prepare_follow_hair_bindings,
        AMD_ARRAY_SIZE(prepare_follow_hair_bindings),
        m_PrepareFollowHairSetLayout));
    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, update_follow_hair_bindings,
        AMD_ARRAY_SIZE(update_follow_hair_bindings),
        m_UpdateFollowHaitSetLayout));
    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, compute_tangent_bindings,
        AMD_ARRAY_SIZE(compute_tangent_bindings),
        m_ComputeTangentSetLayout));

    const VkDescriptorSetLayoutBinding configBindings[] = {
        {IDSRV_CONSTANTS_BUFFER, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1,
            VK_SHADER_STAGE_COMPUTE_BIT},
        {IDSRV_HEAD_TRANSFORM, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1,
            VK_SHADER_STAGE_COMPUTE_BIT} };
    AMD_CHECKED_VULKAN_CALL(getDescriptorLayout(pvkDevice, configBindings,
        AMD_ARRAY_SIZE(configBindings), m_configSetLayout));

    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_GlobalConstraintsSetLayout,
        m_CSIntegrationAndGlobalShapeConstraintsLayout));
    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_LocalConstraintsSetLayout,
        m_CSLocalShapeConstraintsLayout));
    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_LocalConstraintsSetLayout,
        m_CSLocalShapeConstraintsSingleDispatchLayout));
    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_LenghtWindTangentSetLayout,
        m_CSLengthConstriantsWindAndCollisionLayout));
    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_PrepareFollowHairSetLayout,
        m_CSPrepareFollowHairBeforeTurningIntoGuideLayout));
    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_UpdateFollowHaitSetLayout,
        m_CSUpdateFollowHairVerticesLayout));
    AMD_CHECKED_VULKAN_CALL(getPipelineLayout(pvkDevice, m_configSetLayout,
        m_ComputeTangentSetLayout, m_CSComputeTangentsLayout));

    VkDescriptorPoolSize poolSizes[] = { {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1},
                                        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1} };
    VkDescriptorPoolCreateInfo descriptorPoolInfo{
        VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO };
    descriptorPoolInfo.maxSets = 1;
    descriptorPoolInfo.poolSizeCount = AMD_ARRAY_SIZE(poolSizes);
    descriptorPoolInfo.pPoolSizes = poolSizes;
    AMD_CHECKED_VULKAN_CALL(vkCreateDescriptorPool(pvkDevice, &descriptorPoolInfo, nullptr,
        &m_descriptorPool));

    VkDescriptorSetAllocateInfo allocInfo{ VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO };
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &m_configSetLayout;
    allocInfo.descriptorPool = m_descriptorPool;

    AMD_CHECKED_VULKAN_CALL(vkAllocateDescriptorSets(pvkDevice, &allocInfo, &m_configSet));

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// OnCreateDevice
//
// Called when the D3D device is being created. Creates the compute shaders and the D3D
// resources used for the hair simulation.
//
//--------------------------------------------------------------------------------------
VkResult TressFXSimulation::OnCreateDevice(VkDevice pvkDevice,
    TressFX_CollisionCapsule *pCollision,
    uint32_t maxUniformCount,
    VkPhysicalDeviceMemoryProperties memProperties, const DebugMarkerPointer& markerCallbacks)
{
    VkResult vr;
    AMD_CHECKED_VULKAN_CALL(CreateDescriptorSet(pvkDevice));
    // reset m_elapsedTimeSinceLastSim to zero
    m_elapsedTimeSinceLastSim = 0;

    ShaderModule IntegrationAndGlobalShapeConstraints_Module(pvkDevice,
        global_constraints);

    ShaderModule ApplyHairTransformGlobally_Module(pvkDevice, global_constraints);
    ShaderModule ComputeTangents_Module(pvkDevice, compute_tangents);
    ShaderModule LocalShapeConstraints_Module(pvkDevice, local_constraints);
    ShaderModule LocalShapeConstraintsWithIteration_Module(pvkDevice, local_constraints);
    ShaderModule LengthConstraintsWindAndCollision_Module(pvkDevice,
        length_wind_collision);
    ShaderModule UpdateFollowHairVertices_Module(pvkDevice, update_follow_hair);
    ShaderModule PrepareFollowHairBeforeTurningIntoGuide_Data(pvkDevice,
        prepare_follow_hair);

    ShaderModule GenerateTransforms_Module(pvkDevice, global_constraints);

    VkComputePipelineCreateInfo computePipelineInfo[] = {
        getComputePipelineInfo(
            m_CSIntegrationAndGlobalShapeConstraintsLayout,
            IntegrationAndGlobalShapeConstraints_Module.m_shaderModule),
        getComputePipelineInfo(m_CSIntegrationAndGlobalShapeConstraintsLayout,
                                ApplyHairTransformGlobally_Module.m_shaderModule),
        getComputePipelineInfo(m_CSComputeTangentsLayout,
                                ComputeTangents_Module.m_shaderModule),
        getComputePipelineInfo(m_CSLocalShapeConstraintsSingleDispatchLayout,
                                LocalShapeConstraints_Module.m_shaderModule),
        getComputePipelineInfo(m_CSLocalShapeConstraintsLayout,
                                LocalShapeConstraintsWithIteration_Module.m_shaderModule),
        getComputePipelineInfo(m_CSLengthConstriantsWindAndCollisionLayout,
                                LengthConstraintsWindAndCollision_Module.m_shaderModule),
        getComputePipelineInfo(m_CSUpdateFollowHairVerticesLayout,
                                UpdateFollowHairVertices_Module.m_shaderModule),
        getComputePipelineInfo(
            m_CSPrepareFollowHairBeforeTurningIntoGuideLayout,
            PrepareFollowHairBeforeTurningIntoGuide_Data.m_shaderModule),
        getComputePipelineInfo(m_CSIntegrationAndGlobalShapeConstraintsLayout,
                                GenerateTransforms_Module.m_shaderModule),
    };

    VkPipeline pipelines[9]{};
    AMD_CHECKED_VULKAN_CALL(vkCreateComputePipelines(pvkDevice, VK_NULL_HANDLE,
        AMD_ARRAY_SIZE(computePipelineInfo),
        computePipelineInfo, nullptr, pipelines));

    m_CSIntegrationAndGlobalShapeConstraints = pipelines[0];
    markerCallbacks.nameObject(m_CSIntegrationAndGlobalShapeConstraints, "CSIntegrationAndGlobalShapeConstraints");
    m_CSApplyHairTransformGlobally = pipelines[1];
    markerCallbacks.nameObject(m_CSApplyHairTransformGlobally, "CSApplyHairTransformGlobally");
    m_CSComputeTangents = pipelines[2];
    markerCallbacks.nameObject(m_CSComputeTangents, "CSComputeTangents");
    m_CSLocalShapeConstraints = pipelines[3];
    markerCallbacks.nameObject(m_CSLocalShapeConstraints, "CSLocalShapeConstraints");
    m_CSLocalShapeConstraintsSingleDispatch = pipelines[4];
    markerCallbacks.nameObject(m_CSLocalShapeConstraintsSingleDispatch, "CSLocalShapeConstraintsSingleDispatch");
    m_CSLengthConstriantsWindAndCollision = pipelines[5];
    markerCallbacks.nameObject(m_CSLengthConstriantsWindAndCollision, "CSLengthConstriantsWindAndCollision");
    m_CSUpdateFollowHairVertices = pipelines[6];
    markerCallbacks.nameObject(m_CSUpdateFollowHairVertices, "CSUpdateFollowHairVertices");
    m_CSPrepareFollowHairBeforeTurningIntoGuide = pipelines[7];
    markerCallbacks.nameObject(m_CSPrepareFollowHairBeforeTurningIntoGuide, "CSPrepareFollowHairBeforeTurningIntoGuide");
    m_CSGenerateTransforms = pipelines[8];
    markerCallbacks.nameObject(m_CSGenerateTransforms, "CSGenerateTransforms");

    //-------------------------
    // Create constant buffers
    //-------------------------
    AMD_CHECKED_VULKAN_CALL(CreateComputeShaderConstantBuffers(
        pvkDevice, pCollision, maxUniformCount, memProperties));

    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// GenerateTransforms
//
// Create an array of transformation matrices, one for each hair strand, and an associated
// quaternion for the rotation. The transformations generated are based on the position
// of the skinned (deformed) model that the hair strands are on. These transformations
// can be used for simulating the positions and orientations of fur strands on an
// animated mesh.
//
//--------------------------------------------------------------------------------------
// VkResult TressFXSimulation::GenerateTransforms(ID3D11DeviceContext* pd3dContext,
// TressFX_SceneMesh sceneMesh,
//                                              ID3D11UnorderedAccessView**
//                                              ppSkinningTransformationsUAV,
//                                              XMMATRIX *pModelTransformForHead)
//{
//    VkResult vr = VK_SUCCESS;
//
//    // update the constant buffers
//    D3D11_MAPPED_SUBRESOURCE MappedResource;
//
//    pd3dContext->Map(m_pCBCSPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
//    {
//        ConstBufferCS_Per_Frame* pCSPerFrame =
//        (ConstBufferCS_Per_Frame*)MappedResource.pData;
//
//        int numOfStrandsPerThreadGroup = THREAD_GROUP_SIZE /
//        m_pTressFXMesh->m_HairAsset.m_NumOfVerticesInStrand;
//        pCSPerFrame->NumOfStrandsPerThreadGroup = numOfStrandsPerThreadGroup;
//        pCSPerFrame->NumFollowHairsPerGuideHair = (m_bGuideFollowHairPrev ?
//        m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair : 0);
//    }
//    pd3dContext->Unmap(m_pCBCSPerFrame, 0);
//    pd3dContext->CSSetConstantBuffers(0, 1, &m_pCBCSPerFrame);
//
//    pd3dContext->Map(m_pCBGenerateTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0,
//    &MappedResource);
//    {
//        TransformConstantBuffer* pCSTransformCB =
//        (TransformConstantBuffer*)MappedResource.pData;
//        pCSTransformCB->ModelTransformForHead = *pModelTransformForHead;
//    }
//    pd3dContext->Unmap(m_pCBGenerateTransforms, 0);
//    pd3dContext->CSSetConstantBuffers(4, 1, &m_pCBGenerateTransforms);
//
//    //Set the shader resources
//    ID3D11ShaderResourceView* ppSRV[3] = {sceneMesh.pMeshVertices,
//    sceneMesh.pTransformedVerts, m_pTressFXMesh->m_HairSkinMappingSRV};
//
//    pd3dContext->CSSetShaderResources( 4, 3, ppSRV);
//
//    //Bind unordered access views
//    ID3D11UnorderedAccessView* ppUAV[4] = {m_pTressFXMesh->m_InitialHairPositionsUAV, 0,
//    0, m_pTressFXMesh->m_HairTransformsUAV};
//    pd3dContext->CSSetUnorderedAccessViews( 3, 4, ppUAV, NULL );
//
//    // execute the shader
//    int numOfGroupsForCS_StrandLevel = (m_bGuideFollowHairPrev ?
//    m_pTressFXMesh->m_HairAsset.m_NumGuideHairStrands :
//    m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands);
//
//    pd3dContext->CSSetShader(m_CSGenerateTransforms, NULL, 0 );
//    pd3dContext->Dispatch(numOfGroupsForCS_StrandLevel, 1, 1);
//
//    *ppSkinningTransformationsUAV = m_pTressFXMesh->m_HairTransformsUAV;
//
//    // unbind the resources
//    ppSRV [0] = NULL;
//    ppSRV [1] = NULL;
//    ppSRV [2] = NULL;
//    pd3dContext->CSSetShaderResources( 4, 3, ppSRV);
//
//    ID3D11UnorderedAccessView* ppUAVNULL[4] = {NULL, NULL, NULL, NULL};
//    pd3dContext->CSSetUnorderedAccessViews( 3, 4, ppUAVNULL, NULL );
//
//    return vr;
//}

//// Applies skin transforms to all hair so that hair would do rigid transform
// HRESULT TressFXSimulation::ApplyTransformGlobally(ID3D11DeviceContext* pd3dContext,
// ID3D11UnorderedAccessView* pSkinningTransforms, float density, bool
// singleHeadTransform, XMMATRIX *pModelTransformForHead)
//{
//    int numOfStrandsPerThreadGroup = THREAD_GROUP_SIZE /
//    m_pTressFXMesh->m_HairAsset.m_NumOfVerticesInStrand;
//
//    // ConstBufferCS_Per_Frame
//    D3D11_MAPPED_SUBRESOURCE MappedResource;
//
//    pd3dContext->Map(m_pCBCSPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
//    {
//        ConstBufferCS_Per_Frame* pCSPerFrame =
//        (ConstBufferCS_Per_Frame*)MappedResource.pData;
//        pCSPerFrame->NumOfStrandsPerThreadGroup = numOfStrandsPerThreadGroup;
//        pCSPerFrame->NumFollowHairsPerGuideHair = (m_bGuideFollowHairPrev ?
//        m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair : 0);
//        pCSPerFrame->TipSeparationFactor =
//        m_pTressFXMesh->m_HairAsset.m_TipSeparationFactor;
//        pCSPerFrame->NumVerticesPerStrand = g_TressFXNumVerticesPerStrand;
//    }
//    pd3dContext->Unmap(m_pCBCSPerFrame, 0);
//
//    // ConstBufferCS_HeadTransform
//    pd3dContext->Map(m_pCBHeadTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0,
//    &MappedResource);
//    {
//        ConstBufferCS_HeadTransform* pCSHeadTransform =
//        (ConstBufferCS_HeadTransform*)MappedResource.pData;
//        pCSHeadTransform->bSingleHeadTransform = singleHeadTransform;
//        pCSHeadTransform->ModelRotateForHead =
//        XMQuaternionRotationMatrix(*pModelTransformForHead);
//        pCSHeadTransform->ModelTransformForHead = *pModelTransformForHead;
//    }
//    pd3dContext->Unmap(m_pCBHeadTransforms, 0);
//
//    // set const buffers
//    pd3dContext->CSSetConstantBuffers(0, 1, &m_pCBCSPerFrame);
//    pd3dContext->CSSetConstantBuffers(5, 1, &m_pCBHeadTransforms);
//
//    //Set the shader resources
//    ID3D11ShaderResourceView* ppSRV[4] = { NULL, NULL, NULL,
//    m_pTressFXMesh->m_FollowHairRootOffsetSRV };
//    pd3dContext->CSSetShaderResources(0, 4, ppSRV);
//
//    //Bind unordered access views
//    ID3D11UnorderedAccessView* ppUAV[8] =
//    {
//        m_pTressFXMesh->m_HairVertexPositionsUAV,
//        m_pTressFXMesh->m_HairVertexPositionsPrevUAV,
//        m_pTressFXMesh->m_HairVertexTangentsUAV,
//        m_pTressFXMesh->m_InitialHairPositionsUAV,
//        m_pTressFXMesh->m_GlobalRotationsUAV,
//        m_pTressFXMesh->m_LocalRotationsUAV,
//        pSkinningTransforms
//    };
//
//    UINT initCounts = 0;
//    pd3dContext->CSSetUnorderedAccessViews(0, 8, ppUAV, &initCounts);
//
//    //======= Run the compute shader =======
//    int numOfGroupsForCS_VertexLevel = (int)(((float)(m_bGuideFollowHairPrev ?
//    m_pTressFXMesh->m_HairAsset.m_NumGuideHairVertices :
//		m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices) /
//(float)THREAD_GROUP_SIZE)*density);
//
//    // One thread computes one vertex
//    pd3dContext->CSSetShader(m_CSApplyHairTransformGlobally, NULL, 0);
//    pd3dContext->Dispatch(numOfGroupsForCS_VertexLevel, 1, 1);
//
//    // Update follow hair vertices
//    // One thread computes one vertex
//    if (m_bGuideFollowHairPrev)
//    {
//        pd3dContext->CSSetShader(m_CSUpdateFollowHairVertices, NULL, 0);
//        pd3dContext->Dispatch(numOfGroupsForCS_VertexLevel, 1, 1);
//    }
//
//	// Compute tangents for every vertex (guide + follow)
//	int numOfGroupsForCS_TotalVertexLevel =
//(int)(((float)(m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices) /
//(float)THREAD_GROUP_SIZE)*density);
//	pd3dContext->CSSetShader(m_CSComputeTangents, NULL, 0);
//	pd3dContext->Dispatch(numOfGroupsForCS_TotalVertexLevel, 1, 1);
//
//    // Unbind resources for CS
//    ID3D11UnorderedAccessView* ppUAViewNULL[8] = { NULL, NULL, NULL, NULL, NULL, NULL,
//    NULL, NULL };
//    pd3dContext->CSSetUnorderedAccessViews(0, 8, ppUAViewNULL, &initCounts);
//
//    ID3D11ShaderResourceView* ppSRVNULL[4] = { NULL, NULL, NULL, NULL };
//    pd3dContext->CSSetShaderResources(0, 4, ppSRVNULL);
//
//    return S_OK;
//}

const float MATH_PI2 = 3.14159265359f;
#define DEG_TO_RAD2(d) (d * MATH_PI2 / 180)

//--------------------------------------------------------------------------------------
//
// ComputeWindPyramid
//
// Wind noise is generated using blends of four vectors.  The four vectors are
// generated from some angle around winDir.  This code generates the four vectors.
// Shader code generates the per-strand noise from these vectors.
//
// This function currently has some hard coded constants, including a hard-coded
// sin function for magnitude based on frame count.  This will be changed in the next
// major update, but we maintain it in 3.1 for compatibility.
//
//--------------------------------------------------------------------------------------
void ComputeWindPyramid(float4 &wind, float4 &wind1, float4 &wind2, float4 &wind3,
    const float windMag, const tressfx_vec3 &windDir)
{
    static int frame = 0;

    float wM = windMag * (pow(sin(frame * 0.05f), 2.0f) + 0.5f);

    tressfx_vec3 windDirN(windDir);
    windDirN.Normalize();

    tressfx_vec3 XAxis(1.0f, 0, 0);
    tressfx_vec3 xCrossW = XAxis.Cross(windDirN);

    tressfx_quat rotFromXAxisToWindDir;
    rotFromXAxisToWindDir.SetIdentity();

    float angle = asin(xCrossW.Length());

    if (angle > 0.001)
    {
        rotFromXAxisToWindDir.SetRotation(xCrossW.Normalize(), angle);
    }

    float angleToWideWindCone = DEG_TO_RAD2(40.f);

    {
        tressfx_vec3 rotAxis(0, 1.0f, 0);

        tressfx_quat rot(rotAxis, angleToWideWindCone);
        tressfx_vec3 windVec = rotFromXAxisToWindDir * rot * XAxis * wM;
        wind = float4(windVec.x, windVec.y, windVec.z, (float)frame);
    }

    {
        tressfx_vec3 rotAxis(0, -1.0f, 0);
        tressfx_quat rot(rotAxis, angleToWideWindCone);
        tressfx_vec3 windVec = rotFromXAxisToWindDir * rot * XAxis * wM;
        wind1 = float4(windVec.x, windVec.y, windVec.z, (float)frame);
    }

    {
        tressfx_vec3 rotAxis(0, 0, 1.0f);
        tressfx_quat rot(rotAxis, angleToWideWindCone);
        tressfx_vec3 windVec = rotFromXAxisToWindDir * rot * XAxis * wM;
        wind2 = float4(windVec.x, windVec.y, windVec.z, (float)frame);
    }

    {
        tressfx_vec3 rotAxis(0, 0, -1.0f);
        tressfx_quat rot(rotAxis, angleToWideWindCone);
        tressfx_vec3 windVec = rotFromXAxisToWindDir * rot * XAxis * wM;
        wind3 = float4(windVec.x, windVec.y, windVec.z, (float)frame);
    }

    frame++;
}

// scale the stiffness value based on current and target frames rates and minimum
// stiffness value.
float getScaledStiffness(float s0, float s_min_scale, float h, float h0)
{
    float s_min = s0 * s_min_scale;
    float s = ((s0 - s_min) / h0) * h + s_min;
    return s;
}

//--------------------------------------------------------------------------------------
//
// Simulate
//
// Runs the hair simulation which will animate the hair vertices. This function calls
// Dispatch to execute compute shaders that implement the simulation.
//
////--------------------------------------------------------------------------------------
VkResult TressFXSimulation::Simulate(VkDevice pvkDevice, VkCommandBuffer commandBuffer,
    float fElapsedTime, float density,
    tressfx_vec3 &windDir, float windMag,
    XMMATRIX *pModelTransformForHead,
    float targetFrameRate, bool singleHeadTransform,
    bool warp, uint32_t uniformBufferIndex,
    const DebugMarkerPointer& markerCallbacks)
{
    m_elapsedTimeSinceLastSim += fElapsedTime;
    bool bFullSimulate = true;

    // Simulation is sensitive to frame rate. So, we set the target frame rate (defaulted
    // to 1/60)
    // and if the current elapsed time since the last simulation is too early,
    // we run the simulation with lower iterations and stiffness values.
    if ((m_elapsedTimeSinceLastSim < targetFrameRate) && !warp)
    {
        bFullSimulate = false;
    }
    else
    {
        m_elapsedTimeSinceLastSim = 0;
    }

    VkResult vr = VK_SUCCESS;

    int numOfStrandsPerThreadGroup =
        THREAD_GROUP_SIZE / m_pTressFXMesh->m_HairAsset.m_NumOfVerticesInStrand;

    ConstBufferCS_Per_Frame *pCSPerFrame;
    vkMapMemory(pvkDevice, m_pCBCSPerFrameMemory,
        uniformBufferIndex * sizeof(ConstBufferCS_Per_Frame),
        sizeof(ConstBufferCS_Per_Frame), 0,
        reinterpret_cast<void **>(&pCSPerFrame));

    fillConstantBuffer(pCSPerFrame, warp, bFullSimulate, targetFrameRate, numOfStrandsPerThreadGroup, windMag, windDir, fElapsedTime);
    vkUnmapMemory(pvkDevice, m_pCBCSPerFrameMemory);

    // ConstBufferCS_HeadTransform
    ConstBufferCS_HeadTransform *pCSHeadTransform;
    AMD_CHECKED_VULKAN_CALL(vkMapMemory(pvkDevice, m_pCBHeadTransformsMemory, 0,
        sizeof(ConstBufferCS_HeadTransform), 0,
        reinterpret_cast<void **>(&pCSHeadTransform)));
    fillHeadConstantBuffer(pCSHeadTransform, singleHeadTransform, pModelTransformForHead);
    vkUnmapMemory(pvkDevice, m_pCBHeadTransformsMemory);

    markerCallbacks.markBeginRegion(commandBuffer, "Simulation");
    fillSimulationCommands(commandBuffer, density, uniformBufferIndex);
    markerCallbacks.markEndRegion(commandBuffer);

    return vr;
}

void TressFXSimulation::fillSimulationCommands(const VkCommandBuffer &commandBuffer, float &density, const uint32_t &uniformBufferIndex)
{
    VkBufferMemoryBarrier bufferBarrier[] = {
        getBufferBarrier(m_pCBCSPerFrame, VK_ACCESS_HOST_WRITE_BIT,
        VK_ACCESS_UNIFORM_READ_BIT),
        getBufferBarrier(m_pCBHeadTransforms, VK_ACCESS_HOST_WRITE_BIT,
        VK_ACCESS_UNIFORM_READ_BIT),
    };

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_HOST_BIT,
        VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, 0, 0, nullptr,
        AMD_ARRAY_SIZE(bufferBarrier), bufferBarrier, 0, nullptr);

    //======= Run the compute shader =======

    // Increase density a little bit (0.05) to hide popping. This allows simulation to
    // take more hair than rendering and gives a chance
    // to simulate hairs before they get rendered so that there will be enough time for
    // popping to disappear.
    density += 0.05f;

    if (density > 1.0f)
    {
        density = 1.0f;
    }

    int numOfGroupsForCS_VertexLevel =
        (int)(((float)(m_bGuideFollowHairPrev
            ? m_pTressFXMesh->m_HairAsset.m_NumGuideHairVertices
            : m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices) /
            (float)THREAD_GROUP_SIZE) *
            density);

    uint32_t descriptorOffset[] = { uniformBufferIndex * sizeof(ConstBufferCS_Per_Frame) };

    // Prepare follow hair vertices before they are turning into guide ones.
    // One thread computes one vertex
    if (m_bGuideFollowHairPrev && !m_simParams.bGuideFollowSimulation)
    {
        VkDescriptorSet prepareFollowHairSets[] = {
            m_configSet, m_pTressFXMesh->m_PrepareFollowHairSet };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSPrepareFollowHairBeforeTurningIntoGuideLayout, 0,
            AMD_ARRAY_SIZE(prepareFollowHairSets),
            prepareFollowHairSets, AMD_ARRAY_SIZE(descriptorOffset),
            descriptorOffset);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSPrepareFollowHairBeforeTurningIntoGuide);
        vkCmdDispatch(commandBuffer, numOfGroupsForCS_VertexLevel, 1, 1);

        VkBufferMemoryBarrier flushPrevPositionBarrier[] = { getBufferBarrier(
            m_pTressFXMesh->m_HairVertexPositionsPrevBuffer, VK_ACCESS_MEMORY_WRITE_BIT,
            VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_MEMORY_READ_BIT) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
            AMD_ARRAY_SIZE(flushPrevPositionBarrier),
            flushPrevPositionBarrier, 0, nullptr);
    }

    // Integrate and global shape constraints
    // One thread computes one vertex
    VkDescriptorSet globalConstraintSets[] = { m_configSet,
        m_pTressFXMesh->m_GlobalConstraintsSet };

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
        m_CSIntegrationAndGlobalShapeConstraintsLayout, 0,
        AMD_ARRAY_SIZE(globalConstraintSets), globalConstraintSets,
        AMD_ARRAY_SIZE(descriptorOffset), descriptorOffset);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
        m_CSIntegrationAndGlobalShapeConstraints);
    vkCmdDispatch(commandBuffer, numOfGroupsForCS_VertexLevel, 1, 1);

    VkBufferMemoryBarrier globalConstraintsBarriers[] = {
        getBufferBarrier(m_pTressFXMesh->m_HairVertexPositionsBuffer,
        VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
        VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT),
        getBufferBarrier(m_pTressFXMesh->m_HairVertexPositionsPrevBuffer,
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
        VK_ACCESS_SHADER_READ_BIT) };

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
        AMD_ARRAY_SIZE(globalConstraintsBarriers),
        globalConstraintsBarriers, 0, nullptr);

    // Local shape constraints. If the hair is very curly, increase the iteration so that
    // hair style can be preserved well.
    // One thread computes one strand

    // If more than 16 vertices per strand, iterate on the CPU
    if (g_TressFXNumVerticesPerStrand >= 16)
    {
        for (int iteration = 0; iteration < m_simParams.numLocalShapeMatchingIterations;
            iteration++)
        {
            int numOfGroupsForCS_StrandLevel =
                (int)(((float)(m_bGuideFollowHairPrev
                    ? m_pTressFXMesh->m_HairAsset.m_NumGuideHairStrands
                    : m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands) /
                    (float)THREAD_GROUP_SIZE) *
                    density);
            VkDescriptorSet localConstraintSets[] = {
                m_configSet, m_pTressFXMesh->m_LocalConstraintsSet };
            vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                m_CSLocalShapeConstraintsSingleDispatchLayout, 0,
                AMD_ARRAY_SIZE(localConstraintSets),
                localConstraintSets, AMD_ARRAY_SIZE(descriptorOffset),
                descriptorOffset);
            vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
                m_CSLocalShapeConstraints);
            vkCmdDispatch(commandBuffer, numOfGroupsForCS_StrandLevel, 1, 1);

            VkBufferMemoryBarrier localBarrier[] = {
                getBufferBarrier(m_pTressFXMesh->m_HairVertexPositionsBuffer,
                VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
                VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT) };

            vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
                VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
                AMD_ARRAY_SIZE(localBarrier), localBarrier, 0, nullptr);
        }
    }
    else
    {
        int numOfGroupsForCS_StrandLevel =
            (int)(((float)(m_bGuideFollowHairPrev
                ? m_pTressFXMesh->m_HairAsset.m_NumGuideHairStrands
                : m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands) /
                (float)THREAD_GROUP_SIZE) *
                density);
        VkDescriptorSet localConstraintSets[] = { m_configSet,
            m_pTressFXMesh->m_LocalConstraintsSet };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSLocalShapeConstraintsSingleDispatchLayout, 0,
            AMD_ARRAY_SIZE(localConstraintSets), localConstraintSets,
            AMD_ARRAY_SIZE(descriptorOffset), descriptorOffset);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSLocalShapeConstraintsSingleDispatch);
        vkCmdDispatch(commandBuffer, numOfGroupsForCS_StrandLevel, 1, 1);

        VkBufferMemoryBarrier localBarrier[] = {
            getBufferBarrier(m_pTressFXMesh->m_HairVertexPositionsBuffer,
            VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
            VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
            AMD_ARRAY_SIZE(localBarrier), localBarrier, 0, nullptr);
    }

    // Edge length constraints, wind and collisions
    // One thread computes one vertex
    VkDescriptorSet lengthWindCollisionSets[] = {
        m_configSet, m_pTressFXMesh->m_LenghtWindCollisionSet };
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
        m_CSLengthConstriantsWindAndCollisionLayout, 0,
        AMD_ARRAY_SIZE(lengthWindCollisionSets),
        lengthWindCollisionSets, AMD_ARRAY_SIZE(descriptorOffset),
        descriptorOffset);
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
        m_CSLengthConstriantsWindAndCollision);
    vkCmdDispatch(commandBuffer, numOfGroupsForCS_VertexLevel, 1, 1);

    VkBufferMemoryBarrier lengthWindCollisionBarriers[] = {
        getBufferBarrier(m_pTressFXMesh->m_HairVertexPositionsBuffer,
        VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
        VK_ACCESS_MEMORY_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT) };

    vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
        AMD_ARRAY_SIZE(lengthWindCollisionBarriers),
        lengthWindCollisionBarriers, 0, nullptr);

    // Update follow hair vertices
    // One thread computes one vertex
    if (m_bGuideFollowHairPrev)
    {
        VkDescriptorSet updateFollowHairSets[] = { m_configSet,
            m_pTressFXMesh->m_UpdateFollowHairSet };
        vkCmdBindDescriptorSets(
            commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSUpdateFollowHairVerticesLayout, 0, AMD_ARRAY_SIZE(updateFollowHairSets),
            updateFollowHairSets, AMD_ARRAY_SIZE(descriptorOffset), descriptorOffset);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSUpdateFollowHairVertices);
        vkCmdDispatch(commandBuffer, numOfGroupsForCS_VertexLevel, 1, 1);

        VkBufferMemoryBarrier updateFollowHairBarriers[] = {
            getBufferBarrier(m_pTressFXMesh->m_HairVertexPositionsBuffer,
            VK_ACCESS_SHADER_WRITE_BIT | VK_ACCESS_SHADER_READ_BIT,
            VK_ACCESS_SHADER_READ_BIT),
            getBufferBarrier(
                m_pTressFXMesh->m_HairVertexTangentsBuffer, VK_ACCESS_MEMORY_READ_BIT,
                VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT)
        };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
            AMD_ARRAY_SIZE(updateFollowHairBarriers),
            updateFollowHairBarriers, 0, nullptr);
    }
    else
    {
        VkBufferMemoryBarrier makeTangentBarrier[] = { getBufferBarrier(
            m_pTressFXMesh->m_HairVertexTangentsBuffer, VK_ACCESS_MEMORY_READ_BIT,
            VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
            AMD_ARRAY_SIZE(makeTangentBarrier), makeTangentBarrier, 0,
            nullptr);
    }

    // Compute tangents for every vertex (guide + follow)
    {
        int numOfGroupsForCS_TotalVertexLevel =
            (int)(((float)(m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices) /
            (float)THREAD_GROUP_SIZE) *
                density);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSComputeTangents);
        VkDescriptorSet computeTangentSet[] = { m_configSet,
            m_pTressFXMesh->m_ComputeTangentSet };
        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_COMPUTE,
            m_CSComputeTangentsLayout, 0,
            AMD_ARRAY_SIZE(computeTangentSet), computeTangentSet,
            AMD_ARRAY_SIZE(descriptorOffset), descriptorOffset);
        vkCmdDispatch(commandBuffer, numOfGroupsForCS_TotalVertexLevel, 1, 1);

        VkBufferMemoryBarrier computeTangentBarrier[] = {
            getBufferBarrier(m_pTressFXMesh->m_HairVertexTangentsBuffer,
            VK_ACCESS_MEMORY_READ_BIT | VK_ACCESS_MEMORY_WRITE_BIT,
            VK_ACCESS_SHADER_READ_BIT) };

        vkCmdPipelineBarrier(commandBuffer, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
            VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, 0, 0, nullptr,
            AMD_ARRAY_SIZE(computeTangentBarrier), computeTangentBarrier,
            0, nullptr);
    }
}

void TressFXSimulation::fillHeadConstantBuffer(ConstBufferCS_HeadTransform * pCSHeadTransform, bool singleHeadTransform, DirectX::XMMATRIX * pModelTransformForHead)
{
    pCSHeadTransform->bSingleHeadTransform = singleHeadTransform;
    pCSHeadTransform->ModelRotateForHead =
        XMQuaternionRotationMatrix(*pModelTransformForHead);
    pCSHeadTransform->ModelTransformForHead = *pModelTransformForHead;
}

void TressFXSimulation::fillConstantBuffer(ConstBufferCS_Per_Frame * pCSPerFrame, bool warp, bool bFullSimulate, float targetFrameRate, int numOfStrandsPerThreadGroup, float windMag, AMD::tressfx_vec3 & windDir, float fElapsedTime)
{
    pCSPerFrame->bWarp = warp;

    if (bFullSimulate)
    {
        pCSPerFrame->NumLengthConstraintIterations =
            m_simParams.numLengthConstraintIterations;
    }
    else
    {
        pCSPerFrame->NumLengthConstraintIterations = 1;
    }

    pCSPerFrame->bCollision = (m_simParams.bCollision == true) ? 1 : 0;

    pCSPerFrame->GravityMagnitude = m_simParams.gravityMagnitude;

    pCSPerFrame->timeStep = targetFrameRate;

    pCSPerFrame->NumOfStrandsPerThreadGroup = numOfStrandsPerThreadGroup;
    pCSPerFrame->NumFollowHairsPerGuideHair =
        (m_bGuideFollowHairPrev
            ? m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair
            : 0);
    pCSPerFrame->TipSeparationFactor =
        m_pTressFXMesh->m_HairAsset.m_TipSeparationFactor;

    ComputeWindPyramid(pCSPerFrame->Wind, pCSPerFrame->Wind1, pCSPerFrame->Wind2,
        pCSPerFrame->Wind3, windMag, windDir);

    int numSections = m_simParams.numHairSections;

    switch (numSections)
    {
        // hair section 3
    case 4:
        pCSPerFrame->Damping3 = m_simParams.perSectionShapeParams[3].damping;
        pCSPerFrame->StiffnessForLocalShapeMatching3 =
            m_simParams.perSectionShapeParams[3].stiffnessForLocalShapeMatching;
        pCSPerFrame->StiffnessForGlobalShapeMatching3 =
            m_simParams.perSectionShapeParams[3].stiffnessForGlobalShapeMatching;
        pCSPerFrame->GlobalShapeMatchingEffectiveRange3 =
            m_simParams.perSectionShapeParams[3].globalShapeMatchingEffectiveRange;
        // hair section 2
    case 3:
        pCSPerFrame->Damping2 = m_simParams.perSectionShapeParams[2].damping;
        pCSPerFrame->StiffnessForLocalShapeMatching2 =
            m_simParams.perSectionShapeParams[2].stiffnessForLocalShapeMatching;
        pCSPerFrame->StiffnessForGlobalShapeMatching2 =
            m_simParams.perSectionShapeParams[2].stiffnessForGlobalShapeMatching;
        pCSPerFrame->GlobalShapeMatchingEffectiveRange2 =
            m_simParams.perSectionShapeParams[2].globalShapeMatchingEffectiveRange;
        // hair section 1
    case 2:
        pCSPerFrame->Damping1 = m_simParams.perSectionShapeParams[1].damping;
        pCSPerFrame->StiffnessForLocalShapeMatching1 =
            m_simParams.perSectionShapeParams[1].stiffnessForLocalShapeMatching;
        pCSPerFrame->StiffnessForGlobalShapeMatching1 =
            m_simParams.perSectionShapeParams[1].stiffnessForGlobalShapeMatching;
        pCSPerFrame->GlobalShapeMatchingEffectiveRange1 =
            m_simParams.perSectionShapeParams[1].globalShapeMatchingEffectiveRange;
        // hair section 0
    case 1:
        pCSPerFrame->Damping0 = m_simParams.perSectionShapeParams[0].damping;
        pCSPerFrame->StiffnessForLocalShapeMatching0 =
            m_simParams.perSectionShapeParams[0].stiffnessForLocalShapeMatching;
        pCSPerFrame->StiffnessForGlobalShapeMatching0 =
            m_simParams.perSectionShapeParams[0].stiffnessForGlobalShapeMatching;
        pCSPerFrame->GlobalShapeMatchingEffectiveRange0 =
            m_simParams.perSectionShapeParams[0].globalShapeMatchingEffectiveRange;
    }

    if (!bFullSimulate)
    {
        float h = fElapsedTime;
        float h0 = targetFrameRate;
        float s_min_scale =
            0.3f; // minimum stiffness = s_min_scale * current stiffness

        pCSPerFrame->StiffnessForLocalShapeMatching0 = getScaledStiffness(
            m_simParams.perSectionShapeParams[0].stiffnessForLocalShapeMatching,
            s_min_scale, h, h0);
        pCSPerFrame->StiffnessForLocalShapeMatching1 = getScaledStiffness(
            m_simParams.perSectionShapeParams[1].stiffnessForLocalShapeMatching,
            s_min_scale, h, h0);
        pCSPerFrame->StiffnessForLocalShapeMatching2 = getScaledStiffness(
            m_simParams.perSectionShapeParams[2].stiffnessForLocalShapeMatching,
            s_min_scale, h, h0);
        pCSPerFrame->StiffnessForLocalShapeMatching3 = getScaledStiffness(
            m_simParams.perSectionShapeParams[3].stiffnessForLocalShapeMatching,
            s_min_scale, h, h0);
    }

    if (bFullSimulate)
    {
        pCSPerFrame->NumLocalShapeMatchingIterations =
            m_simParams.numLocalShapeMatchingIterations;
    }
    else
    {
        pCSPerFrame->NumLocalShapeMatchingIterations = 1;
    }

    pCSPerFrame->NumVerticesPerStrand = g_TressFXNumVerticesPerStrand;
}

//--------------------------------------------------------------------------------------
//
// CreateComputeShaderConstantBuffers
//
// Called by OnCreateDevice to allocate a constant buffer used by the compute shader code
//
//--------------------------------------------------------------------------------------
VkResult TressFXSimulation::CreateComputeShaderConstantBuffers(
    VkDevice pvkDevice, TressFX_CollisionCapsule *pCollision,
    uint32_t maxUniformBufferCount, VkPhysicalDeviceMemoryProperties memProperties)
{
    (void)pCollision;
    VkResult vr = VK_SUCCESS;
    // const buffer for per frame data
    VkBufferCreateInfo Desc{ VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO };
    // FIXME: Uniform buffer + compute shader crash nvidia driver
    Desc.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
    Desc.size = maxUniformBufferCount * sizeof(ConstBufferCS_Per_Frame);
    AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &Desc, nullptr, &m_pCBCSPerFrame));
    m_pCBCSPerFrameMemory =
        allocBufferMemory(pvkDevice, m_pCBCSPerFrame, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // const buffer for capsule collision
    Desc.size = sizeof(TressFX_CollisionCapsule);
    // data.pSysMem = (void *)pCollision;
    AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &Desc, nullptr, &m_pCBCSCollisionCapsule));
    m_pCBCSCollisionCapsuleMemory =
        allocBufferMemory(pvkDevice, m_pCBCSCollisionCapsule, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    /*    void* scratch_buffer_mapped_memory;
        vkMapMemory(pvkDevice, ..., 0, sizeof(TressFX_CollisionCapsule), 0,
        &scratch_buffer_mapped_memory);
        memcpy(scratch_buffer_mapped_memory, pCollision,
        sizeof(TressFX_CollisionCapsule));
        vkUnmapMemory(pvkDevice, ...);
        vkCmdCopyBuffer(..., ..., m_pCBCSCollisionCapsule, 1, );*/

        // const buffer for hair root transformation
    Desc.size = sizeof(TransformConstantBuffer);
    AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &Desc, nullptr, &m_pCBGenerateTransforms));
    m_pCBGenerateTransformsMemory =
        allocBufferMemory(pvkDevice, m_pCBGenerateTransforms, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    // const buffer for head transformation
    Desc.size = sizeof(ConstBufferCS_HeadTransform);
    AMD_CHECKED_VULKAN_CALL(vkCreateBuffer(pvkDevice, &Desc, nullptr, &m_pCBHeadTransforms));
    m_pCBHeadTransformsMemory =
        allocBufferMemory(pvkDevice, m_pCBHeadTransforms, memProperties, VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    VkDescriptorBufferInfo perFrameCBCS{ m_pCBCSPerFrame, 0,
                                        sizeof(ConstBufferCS_Per_Frame) };
    VkDescriptorBufferInfo headTransform{ m_pCBHeadTransforms, 0,
                                            sizeof(ConstBufferCS_HeadTransform) };
    VkWriteDescriptorSet writeSet[] = {
        getWriteDescriptor(m_configSet, IDSRV_CONSTANTS_BUFFER,
                            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, &perFrameCBCS),
        getWriteDescriptor(m_configSet, IDSRV_HEAD_TRANSFORM,
                            VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, &headTransform),
    };
    vkUpdateDescriptorSets(pvkDevice, AMD_ARRAY_SIZE(writeSet), writeSet, 0, nullptr);
    return VK_SUCCESS;
}

//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when the D3D device is being destroyed
//
//--------------------------------------------------------------------------------------
void TressFXSimulation::OnDestroy(VkDevice pvkDevice)
{
    {
        AMD_SAFE_RELEASE(m_CSIntegrationAndGlobalShapeConstraints, vkDestroyPipeline,
            pvkDevice);
        AMD_SAFE_RELEASE(m_CSApplyHairTransformGlobally, vkDestroyPipeline, pvkDevice);
        AMD_SAFE_RELEASE(m_CSComputeTangents, vkDestroyPipeline, pvkDevice);
        AMD_SAFE_RELEASE(m_CSLocalShapeConstraints, vkDestroyPipeline, pvkDevice);
        AMD_SAFE_RELEASE(m_CSLocalShapeConstraintsSingleDispatch, vkDestroyPipeline,
            pvkDevice);
        AMD_SAFE_RELEASE(m_CSLengthConstriantsWindAndCollision, vkDestroyPipeline,
            pvkDevice);
        AMD_SAFE_RELEASE(m_CSUpdateFollowHairVertices, vkDestroyPipeline, pvkDevice);
        AMD_SAFE_RELEASE(m_CSPrepareFollowHairBeforeTurningIntoGuide, vkDestroyPipeline,
            pvkDevice);
        AMD_SAFE_RELEASE(m_CSGenerateTransforms, vkDestroyPipeline, pvkDevice);

        AMD_SAFE_RELEASE(m_CSIntegrationAndGlobalShapeConstraintsLayout,
            vkDestroyPipelineLayout, pvkDevice);
        // AMD_SAFE_RELEASE(m_CSApplyHairTransformGloballyLayout, vkDestroyPipelineLayout,
        // pvkDevice);
        AMD_SAFE_RELEASE(m_CSComputeTangentsLayout, vkDestroyPipelineLayout, pvkDevice);
        AMD_SAFE_RELEASE(m_CSLocalShapeConstraintsLayout, vkDestroyPipelineLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_CSLocalShapeConstraintsSingleDispatchLayout,
            vkDestroyPipelineLayout, pvkDevice);
        AMD_SAFE_RELEASE(m_CSLengthConstriantsWindAndCollisionLayout,
            vkDestroyPipelineLayout, pvkDevice);
        AMD_SAFE_RELEASE(m_CSUpdateFollowHairVerticesLayout, vkDestroyPipelineLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_CSPrepareFollowHairBeforeTurningIntoGuideLayout,
            vkDestroyPipelineLayout, pvkDevice);
        // AMD_SAFE_RELEASE(m_CSGenerateTransformsLayout, vkDestroyPipelineLayout,
        // pvkDevice);

        AMD_SAFE_RELEASE(m_GlobalConstraintsSetLayout, vkDestroyDescriptorSetLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_LocalConstraintsSetLayout, vkDestroyDescriptorSetLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_LenghtWindTangentSetLayout, vkDestroyDescriptorSetLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_PrepareFollowHairSetLayout, vkDestroyDescriptorSetLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_UpdateFollowHaitSetLayout, vkDestroyDescriptorSetLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_ComputeTangentSetLayout, vkDestroyDescriptorSetLayout,
            pvkDevice);
        AMD_SAFE_RELEASE(m_configSetLayout, vkDestroyDescriptorSetLayout, pvkDevice);
    }
    AMD_SAFE_RELEASE(m_descriptorPool, vkDestroyDescriptorPool, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBCSPerFrame, vkDestroyBuffer, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBCSPerFrameMemory, vkFreeMemory, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBCSCollisionCapsule, vkDestroyBuffer, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBCSCollisionCapsuleMemory, vkFreeMemory, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBGenerateTransforms, vkDestroyBuffer, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBGenerateTransformsMemory, vkFreeMemory, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBHeadTransforms, vkDestroyBuffer, pvkDevice);
    AMD_SAFE_RELEASE(m_pCBHeadTransformsMemory, vkFreeMemory, pvkDevice);
}
} // namespace AMD
