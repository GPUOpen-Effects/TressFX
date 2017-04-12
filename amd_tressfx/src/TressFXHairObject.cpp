// ----------------------------------------------------------------------------
// Interface to strands of hair.  Both in terms of rendering, and the data 
// required for simulation.
// ----------------------------------------------------------------------------
//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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


#include "TressFXHairObject.h"

// TressFX
#include "Math/Matrix44.h"
#include "Math/Vector3D.h"
#include "TressFXAsset.h"
#include "TressFXLayouts.h"
#include "TressFXEngineInterface.h"

#include <memory.h>

using namespace AMD;

#define TRESSFX_MIN_VERTS_PER_STRAND_FOR_GPU_ITERATION 64

TressFXHairObject::TressFXHairObject()
    : m_NumTotalVertice(0)
    , m_NumTotalStrands(0)
    , m_NumVerticePerStrand(0)
    , m_CPULocalShapeIterations(0)
    , mSimulationFrame(0)
    , m_pDrawStrandsBindSet(nullptr)
    , m_pSimBindSet(nullptr)
{
}

TressFXHairObject::~TressFXHairObject() {}

static inline void InitialDataUpload(EI_CommandContextRef    commandContext,
                                     void*                pSource,
                                     AMD::uint32          size,
                                     EI_StructuredBufferRef sb)
{
    void* pDest = EI_SB_Map(commandContext, sb);
    TRESSFX_ASSERT(pDest);
    memcpy(pDest, pSource, size);
    EI_SB_Unmap(commandContext, sb);
}



