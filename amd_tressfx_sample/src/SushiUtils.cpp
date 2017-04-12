
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

#include "SushiUtils.h"

#include "SuObjectManager.h"
#include "SuVector2.h"
#include "SuGPUResourceManager.h"
#include "SuEffect.h"
#include "SuEffectTechnique.h"

AsyncControl::AsyncControl()
{
#if ENABLE_ASYNC_COMPUTE
    m_pComputeQueue = SuRenderManager::GetRef().CreateCommandQueue(SuCommandList::COMPUTE);
    m_pSimulateDoneFenceValue = 0;
    m_pSimulateDoneFence = SuRenderManager::GetRef().CreateFence(m_pSimulateDoneFenceValue);
    m_pSimulateDoneFenceValue++;
    m_pSimulateStartFenceValue = 0;
    m_pSimulateStartFence = SuRenderManager::GetRef().CreateFence(m_pSimulateStartFenceValue);
    m_pSimulateStartFenceValue++;
#endif
}



void AsyncControl::SignalSimStart()
{
#if ENABLE_ASYNC_COMPUTE
    SuCommandQueuePtr frameCommandQueue = SuRenderManager::GetRef().GetFrameCommandQueue();
    frameCommandQueue->Signal(m_pSimulateStartFence, m_pSimulateStartFenceValue);
#endif
}

void  AsyncControl::WaitForCompute()
{
#if ENABLE_ASYNC_COMPUTE
    SuRenderManager::GetRef().CloseAndAppendCommandList(SuRenderManager::GetRef().GetCurrentCommandList());
    SuRenderManager::GetRef().ExecuteCommandLists();
    SuRenderManager::GetRef().CreateCommandList();

    SuCommandQueuePtr frameCommandQueue = SuRenderManager::GetRef().GetFrameCommandQueue();
    frameCommandQueue->Wait(m_pSimulateDoneFence, m_pSimulateDoneFenceValue);
    m_pSimulateDoneFenceValue++;
#endif
}

SuCommandListPtr  AsyncControl::GetComputeCommandList()
{
#if ENABLE_ASYNC_COMPUTE

    // Create command list for simulation dispatches
    return SuRenderManager::GetRef().CreateCommandList(SuCommandList::COMPUTE);
#else
    return SuRenderManager::GetRef().GetCurrentCommandList();
#endif

}


void AsyncControl::WaitForLastFrameHairRenders()
{
#if ENABLE_ASYNC_COMPUTE
    // Have compute work wait for signal from graphics queue to start
    m_pComputeQueue->Wait(m_pSimulateStartFence, m_pSimulateStartFenceValue);
    m_pSimulateStartFenceValue++;
#endif
}

void AsyncControl::SubmitSimCommandList(SuCommandListPtr pSimulateCommandList)
{
#if ENABLE_ASYNC_COMPUTE
    // Submit simulate commands to compute queue, and have queue signal when done.
    pSimulateCommandList->Close();
    m_pComputeQueue->ExecuteCommandList(pSimulateCommandList);
    m_pComputeQueue->Signal(m_pSimulateDoneFence, m_pSimulateDoneFenceValue);
#endif
}
void  AsyncControl::RestoreRenderAsDefaultQueue(SuCommandListPtr pRenderCommandList)
{
    // Restore drawing command list.
    SuRenderManager::GetRef().SetCurrentCommandList(pRenderCommandList);
}


//void  AsyncControl::SubmitComputeCommandList()
//{
//    SuCommandListPtr pRenderCommandList = SuRenderManager::GetRef().GetCurrentCommandList();
//
//#if ENABLE_ASYNC_COMPUTE
//    // Have compute work wait for signal from graphics queue to start
//    m_pComputeQueue->Wait(m_pSimulateStartFence, m_pSimulateStartFenceValue);
//    m_pSimulateStartFenceValue++;
//
//    // Submit simulate commands to compute queue, and have queue signal when done.
//    m_pSimulateCommandList->Close();
//    m_pComputeQueue->ExecuteCommandList(m_pSimulateCommandList);
//    m_pComputeQueue->Signal(m_pSimulateDoneFence, m_pSimulateDoneFenceValue);
//
//    // Restore drawing command list.
//    SuRenderManager::GetRef().SetCurrentCommandList(pRenderCommandList);
//#endif
//}






FullscreenPass::FullscreenPass()
{
    // Create quad vertex buffer.
    SuMemoryBufferPtr quadVertBuffer(SuMemoryBuffer::Allocate(sizeof(SuVector2) * 4));
    quadVertBuffer->Set(0, 0, SuVector2(-1.0, -1.0));
    quadVertBuffer->Set(0, 1, SuVector2(1.0, -1.0));
    quadVertBuffer->Set(0, 2, SuVector2(-1.0, 1.0));
    quadVertBuffer->Set(0, 3, SuVector2(1.0, 1.0));
    m_QuadVertexBuffer =
        SuGPUResourceManager::GetRef().CreateResourceVertex(SuGPUResource::GPU_RESOURCE_STATIC,
            sizeof(SuVector2),
            4,
            quadVertBuffer,
            SuGPUResource::BIND_VERTEX_BUFFER);
}

FullscreenPass::~FullscreenPass() {}


void FullscreenPass::Draw(SuEffectTechnique* pTechnique)
{
    // Local hack
    //SuEffectTechnique& technique = GetTechniqueRef(pso);
    SuEffect* pEffect = const_cast<SuEffect*>(pTechnique->GetParent());
    if (pEffect)
    {
        pEffect->BindVertexBuffer("QuadStream", m_QuadVertexBuffer.get());

        uint32 numPasses;
        bool   techniqueFound = pEffect->Begin(pTechnique, &numPasses);
        if (techniqueFound)
        {
            for (uint32 i = 0; i < numPasses; ++i)
            {
                pEffect->BeginPass(i);
                SuRenderManager::GetRef().DrawNonIndexed(SuRenderManager::TRIANGLE_STRIP, 4, 0);
                pEffect->EndPass();
            }
            pEffect->End();
        }
    }
}



void UnbindUAVS()
{
    SuArray<SuGPURenderableResourceViewPtr> colorViews;
    SuGPUDepthStencilResourceViewPtr        pDepthView;
    SuRenderManager::GetRef().GetRenderTargets(&colorViews, &pDepthView);
    SuRenderManager::GetRef().SetRenderTargets(
        (uint32)colorViews.size(), colorViews.begin(), pDepthView);
}


void SuSetLightForObject(SuObject* pObject)
{
    SuObjectManager::GetRef().SetObjectBeingDrawn(pObject);
}






