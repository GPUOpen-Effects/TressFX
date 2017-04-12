// ----------------------------------------------------------------------------
// Brings together all the TressFX components.
// ----------------------------------------------------------------------------
//
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

#include "TressFXSample.h"

#include "PPLLExample.h"
#include "ShortCut.h"

#include "SuLog.h"
#include "SuString.h"
#include "SuTypes.h"
#include "TressFXAsset.h"

#include "SuEffectManager.h"
#include "SuEffectTechnique.h"
#include "SuGPUResourceManager.h"
#include "SuObjectManager.h"


#include "AMD_TressFX.h"



#include "TressFXLayouts.h"

#include "SushiGPUInterface.h"

#include "HairStrands.h"
#include "SDF.h"
#include "Simulation.h"

// This could instead be retrieved as a variable from the
// script manager, or passed as an argument.
static const size_t AVE_FRAGS_PER_PIXEL = 4;
static const size_t PPLL_NODE_SIZE = 16;



// See TressFXLayouts.h
// By default, app allocates space for each of these, and TressFX uses it.
// These are globals, because there should really just be one instance.
TressFXLayouts*                        g_TressFXLayouts = 0;







extern "C"
{
    void TressFX_DefaultRead(void* ptr, AMD::uint size, EI_Stream* pFile)
    {
        FILE* fp = reinterpret_cast<FILE*>(pFile);
        fread(ptr, size, 1, fp);
    }
    void TressFX_DefaultSeek(EI_Stream* pFile, AMD::uint offset)
    {
        FILE* fp = reinterpret_cast<FILE*>(pFile);
        fseek(fp, offset, SEEK_SET);
    }

    void SuTressFXError(EI_StringHash message)
    {
        SuLogWarning(message);
    }
}




TressFXSample::TressFXSample() : 
  m_eOITMethod(OIT_METHOD_SHORTCUT)
, m_nScreenWidth(0)
, m_nScreenHeight(0)
, m_nPPLLNodes(0)
{
    AMD::g_Callbacks.pfMalloc = malloc;
    AMD::g_Callbacks.pfFree = free;
    AMD::g_Callbacks.pfError = SuTressFXError;

    AMD::g_Callbacks.pfRead = TressFX_DefaultRead;
    AMD::g_Callbacks.pfSeek = TressFX_DefaultSeek;

    AMD::g_Callbacks.pfCreateLayout = SuCreateLayout;
    AMD::g_Callbacks.pfDestroyLayout = SuDestroyLayout;

    AMD::g_Callbacks.pfCreateReadOnlySB = SuCreateReadOnlySB;
    AMD::g_Callbacks.pfCreateReadWriteSB = SuCreateReadWriteSB;
    AMD::g_Callbacks.pfCreateCountedSB = SuCreateCountedSB;

    AMD::g_Callbacks.pfClearCounter = SuClearCounter;
    AMD::g_Callbacks.pfCopy = SuCopy;
    AMD::g_Callbacks.pfMap = SuMap;
    AMD::g_Callbacks.pfUnmap = SuUnmap;

    AMD::g_Callbacks.pfDestroySB = SuDestroy;

    AMD::g_Callbacks.pfCreateRT = SuCreateRT;
    AMD::g_Callbacks.pfCreate2D = SuCreate2D;

    AMD::g_Callbacks.pfClear2D = SuClear2D;

    AMD::g_Callbacks.pfSubmitBarriers = SuSubmitBarriers;

    AMD::g_Callbacks.pfCreateBindSet = SuCreateBindSet;
    AMD::g_Callbacks.pfDestroyBindSet = SuDestroyBindSet;
    AMD::g_Callbacks.pfBind = SuBind;

    AMD::g_Callbacks.pfCreateComputeShaderPSO = SuCreateComputeShaderPSO;
    AMD::g_Callbacks.pfDestroyPSO = SuDestroyPSO;
    AMD::g_Callbacks.pfDispatch = SuDispatch;

    AMD::g_Callbacks.pfCreateIndexBuffer = SuCreateIndexBuffer;
    AMD::g_Callbacks.pfDestroyIB = SuDestroyIB;
    AMD::g_Callbacks.pfDraw = SuDrawIndexedInstanced;

   //m_pFullscreenPass = new FullscreenPass;

}


