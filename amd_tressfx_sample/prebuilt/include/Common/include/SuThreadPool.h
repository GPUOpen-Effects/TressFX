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

#ifndef _Su_THREAD_POOL_H_
#define _Su_THREAD_POOL_H_

#include "SuWorkItem.h"
#include "SuSignal.h"
#include "SuInterlockedCounter.h"

class SuWorkItemThread;

//=====================================================================================================================
/// \brief The work item processor maintains a pool of worker threads and uses them to execute work items in parallel
///
/// To issue a set of tasks in parallel, simply generate the appropriate work item objects, and pass them to the 
/// ExecuteWorkItems method.  The work item processor will distribute these work items to a pool of worker threads
/// in a round-robin fashion.  This fact can and should be exploited by the implementations of the work item classes, in order
/// to provide more coherent memory access.  
///
//=====================================================================================================================
class SuThreadPool
{
public:
   
   /// Creates a work item processor with the specified number of worker threads
   static SU_DLL SuThreadPool* Create( uint32 nThreads );

   virtual SU_DLL  ~SuThreadPool();

   /// Executes a set of work items and returns when they are completed
   SU_DLL void ExecuteWorkItems( const SuWorkItemArray& rWorkItems );

   /// Returns the number of threads that the manager will distribute work items across
   /// This can be useful in determining how to partition the workload
   inline uint32 GetTotalThreads() const { return (uint32) m_threads.size()+1; };

private:

   SuThreadPool() ; ///< Restrict instantiation to Create() factory method

   /// Initializes the work item manager, and creates worker threads
   bool Initialize( uint32 nThreads );


   SuArray<SuWorkItemThread*> m_threads;  ///< Worker threads used for executing work items
   SuSignal m_startSignal;                ///< Signal which worker threads wait on to start work
   SuSignal m_finishSignal;               ///< Signal that the main thread waits on for worker threads to finish outstanding jobs
   SuInterlockedCounter m_threadCount;    ///< Counter for the number of worker threads which are still operating.  

};


#endif