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

#include "AMD_TressFX.h"
#include "SushiGPUInterface.h"

#include "SuEffect.h"
#include "SuEffectParameter.h"
#include "SuEffectTechnique.h"

#include "SuGPUIndexBuffer.h"

#include "SuRenderManager.h"

// for fullscreen.
#include "SuVector2.h"
#include "SuGPUVertexBuffer.h"

// For engine hiding in sample code.
#include "SuObjectManager.h"

//typedef SuCommandListPtr   EI_CommandContext;


void TressFXLogWarning(EI_StringHash warning)
{
    SuLogWarning(warning);
}



//////////////////////////////////////////////////////////////////////////////////////////////
///   Converts EI types to Sushi types.
//////////////////////////////////////////////////////////////////////////////////////////////


inline SuEffectPtr& GetEffect(EI_LayoutManagerRef layout)
{
    return (SuEffectPtr&)layout;
}

inline SuGPUResourceManager* GetResourceManager(EI_Device* pDevice)
{
    return (SuGPUResourceManager*)pDevice;
}

static SuGPUResource::StateType TranslateState(AMD::EI_ResourceState inState)
{
    switch (inState)
    {
    case AMD::EI_STATE_NON_PS_SRV: 
        return SuGPUResource::STATE_NON_PIXEL_SHADER_RESOURCE;
    case AMD::EI_STATE_VS_SRV:
        return SuGPUResource::STATE_NON_PIXEL_SHADER_RESOURCE;
    case AMD::EI_STATE_CS_SRV:
        return SuGPUResource::STATE_NON_PIXEL_SHADER_RESOURCE;
    case AMD::EI_STATE_PS_SRV:
        return  SuGPUResource::STATE_PIXEL_SHADER_RESOURCE;
    case AMD::EI_STATE_UAV:
        return  SuGPUResource::STATE_UNORDERED_ACCESS;
    case AMD::EI_STATE_COPY_DEST:
        return  SuGPUResource::STATE_COPY_DEST;
    case AMD::EI_STATE_COPY_SOURCE:
        return  SuGPUResource::STATE_COPY_SOURCE;
    case AMD::EI_STATE_RENDER_TARGET:
        return  SuGPUResource::STATE_RENDER_TARGET;
    }

    // fallback.
    SuLogWarning("Unknown state.");
    return SuGPUResource::STATE_COMMON;

}

//////////////////////////////////////////////////////////////////////////////////////////////
///   Helper functions.
//////////////////////////////////////////////////////////////////////////////////////////////


static void SuTransition(
    EI_Resource*    pResource,
    AMD::EI_ResourceState  from,
    AMD::EI_ResourceState  to)
{
    if (from == to)
    {
        if (from == AMD::EI_STATE_UAV)
        {
            pResource->resource->UAVBarrier();
        }
        else
        {
            SuLogWarning("transition from %d to %d", (int)from, (int)to);
        }
    }
    else
    {
        pResource->resource->Transition(TranslateState(from), TranslateState(to));
    }
}


static void UpdateConstants(SuArray<SuEffectParameter*>& cb, void* values, int nBytes)
{
    uint8* pCurrent = (uint8*)values;
    for (AMD::int32 i = 0; i < cb.size(); ++i)
    {
        SuEffectParameter* pParam = cb[i];
        uint32             nParamBytes = pParam->GetParameterSize();
        uint32 nCummulativeBytes = static_cast<uint32>(pCurrent - (uint8*)values) + nParamBytes;
        SU_ASSERT(nBytes >= 0);
        if (nCummulativeBytes >(uint32)nBytes)
        {
            SuLogWarning("Layout looking for %ud bytes so far, but bindset only has %ud bytes.",
                (uint32)nCummulativeBytes,
                (uint32)nBytes);
        }
        pParam->SetRawValue(pCurrent, nParamBytes);
        pCurrent += nParamBytes;
    }
}