TressFXSample::~TressFXSample()
{
    //delete m_pFullscreenPass;

    EI_Device* pDevice = GetDevice();

    for (size_t i = 0; i < m_hairStrands.size(); i++)
    {
        //AMD::TressFX_Destroy(&(m_HairObjects[i].first), pDevice);
        HairStrands::Destroy(m_hairStrands[i], pDevice);
    }

    if (m_pPPLL)
    {
        m_pPPLL->Shutdown(pDevice);
        delete m_pPPLL;
    }
    if (m_pShortCut)
    {
        m_pShortCut->Shutdown(pDevice);
        delete m_pShortCut;
    }
    if (m_pSimulation)
    {
        delete m_pSimulation;
    }
#if ENABLE_ROV_TEST
    if (m_pShortCutROV)
    {
        m_pShortCutROV->Destroy(SuGPUResourceManager::GetPtr());
        delete m_pShortCutROV;
    }
    if (m_pKBufferMutex)
    {
        m_pKBufferMutex->Destroy(SuGPUResourceManager::GetPtr());
        delete m_pKBufferMutex;
    }
    if (m_pKBufferROV)
    {
        m_pKBufferROV->Destroy(SuGPUResourceManager::GetPtr());
        delete m_pKBufferROV;
    }
#endif

    for (size_t i = 0; i < m_collisionMeshes.size(); i++)
        CollisionMesh::Destroy(m_collisionMeshes[i],pDevice);

    m_collisionMeshes.clear();


    m_pShortCutBackBufferView = SuGPURenderableResourceViewPtr(0);
    m_pShortCutDepthBufferView = SuGPUDepthStencilResourceViewPtr(0);

    DestroyLayouts();
    UnloadEffects();

}

void TressFXSample::Simulate(double fTime, bool bUpdateCollMesh, bool bSDFCollisionResponse)
{
    m_pSimulation->StartSimulation(fTime, m_hairStrands, m_collisionMeshes, bUpdateCollMesh, bSDFCollisionResponse);
}

void TressFXSample::WaitSimulateDone()
{
    m_pSimulation->WaitOnSimulation();
}


void TressFXSample::ToggleShortCut()
{
    OITMethod newMethod;
    if (m_eOITMethod == OIT_METHOD_PPLL)
        newMethod = OIT_METHOD_SHORTCUT;
    else
        newMethod = OIT_METHOD_PPLL;
    SetOITMethod(newMethod);
}

void TressFXSample::DrawCollisionMesh()
{
    SuCommandListPtr pCommandList = SuRenderManager::GetRef().GetCurrentCommandList();

    for (size_t i = 0; i < m_collisionMeshes.size(); i++)
        m_collisionMeshes[i]->DrawMesh((EI_CommandContextRef)pCommandList);
}



void TressFXSample::DrawSDF()
{
    SuCommandListPtr pCommandList = SuRenderManager::GetRef().GetCurrentCommandList();

#if ENABLE_MARCHING_CUBES
    SuGPUTimerHandle mcTimer = SuRenderManager::GetRef().CreateGPUTimer("Generating Marching Cubes");

    GPU_TIMER_START(mcTimer)

        for (size_t i = 0; i < m_collisionMeshes.size(); i++)
            m_collisionMeshes[i]->GenerateIsoSurface((EI_CommandContextRef)pCommandList);

    GPU_TIMER_STOP(mcTimer)

        UnbindUAVS();  // Engine-required hack to unbind UAVs.

    for (size_t i = 0; i < m_collisionMeshes.size(); i++)
        m_collisionMeshes[i]->DrawIsoSurface((EI_CommandContextRef)pCommandList);
#endif
}



