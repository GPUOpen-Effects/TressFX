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

#ifndef _SU_SSEVEC_H_
#define _SU_SSEVEC_H_

#include <xmmintrin.h>

//=================================================================================================================================
/// \ingroup SushiCommon
/// \brief A SIMD vector implementation using SSE
/// 
/// This class is a thin wrapper for SSE intrinsics, intended to make SIMD code more readable and portable.
/// 
/// This class provides the following services:
///     - Arithmetic operators (+,-,*,/ +=, -=, *=, /=)
///     - Bitwise operators ( &, |, ^ )
///     - Comparison operators (which set the components to 0 or 0xffffffff, based on the comparison)
///     - Various useful utility functions
///     - Loads and stores of SIMD vectors 
///
/// Some important idioms to remember for SIMD programming are: 
///     - Use properly aligned memory (16-bytes for SSE)
///     - For simple conditional selection, use the Select() method instead of if statements
///     - For true conditional branches, use comparison operators and bitwise ops to evaluate the condition, and
///          then use the Mask() method in an if to decide whether to take the branch.  
///  
///         For example, to implement something like:  if( any( a > b ) )
///         You can use the Mask() operator like so: if( SuSIMDVec::Mask( a > b ) )
///
/// See the Intel or AMD programming guides for a more in-depth discussion of vector SIMD programming
///
//=================================================================================================================================
class SIMDALIGN SuSSEVec
{
public:

   /// Number of components in the SIMD vector
   static const uint32 WIDTH = 4;

   /// Required address alignment for SSE vectors
   static const uint32 ALIGNMENT = 16;

   inline SuSSEVec() {}

   /// Initializes the vector by fetching four values from memory.  The address pV must be 16-byte aligned
   inline SuSSEVec( float* pV ) : m_v( _mm_load_ps(pV) ) {};
   
   /// Initializes the vector with four literal values
   inline SuSSEVec( float x, float y, float z, float w ) : m_v( _mm_setr_ps(x,y,z,w) ) {} // they're reversed so that SSEVec( {1,2,3,4} ) == SSEVec(1,2,3,4)

   /// Initializes the vector with an __m128
   inline SuSSEVec( __m128 v )  : m_v(v) {};
   
   /// Initializes all components of the vector to f
   inline SuSSEVec( float f ) : m_v( _mm_set1_ps(f) ) {};

   inline SuSSEVec operator+( const SuSSEVec& rV ) const { return _mm_add_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator-( const SuSSEVec& rV ) const { return _mm_sub_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator*( const SuSSEVec& rV ) const { return _mm_mul_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator/( const SuSSEVec& rV ) const { return _mm_div_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator&( const SuSSEVec& rV ) const { return _mm_and_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator|( const SuSSEVec& rV ) const { return _mm_or_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator^( const SuSSEVec& rV ) const { return _mm_xor_ps( m_v, rV.m_v ); };
   
   inline SuSSEVec operator<( const SuSSEVec& rV )  const { return _mm_cmplt_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator>( const SuSSEVec& rV )  const { return _mm_cmpgt_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator<=( const SuSSEVec& rV ) const { return _mm_cmple_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator>=( const SuSSEVec& rV ) const { return _mm_cmpge_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator==( const SuSSEVec& rV ) const { return _mm_cmpeq_ps( m_v, rV.m_v ); };
   inline SuSSEVec operator!=( const SuSSEVec& rV ) const { return _mm_cmpneq_ps( m_v, rV.m_v ); };

   inline void operator+=( const SuSSEVec& rV ) { m_v = _mm_add_ps(m_v, rV.m_v); };
   inline void operator-=( const SuSSEVec& rV ) { m_v = _mm_sub_ps(m_v, rV.m_v); };
   inline void operator*=( const SuSSEVec& rV ) { m_v = _mm_mul_ps(m_v, rV.m_v); };
   inline void operator/=( const SuSSEVec& rV ) { m_v = _mm_div_ps(m_v, rV.m_v); };
   inline void operator&=( const SuSSEVec& rV ) { m_v = _mm_and_ps(m_v, rV.m_v); };
   inline void operator|=( const SuSSEVec& rV ) { m_v = _mm_or_ps(m_v, rV.m_v); };
   inline void operator^=( const SuSSEVec& rV ) { m_v = _mm_xor_ps(m_v, rV.m_v); };


   /// Creates a four-bit mask from the high bits of each component
   inline int Mask() const { return _mm_movemask_ps(m_v); };
   inline static int Mask( const SuSSEVec& a ) { return a.Mask(); };

   /// Stores the vector to a memory location
   inline void Store( float* pV ) { _mm_store_ps( pV, m_v ); };

   /// Stores the vector to an unaligned memory location
   inline void StoreUnaligned( float* pV ) { _mm_storeu_ps( pV, m_v ); };

   /// Returns a zeroed SIMD vector
   inline static SuSSEVec Zero() { return _mm_setzero_ps(); };
   
   inline static SuSSEVec Min( const SuSSEVec& a, const SuSSEVec& b )  { return _mm_min_ps(a.m_v,b.m_v); };
   inline static SuSSEVec Max( const SuSSEVec& a, const SuSSEVec& b )  { return _mm_max_ps(a.m_v,b.m_v); };
   inline static SuSSEVec Sqrt( const SuSSEVec& a, const SuSSEVec& b ) { return _mm_sqrt_ps(a.m_v); };

   /// Performs a component-wise conditional selection.  
   ///  Component i of the return value is set to:  c[i] ? a[i] : b[i]
   inline static SuSSEVec Select( const SuSSEVec& c, const SuSSEVec& a, const SuSSEVec& b ) {
      return (a & c) | SuSSEVec( _mm_andnot_ps( b.m_v, c.m_v ) );
   };

   // TODO:  Add support for RCP and RSQRT intrinsics, with and without newton-raphson iteration
   // TODO:  Add support for shuffling

   // TODO:  Add some horizontal operations (add, min, max)
   
private:

   __m128 m_v;
};


#endif // _SU_SSEVEC_H_
