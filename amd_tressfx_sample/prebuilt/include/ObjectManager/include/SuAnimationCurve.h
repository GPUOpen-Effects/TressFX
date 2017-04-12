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

#ifndef _SU_ANIMATIONCURVE_H_
#define _SU_ANIMATIONCURVE_H_

#include "SuTypes.h"
#include "SuFile.h"
#include "SuCountedPtr.h"

// tolua_begin
class SuAnimationCurve;
typedef SuCountedPtr<SuAnimationCurve> SuAnimationCurvePtr;
// tolua_end

class SuVector2;
class SuVector3;
class SuVector4;
class SuMatrix4;
class SuQuat;

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief A class to represent an animation curve
///
/// This class defines the interface for an animation curve.  Subclasses might include the general key-frame 
/// curve class, or a more specific curve type such as a bezier curve
///
//=========================================================================================================

// tolua_begin
class SuAnimationCurve
{
public:

   /// Enumeration of the data types supported for animation curves.
   enum AnimationDataType
   {
      FLOAT,
      INT,
      BOOL,
      VECTOR2,
      VECTOR3,
      VECTOR4,
      QUATERNION,
      VECTOR2_INT,
      VECTOR3_INT,
      VECTOR4_INT,
      VECTOR2_BOOL,
      VECTOR3_BOOL,
      VECTOR4_BOOL,
      MATRIX,
      // A reminder:  These enum values are written directly to animation files in binary form, so if you add any types
      // be sure to add them at the end
      // Another reminder:  If you add any types, update the MAX_DATA_SIZE as needed.
   };

   /// Enumeration of animation curve sub-classes
   enum AnimationCurveClass
   {
      KEYFRAME_CURVE = 0   ///< A point-sampled keyframe curve

   };


   /// Virtual Destructor
   virtual SU_DLL ~SuAnimationCurve();

   /// Accessor which tells what type of curve that this is
   virtual SU_DLL AnimationCurveClass GetCurveClass() const = 0;

   /// Accessor which returns the data-type for this animation curve
   virtual SU_DLL AnimationDataType GetDataType() const = 0;

   /// Returns the length of the animation for this curve
   virtual SU_DLL float GetAnimationLength() const = 0;

   // tolua_end

   /// Sampling method for floats.  May be called only if the data type is float.  Asserts otherwise.
   SU_DLL void GetValueFloat( float fTime, float* pFloat ) const ;

   // tolua_begin

   /// Tolua friendly version for floats.  May be called only if the data type is float.  Asserts otherwise.
   SU_DLL float32 GetValueFloat( float fTime ) const ;

   /// Sampling method for ints.  May be called only if the data type is int.  Asserts otherwise.
   SU_DLL void GetValueInt( float fTime, int32* pInt ) const ;

   /// Sampling method for bools.  May be called only if the data type is bool.  Asserts otherwise.
   SU_DLL void GetValueBool( float fTime, bool* pBool ) const ;

   /// Sampling method for vectors.  May be called only if the data type is VECTOR2.  Asserts otherwise.
   SU_DLL void GetValueVec2( float fTime, SuVector2* pVecOut ) const;

   /// Sampling method for vectors.  May be called only if the data type is VECTOR3.  Asserts otherwise.
   SU_DLL void GetValueVec3 ( float fTime, SuVector3* pVecOut ) const;
   
   /// Sampling method for vectors.  May be called only if the data type is VECTOR4.  Asserts otherwise.
   SU_DLL void GetValueVec4( float fTime, SuVector4* pVecOut ) const;

   /// Sampling method for int vectors.  May be called only if the data type is vector4_int.  Asserts otherwise.
   SU_DLL void GetValueVec2Int( float fTime, int32 pVecOut[2] ) const;

   /// Sampling method for int vectors.  May be called only if the data type is vector4_int.  Asserts otherwise.
   SU_DLL void GetValueVec3Int( float fTime, int32 pVecOut[3] ) const;

   /// Sampling method for int vectors.  May be called only if the data type is vector4_int.  Asserts otherwise.
   SU_DLL void GetValueVec4Int( float fTime, int32 pVecOut[4] ) const;

   /// Sampling method for bool vectors.  May be called only if the data type is vector2_bool.  Asserts otherwise.
   SU_DLL void GetValueVec2Bool( float fTime, bool pVecOut[2] ) const;

