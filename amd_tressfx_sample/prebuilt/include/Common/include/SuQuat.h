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

#ifndef _SU_QUAT_H_
#define _SU_QUAT_H_

#include "SuVector3.h"
#include "SuMatrix4.h"

//=========================================================================================================
/// \ingroup Common
/// \brief A SuQuat is q = w + x*i + y*j + z*k where (x,y,z,w) is not necessarily a unit length 
///       vector in 4D.
///
//=========================================================================================================

/// tolua_begin
class SuQuat
{
public:
   /// Constructor
   inline SuQuat ();

   /// Copy Constructor
   inline SuQuat ( const SuQuat& rQuat );

   /// Constructor - provide x, y, z, w as separate values
   inline SuQuat ( float32 fX, float32 fY, float32 fZ, float32 fW );

   /// Constructor - construct from a rotation matrix
   inline SuQuat ( const SuMatrix4& rRotMatrix );

   /// Constructor - from axis/angle pair
   inline SuQuat ( const SuVector3& rAxis, float32 fAngle );

   /// Set the SuQuat from a rotation matrix
   SU_DLL void FromRotationMatrix ( const SuMatrix4& rRotMatrix);
   
   /// Create a rotation matrix from the SuQuat
   SU_DLL void ToRotationMatrix ( SuMatrix4& rRotMatrix ) const;
   
   /// Set the SuQuat from an axis/angle pair
   SU_DLL void FromAxisAngle ( const SuVector3& rAxis, float32 fAngle );
   
   /// Create an axis/angle pair from the SuQuat
   SU_DLL void ToAxisAngle ( SuVector3& rAxis, float32& rAngle) const;

   /// Return the length of 4-tuple
   SU_DLL float32 Length () const;

   /// Return the squared length of 4-tuple
   SU_DLL float32 SquaredLength () const; 

   /// Return the dot product of 4-tuples
   SU_DLL float32 Dot (const SuQuat& rQuat) const; 
    
   /// Make the 4-tuple unit length
   SU_DLL float32 Normalize ();

   /// Rotation of a vector by a SuQuat
   SU_DLL SuVector3 Rotate ( const SuVector3& rVector) const;

   /// Spherical linear interpolation between two SuQuats
   SU_DLL SuQuat& Slerp ( float32 fT, const SuQuat& rQuat1, const SuQuat& rQuat2 );


   // member access:  0 = w, 1 = x, 2 = y, 3 = z
   inline operator const float32* () const         { return &m_fQuat[0];   }
   inline operator float32* ()                     { return &m_fQuat[0];   }   
   inline float32 operator[] (uint32 i) const      { return m_fQuat[i];    }
   inline float32& operator[] (uint32 i)           { return m_fQuat[i];    }
   inline float32 W () const                       { return m_fQuat[0];    }
   inline float32& W ()                            { return m_fQuat[0];    }
   inline float32 X () const                       { return m_fQuat[1];    }
   inline float32& X ()                            { return m_fQuat[1];    }
   inline float32 Y () const                       { return m_fQuat[2];    }
   inline float32& Y ()                            { return m_fQuat[2];    }
   inline float32 Z () const                       { return m_fQuat[3];    }
   inline float32& Z ()                            { return m_fQuat[3];    }

   // to void*
   inline operator const void* () const            { return &m_fQuat[0];   }
   inline operator void* ()                        { return &m_fQuat[0];   }

   // arithmetic operations
   SU_DLL SuQuat operator+ (const SuQuat& rQuat) const;
   SU_DLL SuQuat operator- (const SuQuat& rQuat) const;
   SU_DLL SuQuat operator* (const SuQuat& rQuat) const;
   SU_DLL SuQuat operator* (float32 fScalar) const;
   SU_DLL SuQuat operator/ (float32 fScalar) const;
   SU_DLL SuQuat operator- () const;
   
/// tolua_end
   
   // comparison
   inline bool operator== (const SuQuat& rQuat) const;
   inline bool operator!= (const SuQuat& rQuat) const;
   inline bool operator<  (const SuQuat& rQuat) const;
   inline bool operator<= (const SuQuat& rQuat) const;
   inline bool operator>  (const SuQuat& rQuat) const;
   inline bool operator>= (const SuQuat& rQuat) const;

   // arithmetic updates
   SU_DLL SuQuat& operator+= (const SuQuat& rQuat);
   SU_DLL SuQuat& operator-= (const SuQuat& rQuat);
   SU_DLL SuQuat& operator*= (float32 fScalar);
   SU_DLL SuQuat& operator/= (float32 fScalar);
  
protected:

   /// Compare two SuQuats
   inline int32 CompareQuats ( const SuQuat& rQuat ) const;

   /// Storage for the Quaternion
   ///   w = m_fQuat[0]
   ///   x = m_fQuat[1]
   ///   y = m_fQuat[2]
   ///   z = m_fQuat[3]
   float32 m_fQuat[4];   

// tolua_begin
};
// tolua_end

/// Multiply operator
SuQuat operator* (float32 fScalar, const SuQuat& rQuat);


#include "SuQuat.inl"

#endif // _SU_QUAT_H_