void TressFXSample::InitializeLayouts()
{
    // See TressFXLayouts.h
    // Global storage for layouts.

    if (g_TressFXLayouts == 0)
        g_TressFXLayouts = new TressFXLayouts;

    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();

    EI_LayoutManagerRef renderStrandsLayoutManager = GetLayoutManagerRef(m_pHairStrandsEffect);// &(m_pHairStrandsEffect.get());

                                                                                               // Can these be combined?
    CreateRenderPosTanLayout2(pDevice, renderStrandsLayoutManager);
    CreateRenderLayout2(pDevice, renderStrandsLayoutManager);
    CreatePPLLBuildLayout2(pDevice, renderStrandsLayoutManager);
    CreateShortCutDepthsAlphaLayout2(pDevice, renderStrandsLayoutManager);
    CreateShortCutFillColorsLayout2(pDevice, renderStrandsLayoutManager);


    EI_LayoutManagerRef readLayoutManager = GetLayoutManagerRef(m_pHairResolveEffect);
    CreatePPLLReadLayout2(pDevice, readLayoutManager);
    CreateShortCutResolveDepthLayout2(pDevice, readLayoutManager);
    CreateShortCutResolveColorLayout2(pDevice, readLayoutManager);


#if ENABLE_ROV_TEST
    EI_LayoutManager renderStrandsLayoutManagerROVTest = m_pHairStrandsEffectROVTest.get();
    CreateShortCutROVBuildLayout(pDevice, renderStrandsLayoutManagerROVTest);
    CreateKBufferMutexBuildLayout(pDevice, renderStrandsLayoutManagerROVTest);
    CreateKBufferROVBuildLayout(pDevice, renderStrandsLayoutManagerROVTest);

    EI_LayoutManager readLayoutManagerROVTest = m_pHairResolveEffectROVTest.get();
    CreateShortCutROVReadLayout(pDevice, readLayoutManagerROVTest);
    CreateKBufferMutexReadLayout(pDevice, readLayoutManagerROVTest);
    CreateKBufferROVReadLayout(pDevice, readLayoutManagerROVTest);

    CreateRenderLayoutROVTest(pDevice, renderStrandsLayoutManagerROVTest);
    CreateRenderPosTanLayoutROVTest(pDevice, renderStrandsLayoutManagerROVTest);
#endif

}

void TressFXSample::DestroyLayouts()
{
    if (g_TressFXLayouts != 0)
    {
        EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();
        DestroyAllLayouts(pDevice);

        delete g_TressFXLayouts;
    }
}



// TODO: Loading asset function should be called from OnAssetReload?
void TressFXSample::LoadHairAsset(SuGPUSamplingResourceViewPtr ratColorTextureView)
{

    // TODO Why?
    DestroyLayouts();

    LoadEffects();
    InitializeLayouts();

    m_pPPLL = new PPLLExample;
    m_pShortCut = new ShortCut;
    m_pSimulation = new Simulation;


#if ENABLE_ROV_TEST
    m_pShortCutROV = new TressFXShortCutROV;
    m_pKBufferMutex = new TressFXKBufferMutex;
    m_pKBufferROV = new TressFXKBufferROV;
#endif


    EI_Device* pDevice = GetDevice();

    // Used for uploading initial data
    EI_CommandContextRef uploadCommandContext = (EI_CommandContextRef)SuRenderManager::GetRef().GetCurrentCommandList();

    // load and create hair objects
    hairMohawk = HairStrands::Load(
        "RatBoy_body",
        "Objects\\HairAsset\\Ratboy\\Ratboy_mohawk.tfx",
        "Objects\\HairAsset\\Ratboy\\Ratboy_mohawk.tfxbone",
        "mohawk",
        2, // This is number of follow hairs per one guide hair. It could be zero if there is no follow hair at all. 
        2.0f,
        ratColorTextureView);

    hairShort = HairStrands::Load(
        "RatBoy_body",
        "Objects\\HairAsset\\Ratboy\\Ratboy_short.tfx",
        "Objects\\HairAsset\\Ratboy\\Ratboy_short.tfxbone",
        "hairShort",
        0, // no follow hair
        0.0f,
        ratColorTextureView);

    m_hairStrands.push_back(hairMohawk);
    m_hairStrands.push_back(hairShort);

    // load and create collison meshes
    CollisionMesh* pBody = CollisionMesh::Load(
        "RatBoy_body",
        "Objects\\HairAsset\\Ratboy\\Ratboy_body.tfxmesh",
        50,
        0);
    CollisionMesh* pLeftHand = CollisionMesh::Load(
        "RatBoy_left_hand",
        "Objects\\HairAsset\\Ratboy\\Ratboy_left_hand.tfxmesh",
        18,
        1.5f);
    CollisionMesh* pRightHand = CollisionMesh::Load(
        "RatBoy_right_hand",
        "Objects\\HairAsset\\Ratboy\\Ratboy_right_hand.tfxmesh",
        18,
        1.5f);

    m_collisionMeshes.push_back(pBody);
    m_collisionMeshes.push_back(pLeftHand);
    m_collisionMeshes.push_back(pRightHand);

}

