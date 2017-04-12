//--------------------------------------------------------------------------------------
// File: Quaternion.cpp
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

#include "Math/Quaternion.h"
#include <cassert>
#include <math.h>
#include <memory.h>

namespace AMD
{
    tressfx_quat::tressfx_quat(float x /* = 0.0*/,
                               float y /* = 0.0*/,
                               float z /* = 0.0*/,
                               float w /* = 1.0*/)
        : x(x), y(y), z(z), w(w)
    {
    }

    tressfx_quat::~tressfx_quat(void) {}

    tressfx_quat::tressfx_quat(const tressfx_quat& other)
    {
        x = other.x;
        y = other.y;
        z = other.z;
        w = other.w;
    }

    tressfx_quat::tressfx_quat(const tressfx_mat33& rotMat) { SetRotation(rotMat); }

    tressfx_quat::tressfx_quat(const tressfx_vec3& axis, float angle_radian)
    {
        SetRotation(axis, angle_radian);
    }

    tressfx_quat::tressfx_quat(float* xyz) { memcpy(m, xyz, sizeof(tressfx_quat)); }

    tressfx_quat& tressfx_quat::Normalize()
    {
        float n = w * w + x * x + y * y + z * z;

        if (n == 0)
        {
            w = 1;
            return (*this);
        }

        n = 1.0f / sqrt(n);

        w *= n;
        x *= n;
        y *= n;
        z *= n;

        return (*this);
    }

    void tressfx_quat::SetRotation(const tressfx_vec3& axis, float angle_radian)
    {
        // This function assumes that the axis vector has been normalized.
        float halfAng = 0.5f * angle_radian;
        float sinHalf = sin(halfAng);
        w             = cos(halfAng);

        x = sinHalf * axis.x;
        y = sinHalf * axis.y;
        z = sinHalf * axis.z;
    }

    void tressfx_quat::SetRotation(const tressfx_mat33& rotMat)
    {
        float fTrace = rotMat.m[0][0] + rotMat.m[1][1] + rotMat.m[2][2];
        float fRoot;

        if (fTrace > 0.0f)
        {
            // |w| > 1/2, may as well choose w > 1/2
            fRoot = sqrt(fTrace + 1.0f);  // 2w
            w     = 0.5f * fRoot;
            fRoot = 0.5f / fRoot;  // 1/(4w)
            x     = (rotMat.m[2][1] - rotMat.m[1][2]) * fRoot;
            y     = (rotMat.m[0][2] - rotMat.m[2][0]) * fRoot;
            z     = (rotMat.m[1][0] - rotMat.m[0][1]) * fRoot;
        }
        else
        {
            // |w| <= 1/2
            static size_t s_iNext[3] = { 1, 2, 0 };
            size_t        i          = 0;
            if (rotMat.m[1][1] > rotMat.m[0][0])
            {
                i = 1;
            }
            if (rotMat.m[2][2] > rotMat.m[i][i])
            {
                i = 2;
            }
            size_t j = s_iNext[i];
            size_t k = s_iNext[j];

            fRoot             = sqrt(rotMat.m[i][i] - rotMat.m[j][j] - rotMat.m[k][k] + 1.0f);
            float* apkQuat[3] = { &x, &y, &z };
            *apkQuat[i]       = 0.5f * fRoot;
            fRoot             = 0.5f / fRoot;
            w                 = (rotMat.m[k][j] - rotMat.m[j][k]) * fRoot;
            *apkQuat[j]       = (rotMat.m[j][i] + rotMat.m[i][j]) * fRoot;
            *apkQuat[k]       = (rotMat.m[k][i] + rotMat.m[i][k]) * fRoot;
        }
    }

    void tressfx_quat::SetRotation(const tressfx_quat& quaternion) { *this = quaternion; }

