
//---------------------------------------------------------------------------------------
// Example code for running the shortcut method.
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

#include "ShortCut.h"
#include "TressFXShortCut.h"

#include "HairStrands.h"
#include "TressFXHairObject.h"

#include "SushiGPUInterface.h"
#include "SushiUtils.h"

ShortCut::ShortCut() : m_pTressFXShortCut(nullptr)
{
    m_pFullscreenPass = new FullscreenPass;
}

void ShortCut::Initialize(int width, int height, SuEffect* pStrandEffect, SuEffect* pQuadEffect)
{

    // Geometry passes.
    m_pDepthsAlphaPSO = GetPSO("ShortCutDepthsAlpha", pStrandEffect);
    m_pFillColorsPSO = GetPSO("ShortCutFillColors", pStrandEffect);

    // Fullscreen passes.
    m_pResolveDepthPSO = GetPSO("ShortCutResolveDepth", pQuadEffect);
    m_pResolveColorPSO = GetPSO("ShortCutResolveColor", pQuadEffect);

    GetBackandDepthBufferViews();


    EI_Device* pDevice = GetDevice();

    SU_ASSERT(m_pTressFXShortCut == nullptr);

    if (m_pTressFXShortCut == nullptr)
    {
        m_pTressFXShortCut = new TressFXShortCut;
    }

    m_pTressFXShortCut->Create(pDevice, width, height);
}



void ShortCut::Draw(EI_CommandContextRef commandContext, std::vector<HairStrands*>& hairStrands)
{
    SuGPUTimerHandle depthsAlphaTimer = SuRenderManager::GetRef().CreateGPUTimer("render: S DepthsAlpha");
    SuGPUTimerHandle resolveDepthTimer =
        SuRenderManager::GetRef().CreateGPUTimer("render: S ResolveDepth");
    SuGPUTimerHandle fillColorsTimer = SuRenderManager::GetRef().CreateGPUTimer("render: S FillColors");
    SuGPUTimerHandle resolveColorTimer =
        SuRenderManager::GetRef().CreateGPUTimer("render: S ResolveColor");

    m_pTressFXShortCut->Clear(commandContext);

    // Bind render targets for DepthsAlpha pass and initialize depths
    SuRenderManager::GetRef().SetRenderTargets(
        1, &m_pTressFXShortCut->GetInvAlphaTexture()->rtv, m_pDepthBufferView);
    SuRenderManager::GetRef().SetClearColor(SuVector4(1, 1, 1, 1));
    SuRenderManager::GetRef().Clear(true, false, false);

    m_pTressFXShortCut->BindForDepthsAlpha(commandContext);
    for (size_t i = 0; i < hairStrands.size(); i++)
    {
        TressFXHairObject* pHair = hairStrands[i]->GetTressFXHandle();

        if (pHair)
        {
            GPU_TIMER_START(depthsAlphaTimer);
            pHair->DrawStrands(commandContext, *m_pDepthsAlphaPSO);
            GPU_TIMER_STOP(depthsAlphaTimer);
        }
    }

    m_pTressFXShortCut->DoneDepthsAlpha(commandContext);

    // Bind only depth stencil for ResolveDepth pass
    SuRenderManager::GetRef().SetRenderTargets(0, NULL, m_pDepthBufferView);

    m_pTressFXShortCut->BindForResolveDepth(commandContext);
    GPU_TIMER_START(resolveDepthTimer);
    m_pFullscreenPass->Draw(m_pResolveDepthPSO);
    GPU_TIMER_STOP(resolveDepthTimer);
    m_pTressFXShortCut->DoneResolveDepth(commandContext);

    // Bind and clear render target for FillColors pass
    SuRenderManager::GetRef().SetRenderTargets(
        1, &m_pTressFXShortCut->GetColorsTexture()->rtv, m_pDepthBufferView);
    SuRenderManager::GetRef().SetClearColor(SuVector4(0, 0, 0, 0));
    SuRenderManager::GetRef().Clear(true, false, false);

    m_pTressFXShortCut->BindForFillColors(commandContext);  // put clear in here?
    for (size_t i = 0; i < hairStrands.size(); i++)
    {
        TressFXHairObject* pHair = hairStrands[i]->GetTressFXHandle();

        if (pHair)
        {
            SuObjectManager::GetRef().SetObjectBeingDrawn(hairStrands[i]->GetAsEngineObject());
            GPU_TIMER_START(fillColorsTimer);
            pHair->DrawStrands(commandContext, *m_pFillColorsPSO);
            GPU_TIMER_STOP(fillColorsTimer);
            SuObjectManager::GetRef().SetObjectBeingDrawn(0);
        }
    }

    m_pTressFXShortCut->DoneFillColors(commandContext);


    // TODO move this to a clear "after all pos and tan usage by rendering" place.
    for (size_t i = 0; i < hairStrands.size(); i++)
        hairStrands[i]->TransitionRenderingToSim(commandContext);

    UnbindUAVS();  // Engine-required hack to unbind UAVs.


                   // Bind back buffer for blend
    SuRenderManager::GetRef().SetRenderTargets(
        1, &m_pBackBufferView, m_pDepthBufferView);

    m_pTressFXShortCut->BindForResolveColor(commandContext);
    GPU_TIMER_START(resolveColorTimer);
    m_pFullscreenPass->Draw(m_pResolveColorPSO);
    GPU_TIMER_STOP(resolveColorTimer);
    m_pTressFXShortCut->DoneResolveColor(commandContext);
}


void ShortCut::GetBackandDepthBufferViews()
{
    if (m_pBackBufferView == NULL)
    {
        // Initialize back and depth buffer views
        SuGPUTexture2DArray* tBackBuffer = (SuGPUTexture2DArray*)SuGPUResourceManager::GetRef()
            .GetNamedResource("tBackBuffer")
            .get();
        SuGPUTexture2DArray* tDepthBuffer = (SuGPUTexture2DArray*)SuGPUResourceManager::GetRef()
            .GetNamedResource("tDepthBuffer")
            .get();
        SuGPUResourceViewDescription tBackBufferViewDesc(
            SU_RENDERABLE_VIEW,
            tBackBuffer->GetFormat(),
            SuGPUResource::GPU_RESOURCE_TEXTURE_2D_ARRAY,
            tBackBuffer->GetDefaultResourceDesc());
        SuGPUResourceViewDescription tDepthBufferViewDesc(
            SU_DEPTH_STENCIL_VIEW,
            SU_FORMAT_D32_FLOAT,
            SuGPUResource::GPU_RESOURCE_TEXTURE_2D_ARRAY,
            tDepthBuffer->GetDefaultResourceDesc());
        m_pBackBufferView = tBackBuffer->GetRenderableView(tBackBufferViewDesc);
        m_pDepthBufferView = tDepthBuffer->GetDepthStencilView(tDepthBufferViewDesc);
    }

}

void ShortCut::Shutdown(EI_Device* pDevice)
{
    if (m_pTressFXShortCut)
    {
        m_pTressFXShortCut->Destroy(pDevice);
        delete m_pTressFXShortCut;
        m_pTressFXShortCut = nullptr;
    }
}

ShortCut::~ShortCut()
{
    SU_ASSERT(m_pTressFXShortCut == nullptr);
    delete m_pFullscreenPass;
}