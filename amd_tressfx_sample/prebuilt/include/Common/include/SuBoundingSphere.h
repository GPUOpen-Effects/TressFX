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

#ifndef _SU_BOUNDINGSPHERE_H_
#define _SU_BOUNDINGSPHERE_H_

#include "SuBoundingVolume.h"
#include "SuPoint3.h"
#include "SuVector3.h"

//=========================================================================================================
/// \ingroup Math
/// \brief Bounding sphere
/// 
//=========================================================================================================
// tolua_begin
class SuBoundingSphere : public SuBoundingVolume
{
public:
   /// Constructor
   SU_DLL SuBoundingSphere( const SuPoint3& rCenter = SuPoint3::ORIGIN, float32 fRadius = 1.0f );

   /// Sets the bounding sphere's center
   inline const SuPoint3& GetCenter() const { return m_center; }

   /// Returns the bounding sphere's center
   inline void SetCenter( const SuPoint3& rCenter ) { m_center = rCenter; }

   /// Returns the bounding sphere's radius
   inline float32 GetRadius() const { return m_fRadius; }

   /// Sets the bounding sphere's radius
   inline void SetRadius( float32 fRadius ) { m_fRadius = fRadius; }

   /// Che cks whether a point is inside the bounding sphere
   virtual SU_DLL bool PointInside( const SuPoint3& rPt ) const ;

   /// Checks whether a ray intersects the bounding sphere
   virtual SU_DLL bool RayIntersects( const SuRay& rRay ) const;

// tolua_end

   /// Creates a duplicate of the bounding volume
   virtual SU_DLL SuBoundingVolume* Clone() const;

private:
   SuPoint3 m_center;
   float32  m_fRadius;
// tolua_begin
};
// tolua_end


#endif // _SU_BOUNDINGSPHERE_H_