    void tressfx_quat::GetRotation(tressfx_vec3* pAxis, float* pAngle_radian) const
    {
        *pAngle_radian = 2.0f * acos(w);

        float scale = sqrt(x * x + y * y + z * z);

        if (scale > 0)
        {
            pAxis->x = x / scale;
            pAxis->y = y / scale;
            pAxis->z = z / scale;
        }
        else
        {
            pAxis->x = 0;
            pAxis->y = 0;
            pAxis->z = 0;
        }
    }

    void tressfx_quat::GetRotation(tressfx_mat33* pMat33) const
    {
        float nQ = x * x + y * y + z * z + w * w;
        float s  = 0.0;

        if (nQ > 0.0)
        {
            s = 2.0f / nQ;
        }

        float xs  = x * s;
        float ys  = y * s;
        float zs  = z * s;
        float wxs = w * xs;
        float wys = w * ys;
        float wzs = w * zs;
        float xxs = x * xs;
        float xys = x * ys;
        float xzs = x * zs;
        float yys = y * ys;
        float yzs = y * zs;
        float zzs = z * zs;

        pMat33->Set(1.0f - yys - zzs,
                    xys - wzs,
                    xzs + wys,
                    xys + wzs,
                    1.0f - xxs - zzs,
                    yzs - wxs,
                    xzs - wys,
                    yzs + wxs,
                    1.0f - xxs - yys);
    }

    tressfx_mat33 tressfx_quat::GetMatrix33() const
    {
        tressfx_mat33 mat;
        GetRotation(&mat);
        return mat;
    }

    float tressfx_quat::Length() const { return sqrt(x * x + y * y + z * z + w * w); }

    void tressfx_quat::SetIdentity()
    {
        x = y = z = 0.0;
        w         = 1.0;
    }

    void tressfx_quat::Inverse()
    {
        float lengthSqr = x * x + y * y + z * z + w * w;

        assert(lengthSqr != 0.0);

        x = -x / lengthSqr;
        y = -y / lengthSqr;
        z = -z / lengthSqr;
        w = w / lengthSqr;
    }

    tressfx_quat tressfx_quat::InverseOther() const
    {
        tressfx_quat q(*this);
        q.Inverse();
        return q;
    }

    tressfx_quat& tressfx_quat::operator=(const tressfx_quat& other)
    {
        w = other.w;
        x = other.x;
        y = other.y;
        z = other.z;

        return (*this);
    }

    tressfx_quat& tressfx_quat::operator=(float* xyz)
    {
        memcpy(m, xyz, sizeof(tressfx_quat));
        return *this;
    }

    tressfx_quat tressfx_quat::operator+(const tressfx_quat& other) const
    {
        tressfx_quat q;

        q.w = w + other.w;
        q.x = x + other.x;
        q.y = y + other.y;
        q.z = z + other.z;

        return q;
    }

    tressfx_quat tressfx_quat::operator+(const tressfx_vec3& vec) const
    {
        tressfx_quat q;

        q.w = w;
        q.x = x + vec.x;
        q.y = y + vec.y;
        q.z = z + vec.z;

        return q;
    }

    tressfx_quat tressfx_quat::operator*(const tressfx_quat& other) const
    {
        tressfx_quat q(*this);

        q.w = w * other.w - x * other.x - y * other.y - z * other.z;
        q.x = w * other.x + x * other.w + y * other.z - z * other.y;
        q.y = w * other.y + y * other.w + z * other.x - x * other.z;
        q.z = w * other.z + z * other.w + x * other.y - y * other.x;

        return q;
    }

    tressfx_vec3 tressfx_quat::operator*(const tressfx_vec3& vec) const
    {
        tressfx_vec3 uv, uuv;
        tressfx_vec3 qvec(x, y, z);
        uv  = qvec.Cross(vec);
        uuv = qvec.Cross(uv);
        uv *= (2.0f * w);
        uuv *= 2.0f;

        return vec + uv + uuv;
    }

    tressfx_vec3 operator*(const tressfx_vec3& vec, const tressfx_quat& q) { return q * vec; }

}  // namespace AMD
