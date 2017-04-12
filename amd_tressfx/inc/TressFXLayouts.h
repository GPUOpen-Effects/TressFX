// ----------------------------------------------------------------------------
// Layouts describe resources for each type of shader in TressFX.
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
#include "TressFXConstantBuffers.h"

// uavs on simulation side.
#define TRESSFX_IDUAV_POS_OFFSET 0
#define TRESSFX_IDUAV_POS_PREV_OFFSET 1
#define TRESSFX_IDUAV_POS_PREV_PREV_OFFSET 2
#define TRESSFX_IDUAV_TAN_OFFSET 3

// srvs on render side.
#define TRESSFX_IDSRV_POS_OFFSET 0
#define TRESSFX_IDSRV_TAN_OFFSET 1

#define TRESSFX_IDSRV_THICKNESS_OFFSET 0
#define TRESSFX_IDSRV_ROOT_TEXCOORD_OFFSET 1
#define TRESSFX_IDSRV_COLOR_TEXTURE_OFFSET 2

// PPLL offsets
#define TRESSFX_IDUAV_PPLL_HEADS 0
#define TRESSFX_IDUAV_PPLL_NODES 1
#define TRESSFX_IDSRV_PPLL_HEADS 0
#define TRESSFX_IDSRV_PPLL_NODES 1

// ShortCut offsets
#define TRESSFX_IDUAV_SHORTCUT_DEPTHS 0
#define TRESSFX_IDSRV_SHORTCUT_DEPTHS 0
#define TRESSFX_IDSRV_SHORTCUT_COLORS 1
#define TRESSFX_IDSRV_SHORTCUT_INVALPHA 2



#define TRESSFX_GET_16BYTE_INDEX(s, m) (offsetof(s, m) / 16)



struct TressFXLayouts
{
    EI_BindLayout* pRenderLayout = nullptr;
    EI_BindLayout* pRenderPosTanLayout = nullptr;
    EI_BindLayout* pSimPosTanLayout = nullptr;
    EI_BindLayout* pGenerateSDFLayout = nullptr;
    EI_BindLayout* pSimLayout = nullptr;
    EI_BindLayout* pApplySDFLayout = nullptr;
    EI_BindLayout* pBoneSkinningLayout = nullptr;
    EI_BindLayout* pSDFMarchingCubesLayout = nullptr;
    EI_BindLayout* pPPLLBuildLayout = nullptr;
    EI_BindLayout* pPPLLReadLayout = nullptr;
    EI_BindLayout* pShortCutDepthsAlphaLayout = nullptr;
    EI_BindLayout* pShortCutResolveDepthLayout = nullptr;
    EI_BindLayout* pShortCutFillColorsLayout = nullptr;
    EI_BindLayout* pShortCutResolveColorLayout = nullptr;

#if ENABLE_ROV_TEST
    EI_BindLayout shortCutROVBuildLayout;
    EI_BindLayout shortCutROVReadLayout;
    EI_BindLayout kBufferMutexBuildLayout;
    EI_BindLayout kBufferMutexReadLayout;
    EI_BindLayout kBufferROVBuildLayout;
    EI_BindLayout kBufferROVReadLayout;
    EI_BindLayout renderPosTanLayoutROVTest;
    EI_BindLayout renderLayoutROVTest;
#endif
};

// by default, we just get layouts from a global.
// Layouts should be reusable for all instances.
// These should be allocated by the user. e.g g_TressFXSlots = new
// TressFXSlots::SlotArraysForAllLayouts;
extern TressFXLayouts*                        g_TressFXLayouts;


void CreateSimPosTanLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateRenderPosTanLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateRenderLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateGenerateSDFLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateSimLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateApplySDFLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager);
void CreateBoneSkinningLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager);
void CreateSDFMarchingCubesLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager);
void CreatePPLLBuildLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager);
void CreatePPLLReadLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager);

void CreateShortCutDepthsAlphaLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateShortCutResolveDepthLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateShortCutFillColorsLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);
void CreateShortCutResolveColorLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager);

void DestroyAllLayouts(EI_Device* pDevice);


inline EI_BindLayout* GetSimPosTanLayout() { return g_TressFXLayouts->pSimPosTanLayout; }
inline EI_BindLayout* GetRenderPosTanLayout() { return g_TressFXLayouts->pRenderPosTanLayout; }
inline EI_BindLayout* GetRenderLayout() { return g_TressFXLayouts->pRenderLayout; }
inline EI_BindLayout* GetGenerateSDFLayout() { return g_TressFXLayouts->pGenerateSDFLayout; }
inline EI_BindLayout* GetApplySDFLayout() { return g_TressFXLayouts->pApplySDFLayout; }
inline EI_BindLayout* GetSimLayout() { return g_TressFXLayouts->pSimLayout; }
inline EI_BindLayout* GetBoneSkinningMeshLayout() { return g_TressFXLayouts->pBoneSkinningLayout; }
inline EI_BindLayout* GetSDFMarchingCubesLayout() { return g_TressFXLayouts->pSDFMarchingCubesLayout; }

inline EI_BindLayout* GetPPLLBuildLayout() { return g_TressFXLayouts->pPPLLBuildLayout; }
inline EI_BindLayout* GetPPLLReadLayout() { return g_TressFXLayouts->pPPLLReadLayout; }
inline EI_BindLayout* GetShortCutDepthsAlphaLayout()
{
    return g_TressFXLayouts->pShortCutDepthsAlphaLayout;
}
inline EI_BindLayout* GetShortCutResolveDepthLayout()
{
    return g_TressFXLayouts->pShortCutResolveDepthLayout;
}
inline EI_BindLayout* GetShortCutFillColorsLayout()
{
    return g_TressFXLayouts->pShortCutFillColorsLayout;
}
inline EI_BindLayout* GetShortCutResolveColorLayout()
{
    return g_TressFXLayouts->pShortCutResolveColorLayout;
}

