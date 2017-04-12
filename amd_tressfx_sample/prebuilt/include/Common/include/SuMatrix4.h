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

#ifndef _SUMATRIX4_H
#define _SUMATRIX4_H

#include "SuTypes.h"
#include "SuVector3.h"
#include "SuVector4.h"
#include "SuPoint3.h"
#include "SuAssert.h"
#include <string.h>

class SuQuat;

/// \brief 4x4 Matrix with float32 elements
///
/// \ingroup Math
/// Internally the matrix is stored in column-major order, but the user must not rely on this.
/// SuMatrix4 offers the following accessors that allow the code to be remain independent of the internal matrix
/// representation: operator(), SetRow(), GetRow(), SetColumn(), GetColumn(), GetRowMajor(), GetColumnMajor()

// tolua_begin
class SuMatrix4 
{
// tolua_end

   private:
      float32 m[16];

   public:
      /// Constructs an uninitialized matrix
      inline SuMatrix4();                                                  // tolua_export

      /// Constructor for zero or identity matrix
      inline explicit SuMatrix4( bool bZero );                               // tolua_export

      // tolua_begin
      /// Constructor
      inline SuMatrix4( float32 m00, float32 m10, float32 m20, float32 m30,
                       float32 m01, float32 m11, float32 m21, float32 m31,
                       float32 m02, float32 m12, float32 m22, float32 m32,
                       float32 m03, float32 m13, float32 m23, float32 m33 );
      // tolua_end

      /// Copy Constructor
      inline SuMatrix4( const SuMatrix4& mat );                              // tolua_export

      /// Makes a zero matrix
      SU_DLL void MakeZero();                                                     // tolua_export

      /// Makes an identity matrix
      SU_DLL void MakeIdentity();                                                 // tolua_export

      /// Make a matrix from a matrix
      SU_DLL void MakeFromMatrix( const SuMatrix4& rMat );                        // tolua_export

      /// Makes a diagonal matrix
      SU_DLL void MakeDiagonal( float32 d0, float32 d1, float32 d2, float32 d3 );   // tolua_export

      /// Makes a diagonal matrix
      inline void MakeDiagonal( const SuVector4& v );                        // tolua_export

      /// Makes a translation matrix
      SU_DLL void MakeTranslation( float32 x, float32 y, float32 z );               // tolua_export

      /// Makes a scaling matrix
      SU_DLL void MakeScaling( float32 x, float32 y, float32 z );                    // tolua_export

      /// Makes a rotation matrix from a rotation axis and an angle
      SU_DLL void MakeFromAxisAngle( const SuVector3& rAxis, float32 fAngle );      // tolua_export

      /// Makes a rotation matrix that rotates one vector to another
      SU_DLL void MakeFromToRotation( const SuVector3& from, const SuVector3& to );  // tolua_export

      /// Makes an orthonormal basis matrix from one given axis
      SU_DLL void MakeFromArbitraryAxis( const SuVector3& axis, float32 rotScale );  // tolua_export

      /// Sets a matrix from decomposed translation, rotation, and scaling components
      SU_DLL void MakeTransform( const SuVector3& rTranslation, const SuQuat& rRotation, const SuVector3& rScaleFactors ); // tolua_export

      /// Checks if the matrix is the identity matrix
      SU_DLL bool IsIdentity() const;                                              // tolua_export

      /// Creates a transpose of the matrix
      SU_DLL SuMatrix4 Transpose() const;                                          // tolua_export

      /// Transposes the matrix in-place.  This is faster than using Transpose()
      SU_DLL void TransposeSelf();                                                 // tolua_export

      /// Creates the inverse of the matrix
      SU_DLL SuMatrix4 Inverse() const;                                            // tolua_export

      /// Creates the inverse of an orthonormal matrix
      SU_DLL SuMatrix4 RigidInverse() const;                                       // tolua_export

      /// Normalizes the axes in the upper 3x3 block
      SU_DLL SuMatrix4 UnitizeAxes() const;                                        // tolua_export

      // member access
      /// Casts matrix to float array pointer
      inline operator const float32*() const;
      /// Casts matrix to float array pointer
      inline operator float32*();

      /// Individual matrix element access
      inline float32 operator()( int iRow, int iCol ) const;
      /// Individual matrix element access
      inline float32& operator()( int iRow, int iCol );
     
      /// Individual matrix element access in lua
      inline float32 Get( int iRow, int iCol ) const { return operator()( iRow, iCol ); }; // tolua_export

      /// Invididual matrix element manipulation in lua
      inline void Set( int iRow, int iCol, float32 fValue )  { operator()( iRow, iCol ) = fValue; }; // tolua_export

