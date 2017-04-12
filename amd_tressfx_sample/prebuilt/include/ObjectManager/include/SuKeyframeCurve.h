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

#ifndef _SU_KEYFRAMECURVE_H_
#define _SU_KEYFRAMECURVE_H_

#include "SuAnimationCurve.h"

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief  This class represents an animation curve which is produced by interpolating keyframes
/// 
///   This class is used to handle sampled animation curves.  To construct a curve, an SuKeyframeCurve
///   instance of the appropriate data type should be created, and a sequence of keyframes should be added
///   using the AddKey method.  <br><br>
///
///   The interprettation of the keyframes varies depending on the data type of the curve.  <br><br>
///
///   For integral and boolean typed curves, the curve is assumed to be a step function, where the value at time t
///   is that specified by the latest keyframe which precedes t.  For this type of curve, no interpolation
///   is performed, and nothing special needs to be done to handle discontinuities since the curve is 
///   discontinuous by definition.<br><br>
///
///   For float-typed curves, the curve is assumed to be continuous, and the value at time t is 
///   interpolated over the time interval between the keyframes which enclose t.  For this type of curve,
///   discontinuities must be handled by adding two keyframes at the same time-point with differing values.
//=========================================================================================================

// tolua_begin
class SuKeyframeCurve : public SuAnimationCurve
{
public:

   enum InterpolationMode
   {
      LINEAR,  ///< Linear interpolation.
      SLERP    ///< Spherical linear interpolation.  Valid for quaternion-typed curves only.

      // these enums are written straight to files in binary form when saving curves.  
      // So if you add new values, put them at the end
   };

   enum CutKeyType
   {
      NO_CUT,           ///< Key isn't on a cut
      LEFT_CUT_KEY,     ///< Key is the left-hand key of a cut
      RIGHT_CUT_KEY     ///< Key is the right-hand key of a cut
   };

   /// Constructor for keyframe curves
   SU_DLL SuKeyframeCurve( SuAnimationCurve::AnimationDataType eType = FLOAT, InterpolationMode eMode = LINEAR );

   /// Destructor
   virtual SU_DLL ~SuKeyframeCurve();

   /// Accessor which returns the data-type for this animation curve
   virtual SU_DLL AnimationDataType GetDataType() const;

   /// Accessor which tells what type of curve that this is
   virtual SU_DLL AnimationCurveClass GetCurveClass() const { return KEYFRAME_CURVE; };

   /// Accessor which returns the interpolation mode for this curve
   inline InterpolationMode GetInterpolationMode( ) const { return m_eInterpMode; };

   /// Sets the interpolation mode for this curve.  The default interpolation mode is LINEAR
   inline void SetInterpolationMode( InterpolationMode eMode ) { m_eInterpMode = eMode; };

   /// Adds a key-frame to the animation
   SU_DLL void AddKey( float fTime, const void* pValue ) ;
 
   /// Special AddKey overload for floats.  This is needed because tolua can't use the other version with numbers
   inline void AddKey( float fTime, float fValue ) { AddKey( fTime, &fValue ); };

   /// Special AddKey overload for bools.  This is needed because tolua can't use the other version with booleans
   inline void AddKey( float fTime, bool bValue ) { AddKey( fTime, &bValue ); };


   /// Compresses the curve by detecting and removing unnecessary keyframes
   SU_DLL void Compress( float fErrorTolerance );

   /// Returns the number of keyframes in this curve
   SU_DLL uint32 GetNumKeys() const ;

   /// Returns the time value of the given numbered keyframe
   SU_DLL float GetKeyTime( uint32 nKeyIndex ) const; 

   /// Returns a pointer to the value of the given numbered keyframe.
   SU_DLL const uint8* GetKeyData( uint32 nKeyIndex ) const;

   /// Retrieves raw keyframe value.  Asserts if key index is invalid
   SU_DLL void GetKeyValueRaw( uint32 nKeyIndex, void* pData ) const;

