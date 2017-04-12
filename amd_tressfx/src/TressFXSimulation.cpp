// ----------------------------------------------------------------------------
// Invokes simulation compute shaders.
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

#include "AMD_TressFX.h"
#include "TressFXSimulation.h"

// TressFX
#include "TressFXLayouts.h"
#include "TressFXHairObject.h"

void TressFXSimulation::Initialize(EI_Device* pDevice, EI_LayoutManagerRef simLayoutManager)
{
    mVelocityShockPropagationPSO = EI_CreateComputeShaderPSO(pDevice, simLayoutManager, TRESSFX_STRING_HASH("VelocityShockPropagation"));
    mIntegrationAndGlobalShapeConstraintsPSO = EI_CreateComputeShaderPSO(pDevice, simLayoutManager, TRESSFX_STRING_HASH("IntegrationAndGlobalShapeConstraints"));
    mLocalShapeConstraintsPSO = EI_CreateComputeShaderPSO(pDevice, simLayoutManager, TRESSFX_STRING_HASH("LocalShapeConstraints"));
    mLengthConstriantsWindAndCollisionPSO = EI_CreateComputeShaderPSO(pDevice, simLayoutManager, TRESSFX_STRING_HASH("LengthConstriantsWindAndCollision"));
    mUpdateFollowHairVerticesPSO = EI_CreateComputeShaderPSO(pDevice, simLayoutManager, TRESSFX_STRING_HASH("UpdateFollowHairVertices"));
}

void TressFXSimulation::Shutdown(EI_Device* pDevice)
{
    EI_DestroyPSO(pDevice, mVelocityShockPropagationPSO);
    EI_DestroyPSO(pDevice, mIntegrationAndGlobalShapeConstraintsPSO);
    EI_DestroyPSO(pDevice, mLocalShapeConstraintsPSO);
    EI_DestroyPSO(pDevice, mLengthConstriantsWindAndCollisionPSO);
    EI_DestroyPSO(pDevice, mUpdateFollowHairVerticesPSO);
}

void TressFXSimulation::Simulate(EI_CommandContextRef commandContext, TressFXHairObject& hairObject)
{
    // For dispatching one thread per one vertex
    int numOfGroupsForCS_VertexLevel = (int)((float)hairObject.m_NumTotalVertice / (float)TRESSFX_SIM_THREAD_GROUP_SIZE);

    // For dispatching one thread per one strand
    int numOfGroupsForCS_StrandLevel = (int)(((float)(hairObject.m_NumTotalStrands) / (float)TRESSFX_SIM_THREAD_GROUP_SIZE));

    // Binding
    EI_Bind(commandContext, GetSimPosTanLayout(), hairObject.mPosTanCollection.GetSimBindSet());
    EI_Bind(commandContext, GetSimLayout(), *hairObject.m_pSimBindSet);

    // IntegrationAndGlobalShapeConstraints
    {
        EI_Dispatch(commandContext, *mIntegrationAndGlobalShapeConstraintsPSO, numOfGroupsForCS_VertexLevel);
        hairObject.mPosTanCollection.UAVBarrier(commandContext);
    }

    // VelocityShockPropagation
    {
        EI_Dispatch(commandContext, *mVelocityShockPropagationPSO, numOfGroupsForCS_StrandLevel);
        hairObject.mPosTanCollection.UAVBarrier(commandContext);
    }

    // LocalShapeConstraintsWithIteration
    for (int i = 0; i < hairObject.m_CPULocalShapeIterations; i++)
    {
        EI_Dispatch(commandContext, *mLocalShapeConstraintsPSO, numOfGroupsForCS_StrandLevel);
        hairObject.mPosTanCollection.UAVBarrier(commandContext);
    }

    // LengthConstriantsWindAndCollision
    {
        EI_Dispatch(commandContext, *mLengthConstriantsWindAndCollisionPSO, numOfGroupsForCS_VertexLevel);
        hairObject.mPosTanCollection.UAVBarrier(commandContext);
    }

    // UpdateFollowHairVertices
    {
        EI_Dispatch(commandContext, *mUpdateFollowHairVerticesPSO, numOfGroupsForCS_VertexLevel);
        hairObject.mPosTanCollection.UAVBarrier(commandContext);
    }

    hairObject.mSimulationFrame++;
}

