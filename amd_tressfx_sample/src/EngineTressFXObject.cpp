//---------------------------------------------------------------------------------------
// EngineTressFXObject is just a wrapper to work with Sushi's light management system.
// This sample draws directly through the plugin, rather than through the sushi object 
// management system, so all the draw calls are "not implemented".
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

#include "EngineTressFXObject.h"

#include "SuEffect.h"
#include "SuEffectManager.h"
#include "SuObjectManager.h"
#include "TressFXLayouts.h"

#ifndef GPU_TIMER_START
#include "SuRenderManager.h"
#define GPU_TIMER_START(x) SuRenderManager::GetRef().StartGPUTimer(x);
#define GPU_TIMER_STOP(x) SuRenderManager::GetRef().StopGPUTimer(x);
#endif

/// Default constructor
EngineTressFXObject::EngineTressFXObject(const SuString&    rName,
                                         TressFXHairObject* pTressFXHairObject)
    : SuObject(rName), m_pTressFXHairObject(pTressFXHairObject)
{
    // this->AddProperty("TressFX");
    // SuObjectManager::GetRef().RegisterPluginObject(this);

    // This version keeps the bounding box infinite for rendering purposes.
    // ideally, it would be computed on the GPU during simulation, and
    // view frustum culling would be done there as well, but most engines
    // don't do view frustum culling there (although they are moving that direction).
    // Otherwise, there needs to be a readback.


    SetBoundingVolume(SuBoundingBox(SuPoint3(-SU_FLT_MAX, -SU_FLT_MAX, -SU_FLT_MAX),
                                    SuPoint3(SU_FLT_MAX, SU_FLT_MAX, SU_FLT_MAX)));
}

/// Destructor
EngineTressFXObject::~EngineTressFXObject()
{
    // SuObjectManager::GetRef().RegisterPluginObject(this);
}


/// Implements the SuDrawableObject interface.  \sa SuDrawableObject
bool EngineTressFXObject::Draw(uint32 nInstances)
{
    SuLogError("Not Implemented");
    return false;
}


/// Implements the SuDrawableObject interface.  \sa SuDrawableObject
bool EngineTressFXObject::Draw(const SuString& rEffectTechniqueName, uint32 nInstances)
{
    SuLogError("Not Implemented");
    return false;
}

bool EngineTressFXObject::Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances)
{
    SuLogError("Not Implemented");
    return false;
}

bool EngineTressFXObject::HasMatchingTechnique(uint32 nProperties, const SuString* pProperties)
{
    SuLogError("Not Implemented");
    return false;
}

bool EngineTressFXObject::UpdateObject(SuTime fTime) { return true; }
