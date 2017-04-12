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
#include "SuPlane.h"


//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block 
//
//=================================================================================================================================

//=========================================================================================================
/// Constructor
//=========================================================================================================
inline SuPlane::SuPlane()
{
}

//=================================================================================================================================
/// Constructor - construct directly from an implicit plane equation (Ax + By + Cz + D = 0 )
/// \param fA X component of plane normal
/// \param fB Y component of plane normal
/// \param fC Z component of plane normal
/// \param fD This is the negative of the plane constant that would be used in the other constructors
//=================================================================================================================================

inline SuPlane::SuPlane( float fA, float fB, float fC, float fD ) : m_vNormal( fA, fB, fC ), m_fConstant( -fD )
{
}

//=========================================================================================================
/// Constructor - construct directly from normal N and constant C
///
/// \param rNormal Unit normal for the plane (N)
/// \param fConstant Constant for the plane (C)
//=========================================================================================================   
inline SuPlane::SuPlane ( const SuVector3& rNormal, float32 fConstant ) :
   m_vNormal ( rNormal ),
   m_fConstant ( fConstant )
{
}

//=========================================================================================================
/// Constructor - construct from a plane Normal (N) and point on plane (P)
///
/// \param rNormal Unit normal for the plane (N)
/// \param rPoint Point on plane (P)
//=========================================================================================================
inline SuPlane::SuPlane ( const SuVector3& rNormal, const SuVector3& rPoint ) :
   m_vNormal ( rNormal )
{
   m_fConstant = m_vNormal.Dot ( rPoint );
}

//=========================================================================================================
/// Constructor - construct from three points on the plane P0, P1, P2
/// 
/// \param rPoint0 Point on plane (P0)
/// \param rPoint1 Point on plane (P1)
/// \param rPoint2 Point on plane (P2)
//=========================================================================================================
inline SuPlane::SuPlane ( const SuVector3& rPoint0, const SuVector3 &rPoint1,
                   const SuVector3& rPoint2 )
{
   SuVector3 vEdge0 = rPoint1 - rPoint0;
   SuVector3 vEdge1 = rPoint2 - rPoint1;

   m_vNormal = vEdge0.Cross ( vEdge1 );
   m_vNormal.Normalize ();
   m_fConstant = m_vNormal.Dot ( rPoint0 );
}



//=========================================================================================================
/// Copy Constructor
//=========================================================================================================
inline SuPlane::SuPlane ( const SuPlane& rPlane ) :
   m_vNormal ( rPlane.GetNormal() ),
   m_fConstant ( rPlane.GetConstant() )
{
  
}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================


//=========================================================================================================      
/// Set the normal directly
///
/// \param rNormal Unit normal for the plane (N)
//=========================================================================================================      
inline void SuPlane::SetNormal ( const SuVector3& rNormal )
{
   m_vNormal = rNormal;
}

//=========================================================================================================
/// Get the normal
//=========================================================================================================
inline const SuVector3& SuPlane::GetNormal () const
{
   return m_vNormal;
}


//=========================================================================================================         
/// Set the constant directly
///
/// \param fConstant Constant for the plane (C)
//=========================================================================================================            
inline void SuPlane::SetConstant ( float32 fConstant )
{
   m_fConstant = fConstant;
}

//=========================================================================================================
/// Get the constant
//=========================================================================================================
inline const float32 SuPlane::GetConstant () const
{
   return m_fConstant;
}


//=========================================================================================================
/// Get the plane a SuVector4( N.x, N.y, N.z, C )
//=========================================================================================================
inline SuVector4 SuPlane::GetAsVector4() const
{
   return SuVector4 ( m_vNormal, m_fConstant );
}

//=========================================================================================================
/// Get the distance from the point to the plane
///
/// \param rPoint Point to test against the distance to plane
/// \return Distance from rPoint to plane
//=========================================================================================================
inline const float32 SuPlane::GetDistanceTo ( const SuVector3 &rPoint ) const
{
   return m_vNormal.Dot ( rPoint ) - m_fConstant;
}

//=========================================================================================================
/// Determine which side of the plane a point lies on
///
/// \param rPoint Point to test against the plane
/// \return 0 if the point is on the plane, 
///         1 if the point is on the side of the normal direction, 
///        -1 if the point is on the opposite side of the normal direction
//=========================================================================================================
inline const int32 SuPlane::GetWhichSide ( const SuVector3 &rPoint ) const
{
   float32 fDistance = GetDistanceTo ( rPoint );

   if ( fDistance < 0.0f )
   {
      return -1;
   }
   if ( fDistance > 0.0f )
   {
      return 1;
   }

   return 0;
}


//=================================================================================================================================
/// Modifies the plane so that its normal is of unit length.  The plane constant is scaled accordingly
//=================================================================================================================================

inline void SuPlane::Normalize()
{
   float fLen = m_vNormal.Length();
   m_vNormal /= fLen;
   m_fConstant /= fLen;
}


//=========================================================================================================
/// Assignment
//=========================================================================================================
inline SuPlane& SuPlane::operator= (const SuPlane& rPlane)
{
   m_vNormal = rPlane.GetNormal ();
   m_fConstant = rPlane.GetConstant (); 
   return *this;
}

//=========================================================================================================
/// Comparison equals
//=========================================================================================================
inline bool SuPlane::operator== (const SuPlane& rPlane) const
{
   return ( (m_vNormal == rPlane.m_vNormal) && (m_fConstant == rPlane.m_fConstant) );
}

//=========================================================================================================
/// Comparison not equals   
//=========================================================================================================
inline bool SuPlane::operator!= (const SuPlane& rPlane) const
{
   return ( (m_vNormal != rPlane.m_vNormal) || (m_fConstant != rPlane.m_fConstant) );
}

//=========================================================================================================
/// \param rOrigin      Ray origin
/// \param rDirection   Ray direction
/// \return The value 'T' such that rOrigin + rDirection*T equals the hit location.  Nan if the ray misses the plane
//=========================================================================================================
inline float SuPlane::RayIntersect( const SuPoint3& rOrigin, const SuVector3& rDirection ) const
{
   // ray plane intersection for plane equation form:  ax + by + cz + d = 0
   //  is  -( N.O + D ) / N.D.  Note that the SuPlane class has m_fContant == -d
   return (m_fConstant - m_vNormal.Dot( rOrigin.AsVector() ) ) / m_vNormal.Dot( rDirection );
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



