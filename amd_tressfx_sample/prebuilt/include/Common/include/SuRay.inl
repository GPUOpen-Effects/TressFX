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

//=================================================================================================================================
//
//          Includes / defines / typedefs / static member variable initialization block
//
//=================================================================================================================================

#include "SuAlgorithm.h"
#include <limits>

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block 
//
//=================================================================================================================================

//=================================================================================================================================
/// \param rOrigin      The ray origin
/// \param rDirection   The ray direction.  It need not be normalized
//=================================================================================================================================
inline SuRay::SuRay( const SuPoint3& rOrigin, const SuVector3& rDirection ) 
: m_origin( rOrigin ), m_direction( rDirection ), m_inverseDirection( SuVector3(1.0f/rDirection.x, 1.0f/rDirection.y, 1.0f/rDirection.z ) )
{

}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// \param rBBMin    Lower-left corner of box to test against
/// \param rBBMax    Upper-right corner of box to test against
/// \param pfTMin    If the ray hits, this receives the smallest 'T' value at which the ray is in the box (it may be negative if the origin is in the box)
/// \param pfTMax    If the ray hits, this receives the largest 'T' value at which the ray is in the box (it is always >= 0)
/// \return True if the ray strikes the box, false otherwise
//=================================================================================================================================
inline bool SuRay::IntersectAxisAlignedBox( const SuPoint3& rBBMin, const SuPoint3& rBBMax, float* pfTMin, float* pfTMax ) const
{
   // Locate the 'T' values at which ray enters and leaves each slab
   // As we do this, locate the largest entry value and smallest exit value

   float fTMinSlab; 
   float fTMaxSlab;

   // X aligned slab
   if( m_direction[0] == 0 )
   {
      // why are these branches here?  There is a pathological case that can arise if the ray origin is right on a box corner
      // and the ray is perpendicular to an axis.  In this case, you end up with 0 * INF, which equals Nan.  Nans can result in incorrect answers
      //  The workaround is to use the full ray range if a zero direction is found, and try and reject the box based on the other two axes
      fTMinSlab = 0;
      fTMaxSlab = std::numeric_limits<float>::infinity();
   }
   else
   {
      fTMinSlab = ( rBBMin[0] - m_origin[0] ) * m_inverseDirection[0];
      fTMaxSlab = ( rBBMax[0] - m_origin[0] ) * m_inverseDirection[0];
      if( fTMinSlab > fTMaxSlab )
         SuSwap( fTMinSlab, fTMaxSlab ); // We could replace this with an SSE min intrinsic, it will cut it down to one instruction and remove a branch
   }

   float fTMinHit = fTMinSlab;
   float fTMaxHit = fTMaxSlab;
   
   // Y aligned slab
   if( m_direction[1] == 0 )
   {
      fTMinSlab = 0;
      fTMaxSlab = std::numeric_limits<float>::infinity();
   }
   else
   {
      fTMinSlab = ( rBBMin[1] - m_origin[1] ) * m_inverseDirection[1];
      fTMaxSlab = ( rBBMax[1] - m_origin[1] ) * m_inverseDirection[1];
      if( fTMinSlab > fTMaxSlab )
         SuSwap( fTMinSlab, fTMaxSlab ); // We could replace this with an SSE min intrinsic, it will cut it down to one instruction and remove a branch
   }
 
   fTMinHit = SuMax( fTMinSlab, fTMinHit ); // This would be faster if SuMin and SuMax were overloaded to use an SSE min instruction on floats
   fTMaxHit = SuMin( fTMaxSlab, fTMaxHit );
 
    
   // Z aligned slab
   if( m_direction[2] == 0 )
   {
      fTMinSlab = 0;
      fTMaxSlab = std::numeric_limits<float>::infinity();
   }
   else
   {
      fTMinSlab = ( rBBMin[2] - m_origin[2] ) * m_inverseDirection[2];
      fTMaxSlab = ( rBBMax[2] - m_origin[2] ) * m_inverseDirection[2];
      if( fTMinSlab > fTMaxSlab )
         SuSwap( fTMinSlab, fTMaxSlab ); // We could replace this with an SSE min intrinsic, it will cut it down to one instruction and remove a branch   
   }
   
   fTMinHit = SuMax( fTMinSlab, fTMinHit ); 
   fTMaxHit = SuMin( fTMaxSlab, fTMaxHit );
  
   
   // clean miss
   if( fTMinHit > fTMaxHit )  
      return false;

   // segment is outside box and points away from box
   if( fTMaxHit < 0.0f )
      return false;

   *pfTMin = fTMinHit;
   *pfTMax = fTMaxHit;
   return true;
}

