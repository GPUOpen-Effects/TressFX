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

/// Converts row and column index into an index into the matrix element array
inline int32 SuMatrix4::I (int32 iRow, int32 iCol)
{
   SU_ASSERT ((0 <= iRow) && (iRow < 4) && (0 <= iCol) && (iCol < 4));
   return 4*iCol + iRow;
}

// Constructs an uninitialized matrix
inline SuMatrix4::SuMatrix4 ()
{
}


// ====================================================================================
/// Constructs a 4x4 matrix by specifying all the components. Element m_ij
/// is located in the i'th row and j'th column of the matrix.
///
/// Example: The code \code
/// SuMatrix M( 1.0f,  2.0f,  3.0f,  4.0f,
///             5.0f,  6.0f,  7.0f,  8.0f,
///             9.0f, 10.0f, 11.0f, 12.0f,
///            13.0f, 14.0f, 15.0f, 16.0f);
/// \endcode
/// would create the matrix<br><code>
/// [ 1  2  3  4]<br>
/// [ 5  6  7  8]<br>
/// [ 9 10 11 12]<br>
/// [13 14 15 16]</code>
// ====================================================================================
inline SuMatrix4::SuMatrix4 (float32 m00, float32 m01, float32 m02, float32 m03,
                             float32 m10, float32 m11, float32 m12, float32 m13,
                             float32 m20, float32 m21, float32 m22, float32 m23,
                             float32 m30, float32 m31, float32 m32, float32 m33)
{
   m[I(0, 0)] = m00;
   m[I(1, 0)] = m10;
   m[I(2, 0)] = m20;
   m[I(3, 0)] = m30;
   m[I(0, 1)] = m01;
   m[I(1, 1)] = m11;
   m[I(2, 1)] = m21;
   m[I(3, 1)] = m31;
   m[I(0, 2)] = m02;
   m[I(1, 2)] = m12;
   m[I(2, 2)] = m22;
   m[I(3, 2)] = m32;
   m[I(0, 3)] = m03;
   m[I(1, 3)] = m13;
   m[I(2, 3)] = m23;
   m[I(3, 3)] = m33;
}

// Constructor for zero or identity matrix
inline SuMatrix4::SuMatrix4 (bool bZero)
{
   if (bZero == true)
   {
      MakeZero ();
   }
   else
   {
      MakeDiagonal (1.0f, 1.0f, 1.0f, 1.0f);
   }
}

// Copy Constructor
inline SuMatrix4::SuMatrix4 (const SuMatrix4& mat)
{
   memcpy (m, mat.m, 16*sizeof(float32));
}

inline void SuMatrix4::MakeDiagonal (const SuVector4 &v)
{
   MakeDiagonal (v.x, v.y, v.z, v.w);
}

// cast into float pointer
inline SuMatrix4::operator const float32* () const
{
   return m;
}

// cast into float pointer
inline SuMatrix4::operator float32* ()
{
   return m;
}

// element access
inline float32 SuMatrix4::operator() (int iRow, int iCol) const
{
   return m[I(iRow,iCol)];
}

// element access
inline float32& SuMatrix4::operator() (int iRow, int iCol)
{
   return m[I(iRow,iCol)];
}

// assignment operator
inline SuMatrix4& SuMatrix4::operator= (const SuMatrix4& mat)
{
   memcpy(m, mat.m, 16*sizeof(float32));
   return *this;
}

// equal comparison
inline bool SuMatrix4::operator== (const SuMatrix4& mat) const
{
   return memcmp(m, mat.m, 16*sizeof(float32)) == 0;
}

// not equal comparison
inline bool SuMatrix4::operator!= (const SuMatrix4& mat) const
{
   return memcmp(m, mat.m, 16*sizeof(float32)) != 0;
}

// less than comparins
inline bool SuMatrix4::operator<  (const SuMatrix4& mat) const
{
   return CompareArrays(mat) < 0;
}

