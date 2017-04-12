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

#include "TressFXLayouts.h"


using namespace AMD;

void CreateSimPosTanLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager)
{
    static const int NUM_UAVS = 4;

    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("g_HairVertexPositions"),
        TRESSFX_STRING_HASH("g_HairVertexPositionsPrev"),
        TRESSFX_STRING_HASH("g_HairVertexPositionsPrevPrev"),
        TRESSFX_STRING_HASH("g_HairVertexTangents")
    };

    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        0,
        nullptr,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_CS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pSimPosTanLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, pLayoutManager, layoutDesc);
}



void CreateRenderPosTanLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager)
{
    static const int NUM_SRVS = 2;

    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("g_GuideHairVertexPositions"),
        TRESSFX_STRING_HASH("g_GuideHairVertexTangents")
    };

    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        0,
        nullptr,

        constDesc,
        EI_VS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pRenderPosTanLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, pLayoutManager, layoutDesc);

}



void CreateRenderLayout2(EI_Device* pDevice, EI_LayoutManagerRef pLayoutManager)
{
 
    static const EI_StringHash srvSlotNames[3] =
    {
        TRESSFX_STRING_HASH("g_HairThicknessCoeffs"),
        TRESSFX_STRING_HASH("g_HairStrandTexCd"),
        TRESSFX_STRING_HASH("g_txHairColor")
    };

    EI_StringHash paramNames[] = { "g_NumVerticesPerStrand" };

    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH("tressfxShadeParameters"),
        sizeof(TressFXRenderingConstantBuffer),
        1,
        paramNames
    };

    static const TressFXLayoutDescription renderLayoutDesc =
    {
        3,
        srvSlotNames,

        0,
        nullptr,

        constDesc,
        EI_VS
    };

    if (g_TressFXLayouts->pRenderLayout != nullptr)
        EI_DestroyLayout(pDevice, g_TressFXLayouts->pRenderLayout);

    g_TressFXLayouts->pRenderLayout = EI_CreateLayout(pDevice, pLayoutManager, renderLayoutDesc);
}

void CreateGenerateSDFLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 1;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("g_TrimeshVertexIndices")
    };

    // UAVS
    static const int NUM_UAVS = 2;
    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("g_SignedDistanceField"),
        TRESSFX_STRING_HASH("collMeshVertexPositions")
    };

    EI_StringHash paramNames[] = {
        TRESSFX_STRING_HASH("g_Origin"),
        TRESSFX_STRING_HASH("g_CellSize"),
        TRESSFX_STRING_HASH("g_NumCellsX"),
        TRESSFX_STRING_HASH("g_NumCellsY"),
        TRESSFX_STRING_HASH("g_NumCellsZ"),
        TRESSFX_STRING_HASH("g_MaxMarchingCubesVertices"),
        TRESSFX_STRING_HASH("g_MarchingCubesIsolevel"),
        TRESSFX_STRING_HASH("g_CollisionMargin"),
        TRESSFX_STRING_HASH("g_NumHairVerticesPerStrand"),
        TRESSFX_STRING_HASH("g_NumTotalHairVertices"),
        TRESSFX_STRING_HASH("pad1"),
        TRESSFX_STRING_HASH("pad2"),
        TRESSFX_STRING_HASH("pad3"),
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH("tressfxSDFCollisionParameters"),
        sizeof(TressFXSDFCollisionConstantBuffer),
        AMD_ARRAY_SIZE(paramNames),
        paramNames
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_CS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pGenerateSDFLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);

}

void CreateSimLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 7;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
       TRESSFX_STRING_HASH("g_InitialHairPositions"),
       TRESSFX_STRING_HASH("g_GlobalRotations"),
       //TRESSFX_STRING_HASH("g_LocalRotations"),
       TRESSFX_STRING_HASH("g_HairRestLengthSRV"),
       TRESSFX_STRING_HASH("g_HairStrandType"),
       TRESSFX_STRING_HASH("g_HairRefVecsInLocalFrame"),
       TRESSFX_STRING_HASH("g_FollowHairRootOffset"),
       TRESSFX_STRING_HASH("g_BoneSkinningData")
        //TRESSFX_STRING_HASH("g_BoneSkinningMatrix")
    };

    // UAVS
    static const int NUM_UAVS = 0;
    //static const EI_StringHash uavSlotNames[NUM_UAVS] =
    //{
    //    //TRESSFX_STRING_HASH("g_InitialHairPositions"),
    //    /*TRESSFX_STRING_HASH("g_GlobalRotations"),
    //    TRESSFX_STRING_HASH("g_LocalRotations")*/
    //};

    static const EI_StringHash* uavSlotNames = nullptr;


    EI_StringHash paramNames[] = {
        TRESSFX_STRING_HASH("g_Wind"),    TRESSFX_STRING_HASH("g_Wind1"),
        TRESSFX_STRING_HASH("g_Wind2"),   TRESSFX_STRING_HASH("g_Wind3"),
        TRESSFX_STRING_HASH("g_Shape"),   TRESSFX_STRING_HASH("g_GravTimeTip"),
        TRESSFX_STRING_HASH("g_SimInts"), TRESSFX_STRING_HASH("g_Counts"),
        TRESSFX_STRING_HASH("g_VSP"),     TRESSFX_STRING_HASH("g_BoneSkinningMatrix")

#if TRESSFX_COLLISION_CAPSULES
        , TRESSFX_STRING_HASH("g_centerAndRadius0")
        , TRESSFX_STRING_HASH("g_centerAndRadius1")
        , TRESSFX_STRING_HASH("g_numCollisionCapsules")
#endif
    };

    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH("tressfxSDFCollisionParameters"),
        sizeof(TressFXSDFCollisionConstantBuffer),
        AMD_ARRAY_SIZE(paramNames),
        paramNames
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_CS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pSimLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);
}

void CreateApplySDFLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{

    // SRVS
    static const int NUM_SRVS = 0;


    // UAVS
    static const int NUM_UAVS = 2;
    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("g_HairVertices"),
        TRESSFX_STRING_HASH("g_PrevHairVertices")
    };



    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        0,
        nullptr,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_CS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pApplySDFLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);
}

void CreateBoneSkinningLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 2;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("g_BoneSkinningData"),
        TRESSFX_STRING_HASH("initialVertexPositions")
    };

    // UAVS
    static const int NUM_UAVS = 1;
    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("collMeshVertexPositions")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_CS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pBoneSkinningLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);

}

void CreateSDFMarchingCubesLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 2;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("g_MarchingCubesEdgeTable"),
        TRESSFX_STRING_HASH("g_MarchingCubesTriangleTable")
    };

    // UAVS
    static const int NUM_UAVS = 3;
    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("g_MarchingCubesTriangleVertices"),
        TRESSFX_STRING_HASH("g_MarchingCubesTriangleIndices"),
        TRESSFX_STRING_HASH("g_NumMarchingCubesVertices")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_CS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pSDFMarchingCubesLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);
}


void CreatePPLLBuildLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{

    // SRVS
    static const int NUM_SRVS = 0;

    // UAVS
    static const int NUM_UAVS = 2;
    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("tRWFragmentListHead"),
        TRESSFX_STRING_HASH("LinkedListUAV")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        0,
        nullptr,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_PS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pPPLLBuildLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);

 }

void CreatePPLLReadLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{

    // SRVS
    static const int NUM_SRVS = 2;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("tFragmentListHead"),
        TRESSFX_STRING_HASH("LinkedListSRV")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        0,
        nullptr,

        constDesc,
        EI_PS
    };


    EI_BindLayout*&  pLayout = g_TressFXLayouts->pPPLLReadLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);
}

void CreateShortCutDepthsAlphaLayout2(EI_Device* pDevice,
                                     EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 0;

    // UAVS
    static const int NUM_UAVS = 1;
    static const EI_StringHash uavSlotNames[NUM_UAVS] =
    {
        TRESSFX_STRING_HASH("RWFragmentDepthsTexture")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        0,
        nullptr,

        NUM_UAVS,
        uavSlotNames,

        constDesc,
        EI_PS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pShortCutDepthsAlphaLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);

}

void CreateShortCutResolveDepthLayout2(EI_Device* pDevice,
                                      EI_LayoutManagerRef layoutManager)
{

    // SRVS
    static const int NUM_SRVS = 1;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("FragmentDepthsTexture")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        0,
        nullptr,

        constDesc,
        EI_PS
    };


    EI_BindLayout*&  pLayout = g_TressFXLayouts->pShortCutResolveDepthLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);

}

void CreateShortCutFillColorsLayout2(EI_Device* pDevice, EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 1;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("FragmentDepthsTexture")
    };


    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        0,
        nullptr,

        constDesc,
        EI_PS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pShortCutFillColorsLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);

}

