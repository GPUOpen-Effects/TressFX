//--------------------------------------------------------------------------------------
// File: Vector3D.h
//
//
// Copyright (c) 2017 Advanced Micro Devices, Inc. All rights reserved.
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
//--------------------------------------------------------------------------------------

#pragma once

#pragma warning(push)
#pragma warning(                                                                                  \
    disable : 4201)  // disable warning C4201: nonstandard extension used : nameless struct/union

#include <cmath>

namespace AMD
{
    class tressfx_vec3
    {
    public:
        union
        {
            struct
            {
                float m[4];
            };  // x, y, z, w
            struct
            {
                float x, y, z, w;
            };
        };

        tressfx_vec3()
        {
            x = y = z = 0;
            w         = 1.f;
        }

        tressfx_vec3(float x, float y, float z)
        {
            m[0] = x;
            m[1] = y;
            m[2] = z;
        };
        tressfx_vec3(float* xyz);
        tressfx_vec3(const tressfx_vec3& other);
        tressfx_vec3(const tressfx_vec3& begin, const tressfx_vec3& end);
        ~tressfx_vec3(){};

    public:
        tressfx_vec3& Normalize();
        tressfx_vec3  NormalizeOther() const;
        tressfx_vec3 Cross(const tressfx_vec3& v) const
        {
            return tressfx_vec3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
        };
        float Dot(const tressfx_vec3& v) const { return x * v.x + y * v.y + z * v.z; };
        float                         Length() const { return sqrt(x * x + y * y + z * z); };
        float                         LengthSqr() const { return (x * x + y * y + z * z); };
        void Set(float xIn, float yIn, float zIn)
        {
            m[0] = xIn;
            m[1] = yIn;
            m[2] = zIn;
        };

        const float& operator[](unsigned int i) const { return m[i]; }
        float& operator[](unsigned int i) { return m[i]; }

        tressfx_vec3& operator=(const tressfx_vec3& other);
        tressfx_vec3& operator=(float val);
        tressfx_vec3& operator=(float* xyz);
        bool operator!=(float val) const;
        bool operator<(float val) const;
        bool operator>(float val) const;
        bool operator==(float val) const;
        bool operator==(const tressfx_vec3& other) const;
        bool operator!=(const tressfx_vec3& other) const;
        tressfx_vec3& operator-=(const tressfx_vec3& other);
        tressfx_vec3& operator+=(const tressfx_vec3& other);
        tressfx_vec3& operator*=(float val);
        tressfx_vec3 operator-(const tressfx_vec3& other) const;
        tressfx_vec3 operator+(const tressfx_vec3& other) const;
        tressfx_vec3 operator/(float val) const;
        tressfx_vec3 operator*(float val) const;
        float operator*(const tressfx_vec3& other) const;

        friend tressfx_vec3 operator*(float val, const tressfx_vec3& other);
        friend tressfx_vec3 operator-(const tressfx_vec3& other);
    };

}  // namespace AMD

#pragma warning(pop)