   /// Retrieves keyframe value.  May be called only if the data type is float.  Asserts otherwise.
   SU_DLL void GetKeyValueFloat( uint32 nKeyIndex, float32* pfData ) const;

    /// Retrieves keyframe value.  May be called only if the data type is int.  Asserts otherwise.
   SU_DLL void GetKeyValueInt( uint32 nKeyIndex, int32* pInt ) const ;

   /// Retrieves keyframe value.   May be called only if the data type is bool.  Asserts otherwise.
   SU_DLL void GetKeyValueBool( uint32 nKeyIndex, bool* pBool ) const ;

   /// Retrieves keyframe value.    May be called only if the data type is VECTOR2.  Asserts otherwise.
   SU_DLL void GetKeyValueVec2( uint32 nKeyIndex, SuVector2* pVecOut ) const;

   /// Retrieves keyframe value.    May be called only if the data type is VECTOR3.  Asserts otherwise.
   SU_DLL void GetKeyValueVec3 ( uint32 nKeyIndex, SuVector3* pVecOut ) const;
   
   /// Retrieves keyframe value.    May be called only if the data type is VECTOR4.  Asserts otherwise.
   SU_DLL void GetKeyValueVec4( uint32 nKeyIndex, SuVector4* pVecOut ) const;

   /// Retrieves keyframe value.    May be called only if the data type is vector4_int.  Asserts otherwise.
   SU_DLL void GetKeyValueVec2Int( uint32 nKeyIndex, int32 pVecOut[2] ) const;

   /// Retrieves keyframe value.    May be called only if the data type is vector4_int.  Asserts otherwise.
   SU_DLL void GetKeyValueVec3Int( uint32 nKeyIndex, int32 pVecOut[3] ) const;

   /// Retrieves keyframe value.    May be called only if the data type is vector4_int.  Asserts otherwise.
   SU_DLL void GetKeyValueVec4Int( uint32 nKeyIndex, int32 pVecOut[4] ) const;

   /// Retrieves keyframe value .    May be called only if the data type is vector2_bool.  Asserts otherwise.
   SU_DLL void GetKeyValueVec2Bool( uint32 nKeyIndex, bool pVecOut[2] ) const;

   /// Retrieves keyframe value .    May be called only if the data type is vector3_bool.  Asserts otherwise.
   SU_DLL void GetKeyValueVec3Bool( uint32 nKeyIndex, bool pVecOut[3] ) const;

   /// Retrieves keyframe value as vec4 bool. May be called only if the data type is vector4_bool.  Asserts otherwise.
   SU_DLL void GetKeyValueVec4Bool( uint32 nKeyIndex, bool pVecOut[4] ) const;

   /// Retrieves keyframe value as matrix.  May be called only if the data type is matrix.  Asserts otherwise.
   SU_DLL void GetKeyValueMatrix( uint32 nKeyIndex, SuMatrix4* pMatOut ) const;

   /// Retrieves keyframe value as quat.    May be called only if the data type is Quaternion.  Asserts otherwise.
   SU_DLL void GetKeyValueQuat( uint32 nKeyIndex, SuQuat* pQuatOut ) const;



   /// Returns the role the key plays on a cut point
   SU_DLL CutKeyType GetCutTypeAtKey( uint32 nKeyIndex ) const;

   /// Returns the length of the animation.  This is equal to the time value for the last keyframe
   virtual SU_DLL float GetAnimationLength() const;

   /// Checks whether the curve has a cut at a given time, and returns the left cut key index
   virtual SU_DLL bool HasCutAtTime( float fTime, uint32* pLeftIndexOut ) const;

   /// Checks whether the curve has a cut inside a given time interval
   virtual SU_DLL bool HasCutInTimeInterval( float fMinTime, float fMaxTime ) const;

   /// Implementation of 'Equals' for keyframe curves
   virtual SU_DLL bool Equals( SuAnimationCurvePtr pOther ) const;