      /// Replaces a matrix row with the given row vector
      inline void SetRow( int32 iRow, const SuVector4& rowVec );                    // tolua_export

      /// Returns a row as a vector
      inline SuVector4 GetRow( int32 iRow ) const;                                  // tolua_export

      /// Replaces a matrix column with the given column vector
      inline void SetColumn(int32 iCol, const SuVector4& colVec );                 // tolua_export

      /// Returns a column as a vector
      inline SuVector4 GetColumn( int32 iCol ) const;                               // tolua_export

      /// Converts the matrix into a row-major float array
      SU_DLL void GetRowMajor( float32* pData ) const;

      /// Converts the matrix into a 4x3 row-major float array
      SU_DLL void GetRowMajor4x3( float32* pData ) const;

      /// Converts the matrix into a column-major float array
      SU_DLL void GetColumnMajor( float32* pData ) const;

      /// Sets the matrix with a column-major float array
      SU_DLL void SetColumnMajor( const float32* pData ) ;

      /// Decomposes a transformation matrix into its translation, rotation, and scaling components
      SU_DLL void Decompose( SuVector3* pTranslation, SuQuat* pRotation, SuVector3* pScaleFactors ) const; // tolua_export

      // assignment
      inline SuMatrix4& operator=( const SuMatrix4& mat );

      // comparison
      inline bool operator==( const SuMatrix4& mat ) const;                  // tolua_export
      inline bool operator!=( const SuMatrix4& mat ) const;
      inline bool operator< ( const SuMatrix4& mat ) const;                  // tolua_export
      inline bool operator<=( const SuMatrix4& mat ) const;                  // tolua_export
      inline bool operator> ( const SuMatrix4& mat ) const;
      inline bool operator>=( const SuMatrix4& mat ) const;

      // arithmetic operations
      /// Matrix-matrix addition
      inline SuMatrix4 operator+( const SuMatrix4& mat ) const;              // tolua_export
      /// Matrix-matrix subtraction
      inline SuMatrix4 operator-( const SuMatrix4& mat ) const;              // tolua_export
      /// Matrix-Matrix multiplication
      inline SuMatrix4 operator*( const SuMatrix4& mat ) const;              // tolua_export
      /// Matrix-scalar multiplication
      inline SuMatrix4 operator*( float32 s ) const;                         // tolua_export
      /// Matrix-scalar division
      inline SuMatrix4 operator/( float32 s ) const;                         // tolua_export
      /// Matrix negation
      inline SuMatrix4 operator-() const;                                  // tolua_export

      // arithmetic updates
      /// Matrix-matrix addition
      inline SuMatrix4& operator+=( const SuMatrix4& mat );
      /// Matrix-matrix subtraction
      inline SuMatrix4& operator-=( const SuMatrix4& mat );
      /// Matrix-scalar multiplication
      inline SuMatrix4& operator*=( float32 s );
      /// Matrix-scalar division
      inline SuMatrix4& operator/=( float32 s );

      // matrix-vector operations
      /// Matrix-vector multiplication (multiplies a column vector from the right)
      inline SuVector4 operator*( const SuVector4& vec4 ) const;                    // tolua_export
      /// Matrix-vector multiplication using the upper 3x3 matrix (multiplies a column vector from the right)
      inline SuVector3 operator*( const SuVector3& vec3 ) const;                    // tolua_export
      /// Transforms a point with the matrix
      inline SuPoint3 operator*( const SuPoint3& pt ) const;                        // tolua_export

      /// Zero matrix
      static SU_DLL const SuMatrix4 ZERO;                                          // tolua_export

      /// Identity matrix
      static SU_DLL const SuMatrix4 IDENTITY;                                      // tolua_export

   private:
      SU_DLL int32 CompareArrays( const SuMatrix4& mat ) const;
      inline static int32 I( int32 iRow, int32 iCol );

// tolua_begin
};
// tolua_end

SU_DLL void SuGetPitchYawFromMatrix( const SuMatrix4& rMat, float32* pfPitch, float32* pfYaw );

SU_DLL SuMatrix4 SuMakeRotationMatrixFromPitchYaw( float32 fPitch, float32 fYaw );

// c * M
/// Scalar-matrix multiply
inline const SuMatrix4 operator* (const float32 s, const SuMatrix4& mat);

// v^T * M
/// Multiplies a row-vector from the left with a matrix
inline SuVector4 operator* (const SuVector4& vec4, const SuMatrix4& mat);

/// Multiplies a row-vector from the left with the upper 3x3 portion of a matrix
inline SuVector3 operator* (const SuVector3& vec3, const SuMatrix4& mat);

#include "SuMatrix4.inl"




#endif
