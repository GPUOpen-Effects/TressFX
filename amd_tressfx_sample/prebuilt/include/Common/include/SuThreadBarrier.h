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

#ifndef _SU_THREADBARRIER_H_
#define _SU_THREADBARRIER_H_

#include "SuInterlockedCounter.h"

class SuThreadBarrierListener;

//=================================================================================================================================
/// \ingroup ParticlePhysicsPlugin
/// \brief This class provides a 'barrier' abstraction, that can be used to force a group of threads to reach a synch point
/// 
///   The thread barrier is an abstraction that will force a group of threads to reach a synchronization point before 
///     proceeding.  This could be implemented using SuSignal objects, but the SuThreadBarrier uses a spin-wait loop,
///     which can be much more efficient than blocking if the wait times are short and threads must synchronize repeatedly
///
///   Threads will wait at the barrier by calling the 'Wait' method, which will block until all of the outstanding threads
///     have arrived at the barrier.  When this happens, the threads are released, and the barrier is atomically reset
///
//=================================================================================================================================
class SuThreadBarrier
{
public:

   /// Constructor
   SU_DLL SuThreadBarrier( uint32 nMaxThreads );

   /// Waits in a spin-loop until all other threads have also called 'Wait'
   SU_DLL void Wait( SuThreadBarrierListener* pListener = NULL );

   /// Returns the number of threads that this barrier will wait for
   inline uint32 GetThreadCount() const { return m_nMaxThreads; };

private:

   void WaitOnCounter( SuInterlockedCounter& rCounter, SuThreadBarrierListener* pListener );

   uint32 m_nMaxThreads;
   SuInterlockedCounter m_nThreadsEven;
   SuInterlockedCounter m_nThreadsOdd;
   bool m_bEven;
  
};

//=================================================================================================================================
/// \ingroup SuPartriclePhysicsPlugin
/// \brief  An interface for an object which can be notified when all threads have reached a particular barrier
///
///  Barrier listeners can be used to notify an object that all threads have reached a particular barrier.  This is useful
///   for performing any cleanup that must happen after a parallel operation (or setup for a subsequent one).  Calls to 
///   'Notify' will happen when the barrier is reached, but before the waiting threads have been released.
///
///  These calls will be issued by one, and only one thread, but the exact thread that issues them is not well defined.
///
//=================================================================================================================================
class SuThreadBarrierListener
{
public:

   /// Notifies the listener that the barrier has been reached
   /// \param pBarrier  The barrier that was reached
   virtual void OnBarrier( SuThreadBarrier* pBarrier ) = 0;

};


#endif // _SU_THREADBARRIER_H_
