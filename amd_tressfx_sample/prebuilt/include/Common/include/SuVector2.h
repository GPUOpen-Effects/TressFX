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

#ifndef _SUVECTOR2_H
#define _SUVECTOR2_H

#include "SuTypes.h"
#include "SuMath.h"

/// \brief 2D vector with float32 components
///
/// \ingroup Math

// tolua_begin
class SuVector2
{
// tolua_end
   public:
      float32 x;                                          // tolua_export
      float32 y;                                          // tolua_export

      /// Constructor
      inline SuVector2 ();                                // tolua_export

      /// Constructor
      inline SuVector2 (float32 aX, float32 aY);          // tolua_export

      /// Constructor
      inline SuVector2 (const SuVector2& v);              // tolua_export

      // assignment
      inline SuVector2& operator= (const SuVector2& v);

      // comparison
      inline bool operator== (const SuVector2& v) const;  // tolua_export
      inline bool operator!= (const SuVector2& v) const;
      inline bool operator<  (const SuVector2& v) const;  // tolua_export
      inline bool operator<= (const SuVector2& v) const;  // tolua_export
      inline bool operator>  (const SuVector2& v) const;
      inline bool operator>= (const SuVector2& v) const;
      
      // arithmetic operations
      inline SuVector2 operator+ (const SuVector2& v) const;   // tolua_export
      inline SuVector2 operator- (const SuVector2& v) const;   // tolua_export
      inline SuVector2 operator* (float32 s) const;            // tolua_export
      inline SuVector2 operator/ (float32 s) const;            // tolua_export
      inline SuVector2 operator- () const;                     // tolua_export
      
      // arithmetic updates
      inline SuVector2& operator+= (const SuVector2& v);
      inline SuVector2& operator-= (const SuVector2& v);
      inline SuVector2& operator*= (float32 s);
      inline SuVector2& operator/= (float32 s);

      // member access
      inline const float32& operator[] (int32 i) const;
      inline float32& operator[] (int32 i);

      /// Get a pointer back
      inline operator float32* () const {return (float32*)&x;}

      // vector operations
      /// Computes the dot product
      inline float32 Dot (const SuVector2& v) const;           // tolua_export

      /// Computes the squared length of the vector
      inline float32 SquaredLength () const;                   // tolua_export

      /// Computes the length of the vector
      inline float32 Length () const;                          // tolua_export

      /// Normalizes the vector
      SU_DLL float32 Normalize ();                                    // tolua_export
           
      // special vectors
      /// Zero-length vector
      static SU_DLL const SuVector2 ZERO;                             // tolua_export

      /// Unit-length vector in X
      static SU_DLL const SuVector2 UNIT_X;                           // tolua_export

      /// Unit-length vector in Y
      static SU_DLL const SuVector2 UNIT_Y;                           // tolua_export

// tolua_begin
};
// tolua_end

inline const SuVector2 operator* (const float32 s, const SuVector2& v);

// inline function implementations
#include "SuVector2.inl"

#endif
