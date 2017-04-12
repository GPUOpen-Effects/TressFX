//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef _SUDRAWABLEOBJECT_H
#define _SUDRAWABLEOBJECT_H

#include "SuTypes.h"

// forward declaration
class SuCamera;

/// \brief Interface for objects that can be rendered
// tolua_begin
class SuDrawableObject
{
public:
	/// Draws the object using default settings
	///
	/// \return true on success, false otherwise
	virtual bool Draw(uint32 nInstances = 1) = 0;

	/// Draws the object using a particular technique of the object's effect
	///
	/// \param rEffectTechniqueName The name of the technique to use for drawing the object.
	///
	/// \return true on success, false otherwise
	virtual bool Draw(const SuString& rEffectTechniqueName, uint32 nInstances = 1) = 0;

#if 0
	// tolua only understands this syntax, which is not legal C++
	virtual bool Draw(uint32 nProperties, const SuString pProperties[nProperties], uint32 nInstances = 1) = 0;
	virtual bool HasMatchingTechnique(uint32 nProperties, const SuString pProperties[nProperties]) = 0;
#else
// tolua_end

	/// Draws the object using the property list to find a technique for drawing
	///
	/// \param rEffectProperties List of effect technique property strings. The first technique
	///        in the object's effect that matches all the properties will be used for rendering.
	///
	///
	/// \return true on sucess, false otherwise
	virtual bool Draw(uint32 nProperties, const SuString* pProperties, uint32 nInstances = 1) = 0;

	/// Determines whether or not an object has a technique with one of the specified properties
	/// 
	/// \return true if a Draw(nProperties,pProperties) call would do anything, false otherwise
	virtual bool HasMatchingTechnique(uint32 nProperties, const SuString* pProperties) = 0;
// tolua_begin
#endif

};
//tolua_end

#endif
