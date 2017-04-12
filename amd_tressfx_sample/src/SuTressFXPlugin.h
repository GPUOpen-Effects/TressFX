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

#ifndef _SuTressFXPlugin_H_
#define _SuTressFXPlugin_H_

#include "SuPlugin.h"
#include "SuTypes.h"

#include "SushiGPUInterface.h"

#include "SuEffect.h"
#include "SuEffectReloadListener.h"
#include "SuGPUResource.h"


//=================================================================================================================================
/// \ingroup
/// \brief
///
//=================================================================================================================================

#include "TressFXSample.h"


class TressFXHairObject;
class TressFXSDFCollision;
class SuEffect;
class SuObjectLightTable;
class SuLightUpdateHandlerFactory;
class TressFXPPLL;
class SuTressFXSDFMarchingCubes;
class TressFXShortCut;
class TressFXSimulation;



#define ENABLE_LEGACY_LIGHTS 1

// tolua_begin
class SuTressFXPlugin : public SuPlugin, public SuEffectReloadListener
{
public:
    // tolua_end
    SuTressFXPlugin()
        : SuPlugin()
#if ENABLE_LEGACY_LIGHTS
        // ick different convention.
        , m_pObjToLightMapping(0)
        , m_pLightUpdateHandlerFactory(0)
#endif
        , m_bUpdateCollMesh(true)
        , m_bSDFCollisionResponse(true)
    {
    }
    virtual ~SuTressFXPlugin(){};

    virtual const char* GetPluginClassName();

    virtual bool Initialize(lua_State* pState);

    virtual void Unload();

    /// Implements effect reloading
    virtual void OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew);


    // tolua_begin


    void LoadHairAsset();
    void Shutdown();

    void DrawHairShadows();
    void EnableAnimation(bool bEnable);

    void OnResize(int width, int height);
    void Break();
    void DrawHair();

    // Simulation and collision
    void Simulate(SuTime fTime);
    void SignalSimulateStart() { SuLogError("Should not be called."); }
    void WaitSimulateDone();
    void EnableUpdatingCollisionMesh(bool bUpdateCollMesh) { m_bUpdateCollMesh = bUpdateCollMesh; }
    void EnableSDFCollisionResponse(bool bSDFCollisionResponse) { m_bSDFCollisionResponse = bSDFCollisionResponse;  }

    void UpdateSimulationParameters(const TressFXSimulationSettings& mohawkParameters,
                                    const TressFXSimulationSettings& shortHairParameters);
    void ToggleShortCut();

    // debug drawing
    void DrawCollisionMesh();
    void DrawSDF();

    int       GetNumTressFXObjects() { return m_pSample->GetNumTressFXObjects(); }
    SuObject* GetTressFXObject(int index) { return m_pSample->GetTressFXObject(index); }

    // tolua_end

private:

    TressFXSample* m_pSample;

    bool m_bUpdateCollMesh;
    bool m_bSDFCollisionResponse;


#if ENABLE_LEGACY_LIGHTS
    // Light management
    SuObjectLightTable*          m_pObjToLightMapping;
    SuLightUpdateHandlerFactory* m_pLightUpdateHandlerFactory;
#endif


    // tolua_begin
};
// tolua_end


#endif  // _SuTressFXPlugin_H_