// less equal comparison
inline bool SuMatrix4::operator<= (const SuMatrix4& mat) const
{
   return CompareArrays(mat) <= 0;
}

// greater than comparison
inline bool SuMatrix4::operator>  (const SuMatrix4& mat) const
{
   return CompareArrays(mat) > 0;
}

// greater equal comparison
inline bool SuMatrix4::operator>= (const SuMatrix4& mat) const
{
   return CompareArrays(mat) >= 0;
}

// mat-mat addition
inline SuMatrix4 SuMatrix4::operator+ (const SuMatrix4& mat) const
{
   SuMatrix4 sum;
   for (int32 i = 0; i < 16; i++)
      sum.m[i] = m[i] + mat.m[i];
   return sum;
}

// mat-mat subtraction
inline SuMatrix4 SuMatrix4::operator- (const SuMatrix4& mat) const
{
   SuMatrix4 diff;
   for (int32 i = 0; i < 16; i++)
      diff.m[i] = m[i] - mat.m[i];
   return diff;
}

// mat-scalar mult
inline SuMatrix4 SuMatrix4::operator* (float32 s) const
{
   SuMatrix4 prod;
   for (int32 i = 0; i < 16; i++)
      prod.m[i] = s * m[i];
   return prod;
}

// mat-scalar divide
inline SuMatrix4 SuMatrix4::operator/ (float32 s) const
{
   SuMatrix4 quot;
   float32 sInv = 1.0f/s;   
   for (int32 i = 0; i < 16; i++)
      quot.m[i] = sInv * m[i];
   return quot;
}

// mat negate
inline SuMatrix4 SuMatrix4::operator- () const
{
   SuMatrix4 neg;
   for (int32 i = 0; i < 16; i++)
      neg.m[i] = -m[i];
   return neg;
}

//scalar-mat multiply
inline const SuMatrix4 operator* (const float32 s, const SuMatrix4& mat)
{
   SuMatrix4 prod;
   const float32 *mm = mat;
   float32 *m = (float32 *)prod;
   for (int32 i = 0; i < 16; i++)
      m[i] = s*mm[i];
   return prod;
}

// mat-mat accumulation
inline SuMatrix4& SuMatrix4::operator+= (const SuMatrix4& mat)
{
   for (int32 i = 0; i < 16; i++)
      m[i] += mat.m[i];
   return *this;
}

// mat-mat neg. accumulation
inline SuMatrix4& SuMatrix4::operator-= (const SuMatrix4& mat)
{
   for (int32 i = 0; i < 16; i++)
      m[i] -= mat.m[i];
   return *this;
}

// mat-scalar mult
inline SuMatrix4& SuMatrix4::operator*= (float32 s)
{
   for (int32 i = 0; i < 16; i++)
      m[i] *= s;
   return *this;
}

// mat-scalar divide
inline SuMatrix4& SuMatrix4::operator/= (float32 s)
{
   float32 sInv = 1.0f/s;
   for (int32 i = 0; i < 16; i++)
      m[i] *= sInv;
   return *this;
}


inline SuMatrix4 SuMatrix4::operator* (const SuMatrix4& mat) const
{
   SuMatrix4 prod;
   int i, j;

   for (i=0; i<4; i++)
   {
      for (j=0; j<4; j++)
      {
         prod.m[i + j*4] = m[i + 0*4] * mat.m[0 + j*4] +
                           m[i + 1*4] * mat.m[1 + j*4] +
                           m[i + 2*4] * mat.m[2 + j*4] +
                           m[i + 3*4] * mat.m[3 + j*4];
      }
   }
   return prod;
}