void TressFXSample::LoadEffects()
{
    // if (!m_pHairStrandsEffect)
    {
        m_pHairStrandsEffect = SuEffectManager::GetRef().LoadEffect("oHair.sufx");
        m_pHairResolveEffect = SuEffectManager::GetRef().LoadEffect("qHair.sufx");
#if ENABLE_ROV_TEST
        m_pHairStrandsEffectROVTest = SuEffectManager::GetRef().LoadEffect("oHairROVTest.sufx");
        m_pHairResolveEffectROVTest = SuEffectManager::GetRef().LoadEffect("qHairROVTest.sufx");
#endif
    }
}

void TressFXSample::UnloadEffects()
{
    // if (!m_pHairStrandsEffect)
    {
        m_pHairStrandsEffect = SuEffectPtr(0);
        m_pHairResolveEffect = SuEffectPtr(0);
#if ENABLE_ROV_TEST
        m_pHairStrandsEffectROVTest = SuEffectPtr(0);
        m_pHairResolveEffectROVTest = SuEffectPtr(0);
#endif
    }
}

//=================================================================================================================================
//=================================================================================================================================
void TressFXSample::OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew)
{
    //m_pSample->OnEffectReload(pOld, pNew);

    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();

    if (m_pHairStrandsEffect == pOld)
    {
        //m_pHairStrandsEffect = pNew;
        SetStrandLayoutManager(pNew);
    }

    if (m_pHairResolveEffect == pOld)
    {
        SetPPLLResolveLayoutManager(pNew);
    }
}


void TressFXSample::SetStrandLayoutManager(SuEffectPtr pNew)
{
    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();

    m_pHairStrandsEffect = pNew;

    EI_LayoutManagerRef renderStrandsLayoutManager = GetLayoutManagerRef(m_pHairStrandsEffect);

    // Can these be combined?
    CreateRenderPosTanLayout2(pDevice, renderStrandsLayoutManager);
    CreateRenderLayout2(pDevice, renderStrandsLayoutManager);
    CreatePPLLBuildLayout2(pDevice, renderStrandsLayoutManager);

    CreateShortCutDepthsAlphaLayout2(pDevice, renderStrandsLayoutManager);
    CreateShortCutFillColorsLayout2(pDevice, renderStrandsLayoutManager);
}

void TressFXSample::SetPPLLResolveLayoutManager(SuEffectPtr pNew)
{
    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();

    m_pHairResolveEffect = pNew;

    EI_LayoutManagerRef readLayoutManager = GetLayoutManagerRef(m_pHairResolveEffect);
    CreatePPLLReadLayout2(pDevice, readLayoutManager);

}

void TressFXSample::UpdateSimulationParameters(
    const TressFXSimulationSettings& mohawkParameters,
    const TressFXSimulationSettings& shortHairParameters)
{
    hairMohawk->GetTressFXHandle()->UpdateSimulationParameters(mohawkParameters);
    hairShort->GetTressFXHandle()->UpdateSimulationParameters(shortHairParameters);
}


void TressFXSample::OnResize(int width, int height)
{
    // Engine specifics.
    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();

    m_nScreenWidth = width;
    m_nScreenHeight = height;

    // TressFX Usage
    switch (m_eOITMethod)
    {
    case OIT_METHOD_PPLL:
        m_nPPLLNodes = width * height * AVE_FRAGS_PER_PIXEL;
        m_pPPLL->Shutdown(pDevice);
        m_pPPLL->Initialize(width, height, m_nPPLLNodes,  PPLL_NODE_SIZE, m_pHairStrandsEffect.get(), m_pHairResolveEffect.get());
        break;
    case OIT_METHOD_SHORTCUT:
        m_pShortCut->Shutdown(pDevice);
        m_pShortCut->Initialize(width, height, m_pHairStrandsEffect.get(), m_pHairResolveEffect.get());
        break;
#if ENABLE_ROV_TEST
    case OIT_METHOD_SHORTCUT_ROV:
        m_pShortCutROV->Destroy(pDevice);
        m_pShortCutROV->Create(pDevice, width, height);
        break;
    case OIT_METHOD_KBUFFER_MUTEX:
        m_pKBufferMutex->Destroy(pDevice);
        m_pKBufferMutex->Create(pDevice, width, height);
        break;
    case OIT_METHOD_KBUFFER_ROV:
        m_pKBufferROV->Destroy(pDevice);
        m_pKBufferROV->Create(pDevice, width, height);
        break;
#endif
    };
}


