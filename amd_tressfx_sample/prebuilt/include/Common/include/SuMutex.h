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

#ifndef _Su_MUTEX_H_
#define _Su_MUTEX_H_

                     
//=====================================================================================================================
/// \brief A class to encapsulate a critical section for thread synchronization
///
///  This class provides mutual exclusion by serializing threads based on calls to the 'Take()' method.
///  Threads that are about to enter the critical section should 'Take()' the mutex, then 'Release()' it after
///  leaving the critical section.  This class will use OS synchronization mechanisms to provide mutual exclusion
///
///  On Win32, this class is implemented using critical section objects, which makes it relatively lightweight
///
//=====================================================================================================================
class SuMutex
{
public:

    /// Default constructor
    SU_DLL SuMutex(  );

    /// Destructor
    SU_DLL ~SuMutex();

    /// Takes the mutex
    SU_DLL void Take();

    /// Releases the mutex
    SU_DLL void Release();

private:

    void* m_pImpl; ///< Pointer to platform-specific mutex implementation (PIMPL idiom)

};


#endif