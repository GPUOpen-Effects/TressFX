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


#include "SuQuat.h"



//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block 
//
//=================================================================================================================================

//=========================================================================================================
/// Constructor
//=========================================================================================================
inline SuQuat::SuQuat ()
{
}

//=========================================================================================================
/// Copy Constructor
//=========================================================================================================
inline SuQuat::SuQuat ( const SuQuat& rQuat )   
{
   memcpy ( &m_fQuat[0], &rQuat.m_fQuat[0], sizeof(m_fQuat) );
}

//=========================================================================================================
/// Constructor - provide x, y, z, w as separate values
//=========================================================================================================
inline SuQuat::SuQuat ( float32 fX, float32 fY, float32 fZ, float32 fW )   
{
   m_fQuat[0] = fW;
   m_fQuat[1] = fX;
   m_fQuat[2] = fY;
   m_fQuat[3] = fZ;
}

//=========================================================================================================
/// Constructor - construct from a rotation matrix
/// \param rRotMatrix Rotation matrix to construct quaternion from
//=========================================================================================================
inline SuQuat::SuQuat ( const SuMatrix4& rRotMatrix )
{
   FromRotationMatrix ( rRotMatrix );
}

//=========================================================================================================
/// Constructor - from axis/angle pair
/// \param rAxis Axis around which to rotate
/// \param fAngle Angle to rotate around axis (in radians)
//=========================================================================================================
inline SuQuat::SuQuat ( const SuVector3& rAxis, float32 fAngle )
{
   FromAxisAngle ( rAxis, fAngle );
}


//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=========================================================================================================
/// Comparison operator equals
//=========================================================================================================
inline bool SuQuat::operator== (const SuQuat& rQuat) const
{
   return CompareQuats ( rQuat ) == 0;
}

//=========================================================================================================
/// Comparison operator not equals
//=========================================================================================================
inline bool SuQuat::operator!= (const SuQuat& rQuat) const
{
   return CompareQuats ( rQuat ) != 0;
}

//=========================================================================================================
/// Comparison operator less
//=========================================================================================================
inline bool SuQuat::operator<  (const SuQuat& rQuat) const
{
   return CompareQuats ( rQuat ) < 0;
}

//=========================================================================================================
/// Comparison operator less equal
//=========================================================================================================
inline bool SuQuat::operator<= (const SuQuat& rQuat) const
{
   return CompareQuats ( rQuat ) <= 0;
}

//=========================================================================================================
/// Comparison operator greater
//=========================================================================================================
inline bool SuQuat::operator>  (const SuQuat& rQuat) const
{
   return CompareQuats ( rQuat ) > 0;
}

//=========================================================================================================
/// Comparison operator greater equal
//=========================================================================================================
inline bool SuQuat::operator>= (const SuQuat& rQuat) const
{
   return CompareQuats ( rQuat ) >= 0;
}


//=================================================================================================================================
//
//          Protected methods block
//
//=================================================================================================================================

//=========================================================================================================
/// Compare two quaternions
//=========================================================================================================
int32 SuQuat::CompareQuats ( const SuQuat& rQuat ) const
{
   return memcmp(m_fQuat, rQuat.m_fQuat, 4 * sizeof(float32) );
}

//=================================================================================================================================
//
//          Private methods block
//
//=================================================================================================================================