void TressFXSample::DrawHairShadows()
{
    SuGPUTimerHandle shadowTimer = SuRenderManager::GetRef().CreateGPUTimer("Shadow");

    EI_CommandContextRef pRenderCommandList = (EI_CommandContextRef)SuRenderManager::GetRef().GetCurrentCommandList();
    EI_PSO* pso = GetPSOPtr(m_pHairStrandsEffect->GetTechnique("DepthOnly"));
    SU_ASSERT(pso != nullptr);


    for (size_t i = 0; i < m_hairStrands.size(); i++)
    {
        TressFXHairObject* pHair = m_hairStrands[i]->GetTressFXHandle();

        if (pHair)
        {
            GPU_TIMER_START(shadowTimer)
                pHair->DrawStrands(pRenderCommandList, *pso);
            GPU_TIMER_STOP(shadowTimer)
        }
    }
}

void TressFXSample::DrawHair()
{
    SuGPUTimerHandle drawTimer = SuRenderManager::GetRef().CreateGPUTimer("TRESSFX RENDERING TOTAL");

    // Set shader constants for fragment buffer size
    float4 vFragmentBufferSize;
    vFragmentBufferSize.x = (float)m_nScreenWidth;
    vFragmentBufferSize.y = (float)m_nScreenHeight;
    vFragmentBufferSize.z = (float)m_nScreenWidth * m_nScreenHeight;
    vFragmentBufferSize.w = 0;

    m_pHairStrandsEffect->GetParameter("nNodePoolSize")->SetInt(m_nPPLLNodes);
    m_pHairResolveEffect->GetParameter("nNodePoolSize")->SetInt(m_nPPLLNodes);
    m_pHairStrandsEffect->GetParameter("vFragmentBufferSize")
        ->SetFloatVector(&vFragmentBufferSize.x);
    m_pHairResolveEffect->GetParameter("vFragmentBufferSize")
        ->SetFloatVector(&vFragmentBufferSize.x);
#if ENABLE_ROV_TEST
    m_pHairStrandsEffectROVTest->GetParameter("nNodePoolSize")->SetInt(m_nPPLLNodes);
    m_pHairResolveEffectROVTest->GetParameter("nNodePoolSize")->SetInt(m_nPPLLNodes);
    m_pHairStrandsEffectROVTest->GetParameter("vFragmentBufferSize")
        ->SetFloatVector(&vFragmentBufferSize.x);
    m_pHairResolveEffectROVTest->GetParameter("vFragmentBufferSize")
        ->SetFloatVector(&vFragmentBufferSize.x);
#endif

    GPU_TIMER_START(drawTimer);
    switch (m_eOITMethod)
    {
    case OIT_METHOD_PPLL:
        DrawHairPPLL();
        break;
    case OIT_METHOD_SHORTCUT:
        DrawHairShortCut();
        break;
#if ENABLE_ROV_TEST
    case OIT_METHOD_SHORTCUT_ROV:
        DrawHairShortCutROV();
        break;
    case OIT_METHOD_KBUFFER_MUTEX:
        DrawHairKBufferMutex();
        break;
    case OIT_METHOD_KBUFFER_ROV:
        DrawHairKBufferROV();
        break;
#endif
    };
    GPU_TIMER_STOP(drawTimer);
}

void TressFXSample::SetOITMethod(OITMethod method)
{
    if (method == m_eOITMethod)
        return;

    DestroyOITResources(m_eOITMethod);
    m_eOITMethod = method;
    OnResize(m_nScreenWidth, m_nScreenHeight);
}

void TressFXSample::DrawHairPPLL()
{


    m_pPPLL->Draw(m_hairStrands);

}

void TressFXSample::DrawHairShortCut()
{
    EI_CommandContextRef pRenderCommandList = GetContext();
    m_pShortCut->Draw(pRenderCommandList, m_hairStrands);
}

