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

#ifndef _SUDISPATCHABLEOBJECT_H
#define _SUDISPATCHABLEOBJECT_H

#include "SuTypes.h"

/// \brief Interface for objects that can be rendered
// tolua_begin
class SuDispatchableObject
{
public:
   /// Dispatches the specified technique with an nX by nY by nZ array of thread groups
   ///
   /// \return true on success, false otherwise
   virtual bool Dispatch( const SuString& rEffectTechniqueName, int32 nX, int32 nY, int32 nZ ) = 0;


#if 0
   virtual bool HasMatchingComputeTechnique( uint32 nProperties, const SuString pProperties[nProperties] ) = 0;
#else
   // tolua_end

   /// Determines whether or not an object has a technique with one of the specified properties
   /// 
   /// \return true if a Draw(nProperties,pProperties) call would do anything, false otherwise
   virtual bool HasMatchingComputeTechnique( uint32 nProperties, const SuString* pProperties ) = 0;

   // tolua_begin
#endif

};
//tolua_end

#endif
