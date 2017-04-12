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

#ifndef _SU_OCCLUSIONQUERY_H_
#define _SU_OCCLUSIONQUERY_H_

#include "SuTypes.h"

// tolua_begin
//=================================================================================================================================
/// \ingroup RenderManager
/// \brief Represents a hardware occlusion query
///  
//=================================================================================================================================
class SuOcclusionQuery
{
public:
   /// Destructor
   virtual SU_DLL ~SuOcclusionQuery() {}

   /// Begins an occlusion query
   ///
   /// All draw calls between a Begin() and End() call contribute to the occlusion query result
   virtual bool Begin() = 0;

   /// Ends an occlusion query
   ///
   /// All draw calls between a Begin() and End() call contribute to the occlusion query result
   virtual bool End() = 0;

   /// Checks availability of the result of an occlusion query
   ///
   /// This method checks if an occlusion query result is available yet and can return the result.
   /// Unlike GetResultSynchronized() it doesn't flush the GPU pipeline.
   ///
   /// \param pData Pointer to where result will be stored if it is available. May be NULL.
   ///
   /// \return true if result is available, false if draw calls belonging to the occlusion query are still being processed.
   virtual bool CheckResult( uint32* pData ) = 0;

   /// Returns result of an occlusion query, flushing the GPU pipeline if necessary
   ///
   /// This method returns the result of an occlusion query. It might block until all draw calls contributing to the query
   /// result have been processed by the GPU. This can create pipeline stalls and degrade performance. Use with care.
   ///
   /// \return The count of fragment that passed Z-, stencil-, and alpha tests.
   virtual uint32 GetResultSynchronized() = 0;

// tolua_begin
};
// tolua_end

#endif // _SU_OCCLUSIONQUERY_H_
