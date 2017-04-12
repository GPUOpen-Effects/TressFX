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

#ifndef _SUVECTOR3_H
#define _SUVECTOR3_H

#include "SuTypes.h"
#include "SuAssert.h"
//#include "SuPoint3.h"
#include "SuMath.h"


/// \brief 3D vector with float32 components
///
/// \ingroup Math

// tolua_begin
class SuVector3
{
// tolua_end

   public:
      float32 x;                                                  // tolua_export
      float32 y;                                                  // tolua_export
      float32 z;                                                  // tolua_export

      /// Constructor
      inline SuVector3 ();                                        // tolua_export

      /// Constructor
      inline SuVector3 (float32 aX, float32 aY, float32 aZ);      // tolua_export

      /// Constructor
      inline SuVector3 (const SuVector3& v);                      // tolua_export

      /// Constructor
      //inline explicit SuVector3 (const SuPoint3& v);                 // tolua_export

      /// Assignment
      inline SuVector3& operator= (const SuVector3& v);

      // Cast the point to a point
      //inline const SuPoint3& AsPoint() { return *reinterpret_cast<SuPoint3*>(this); }

      // comparison
      inline bool operator== (const SuVector3& v) const;          // tolua_export
      inline bool operator!= (const SuVector3& v) const;
      inline bool operator<  (const SuVector3& v) const;          // tolua_export
      inline bool operator<= (const SuVector3& v) const;          // tolua_export
      inline bool operator>  (const SuVector3& v) const;
      inline bool operator>= (const SuVector3& v) const;
      
      // arithmetic operations
      inline SuVector3 operator+ (const SuVector3& v) const;      // tolua_export
      inline SuVector3 operator- (const SuVector3& v) const;      // tolua_export
      inline SuVector3 operator* (const SuVector3& v) const;      // tolua_export
      inline SuVector3 operator* (float32 s) const;               // tolua_export
      inline SuVector3 operator/ (float32 s) const;               // tolua_export
      inline SuVector3 operator- () const;                        // tolua_export
      
      // arithmetic updates
      inline SuVector3& operator+= (const SuVector3& v);
      inline SuVector3& operator-= (const SuVector3& v);
      inline SuVector3& operator*= (float32 s);
      inline SuVector3& operator/= (float32 s);

      // member access
      inline const float32& operator[] (int32 i) const;
      inline float32& operator[] (int32 i);

      /// Get a pointer back
      inline operator float32* () const {return (float32*)&x;}

      // vector operations
      /// Computes the dot product
      inline float32 Dot (const SuVector3& v) const;              // tolua_export

      /// Computes the squared length of the vector
      inline float32 SquaredLength () const;                      // tolua_export

      /// Computes the length of the vector
      inline float32 Length () const;                             // tolua_export

      /// Normalizes the vector
      SU_DLL float32 Normalize ();                                       // tolua_export

      // The cross products are computed using the right-handed rule.  Be aware
      // that some graphics APIs use a left-handed rule.  If you have to compute
      // a cross product with these functions and send the result to the API
      // that expects left-handed, you will need to change sign on the vector
      // (replace each component value c by -c).
      /// Computes the cross-product between two vectors
      inline SuVector3 Cross (const SuVector3& v) const;          // tolua_export
           
      // special vectors
      /// Zero-length vector
      static SU_DLL const SuVector3 ZERO;                                // tolua_export

      /// Unit-length vector in X
      static SU_DLL const SuVector3 UNIT_X;                              // tolua_export

      /// Unit-length vector in Y
      static SU_DLL const SuVector3 UNIT_Y;                              // tolua_export

      /// Unit-length vector in Z
      static SU_DLL const SuVector3 UNIT_Z;                              // tolua_export

// tolua_begin
};
// tolua_end

inline const SuVector3 operator* (const float32 s, const SuVector3& v);

// inline function implementations
#include "SuVector3.inl"

#endif