// Load simulation compute shader and create all buffers
// command context used to upload initial data.
void TressFXHairObject::Create(AMD::TressFXAsset* asset,
                               EI_Device*   pDevice,
                               EI_CommandContextRef  commandContext,
                               EI_StringHash      name,
                               EI_SRV             hairColorTextureSRV
                              )
{
    m_NumTotalVertice = asset->m_numTotalVertices;
    m_NumTotalStrands = asset->m_numTotalStrands;
    m_NumVerticePerStrand = asset->m_numVerticesPerStrand;

    // Create buffers for simulation
    {
        mPosTanCollection.CreateGPUResources(pDevice, m_NumTotalVertice, name);

        // initial hair positions
        mInitialHairPositionsBuffer =
            EI_CreateReadOnlySB(pDevice,
                                 sizeof(AMD::TRESSFX::float4),
                                 m_NumTotalVertice,
                                 TRESSFX_STRING_HASH("InitialPosition"),
                                 name
                                );

        // global rotations
            mGlobalRotationsBuffer =
                EI_CreateReadOnlySB(pDevice,
                                     sizeof(AMD::TRESSFX::float4),
                                     m_NumTotalVertice,
                                     TRESSFX_STRING_HASH("GlobalRotations"), name);



        // rest lengths
            mHairRestLengthSRVBuffer =
                EI_CreateReadOnlySB(pDevice,
                                    sizeof(float),
                                    m_NumTotalVertice,
                                    TRESSFX_STRING_HASH("RestLength"), name);



        // strand types
            mHairStrandTypeBuffer = EI_CreateReadOnlySB(pDevice,
                                                        sizeof(int),
                                                        m_NumTotalStrands,
                                                        TRESSFX_STRING_HASH("StrandType"), name);



        // reference vectors in local frame
            mHairRefVecsInLocalFrameBuffer =
                EI_CreateReadOnlySB(pDevice,
                                    sizeof(AMD::TRESSFX::float4),
                                    m_NumTotalVertice,
                                    TRESSFX_STRING_HASH("LocalRef"), name);


        // follow hair root offsets
            mFollowHairRootOffsetBuffer =
                EI_CreateReadOnlySB(pDevice,
                                    sizeof(AMD::TRESSFX::float4),
                                    m_NumTotalStrands,
                                    TRESSFX_STRING_HASH("RootOffset"), name);


        // bone skinning data
         mBoneSkinningDataBuffer = EI_CreateReadOnlySB(pDevice, sizeof(AMD::TressFXBoneSkinningData), m_NumTotalStrands, TRESSFX_STRING_HASH("SkinningData"),name);
    }

    // UPLOAD INITIAL DATA
    // UAVs must first be transitioned for copy dest, since they start with UAV state.
    // When done, we transition to appropriate state for start of first frame.

    mPosTanCollection.UploadGPUData(
        commandContext, asset->m_positions, asset->m_tangents, m_NumTotalVertice);

    InitialDataUpload(commandContext, asset->m_positions, sizeof(AMD::TRESSFX::float4) * m_NumTotalVertice, *mInitialHairPositionsBuffer);
    InitialDataUpload(commandContext, asset->m_globalRotations, sizeof(AMD::TRESSFX::float4) * m_NumTotalVertice, *mGlobalRotationsBuffer);
    InitialDataUpload(commandContext, asset->m_restLengths, sizeof(float) * m_NumTotalVertice, *mHairRestLengthSRVBuffer);
    InitialDataUpload(commandContext, asset->m_strandTypes, sizeof(int) * m_NumTotalStrands, *mHairStrandTypeBuffer);
    InitialDataUpload(commandContext, asset->m_refVectors, sizeof(AMD::TRESSFX::float4) * m_NumTotalVertice, *mHairRefVecsInLocalFrameBuffer);
    InitialDataUpload(commandContext, asset->m_followRootOffsets, sizeof(AMD::TRESSFX::float4) * m_NumTotalStrands, *mFollowHairRootOffsetBuffer);
    InitialDataUpload(commandContext, asset->m_boneSkinningData, sizeof(AMD::TressFXBoneSkinningData) * m_NumTotalStrands, *mBoneSkinningDataBuffer);

    EI_Barrier copyBarriers[] = 
    {
        { mInitialHairPositionsBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV },
        { mGlobalRotationsBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV },
        { mHairRestLengthSRVBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV },
        { mHairStrandTypeBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV },
        { mHairRefVecsInLocalFrameBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV },
        { mFollowHairRootOffsetBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV },
        { mBoneSkinningDataBuffer, EI_STATE_COPY_DEST, EI_STATE_NON_PS_SRV }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(copyBarriers), (EI_Barrier*) copyBarriers);

    m_NumOfVerticesInStrand      = asset->m_numVerticesPerStrand;
    m_NumFollowHairsPerGuideHair = asset->m_numFollowStrandsPerGuide;

    // Bind set
    TressFXBindSet bindSet;

    EI_SRV      SRVs[7];

    SRVs[0] = EI_GetSRV(mInitialHairPositionsBuffer);
    SRVs[1] = EI_GetSRV(mGlobalRotationsBuffer);
    SRVs[2] = EI_GetSRV(mHairRestLengthSRVBuffer);
    SRVs[3] = EI_GetSRV(mHairStrandTypeBuffer);
    SRVs[4] = EI_GetSRV(mHairRefVecsInLocalFrameBuffer);
    SRVs[5] = EI_GetSRV(mFollowHairRootOffsetBuffer);
    SRVs[6] = EI_GetSRV(mBoneSkinningDataBuffer);

    bindSet.nSRVs  = AMD_ARRAY_SIZE(SRVs);
    bindSet.nUAVs  = 0;
    bindSet.uavs   = nullptr;
    bindSet.srvs   = SRVs;
    bindSet.values = &(mSimCB);
    bindSet.nBytes = sizeof(TressFXSimulationConstantBuffer);

    m_pSimBindSet = EI_CreateBindSet(pDevice, bindSet);

    // Set up with defaults.
    UpdateSimulationParameters(TressFXSimulationSettings());

    // Rendering setup
    CreateRenderingGPUResources(pDevice, *asset, name);
    PopulateDrawStrandsBindSet(pDevice, hairColorTextureSRV);
    UploadRenderingGPUResources(commandContext, *asset);

}