#if ENABLE_ROV_TEST
void SuTressFXPlugin::DrawHairShortCutROV()
{
    SuGPUTimerHandle fillTimer = SuRenderManager::GetRef().CreateGPUTimer("render: SROV Fill");
    SuGPUTimerHandle resolveTimer = SuRenderManager::GetRef().CreateGPUTimer("render: SROV Resolve");

    EI_CommandContext pRenderCommandList = SuRenderManager::GetRef().GetCurrentCommandList();
    EI_PSO            buildPSO =
        const_cast<SuEffectTechnique*>(m_pHairStrandsEffectROVTest->GetTechnique("ShortCutROV"));
    EI_PSO readPSO =
        const_cast<SuEffectTechnique*>(m_pHairResolveEffectROVTest->GetTechnique("ShortCutROV"));

    m_pShortCutROV->Clear(pRenderCommandList);

    if (m_pShortCutBackBufferView == NULL)
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
        m_pShortCutBackBufferView = tBackBuffer->GetRenderableView(tBackBufferViewDesc);
        m_pShortCutDepthBufferView = tDepthBuffer->GetDepthStencilView(tDepthBufferViewDesc);
    }

    // Bind render targets for DepthsAlpha pass and initialize depths
    SuRenderManager::GetRef().SetRenderTargets(
        1, &m_pShortCutROV->GetInvAlphaTexture().rtv, m_pShortCutDepthBufferView);
    SuRenderManager::GetRef().SetClearColor(SuVector4(1, 1, 1, 1));
    SuRenderManager::GetRef().Clear(true, false, false);

    m_pShortCutROV->BindForBuild(pRenderCommandList);  // put clear in here?
    for (size_t i = 0; i < m_HairObjects.size(); i++)
    {
        TressFXHairObject* pHair = m_HairObjects[i].first;

        if (pHair)
        {
            GPU_TIMER_START(fillTimer);
            pHair->DrawStrands(pRenderCommandList,
                GetRenderPosTanLayoutROVTest(),
                GetRenderLayoutROVTest(),
                buildPSO);
            GPU_TIMER_STOP(fillTimer);
        }
    }

    m_pShortCutROV->DoneBuilding(pRenderCommandList);


    // TODO move this to a clear "after all pos and tan usage by rendering" place.
    for (size_t i = 0; i < m_HairObjects.size(); i++)
        m_HairObjects[i].first->GetPosTanCollection().TransitionRenderingToSim(pRenderCommandList);

    UnbindUAVS();  // Engine-required hack to unbind UAVs.


                   // Bind back buffer for blend
    SuRenderManager::GetRef().SetRenderTargets(
        1, &m_pShortCutBackBufferView, m_pShortCutDepthBufferView);

    m_pShortCutROV->BindForRead(pRenderCommandList);
    // Enables lights.  Only lights marked to hit TressFX will be applied.
    if (m_EngineTressFXObjects.size() > 0)
    {
        SuObjectManager::GetRef().SetObjectBeingDrawn(m_EngineTressFXObjects[0]);
        GPU_TIMER_START(resolveTimer);
        DrawFullscreen(pRenderCommandList, readPSO);
        GPU_TIMER_STOP(resolveTimer);
        SuObjectManager::GetRef().SetObjectBeingDrawn(0);
    }

    m_pShortCutROV->DoneReading(pRenderCommandList);
}

void SuTressFXPlugin::DrawHairKBufferMutex()
{
    SuGPUTimerHandle fillTimer = SuRenderManager::GetRef().CreateGPUTimer("render: KMtx Fill");
    SuGPUTimerHandle resolveTimer = SuRenderManager::GetRef().CreateGPUTimer("render: KMtx Resolve");

    EI_CommandContext pRenderCommandList = SuRenderManager::GetRef().GetCurrentCommandList();
    EI_PSO            buildPSO =
        const_cast<SuEffectTechnique*>(m_pHairStrandsEffectROVTest->GetTechnique("MutexDeferred"));
    EI_PSO readPSO =
        const_cast<SuEffectTechnique*>(m_pHairResolveEffectROVTest->GetTechnique("MutexDeferred"));

    m_pKBufferMutex->Clear(pRenderCommandList);

    m_pKBufferMutex->BindForBuild(pRenderCommandList);  // put clear in here?
    for (size_t i = 0; i < m_HairObjects.size(); i++)
    {
        TressFXHairObject* pHair = m_HairObjects[i].first;

        if (pHair)
        {
            GPU_TIMER_START(fillTimer);
            pHair->DrawStrands(pRenderCommandList,
                GetRenderPosTanLayoutROVTest(),
                GetRenderLayoutROVTest(),
                buildPSO);
            GPU_TIMER_STOP(fillTimer);
        }
    }

    m_pKBufferMutex->DoneBuilding(pRenderCommandList);


    // TODO move this to a clear "after all pos and tan usage by rendering" place.
    for (size_t i = 0; i < m_HairObjects.size(); i++)
        m_HairObjects[i].first->GetPosTanCollection().TransitionRenderingToSim(pRenderCommandList);

    UnbindUAVS();  // Engine-required hack to unbind UAVs.


    m_pKBufferMutex->BindForRead(pRenderCommandList);

    if (m_EngineTressFXObjects.size() > 0)
    {
        // Enables lights.  Only lights marked to hit TressFX will be applied.
        SuObjectManager::GetRef().SetObjectBeingDrawn(m_EngineTressFXObjects[0]);
        GPU_TIMER_START(resolveTimer);
        DrawFullscreen(pRenderCommandList, readPSO);
        GPU_TIMER_STOP(resolveTimer);
        SuObjectManager::GetRef().SetObjectBeingDrawn(0);
    }
    m_pKBufferMutex->DoneReading(pRenderCommandList);
}

