// ----------------------------------------------------------------------------
// Interface for TressFX OIT using per-pixel linked lists.
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

#include <stdint.h>

//#include "AMD_TressFX.h"

#include "TressFXEngineInterface.h"
#include "TressFXGPUInterface.h"
#include "TressFXLayouts.h"
#include "TressFXPPLL.h"

using namespace AMD;

TressFXPPLL::TressFXPPLL()
    : m_nScreenWidth(0)
    , m_nScreenHeight(0)
    , m_nNodes(0)
    , m_nNodeSize(0)
    , m_bCreated(false)
    , m_pHeads()
    , m_pPPLLBuildBindSet(nullptr)
    , m_pPPLLReadBindSet(nullptr)
    , m_pNodes()
{
}

TressFXPPLL::~TressFXPPLL()
{
    TRESSFX_ASSERT(!m_bCreated);
    Destroy(nullptr);
}

bool TressFXPPLL::Create(
    EI_Device* pDevice, int width, int height, int nNodes, int nodeSize)
{
    TRESSFX_ASSERT(!m_bCreated);

    m_nNodes        = nNodes;
    m_nNodeSize     = nodeSize;
    m_nScreenWidth  = width;
    m_nScreenHeight = height;

    m_pHeads = EI_RW2D_Create(pDevice, width, height, 1, TRESSFX_STRING_HASH("PPLLHeads"));
    m_pNodes = EI_CreateCountedSB(pDevice, nodeSize, nNodes, TRESSFX_STRING_HASH("PPLLNodes"), TRESSFX_STRING_HASH("Global"));

    CreateBuildBindSet(pDevice);
    CreateReadBindSet(pDevice);

    m_bCreated = true;

    return true;
}

// return the set?
void TressFXPPLL::CreateBuildBindSet(EI_Device* pDevice)
{
    EI_Resource* uavs[2];

    uavs[TRESSFX_IDUAV_PPLL_HEADS] = m_pHeads;
    uavs[TRESSFX_IDUAV_PPLL_NODES] = m_pNodes;

    TressFXBindSet bindSet;
    // For now, none will have values.
    bindSet.nBytes = 0;
    bindSet.values = 0;

    bindSet.nUAVs     = AMD_ARRAY_SIZE(uavs);
    bindSet.uavs = uavs;
    bindSet.nSRVs     = 0;
    m_pPPLLBuildBindSet = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXPPLL::CreateReadBindSet(EI_Device* pDevice)
{
    EI_Resource* srvs[2];

    srvs[TRESSFX_IDSRV_PPLL_HEADS] = m_pHeads;
    srvs[TRESSFX_IDSRV_PPLL_NODES] = m_pNodes;

    TressFXBindSet bindSet;
    // For now, none will have values.
    bindSet.nBytes = 0;
    bindSet.values = 0;


    bindSet.nSRVs    = AMD_ARRAY_SIZE(srvs);
    bindSet.srvs     = srvs;
    bindSet.nUAVs    = 0;
    m_pPPLLReadBindSet = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXPPLL::Destroy(EI_Device* context)
{
    if (m_bCreated)
    {
        EI_DestroyBindSet(context, m_pPPLLBuildBindSet);
        EI_DestroyBindSet(context, m_pPPLLReadBindSet);

        EI_Destroy(context, m_pHeads);
        EI_SB_Destroy(context, m_pNodes);
        //  EI_DestroyBindSet(context, mBindSetForBuilding);
        // EI_DestroyBindSet(context, mBindSetForReading);
    }
    m_bCreated = false;
}


void TressFXPPLL::Clear(EI_CommandContextRef context)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("PPLL cleared without being created.");
    }
    // Consider being explicit on values here.
    // In DX, UAV counter clears are actually done when UAV is set, which makes this a bit weird, I
    // suppose.
    // Also try clearing in shader during fullscreen pass (although can't miss frames, and makes
    // reads slower)
    // or look at 0-based for fast clears.

    EI_RW2D_Clear(&context, m_pHeads, TRESSFX_PPLL_NULL_PTR);

    // Really, this should be a upload 0 with transitions around it.
    EI_SB_ClearCounter(context, *m_pNodes, 0);
}

void TressFXPPLL::DoneBuilding(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("PPLL DoneBuilding without being created.");
    }

    EI_Barrier writeToRead[] = 
    {
        { m_pHeads, EI_STATE_UAV, EI_STATE_PS_SRV },
        { m_pNodes, EI_STATE_UAV, EI_STATE_PS_SRV }
    };

    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(writeToRead), writeToRead);
}

void TressFXPPLL::DoneReading(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("PPLL DoneReading without being created.");
    }

    EI_Barrier readToWrite[] = 
    {
        { m_pHeads, EI_STATE_PS_SRV, EI_STATE_UAV },
        { m_pNodes, EI_STATE_PS_SRV, EI_STATE_UAV }
    };

    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(readToWrite), readToWrite);
}

void TressFXPPLL::BindForBuild(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("PPLL BindForBuild without being created.");
    }
    EI_Bind(commandContext, GetPPLLBuildLayout(), *m_pPPLLBuildBindSet);
}

void TressFXPPLL::BindForRead(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("PPLL BindForRead without being created.");
    }
    EI_Bind(commandContext, GetPPLLReadLayout(), *m_pPPLLReadBindSet);
}