// Generates a name from object and resource name to create a unique, single string.  
// For example, "ruby.positions" or just "positions".
static void GenerateCompositeName(SuString& fullName, const char* resourceName, const char* objectName)
{
    SU_ASSERT(resourceName != nullptr && objectName != nullptr);

    if (strlen(objectName) > 0)
        fullName.Format("%s.%s", resourceName, objectName);
    else
        fullName.Format("%s", resourceName);
}

typedef SuGPUSamplingResourceViewPtr SuSRVPtr;
typedef SuGPUUnorderedAccessViewPtr SuUAVPtr;

class EI_BindSet
{
public:
    int     nSRVs;
    SuSRVPtr* srvs;
    int     nUAVs;
    SuUAVPtr* uavs;
    //SuArray<SuGPUSamplingResourceViewPtr> srvs;
    //SuArray<SuGPUUnorderedAccessViewPtr> uavs;

    void*   values;
    int     nBytes;
};




// This is just a helper function right now.
EI_Resource* SuCreateSB(EI_Device*  pContext,
                                const AMD::uint32 structSize,
                                const AMD::uint32 structCount,
                                EI_StringHash     resourceName,
                                EI_StringHash objectName,
                                bool              bUAV,
                                bool              bCounter)
{
    SU_ASSERT(!bCounter ||
              (bCounter && bUAV));  // you can't have a counter if you don't also have a UAV.

    EI_Resource* pSB = new EI_Resource;

    EI_Resource& r = *pSB;

//    r.resource = SuGPUStructuredBufferPtr(0);
//    r.uav      = SuGPUUnorderedAccessViewPtr(0);
//    r.srv      = SuGPUSamplingResourceViewPtr(0);

    SuString strHash;
    GenerateCompositeName(strHash, resourceName, objectName);

    SuGPUResourceManager* pManager = (SuGPUResourceManager*)pContext;

    SuGPUStructuredBufferPtr sbPtr = pManager->CreateStructuredBuffer(
        (uint32_t)structSize,
        (uint32_t)structCount,
        (bUAV ? SuGPUResource::BIND_SHADER_RESOURCE | SuGPUResource::BIND_UNORDERED_ACCESS
              : SuGPUResource::BIND_SHADER_RESOURCE),
        SuMemoryBufferPtr(0),  // initial data.  We will map and upload explicitly.
        strHash,
        0,
        0);

    r.resource = sbPtr.cast<SuGPUResource>();

    SU_ASSERT(r.resource);

    SuGPUResourceDescription resourceDesc;
    resourceDesc.nFlags                = 0;
    resourceDesc.Buffer.nElementOffset = 0;
    resourceDesc.Buffer.nElementWidth  = (uint32_t)structCount;

    SU_ASSERT(SuGPUResource::GPU_RESOURCE_BUFFER == r.resource->GetType());

    SuGPUResourceViewDescription srvDesc(
        SU_SAMPLING_VIEW, SU_FORMAT_UNKNOWN, SuGPUResource::GPU_RESOURCE_BUFFER, resourceDesc);
    r.srv = r.resource->GetSamplingView(srvDesc);
    SU_ASSERT(r.srv);

    if (bCounter)
    {
        // adding counter view for UAV
        resourceDesc.nFlags = SU_COUNTER_VIEW;
    }

    if (bUAV)
    {
        SuGPUResourceViewDescription uavDesc =
            SuGPUResourceViewDescription(SU_UNORDERED_ACCESS_VIEW,
                                         SU_FORMAT_UNKNOWN,
                                         SuGPUResource::GPU_RESOURCE_BUFFER,
                                         resourceDesc);
        r.uav = r.resource->GetUnorderedAccessView(uavDesc);

        SU_ASSERT(r.uav);
    }

    return pSB;
}

