// ----------------------------------------------------------------------------
// Example implementatino of callback functions for graphics.  
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

#pragma once

#include "AMD_TressFX.h"
#include "TressFXGPUInterface.h"

#include "SuCommandList.h"
#include "SuGPUIndexBuffer.h"
#include "SuGPUResourceManager.h"
#include "SuGPUStructuredBuffer.h"
#include "SuGPUTexture2DArray.h"
#include "SuTextureSlot.h"
#include "SuTextureSlotBinder.h"
#include "SuUAVSlot.h"
#include "SuUAVSlotBinder.h"
#include "SuEffect.h"

// For timers
#include "SuRenderManager.h"

extern "C"
{
    EI_BindLayout* SuCreateLayout(EI_Device* pDevice, EI_LayoutManagerRef layoutManager, const AMD::TressFXLayoutDescription& description);
    void SuDestroyLayout(EI_Device* pDevice, EI_BindLayout* pLayout);

    // Creates a structured buffer and srv (StructuredBuffer). It necessarily needs data to start, so
    // begin state should be EI_STATE_COPY_DEST.
    EI_StructuredBuffer* SuCreateReadOnlySB(EI_Device* pDevice,
        const AMD::uint32 structSize,
        const AMD::uint32 structCount,
        EI_StringHash  resourceName,
        EI_StringHash  objectName);

    // Creates a structured buffer and default UAV/SRV (StructuredBuffer and RWStructuredBuffer in
    // HLSL).  It will be used as UAV, so begin state should be EI_STATE_UAV.
    EI_StructuredBuffer* SuCreateReadWriteSB(EI_Device* pDevice,
        const AMD::uint32 structSize,
        const AMD::uint32 structCount,
        EI_StringHash  resourceName,
        EI_StringHash  objectName);

    // Same as EI_CreateReadWriteSB, but also includes an atomic counter.  Atomic counter is cleared to
    // zero each frame (EI_SB_ClearCounter)
    EI_StructuredBuffer* SuCreateCountedSB(EI_Device* pDevice,
        const AMD::uint32 structSize,
        const AMD::uint32 structCount,
        EI_StringHash  resourceName,
        EI_StringHash  objectName);

    void SuDestroy(EI_Device* pDevice, EI_Resource* pResource);



    void SuClearCounter(EI_CommandContextRef    pContext,
        EI_StructuredBufferRef sb,
        AMD::uint32          clearValue);

    void SuCopy(EI_CommandContextRef pContext, EI_StructuredBuffer& from, EI_StructuredBuffer& to);
    void* SuMap(EI_CommandContextRef pContext, EI_StructuredBuffer& sb);

    bool SuUnmap(EI_CommandContextRef pContext, EI_StructuredBuffer& sb);

    EI_Resource* SuCreate2D(EI_Device* pContext,
        const size_t     width,
        const size_t     height,
        const size_t     arraySize,
        EI_StringHash    strHash);

    // Used by Shortcut.
    EI_Resource* SuCreateRT(EI_Device* pContext,
        const size_t     width,
        const size_t     height,
        const size_t     channels,
        EI_StringHash    strHash,
        float            clearR,
        float            clearG,
        float            clearB,
        float            clearA);


    void SuClear2D(EI_CommandContext* pContext, EI_Resource* pResource, AMD::uint32 clearValue);


    void SuSubmitBarriers(EI_CommandContextRef commands,
        int numBarriers,
        AMD::EI_Barrier* barriers);

    EI_BindSet* SuCreateBindSet(EI_Device* commandContext, AMD::TressFXBindSet& bindSet);
    void SuDestroyBindSet(EI_Device* pDevice, EI_BindSet* pBindSet);
    void SuBind(EI_CommandContextRef commandContext, EI_BindLayout* pLayout, EI_BindSet& set);

    EI_PSO* SuCreateComputeShaderPSO(EI_Device* pDevice, EI_LayoutManagerRef layoutManager, const EI_StringHash& shaderName);
    void SuDestroyPSO(EI_Device* pDevice, EI_PSO* pso);
    void SuDispatch(EI_CommandContextRef commandContext, EI_PSO& pso, int nThreadGroups);

    EI_IndexBuffer* SuCreateIndexBuffer(EI_Device* context, AMD::uint32 indexCount, void* pInitialData, EI_StringHash objectName);
    void SuDestroyIB(EI_Device* pDevice, EI_IndexBuffer* pBuffer);
    void SuDrawIndexedInstanced(EI_CommandContextRef commandContext, EI_PSO& pso, AMD::EI_IndexedDrawParams& drawParams);
}


class EI_Resource
{
public:

    SuGPUResourcePtr resource;
    SuGPUUnorderedAccessViewPtr    uav;
    SuGPUSamplingResourceViewPtr   srv;
    SuGPURenderableResourceViewPtr rtv;
};





struct EI_BindLayout
{
    SuArray<const SuTextureSlot*> srvs;
    SuArray<const SuUAVSlot*> uavs;
    SuArray<SuEffectParameter*> constants;
};

#define EI_FORWARD_DECLARED 1



inline EI_Device* GetDevice()
{
    return (EI_Device*)SuGPUResourceManager::GetPtr();;
}

inline EI_CommandContextRef GetContext()
{
    return (EI_CommandContextRef)SuRenderManager::GetRef().GetCurrentCommandList();
}

inline EI_LayoutManagerRef GetLayoutManagerRef(SuEffectPtr& pEffect)
{
    return (EI_LayoutManagerRef&)pEffect;
}

inline EI_PSO* GetPSOPtr(const SuEffectTechnique* pTechnique)
{
    return (EI_PSO*)pTechnique;
}

inline SuEffectTechnique& GetTechniqueRef(EI_PSO& pso)
{
    return (SuEffectTechnique&)pso;
}

EI_PSO* GetPSO(const char* techniqueName, SuEffect* pEffect);


