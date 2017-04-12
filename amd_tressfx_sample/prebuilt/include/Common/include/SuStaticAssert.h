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

#ifndef _SU_STATICASSERT_H_
#define _SU_STATICASSERT_H_

#if defined __GNUC__

#define SU_STATIC_ASSERT(x) do { switch (true) { case false: case (x): default: break; } }  while (0)

#else

/// \ingroup Common
/// Helper template in the implementation of SU_STATIC_ASSERT
/// \sa SU_STATIC_ASSERT
template<bool b> struct SU_STATIC_ASSERT_FAILURE;   // forward declare a templatized struct  


/// \ingroup Common
/// Helper template in the implementation of SU_STATIC_ASSERT
/// \sa SU_STATIC_ASSERT
template<> struct SU_STATIC_ASSERT_FAILURE<true> { };  // define the 'true' version of the template  


/// \ingroup Common
/// Helper template in the implementation of SU_STATIC_ASSERT
/// \sa SU_STATIC_ASSERT
template<int n> struct SU_STATIC_ASSERT_TEST;      // dummy template used for the test      

// Tp implement the assert, we try to instantiate SU_STATIC_ASSERT_TEST with the size of SU_STATIC_ASSERT_FAILURE<x>. 
// If x is false, then this will generate a compile error, because the size of the <false> template struct is unknown

/// \ingroup Common
/// A macro which provides a compile-time assertion mechanism.  Use only with expressions that are known at compile time
#define SU_STATIC_ASSERT(x) SU_STATIC_ASSERT_TEST<  sizeof( SU_STATIC_ASSERT_FAILURE<(x)> ) >;

// static examples (uncomment to test)
// SU_STATIC_ASSERT(true)    // does nothing
// SU_STATIC_ASSERT( 1 != 1 ) // compile error
// SU_STATIC_ASSERT( false )  // compile error

#endif


#endif // _SU_STATICASSERT_H_
