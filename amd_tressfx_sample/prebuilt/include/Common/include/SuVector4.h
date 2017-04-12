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

#ifndef _SUVECTOR4_H
#define _SUVECTOR4_H

#include "SuTypes.h"
#include "SuMath.h"
#include "SuVector3.h"


/// \brief 4D vector with float32 components
///
/// \ingroup Math

// tolua_begin
class SuVector4
{
// tolua_end

   public:
      float32 x;                                                            // tolua_export
      float32 y;                                                            // tolua_export
      float32 z;                                                            // tolua_export
      float32 w;                                                            // tolua_export

      /// Constructor
      inline SuVector4 ();                                                  // tolua_export

      /// Constructor
      inline SuVector4 (float32 aX, float32 aY, float32 aZ, float32 aW);    // tolua_export

      /// Constructor
      inline SuVector4 (const SuVector4& v);                                // tolua_export

      /// Constructor
      inline explicit SuVector4 (const SuVector3& v, float32 aW = 0.0f);    // tolua_export

      // assignment
      inline SuVector4& operator= (const SuVector4& v);

      // comparison
      inline bool operator== (const SuVector4& v) const;                    // tolua_export
      inline bool operator!= (const SuVector4& v) const;
      inline bool operator<  (const SuVector4& v) const;                    // tolua_export
      inline bool operator<= (const SuVector4& v) const;                    // tolua_export
      inline bool operator>  (const SuVector4& v) const;
      inline bool operator>= (const SuVector4& v) const;
      
      // arithmetic operations
      inline SuVector4 operator+ (const SuVector4& v) const;                // tolua_export
      inline SuVector4 operator- (const SuVector4& v) const;                // tolua_export
      inline SuVector4 operator* (float32 s) const;                         // tolua_export
      inline SuVector4 operator/ (float32 s) const;                         // tolua_export
      inline SuVector4 operator- () const;                                  // tolua_export
      
      // arithmetic updates
      inline SuVector4& operator+= (const SuVector4& v);
      inline SuVector4& operator-= (const SuVector4& v);
      inline SuVector4& operator*= (float32 s);
      inline SuVector4& operator/= (float32 s);

      // member access
      inline const float32& operator[] (int32 i) const;
      inline float32& operator[] (int32 i);

      /// Get a pointer back
      inline operator float32* () const {return (float32*)&x;}

      /// Returns the first three components as an SuVector3
      inline SuVector3 XYZ() const { return SuVector3(x,y,z); };

      // vector operations
      /// Computes the dot product
      inline float32 Dot (const SuVector4& v) const;                         // tolua_export

      /// Computes the squared length of the vector
      inline float32 SquaredLength () const;                                 // tolua_export

      /// Computes the length of the vector
      SU_DLL float32 Length () const;                                               // tolua_export

      /// Normalizes the vector
      SU_DLL float32 Normalize ();                                                  // tolua_export

      // special vectors
      /// Zero-length vector
      static SU_DLL const SuVector4 ZERO;                                           // tolua_export

      /// Unit-length vector in X
      static SU_DLL const SuVector4 UNIT_X;                                         // tolua_export

      /// Unit-length vector in Y
      static SU_DLL const SuVector4 UNIT_Y;                                         // tolua_export

      /// Unit-length vector in Z
      static SU_DLL const SuVector4 UNIT_Z;                                         // tolua_export

      /// Unit-length vector in W
      static SU_DLL const SuVector4 UNIT_W;                                         // tolua_export

// tolua_begin
};
// tolua_end

inline const SuVector4 operator* (const float32 s, const SuVector4& v);

#include "SuVector4.inl"

#endif