void CreateShortCutResolveColorLayout2(EI_Device* pDevice,
                                      EI_LayoutManagerRef layoutManager)
{
    // SRVS
    static const int NUM_SRVS = 3;
    static const EI_StringHash srvSlotNames[NUM_SRVS] =
    {
        TRESSFX_STRING_HASH("FragmentDepthsTexture"),
        TRESSFX_STRING_HASH("FragmentColorsTexture"),
        TRESSFX_STRING_HASH("tAccumInvAlpha")
    };

    static TressFXConstantBufferDesc constDesc =
    {
        TRESSFX_STRING_HASH(""),
        0,
        0,
        nullptr
    };

    static const TressFXLayoutDescription layoutDesc =
    {
        NUM_SRVS,
        srvSlotNames,

        0,
        nullptr,

        constDesc,
        EI_PS
    };

    EI_BindLayout*&  pLayout = g_TressFXLayouts->pShortCutResolveColorLayout;

    if (pLayout != nullptr)
        EI_DestroyLayout(pDevice, pLayout);

    pLayout = EI_CreateLayout(pDevice, layoutManager, layoutDesc);
}

#if ENABLE_ROV_TEST
void CreateShortCutROVBuildLayout(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::ShortCutROVBuild& slots        = g_TressFXSlots->shortCutROVBuild;
    EI_BindLayout&                  engineLayout = g_TressFXLayouts->shortCutROVBuildLayout;

    slots.uavSlots[TRESSFX_IDUAV_SHORTCUT_ROV_HEADS] =
        EI_GetUAVSlot(layoutManager, TRESSFX_STRING_HASH("tRWFragmentListHead"));
    slots.uavSlots[TRESSFX_IDUAV_SHORTCUT_ROV_NODES] =
        EI_GetUAVSlot(layoutManager, TRESSFX_STRING_HASH("RWShortCutROVFragmentData"));

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nUAVs    = AMD_ARRAY_SIZE(slots.uavSlots);
    tressfxLayout.uavSlots = slots.uavSlots;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateShortCutROVReadLayout(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::ShortCutROVRead& slots        = g_TressFXSlots->shortCutROVRead;
    EI_BindLayout&                 engineLayout = g_TressFXLayouts->shortCutROVReadLayout;

    slots.srvSlots[TRESSFX_IDSRV_SHORTCUT_ROV_HEADS] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("tFragmentListHead"));
    slots.srvSlots[TRESSFX_IDSRV_SHORTCUT_ROV_NODES] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("ShortCutROVFragmentData"));
    slots.srvSlots[TRESSFX_IDSRV_SHORTCUT_ROV_INVALPHA] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("tAccumInvAlpha"));

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nSRVs    = AMD_ARRAY_SIZE(slots.srvSlots);
    tressfxLayout.srvSlots = slots.srvSlots;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateKBufferMutexBuildLayout(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::KBufferMutexBuild& slots        = g_TressFXSlots->kBufferMutexBuild;
    EI_BindLayout&                   engineLayout = g_TressFXLayouts->kBufferMutexBuildLayout;

    slots.uavSlots[TRESSFX_IDUAV_KBUFFER_MUTEX_HEADS] =
        EI_GetUAVSlot(layoutManager, TRESSFX_STRING_HASH("tRWFragmentListHead"));
    slots.uavSlots[TRESSFX_IDUAV_KBUFFER_MUTEX_NODES] =
        EI_GetUAVSlot(layoutManager, TRESSFX_STRING_HASH("RWFragmentArray"));

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nUAVs    = AMD_ARRAY_SIZE(slots.uavSlots);
    tressfxLayout.uavSlots = slots.uavSlots;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateKBufferMutexReadLayout(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::KBufferMutexRead& slots        = g_TressFXSlots->kBufferMutexRead;
    EI_BindLayout&                  engineLayout = g_TressFXLayouts->kBufferMutexReadLayout;

    slots.srvSlots[TRESSFX_IDSRV_KBUFFER_MUTEX_HEADS] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("tFragmentListHead"));
    slots.srvSlots[TRESSFX_IDSRV_KBUFFER_MUTEX_NODES] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("FragmentArray"));

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nSRVs    = AMD_ARRAY_SIZE(slots.srvSlots);
    tressfxLayout.srvSlots = slots.srvSlots;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateKBufferROVBuildLayout(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::KBufferROVBuild& slots        = g_TressFXSlots->kBufferROVBuild;
    EI_BindLayout&                 engineLayout = g_TressFXLayouts->kBufferROVBuildLayout;

    slots.uavSlots[TRESSFX_IDUAV_KBUFFER_ROV_HEADS] =
        EI_GetUAVSlot(layoutManager, TRESSFX_STRING_HASH("tRWFragmentListHead"));
    slots.uavSlots[TRESSFX_IDUAV_KBUFFER_ROV_NODES] =
        EI_GetUAVSlot(layoutManager, TRESSFX_STRING_HASH("RWFragmentArray"));

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nUAVs    = AMD_ARRAY_SIZE(slots.uavSlots);
    tressfxLayout.uavSlots = slots.uavSlots;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateKBufferROVReadLayout(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::KBufferROVRead& slots        = g_TressFXSlots->kBufferROVRead;
    EI_BindLayout&                engineLayout = g_TressFXLayouts->kBufferROVReadLayout;

    slots.srvSlots[TRESSFX_IDSRV_KBUFFER_ROV_HEADS] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("tFragmentListHead"));
    slots.srvSlots[TRESSFX_IDSRV_KBUFFER_ROV_NODES] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("FragmentArray"));

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nSRVs    = AMD_ARRAY_SIZE(slots.srvSlots);
    tressfxLayout.srvSlots = slots.srvSlots;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateRenderPosTanLayoutROVTest(EI_Device* pDevice,
                                     EI_LayoutManager layoutManager)
{
    TressFXSlots::RenderPosTan& slots        = g_TressFXSlots->renderPosTanROVTest;
    EI_BindLayout&              engineLayout = g_TressFXLayouts->renderPosTanLayoutROVTest;

    slots.srvSlots[TRESSFX_IDSRV_POS_OFFSET] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("g_GuideHairVertexPositions"));
    slots.srvSlots[TRESSFX_IDSRV_TAN_OFFSET] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("g_GuideHairVertexTangents"));


    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nSRVs    = AMD_ARRAY_SIZE(slots.srvSlots);
    tressfxLayout.srvSlots = slots.srvSlots;
    tressfxLayout.stage    = EI_VS;
    engineLayout           = EI_CreateLayout(pDevice, tressfxLayout);
}

