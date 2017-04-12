

//---------------------------------------------------------------------------------------
// Example code that encapsulates three related objects.
// 1.  The TressFXHairObject
// 2.  An interface to get the current set of bones in world space that drive the hair object.
// 3.  An interface to set up for drawing the strands, such as setting lighting parmeters, etc.
// Normally, you'd probably contain the TressFXObject in the engine wrapper, but we've arranged it this 
// way to focus on the important aspects of integration.
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

#ifndef HAIR_STRAND
#define HAIR_STRAND

#include "AMD_TressFX.h"

#include "SuString.h"
#include "SuTypes.h"
#include "SuObjectManager.h"

class EngineTressFXObject;
class TressFXSimulation;


class HairStrands
{
public:

    static HairStrands* Load(
        const SuString&   sushiModelName,
        const SuString&   tfxFilePath,
        const SuString&   tfxboneFilePath,
        const SuString&   hairObjectName,
        int               numFollowHairsPerGuideHair,
        float             tipSeparationFactor,
        SuGPUSamplingResourceViewPtr colorTextureView);

    static void Destroy(HairStrands* pInstance, EI_Device* pDevice);


    SuObject* GetAsEngineObject() { return (SuObject*) m_pEngineObjectWrapper; }
    TressFXHairHandle GetTressFXHandle() { return m_pStrands; }


    void TransitionSimToRendering(EI_CommandContextRef context);

    void TransitionRenderingToSim(EI_CommandContextRef context); 

    void UpdateBones(EI_CommandContextRef context);

    void Simulate(EI_CommandContextRef context, TressFXSimulation* pSimulation);


private:

    HairStrands();
    ~HairStrands();

    void Initialize(
        const SuString&   sushiModelName,
        const SuString&   tfxFilePath,
        const SuString&   tfxboneFilePath,
        const SuString&   hairObjectName,
        int               numFollowHairsPerGuideHair,
        float             tipSeparationFactor,
        SuGPUSamplingResourceViewPtr colorTextureView);

    void Destroy(EI_Device* pDevice);
    
    TressFXHairObject* m_pStrands;
    EngineTressFXObject* m_pEngineObjectWrapper;
    SuAnimatedModel* m_pSkeleton;
};


#endif