void TressFXHairObject::Destroy(EI_Device* pDevice)
{
    // Destroy rendering resources.
    EI_DestroyBindSet(pDevice, m_pDrawStrandsBindSet);
    if (mHairTexCoords)
    {
        EI_Destroy(pDevice, mHairTexCoords);
    }
    AMD_SAFE_RESOURCE_DELETE(pDevice, mHairVertexRenderParams);


    // destroy Sim resources.
    EI_DestroyBindSet(pDevice, m_pSimBindSet);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mInitialHairPositionsBuffer);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mGlobalRotationsBuffer);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mHairRestLengthSRVBuffer);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mHairStrandTypeBuffer);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mHairRefVecsInLocalFrameBuffer);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mFollowHairRootOffsetBuffer);
    AMD_SAFE_RESOURCE_DELETE(pDevice, mBoneSkinningDataBuffer);

    // destroy shared resources
    mPosTanCollection.Destroy(pDevice);

    EI_IB_Destroy(pDevice, mpIndexBuffer);
}


// TODO Move wind settings to Simulation Parameters or whatever.

// Wind is in a pyramid around the main wind direction.
// To add a random appearance, the shader will sample some direction
// within this cone based on the strand index.
// This function computes the vector for each edge of the pyramid.
static void SetWindCorner(AMD::tressfx_quat     rotFromXAxisToWindDir,
                          AMD::tressfx_vec3     rotAxis,
                          float                 angleToWideWindCone,
                          float                 wM,
                          AMD::TRESSFX::float4& outVec)
{
    static const AMD::tressfx_vec3 XAxis(1.0f, 0, 0);
    AMD::tressfx_quat              rot(rotAxis, angleToWideWindCone);
    AMD::tressfx_vec3              newWindDir = rotFromXAxisToWindDir * rot * XAxis;
    outVec.x                                  = newWindDir.x * wM;
    outVec.y                                  = newWindDir.y * wM;
    outVec.z                                  = newWindDir.z * wM;
    outVec.w                                  = 0;  // unused.
}

const float MATH_PI2 = 3.14159265359f;
#define DEG_TO_RAD2(d) (d * MATH_PI2 / 180)


void TressFXHairObject::SetWind(const AMD::tressfx_vec3& windDir, float windMag, int frame)
{
    float wM = windMag * (pow(sin(frame * 0.01f), 2.0f) + 0.5f);

    AMD::tressfx_vec3 windDirN(windDir);
    windDirN.Normalize();

    AMD::tressfx_vec3 XAxis(1.0f, 0, 0);
    AMD::tressfx_vec3 xCrossW = XAxis.Cross(windDirN);

    AMD::tressfx_quat rotFromXAxisToWindDir;
    rotFromXAxisToWindDir.SetIdentity();

    float angle = asin(xCrossW.Length());

    if (angle > 0.001)
    {
        rotFromXAxisToWindDir.SetRotation(xCrossW.Normalize(), angle);
    }

    float angleToWideWindCone = DEG_TO_RAD2(40.f);

    SetWindCorner(rotFromXAxisToWindDir,
                  AMD::tressfx_vec3(0, 1.0, 0),
                  angleToWideWindCone,
                  wM,
                  mSimCB.m_Wind);
    SetWindCorner(rotFromXAxisToWindDir,
                  AMD::tressfx_vec3(0, -1.0, 0),
                  angleToWideWindCone,
                  wM,
                  mSimCB.m_Wind1);
    SetWindCorner(rotFromXAxisToWindDir,
                  AMD::tressfx_vec3(0, 0, 1.0),
                  angleToWideWindCone,
                  wM,
                  mSimCB.m_Wind2);
    SetWindCorner(rotFromXAxisToWindDir,
                  AMD::tressfx_vec3(0, 0, -1.0),
                  angleToWideWindCone,
                  wM,
                  mSimCB.m_Wind3);

    // fourth component unused. (used to store frame number, but no longer used).
}

void TressFXHairObject::UpdatePerObjectRenderParams()
{
    mRenderCB.m_NumVerticesPerStrand = m_NumVerticePerStrand;
}

