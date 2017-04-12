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

#ifndef _SU_SIMDVEC_H_
#define _SU_SIMDVEC_H_

// We select a SIMD vector type based on #defines.
//
// This lets us select between different SIMD instruction sets at compile time (SSE, AltiVec?, AVX?) 
//  There will  always be two SIMD vector types defined.  One, SuSIMDVec, is a general type which is independent of SIMD width, and
//  another, SuSIMDVec4 a special type for 4-wide vectors.  For code which does not care about the SIMD width 
//  (for example, bulk array operations), the SuSIMDVec type should be preferred over SuSIMDVec4
//
// SIMD instrinsics are not exposed to the engine by default.  To use SIMD, one must #include SuSIMDVec.h and
//  #define the appropriate symbol for the desired SIMD API. For example:
//
//  #define SU_SIMD_SSE     // if using SSE intrinsics
//  #include "SuSIMDVec.h"
//
//  To load floating point values into SIMDVec's, the floating point data must be properly aligned, like so:
//
//     SIMDALIGN float v[SuSIMDVec::WIDTH]
//     SuSIMDVec vec(v); // GOOD:  V is aligned correctly, loads the contents of v into a SIMD register
//
//     float v[SuSIMDVec::WIDTH]
//     SuSIMDVec vec(v); // BAD:  V could be mis-aligned, an exception could occur

#ifdef SU_SIMD_SSE

   #define SIMDALIGN __declspec(align(16))
  
   #include "SuSSEVec.h"
   typedef SuSSEVec  SuSIMDVec;
   typedef SuSSEVec  SuSIMDVec4;


   // Indicates that a SIMD wrapper is available.  #ifdefs on this can be used
   //  to select between SIMD and non-SIMD code paths in the engine
   #define SU_SIMD_ENABLED

#else
   // barf if there is no SIMD symbol defined
   #error "No SU_SIMD symbol defined.  To use SIMD intrinsics, define a SIMD vector symbol.  See SuSIMDVec.h"
#endif

#endif // _SU_SIMDVEC_H_