//=================================================================================================================================
/// Tests for intersection with a box, but does not return intersection distances
/// \param rBBMin   Lower-left corner of box to test against
/// \param rBBMax   Upper-right corner of box to test against
/// \return True if the ray intersects the box, false otherwise
//=================================================================================================================================
inline bool SuRay::IntersectAxisAlignedBox( const SuPoint3& rBBMin, const SuPoint3& rBBMax ) const
{
   // early-out.  Since we dont care about intersection distance, skip test if ray origin is inside box
   if( m_origin.x >= rBBMin.x && m_origin.x <= rBBMax.x && 
       m_origin.y >= rBBMin.y && m_origin.y <= rBBMax.y && 
       m_origin.z >= rBBMin.z && m_origin.z <= rBBMax.z )
       return true;

   // Locate the 'T' values at which ray enters and leaves each slab
   // As we do this, locate the largest entry value and smallest exit value
   float fTMinSlab; 
   float fTMaxSlab;

   // X aligned slab
   if( m_direction[0] == 0 )
   {
      // why are these branches here?  There is a pathological case that can arise if the ray origin is right on a box corner
      // and the ray is perpendicular to an axis.  In this case, you end up with 0 * INF, which equals Nan.  Nans can result in incorrect answers
      //  The workaround is to use the full ray range if a zero direction is found, and try and reject the box based on the other two axes
      fTMinSlab = 0;
      fTMaxSlab = std::numeric_limits<float>::infinity();
   }
   else
   {
      fTMinSlab = ( rBBMin[0] - m_origin[0] ) * m_inverseDirection[0];
      fTMaxSlab = ( rBBMax[0] - m_origin[0] ) * m_inverseDirection[0];
      if( fTMinSlab > fTMaxSlab )
         SuSwap( fTMinSlab, fTMaxSlab ); // We could replace this with an SSE min intrinsic, it will cut it down to one instruction and remove a branch
   }

   float fTMinHit = fTMinSlab;
   float fTMaxHit = fTMaxSlab;
   
   // Y aligned slab
   if( m_direction[1] == 0 )
   {
      fTMinSlab = 0;
      fTMaxSlab = std::numeric_limits<float>::infinity();
   }
   else
   {
      fTMinSlab = ( rBBMin[1] - m_origin[1] ) * m_inverseDirection[1];
      fTMaxSlab = ( rBBMax[1] - m_origin[1] ) * m_inverseDirection[1];
      if( fTMinSlab > fTMaxSlab )
         SuSwap( fTMinSlab, fTMaxSlab ); // We could replace this with an SSE min intrinsic, it will cut it down to one instruction and remove a branch
   }
 
   fTMinHit = SuMax( fTMinSlab, fTMinHit ); // This would be faster if SuMin and SuMax were overloaded to use an SSE min instruction on floats
   fTMaxHit = SuMin( fTMaxSlab, fTMaxHit );
 
    
   // Z aligned slab
   if( m_direction[2] == 0 )
   {
      fTMinSlab = 0;
      fTMaxSlab = std::numeric_limits<float>::infinity();
   }
   else
   {
      fTMinSlab = ( rBBMin[2] - m_origin[2] ) * m_inverseDirection[2];
      fTMaxSlab = ( rBBMax[2] - m_origin[2] ) * m_inverseDirection[2];
      if( fTMinSlab > fTMaxSlab )
         SuSwap( fTMinSlab, fTMaxSlab ); // We could replace this with an SSE min intrinsic, it will cut it down to one instruction and remove a branch   
   }
   
   fTMinHit = SuMax( fTMinSlab, fTMinHit ); 
   fTMaxHit = SuMin( fTMaxSlab, fTMaxHit );

   // clean miss
   if( fTMinHit > fTMaxHit )  
      return false;

   // segment is outside box and points away from box
   if( fTMaxHit < 0.0f )
      return false;

   return true;
}