   /// Helper method that indicates whether the curve is discrete (i.e. A step curve).  All int and bool valued curves are discrete
   SU_DLL bool IsDiscrete( ) const;

   /// Writes the curve to a binary file
   virtual SU_DLL bool WriteToFile( SuFile* pFile ) const;

   /// Constructs a curve by reading it from a binary file
   static SU_DLL SuKeyframeCurve* CreateFromFile( SuFile* pFile );

   // jlm. 11-30-12, adding these 2 use keyframe functionality
   static SU_DLL  SuKeyframeCurve* CreateFromFile( const SuString& rFileName );
   virtual SU_DLL float GetValueRaw( float fTime);

   /// Method to sample the raw value at a particular time. 
   virtual SU_DLL void GetValueRaw( float fTime, void* pValueOut ) const;

   /// Method to sample the raw gradient at a particular time.
   virtual SU_DLL void GetGradientRaw( float fTime, void* pValueOut, float fGradLength ) const;

   /// Removes all keys from the keyframe curve. 
   void SU_DLL Clear();

   // tolua_end

private:

   /// Current keyframe curve file version
   static const uint16 KEYFRAME_CURVE_FILE_VERSION = 1;

   /// A header struct for a keyframe curve data file
   /// NOTE:  This structure is written directly to curve files in binary form.  Beware of changing it
   struct SuKeyframeCurveFileHeader
   {
      uint16    nVersion;     ///< Keyframe curve file version
      uint8     nDataType;    ///< Keyframe curve data type
      uint8     nInterpMode;  ///< Interpolation mode
      uint32    nKeyCount;    ///< Number of keyframes
      uint32    nDataSize;    ///< Size of keyframe value data block
   };

   /// \brief A data structure to represent a keyframe
   /// Keyframes are added to the animation curve in sorted order.
   /// It is possible for consecutive keyframes to have the same time value.  This indicates a discontinuity in the animation.
   /// NOTE:  This structure is written directly to curve files in binary form.  Beware of changing it.
   struct Keyframe
   {
      float fTime;
      uint32 nValueOffset; ///< The offset of this keyframe's value in the curve's value array (in bytes).
   };

   /// Locates the keyframe interval containing a particular time value.
   void LocateKeyframeInterval( float fTime, const Keyframe** pFirstFrame, const Keyframe** pLastFrame ) const;

   /// Interpolates between the values for two keyframes
   void ValueLerp( float fTime, const Keyframe* pFirst, const Keyframe* pSecond, void* pValueOut ) const;

   /// Helper method for linear value interpolation
   void ValueLerpLinear( float fTime, const Keyframe* pFirst, const Keyframe* pSecond, void* pValueOut ) const;

   /// Helper method for slerp value interpolation
   void ValueLerpSlerp( float fTime, const Keyframe* pFirst, const Keyframe* pSecond, void* pValueOut ) const;


   /// A helper method which obtains a pointer to a keyframe's value in the value array
   const void* GetKeyframeValue( const Keyframe& rFrame ) const;

   /// A helper method which doubles the size of the keyframe value array
   void GrowValueArray( );

   /// A helper method for compressing int or boolean valued curves
   void CompressDiscreteCurve( );
  
   /// A helper method for compressing float valued curves
   void CompressContinuousCurve( float fErrorTolerance );

   /// Returns the key index of the left cut point at a specified time
   int32 FindLeftCutKeyIndex( float fTime ) const;

   /// This value is used to optimize the search for the correct keyframe interval
   mutable uint32 m_nIntervalIndex;

   /// The current interpolation mode for this curve.
   InterpolationMode m_eInterpMode;

   /// Keyframes for this animation curve
   SuArray<Keyframe> m_keyframes;

   /// The data type of this animation curve
   AnimationDataType m_eDataType;

   /// This array is used to store the values for all keyframes in contiguous, unstructured memory.
   SuArray<uint8> m_valueArray;

// tolua_begin
};
// tolua_end


#endif // _SU_KEYFRAMECURVE_H_