   /// Sampling method for bool vectors.  May be called only if the data type is vector3_bool.  Asserts otherwise.
   SU_DLL void GetValueVec3Bool( float fTime, bool pVecOut[3] ) const;

   /// Sampling method for bool vectors.  May be called only if the data type is vector4_bool.  Asserts otherwise.
   SU_DLL void GetValueVec4Bool( float fTime, bool pVecOut[4] ) const;

   /// Sampling method for matrices.  May be called only if the data type is matrix.  Asserts otherwise.
   SU_DLL void GetValueMatrix( float fTime, SuMatrix4* pMatOut ) const;

   /// Sampling method for quaternions.  May be called only if the data type is Quaternion.  Asserts otherwise.
   SU_DLL void GetValueQuat( float fTime, SuQuat* pQuatOut ) const;

   /// Computes the first derivative of the curve (change in value per unit change in time).
   SU_DLL void GetGradientFloat( float fTime, float* pGrad, float fGradLength=1.0f ) const;

   /// Computes the first derivative of the curve (change in value per unit change in time).
   SU_DLL void GetGradientVec2( float fTime, SuVector2* pVecOut, float fGradLength=1.0f ) const;
   
   /// Computes the first derivative of the curve (change in value per unit change in time).
   SU_DLL void GetGradientVec3( float fTime, SuVector3* pVecOut, float fGradLength=1.0f ) const;
   
   /// Computes the first derivative of the curve (change in value per unit change in time).
   SU_DLL void GetGradientVec4( float fTime, SuVector4* pVecOut, float fGradLength=1.0f ) const;

   /// Computes the first derivative of the curve (change in value per unit change in time).
   SU_DLL void GetGradientMatrix( float fTime, SuMatrix4* pMatOut, float fGradLength=1.0f ) const;

   /// Computes the first derivative of the curve (change in value per unit change in time).
   SU_DLL void GetGradientQuat( float fTime, SuQuat* pQuatOut, float fGradLength=1.0f ) const;

   /// Checks whether the curve has a cut anywhere inside a particular time interval
   virtual SU_DLL bool HasCutInTimeInterval( float fMinTime, float fMaxTime ) const = 0;

   //=================================================================================================================================
   /// \brief Determines whether or not two curves are identical.
   ///    Subclass implementations are responsible for ensuring that the class of the curve passed to this method is compatible 
   ///    with the class of the calling curve.  The result of comparing curves of different classes is not well defined.  
   ///    Subclasses may return false if the class of the curve passed is not the same as their own class.  Subclasses may also
   ///    return a conservative estimate (false) if exact comparison is expensive or difficult.
   ///  
   /// \return True if the two curves are identical.  False if they are not, or if the curve classes are incompatible
   //=================================================================================================================================
   virtual SU_DLL bool Equals( SuAnimationCurvePtr pOther ) const = 0;

// tolua_end

   /// Method to sample the raw value at a particular time.  
   /// \param fTime
   ///   The time value at which to sample
   /// \param pValueOut
   ///   Pointer to a block of data which will receive the value.  The amount of data written
   ///   is equal to the value returned by GetValueSize().  Caller is responsible for ensuring that
   ///   the pointer points at a sufficiently large chunk of memory.
   virtual SU_DLL void GetValueRaw( float fTime, void* pValueOut ) const = 0;

   /// Method to compute the gradient of the curve at a particular time.  This method should not be called for
   /// integer and boolean valued curves
   /// \param fTime
   ///     The time value at which to calculate the gradient
   /// \param pValueOut
   ///   Pointer to a block of data which will receive the value.  The amount of data written
   ///   is equal to the value returned by GetValueSize().  Caller is responsible for ensuring that
   ///   the pointer points at a sufficiently large chunk of memory.   
   /// \param fGradLength  Length of the desired gradient (size of time step)
   virtual SU_DLL void GetGradientRaw( float fTime, void* pValueOut, float fGradLength ) const = 0;


   /// Stores the curve in a binary file
   virtual SU_DLL bool WriteToFile( SuFile* pFile ) const = 0;


   /// Helper method which returns the size of the values produced by this curve.
   SU_DLL uint32 GetValueSize() const;

   static const size_t MAX_DATA_SIZE = 16 * 4; // sizeof(SuMatrix4)



// tolua_begin
};
// tolua_end

#endif // _SU_ANIMATIONCURVE_H_
