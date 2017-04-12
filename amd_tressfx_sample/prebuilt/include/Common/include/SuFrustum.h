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

#ifndef _SU_FRUSTUM_H_
#define _SU_FRUSTUM_H_


#include "SuTypes.h"
#include "SuMatrix4.h"
#include "SuPlane.h"
#include "SuBoundingBox.h"
#include "SuBoundingSphere.h"

//=================================================================================================================================
/// \ingroup Math
/// \brief A class to encapsulate a viewing frustum
/// 
///  The frustum is represented by six planes.  The normals of the planes point towards the outside of the frustum
//=================================================================================================================================
// tolua_begin

class SuFrustum
{
public:
   
   /// Number of planes in the frustum
   // tolua_end
   static SU_DLL const uint8 PLANE_COUNT = 6; // tolua can't parse this for some reason
   // tolua_begin

   /// Frustum culling results
   enum CullResult
   {
      INSIDE,     ///< Bounding volume is completely inside the view frustum
      OUTSIDE,    ///< Bounding volume is completely outside the view frustum
      STRADDLE    ///< Bounding volume is partially inside and partially outside the view frustum
   };

   /// Enumeration of frustum planes
   enum FrustumPlane
   {
      LEFT, 
      RIGHT,
      TOP,
      BOTTOM,
      HITHER,  // I'm using hither and yon instead of near and far because near and far are C++ keywords
      YON      
   };

   /// Default constructor, initializes the frustum to create a simple unit box
   SU_DLL SuFrustum();

   /// Constructor to build a frustum from a set of planes
   SU_DLL SuFrustum( const SuPlane pPlanes[6] ); 

   /// Sets up a frustum for a D3D style view-projection matrix
   SU_DLL void SetFromMatrixD3D( const SuMatrix4& rWVP );

   /// Sets up a frustum for an OGL style view-projection matrix
   SU_DLL void SetFromMatrixOGL( const SuMatrix4& rWVP );

   /// Culls an axis-aligned bounding box
   SU_DLL CullResult CullAxisAlignedBox( const SuPoint3& rMinPt, const SuPoint3& rMaxPt ) const;
   SU_DLL CullResult CullAxisAlignedBox( const SuBoundingBox& rBox) const;

   /// Culls a bounding sphere
   SU_DLL CullResult CullSphere( const SuPoint3& rCenter, float fRadius ) const;
   SU_DLL CullResult CullSphere( const SuBoundingSphere& rSphere ) const;

   /// Accessor for the frustum planes
   inline const SuPlane& GetPlane( FrustumPlane i ) const { return m_planes[i]; };

// tolua_end
private:

   /// The planes of the frustum.  Plane normals point out.
   SuPlane m_planes[ PLANE_COUNT ];

// tolua_begin
};
// tolua_end




#endif // _SU_FRUSTUM_H_
