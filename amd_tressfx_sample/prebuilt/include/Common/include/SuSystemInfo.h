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

#ifndef _SU_SYSTEMINFO_H_
#define _SU_SYSTEMINFO_H_


//=================================================================================================================================
/// \ingroup 
/// \brief A meta-state class that exposes various pieces of useful system information
/// 
//=================================================================================================================================
class SuSystemInfo
{
public:

   /// Returns the number of logical processors on the system (as reported by the operating system)
   static SU_DLL uint32 GetProcessorCount();


   /// Flags identifying the various SIMD instruction sets
   enum SIMDISA
   {
      SU_ISA_SSE  = 1,
      SU_ISA_SSE2 = 2,
      SU_ISA_SSE3 = 4
   };

   /// Returns a bitwise combination of SIMDISA flags, indicating which SIMD instructions are supported by the CPU
   static SU_DLL uint32 GetSIMDISA();

};


#endif // _SU_SYSTEMINFO_H_
