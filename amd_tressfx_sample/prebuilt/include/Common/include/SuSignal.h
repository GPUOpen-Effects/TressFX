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

#ifndef _Su_SIGNAL_H_
#define _Su_SIGNAL_H_

//=====================================================================================================================
/// \brief This class encapsulates a wait-signal type synchronization mechanism
///
///  The SuSignal class is used as a fence, in order to force a thread to wait for a particular event to occur.
///  For example, a worker thread may wait to receive a signal from a control thread when work is available.
///
//=====================================================================================================================

class SuSignal
{
public:
   
    /// Constructor
    SU_DLL SuSignal();

    /// Destructor
    SU_DLL ~SuSignal();

    /// Causes the calling thread to wait for a subsequent 'Signal' operation, or a timeout
    SU_DLL bool Wait( uint32 nTimeout );

    /// Sets the signal to the signaled state.  Threads which are waiting may resume execution
    SU_DLL void Signal();

    /// Sets the signal to the non-signaled state
    SU_DLL void Reset();

    /// Waits on a particular set of signals
    static SU_DLL void WaitForSignals( uint32 nTimeout, SuArray<SuSignal*>& rSignals );


private:

    void* m_hEvent;  ///< Handle to a platform-specific event object

};


#endif