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


#pragma once

#include "SuObject.h"
#include "TressFXHairObject.h"


class EngineTressFXObject : public SuObject
{
public:
    /// Default constructor
    EngineTressFXObject(const SuString& rName, TressFXHairObject* pTressFXHairObject);

    /// Destructor
    virtual ~EngineTressFXObject();


    /// Implements the SuDrawableObject interface.  \sa SuDrawableObject
    virtual bool Draw(uint32 nInstances = 1);

    /// Implements the SuDrawableObject interface.  \sa SuDrawableObject
    virtual bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances = 1);

    virtual bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances = 1);
    virtual bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties);

    virtual bool UpdateObject(SuTime fTime);

    TressFXHairHandle GetTressFXHandle() { return m_pTressFXHairObject;  }

private:
    // lifetime managed externally.
    TressFXHairObject* m_pTressFXHairObject;
};