void TressFXHairObject::DrawStrands(EI_CommandContextRef commandContext,
                                    EI_PSO&            pso)
{
    UpdatePerObjectRenderParams();

    // at some point, should probably pass these in to EI_BindAndDrawIndexedInstanced.

    EI_Bind(commandContext, GetRenderPosTanLayout(), mPosTanCollection.GetRenderBindSet());
    //EI_Bind(commandContext, renderLayout, *m_pDrawStrandsBindSet);
    EI_Bind(commandContext, GetRenderLayout(), *m_pDrawStrandsBindSet);
    // This could be an argument at some point.
    AMD::uint32 nStrandCopies = 1;
    float  density = 1.0f;

    EI_IndexedDrawParams drawParams;
    drawParams.pIndexBuffer = mpIndexBuffer;
    drawParams.numIndices = AMD::uint32(density * mtotalIndices);
    drawParams.numInstances = nStrandCopies;

    EI_Draw(commandContext, pso, drawParams);
}

void TressFXHairObject::CreateRenderingGPUResources(EI_Device*   pDevice,
                                                    AMD::TressFXAsset& asset,
                                                    EI_StringHash      name)
{
    // If rendering is seperated, we might be copying the asset count to the local member variable.
    // But since this is currently loaded after simulation right now, we'll just make sure
    // it's already set.
    TRESSFX_ASSERT(asset.m_numTotalStrands == m_NumTotalStrands);
    TRESSFX_ASSERT(asset.m_numTotalVertices == m_NumTotalVertice);
    mtotalIndices = asset.GetNumHairTriangleIndices();  // asset.GetNumHairTriangleIndices();

    if (asset.m_strandUV)
    {
        mHairTexCoords = EI_CreateReadOnlySB(pDevice,
                                             2 * sizeof(AMD::real32),
                                             m_NumTotalStrands,
                                             TRESSFX_STRING_HASH("TexCoords"), name);
    }

    mHairVertexRenderParams = EI_CreateReadOnlySB(pDevice,
                                                  sizeof(AMD::real32),
                                                  m_NumTotalVertice,
                                                  TRESSFX_STRING_HASH("VertRenderParams"), name);

    // TODO seperate creation from upload. Go through a real interface.
    mpIndexBuffer = EI_CreateIndexBuffer(pDevice, mtotalIndices, asset.m_triangleIndices, name);
}

void TressFXHairObject::UploadRenderingGPUResources(EI_CommandContextRef  commandContext,
                                                    AMD::TressFXAsset& asset)
{
    TRESSFX_ASSERT(asset.m_numTotalStrands == m_NumTotalStrands);
    TRESSFX_ASSERT(asset.m_numTotalVertices == m_NumTotalVertice);
    TRESSFX_ASSERT(mtotalIndices == asset.GetNumHairTriangleIndices());

    if (asset.m_strandUV)
    {
        InitialDataUpload(commandContext,
                                 asset.m_strandUV,
                                 2 * sizeof(AMD::real32) * m_NumTotalStrands,
                                 *mHairTexCoords);
    }

    InitialDataUpload(commandContext,
                             asset.m_thicknessCoeffs,
                             sizeof(AMD::real32) * m_NumTotalVertice,
                             *mHairVertexRenderParams);

    EI_Barrier copyToVS[] = 
    {
        {mHairTexCoords, EI_STATE_COPY_DEST, EI_STATE_VS_SRV},
        { mHairVertexRenderParams, EI_STATE_COPY_DEST, EI_STATE_VS_SRV }
    };

    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(copyToVS), copyToVS);

}


