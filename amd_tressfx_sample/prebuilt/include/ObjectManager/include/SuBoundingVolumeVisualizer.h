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

#ifndef _SU_BOUNDINGVOLUMEVISUALIZER_H_
#define _SU_BOUNDINGVOLUMEVISUALIZER_H_

#include "SuTypes.h"
#include "SuVector3.h"

class SuBoundingBox;
class SuBoundingSphere;
class SuObject;
class SuBox;
class SuSphere;
class SuMatrix4;

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief An object which knows how to draw bounding volumes
/// 
//=========================================================================================================
// tolua_begin
class SuBoundingVolumeVisualizer
{
public:
   /// Constructor
   SU_DLL SuBoundingVolumeVisualizer();

   /// Destructor
   virtual SU_DLL ~SuBoundingVolumeVisualizer();

   /// Draws a bounding box
   SU_DLL void DrawBoundingBox( const SuBoundingBox& rBB, const SuVector3& rColor = SuVector3(1,1,1) ) const;

   /// Draws a bounding box with a model-world transform applied
   SU_DLL void DrawTransformedBoundingBox( const SuBoundingBox& rBB, const SuMatrix4& rTransform, const SuVector3& rColor = SuVector3(1,1,1) ) const ;

   /// Draws a bounding sphere
   SU_DLL void DrawBoundingSphere( const SuBoundingSphere& rBS, const SuVector3& rColor = SuVector3(1,1,1) ) const;


// tolua_end

private:
   SuBox*    m_pBox;
   SuSphere* m_pSphere;
// tolua_begin
};
// tolua_end


#endif // _SU_BOUNDINGVOLUMEVISUALIZER_H_