extern "C"
{

    EI_Resource* SuCreate2D(EI_Device* pContext,
        const size_t     width,
        const size_t     height,
        const size_t     arraySize,
        EI_StringHash    strHash)
    {
        (void)pContext;

        EI_Resource* pRW2D = new EI_Resource;

        SuGPUTexture2DArrayPtr texPtr = SuGPUResourceManager::GetPtr()->CreateTexture2DArray(SuGPUResource::GPU_RESOURCE_DYNAMIC,
            SuGPUResource::BIND_RENDER_TARGET |
            SuGPUResource::BIND_SHADER_RESOURCE |
            SuGPUResource::BIND_UNORDERED_ACCESS,
            0,
            SuGPUResourceFormat::SU_FORMAT_R32_UINT,
            SuGPUTexture::MIP_NONE,
            1,  // Mip levels
            (uint16)width,
            (uint16)height,
            (uint16)arraySize,
            1,
            0,  // sample count and quality
            SuMemoryBufferPtr(0),
            strHash);

        pRW2D->resource = texPtr.cast<SuGPUResource>();
        pRW2D->srv = texPtr->GetDefaultSamplingView();
        SuGPUResourceDescription     desc = texPtr->GetDefaultResourceDesc();
        SuGPUResourceViewDescription viewDesc =
            SuGPUResourceViewDescription(SU_UNORDERED_ACCESS_VIEW,
                texPtr->GetFormat(),
                SuGPUResource::GPU_RESOURCE_TEXTURE_2D_ARRAY,
                desc);
        pRW2D->uav = texPtr->GetUnorderedAccessView(viewDesc);
        pRW2D->rtv = SuGPURenderableResourceViewPtr(0);

        return pRW2D;
    }

    EI_Resource* SuCreateRT(EI_Device* pContext,
        const size_t     width,
        const size_t     height,
        const size_t     channels,
        EI_StringHash    strHash,
        float            clearR,
        float            clearG,
        float            clearB,
        float            clearA)
    {
        EI_Resource* pRW2D = new EI_Resource;

        SuGPUResourceFormat format;
        if (channels == 1)
        {
            format = SU_FORMAT_R16_FLOAT;
        }
        else if (channels == 2)
        {
            format = SU_FORMAT_R16G16_FLOAT;
        }
        else if (channels == 4)
        {
            format = SU_FORMAT_R16G16B16A16_FLOAT;
        }

        SuGPUTexture2DArrayClearInfo clearInfo;
        clearInfo.viewFormat = format;
        clearInfo.color = SuVector4(clearR, clearG, clearB, clearA);

        SuGPUTexture2DArrayPtr texPtr = SuGPUResourceManager::GetPtr()->CreateTexture2DArray(SuGPUResource::GPU_RESOURCE_DYNAMIC,
            SuGPUResource::BIND_RENDER_TARGET |
            SuGPUResource::BIND_SHADER_RESOURCE,
            0,
            format,
            SuGPUTexture::MIP_NONE,
            1,  // Mip levels
            (uint16)width,
            (uint16)height,
            1,
            1,
            0,  // sample count and quality
            SuMemoryBufferPtr(0),
            strHash,
            NULL,
            0,
            &clearInfo);

        pRW2D->resource = texPtr.cast<SuGPUResource>();

        pRW2D->srv = texPtr->GetDefaultSamplingView();
        pRW2D->uav = SuGPUUnorderedAccessViewPtr(0);
        SuGPUResourceDescription     desc = pRW2D->resource->GetDefaultResourceDesc();
        SuGPUResourceViewDescription viewDesc =
            SuGPUResourceViewDescription(SU_RENDERABLE_VIEW,
                texPtr->GetFormat(),
                SuGPUResource::GPU_RESOURCE_TEXTURE_2D_ARRAY,
                desc);
        pRW2D->rtv = texPtr->GetRenderableView(viewDesc);

        return pRW2D;
    }

    EI_BindLayout* SuCreateLayout(EI_Device* pDevice, EI_LayoutManagerRef layoutManager, const AMD::TressFXLayoutDescription& description)
    {
        (void)pDevice;

        EI_BindLayout* pLayout = new EI_BindLayout;

        SuEffectPtr pEffect = GetEffect(layoutManager);


        for (int i = 0; i < description.nSRVs; ++i)
        {
            pLayout->srvs.push_back(pEffect->GetTextureSlot(description.srvNames[i]));
        }

        for (int i = 0; i < description.nUAVs; ++i)
        {
            const SuUAVSlot* pSlot = pEffect->GetUAVSlot(description.uavNames[i]);
            SU_ASSERT(pSlot != nullptr);
            pLayout->uavs.push_back(pSlot);
        }

        (void)description.constants.constantBufferName; // Sushi doesn't use constant buffer names.  It sets individually.

        for (int i = 0; i < description.constants.nConstants; i++)
        {
            pLayout->constants.push_back(pEffect->GetParameter(description.constants.parameterNames[i]));
        }


        return pLayout;

    }

    void SuDestroyLayout(EI_Device* pDevice, EI_BindLayout* pLayout)
    {
        (void)pDevice;
        AMD_SAFE_DELETE(pLayout);
    }

    // Creates structured buffer. SRV only.  Begin state should be Upload.
    EI_Resource* SuCreateReadOnlySB(EI_Device*  pContext,
        const AMD::uint32 structSize,
        const AMD::uint32 structCount,
        EI_StringHash     resourceName, EI_StringHash objectName)
    {
        return SuCreateSB(pContext, structSize, structCount, resourceName, objectName, false, false);
    }


    // Creates read/write structured buffer. has UAV so begin state should be UAV.
    EI_Resource* SuCreateReadWriteSB(EI_Device*  pContext,
        const AMD::uint32 structSize,
        const AMD::uint32 structCount,
        EI_StringHash     resourceName, EI_StringHash objectName)
    {
        return SuCreateSB(pContext, structSize, structCount, resourceName, objectName, true, false);
    }

    // Creates read/write structured buffer with a counter. has UAV so begin state should be UAV.
    EI_Resource* SuCreateCountedSB(EI_Device*  pContext,
        const AMD::uint32 structSize,
        const AMD::uint32 structCount,
        EI_StringHash     resourceName, EI_StringHash objectName)
    {
        return SuCreateSB(pContext, structSize, structCount, resourceName, objectName, true, true);
    }

    void SuDestroy(EI_Device* pDevice, EI_Resource* pRW2D)
    {
        SuGPUResourceManager* pResourceManager = GetResourceManager(pDevice);

        pRW2D->uav = SuGPUUnorderedAccessViewPtr(0);
        pRW2D->srv = SuGPUSamplingResourceViewPtr(0);
        pRW2D->rtv = SuGPURenderableResourceViewPtr(0);
        pRW2D->resource = SuGPUResourcePtr(0);
        delete pRW2D;
    }

    void SuClearCounter(EI_CommandContextRef    pContext,
        EI_StructuredBufferRef sb,
        AMD::uint32          clearValue)
    {
        (void)pContext;
        sb.uav->SetInitialCount(clearValue);
    }

    void SuCopy(EI_CommandContextRef pContext, EI_StructuredBuffer& from, EI_StructuredBuffer& to)
    {
        (void)pContext;
        to.resource->CopyResource(*from.resource);
    }


    // Map gets a pointer to upload heap / mapped memory.
    // Unmap issues the copy.
    // This is only ever used for the initial upload.
    //
    void* SuMap(EI_CommandContextRef pContext, EI_StructuredBuffer& sb)
    {
        SU_ASSERT(sb.resource->GetType() == SuGPUResource::GPU_RESOURCE_BUFFER);
        SuGPUBuffer* pBuffer = static_cast<SuGPUBuffer*>(sb.resource.get());
        SuHandle handle = pBuffer->Map(SuGPUResource::MAP_WRITE_DISCARD);
        return (void*)handle;
    }

    bool SuUnmap(EI_CommandContextRef pContext, EI_StructuredBuffer& sb)
    {
        return sb.resource->Unmap();
    }

    void SuSubmitBarriers(EI_CommandContextRef context,
        int numBarriers,
        AMD::EI_Barrier* barriers)
    {
        (void)context;
        for (int i = 0; i < numBarriers; ++i)
        {
            SuTransition(barriers[i].pResource, barriers[i].from, barriers[i].to);
        }
    }

    void SuClear2D(EI_CommandContext* pContext, EI_RWTexture2D* pResource, AMD::uint32 clearValue)
    {
        (void)pContext;

        uint32_t clearVector[4];
        clearVector[0] = clearVector[1] = clearVector[2] = clearVector[3] = clearValue;
        pResource->uav->ClearUInt(clearVector);
        pResource->resource->UAVBarrier();
    }


    EI_BindSet* SuCreateBindSet(EI_Device* commandContext, AMD::TressFXBindSet& bindSet)
    {
        EI_BindSet* pBindSet = new EI_BindSet;

        // should just replace with a SuArray.  Same as constant buffers.

        pBindSet->nBytes = bindSet.nBytes;
        pBindSet->values = bindSet.values;

        // Maybe replace this with SuArray usage.

        pBindSet->nSRVs = bindSet.nSRVs;
        if (bindSet.nSRVs > 0)
        {
            pBindSet->srvs = new SuSRVPtr[bindSet.nSRVs];
            for (int i = 0; i < bindSet.nSRVs; ++i)
            {
                pBindSet->srvs[i] = bindSet.srvs[i]->srv;
            }
        }
        else
        {
            pBindSet->srvs = nullptr;
        }

        pBindSet->nUAVs = bindSet.nUAVs;
        if (pBindSet->nUAVs > 0)
        {
            pBindSet->uavs = new SuUAVPtr[bindSet.nUAVs];
            for (int i = 0; i < bindSet.nUAVs; ++i)
            {
                pBindSet->uavs[i] = bindSet.uavs[i]->uav;
            }
        }
        else
        {
            pBindSet->uavs = nullptr;
        }

        return pBindSet;
    }

    void SuDestroyBindSet(EI_Device* pDevice, EI_BindSet* pBindSet)
    {
        if (pBindSet->nSRVs > 0)
        {
            // should just call destructors,which should be setting to null.
            delete[] pBindSet->srvs;
        }

        if (pBindSet->nUAVs > 0)
        {
            delete[] pBindSet->uavs;
        }

        delete pBindSet;
    }

    void SuBind(EI_CommandContextRef commandContext, EI_BindLayout* pLayout, EI_BindSet& set)
    {
        (void)commandContext;
        SU_ASSERT(set.nSRVs == pLayout->srvs.size());
        for (AMD::int32 i = 0; i < set.nSRVs; ++i)
        {
            pLayout->srvs[i]->BindResource(set.srvs[i]);
        }

        SU_ASSERT(set.nUAVs == pLayout->uavs.size());
        for (AMD::int32 i = 0; i < set.nUAVs; ++i)
        {
            pLayout->uavs[i]->BindResource(set.uavs[i]);
        }

        UpdateConstants(pLayout->constants, set.values, set.nBytes);

    }


    EI_PSO* SuCreateComputeShaderPSO(EI_Device*     pDevice,
        EI_LayoutManagerRef     layoutManager,
        const EI_StringHash& shaderName)
    {
        (void)pDevice;
        // SuEffect& effect = (SuEffect&)layoutManager;
        SuEffectPtr pEffect = GetEffect(layoutManager);

        //return const_cast<SuEffectTechnique*>(
        const SuEffectTechnique* pTechnique = pEffect->GetTechnique(SuString("TressFXSimulation_") + shaderName);

        return GetPSOPtr(pTechnique);
    }

    void SuDestroyPSO(EI_Device* pDevice, EI_PSO* pso)
    {
        (void)pDevice;
        (void)pso;
    }

    // All our compute shaders have dimensions of (N,1,1)
    void SuDispatch(EI_CommandContextRef commandContext, EI_PSO& pso, int nThreadGroups)
    {
        (void)commandContext;

        //SuEffectTechnique* pTechnique = pso;
        SuEffectTechnique& technique = GetTechniqueRef(pso);
        SuEffect*          pEffect = const_cast<SuEffect*>(technique.GetParent());

        uint32 numPasses;

        pEffect->Begin(&technique, &numPasses);
        SU_ASSERT(numPasses == 1);
        pEffect->BeginPass(0);
        SuRenderManager::GetRef().Dispatch(nThreadGroups, 1, 1);
        pEffect->EndPass();
        pEffect->End();
    }

    // Initialize and leave in state for use as index buffer.
    // Index buffers are for either triangle-strip hair, or line segments.
    EI_IndexBuffer* SuCreateIndexBuffer(EI_Device* context,
        AMD::uint32      indexCount,
        void*            pInitialData, EI_StringHash objectName)
    {
        (void)objectName;

        SuGPUIndexBufferPtr* pIBPtr = new SuGPUIndexBufferPtr;
        SuGPUIndexBufferPtr& ib = *pIBPtr;
        SuGPUResourceManager* pResourceManager = (SuGPUResourceManager*)context;

        ib = SuGPUResourceManager::GetPtr()->CreateResourceIndex(
            SuGPUResource::GPU_RESOURCE_DYNAMIC, TRESSFX_INDEX_SIZE, indexCount, SuMemoryBufferPtr(0));

        uint8* p = (uint8*)ib->Map(SuGPUResource::MAP_WRITE_DISCARD);
        memcpy(p, pInitialData, TRESSFX_INDEX_SIZE * indexCount);
        ib->Unmap();
        ib->Transition(SuGPUResource::STATE_COPY_DEST, SuGPUResource::STATE_INDEX_BUFFER);

        return (EI_IndexBuffer*)pIBPtr;
    }

    void SuDestroyIB(EI_Device* pDevice, EI_IndexBuffer* pBuffer)
    {
        delete (SuGPUIndexBufferPtr*)pBuffer;
    }


    void SuDrawIndexedInstanced(EI_CommandContextRef     commandContext,
        EI_PSO&                pso,
        AMD::EI_IndexedDrawParams& drawParams)
    {
        (void)commandContext;
        //const SuEffectTechnique* pTechnique = pso;
        const SuEffectTechnique& technique = (const SuEffectTechnique&)(pso);
        SuEffect*                pEffect = const_cast<SuEffect*>(technique.GetParent());
        SuGPUIndexBufferPtr pIndexBuffer = *((SuGPUIndexBufferPtr*)drawParams.pIndexBuffer);

        pEffect->BindIndexBuffer(pIndexBuffer.get());

        uint32 numPasses;
        bool   techniqueFound = pEffect->Begin(&technique, &numPasses);

        if (techniqueFound)
        {
            for (uint32 i = 0; i < numPasses; ++i)
            {
                pEffect->BeginPass(i);
                SuRenderManager::GetRef().DrawIndexedInstanced(SuRenderManager::TRIANGLE_LIST,
                    0,
                    0 /*Doesn't matter*/,
                    0 /*Doesn't matter*/,
                    drawParams.numIndices,
                    0,
                    TRESSFX_INDEX_SIZE,
                    drawParams.numInstances,
                    0);
                pEffect->EndPass();
            }
            pEffect->End();
        }
    }



}


EI_PSO* GetPSO(const char* techniqueName, SuEffect* pEffect)
{
    if (pEffect == nullptr)
    {
        SuLogWarning("Could not get technique named %s because effect is null.", techniqueName);
        return nullptr;
    }
    EI_PSO*            pTechnique = (EI_PSO*)(pEffect->GetTechnique(techniqueName));

    if (pTechnique == nullptr)
    {
        SuLogWarning("Could not get technique named %s", techniqueName);
        return nullptr;
    }

    return pTechnique;
}










