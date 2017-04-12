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

#ifndef _Su_THREAD_H_
#define _Su_THREAD_H_


//=====================================================================================================================
/// \brief A base class which encapsulates thread creation.
///
/// This class is modeled after the java threading system.  To create a thread, simply subclass the SuThread class 
/// and overload the OnExecuteThread method.  The base class hides the details of thread creation
//=====================================================================================================================

class SuThread
{
public:

    /// Default constructor for threads.  The thread is created by not executed.  To start executing a thread, call Start()
    SU_DLL SuThread( );

    virtual SU_DLL ~SuThread();

    /// Sets thread affinity
    SU_DLL void SetAffinity( uint32 nMask );

    /// Starts the thread
    SU_DLL bool Start();

    /// Determines whether or not the thread is still running
    SU_DLL bool IsActive() ;

    /// Sleeps for the specified amount of time, in ms.  
    SU_DLL void Sleep( uint32 nTime );

protected:

    // Protected interface for use by thread sub-classes

    /// This method is called on thread start to execute the thread.  Thread execution stops when this method returns.
    virtual void OnExecuteThread() = 0;


private:

    /// Thread execution procedure, its a friend so that it can call OnExecuteThread
#if defined ( _WINDOWS )
    friend uint32 __stdcall  SuThreadProc( void* pThreadData );
#endif

    bool m_bDeadFlag;       ///< Set when thread terminates
    void* m_nThreadHandle;  ///< Thread handle

};

#endif