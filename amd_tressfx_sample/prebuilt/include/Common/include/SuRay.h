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

#ifndef _SU_RAY_H_
#define _SU_RAY_H_

#include "SuPoint3.h"
#include "SuVector3.h"

//=================================================================================================================================
/// \ingroup Common
/// \brief A class to encapsulate a set of raycasting utilities
/// 
///  The SuRay class is and provides a suite of methods for testing for intersection between the ray and various geometric objects.    
///
///  This class is intended to provide a suite of reasonably efficient, general purpose operations for simple tasks
///    such as picking, which are becoming increasingly common in our demos.  If raycasting is done in performance critical code, 
///    then more specialized routines may be necessary.
///
//=================================================================================================================================

// tolua_begin
class SuRay
{
public:

   /// Ray constructor
   inline SuRay( const SuPoint3& rOrigin, const SuVector3& rDirection );

   /// Returns the ray origin point
   inline const SuPoint3& GetOrigin() const { return m_origin; };

   /// Returns the ray direction vector
   inline const SuVector3& GetDirection() const { return m_direction; };

   /// Returns the reciprocal of the ray direction vector
   inline const SuVector3& GetInverseDirection() const { return m_inverseDirection; };

   /// Tests for intersection between a ray and an axis-aligned bounding box and returns the 'T' range at which the ray traverses the box
   inline bool IntersectAxisAlignedBox( const SuPoint3& rMin, const SuPoint3& rMax, float* pfTMin, float* pfTMax ) const;

   /// Tests for intersection between a ray and an axis-aligned bounding box, but does not return intersection distances
   inline bool IntersectAxisAlignedBox( const SuPoint3& rMin, const SuPoint3& rMax ) const;

   /// Computes intersection distance for an axis-aligned plane
   inline float IntersectAxisAlignedPlane( float fValue, int32 nAxis ) const;

   /// Tests for intersection between a ray and a triangle
   inline bool IntersectTriangle(  const SuPoint3& vert0, const SuPoint3& vert1, const SuPoint3& vert2, float* pfTOut ) const;

   /// Computes a position along the ray at a given distance
   inline SuPoint3 GetPosition( float t ) const { return m_origin + (m_direction*t); };

// tolua_end
private:

   SuPoint3 m_origin;
   SuVector3 m_direction;
   SuVector3 m_inverseDirection; ///< The reciprocal ray direction is used in a variety of raytracing tests and is worth precomputing and storing

// tolua_begin
};
// tolua_end

#include "SuRay.inl"

#endif // _SU_RAY_H_
