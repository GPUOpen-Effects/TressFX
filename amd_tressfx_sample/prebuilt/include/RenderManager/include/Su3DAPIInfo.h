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

#ifndef _SU_3DAPIINFO_H_
#define _SU_3DAPIINFO_H_

#include "SuTypes.h"

//=========================================================================================================
/// \ingroup RenderManager
/// \brief Helper class that exposes some information about specific conventions of the currently used
///        3D API
/// 
//=========================================================================================================
// tolua_begin
class Su3DAPIInfo
{
public:
   /// Helper method to determine the Z range in normalized device coordinate space.  This method is needed because
   /// certain algorithms (e.g. view frustum plane extraction) depend on it
   ///
   /// \return true if the normalized device coordinate space goes from -1 to 1 (OGL style)
   ///         false if it goes from 0 to 1 (D3D style)
   static SU_DLL bool IsNDCSpaceSigned();

   /// If this method returns true, it means that the 3D API requires that the scene be flipped around the vertical axis
   /// when rendering to a cube map.  This is necessary in OGL because OGL cube maps are parameterized differently than D3D cubemaps
   static SU_DLL bool NeedCubeMapFlip();

   /// Returns whether pixel and texel centers match up when rendering fullscreen quads. This is not the case in D3D9
   static SU_DLL bool DoPixelAndTexelCentersMatch();

   /// Returns whether render to vertex buffer is supported on the current API.
   static SU_DLL bool IsR2VBSupported();

   /// Returns whether or not MRTs may have independent bit depths.
   static SU_DLL bool QueryMRTIndependentBitDepths();
   
   /// Returns whether GPU tessellation is supported:
   static SU_DLL bool QueryTessellationSupport();

// tolua_end

private:
   /// Disallow instantiation
   Su3DAPIInfo();

// tolua_begin
};
// tolua_end


#endif // _SU_3DAPIINFO_H_
