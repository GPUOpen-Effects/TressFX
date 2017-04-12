
//---------------------------------------------------------------------------------------
// Example code for the per-pixel linked list version of TressFX OIT. 
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


#include "PPLLExample.h"

#include "AMD_TressFX.h"
#include "TressFXLayouts.h"
#include "TressFXPPLL.h"

#include "SushiGPUInterface.h"

#include "HairStrands.h"
#include "TressFXHairObject.h"

#include "SushiUtils.h"


PPLLExample::PPLLExample() : m_pPPLL(nullptr)
{
    m_pFullscreenPass = new FullscreenPass;
}

void PPLLExample::Initialize(int width, int height, int nNodes, int nodeSize, SuEffect* pStrandEffect, SuEffect* pQuadEffect)
{
    SU_ASSERT(m_pPPLL == nullptr);

    if (m_pPPLL == nullptr)
    {
        m_pPPLL = new TressFXPPLL;
    }

    m_pBuildPSO = GetPSO("TressFX2",pStrandEffect);
    m_pReadPSO = GetPSO("TressFX2", pQuadEffect);

    m_pPPLL->Create(GetDevice(), width, height, nNodes, nodeSize);
}
void PPLLExample::Shutdown(EI_Device* pDevice)
{
    if (m_pPPLL)
    {
        m_pPPLL->Destroy(pDevice);
        delete m_pPPLL;
        m_pPPLL = nullptr;
    }
}
PPLLExample::~PPLLExample()
{
    SU_ASSERT(m_pPPLL == nullptr);
    delete m_pFullscreenPass;
}

void PPLLExample::Draw(std::vector<HairStrands*>& hairStrands)
{

    EI_CommandContextRef commandContext = GetContext();

    SuGPUTimerHandle fillTimer = SuRenderManager::GetRef().CreateGPUTimer("render: PPLL Fill");
    SuGPUTimerHandle resolveTimer = SuRenderManager::GetRef().CreateGPUTimer("render: PPLL Resolve");

    m_pPPLL->Clear(commandContext);

    m_pPPLL->BindForBuild(commandContext);  // put clear in here?

    for (size_t i = 0; i < hairStrands.size(); i++)
    {
        TressFXHairObject* pHair = hairStrands[i]->GetTressFXHandle();

        if (pHair)
        {
            GPU_TIMER_START(fillTimer)
                pHair->DrawStrands(commandContext, *m_pBuildPSO);
            GPU_TIMER_STOP(fillTimer)
        }
    }

    m_pPPLL->DoneBuilding(commandContext);


    // TODO move this to a clear "after all pos and tan usage by rendering" place.
    for (size_t i = 0; i < hairStrands.size(); i++)
        hairStrands[i]->TransitionRenderingToSim(commandContext);

    UnbindUAVS();  // Engine-required hack to unbind UAVs.


    m_pPPLL->BindForRead(commandContext);

    // Enables lights.  Only lights marked to hit TressFX will be applied.
    if (hairStrands.size() > 0)
    {
        SuSetLightForObject(hairStrands[0]->GetAsEngineObject());

        GPU_TIMER_START(resolveTimer)
            m_pFullscreenPass->Draw(m_pReadPSO);
        GPU_TIMER_STOP(resolveTimer)

        SuSetLightForObject(0);
    }

    m_pPPLL->DoneReading(commandContext);

}