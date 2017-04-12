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

#ifndef _SUPOINT3_H
#define _SUPOINT3_H

#include "SuTypes.h"
#include "SuVector3.h"
#include "SuMath.h"

/// \brief 3D point with float32 components
///
/// \ingroup Math

// tolua_begin
class SuPoint3
{
// tolua_end

   public:
      float32 x;                                                     // tolua_export
      float32 y;                                                     // tolua_export
      float32 z;                                                     // tolua_export

      /// Constructor
      inline SuPoint3 ();                                            // tolua_export

      /// Constructor
      inline SuPoint3 (float32 aX, float32 aY, float32 aZ);          // tolua_export

      /// Constructor
      inline SuPoint3 (const SuPoint3& pt);                          // tolua_export

      /// Constructor
      inline explicit SuPoint3 (const SuVector3& v);                 // tolua_export

      // assignment
      inline SuPoint3& operator= (const SuPoint3& pt);

      /// Cast the point to a 3-vector
      inline const SuVector3& AsVector() const { return *reinterpret_cast<const SuVector3*>(this); }

      // comparison
      inline bool operator== (const SuPoint3& pt) const;              // tolua_export
      inline bool operator!= (const SuPoint3& pt) const;
      inline bool operator<  (const SuPoint3& pt) const;              // tolua_export        
      inline bool operator<= (const SuPoint3& pt) const;              // tolua_export
      inline bool operator>  (const SuPoint3& pt) const;
      inline bool operator>= (const SuPoint3& pt) const;
      
      // arithmetic operations
      inline SuPoint3 operator+ (const SuVector3& v) const;          // tolua_export
      inline SuPoint3 operator+ (const SuPoint3& pt) const;          // tolua_export
      inline SuPoint3 operator- (const SuVector3& v) const;          // tolua_export
      inline SuVector3 operator- (const SuPoint3& pt) const;         // tolua_export
      inline SuPoint3 operator* (float32 s) const;                   // tolua_export
      inline SuPoint3 operator/ (float32 s) const;                   // tolua_export
      inline SuPoint3 operator- () const;                        

      // arithmetic updates
      inline SuPoint3& operator+= (const SuVector3& v);
      inline SuPoint3& operator+= (const SuPoint3& p);
      inline SuPoint3& operator-= (const SuVector3& v);
	  inline SuPoint3& operator-= (const SuPoint3& p);
      inline SuPoint3& operator*= (float32 s);
      inline SuPoint3& operator/= (float32 s);

      // member access
      inline const float32& operator[] (int32 i) const;
      inline float32& operator[] (int32 i);

      // special vectors
      /// Zero-length vector
      SU_DLL static const SuPoint3 ORIGIN;                                  // tolua_export

// tolua_begin
};
// tolua_end

inline SuPoint3 operator* (float32 s, const SuPoint3& p);

// inline function implementations
#include "SuPoint3.inl"

#endif
