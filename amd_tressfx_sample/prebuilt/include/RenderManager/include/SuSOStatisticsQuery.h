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

#ifndef _SU_SO_STATISTICS_QUERY_H_
#define _SU_SO_STATISTICS_QUERY_H_

#include "SuTypes.h"

// tolua_begin
//=================================================================================================================================
/// \ingroup RenderManager
/// \brief Represents a hardware stream out statistics query
///  
//=================================================================================================================================
class SuSOStatisticsQuery
{
public:
   /// Destructor
   virtual ~SuSOStatisticsQuery() {}

   /// Begins a stream out statistics query
   ///
   /// All draw calls between a Begin() and End() call contribute to the stream out statistics query result
   virtual bool Begin() = 0;

   /// Ends a stream out statistics query
   ///
   /// All draw calls between a Begin() and End() call contribute to the stream out statistics query result
   virtual bool End() = 0;

   /// Checks availability of the result of an stream out statistics query
   ///
   /// This method checks if an stream out statistics query result is available yet and can return the result.
   /// Unlike GetResultSynchronized() it doesn't flush the GPU pipeline.
   ///
   /// \param pNumPrimitivesWritten     Pointer to where the result number of primitives would be stored
   /// \param pnPrimitivesStorageNeeded Pointer to where the result primitives storage needed value would be stored
   ///
   /// \return true if result is available, false if draw calls belonging to the 
   ///              stream out statistics query are still being processed.
   virtual bool CheckResult( uint32* pNumPrimitivesWritten, uint32* pnPrimitivesStorageNeeded ) = 0;

   /// Returns the number of primitives written (the result of a stream out statistics query), 
   /// flushing the GPU pipeline if necessary
   ///
   /// This method returns the result of a stream out statistics query, specifically, the number of 
   /// primitives written. It might block until all draw calls contributing to the query result have 
   /// been processed by the GPU. This can create pipeline stalls and degrade performance. Use with care.
   ///
   /// \return The number of primitives (that is, points, lines, and triangles) written to the stream-output buffers
   virtual uint32 GetNumPrimitivesWritten() = 0;

   /// Returns the required storage for all written primitives (the result of a stream out statistics query), 
   /// flushing the GPU pipeline if necessary
   ///
   /// This method returns the result of a stream out statistics query, specifically, the required storage for all 
   /// written primitives. It might block until all draw calls contributing to the query result have 
   /// been processed by the GPU. This can create pipeline stalls and degrade performance. Use with care.
   ///
   /// \return Number of primitives that would have been written to the stream-output buffers if there 
   ///         had been enough space for them all
   virtual uint32 GetPrimitivesStorageNeeded() = 0;

   /// Returns the stream out statistics such as the number of primitives written and the required 
   /// storage for all written primitives, flushing the GPU pipeline if necessary
   ///
   /// This method returns the result of a stream out statistics query, specifically, the number of 
   /// primitives written and the required storage for all written primitives. 
   /// It might block until all draw calls contributing to the query result have 
   /// been processed by the GPU. This can create pipeline stalls and degrade performance. Use with care.
   ///
   virtual void GetSOStatisticsResults( uint32* pnPrimitivesWritten, uint32* pnPrimitivesStorageNeeded ) = 0;

// tolua_begin
};
// tolua_end

#endif // _SU_SO_STATISTICS_QUERY_H_