void TressFXHairObject::PopulateDrawStrandsBindSet(EI_Device* pDevice,
                                                   EI_SRV           hairColorTextureSRV)
{
    EI_SRV srvs[3];

    srvs[TRESSFX_IDSRV_THICKNESS_OFFSET] = EI_GetSRV(mHairVertexRenderParams);

    // The number (1 to 3) is based on whether we have texture coordinates and a texture,
    // determined below.
    // This will have to match the shader expectation, which is checked elsewhere.

    // CLEANUP Call the TressFX one a description?
    TressFXBindSet bindSetDesc;
    bindSetDesc.nUAVs = 0;
    bindSetDesc.uavs = 0;
    bindSetDesc.srvs = srvs;
    bindSetDesc.nSRVs = 1;



    if (hairColorTextureSRV)
    {
        if (! EI_GetSRV(mHairTexCoords) )
        {
            TressFXLogWarning(
                "hair color texture specified for a model that has no hair texture coordinates. It will be ignored.");
        }
        else
        {
            srvs[TRESSFX_IDSRV_ROOT_TEXCOORD_OFFSET] = EI_GetSRV(mHairTexCoords);
            ++bindSetDesc.nSRVs;
            if (hairColorTextureSRV)
            {
                srvs[TRESSFX_IDSRV_COLOR_TEXTURE_OFFSET] = hairColorTextureSRV;
                ++bindSetDesc.nSRVs;
            }
        }
    }

    bindSetDesc.values  = &(mRenderCB);
    bindSetDesc.nBytes  = sizeof(mRenderCB);
    m_pDrawStrandsBindSet = EI_CreateBindSet(pDevice, bindSetDesc);
}

void TressFXHairObject::UpdateBoneMatrices(EI_CommandContextRef  commandContext, const float* pBoneMatricesInWS, size_t sizeOfBoneMatrices)
{
    memcpy(mSimCB.m_BoneSkinningMatrix,
           pBoneMatricesInWS,
           (AMD::uint32)(sizeOfBoneMatrices < AMD_TRESSFX_MAX_NUM_BONES * sizeof(float) * 16
                        ? sizeOfBoneMatrices
                        : AMD_TRESSFX_MAX_NUM_BONES * sizeof(float) * 16));
}


void TressFXHairObject::UpdateSimulationParameters(const TressFXSimulationSettings& settings)
{
    mSimCB.SetVelocityShockPropogation(settings.m_vspCoeff);
    mSimCB.SetVSPAccelThreshold(settings.m_vspAccelThreshold);

    mSimCB.SetDamping(settings.m_damping);
    mSimCB.SetLocalStiffness(settings.m_localConstraintStiffness);
    mSimCB.SetGlobalStiffness(settings.m_globalConstraintStiffness);
    mSimCB.SetGlobalRange(settings.m_globalConstraintsRange);
    mSimCB.SetLocalStiffness(settings.m_localConstraintStiffness);

    mSimCB.SetGravity(settings.m_gravityMagnitude);
    mSimCB.SetTimeStep(1.f / 60.f);
    mSimCB.SetCollision(false);
    mSimCB.SetVerticesPerStrand(m_NumOfVerticesInStrand);
    mSimCB.SetFollowHairsPerGuidHair(m_NumFollowHairsPerGuideHair);
    mSimCB.SetTipSeperation(settings.m_tipSeparation);

    // Right now, we do all local contraint iterations on the CPU.
    // It's actually a bit faster to

    if (m_NumVerticePerStrand >= TRESSFX_MIN_VERTS_PER_STRAND_FOR_GPU_ITERATION)
    {
        mSimCB.SetLocalIterations((int)settings.m_localConstraintsIterations);
        m_CPULocalShapeIterations = 1;
    }
    else
    {
        mSimCB.SetLocalIterations(1);
        m_CPULocalShapeIterations = (int)settings.m_localConstraintsIterations;
    }

    mSimCB.SetLengthIterations((int)settings.m_lengthConstraintsIterations);

    // Set wind parameters
    AMD::tressfx_vec3 windDir(
        settings.m_windDirection[0], settings.m_windDirection[1], settings.m_windDirection[2]);
    float windMag = settings.m_windMagnitude;
    SetWind(windDir, windMag, mSimulationFrame);


#if TRESSFX_COLLISION_CAPSULES
    mSimCB.m_numCollisionCapsules.x = 0;

    // Below is an example showing how to pass capsule collision objects. 
    /*
    mSimCB.m_numCollisionCapsules.x = 1;
    mSimCB.m_centerAndRadius0[0] = { 0, 0.f, 0.f, 50.f };
    mSimCB.m_centerAndRadius1[0] = { 0, 100.f, 0, 10.f };
    */
#endif



    // Bone matrix set elsewhere. It is not dependent on the settings passed in here.
}


