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
inline SuVector2::SuVector2 () 
{
   // this doesn't initialize the vector for efficiency reasons when creating large vector arrays
}

/// Constructor
inline SuVector2::SuVector2 (float32 aX, float32 aY)
: x(aX), y(aY)
{
}

/// Copy Constructor
inline SuVector2::SuVector2 (const SuVector2& v)
: x(v.x), y(v.y)
{
}

inline SuVector2& SuVector2::operator= (const SuVector2& v)
{
   x = v.x;
   y = v.y;
   
   return *this;
}
            
inline bool SuVector2::operator== (const SuVector2& v) const
{
   return ((x == v.x) && (y == v.y));
}

inline bool SuVector2::operator!= (const SuVector2& v) const
{
   return ((x != v.x) || (y != v.y));
}

inline bool SuVector2::operator< (const SuVector2& v) const
{
   return ((x < v.x) && (y < v.y));
}

inline bool SuVector2::operator<= (const SuVector2& v) const
{
   return ((x <= v.x) && (y <= v.y));
}

inline bool SuVector2::operator> (const SuVector2& v) const
{
   return ((x > v.x) && (y > v.y));
}

inline bool SuVector2::operator>= (const SuVector2& v) const
{
   return ((x >= v.x) && (y >= v.y));
}

inline SuVector2 SuVector2::operator+ (const SuVector2& v) const
{
   SuVector2 sum;

   sum.x = x + v.x;
   sum.y = y + v.y;

   return sum;
}

inline SuVector2 SuVector2::operator- (const SuVector2& v) const
{
   SuVector2 sum;

   sum.x = x - v.x;
   sum.y = y - v.y;

   return sum;
}

inline SuVector2 SuVector2::operator* (float32 s) const
{
   SuVector2 prod;

   prod.x = x * s;
   prod.y = y * s;

   return prod;
}

inline SuVector2 SuVector2::operator/ (float32 s) const
{
   SuVector2 prod;

   prod.x = x / s;
   prod.y = y / s;

   return prod;
}

inline SuVector2 SuVector2::operator- () const
{
   SuVector2 neg;

   neg.x = -x;
   neg.y = -y;

   return neg;
}

const SuVector2 operator* (const float32 s, const SuVector2& v)
{
   SuVector2 prod;

   prod.x = v.x * s;
   prod.y = v.y * s;

   return prod;
}
      
inline SuVector2& SuVector2::operator+= (const SuVector2& v)
{
   x += v.x;
   y += v.y;
   return *this;
}

inline SuVector2& SuVector2::operator-= (const SuVector2& v)
{
   x -= v.x;
   y -= v.y;
   return *this;
}

inline SuVector2& SuVector2::operator*= (float32 s)
{
   x *= s;
   y *= s;
   return *this;
}

inline SuVector2& SuVector2::operator/= (float32 s)
{
   x /= s;
   y /= s;
   return *this;
}

inline float32& SuVector2::operator[] (int32 i)
{
   SU_ASSERT (0 <= i && i < 2);
   return *(((float32 *)this) + i);
}

inline const float32& SuVector2::operator[] (int32 i) const
{
   SU_ASSERT (0 <= i && i < 2);
   return *(((float32 *)this) + i);
}

inline float32 SuVector2::SquaredLength () const
{
   return (x*x + y*y);
}

inline float32 SuVector2::Length () const
{
   return SuSqrt (SquaredLength ());
}

inline float32 SuVector2::Dot (const SuVector2& v) const
{
   return (x*v.x + y*v.y);
}
