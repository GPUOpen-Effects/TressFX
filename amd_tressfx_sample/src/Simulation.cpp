//---------------------------------------------------------------------------------------
// Example code for managing simulation
//-------------------------------------------------------------------------------------
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

#include "Simulation.h"

#include "AMD_TressFX.h"

#include "TressFXLayouts.h"
#include "TressFXSimulation.h"

#include "SushiGPUInterface.h"
#include "SuEffectManager.h"

#include "HairStrands.h"
#include "SDF.h"

#define DEFINE_TO_STRING(x) #x
#define DEFINE_VALUE_TO_STRING(x) DEFINE_TO_STRING(x)

Simulation::Simulation()
    : mSimulation()
    , mSDFCollisionSystem()
{
    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();

    SuEffectManager::GetRef().AddDefine(DEFINE_TO_STRING(AMD_TRESSFX_MAX_NUM_BONES), DEFINE_VALUE_TO_STRING(AMD_TRESSFX_MAX_NUM_BONES));
    m_pTressFXSimEffect = SuEffectManager::GetRef().LoadEffect("cTressFXSimulation.sufx");
    m_pTressFXSDFCollisionEffect = SuEffectManager::GetRef().LoadEffect("cTressFXSDFCollision.sufx");


    EI_LayoutManagerRef simLayoutManager = GetLayoutManagerRef(m_pTressFXSimEffect);
    CreateSimPosTanLayout2(pDevice, simLayoutManager);
    CreateSimLayout2(pDevice, simLayoutManager);

    EI_LayoutManagerRef applySDFManager = GetLayoutManagerRef(m_pTressFXSDFCollisionEffect);
    CreateApplySDFLayout2(pDevice, applySDFManager);

    EI_LayoutManagerRef sdfCollisionManager = GetLayoutManagerRef(m_pTressFXSDFCollisionEffect);
    CreateGenerateSDFLayout2(pDevice, sdfCollisionManager);


   // mSimulation = new TressFXSimulation();
    mSimulation.Initialize(pDevice, simLayoutManager);

    mSDFCollisionSystem.Initialize(pDevice, sdfCollisionManager);
}

Simulation::~Simulation()
{
    EI_Device* pDevice = GetDevice();

    mSimulation.Shutdown(pDevice);
    mSDFCollisionSystem.Shutdown(pDevice);


    m_pTressFXSimEffect = SuEffectPtr(0);
    m_pTressFXSDFCollisionEffect = SuEffectPtr(0);
}


void Simulation::StartSimulation(
    double fTime,
    std::vector<HairStrands*>& hairStrands,
    std::vector<CollisionMesh*>& collisionMeshes,
    bool bUpdateCollMesh, bool bSDFCollisionResponse
    )
{
    // When we are done submitting sim commands, we will restore this as the default command list.
    // TODO Maybe pass this explicitly, rather than setting as default and retrieving from there.
    SuCommandListPtr pRenderCommandList = SuRenderManager::GetRef().GetCurrentCommandList();

    // Signal from graphics queue that compute can start.
    // Must call before Simulate() and before submitting
    // graphics commands to overlap with compute.
    m_ComputeQueue.SignalSimStart();

    // When running async, we are getting a command list for submission on the async compute queue.
    // We are only accumulating commands for submission now.  There will be a wait for actual
    // submission after this.
    //EI_CommandContextRef simContext = (EI_CommandContextRef) m_ComputeQueue.GetComputeCommandList();

    SuCommandListPtr pSimCommandList = m_ComputeQueue.GetComputeCommandList();
    EI_CommandContextRef simContext = (EI_CommandContextRef)pSimCommandList;


    SuGPUTimerHandle simulationTotalTimer =
        SuRenderManager::GetRef().CreateGPUTimer("TRESSFX SIMULATION TOTAL");

    GPU_TIMER_START(simulationTotalTimer)

        if (bUpdateCollMesh)
        {
            // Updates the skinned version of the mesh,
            // which is input to SDF.
            // We are using a compute-based skinning system here, 
            // which is not part of the TressFX library.
            for (size_t i = 0; i < collisionMeshes.size(); i++)
                collisionMeshes[i]->SkinTheMesh(simContext, fTime);
            
            UpdateCollisionMesh(simContext, collisionMeshes);
        }

    RunSimulation(simContext, hairStrands);

    if (bSDFCollisionResponse)
        RunCollision(simContext, hairStrands, collisionMeshes);


    for (size_t i = 0; i < hairStrands.size(); i++)
        hairStrands[i]->TransitionSimToRendering(simContext);

    GPU_TIMER_STOP(simulationTotalTimer)

    // Have compute work wait for signal from graphics queue that we can start 
    // issuing the sim commands.

    m_ComputeQueue.WaitForLastFrameHairRenders();
    m_ComputeQueue.SubmitSimCommandList(pSimCommandList);
    m_ComputeQueue.RestoreRenderAsDefaultQueue(pRenderCommandList);


}

void Simulation::WaitOnSimulation()
{
    m_ComputeQueue.WaitForCompute();
}


void Simulation::UpdateCollisionMesh(EI_CommandContextRef simContext, std::vector<CollisionMesh*>& collisionMeshes)
{

    {
        SuGPUTimerHandle sdfTimer = SuRenderManager::GetRef().CreateGPUTimer("Generating SDF");

        GPU_TIMER_START(sdfTimer)
            for (size_t i = 0; i < collisionMeshes.size(); i++)
                collisionMeshes[i]->AccumulateSDF(simContext, mSDFCollisionSystem);
        GPU_TIMER_STOP(sdfTimer)
    }
}

void Simulation::RunCollision(EI_CommandContextRef simContext, std::vector<HairStrands*>& hairStrands, std::vector<CollisionMesh*>& collisionMeshes)
{
    SuGPUTimerHandle applySDFTimer =
        SuRenderManager::GetRef().CreateGPUTimer("Apply SDF");

    // We will apply every collision mesh to every set of strands.
    // This is of course not necessary in general - a BB check, for example, 
    // could check for overlaps first.

    GPU_TIMER_START(applySDFTimer)
        for (size_t i = 0; i < hairStrands.size(); i++)
        {
            TressFXHairHandle pHair = hairStrands[i]->GetTressFXHandle();

            for (size_t j = 0; j < collisionMeshes.size(); j++)
                collisionMeshes[j]->ApplySDF(simContext, mSDFCollisionSystem, pHair);
        }
    GPU_TIMER_STOP(applySDFTimer)
}


void Simulation::RunSimulation(EI_CommandContextRef simContext, std::vector<HairStrands*>& hairStrands)
{
    SuGPUTimerHandle simulationTimer =
        SuRenderManager::GetRef().CreateGPUTimer("Simulation");

    GPU_TIMER_START(simulationTimer)
        for (size_t i = 0; i < hairStrands.size(); i++)
        {
            // update bone matrices for bone skinning of first two vertices of hair strands
            hairStrands[i]->UpdateBones(simContext);

            // Run simulation
            hairStrands[i]->Simulate(simContext, &mSimulation);

        }
    GPU_TIMER_STOP(simulationTimer)

}


