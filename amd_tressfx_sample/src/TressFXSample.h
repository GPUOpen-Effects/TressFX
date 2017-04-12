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

#ifndef TRESSFX_SAMPLE_H
#define TRESSFX_SAMPLE_H

#include "SuTypes.h"

#include "SushiGPUInterface.h"

#include "EngineTressFXObject.h"
#include "SuAnimatedModel.h"
#include "SuCommandQueue.h"
#include "SuEffect.h"
#include "SuEffectReloadListener.h"
#include "SuGPUResource.h"

#include "TressFXAsset.h"
#include "TressFXGPUInterface.h"
#include "TressFXSDFCollision.h"
#include "TressFXSettings.h"

#include <vector>

//=================================================================================================================================
/// \ingroup
/// \brief
///
//=================================================================================================================================


class PPLLExample;
class ShortCut;
class CollisionMesh;
class HairStrands;
class Simulation;

//class TressFXHairObject;
class SuEffect;
//class TressFXShortCut;
//class TressFXSimulation;

#if ENABLE_ROV_TEST
class TressFXShortCutROV;
class TressFXKBufferMutex;
class TressFXKBufferROV;
#endif


#ifdef SU_3D_API_D3D12
#define ENABLE_ASYNC_COMPUTE 1
#else
#define ENABLE_ASYNC_COMPUTE 0
#endif


//class SuTressFXBoneSkinning;

// Generic interface to get the data we want from the engine.
class IEngineInterface
{
public:
    //void GetBoneMatrices(int i, &nBones, &boneMatrices);
    //void SetLights(int i);
    // EI_LayoutManagerRef GetDrawStrandsLayoutManager();
    // EI_ ... GetQuadLayoutManager()
    // EI_ ... GetSimLayoutManager()

};



class FullscreenPass;

class TressFXSample 
{
public:
    TressFXSample();
    ~TressFXSample();

    void LoadHairAsset(SuGPUSamplingResourceViewPtr colorTextureView);

    void DrawHairShadows();

    void OnResize(int width, int height);

    void OnEffectReload(SuEffectPtr pOld, SuEffectPtr pNew);
    void SetStrandLayoutManager(SuEffectPtr pEffect);
    void SetPPLLResolveLayoutManager(SuEffectPtr pEffect);

    void DrawHair();

    // Simulation and collision
    void Simulate(SuTime fTime, bool bUpdateCollMesh, bool bSDFCollisionResponse);
    void WaitSimulateDone();
    void SetSDFCollisionMargin(float collisionMargin);
    void UpdateSimulationParameters(const TressFXSimulationSettings& mohawkParameters,
        const TressFXSimulationSettings& shortHairParameters);
    void ToggleShortCut();

    // debug drawing
    void DrawCollisionMesh();
    void DrawSDFGrid();
    void DrawSDF();
    void SetSDFIsoLevel(float isoLevel);

    int       GetNumTressFXObjects() { return static_cast<int>(m_hairStrands.size()); }
    SuObject* GetTressFXObject(int index); 


private:

    std::vector<CollisionMesh*> m_collisionMeshes;
    std::vector<HairStrands*> m_hairStrands;

    // to keep track of which is which in the above array;
    HairStrands*        hairMohawk;
    HairStrands*        hairShort;

    PPLLExample*     m_pPPLL;
    ShortCut* m_pShortCut;
    Simulation* m_pSimulation;

#if ENABLE_ROV_TEST
    TressFXShortCutROV*  m_pShortCutROV;
    TressFXKBufferMutex* m_pKBufferMutex;
    TressFXKBufferROV*   m_pKBufferROV;
#endif

    // Views for back and depth buffer created in Normal.lua
    SuGPURenderableResourceViewPtr   m_pShortCutBackBufferView;
    SuGPUDepthStencilResourceViewPtr m_pShortCutDepthBufferView;

    enum OITMethod
    {
        OIT_METHOD_PPLL,
        OIT_METHOD_SHORTCUT
#if ENABLE_ROV_TEST
        ,
        OIT_METHOD_SHORTCUT_ROV,
        OIT_METHOD_KBUFFER_MUTEX,
        OIT_METHOD_KBUFFER_ROV
#endif
    };

    OITMethod m_eOITMethod;
    int       m_nScreenWidth;
    int       m_nScreenHeight;
    int       m_nPPLLNodes;

    // Need to handle these better.
    void        LoadEffects();
    void        UnloadEffects();
    SuEffectPtr m_pHairStrandsEffect;
    SuEffectPtr m_pHairResolveEffect;

#if ENABLE_ROV_TEST
    SuEffectPtr m_pHairStrandsEffectROVTest;
    SuEffectPtr m_pHairResolveEffectROVTest;
#endif

    SuCountedPtr<SuGPUResource> m_pHairColorTexture;

    void InitializeLayouts();
    void DestroyLayouts();
    void DrawHairPPLL();
    void DrawHairShortCut();
#if ENABLE_ROV_TEST
    void DrawHairShortCutROV();
    void DrawHairKBufferMutex();
    void DrawHairKBufferROV();
#endif

    void SetOITMethod(OITMethod method);
    void DestroyOITResources(OITMethod method);


};


#endif  // TRESSFX_SAMPLE_H


