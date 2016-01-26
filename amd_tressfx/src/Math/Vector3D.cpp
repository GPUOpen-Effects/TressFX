//--------------------------------------------------------------------------------------
// File: Vector3D.cpp
//
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
//--------------------------------------------------------------------------------------

#include <assert.h>
#include "Vector3D.h"

CVector3D::CVector3D(const CVector3D& other)
{
    m_X = other.m_X;
    m_Y = other.m_Y;
    m_Z = other.m_Z;
}

CVector3D::CVector3D(const CVector3D& begin, const CVector3D& end)
{
    m_X = end.m_X - begin.m_X;
    m_Y = end.m_Y - begin.m_Y;
    m_Z = end.m_Z - begin.m_Z;
}

CVector3D& CVector3D::Normalize()
{
    float d = sqrt(m_X * m_X + m_Y * m_Y + m_Z * m_Z);

    if ( d == 0 )
        return *this;

    m_X = m_X / d;
    m_Y = m_Y / d;
    m_Z = m_Z / d;

    return *this;
}

CVector3D CVector3D::NormalizeOther() const
{
    CVector3D n(*this);
    return n.Normalize();
}

void CVector3D::RotateW(const CVector3D& axis, float ang)
{
    CMatrix44 mat44;
    mat44.SetIdentity();
    mat44.SetRotation(axis, ang);

    RotateW(mat44);
}

void CVector3D::RotateW(CMatrix44 mat44)
{
    *this = mat44 * (*this);
}

CVector3D CVector3D::operator-(const CVector3D& other) const
{
    return CVector3D(m_X - other.m_X, m_Y - other.m_Y, m_Z - other.m_Z);
}

CVector3D CVector3D::operator+(const CVector3D& other) const
{
    return CVector3D(m_X + other.m_X, m_Y + other.m_Y, m_Z + other.m_Z);
}

CVector3D CVector3D::operator/(float val) const
{
    if ( val != 0.0f )
        return CVector3D(m_X / val, m_Y / val, m_Z / val);
    else
        return CVector3D(0.0f, 0.0f, 0.0f);
}

CVector3D& CVector3D::operator=(const CVector3D& other)
{
    m_X = other.m_X;
    m_Y = other.m_Y;
    m_Z = other.m_Z;

    return *this;
}

CVector3D& CVector3D::operator=(float val)
{
    m_X = val;
    m_Y = val;
    m_Z = val;

    return *this;
}

bool CVector3D::operator<(float val) const
{
    return (LengthSqr() < val*val);
}

bool CVector3D::operator>(float val) const
{
    return (LengthSqr() > val*val);
}

bool CVector3D::operator!=(float val) const
{
    return (m_X != val || m_Y != val || m_Z != val );
}

bool CVector3D::operator==(float val) const
{
    return (m_X == val && m_Y == val && m_Z == val );
}

bool CVector3D::operator==(const CVector3D& other) const
{
    return (m_X == other.m_X && m_Y == other.m_Y && m_Z == other.m_Z );
}

bool CVector3D::operator!=(const CVector3D& other) const
{
    return (m_X != other.m_X || m_Y != other.m_Y || m_Z != other.m_Z );
}

CVector3D& CVector3D::operator-=(const CVector3D& other)
{
    m_X -= other.m_X;
    m_Y -= other.m_Y;
    m_Z -= other.m_Z;

    return *this;
}

CVector3D& CVector3D::operator+=(const CVector3D& other)
{
    m_X += other.m_X;
    m_Y += other.m_Y;
    m_Z += other.m_Z;

    return *this;
}

CVector3D& CVector3D::operator*=(float val)
{
    m_X *= val;
    m_Y *= val;
    m_Z *= val;

    return *this;
}

CVector3D CVector3D::operator*(float val) const
{
    CVector3D vec(*this);

    vec.m_X *= val;
    vec.m_Y *= val;
    vec.m_Z *= val;

    return vec;
}

float CVector3D::operator*(const CVector3D& other) const
{
    return Dot(other);
}

CVector3D CVector3D::operator/(const CVector3D& other) const
{
    CVector3D vec;

    float e = 1e-10f;

    float x = 0;
    float y = 0;
    float z = 0;

    if ( 0 <= other.m_X && other.m_X <= e )
        x = other.m_X + e;
    else if ( -e <= other.m_X && other.m_X <= 0 )
        x = other.m_X - e;
    else
        x = other.m_X;

    if ( 0 <= other.m_Y && other.m_Y <= e )
        y = other.m_Y + e;
    else if ( -e <= other.m_Y && other.m_Y <= 0 )
        y = other.m_Y - e;
    else
        y = other.m_Y;

    if ( 0 <= other.m_Z && other.m_Z <= e )
        z = other.m_Z + e;
    else if ( -e <= other.m_Z && other.m_Z <= 0 )
        z = other.m_Z - e;
    else
        z = other.m_Z;

    vec.m_X = m_X / x;
    vec.m_Y = m_Y / y;
    vec.m_Z = m_Z / z;

    return vec;
}

CVector3D operator*(float val, const CVector3D& other)
{
    return other * val;
}

CVector3D operator-(const CVector3D& other)
{
    CVector3D vec(other);

    vec.m_X = -vec.m_X;
    vec.m_Y = -vec.m_Y;
    vec.m_Z = -vec.m_Z;

    return vec;
}
