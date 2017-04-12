//--------------------------------------------------------------------------------------
// File: Matrix33.cpp
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

#include <cassert>

#include "Math/Matrix33.h"
#include "Math/Vector3D.h"

namespace AMD
{
    const tressfx_mat33 tressfx_mat33::IDENTITY =
        tressfx_mat33(1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
    const tressfx_mat33 tressfx_mat33::ZERO =
        tressfx_mat33(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

    tressfx_mat33::tressfx_mat33(void)
    {
        for (int i = 0; i < 3; i++)
            for (int j  = 0; j < 3; j++)
                m[i][j] = 0;
    }

    tressfx_mat33::tressfx_mat33(const tressfx_mat33& other)
    {
        for (int i = 0; i < 3; i++)
            for (int j  = 0; j < 3; j++)
                m[i][j] = other.m[i][j];
    }

    tressfx_mat33::tressfx_mat33(float e00,
                                 float e01,
                                 float e02,
                                 float e10,
                                 float e11,
                                 float e12,
                                 float e20,
                                 float e21,
                                 float e22)
    {
        m[0][0] = e00;
        m[0][1] = e01;
        m[0][2] = e02;

        m[1][0] = e10;
        m[1][1] = e11;
        m[1][2] = e12;

        m[2][0] = e20;
        m[2][1] = e21;
        m[2][2] = e22;
    }

    tressfx_mat33::~tressfx_mat33(void) {}

    void tressfx_mat33::SetIdentity()
    {
        m[0][0] = 1.0;
        m[0][1] = 0.0;
        m[0][2] = 0.0;

        m[1][0] = 0.0;
        m[1][1] = 1.0;
        m[1][2] = 0.0;

        m[2][0] = 0.0;
        m[2][1] = 0.0;
        m[2][2] = 1.0;
    }

    float tressfx_mat33::GetElement(int i, int j) const
    {
        assert(0 <= i && i < 3);
        assert(0 <= i && j < 3);

        return m[i][j];
    }

    void tressfx_mat33::SetElement(int i, int j, float val)
    {
        assert(0 <= i && i < 3);
        assert(0 <= i && j < 3);

        m[i][j] = val;
    }

    void tressfx_mat33::Set(float e00,
                            float e01,
                            float e02,
                            float e10,
                            float e11,
                            float e12,
                            float e20,
                            float e21,
                            float e22)
    {
        m[0][0] = e00;
        m[0][1] = e01;
        m[0][2] = e02;

        m[1][0] = e10;
        m[1][1] = e11;
        m[1][2] = e12;

        m[2][0] = e20;
        m[2][1] = e21;
        m[2][2] = e22;
    }

    void tressfx_mat33::SetRotation(const tressfx_vec3& axis, float ang)
    {
#define vsin(x) ((1.0f) - cos(x))

        float nx = axis.x;
        float ny = axis.y;
        float nz = axis.z;

        m[0][0] = nx * nx * vsin(ang) + cos(ang);
        m[0][1] = nx * ny * vsin(ang) - nz * sin(ang);
        m[0][2] = nx * nz * vsin(ang) + ny * sin(ang);

        m[1][0] = nx * ny * vsin(ang) + nz * sin(ang);
        m[1][1] = ny * ny * vsin(ang) + cos(ang);
        m[1][2] = ny * nz * vsin(ang) - nx * sin(ang);

        m[2][0] = nx * nz * vsin(ang) - ny * sin(ang);
        m[2][1] = ny * nz * vsin(ang) + nx * sin(ang);
        m[2][2] = nz * nz * vsin(ang) + cos(ang);

#undef vsin
    }

    void tressfx_mat33::Inverse()
    {
        float det = m[0][0] * (m[2][2] * m[1][1] - m[2][1] * m[1][2]) -
                    m[1][0] * (m[2][2] * m[0][1] - m[2][1] * m[0][2]) +
                    m[2][0] * (m[1][2] * m[0][1] - m[1][1] * m[0][2]);

        m[0][0] = m[2][2] * m[1][1] - m[2][1] * m[1][2];
        m[0][1] = -m[2][2] * m[0][1] - m[2][1] * m[0][2];
        m[0][2] = m[1][2] * m[0][1] - m[1][1] * m[0][2];

        m[1][0] = -m[2][2] * m[1][0] - m[2][0] * m[1][2];
        m[1][1] = m[2][2] * m[0][0] - m[2][0] * m[0][2];
        m[1][2] = -m[1][2] * m[0][0] - m[1][0] * m[0][2];

        m[2][0] = m[2][1] * m[1][0] - m[2][0] * m[1][1];
        m[2][1] = -m[2][1] * m[0][0] - m[2][0] * m[0][1];
        m[2][2] = m[1][1] * m[0][0] - m[1][0] * m[0][1];

        (*this) *= 1 / det;
    }

    tressfx_mat33 tressfx_mat33::InverseOther() const
    {
        tressfx_mat33 other(*this);
        other.Inverse();
        return other;
    }

    void tressfx_mat33::Transpose() { *this = TransposeOther(); }

    tressfx_mat33 tressfx_mat33::TransposeOther() const
    {
        tressfx_mat33 other;

        for (int i = 0; i < 3; i++)
            for (int j        = 0; j < 3; j++)
                other.m[i][j] = m[j][i];

        return other;
    }

    tressfx_vec3 tressfx_mat33::operator*(const tressfx_vec3& vec) const
    {
        tressfx_vec3 ret;

        ret.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][2] * vec.z;
        ret.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][2] * vec.z;
        ret.z = m[2][0] * vec.x + m[2][1] * vec.y + m[2][2] * vec.z;

        return ret;
    }

