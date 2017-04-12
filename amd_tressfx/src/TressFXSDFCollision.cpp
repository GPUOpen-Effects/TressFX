// ----------------------------------------------------------------------------
// This wraps a single signed distance field, used for TressFX collision.
// it provides the interface to generate the signed distance field, as well
// as apply that signed distance field to a TressFX simulation.
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


// TressFX
#include "TressFXAsset.h"
#include "TressFXHairObject.h"
#include "TressFXLayouts.h"
#include "TressFXSDFCollision.h"
#include "TressFXSDFInputMeshInterface.h"

#include <limits.h>

using namespace AMD::TRESSFX;
using namespace AMD;

TressFXSDFCollision::TressFXSDFCollision()
    : m_CollisionMargin(0)
    , m_NumCellsInXAxis(60)
    , m_GridAllocationMutliplier(1.4f)
    , m_NumTotalCells(INT_MAX)
    , m_pSimBindSet(nullptr)
{
}

TressFXSDFCollision::~TressFXSDFCollision() {}

void TressFXSDFCollision::Initialize(EI_Device*              pDevice,
                                     TressFXSDFInputMeshInterface* pCollMesh,
                                     EI_StringHash modelName)
{
    m_pInputCollisionMesh = pCollMesh;

    // initialize SDF grid using the associated model's bounding box
    AMD::tressfx_vec3 bmin, bmax;
    m_pInputCollisionMesh->GetBoundingBox(bmin, bmax);
    m_CellSize               = (bmax.x - bmin.x) / m_NumCellsInXAxis;
    int numExtraPaddingCells = (int)(0.8f * (float)m_NumCellsInXAxis);
    m_PaddingBoundary = numExtraPaddingCells * m_CellSize, numExtraPaddingCells * m_CellSize,
    numExtraPaddingCells * m_CellSize;

    UpdateSDFGrid(bmin, bmax);

    bmin -= m_PaddingBoundary;
    bmax += m_PaddingBoundary;

    m_NumCellsX = (int)((bmax.x - bmin.x) / m_CellSize);
    m_NumCellsY = (int)((bmax.y - bmin.y) / m_CellSize);
    m_NumCellsZ = (int)((bmax.z - bmin.z) / m_CellSize);
    m_NumTotalCells =
        TressFXMin(m_NumTotalCells,
              (int)(m_GridAllocationMutliplier * m_NumCellsX * m_NumCellsY * m_NumCellsZ));

    // UAV
    m_SignedDistanceFieldUAV =
        EI_CreateReadWriteSB(pDevice, sizeof(int), m_NumTotalCells, TRESSFX_STRING_HASH("SDF"), modelName);

    // Bind set
    TressFXBindSet bindSet;

    EI_UAV      UAVs[2];
    EI_SRV      SRVs[1];

    UAVs[0] = EI_GetUAV(m_SignedDistanceFieldUAV);
    UAVs[1] = EI_GetUAV(m_pInputCollisionMesh->GetMeshBuffer());

    SRVs[0] = EI_GetSRV(m_pInputCollisionMesh->GetTrimeshVertexIndicesBuffer());

    bindSet.nSRVs  = AMD_ARRAY_SIZE(SRVs);
    bindSet.nUAVs  = AMD_ARRAY_SIZE(UAVs);
    bindSet.uavs   = UAVs;
    bindSet.srvs   = SRVs;
    bindSet.values = &(m_ConstBuffer);
    bindSet.nBytes = sizeof(TressFXSDFCollisionConstantBuffer);
    m_pSimBindSet   = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXSDFCollision::UpdateSDFGrid(const AMD::tressfx_vec3& tight_bbox_min,
                                        const AMD::tressfx_vec3& tight_bbox_max)
{
    AMD::tressfx_vec3 bmin = tight_bbox_min - m_PaddingBoundary;
    m_Origin               = bmin;
}

void TressFXSDFCollision::Update(EI_CommandContextRef          commandContext,
                                 TressFXSDFCollisionSystem& system)
{
    if (!m_pInputCollisionMesh)
        return;

    // Update the grid data based on the current bounding box
    AMD::tressfx_vec3 min, max;
    m_pInputCollisionMesh->GetBoundingBox(min, max);
    UpdateSDFGrid(min, max);

    // Set the constant buffer parameters
    m_ConstBuffer.m_Origin.x  = m_Origin.x;
    m_ConstBuffer.m_Origin.y  = m_Origin.y;
    m_ConstBuffer.m_Origin.z  = m_Origin.z;
    m_ConstBuffer.m_Origin.w  = 0;
    m_ConstBuffer.m_CellSize  = m_CellSize;
    m_ConstBuffer.m_NumCellsX = m_NumCellsX;
    m_ConstBuffer.m_NumCellsY = m_NumCellsY;
    m_ConstBuffer.m_NumCellsZ = m_NumCellsZ;

    // Binding UAVs, SRVs and CBs
    EI_Bind(commandContext, GetGenerateSDFLayout(), *m_pSimBindSet);

    // Run InitializeSignedDistanceField. One thread per one cell.
    {
        int numDispatchSize =
            (int)ceil((float)m_NumTotalCells / (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
        EI_Dispatch(commandContext, *(system.m_InitializeSignedDistanceFieldPSO), numDispatchSize);
    }


    EI_Barrier uavMeshAndSDF[] = 
    {
        { & (m_pInputCollisionMesh->GetMeshBuffer()), EI_STATE_UAV, EI_STATE_UAV },
        { m_SignedDistanceFieldUAV, EI_STATE_UAV, EI_STATE_UAV }
    };

    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(uavMeshAndSDF), uavMeshAndSDF);

    // Run ConstructSignedDistanceField. One thread per each triangle
    {
        int numDispatchSize = (int)ceil((float)m_pInputCollisionMesh->GetNumMeshTriangle() /
                                        (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
        EI_Dispatch(commandContext, *(system.m_ConstructSignedDistanceFieldPSO), numDispatchSize);
    }

    // State transition for DX12
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(uavMeshAndSDF), uavMeshAndSDF);

    // Run FinalizeSignedDistanceField. One thread per each triangle
    {
        int numDispatchSize =
            (int)ceil((float)m_NumTotalCells / (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
        EI_Dispatch(commandContext, *(system.m_FinalizeSignedDistanceFieldPSO), numDispatchSize);
    }

    // State transition for DX12
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(uavMeshAndSDF), uavMeshAndSDF);
}

void TressFXSDFCollision::CollideWithHair(EI_CommandContextRef          commandContext,
                                          TressFXSDFCollisionSystem& system,
                                          TressFXHairObject&         hairObject)
{
    if (!m_pInputCollisionMesh)
        return;

    int numTotalHairVertices = hairObject.GetNumTotalHairVertices();

    // Get vertex buffers from the hair object.
    PosTanCollection& collection = hairObject.GetPosTanCollection();

    // Set the constant buffer parameters
    m_ConstBuffer.m_Origin.x                 = m_Origin.x;
    m_ConstBuffer.m_Origin.y                 = m_Origin.y;
    m_ConstBuffer.m_Origin.z                 = m_Origin.z;
    m_ConstBuffer.m_Origin.w                 = 0;
    m_ConstBuffer.m_CellSize                 = m_CellSize;
    m_ConstBuffer.m_NumCellsX                = m_NumCellsX;
    m_ConstBuffer.m_NumCellsY                = m_NumCellsY;
    m_ConstBuffer.m_NumCellsZ                = m_NumCellsZ;
    m_ConstBuffer.m_CollisionMargin          = m_CollisionMargin * m_CellSize;
    m_ConstBuffer.m_NumTotalHairVertices     = hairObject.GetNumTotalHairVertices();
    m_ConstBuffer.m_NumHairVerticesPerStrand = hairObject.GetNumVerticesPerStrand();

    // Binding UAVs
    EI_Bind(commandContext, GetApplySDFLayout(), collection.GetApplySDFBindSet());
    EI_Bind(commandContext, GetGenerateSDFLayout(), *m_pSimBindSet);

    // Run CollideHairVerticesWithSdf. One thread per one hair vertex.
    {
        int numDispatchSize =
            (int)ceil((float)numTotalHairVertices / (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
        EI_Dispatch(commandContext, *(system.m_CollideHairVerticesWithSdfPSO), numDispatchSize);
    }

    // State transition for DX12
    collection.UAVBarrier(commandContext);

    //EI_SB_Transition(commandContext, m_SignedDistanceFieldUAV, EI_STATE_UAV, EI_STATE_UAV);
    EI_Barrier uavSDF[] = 
    {
        { m_SignedDistanceFieldUAV, EI_STATE_UAV, EI_STATE_UAV }
    };
    EI_SubmitBarriers(commandContext, 1, uavSDF);
}

void TressFXSDFCollision::Destroy(EI_Device* pDevice)
{
    EI_DestroyBindSet(pDevice, m_pSimBindSet);
    EI_Destroy(pDevice, m_SignedDistanceFieldUAV);
}

