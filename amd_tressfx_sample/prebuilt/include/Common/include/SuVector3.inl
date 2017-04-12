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

/// Constructor
inline SuVector3::SuVector3 () 
{
   // this doesn't initialize the vector for efficiency reasons when creating large vector arrays
}

/// Constructor
inline SuVector3::SuVector3 (float32 aX, float32 aY, float32 aZ)
: x(aX), y(aY), z(aZ)
{
}

/// Copy Constructor
inline SuVector3::SuVector3 (const SuVector3& v)
: x(v.x), y(v.y), z(v.z)
{
}

//inline SuVector3::SuVector3 (const SuPoint3& v)
//: x(v.x), y(v.y), z(v.z)
//{
//}

inline SuVector3& SuVector3::operator= (const SuVector3& v)
{
   x = v.x;
   y = v.y;
   z = v.z;
   
   return *this;
}
            
inline bool SuVector3::operator== (const SuVector3& v) const
{
   return ((x == v.x) && (y == v.y) && (z == v.z));
}

inline bool SuVector3::operator!= (const SuVector3& v) const
{
   return ((x != v.x) || (y != v.y) || (z != v.z));
}

inline bool SuVector3::operator< (const SuVector3& v) const
{
   return ((x < v.x) && (y < v.y) && (z < v.z));
}

inline bool SuVector3::operator<= (const SuVector3& v) const
{
   return ((x <= v.x) && (y <= v.y) && (z <= v.z));
}

inline bool SuVector3::operator> (const SuVector3& v) const
{
   return ((x > v.x) && (y > v.y) && (z > v.z));
}

inline bool SuVector3::operator>= (const SuVector3& v) const
{
   return ((x >= v.x) && (y >= v.y) && (z >= v.z));
}

inline SuVector3 SuVector3::operator+ (const SuVector3& v) const
{
   SuVector3 sum;

   sum.x = x + v.x;
   sum.y = y + v.y;
   sum.z = z + v.z;

   return sum;
}

inline SuVector3 SuVector3::operator- (const SuVector3& v) const
{
   SuVector3 sum;

   sum.x = x - v.x;
   sum.y = y - v.y;
   sum.z = z - v.z;

   return sum;
}

inline SuVector3 SuVector3::operator* ( const SuVector3& v ) const
{
   return SuVector3( x*v.x, y*v.y, z*v.z );
}

inline SuVector3 SuVector3::operator* (float32 s) const
{
   SuVector3 prod;

   prod.x = x * s;
   prod.y = y * s;
   prod.z = z * s;

   return prod;
}

inline SuVector3 SuVector3::operator/ (float32 s) const
{
   SuVector3 prod;

   prod.x = x / s;
   prod.y = y / s;
   prod.z = z / s;

   return prod;
}

inline SuVector3 SuVector3::operator- () const
{
   SuVector3 neg;

   neg.x = -x;
   neg.y = -y;
   neg.z = -z;

   return neg;
}

const SuVector3 operator* (const float32 s, const SuVector3& v)
{
   SuVector3 prod;

   prod.x = v.x * s;
   prod.y = v.y * s;
   prod.z = v.z * s;

   return prod;
}
      
inline SuVector3& SuVector3::operator+= (const SuVector3& v)
{
   x += v.x;
   y += v.y;
   z += v.z;
   return *this;
}

inline SuVector3& SuVector3::operator-= (const SuVector3& v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
   return *this;
}

inline SuVector3& SuVector3::operator*= (float32 s)
{
   x *= s;
   y *= s;
   z *= s;
   return *this;
}

inline SuVector3& SuVector3::operator/= (float32 s)
{
   x /= s;
   y /= s;
   z /= s;
   return *this;
}

inline float32& SuVector3::operator[] (int32 i)
{
   SU_ASSERT (0 <= i && i < 3);
   return *(((float32 *)this) + i);
}

inline const float32& SuVector3::operator[] (int32 i) const
{
   SU_ASSERT (0 <= i && i < 3);
   return *(((float32 *)this) + i);
}

inline float32 SuVector3::SquaredLength () const
{
   return (x*x + y*y + z*z);
}

inline float32 SuVector3::Length () const
{
   return SuSqrt (SquaredLength ());
}

inline float32 SuVector3::Dot (const SuVector3& v) const
{
   return (x*v.x + y*v.y + z*v.z);
}

/// The cross products are computed using the right-handed rule.  Be aware
/// that some graphics APIs use a left-handed rule.  If you have to compute
/// a cross product with these functions and send the result to the API
/// that expects left-handed, you will need to change sign on the vector
/// (replace each component value c by -c).
inline SuVector3 SuVector3::Cross (const SuVector3& v) const
{
   SuVector3 cross;
   
   cross.x = y*v.z - z*v.y;
   cross.y = z*v.x - x*v.z;
   cross.z = x*v.y - y*v.x;
   
   return cross;
}
