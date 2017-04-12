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

#ifndef _SU_BOUNDINGBOX_H_
#define _SU_BOUNDINGBOX_H_

#include "SuBoundingVolume.h"
#include "SuPoint3.h"
#include "SuVector3.h"
#include "SuMatrix4.h"


//=========================================================================================================
/// \ingroup Math
/// \brief Axis-aligned bounding box
/// 
//=========================================================================================================
// tolua_begin
class SuBoundingBox : public SuBoundingVolume
{
public:
   /// Constructor
   SU_DLL SuBoundingBox( const SuPoint3& rMinCorner = SuPoint3( -0.5f, -0.5f, -0.5f ),
                  const SuPoint3& rMaxCorner = SuPoint3( 0.5f, 0.5f, 0.5f ) )
      : SuBoundingVolume( SuBoundingVolume::BOX ), 
        m_minCorner( rMinCorner ),
        m_maxCorner( rMaxCorner )
   {
   }

   virtual SU_DLL ~SuBoundingBox();

   /// Sets the bounding box's minimum corner
   inline const SuPoint3& GetMinCorner() const { return m_minCorner; }

   /// Returns the bounding box's minimum corner
   inline void SetMinCorner( const SuPoint3& rMinCorner ) { m_minCorner = rMinCorner; }

   /// Sets the bounding box's maximum corner
   inline const SuPoint3& GetMaxCorner() const { return m_maxCorner; }

   /// Return the eight corner points
   SU_DLL void Boxify (SuPoint3 box[8]) const; 

   /// Returns the bounding box;s maximum corner
   SU_DLL void SetMaxCorner( const SuPoint3& rMaxCorner ) { m_maxCorner = rMaxCorner; }

   /// Returns the bounding box's center
   inline SuPoint3 GetCenter() const { return 0.5f * (m_minCorner + m_maxCorner); }

   /// Checks whether a point is inside the bounding box
   virtual SU_DLL bool PointInside( const SuPoint3& rPt ) const { return m_minCorner.x <= rPt.x && rPt.x <= m_maxCorner.x &&
                                                                         m_minCorner.y <= rPt.y && rPt.y <= m_maxCorner.y &&
                                                                         m_minCorner.z <= rPt.z && rPt.z <= m_maxCorner.z;    }

   /// Checks whether a ray intersects the bounding box
   virtual SU_DLL bool RayIntersects( const SuRay& rRay ) const;

   /// Expands the bounding box to include the specified point
   void SU_DLL Expand( const SuPoint3& rPoint );   

   /// Transforms the bounding box by the specified matrix, and computes the axis-aligned bounding box for the resulting oriented box
   void SU_DLL Transform( const SuMatrix4& rMatrix, SuBoundingBox* pBoxOut ) const;

// tolua_end

   /// Creates a duplicate of the bounding volume
   virtual SU_DLL SuBoundingVolume* Clone() const;

private:
   SuPoint3 m_minCorner;
   SuPoint3 m_maxCorner;
// tolua_begin
};
// tolua_end


#endif // _SU_BOUNDINGBOX_H_
