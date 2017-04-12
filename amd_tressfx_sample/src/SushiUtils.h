#pragma once

#include "SuTypes.h"

#include "SuCommandQueue.h"
#include "SuRenderManager.h"
#include "SuGPUResourceManager.h"

#ifdef SU_3D_API_D3D12
#define ENABLE_ASYNC_COMPUTE 1
#else
#define ENABLE_ASYNC_COMPUTE 0
#endif



class AsyncControl
{
public:
    AsyncControl();
    ~AsyncControl() {} // Everything that matters is refcounted.
    void WaitForCompute();
    //void SubmitComputeCommandList();

    void SignalSimStart();
    SuCommandListPtr GetComputeCommandList();
    void WaitForLastFrameHairRenders();
    void SubmitSimCommandList(SuCommandListPtr pRenderCommandList);
    void RestoreRenderAsDefaultQueue(SuCommandListPtr pRenderCommandList);

private:

#if ENABLE_ASYNC_COMPUTE
    SuCommandQueuePtr m_pComputeQueue;
    SuFencePtr        m_pSimulateDoneFence;
    uint64            m_pSimulateDoneFenceValue;
    SuFencePtr        m_pSimulateStartFence;
    uint64            m_pSimulateStartFenceValue;
#endif

};


// converting types.

class SuEffectTechnique;
class EI_PSO;

class FullscreenPass
{
public:
    FullscreenPass();
    ~FullscreenPass();

    void Draw(SuEffectTechnique* pTechnique);
    void Draw(EI_PSO* pPSO) { return Draw((SuEffectTechnique*)pPSO); }

private:
    // quad vertex buffer
    SuGPUVertexBufferPtr m_QuadVertexBuffer;
};



void UnbindUAVS();

class SuObject;
void SuSetLightForObject(SuObject* pObject);

#ifndef GPU_TIMER_START

#define GPU_TIMER_START2( v, s ) \
  SuGPUTimerHandle v = SuRenderManager::GetRef().CreateGPUTimer( #s );


#define GPU_TIMER_START(x) SuRenderManager::GetRef().StartGPUTimer(x);
#define GPU_TIMER_STOP(x) SuRenderManager::GetRef().StopGPUTimer(x);

#endif


