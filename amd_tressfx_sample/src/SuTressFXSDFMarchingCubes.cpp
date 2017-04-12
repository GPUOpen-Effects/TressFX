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

#include "PCH.h"

#include "SushiGPUInterface.h"

#include "MarchingCubesTables.h"
#include "SuTressFXSDFMarchingCubes.h"
#include "TressFXSDFCollision.h"

#include "SuEffect.h"
#include "SuGPUIndexBuffer.h"
#include "SuGPUVertexBuffer.h"
#include "SuObjectManager.h"
#include "SuRenderManager.h"


SuTressFXSDFMarchingCubes::SuTressFXSDFMarchingCubes() 
    : m_MaxMarchingCubesVertices(128 * 1024)
    , m_MarchingCubesTriangleVerticesUAV(nullptr)
    , m_NumMarchingCubesVerticesUAV(nullptr)
    , m_MarchingCubesEdgeTableSRV(nullptr)
    , m_MarchingCubesTriangleTableSRV(nullptr)
{}

SuTressFXSDFMarchingCubes::~SuTressFXSDFMarchingCubes() {  }


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


void SuTressFXSDFMarchingCubes::Initialize(EI_StringHash name)
{
    // load an effect for rendering
    m_pRenderEffect = SuEffectManager::GetRef().LoadEffect(_T("oSDF.sufx"));

    // load a compute shader
    m_pComputeEffect = SuEffectManager::GetRef().LoadEffect("cTressFXMarchingCubes.sufx");

    EI_Device* pDevice = (EI_Device*) SuGPUResourceManager::GetPtr();


    AMD::uint32 vertexBlockSize = sizeof(VertexData);
    const int         NUM_ELEMENTS_EDGE_TABLE = 256;
    const int         NUM_ELEMENTS_TRI_TABLE = 256 * 16;


    // UAV
    //SuMemoryBufferPtr pMCVertexBuffer(SuMemoryBuffer::Allocate(
    //    (uint32)(vertexBlockSize * m_MaxMarchingCubesVertices)));  // position + normal

    m_MarchingCubesTriangleVerticesUAV = EI_CreateReadWriteSB(pDevice, vertexBlockSize, m_MaxMarchingCubesVertices, TRESSFX_STRING_HASH("MCTriVerts"), name);
    m_NumMarchingCubesVerticesUAV = EI_CreateReadWriteSB(pDevice, sizeof(int), 1, TRESSFX_STRING_HASH("NumMCVerts"), name);
    m_MarchingCubesEdgeTableSRV = EI_CreateReadOnlySB(pDevice, sizeof(int), NUM_ELEMENTS_EDGE_TABLE, TRESSFX_STRING_HASH("MCEdgeTable"), name);
    m_MarchingCubesTriangleTableSRV = EI_CreateReadOnlySB(pDevice, sizeof(int), NUM_ELEMENTS_TRI_TABLE, TRESSFX_STRING_HASH("MCTriTable"), name);

    //------------------
    // Initial data.
    //
    // Just need tables for generating marching cubes.
    // edge and triangle table should probably be combined at some point.
    //------------------

    EI_CommandContextRef commandContext = (EI_CommandContextRef) SuRenderManager::GetRef().GetCurrentCommandList();

    AMD::EI_Barrier uavToCopy[] = 
    {
        { m_MarchingCubesTriangleVerticesUAV , AMD::EI_STATE_UAV, AMD::EI_STATE_COPY_DEST},
        { m_NumMarchingCubesVerticesUAV, AMD::EI_STATE_UAV, AMD::EI_STATE_COPY_DEST }
    };
    SuSubmitBarriers(commandContext, AMD_ARRAY_SIZE(uavToCopy), uavToCopy);

    // write zero for the number of vertices.
    void* pDest = EI_SB_Map(commandContext, *m_NumMarchingCubesVerticesUAV);
    TRESSFX_ASSERT(pDest);
    static const int zero = 0;
    memcpy(pDest, &zero, sizeof(int) * 1);
    EI_SB_Unmap(commandContext, *m_NumMarchingCubesVerticesUAV);

    // Fill in the edge table.
    pDest = EI_SB_Map(commandContext, *m_MarchingCubesEdgeTableSRV);
    TRESSFX_ASSERT(pDest);
    memcpy(pDest, MARCHING_CUBES_EDGE_TABLE, sizeof(int) * NUM_ELEMENTS_EDGE_TABLE);
    EI_SB_Unmap(commandContext, *m_MarchingCubesEdgeTableSRV);

    // Fill in the edge table.
    pDest = EI_SB_Map(commandContext, *m_MarchingCubesTriangleTableSRV);
    TRESSFX_ASSERT(pDest);
    memcpy(pDest, MARCHING_CUBES_TRIANGLE_TABLE, sizeof(int) * NUM_ELEMENTS_TRI_TABLE);
    SuUnmap(commandContext, *m_MarchingCubesTriangleTableSRV);

    AMD::EI_Barrier uploadDone[] =
    {
        { m_MarchingCubesTriangleVerticesUAV , AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_UAV },
        { m_NumMarchingCubesVerticesUAV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_UAV },
        { m_MarchingCubesEdgeTableSRV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_NON_PS_SRV },
        { m_MarchingCubesTriangleTableSRV, AMD::EI_STATE_COPY_DEST, AMD::EI_STATE_NON_PS_SRV }
    };
    SuSubmitBarriers(commandContext, AMD_ARRAY_SIZE(uploadDone), uploadDone);


}


