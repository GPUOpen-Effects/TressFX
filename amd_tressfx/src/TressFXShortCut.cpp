// ----------------------------------------------------------------------------
// Interface for the shortcut method.
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

#include "TressFXEngineInterface.h"
#include "TressFXGPUInterface.h"
#include "TressFXLayouts.h"
#include "TressFXShortCut.h"

using namespace AMD;

TressFXShortCut::TressFXShortCut()
    : m_nScreenWidth(0)
    , m_nScreenHeight(0)
    , m_bCreated(false)
    , m_pDepths()
    , m_pShortCutDepthsAlphaBindSet(nullptr)
    , m_pShortCutResolveDepthBindSet(nullptr)
    , m_pShortCutFillColorsBindSet(nullptr)
    , m_pShortCutResolveColorBindSet(nullptr)
    , m_pInvAlpha()
    , m_pColors()
{
}

TressFXShortCut::~TressFXShortCut()
{
    TRESSFX_ASSERT(!m_bCreated);
    Destroy(nullptr);
}

bool TressFXShortCut::Create(EI_Device* pDevice, int width, int height)
{
    TRESSFX_ASSERT(!m_bCreated);

    m_nScreenWidth  = width;
    m_nScreenHeight = height;

    m_pDepths = EI_RW2D_Create(pDevice,
                                    width,
                                    height,
                                    TRESSFX_SHORTCUT_K,
                                    TRESSFX_STRING_HASH("ShortCutDepthsTexture"));
    m_pInvAlpha = EI_FP16RT_Create(pDevice,
                                   width,
                                   height,
                                   1,
                                   TRESSFX_STRING_HASH("ShortCutInvAlphaTexture"),
                                   1.0f,
                                   1.0f,
                                   1.0f,
                                   1.0f);
    m_pColors = EI_FP16RT_Create(pDevice,
                                 width,
                                 height,
                                 4,
                                 TRESSFX_STRING_HASH("ShortCutColorsTexture"),
                                 0.0f,
                                 0.0f,
                                 0.0f,
                                 0.0f);

    CreateDepthsAlphaBindSet(pDevice);
    CreateResolveDepthBindSet(pDevice);
    CreateFillColorsBindSet(pDevice);
    CreateResolveColorBindSet(pDevice);

    m_bCreated = true;

    return true;
}

