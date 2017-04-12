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


inline SuVector4::SuVector4 ()
{
}

inline SuVector4::SuVector4 (float32 aX, float32 aY, float32 aZ, float32 aW)
: x(aX), y(aY), z(aZ), w(aW)
{
}

inline SuVector4::SuVector4 (const SuVector4& v)
: x(v.x), y(v.y), z(v.z), w(v.w)
{
}

inline SuVector4::SuVector4 (const SuVector3& v, float32 aW)
: x(v.x), y(v.y), z(v.z), w(aW)
{
}

inline SuVector4& SuVector4::operator= (const SuVector4& v)
{
   x = v.x;
   y = v.y;
   z = v.z;
   w = v.w;
   
   return *this;
}
            
inline bool SuVector4::operator== (const SuVector4& v) const
{
   return ((x == v.x) && (y == v.y) && (z == v.z) && (w == v.w));
}

inline bool SuVector4::operator!= (const SuVector4& v) const
{
   return ((x != v.x) || (y != v.y) || (z != v.z) || (w != v.w));
}

inline bool SuVector4::operator< (const SuVector4& v) const
{
   return ((x < v.x) && (y < v.y) && (z < v.z) && (w < v.w));
}

inline bool SuVector4::operator<= (const SuVector4& v) const
{
   return ((x <= v.x) && (y <= v.y) && (z <= v.z) && (w <= v.w));
}

inline bool SuVector4::operator> (const SuVector4& v) const
{
   return ((x > v.x) && (y > v.y) && (z > v.z) && (w > v.w));
}

inline bool SuVector4::operator>= (const SuVector4& v) const
{
   return ((x >= v.x) && (y >= v.y) && (z >= v.z) && (w >= v.w));
}

inline SuVector4 SuVector4::operator+ (const SuVector4& v) const
{
   SuVector4 sum;

   sum.x = x + v.x;
   sum.y = y + v.y;
   sum.z = z + v.z;
   sum.w = w + v.w;

   return sum;
}

inline SuVector4 SuVector4::operator- (const SuVector4& v) const
{
   SuVector4 sum;

   sum.x = x - v.x;
   sum.y = y - v.y;
   sum.z = z - v.z;
   sum.w = w - v.w;

   return sum;
}

inline SuVector4 SuVector4::operator* (float32 s) const
{
   SuVector4 prod;

   prod.x = x * s;
   prod.y = y * s;
   prod.z = z * s;
   prod.w = w * s;

   return prod;
}

inline SuVector4 SuVector4::operator/ (float32 s) const
{
   SuVector4 prod;

   prod.x = x / s;
   prod.y = y / s;
   prod.z = z / s;
   prod.w = w / s;

   return prod;
}

inline SuVector4 SuVector4::operator- () const
{
   SuVector4 neg;

   neg.x = -x;
   neg.y = -y;
   neg.z = -z;
   neg.w = -w;

   return neg;
}

const SuVector4 operator* (const float32 s, const SuVector4& v)
{
   SuVector4 prod;

   prod.x = v.x * s;
   prod.y = v.y * s;
   prod.z = v.z * s;
   prod.w = v.w * s;

   return prod;
}
      
inline SuVector4& SuVector4::operator+= (const SuVector4& v)
{
   x += v.x;
   y += v.y;
   z += v.z;
   w += v.w;
   return *this;
}

inline SuVector4& SuVector4::operator-= (const SuVector4& v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
   w -= v.w;
   return *this;
}

inline SuVector4& SuVector4::operator*= (float32 s)
{
   x *= s;
   y *= s;
   z *= s;
   w *= s;
   return *this;
}

inline SuVector4& SuVector4::operator/= (float32 s)
{
   x /= s;
   y /= s;
   z /= s;
   w /= s;
   return *this;
}

inline float32& SuVector4::operator[] (int32 i)
{
   SU_ASSERT (0 <= i && i < 4);
   return *(((float32 *)this) + i);
}

inline const float32& SuVector4::operator[] (int32 i) const
{
   SU_ASSERT (0 <= i && i < 4);
   return *(((float32 *)this) + i);
}

inline float32 SuVector4::SquaredLength () const
{
   return (x*x + y*y + z*z + w*w);
}

inline float32 SuVector4::Length () const
{
   return SuSqrt (SquaredLength ());
}

inline float32 SuVector4::Dot (const SuVector4& v) const
{
   return (x*v.x + y*v.y + z*v.z + w*v.w);
}