void SuTressFXSDFMarchingCubes::Destroy()
{
    EI_Device* pDevice = (EI_Device*) SuGPUResourceManager::GetPtr();

    AMD_SAFE_RESOURCE_DELETE(pDevice, m_MarchingCubesTriangleVerticesUAV);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_NumMarchingCubesVerticesUAV);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_MarchingCubesEdgeTableSRV);
    AMD_SAFE_RESOURCE_DELETE(pDevice, m_MarchingCubesTriangleTableSRV);

    m_pComputeEffect = SuEffectPtr(0);
    m_pRenderEffect  = SuEffectPtr(0);
}

void SuTressFXSDFMarchingCubes::Draw()
{
    //----------------------------
    // Draw marching cubes mesh
    //----------------------------
    {
        bool ok = true;
        m_pRenderEffect->GetParameter("cColor")->SetFloatVector(SuVector3(1.0f, 1.0f, 0.0f));
        m_pRenderEffect->GetParameter("vLightDir")->SetFloatVector(SuVector3(1.0f, 1.0f, 1.0f));

        m_pRenderEffect->GetTextureSlot("vertices")->BindResource(m_MarchingCubesTriangleVerticesUAV->srv);


        uint32 nNumPasses;
        if (m_pRenderEffect->Begin("MeshUAV", &nNumPasses))
        {
            SU_ASSERT(nNumPasses == 1);

            if (m_pRenderEffect->BeginPass(0))
            {
                SuRenderManager::GetRef().DrawNonIndexed(
                    SuRenderManager::TRIANGLE_LIST, m_MaxMarchingCubesVertices, 0);

                if (m_pRenderEffect->EndPass())
                    ok = true;
            }

            ok = m_pRenderEffect->End() && ok;  // always call end if begin succeeded.
        }

        SU_ASSERT(ok);
    }

}

void SuTressFXSDFMarchingCubes::DrawGrid()
{
    // Render SDF grid
    m_LineRenderer.SetColor(SuVector4(0.f, 0.f, 1.f, 0));

    for (int j = 0; j <= m_NumCellsY; ++j)
    {
        for (int k = 0; k <= m_NumCellsZ; ++k)
        {
            SuVector3 p0 = m_Origin + SuVector3(0, j * m_CellSize, k * m_CellSize);
            SuVector3 p1 = p0 + SuVector3(m_NumCellsX * m_CellSize, 0, 0);

            m_LineRenderer.DrawLine((SuPoint3&)p0, (SuPoint3&)p1);
        }

        for (int i = 0; i <= m_NumCellsX; ++i)
        {
            SuVector3 p0 = m_Origin + SuVector3(i * m_CellSize, j * m_CellSize, 0);
            SuVector3 p1 = p0 + SuVector3(0, 0, m_NumCellsZ * m_CellSize);

            m_LineRenderer.DrawLine((SuPoint3&)p0, (SuPoint3&)p1);
        }
    }
}