void SuTressFXPlugin::DrawHairKBufferROV()
{
    SuGPUTimerHandle fillTimer = SuRenderManager::GetRef().CreateGPUTimer("render: KROV Fill");
    SuGPUTimerHandle resolveTimer = SuRenderManager::GetRef().CreateGPUTimer("render: KROV Resolve");

    EI_CommandContext pRenderCommandList = SuRenderManager::GetRef().GetCurrentCommandList();
    EI_PSO            buildPSO = const_cast<SuEffectTechnique*>(
        m_pHairStrandsEffectROVTest->GetTechnique("MutexDeferredROV"));
    EI_PSO readPSO = const_cast<SuEffectTechnique*>(
        m_pHairResolveEffectROVTest->GetTechnique("MutexDeferredROV"));

    m_pKBufferROV->Clear(pRenderCommandList);

    m_pKBufferROV->BindForBuild(pRenderCommandList);  // put clear in here?
    for (size_t i = 0; i < m_HairObjects.size(); i++)
    {
        TressFXHairObject* pHair = m_HairObjects[i].first;

        if (pHair)
        {
            GPU_TIMER_START(fillTimer);
            pHair->DrawStrands(pRenderCommandList,
                GetRenderPosTanLayoutROVTest(),
                GetRenderLayoutROVTest(),
                buildPSO);
            GPU_TIMER_STOP(fillTimer);
        }
    }

    m_pKBufferROV->DoneBuilding(pRenderCommandList);


    // TODO move this to a clear "after all pos and tan usage by rendering" place.
    for (size_t i = 0; i < m_HairObjects.size(); i++)
        m_HairObjects[i].first->GetPosTanCollection().TransitionRenderingToSim(pRenderCommandList);

    UnbindUAVS();  // Engine-required hack to unbind UAVs.


    m_pKBufferROV->BindForRead(pRenderCommandList);
    if (m_EngineTressFXObjects.size() > 0)
    {
        // Enables lights.  Only lights marked to hit TressFX will be applied.
        SuObjectManager::GetRef().SetObjectBeingDrawn(m_EngineTressFXObjects[0]);
        GPU_TIMER_START(resolveTimer);
        DrawFullscreen(pRenderCommandList, readPSO);
        GPU_TIMER_STOP(resolveTimer);
        SuObjectManager::GetRef().SetObjectBeingDrawn(0);
    }
    m_pKBufferROV->DoneReading(pRenderCommandList);
}
#endif


void TressFXSample::DestroyOITResources(OITMethod method)
{
    // Engine specifics.
    EI_Device* pDevice = (EI_Device*)SuGPUResourceManager::GetPtr();


    // TressFX Usage
    switch (m_eOITMethod)
    {
    case OIT_METHOD_PPLL:
        m_pPPLL->Shutdown(pDevice);
        break;
    case OIT_METHOD_SHORTCUT:
        m_pShortCut->Shutdown(pDevice);
        break;
#if ENABLE_ROV_TEST
    case OIT_METHOD_SHORTCUT_ROV:
        m_pShortCutROV->Destroy(pDevice);
        break;
    case OIT_METHOD_KBUFFER_MUTEX:
        m_pKBufferMutex->Destroy(pDevice);
        break;
    case OIT_METHOD_KBUFFER_ROV:
        m_pKBufferROV->Destroy(pDevice);
        break;
#endif
    };
}


SuObject* TressFXSample::GetTressFXObject(int index) { return m_hairStrands[index]->GetAsEngineObject();  }


//=================================================================================================================================
//
//          Protected methods block
//
//=================================================================================================================================


//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================

