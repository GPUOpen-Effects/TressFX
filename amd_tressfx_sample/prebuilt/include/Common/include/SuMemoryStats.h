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

#ifndef _SU_MEMORYSTATS_H_
#define _SU_MEMORYSTATS_H_

#include "SuTypes.h"

#if defined (_LINUX)

#include <malloc.h>
#include <cstddef>
#include <cstdlib>
#include <new>

#endif

//=========================================================================================================
/// \ingroup Common
/// \brief Helper class that reports memory usage.
///
/// This class works in conjunction with custom implementations of global operator new() and operator
/// delete().
//=========================================================================================================
class SuMemoryStats
{
public:
   /// \return The total size of all currently allocated memory blocks, in bytes
   static inline size_t GetCurrentMemoryUsage() { return ms_nCurrMemUsage; }

   /// \return The maximum amount of allocated memory since beginning of program execution in bytes
   static inline size_t GetPeakMemoryUsage() { return ms_nPeakMemUsage; }

private:
   friend void* operator new( std::size_t nSize );
   friend void operator delete( void* pRawMem );

   /// Add a successful memory allocation to the stats
   static SU_DLL void MemoryAllocated( size_t nNumBytes );

   /// Add a successful memory free to the stats
   static SU_DLL void MemoryFreed( size_t nNumBytes );

   static SU_DLL size_t ms_nCurrMemUsage;
   static SU_DLL size_t ms_nPeakMemUsage;
};


#endif // _SU_MEMORYSTATS_H_