void SuTressFXSDFMarchingCubes::Update(EI_CommandContextRef commandContext)
{
    SU_ASSERT(m_pSDF);

    m_Origin =
        SuVector3(m_pSDF->GetGridOrigin().x, m_pSDF->GetGridOrigin().y, m_pSDF->GetGridOrigin().z);
    m_CellSize = m_pSDF->GetGridCellSize();
    m_pSDF->GetGridNumCells(m_NumCellsX, m_NumCellsY, m_NumCellsZ);
    m_NumTotalCells = m_pSDF->GetGridNumTotalCells();

    //-------------------------------
    // RunMarchingCubesOnSdf
    //-------------------------------
    {
        // Set the constant buffer parameters
        m_pComputeEffect->GetParameter("g_MaxMarchingCubesVertices")
            ->SetInt(m_MaxMarchingCubesVertices);
        m_pComputeEffect->GetParameter("g_MarchingCubesIsolevel")
            ->SetFloat(m_CellSize * m_SDFIsoLevel);

        // Set the constant buffer parameters
        m_pComputeEffect->GetParameter("g_Origin")->SetFloatVector(&m_Origin.x);
        m_pComputeEffect->GetParameter("g_CellSize")->SetFloat(m_CellSize);
        m_pComputeEffect->GetParameter("g_NumCellsX")->SetInt(m_NumCellsX);
        m_pComputeEffect->GetParameter("g_NumCellsY")->SetInt(m_NumCellsY);
        m_pComputeEffect->GetParameter("g_NumCellsZ")->SetInt(m_NumCellsZ);

        // Binding UAVs
        EI_StructuredBuffer& sdfDataBuffer = m_pSDF->GetSDFDataGPUBuffer();
        m_pComputeEffect->GetUAVSlot("g_SignedDistanceField")->BindResource(sdfDataBuffer.uav);

        m_pComputeEffect->GetUAVSlot("g_MarchingCubesTriangleVertices")->BindResource(m_MarchingCubesTriangleVerticesUAV->uav);
        m_pComputeEffect->GetUAVSlot("g_NumMarchingCubesVertices")->BindResource(m_NumMarchingCubesVerticesUAV->uav);



        // Binding SRVs
        // These should always be true, so shoudl be able to move to initialization.

        m_pComputeEffect->GetTextureSlot("g_MarchingCubesEdgeTable")->BindResource(m_MarchingCubesEdgeTableSRV->srv);
        m_pComputeEffect->GetTextureSlot("g_MarchingCubesTriangleTable")->BindResource(m_MarchingCubesTriangleTableSRV->srv);



        // Run InitializeMCVertices. One thread per each cell
        {
            int numDispatchSize = (int)ceil((float)m_MaxMarchingCubesVertices /
                                            (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
            uint32   numPasses;
            SuString techniqueName = "TressFXCollision_InitializeMCVertices";
            m_pComputeEffect->Begin(techniqueName, &numPasses);
            m_pComputeEffect->BeginPass(0);
            SuRenderManager::GetRef().Dispatch(numDispatchSize, 1, 1);
            m_pComputeEffect->EndPass();
            m_pComputeEffect->End();
        }

        // Run RunMarchingCubesOnSdf. One thread per each cell
        {
            int numDispatchSize =
                (int)ceil((float)m_NumTotalCells / (float)TRESSFX_SIM_THREAD_GROUP_SIZE);
            uint32   numPasses;
            SuString techniqueName = "TressFXCollision_RunMarchingCubesOnSdf";
            m_pComputeEffect->Begin(techniqueName, &numPasses);
            m_pComputeEffect->BeginPass(0);
            SuRenderManager::GetRef().Dispatch(numDispatchSize, 1, 1);
            m_pComputeEffect->EndPass();
            m_pComputeEffect->End();
        }

        // State transition for DX12
        // Although this could probably be removed.
        // Next transition is when rendering.
        AMD::EI_Barrier uavs[] =
        {
            { &sdfDataBuffer, AMD::EI_STATE_UAV, AMD::EI_STATE_UAV },
            { m_MarchingCubesTriangleVerticesUAV, AMD::EI_STATE_UAV, AMD::EI_STATE_UAV },
            { m_NumMarchingCubesVerticesUAV, AMD::EI_STATE_UAV, AMD::EI_STATE_UAV },
        };
        SuSubmitBarriers(commandContext, AMD_ARRAY_SIZE(uavs), uavs);

    }
}