// Positions and tangetns are handled in the following order, from the point of view of each
// buffer.
//
// Positions updated from previous sim output by copy. (COPY_DEST)
// Simulate updates mPositions and mPositionsPrev (UAVs)  also updates mTangents (this might move)
// SDF updates mPositions and mPositionsPrev (UAVs)
// Render with mPositions and mTangnets (PS SRVs)

PosTanCollection::PosTanCollection() {}
PosTanCollection::~PosTanCollection() {}

void PosTanCollection::CreateGPUResources(EI_Device* pDevice, int numVertices, EI_StringHash name)
{
    m_PositionsPrev = EI_CreateReadWriteSB(pDevice, sizeof(AMD::TRESSFX::float4), numVertices, TRESSFX_STRING_HASH("PosPrev"), name);
    m_PositionsPrevPrev = EI_CreateReadWriteSB(pDevice, sizeof(AMD::TRESSFX::float4), numVertices, TRESSFX_STRING_HASH("PosPrevPrev"), name);
    m_Positions = EI_CreateReadWriteSB(pDevice, sizeof(AMD::TRESSFX::float4), numVertices, TRESSFX_STRING_HASH("Pos"), name);
    m_Tangents = EI_CreateReadWriteSB(pDevice, sizeof(AMD::TRESSFX::float4), numVertices, TRESSFX_STRING_HASH("Tan"), name);

    TressFXBindSet bindSet;
    // No constant buffer.
    bindSet.nBytes = 0;
    bindSet.values = 0;

    EI_UAV simUAVs[4];
    EI_UAV applySDFUAVs[2];
    EI_SRV renderSRVs[2];

    // We supply storage for the list of uavs in this class.
    simUAVs[TRESSFX_IDUAV_POS_OFFSET]           = EI_GetUAV(m_Positions);
    simUAVs[TRESSFX_IDUAV_POS_PREV_OFFSET]      = EI_GetUAV(m_PositionsPrev);
    simUAVs[TRESSFX_IDUAV_POS_PREV_PREV_OFFSET] = EI_GetUAV(m_PositionsPrevPrev);
    simUAVs[TRESSFX_IDUAV_TAN_OFFSET]           = EI_GetUAV(m_Tangents);

    bindSet.nSRVs = 0;
    bindSet.nUAVs = AMD_ARRAY_SIZE(simUAVs);
    TRESSFX_ASSERT(bindSet.nUAVs == 4);  // check that I used the macro right.
    bindSet.uavs = simUAVs;
    m_pSimBindSets = EI_CreateBindSet(pDevice, bindSet);

    applySDFUAVs[TRESSFX_IDUAV_POS_OFFSET]           = EI_GetUAV(m_Positions);
    applySDFUAVs[TRESSFX_IDUAV_POS_PREV_OFFSET]      = EI_GetUAV(m_PositionsPrev);
    //applySDFUAVs[TRESSFX_IDUAV_POS_PREV_PREV_OFFSET] = mPositionsPrevPrev.uav;

    bindSet.nSRVs     = 0;
    bindSet.nUAVs     = AMD_ARRAY_SIZE(applySDFUAVs);
    bindSet.uavs      = applySDFUAVs;
    m_pApplySDFBindSets = EI_CreateBindSet(pDevice, bindSet);

    renderSRVs[TRESSFX_IDSRV_POS_OFFSET] = EI_GetSRV(m_Positions);
    renderSRVs[TRESSFX_IDSRV_TAN_OFFSET] = EI_GetSRV(m_Tangents);

    bindSet.nSRVs   = AMD_ARRAY_SIZE(renderSRVs);
    bindSet.srvs    = renderSRVs;
    bindSet.nUAVs   = 0;
    m_pRenderBindSets = EI_CreateBindSet(pDevice, bindSet);
}


