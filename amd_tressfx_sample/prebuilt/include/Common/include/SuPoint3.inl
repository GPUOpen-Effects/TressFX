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
inline SuPoint3::SuPoint3 () 
{
   // this doesn't initialize the vector for efficiency reasons when creating large vector arrays
}

/// Constructor
inline SuPoint3::SuPoint3 (float32 aX, float32 aY, float32 aZ)
: x(aX), y(aY), z(aZ)
{
}

/// Copy Constructor
inline SuPoint3::SuPoint3 (const SuPoint3& p)
: x(p.x), y(p.y), z(p.z)
{
}

/// Constructor
inline SuPoint3::SuPoint3 (const SuVector3& v)
: x(v.x), y(v.y), z(v.z)
{
}

inline SuPoint3& SuPoint3::operator= (const SuPoint3& v)
{
   x = v.x;
   y = v.y;
   z = v.z;
   
   return *this;
}
            
inline bool SuPoint3::operator== (const SuPoint3& v) const
{
   return ((x == v.x) && (y == v.y) && (z == v.z));
}

inline bool SuPoint3::operator!= (const SuPoint3& v) const
{
   return ((x != v.x) || (y != v.y) || (z != v.z));
}

inline bool SuPoint3::operator< (const SuPoint3& v) const
{
   return ((x < v.x) && (y < v.y) && (z < v.z));
}

inline bool SuPoint3::operator<= (const SuPoint3& v) const
{
   return ((x <= v.x) && (y <= v.y) && (z <= v.z));
}

inline bool SuPoint3::operator> (const SuPoint3& v) const
{
   return ((x > v.x) && (y > v.y) && (z > v.z));
}

inline bool SuPoint3::operator>= (const SuPoint3& v) const
{
   return ((x >= v.x) && (y >= v.y) && (z >= v.z));
}

inline SuPoint3 SuPoint3::operator+ (const SuPoint3& p) const
{
   SuPoint3 sum;

   sum.x = x + p.x;
   sum.y = y + p.y;
   sum.z = z + p.z;

   return sum;
}

inline SuPoint3 SuPoint3::operator+ (const SuVector3& v) const
{
   SuPoint3 sum;

   sum.x = x + v.x;
   sum.y = y + v.y;
   sum.z = z + v.z;

   return sum;
}

inline SuPoint3 SuPoint3::operator- (const SuVector3& v) const
{
   SuPoint3 sum;

   sum.x = x - v.x;
   sum.y = y - v.y;
   sum.z = z - v.z;

   return sum;
}

inline SuVector3 SuPoint3::operator- (const SuPoint3& p) const
{
   SuVector3 sum;

   sum.x = x - p.x;
   sum.y = y - p.y;
   sum.z = z - p.z;

   return sum;
}

inline SuPoint3 SuPoint3::operator* (float32 s) const
{
   SuPoint3 prod;

   prod.x = x * s;
   prod.y = y * s;
   prod.z = z * s;

   return prod;
}

inline SuPoint3 SuPoint3::operator/ (float32 s) const
{
   SuPoint3 prod;

   prod.x = x / s;
   prod.y = y / s;
   prod.z = z / s;

   return prod;
}

SuPoint3 operator* (float32 s, const SuPoint3& p)
{
   SuPoint3 prod;

   prod.x = p.x * s;
   prod.y = p.y * s;
   prod.z = p.z * s;

   return prod;
}
inline SuPoint3 SuPoint3::operator- () const
{
   SuPoint3 neg;

   neg.x = -x;
   neg.y = -y;
   neg.z = -z;

   return neg;
}

inline SuPoint3& SuPoint3::operator+= (const SuVector3& v)
{
   x += v.x;
   y += v.y;
   z += v.z;
   return *this;
}

inline SuPoint3& SuPoint3::operator+= (const SuPoint3& p)
{
   x += p.x;
   y += p.y;
   z += p.z;
   return *this;
}

inline SuPoint3& SuPoint3::operator-= (const SuVector3& v)
{
   x -= v.x;
   y -= v.y;
   z -= v.z;
   return *this;
}

inline SuPoint3& SuPoint3::operator-= (const SuPoint3& p)
{
	x -= p.x;
	y -= p.y;
	z -= p.z;
	return *this;
}

inline SuPoint3& SuPoint3::operator*= (float32 s)
{
   x *= s;
   y *= s;
   z *= s;
   return *this;
}

inline SuPoint3& SuPoint3::operator/= (float32 s)
{
   x /= s;
   y /= s;
   z /= s;
   return *this;
}

inline float32& SuPoint3::operator[] (int32 i)
{
   SU_ASSERT (0 <= i && i < 3);
   return *(((float32 *)this) + i);
}

inline const float32& SuPoint3::operator[] (int32 i) const
{
   SU_ASSERT (0 <= i && i < 3);
   return *(((float32 *)this) + i);
}
