//--------------------------------------------------------------------------------------
// File: Vector3D.cpp
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

#include "Math/Vector3D.h"
#include <assert.h>
#include <memory.h>

namespace AMD
{
    tressfx_vec3::tressfx_vec3(const tressfx_vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    tressfx_vec3::tressfx_vec3(const tressfx_vec3& begin, const tressfx_vec3& end)
    {
        x = end.x - begin.x;
        y = end.y - begin.y;
        z = end.z - begin.z;
    }

    tressfx_vec3::tressfx_vec3(float* xyz) { memcpy(m, xyz, sizeof(tressfx_vec3)); }

    tressfx_vec3& tressfx_vec3::Normalize()
    {
        float d = sqrt(x * x + y * y + z * z);

        if (d == 0)
        {
            return *this;
        }

        x = x / d;
        y = y / d;
        z = z / d;

        return *this;
    }

    tressfx_vec3 tressfx_vec3::NormalizeOther() const
    {
        tressfx_vec3 n(*this);
        return n.Normalize();
    }

    tressfx_vec3 tressfx_vec3::operator-(const tressfx_vec3& other) const
    {
        return tressfx_vec3(x - other.x, y - other.y, z - other.z);
    }

    tressfx_vec3 tressfx_vec3::operator+(const tressfx_vec3& other) const
    {
        return tressfx_vec3(x + other.x, y + other.y, z + other.z);
    }

    tressfx_vec3 tressfx_vec3::operator/(float val) const
    {
        if (val != 0.0f)
        {
            return tressfx_vec3(x / val, y / val, z / val);
        }
        else
        {
            return tressfx_vec3(0.0f, 0.0f, 0.0f);
        }
    }

    tressfx_vec3& tressfx_vec3::operator=(const tressfx_vec3& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;

        return *this;
    }

    tressfx_vec3& tressfx_vec3::operator=(float val)
    {
        x = val;
        y = val;
        z = val;

        return *this;
    }

    tressfx_vec3& tressfx_vec3::operator=(float* xyz)
    {
        memcpy(m, xyz, sizeof(tressfx_vec3));
        return *this;
    }

    bool tressfx_vec3::operator<(float val) const { return (LengthSqr() < val * val); }

    bool tressfx_vec3::operator>(float val) const { return (LengthSqr() > val * val); }

    bool tressfx_vec3::operator!=(float val) const { return (x != val || y != val || z != val); }

    bool tressfx_vec3::operator==(float val) const { return (x == val && y == val && z == val); }

    bool tressfx_vec3::operator==(const tressfx_vec3& other) const
    {
        return (x == other.x && y == other.y && z == other.z);
    }

    bool tressfx_vec3::operator!=(const tressfx_vec3& other) const
    {
        return (x != other.x || y != other.y || z != other.z);
    }

    tressfx_vec3& tressfx_vec3::operator-=(const tressfx_vec3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    tressfx_vec3& tressfx_vec3::operator+=(const tressfx_vec3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    tressfx_vec3& tressfx_vec3::operator*=(float val)
    {
        x *= val;
        y *= val;
        z *= val;

        return *this;
    }

    tressfx_vec3 tressfx_vec3::operator*(float val) const
    {
        tressfx_vec3 vec(*this);

        vec.x *= val;
        vec.y *= val;
        vec.z *= val;

        return vec;
    }

    float tressfx_vec3::operator*(const tressfx_vec3& other) const { return Dot(other); }


    tressfx_vec3 operator*(float val, const tressfx_vec3& other) { return other * val; }

    tressfx_vec3 operator-(const tressfx_vec3& other)
    {
        tressfx_vec3 vec(other);

        vec.x = -vec.x;
        vec.y = -vec.y;
        vec.z = -vec.z;

        return vec;
    }

}  // namespace AMD
