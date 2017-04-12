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

#ifndef _SUAUTODRAWABLEOBJECT_H
#define _SUAUTODRAWABLEOBJECT_H

#include "SuTypes.h"
#include "SuRenderManager.h"


/// \brief Interface for objects that can be rendered with a streaming output buffer
// tolua_begin
class SuAutoDrawableObject
{
public:
      
   /// Draws the object using a particular technique of the object's effect using data written to a 
   /// streaming output buffer. Note that this type of draw call will only work in DirectX10.
   ///
   /// \param ePrimType            The primitive type. Note that this should only be POINT_LIST, TRIANGLE_LIST or LINE_LIST. 
   ///                             All other choices will generate an error.
   /// \param rEffectTechniqueName The name of the technique to use for drawing the object.
   ///
   /// \return true on success, false otherwise or if the user specified an unsupported primitive type
   virtual bool DrawAuto( SuRenderManager::PrimType ePrimType, const SuString& rEffectTechniqueName ) = 0;

   /// Draws the object using a particular technique of the object's effect using data written to a 
   /// streaming output buffer. Note that this type of draw call will only work in DirectX10.
   ///
   /// \param rEffectTechniqueName The name of the technique to use for drawing the object.
   ///
   /// \return true on success, false otherwise or if the user specified an unsupported primitive type
   virtual bool DrawAuto( const SuString& rEffectTechniqueName ) = 0;


}; // End of SuAutoDrawableObject interface declaration

//tolua_end

#endif   // End of _SUAUTODRAWABLEOBJECT_H