//=================================================================================================================================
/// \param vert0
/// \param vert1
/// \param vert2
///      Vertices of the triangle
/// \param pfTOut Receives the intersection distance, if the ray hits, unmodified otherwise.  May not be NULL
///
/// \return True if the ray intersects the triangle, false otherwise
//=================================================================================================================================
inline bool SuRay::IntersectTriangle( const SuPoint3& vert0, const SuPoint3& vert1, const SuPoint3& vert2, float* pfTOut ) const
{
   // This is the Moller Trumbore ray-triangle intersection test
   // code stolen from: http://www.cs.lth.se/home/Tomas_Akenine_Moller/raytri/raytri.c

   #define EPSILON 0.000001f
   #define CROSS(dest,v1,v2) \
             dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
             dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
             dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
   #define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
   #define SUB(dest,v1,v2) \
             dest[0]=v1[0]-v2[0]; \
             dest[1]=v1[1]-v2[1]; \
             dest[2]=v1[2]-v2[2]; 


   float edge1[3], edge2[3], tvec[3], pvec[3], qvec[3];

   // find vectors for two edges sharing vert0 
   SUB(edge1, vert1, vert0);
   SUB(edge2, vert2, vert0);

   // begin calculating determinant - also used to calculate U parameter 
   CROSS(pvec, m_direction, edge2);

   // if determinant is near zero, ray lies in plane of triangle 
   float det = DOT(edge1, pvec);

   if (det > EPSILON)
   {
      // calculate distance from vert0 to ray origin 
      SUB(tvec, m_origin, vert0);
      
      // calculate U parameter and test bounds 
      float u = DOT(tvec, pvec);
      if ( u < 0.0 || u > det)
	      return false;
      
      // prepare to test V parameter 
      CROSS(qvec, tvec, edge1);
      
      // calculate V parameter and test bounds 
      float v = DOT(m_direction, qvec);
      if ( v < 0.0 || u + v > det)
	      return false;
   }
   else if(det < -EPSILON)
   {
      // calculate distance from vert0 to ray origin 
      SUB(tvec, m_origin, vert0);
      
      // calculate U parameter and test bounds 
      float u = DOT(tvec, pvec);
      if (u > 0.0 || u < det)
	      return false;
      
      // prepare to test V parameter 
      CROSS(qvec, tvec, edge1);
      
      // calculate V parameter and test bounds 
      float v = DOT(m_direction, qvec) ;
      if (v > 0.0 || u + v < det)
	      return false;
   }
   else 
      return false;  // ray is parallell to the plane of the triangle 

   // calculate t, ray intersects triangle 
   float inv_det = 1.0f / det;
   *pfTOut = DOT(edge2, qvec) * inv_det;

   return true;
    
   // give a hoot, don't pollute 
   #undef EPSILON
   #undef CROSS
   #undef DOT
   #undef SUB
}
   

//=================================================================================================================================
/// \param fValue The position of the plane on the axis
/// \param nAxis  The axis of interest
///  Returns the 't' value at which the ray intersects an axis-aligned plane.  This will return an inf. if the ray is
///       parallel to the plane
//=================================================================================================================================
inline float SuRay::IntersectAxisAlignedPlane( float fValue, int32 nAxis ) const
{
   return ( fValue - m_origin[nAxis] ) * m_inverseDirection[nAxis];
}

//=================================================================================================================================
//
//          Protected methods block
//
//=================================================================================================================================



//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================



