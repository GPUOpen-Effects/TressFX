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

#ifndef _SU_PLANE_H_
#define _SU_PLANE_H_

#include "SuVector3.h"

//=========================================================================================================
/// \ingroup Common
/// \brief Plane composed of a unit normal and plane constant
//=========================================================================================================

// tolua_begin
class SuPlane
{
public:

   /// Constructor
   inline SuPlane ();

   /// Constructor - construct based on implicit equation ( Ax + By + Cz + D = 0 )
   inline SuPlane( float fA, float fB, float fC, float fD );

   /// Constructor - construct directly from normal N and constant C
   inline SuPlane ( const SuVector3& rNormal, float32 fConstant );

   /// Constructor - construct from a plane Normal (N) and point on plane (P)
   inline SuPlane ( const SuVector3& rNormal, const SuVector3& rPoint );

   /// Constructor - construct from three points on the plane P0, P1, P2
   inline SuPlane ( const SuVector3& rPoint0, const SuVector3 &rPoint1, const SuVector3& rPoint2 );                                   

   /// Copy Constructor - copy from another plane
   inline SuPlane ( const SuPlane& rPlane );

   /// Set the normal directly
   inline void SetNormal ( const SuVector3& rNormal );

   /// Get the normal
   inline const SuVector3& GetNormal () const;

   /// Set the constant directly
   inline void SetConstant ( float32 fConstant ); 

   /// Get the constant
   inline const float32 GetConstant () const;

   /// Get the plane a SuVector4( N.x, N.y, N.z, C )
   inline SuVector4 GetAsVector4() const;

   /// Get the distance from the point to the plane
   inline const float32 GetDistanceTo ( const SuVector3 &rPoint ) const;

   /// Determine which side of the plane a point lies on
   inline const int32 GetWhichSide ( const SuVector3 &rPoint ) const;

   /// Normalizes the normal of this plane
   inline void Normalize();

   /// Computes the intersection of a ray with a plane
   inline float RayIntersect( const SuPoint3& rOrigin, const SuVector3& rDirection ) const;

// tolua_end


   /// Assignment
   inline SuPlane& operator= (const SuPlane& rPlane);

   /// Comparison equals
   inline bool operator== (const SuPlane& rPlane) const;

   /// Comparison not equals   
   inline bool operator!= (const SuPlane& rPlane) const;
      

protected:
   /// Unit normal
   SuVector3 m_vNormal;

   /// Plane constant
   float32 m_fConstant;

// tolua_begin
};
// tolua_end

#include "SuPlane.inl"

#endif // _SU_PLANE_H_