    tressfx_mat33 tressfx_mat33::operator*(const tressfx_mat33& other) const
    {
        tressfx_mat33 ret;

        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
                ret.m[i][j] =
                    m[i][0] * other.m[0][j] + m[i][1] * other.m[1][j] + m[i][2] * other.m[2][j];

        return ret;
    }

    tressfx_mat33 tressfx_mat33::operator+(const tressfx_mat33& other) const
    {
        tressfx_mat33 ret;

        for (int i = 0; i < 3; i++)
            for (int j      = 0; j < 3; j++)
                ret.m[i][j] = m[i][j] + other.m[i][j];

        return ret;
    }

    tressfx_mat33 tressfx_mat33::operator-(const tressfx_mat33& other) const
    {
        tressfx_mat33 ret;

        for (int i = 0; i < 3; i++)
            for (int j      = 0; j < 3; j++)
                ret.m[i][j] = m[i][j] - other.m[i][j];

        return ret;
    }

    tressfx_mat33 tressfx_mat33::operator*(float val) const
    {
        tressfx_mat33 ret;

        for (int i = 0; i < 3; i++)
            for (int j      = 0; j < 3; j++)
                ret.m[i][j] = m[i][j] * val;

        return ret;
    }

    tressfx_mat33 tressfx_mat33::operator/(float val) const
    {
        tressfx_mat33 ret;

        // assert(val != 0);

        float eps = 1e-10f;

        if (0 <= val && val <= eps)
            val += eps;
        else if (-eps <= val && val <= 0)
            val -= eps;

        for (int i = 0; i < 3; i++)
            for (int j      = 0; j < 3; j++)
                ret.m[i][j] = m[i][j] / val;

        return ret;
    }

    tressfx_mat33& tressfx_mat33::operator*=(float val)
    {
        for (int i = 0; i < 3; i++)
            for (int j  = 0; j < 3; j++)
                m[i][j] = val * m[i][j];

        return (*this);
    }

    tressfx_mat33& tressfx_mat33::operator-=(const tressfx_mat33& other)
    {
        for (int i = 0; i < 3; i++)
            for (int j  = 0; j < 3; j++)
                m[i][j] = m[i][j] - other.m[i][j];

        return (*this);
    }

    tressfx_mat33& tressfx_mat33::operator+=(const tressfx_mat33& other)
    {
        for (int i = 0; i < 3; i++)
            for (int j  = 0; j < 3; j++)
                m[i][j] = m[i][j] + other.m[i][j];

        return (*this);
    }

    tressfx_mat33& tressfx_mat33::operator=(const tressfx_mat33& other)
    {
        for (int i = 0; i < 3; i++)
            for (int j  = 0; j < 3; j++)
                m[i][j] = other.m[i][j];

        return (*this);
    }

    // tressfx_mat33& tressfx_mat33::operator=(float a)
    //{
    //  for ( int i = 0; i < 3; i++ )
    //      for ( int j = 0; j < 3; j++ )
    //          m[i][j] = a;
    //
    //  return (*this);
    //}

    bool tressfx_mat33::operator==(const tressfx_mat33& other)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                if (m[i][j] != other.m[i][j])
                    return false;
            }

        return true;
    }

    bool tressfx_mat33::operator!=(const tressfx_mat33& other)
    {
        return !(*this).operator==(other);
    }

    bool tressfx_mat33::operator==(float a)
    {
        for (int i = 0; i < 3; i++)
            for (int j = 0; j < 3; j++)
            {
                if (m[i][j] != a)
                    return false;
            }

        return true;
    }

    bool tressfx_mat33::operator!=(float a) { return !(*this).operator==(a); }

    float& tressfx_mat33::operator()(int i, int j)
    {
        assert(0 <= i && i < 3);
        assert(0 <= i && j < 3);

        return m[i][j];
    }

    const float& tressfx_mat33::operator()(int i, int j) const
    {
        assert(0 <= i && i < 3);
        assert(0 <= i && j < 3);

        return m[i][j];
    }

    tressfx_mat33 operator*(float val, const tressfx_mat33& other) { return other * val; }

    tressfx_mat33 operator-(const tressfx_mat33& other)
    {
        tressfx_mat33 ret;

        for (int i = 0; i < 3; i++)
            for (int j      = 0; j < 3; j++)
                ret.m[i][j] = -other.m[i][j];

        return ret;
    }

}  // namespace AMD
