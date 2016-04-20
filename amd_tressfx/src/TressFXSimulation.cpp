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

#include "AMD_Types.h"
#include "TressFXSimulation.h"
#include "TressFXMesh.h"
#include "TressFXPrecompiledShaders.h"
#include "Math\\Transform.h"

#include <algorithm>

#ifndef AMD_V_RETURN
#define AMD_V_RETURN(x)    { hr = (x); if ( FAILED(hr) ) { return hr; } }
#endif

using namespace DirectX;

extern int g_TressFXNumVerticesPerStrand;

struct TransformConstantBuffer
{
    XMMATRIX ModelTransformForHead;
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

    int NumVerticesPerStrand; // should be 2^n (n is integer and greater and 3) and less than or equal to THREAD_GROUP_SIZE. i.e. 8, 16, 32 or 64
    float pad[2];
};

struct ConstBufferCS_HeadTransform
{
    XMMATRIX ModelTransformForHead;
    XMMATRIX InvModelTransformForHead;
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
TressFXSimulation::~TressFXSimulation(void)
{
}

//--------------------------------------------------------------------------------------
//
// OnCreateDevice
//
// Called when the D3D device is being created. Creates the compute shaders and the D3D
// resources used for the hair simulation.
//
//--------------------------------------------------------------------------------------
HRESULT TressFXSimulation::OnCreateDevice(ID3D11Device* pd3dDevice, TressFX_CollisionCapsule *pCollision)
{
    HRESULT hr;

    // reset m_elapsedTimeSinceLastSim to zero
    m_elapsedTimeSinceLastSim = 0;

    AMD_V_RETURN(pd3dDevice->CreateComputeShader(IntegrationAndGlobalShapeConstraints_Data, sizeof(IntegrationAndGlobalShapeConstraints_Data),
        NULL, &m_CSIntegrationAndGlobalShapeConstraints));
    AMD_V_RETURN(pd3dDevice->CreateComputeShader(LocalShapeConstraints_Data, sizeof(LocalShapeConstraints_Data),
        NULL, &m_CSLocalShapeConstraints));
    AMD_V_RETURN(pd3dDevice->CreateComputeShader(LocalShapeConstraintsWithIteration_Data, sizeof(LocalShapeConstraintsWithIteration_Data),
        NULL, &m_CSLocalShapeConstraintsSingleDispatch));
    AMD_V_RETURN(pd3dDevice->CreateComputeShader(LengthConstriantsWindAndCollision_Data, sizeof(LengthConstriantsWindAndCollision_Data),
        NULL, &m_CSLengthConstriantsWindAndCollision));
    AMD_V_RETURN(pd3dDevice->CreateComputeShader(UpdateFollowHairVertices_Data, sizeof(UpdateFollowHairVertices_Data),
        NULL, &m_CSUpdateFollowHairVertices));
    AMD_V_RETURN(pd3dDevice->CreateComputeShader(PrepareFollowHairBeforeTurningIntoGuide_Data, sizeof(PrepareFollowHairBeforeTurningIntoGuide_Data),
        NULL, &m_CSPrepareFollowHairBeforeTurningIntoGuide));
    AMD_V_RETURN(pd3dDevice->CreateComputeShader(GenerateTransforms_Data, sizeof(GenerateTransforms_Data),
        NULL, &m_CSGenerateTransforms));

    //-------------------------
    // Create constant buffers
    //-------------------------
    AMD_V_RETURN(CreateComputeShaderConstantBuffers(pd3dDevice, pCollision));

    return S_OK;
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
HRESULT TressFXSimulation::GenerateTransforms(ID3D11DeviceContext* pd3dContext, TressFX_SceneMesh sceneMesh,
                                              ID3D11UnorderedAccessView** ppSkinningTransformationsUAV,
                                              XMMATRIX *pModelTransformForHead)
{
    HRESULT hr = S_OK;

    // update the constant bufferss
    D3D11_MAPPED_SUBRESOURCE MappedResource;

    pd3dContext->Map(m_pCBCSPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
    {
        ConstBufferCS_Per_Frame* pCSPerFrame = (ConstBufferCS_Per_Frame*)MappedResource.pData;

        int numOfStrandsPerThreadGroup = THREAD_GROUP_SIZE / m_pTressFXMesh->m_HairAsset.m_NumOfVerticesInStrand;
        pCSPerFrame->NumOfStrandsPerThreadGroup = numOfStrandsPerThreadGroup;
        pCSPerFrame->NumFollowHairsPerGuideHair = (m_bGuideFollowHairPrev ? m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair : 0);
    }
    pd3dContext->Unmap(m_pCBCSPerFrame, 0);
    pd3dContext->CSSetConstantBuffers(0, 1, &m_pCBCSPerFrame);

    pd3dContext->Map(m_pCBGenerateTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
    {
        TransformConstantBuffer* pCSTransformCB = (TransformConstantBuffer*)MappedResource.pData;
        pCSTransformCB->ModelTransformForHead = *pModelTransformForHead;
    }
    pd3dContext->Unmap(m_pCBGenerateTransforms, 0);
    pd3dContext->CSSetConstantBuffers(4, 1, &m_pCBGenerateTransforms);

    //Set the shader resources
    ID3D11ShaderResourceView* ppSRV[3] = {sceneMesh.pMeshVertices, sceneMesh.pTransformedVerts, m_pTressFXMesh->m_HairSkinMappingSRV};

    pd3dContext->CSSetShaderResources( 4, 3, ppSRV);

    //Bind unordered access views
    ID3D11UnorderedAccessView* ppUAV[4] = {m_pTressFXMesh->m_InitialHairPositionsUAV, 0, 0, m_pTressFXMesh->m_HairTransformsUAV};
    pd3dContext->CSSetUnorderedAccessViews( 4, 4, ppUAV, NULL );

    // execute the shader
    int numOfGroupsForCS_StrandLevel = (m_bGuideFollowHairPrev ? m_pTressFXMesh->m_HairAsset.m_NumGuideHairStrands : m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands);

    pd3dContext->CSSetShader(m_CSGenerateTransforms, NULL, 0 );
    pd3dContext->Dispatch(numOfGroupsForCS_StrandLevel, 1, 1);

    *ppSkinningTransformationsUAV = m_pTressFXMesh->m_HairTransformsUAV;

    // unbind the resources
    ppSRV [0] = NULL;
    ppSRV [1] = NULL;
    ppSRV [2] = NULL;
    pd3dContext->CSSetShaderResources( 4, 3, ppSRV);

    ID3D11UnorderedAccessView* ppUAVNULL[4] = {NULL, NULL, NULL, NULL};
    pd3dContext->CSSetUnorderedAccessViews( 4, 4, ppUAVNULL, NULL );

    return hr;
}

const float MATH_PI2 = 3.14159265359f;
#define DEG_TO_RAD2(d) (d * MATH_PI2 / 180)

//--------------------------------------------------------------------------------------
//
// Simulate
//
// Runs the hair simulation wich will animate the hair vertices. This function calls
// Dispatch to execute compute shaders that implement the simulation.
//
//--------------------------------------------------------------------------------------
HRESULT TressFXSimulation::Simulate(ID3D11DeviceContext* pd3dContext, float fElapsedTime, float density,
    tressfx_vec3 & windDir, float windMag, XMMATRIX *pModelTransformForHead,
    ID3D11UnorderedAccessView *pSkinningTransforms, float targetFrameRate/* = 1.0f/60.0f*/, bool singleHeadTransform, bool warp)
{
    m_elapsedTimeSinceLastSim += fElapsedTime;

    // Simulation is sensitive to frame rate. So, we set the target frame rate (defaulted to 1/60) and skip the simulation if the current elapsed time
    // since the last simulation is too early.
    if ((m_elapsedTimeSinceLastSim < targetFrameRate) && !warp)
    {
        return S_OK;
    }

    m_elapsedTimeSinceLastSim = 0;

    HRESULT hr = S_OK;
    UINT initCounts = 0;

    int numOfStrandsPerThreadGroup = THREAD_GROUP_SIZE/m_pTressFXMesh->m_HairAsset.m_NumOfVerticesInStrand;

    // ConstBufferCS_Per_Frame
    D3D11_MAPPED_SUBRESOURCE MappedResource;

    pd3dContext->Map(m_pCBCSPerFrame, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
    {
        ConstBufferCS_Per_Frame* pCSPerFrame = (ConstBufferCS_Per_Frame*)MappedResource.pData;

        pCSPerFrame->bWarp = warp;

        pCSPerFrame->NumLengthConstraintIterations = m_simParams.numLengthConstraintIterations;
        pCSPerFrame->bCollision = (m_simParams.bCollision == true) ? 1 : 0;

        pCSPerFrame->GravityMagnitude = m_simParams.gravityMagnitude;

        pCSPerFrame->timeStep = targetFrameRate;

        pCSPerFrame->NumOfStrandsPerThreadGroup = numOfStrandsPerThreadGroup;
        pCSPerFrame->NumFollowHairsPerGuideHair = (m_bGuideFollowHairPrev ? m_pTressFXMesh->m_HairAsset.m_NumFollowHairsPerGuideHair : 0);
        pCSPerFrame->TipSeparationFactor = m_pTressFXMesh->m_HairAsset.m_TipSeparationFactor;

        static int frame = 0;

        float wM = windMag * (pow(sin(frame*0.05f), 2.0f) + 0.5f);

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
            tressfx_vec3 newWindDir = rotFromXAxisToWindDir * rot * XAxis;
            pCSPerFrame->Wind = float4(newWindDir.x * wM, newWindDir.y * wM, newWindDir.z * wM, (float)frame);
        }

        {
            tressfx_vec3 rotAxis(0, -1.0f, 0);
            tressfx_quat rot(rotAxis, angleToWideWindCone);
            tressfx_vec3 newWindDir = rotFromXAxisToWindDir * rot * XAxis;
            pCSPerFrame->Wind1 = float4(newWindDir.x * wM, newWindDir.y * wM, newWindDir.z * wM, (float)frame);
        }

        {
            tressfx_vec3 rotAxis(0, 0, 1.0f);
            tressfx_quat rot(rotAxis, angleToWideWindCone);
            tressfx_vec3 newWindDir = rotFromXAxisToWindDir * rot * XAxis;
            pCSPerFrame->Wind2 = float4(newWindDir.x * wM, newWindDir.y * wM, newWindDir.z * wM, (float)frame);
        }

        {
            tressfx_vec3 rotAxis(0, 0, -1.0f);
            tressfx_quat rot(rotAxis, angleToWideWindCone);
            tressfx_vec3 newWindDir = rotFromXAxisToWindDir * rot * XAxis;
            pCSPerFrame->Wind3 = float4(newWindDir.x * wM, newWindDir.y * wM, newWindDir.z * wM, (float)frame);
        }

        frame++;

        int numSections = m_simParams.numHairSections;

        // hair section 0
        if (numSections > 0)
        {
            pCSPerFrame->Damping0 = m_simParams.perSectionShapeParams[0].damping;
            pCSPerFrame->StiffnessForLocalShapeMatching0 = m_simParams.perSectionShapeParams[0].stiffnessForLocalShapeMatching;
            pCSPerFrame->StiffnessForGlobalShapeMatching0 = m_simParams.perSectionShapeParams[0].stiffnessForGlobalShapeMatching;
            pCSPerFrame->GlobalShapeMatchingEffectiveRange0 = m_simParams.perSectionShapeParams[0].globalShapeMatchingEffectiveRange;
        }

        // hair section 1
        if (numSections > 1)
        {
            pCSPerFrame->Damping1 = m_simParams.perSectionShapeParams[1].damping;
            pCSPerFrame->StiffnessForLocalShapeMatching1 = m_simParams.perSectionShapeParams[1].stiffnessForLocalShapeMatching;
            pCSPerFrame->StiffnessForGlobalShapeMatching1 = m_simParams.perSectionShapeParams[1].stiffnessForGlobalShapeMatching;
            pCSPerFrame->GlobalShapeMatchingEffectiveRange1 = m_simParams.perSectionShapeParams[1].globalShapeMatchingEffectiveRange;
        }

        // hair section 2
        if (numSections > 2)
        {
            pCSPerFrame->Damping2 = m_simParams.perSectionShapeParams[2].damping;
            pCSPerFrame->StiffnessForLocalShapeMatching2 = m_simParams.perSectionShapeParams[2].stiffnessForLocalShapeMatching;
            pCSPerFrame->StiffnessForGlobalShapeMatching2 = m_simParams.perSectionShapeParams[2].stiffnessForGlobalShapeMatching;
            pCSPerFrame->GlobalShapeMatchingEffectiveRange2 = m_simParams.perSectionShapeParams[2].globalShapeMatchingEffectiveRange;
        }

        // hair section 3
        if (numSections > 3)
        {
            pCSPerFrame->Damping3 = m_simParams.perSectionShapeParams[3].damping;
            pCSPerFrame->StiffnessForLocalShapeMatching3 = m_simParams.perSectionShapeParams[3].stiffnessForLocalShapeMatching;
            pCSPerFrame->StiffnessForGlobalShapeMatching3 = m_simParams.perSectionShapeParams[3].stiffnessForGlobalShapeMatching;
            pCSPerFrame->GlobalShapeMatchingEffectiveRange3 = m_simParams.perSectionShapeParams[3].globalShapeMatchingEffectiveRange;
        }

        pCSPerFrame->NumLocalShapeMatchingIterations = m_simParams.numLocalShapeMatchingIterations;
        pCSPerFrame->NumVerticesPerStrand = g_TressFXNumVerticesPerStrand;
    }
    pd3dContext->Unmap(m_pCBCSPerFrame, 0);

    // ConstBufferCS_HeadTransform
    pd3dContext->Map(m_pCBHeadTransforms, 0, D3D11_MAP_WRITE_DISCARD, 0, &MappedResource);
    {
        ConstBufferCS_HeadTransform* pCSHeadTransform = (ConstBufferCS_HeadTransform*)MappedResource.pData;
        pCSHeadTransform->bSingleHeadTransform = singleHeadTransform;
        pCSHeadTransform->ModelRotateForHead = XMQuaternionRotationMatrix(*pModelTransformForHead);
        pCSHeadTransform->ModelTransformForHead = *pModelTransformForHead;
        pCSHeadTransform->InvModelTransformForHead = DirectX::XMMatrixInverse(NULL, pCSHeadTransform->ModelTransformForHead);

        //pCSHeadTransform->ModelTransformForHead = XMMatrixSet(1,0,0,0, 0,1,0,0, 0,0,1,0, -26,36,-58,1);
    }
    pd3dContext->Unmap(m_pCBHeadTransforms, 0);

    // set const buffers
    pd3dContext->CSSetConstantBuffers(0, 1, &m_pCBCSPerFrame);
    pd3dContext->CSSetConstantBuffers(3, 1, &m_pCBCSCollisionCapsule);
    pd3dContext->CSSetConstantBuffers(5, 1, &m_pCBHeadTransforms);

    //Set the shader resources
    ID3D11ShaderResourceView* ppSRV[4] =  { m_pTressFXMesh->m_HairRestLengthSRV,
                                            m_pTressFXMesh->m_HairStrandTypeSRV,
                                            m_pTressFXMesh->m_HairRefVecsInLocalFrameSRV,
                                            m_pTressFXMesh->m_FollowHairRootOffsetSRV
                                          };

    pd3dContext->CSSetShaderResources( 0, 4, ppSRV);

    //Bind unordered access views
    ID3D11UnorderedAccessView* ppUAV[D3D11_PS_CS_UAV_REGISTER_COUNT] = {
                                            m_pTressFXMesh->m_HairVertexPositionsUAV,
                                            m_pTressFXMesh->m_HairVertexPositionsRelativeUAV,
                                            m_pTressFXMesh->m_HairVertexPositionsPrevUAV,
                                            m_pTressFXMesh->m_HairVertexTangentsUAV,
                                            m_pTressFXMesh->m_InitialHairPositionsUAV,
                                            m_pTressFXMesh->m_GlobalRotationsUAV,
                                            m_pTressFXMesh->m_LocalRotationsUAV,
                                            pSkinningTransforms
                                          };

    pd3dContext->CSSetUnorderedAccessViews( 0, D3D11_PS_CS_UAV_REGISTER_COUNT, ppUAV, &initCounts );

    //======= Run the compute shader =======

    // Increase density a little bit (0.05) to hide popping. This allows simulation to take more hair than rendering and gives a chance
    // to simulate hairs before they get rendered so that there will be enough time for popping to disappear.
    density += 0.05f;

    if ( density > 1.0f )
    {
        density = 1.0f;
    }

    int numOfGroupsForCS_VertexLevel = (int)(((float)(m_bGuideFollowHairPrev? m_pTressFXMesh->m_HairAsset.m_NumGuideHairVertices :
        m_pTressFXMesh->m_HairAsset.m_NumTotalHairVertices) / (float)THREAD_GROUP_SIZE)*density);

    // Prepare follow hair vertices before they are turning into guide ones.
    // One thread computes one vertex
    if ( m_bGuideFollowHairPrev && !m_simParams.bGuideFollowSimulation )
    {
        pd3dContext->CSSetShader(m_CSPrepareFollowHairBeforeTurningIntoGuide, NULL, 0 );
        pd3dContext->Dispatch(numOfGroupsForCS_VertexLevel, 1, 1);
    }

    // Integrate and global shape constraints
    // One thread computes one vertex
    pd3dContext->CSSetShader(m_CSIntegrationAndGlobalShapeConstraints, NULL, 0 );
    pd3dContext->Dispatch(numOfGroupsForCS_VertexLevel, 1, 1);

    // Local shape constraints. If the hair is very curly, increase the iteration so that
    // hair style can be preserved well.
    // One thread computes one strand

    // If more than 16 vertices per strand, iterate on the CPU
    if (g_TressFXNumVerticesPerStrand > 16 )
    {
        for ( int iteration = 0; iteration < m_simParams.numLocalShapeMatchingIterations; iteration++)
        {
            int numOfGroupsForCS_StrandLevel = (int)(((float)(m_bGuideFollowHairPrev? m_pTressFXMesh->m_HairAsset.m_NumGuideHairStrands :
                m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands)/(float)THREAD_GROUP_SIZE)*density);
            pd3dContext->CSSetShader(m_CSLocalShapeConstraints, NULL, 0 );
            pd3dContext->Dispatch(numOfGroupsForCS_StrandLevel, 1, 1);
        }
    }
    else
    {
        int numOfGroupsForCS_StrandLevel = (int)(((float)(m_bGuideFollowHairPrev? m_pTressFXMesh->m_HairAsset.m_NumGuideHairStrands :
            m_pTressFXMesh->m_HairAsset.m_NumTotalHairStrands)/(float)THREAD_GROUP_SIZE)*density);
        pd3dContext->CSSetShader(m_CSLocalShapeConstraintsSingleDispatch, NULL, 0 );
        pd3dContext->Dispatch(numOfGroupsForCS_StrandLevel, 1, 1);
    }

    // Edge length constraints, wind and collisions
    // One thread computes one vertex
    pd3dContext->CSSetShader(m_CSLengthConstriantsWindAndCollision, NULL, 0 );
    pd3dContext->Dispatch(numOfGroupsForCS_VertexLevel, 1, 1);

    // Update follow hair vertices
    // One thread computes one vertex
    if ( m_bGuideFollowHairPrev )
    {
        pd3dContext->CSSetShader( m_CSUpdateFollowHairVertices, NULL, 0 );
        pd3dContext->Dispatch(numOfGroupsForCS_VertexLevel, 1, 1);
    }

    // Unbind resources for CS
    ID3D11UnorderedAccessView* ppUAViewNULL[D3D11_PS_CS_UAV_REGISTER_COUNT] = { 0 };
    pd3dContext->CSSetUnorderedAccessViews( 0, D3D11_PS_CS_UAV_REGISTER_COUNT, ppUAViewNULL, &initCounts );

    ID3D11ShaderResourceView* ppSRVNULL[4] = { NULL, NULL, NULL, NULL};
    pd3dContext->CSSetShaderResources( 0, 4, ppSRVNULL );

    m_bGuideFollowHairPrev = m_simParams.bGuideFollowSimulation;

    return hr;
}

//--------------------------------------------------------------------------------------
//
// CreateComputeShaderConstantBuffers
//
// Called by OnCreateDevice to allocate a constant buffer used by the compute shader code
//
//--------------------------------------------------------------------------------------
HRESULT TressFXSimulation::CreateComputeShaderConstantBuffers(ID3D11Device* pd3dDevice, TressFX_CollisionCapsule *pCollision)
{
    HRESULT hr = S_OK;

    // const buffer for per frame data
    D3D11_BUFFER_DESC Desc;
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.MiscFlags = 0;
    Desc.ByteWidth = sizeof( ConstBufferCS_Per_Frame );
    AMD_SAFE_RELEASE(m_pCBCSPerFrame);
    AMD_V_RETURN( pd3dDevice->CreateBuffer(&Desc, NULL, &m_pCBCSPerFrame) );

    // const buffer for capsule collision
    Desc.Usage = D3D11_USAGE_IMMUTABLE;
    Desc.CPUAccessFlags = 0;
    Desc.ByteWidth = sizeof(TressFX_CollisionCapsule);
    D3D11_SUBRESOURCE_DATA data;
    data.pSysMem = (void *)pCollision;
    AMD_SAFE_RELEASE(m_pCBCSCollisionCapsule);
    AMD_V_RETURN( pd3dDevice->CreateBuffer(&Desc, &data, &m_pCBCSCollisionCapsule) );

    // const buffer for hair root transformation
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.MiscFlags = 0;
    Desc.ByteWidth = sizeof( TransformConstantBuffer );
    AMD_SAFE_RELEASE(m_pCBGenerateTransforms);
    AMD_V_RETURN( pd3dDevice->CreateBuffer(&Desc, NULL, &m_pCBGenerateTransforms) );

    // const buffer for head transformation
    Desc.Usage = D3D11_USAGE_DYNAMIC;
    Desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    Desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    Desc.MiscFlags = 0;
    Desc.ByteWidth = sizeof( ConstBufferCS_HeadTransform );
    AMD_SAFE_RELEASE(m_pCBHeadTransforms);
    AMD_V_RETURN( pd3dDevice->CreateBuffer(&Desc, NULL, &m_pCBHeadTransforms) );

    return S_OK;
}


//--------------------------------------------------------------------------------------
//
// OnDestroy
//
// Called when the D3D device is being destroyed
//
//--------------------------------------------------------------------------------------
void TressFXSimulation::OnDestroy(bool destroyShaders)
{
    if (destroyShaders)
    {
        AMD_SAFE_RELEASE(m_CSIntegrationAndGlobalShapeConstraints);
        AMD_SAFE_RELEASE(m_CSLocalShapeConstraints);
        AMD_SAFE_RELEASE(m_CSLocalShapeConstraintsSingleDispatch);
        AMD_SAFE_RELEASE(m_CSLengthConstriantsWindAndCollision);
        AMD_SAFE_RELEASE(m_CSUpdateFollowHairVertices);
        AMD_SAFE_RELEASE(m_CSPrepareFollowHairBeforeTurningIntoGuide);
        AMD_SAFE_RELEASE(m_CSGenerateTransforms);
    }

    AMD_SAFE_RELEASE(m_pCBCSPerFrame);
    AMD_SAFE_RELEASE(m_pCBCSCollisionCapsule);
    AMD_SAFE_RELEASE(m_pCBGenerateTransforms);
    AMD_SAFE_RELEASE(m_pCBHeadTransforms);
}

} // namespace AMD
