// ----------------------------------------------------------------------------
// Interface to the demo
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

#include "PCH.h"


#include "SuLog.h"
#include "SuString.h"
#include "SuTressFXPlugin.h"
#include "SuTressFXPluginToluaBinding.h"
#include "SuTypes.h"
#include "TressFXAsset.h"
#include "tolua++.h"
#include <windows.h>

#if ENABLE_LEGACY_LIGHTS
#include "SuBatchLineRenderer.h"
#include "SuLightManager.h"
#include "SuLightUpdateHandlerFactory.h"
#include "SuObjectLightTable.h"
#include "SuScriptManager.h"
#endif

#include "SuGPUResourceManager.h"
#include "SuObjectManager.h"
#include "SuWindowManager.h"


#include "SuGUIManager.h"
#include "SuObjectManager.h"
#include "SuEffectManager.h"


// This could instead be retrieved as a variable from the
// script manager, or passed as an argument.

// plugin export function
extern "C" {
__declspec(dllexport) SuPlugin* SuPluginCreate()
{
    static SuTressFXPlugin plug;
    return &plug;
}
}

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================


//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================




const char* SuTressFXPlugin::GetPluginClassName()
{
    static const char nameString[] = "SuTressFXPlugin";
    return nameString;
}

bool SuTressFXPlugin::Initialize(lua_State* pState)
{
    tolua_SuTressFXPlugin_open(pState);



#if ENABLE_LEGACY_LIGHTS
    m_pObjToLightMapping = new SuObjectLightTable;
    SU_ASSERT(m_pObjToLightMapping != NULL);
    m_pLightUpdateHandlerFactory = new SuLightUpdateHandlerFactory(*m_pObjToLightMapping);
    SU_ASSERT(m_pLightUpdateHandlerFactory != NULL);

    m_pLightUpdateHandlerFactory->RegisterFactory();

    SuScriptManager::GetRef().SetVariable(
        "Sushi.objToLightMapping", m_pObjToLightMapping, "SuObjectLightTable");
#endif


    m_pSample = new TressFXSample;

    SuEffectManager::GetRef().AddReloadListener(this);

    return true;
}

void SuTressFXPlugin::Shutdown()
{
    delete m_pSample;

}

void SuTressFXPlugin::Unload()
{
#if ENABLE_LEGACY_LIGHTS
    SuLightManager::ShutDown();
    SU_SAFE_DELETE(m_pLightUpdateHandlerFactory);
    SU_SAFE_DELETE(m_pObjToLightMapping);
#endif

 

    SuEffectManager::GetRef().RemoveReloadListener(this);
    //UnloadEffects();
}





// Get a model by its name. This may return nullptr if the object doesn't exist or fails to convert
// to SuAnimatedModel type.
SuAnimatedModel* GetModelByName(const SuString& modelname)
{
    SuObject* pObject = SuObjectManager::GetRef().GetObjectByName(modelname);

    if (pObject == nullptr)
    {
        SuLogError("Could not find object called %s", modelname);
        return nullptr;
    }

    return dynamic_cast<SuAnimatedModel*>(pObject);
}

void SuTressFXPlugin::EnableAnimation(bool bEnable)
{
    GetModelByName("RatBoy_body")->ToggleAnimation(bEnable);
    GetModelByName("RatBoy_left_hand")->ToggleAnimation(bEnable);
    GetModelByName("RatBoy_right_hand")->ToggleAnimation(bEnable);
    GetModelByName("RatBoy_tail")->ToggleAnimation(bEnable);
}

void SuTressFXPlugin::Simulate(SuTime fTime)
{

    m_pSample->Simulate(fTime, m_bUpdateCollMesh, m_bSDFCollisionResponse);

}


// Submit existing drawing commands to overlap with simulation.
// Have graphics queue wait until simulation buffers complete to draw hair.
// Must call after Simulate()
void SuTressFXPlugin::WaitSimulateDone()
{
    m_pSample->WaitSimulateDone();
}


void SuTressFXPlugin::UpdateSimulationParameters(
    const TressFXSimulationSettings& mohawkParameters,
    const TressFXSimulationSettings& shortHairParameters)
{
    m_pSample->UpdateSimulationParameters(mohawkParameters, shortHairParameters);
}

void SuTressFXPlugin::ToggleShortCut()
{
    m_pSample->ToggleShortCut();
}

void SuTressFXPlugin::DrawCollisionMesh()
{
    m_pSample->DrawCollisionMesh();
}


void SuTressFXPlugin::DrawSDF()
{
    m_pSample->DrawSDF();
}



#include "SuMaterial.h"
#include "SuMaterialManager.h"
#include "SuObjectUtils.h"
static SuGPUSamplingResourceViewPtr GetColorTextureViewFromMaterial(const SuString& slotName,
                                                                    const SuString& materialName)
{
    SuMaterial*     pMaterial       = SuMaterialManager::GetRef().GetMaterialByName(materialName);
    SuSlotViewArray textureBindings = pMaterial->GetTextureBindings();
    // find the color texture.
    for (int i = 0; i < textureBindings.size(); ++i)
    {
        SuSlotViewPair pair = textureBindings[i];
        if (pair.first->GetName() == slotName)
            return pair.second;
    }
    return SuGPUSamplingResourceViewPtr(0);
}




// TODO: Loading asset function should be called from OnAssetReload?
void SuTressFXPlugin::LoadHairAsset()
{

    SuGPUSamplingResourceViewPtr ratColorTextureView;

    ratColorTextureView = GetColorTextureViewFromMaterial("tColor", "Ratboy_Body_MAT");
    if (!ratColorTextureView)
    {
        SuLogWarning("Could not find rat color texture.");
    }

    m_pSample->LoadHairAsset(ratColorTextureView);



    // enable animation
    EnableAnimation(true);
}


//=================================================================================================================================
//=================================================================================================================================
void SuTressFXPlugin::OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew)
{
    m_pSample->OnEffectReload(pOld, pNew);
}

void SuTressFXPlugin::OnResize(int width, int height)
{
    m_pSample->OnResize(width, height);
}


void SuTressFXPlugin::DrawHairShadows()
{
    m_pSample->DrawHairShadows();
}

void SuTressFXPlugin::DrawHair()
{
    m_pSample->DrawHair();
}







void SuTressFXPlugin::Break() { SuLogInfo("Break"); }
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