// return the set?
void TressFXShortCut::CreateDepthsAlphaBindSet(EI_Device* pDevice)
{
    EI_Resource* uavs[1];
    uavs[TRESSFX_IDUAV_SHORTCUT_DEPTHS] = m_pDepths;

    TressFXBindSet bindSet;
    // For now, none will have values.
    bindSet.nBytes = 0;
    bindSet.values = 0;

    bindSet.nUAVs               = AMD_ARRAY_SIZE(uavs);
    bindSet.uavs                = uavs;
    bindSet.nSRVs               = 0;
    m_pShortCutDepthsAlphaBindSet = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXShortCut::CreateResolveDepthBindSet(EI_Device* pDevice)
{
    EI_Resource* srvs[1];
    srvs[TRESSFX_IDSRV_SHORTCUT_DEPTHS] = m_pDepths;

    TressFXBindSet bindSet;
    // For now, none will have values.
    bindSet.nBytes = 0;
    bindSet.values = 0;


    bindSet.nSRVs                = AMD_ARRAY_SIZE(srvs);
    bindSet.srvs                 = srvs;
    bindSet.nUAVs                = 0;
    m_pShortCutResolveDepthBindSet = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXShortCut::CreateFillColorsBindSet(EI_Device* pDevice)
{
    EI_Resource* srvs[1];
    srvs[TRESSFX_IDSRV_SHORTCUT_DEPTHS] = m_pDepths;

    TressFXBindSet bindSet;
    // For now, none will have values.
    bindSet.nBytes = 0;
    bindSet.values = 0;


    bindSet.nSRVs              = AMD_ARRAY_SIZE(srvs);
    bindSet.srvs               = srvs;
    bindSet.nUAVs              = 0;
    m_pShortCutFillColorsBindSet = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXShortCut::CreateResolveColorBindSet(EI_Device* pDevice)
{
    EI_Resource* srvs[3];

    srvs[TRESSFX_IDSRV_SHORTCUT_DEPTHS]   = m_pDepths;
    srvs[TRESSFX_IDSRV_SHORTCUT_COLORS]   = m_pColors;
    srvs[TRESSFX_IDSRV_SHORTCUT_INVALPHA] = m_pInvAlpha;

    TressFXBindSet bindSet;
    // For now, none will have values.
    bindSet.nBytes = 0;
    bindSet.values = 0;


    bindSet.nSRVs                = AMD_ARRAY_SIZE(srvs);
    bindSet.srvs                 = srvs;
    bindSet.nUAVs                = 0;
    m_pShortCutResolveColorBindSet = EI_CreateBindSet(pDevice, bindSet);
}

void TressFXShortCut::Destroy(EI_Device* context)
{
    if (m_bCreated)
    {
        EI_DestroyBindSet(context, m_pShortCutDepthsAlphaBindSet);
        EI_DestroyBindSet(context, m_pShortCutResolveDepthBindSet);
        EI_DestroyBindSet(context, m_pShortCutResolveColorBindSet);
        EI_DestroyBindSet(context, m_pShortCutFillColorsBindSet);

        EI_RW2D_Destroy(context, m_pDepths);
        EI_RW2D_Destroy(context, m_pColors);
        EI_RW2D_Destroy(context, m_pInvAlpha);
    }
    m_bCreated = false;
}


void TressFXShortCut::Clear(EI_CommandContextRef context)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("Depths texture cleared without being created.");
    }
    // Consider being explicit on values here.
    // In DX, UAV counter clears are actually done when UAV is set, which makes this a bit weird, I
    // suppose.
    // Also try clearing in shader during fullscreen pass (although can't miss frames, and makes
    // reads slower)
    // or look at 0-based for fast clears.

    EI_RW2D_Clear(&context, m_pDepths, TRESSFX_SHORTCUT_INITIAL_DEPTH);
}

void TressFXShortCut::DoneDepthsAlpha(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut DoneDepthsAlpha without being created.");
    }

    EI_Barrier writeToRead[] = 
    {
        { m_pDepths, EI_STATE_UAV, EI_STATE_PS_SRV },
        { m_pInvAlpha, EI_STATE_RENDER_TARGET, EI_STATE_PS_SRV }
    };

    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(writeToRead), writeToRead);
    
}

void TressFXShortCut::DoneResolveDepth(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut DoneResolveDepth without being created.");
    }
}

void TressFXShortCut::DoneFillColors(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut DoneFillColors without being created.");
    }

    EI_Barrier colorRT2SRV[] = 
    {
        { m_pColors, EI_STATE_RENDER_TARGET, EI_STATE_PS_SRV }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(colorRT2SRV), colorRT2SRV);
}

void TressFXShortCut::DoneResolveColor(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut DoneResolveColor without being created.");
    }

    EI_Barrier doneResolveColor[] = 
    {
        { m_pDepths, EI_STATE_PS_SRV, EI_STATE_UAV },
        { m_pColors, EI_STATE_PS_SRV, EI_STATE_RENDER_TARGET },
        { m_pInvAlpha, EI_STATE_PS_SRV, EI_STATE_RENDER_TARGET }
    };
    EI_SubmitBarriers(commandContext, AMD_ARRAY_SIZE(doneResolveColor), doneResolveColor);
}

void TressFXShortCut::BindForDepthsAlpha(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut BindForDepthsAlpha without being created.");
    }
    EI_Bind(commandContext, GetShortCutDepthsAlphaLayout(), *m_pShortCutDepthsAlphaBindSet);
}

void TressFXShortCut::BindForResolveDepth(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut BindForResolveDepth without being created.");
    }
    EI_Bind(commandContext, GetShortCutResolveDepthLayout(), *m_pShortCutResolveDepthBindSet);
}

void TressFXShortCut::BindForFillColors(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut BindForFillColors without being created.");
    }
    EI_Bind(commandContext, GetShortCutFillColorsLayout(), *m_pShortCutFillColorsBindSet);
}

void TressFXShortCut::BindForResolveColor(EI_CommandContextRef commandContext)
{
    if (!m_bCreated)
    {
        TressFXLogWarning("ShortCut BindForResolveColor without being created.");
    }
    EI_Bind(commandContext, GetShortCutResolveColorLayout(), *m_pShortCutResolveColorBindSet);
}