void PosTanCollection::Destroy(EI_Device* pDevice)
{
    EI_DestroyBindSet(pDevice, m_pRenderBindSets);
    EI_DestroyBindSet(pDevice, m_pApplySDFBindSets);
    EI_DestroyBindSet(pDevice, m_pSimBindSets);

    AMD_SAFE_RESOURCE_DELETE(pDevice, m_Tangents);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_Positions);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_PositionsPrevPrev);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_PositionsPrev);
}


void PosTanCollection::UploadGPUData(EI_CommandContextRef commandContext, void* pos, void* tan, int numVertices)
{
    TRESSFX_ASSERT(m_Positions != nullptr);
    TRESSFX_ASSERT(m_Tangents != nullptr);
    TRESSFX_ASSERT(m_PositionsPrev != nullptr);
    TRESSFX_ASSERT(m_PositionsPrevPrev != nullptr);

    EI_Barrier uavToUpload[] = 
    {
        { m_Positions, EI_STATE_UAV, EI_STATE_COPY_DEST },
        { m_Tangents, EI_STATE_UAV, EI_STATE_COPY_DEST },
        { m_PositionsPrev, EI_STATE_UAV, EI_STATE_COPY_DEST },
        { m_PositionsPrevPrev, EI_STATE_UAV, EI_STATE_COPY_DEST }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(uavToUpload), uavToUpload);


    InitialDataUpload(commandContext, pos, sizeof(AMD::TRESSFX::float4) * numVertices, *m_Positions);
    InitialDataUpload(commandContext, tan, sizeof(AMD::TRESSFX::float4) * numVertices, *m_Tangents);
    InitialDataUpload(commandContext, pos, sizeof(AMD::TRESSFX::float4) * numVertices, *m_PositionsPrev);
    InitialDataUpload(commandContext, pos, sizeof(AMD::TRESSFX::float4) * numVertices, *m_PositionsPrevPrev);

    EI_Barrier uploadToUAV[] =
    {
        { m_Positions, EI_STATE_COPY_DEST, EI_STATE_UAV },
        { m_Tangents, EI_STATE_COPY_DEST, EI_STATE_UAV },
        { m_PositionsPrev, EI_STATE_COPY_DEST, EI_STATE_UAV },
        { m_PositionsPrevPrev, EI_STATE_COPY_DEST, EI_STATE_UAV },
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(uploadToUAV), uploadToUAV);

}

void PosTanCollection::TransitionSimToRendering(EI_CommandContextRef commandContext)
{
    TRESSFX_ASSERT(m_Positions != nullptr);
    TRESSFX_ASSERT(m_Tangents != nullptr);

    EI_Barrier simToRender[] = 
    {
        { m_Positions, EI_STATE_UAV, EI_STATE_VS_SRV },
        { m_Tangents, EI_STATE_UAV, EI_STATE_VS_SRV }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(simToRender), simToRender);
}

void PosTanCollection::TransitionRenderingToSim(EI_CommandContextRef commandContext)
{
    TRESSFX_ASSERT(m_Positions != nullptr);
    TRESSFX_ASSERT(m_Tangents != nullptr);

    EI_Barrier renderToSim[] =
    {
        { m_Positions, EI_STATE_VS_SRV, EI_STATE_UAV },
        { m_Tangents, EI_STATE_VS_SRV, EI_STATE_UAV }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(renderToSim), renderToSim);
}

void PosTanCollection::UAVBarrier(EI_CommandContextRef commandContext)
{
    TRESSFX_ASSERT(m_Positions != nullptr);
    TRESSFX_ASSERT(m_PositionsPrev != nullptr);
    TRESSFX_ASSERT(m_PositionsPrevPrev != nullptr);

    EI_Barrier uav[] = 
    {
        { m_Positions, EI_STATE_UAV, EI_STATE_UAV },
        { m_PositionsPrev, EI_STATE_UAV, EI_STATE_UAV },
        { m_PositionsPrevPrev, EI_STATE_UAV, EI_STATE_UAV }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(uav), uav);

    // Assuming tangent is only written by one kernel, so will get caught with transition to SRV
    // for rendering.
}