inline SuVector4 SuMatrix4::operator* (const SuVector4& vec4) const
{
   SuVector4 prod;

   prod.x = vec4.x*m[0] + vec4.y*m[4] + vec4.z*m[8]  + vec4.w*m[12];
   prod.y = vec4.x*m[1] + vec4.y*m[5] + vec4.z*m[9]  + vec4.w*m[13];
   prod.z = vec4.x*m[2] + vec4.y*m[6] + vec4.z*m[10] + vec4.w*m[14];
   prod.w = vec4.x*m[3] + vec4.y*m[7] + vec4.z*m[11] + vec4.w*m[15];

   return prod;
}

inline SuVector3 SuMatrix4::operator* (const SuVector3& vec3) const
{
   SuVector3 prod;

   prod.x = vec3.x*m[0] + vec3.y*m[4] + vec3.z*m[8];
   prod.y = vec3.x*m[1] + vec3.y*m[5] + vec3.z*m[9];
   prod.z = vec3.x*m[2] + vec3.y*m[6] + vec3.z*m[10];

   return prod;
}

inline SuPoint3 SuMatrix4::operator* (const SuPoint3& pt) const
{
   SuPoint3 prod;

   prod.x = pt.x*m[0] + pt.y*m[4] + pt.z*m[8]  + m[12];
   prod.y = pt.x*m[1] + pt.y*m[5] + pt.z*m[9]  + m[13];
   prod.z = pt.x*m[2] + pt.y*m[6] + pt.z*m[10] + m[14];

   return prod;
}

inline SuVector4 operator* (const SuVector4& vec4, const SuMatrix4& mat)
{
   SuVector4 prod;
   const float32* m = mat;

   prod.x = vec4.x*m[0]  + vec4.y*m[1] + vec4.z*m[2]  + vec4.w*m[3];
   prod.y = vec4.x*m[4]  + vec4.y*m[5] + vec4.z*m[6]  + vec4.w*m[7];
   prod.z = vec4.x*m[8]  + vec4.y*m[9] + vec4.z*m[10] + vec4.w*m[11];
   prod.w = vec4.x*m[12] + vec4.y*m[13] + vec4.z*m[14] + vec4.w*m[15];

   return prod;
}

inline SuVector3 operator* (const SuVector3& vec3, const SuMatrix4& mat)
{
   SuVector3 prod;
   const float32* m = mat;

   prod.x = vec3.x*m[0] + vec3.y*m[1] + vec3.z*m[2];
   prod.y = vec3.x*m[4] + vec3.y*m[5] + vec3.z*m[6];
   prod.z = vec3.x*m[8] + vec3.y*m[9] + vec3.z*m[10];

   return prod;
}

inline void SuMatrix4::SetRow (int32 iRow, const SuVector4& vec)
{
   SU_ASSERT ((0 <= iRow) && (iRow < 4));
   m[I(iRow, 0)] = vec.x;
   m[I(iRow, 1)] = vec.y;
   m[I(iRow, 2)] = vec.z;
   m[I(iRow, 3)] = vec.w;
}

inline SuVector4 SuMatrix4::GetRow (int32 iRow) const
{
   SU_ASSERT ((0 <= iRow) && (iRow < 4));
   SuVector4 vec;

   vec.x = m[I(iRow, 0)];
   vec.y = m[I(iRow, 1)];
   vec.z = m[I(iRow, 2)];
   vec.w = m[I(iRow, 3)];
   return vec;
}

inline void SuMatrix4::SetColumn (int32 iCol, const SuVector4& vec)
{
   SU_ASSERT ((0 <= iCol) && (iCol < 4));
   m[I(0, iCol)] = vec.x;
   m[I(1, iCol)] = vec.y;
   m[I(2, iCol)] = vec.z;
   m[I(3, iCol)] = vec.w;
}

inline SuVector4 SuMatrix4::GetColumn (int32 iCol) const
{
   SU_ASSERT ((0 <= iCol) && (iCol < 4));
   SuVector4 vec;

   vec.x = m[I(0, iCol)];
   vec.y = m[I(1, iCol)];
   vec.z = m[I(2, iCol)];
   vec.w = m[I(3, iCol)];
   return vec;
}