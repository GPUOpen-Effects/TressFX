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

#ifndef _SU_BATCHLINERENDERER_H_
#define _SU_BATCHLINERENDERER_H_

#include "SuVector4.h"
#include "SuPoint3.h"
#include "SuMatrix4.h"

#include "SuEffect.h"
#include "SuEffectParameter.h"
#include "SuGPUVertexBuffer.h"
#include "SuCountedPtr.h"
#include "SuArray.h"

typedef SuCountedPtr<SuGPUVertexBuffer> SuGPUVertexBufferPtr;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief This is a utility class for drawing line primitives in bulk
/// 
///  This class provides a simple mechanism for rendering large numbers of line primitives with constant colors.  This class gives
///   a simple immediate mode interface for drawing lines.  Batches of lines are rendered with the specified color using 'DrawLine'
///   method, or any of the helper functions provided.  
///
///  This class is implemented by batching and deferring all 'DrawLine' calls until the color is changed, or until a 'Flush' call is issued.
///    This means that after drawing a group of lines, you must call 'Flush' to get the lines to actually show up
/// 
//=================================================================================================================================
// tolua_begin
class SuBatchLineRenderer
{
public:

   /// Changes the line color
   SU_DLL SuBatchLineRenderer( );
   
   /// Changes the line color
   SU_DLL ~SuBatchLineRenderer( );

   /// Changes the line color
   SU_DLL void SetColor( const SuVector4& rColor );

   /// Draws a line between two points
   SU_DLL void DrawLine( const SuPoint3& r0, const SuPoint3& r1 );

   /// Helper method that draws the wireframe of an axis-aligned bounding box
   SU_DLL void DrawWireAABB( const SuPoint3& rMin, const SuPoint3& rMax );

   /// Submits any pending geometry
   SU_DLL void Flush();

// tolua_end

private:

   /// Submits the last batch of lines
   void SubmitBatch( );

   /// Allocates or re-allocates the VB used for line drawing
   void AllocateVB( size_t nVertexCount );

   SuMatrix4 m_mTransform;
   SuVector4 m_color;
   SuArray< SuPoint3 > m_currentBatch;


   SuEffectPtr m_pEffect;
   SuGPUVertexBufferPtr m_pLineVB;
   SuEffectParameter* m_pColorParam;
   SuEffectParameter* m_pTransformParam;

// tolua_begin
};
// tolua_end


#endif // _SU_BATCHLINERENDERER_H_