void CreateRenderLayoutROVTest(EI_Device* pDevice, EI_LayoutManager layoutManager)
{
    TressFXSlots::RenderSlots& slots        = g_TressFXSlots->renderSlotsROVTest;
    EI_BindLayout&             engineLayout = g_TressFXLayouts->renderLayoutROVTest;

    slots.srvSlots[TRESSFX_IDSRV_THICKNESS_OFFSET] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("g_HairThicknessCoeffs"));
    slots.srvSlots[TRESSFX_IDSRV_ROOT_TEXCOORD_OFFSET] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("g_HairStrandTexCd"));
    slots.srvSlots[TRESSFX_IDSRV_COLOR_TEXTURE_OFFSET] =
        EI_GetSRVSlot(layoutManager, TRESSFX_STRING_HASH("g_txHairColor"));

    // TODO This should probably move to another location.
    // slots.parameters[TRESSFX_ID_NUMVERTS_OFFSET] = EI_GetParameter(layoutManager,
    // TRESSFX_STRING_HASH("g_NumVerticesPerStrand"));

    EI_StringHash paramNames[] = { "g_NumVerticesPerStrand" };
    slots.pCB                  = EI_CreateConstantBuffer(pDevice,
                                        layoutManager,
                                        TRESSFX_STRING_HASH("tressfxShadeParameters"),
                                        sizeof(TressFXRenderingConstantBuffer),
                                        1,
                                        paramNames);

    TressFXBindLayout tressfxLayout;
    Zero(tressfxLayout);

    tressfxLayout.nSRVs    = AMD_ARRAY_SIZE(slots.srvSlots);
    tressfxLayout.srvSlots = slots.srvSlots;
    tressfxLayout.pCB      = slots.pCB;
    // tressfxLayout.nConstants = AMD_ARRAY_SIZE(slots.parameters);
    // tressfxLayout.constants = slots.parameters;
    engineLayout = EI_CreateLayout(pDevice, tressfxLayout);
}
#endif

void DestroyAllLayouts(EI_Device* pDevice)
{
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pRenderLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pRenderPosTanLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pSimPosTanLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pGenerateSDFLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pSimLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pApplySDFLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pBoneSkinningLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pSDFMarchingCubesLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pPPLLBuildLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pPPLLReadLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pShortCutDepthsAlphaLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pShortCutResolveDepthLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pShortCutFillColorsLayout);
    EI_DestroyLayout(pDevice, g_TressFXLayouts->pShortCutResolveColorLayout);
}

