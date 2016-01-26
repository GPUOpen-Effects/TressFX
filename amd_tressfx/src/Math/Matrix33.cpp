//--------------------------------------------------------------------------------------
// File: Matrix33.cpp
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

#include <cassert>

#include "Matrix33.h"
#include "Vector3D.h"

const CMatrix33 CMatrix33::IDENTITY = CMatrix33(1.0, 0.0, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0);
const CMatrix33 CMatrix33::ZERO = CMatrix33(0.0);

#define vsin(x) ((1.0f) - cos(x))

CMatrix33::CMatrix33(void)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = 0;
}

CMatrix33::CMatrix33(const CMatrix33& other)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = other.e[i][j];
}

CMatrix33::CMatrix33(float e00, float e01, float e02, float e10, float e11, float e12, float e20, float e21, float e22)
{
    e[0][0] = e00;
    e[0][1] = e01;
    e[0][2] = e02;

    e[1][0] = e10;
    e[1][1] = e11;
    e[1][2] = e12;

    e[2][0] = e20;
    e[2][1] = e21;
    e[2][2] = e22;
}

CMatrix33::CMatrix33(float a)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = a;
}

CMatrix33::~CMatrix33(void)
{
}

void CMatrix33::SetIdentity()
{
    e[0][0] = 1.0;
    e[0][1] = 0.0;
    e[0][2] = 0.0;

    e[1][0] = 0.0;
    e[1][1] = 1.0;
    e[1][2] = 0.0;

    e[2][0] = 0.0;
    e[2][1] = 0.0;
    e[2][2] = 1.0;
}

float CMatrix33::GetElement(int i, int j) const
{
    assert(0 <= i && i < 3);
    assert(0 <= i && j < 3);

    return e[i][j];
}

void CMatrix33::SetElement(int i, int j, float val)
{
    assert(0 <= i && i < 3);
    assert(0 <= i && j < 3);

    e[i][j] = val;
}

void CMatrix33::Set(float e00, float e01, float e02, float e10, float e11, float e12, float e20, float e21, float e22)
{
    e[0][0] = e00;
    e[0][1] = e01;
    e[0][2] = e02;

    e[1][0] = e10;
    e[1][1] = e11;
    e[1][2] = e12;

    e[2][0] = e20;
    e[2][1] = e21;
    e[2][2] = e22;
}

void CMatrix33::SetRotation(const CVector3D& axis, float ang)
{
    float nx = axis.m_X;
    float ny = axis.m_Y;
    float nz = axis.m_Z;

    e[0][0] = nx*nx*vsin(ang) + cos(ang);
    e[0][1] = nx*ny*vsin(ang) - nz*sin(ang);
    e[0][2] = nx*nz*vsin(ang) + ny*sin(ang);

    e[1][0] = nx*ny*vsin(ang) + nz*sin(ang);
    e[1][1] = ny*ny*vsin(ang) + cos(ang);
    e[1][2] = ny*nz*vsin(ang) - nx*sin(ang);

    e[2][0] = nx*nz*vsin(ang) - ny*sin(ang);
    e[2][1] = ny*nz*vsin(ang) + nx*sin(ang);
    e[2][2] = nz*nz*vsin(ang) + cos(ang);
}

void CMatrix33::Inverse()
{
    float det = e[0][0] * ( e[2][2] * e[1][1] - e[2][1] * e[1][2] ) -
                 e[1][0] * ( e[2][2] * e[0][1] - e[2][1] * e[0][2] ) +
                 e[2][0] * ( e[1][2] * e[0][1] - e[1][1] * e[0][2] );

    e[0][0] = e[2][2] * e[1][1] - e[2][1] * e[1][2];
    e[0][1] = -e[2][2] * e[0][1] - e[2][1] * e[0][2];
    e[0][2] = e[1][2] * e[0][1] - e[1][1] * e[0][2];

    e[1][0] = -e[2][2] * e[1][0] - e[2][0] * e[1][2];
    e[1][1] = e[2][2] * e[0][0] - e[2][0] * e[0][2];
    e[1][2] = -e[1][2] * e[0][0] - e[1][0] * e[0][2];

    e[2][0] = e[2][1] * e[1][0] - e[2][0] * e[1][1];
    e[2][1] = -e[2][1] * e[0][0] - e[2][0] * e[0][1];
    e[2][2] = e[1][1] * e[0][0] - e[1][0] * e[0][1];

    (*this) *= 1/det;
}

CMatrix33 CMatrix33::InverseOther() const
{
    CMatrix33 other(*this);
    other.Inverse();
    return other;
}

void CMatrix33::Transpose()
{
    *this = TransposeOther();
}

CMatrix33 CMatrix33::TransposeOther() const
{
    CMatrix33 other;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            other.e[i][j] = e[j][i];

    return other;
}

CVector3D CMatrix33::operator*(const CVector3D& vec) const
{
    CVector3D ret;

    ret.m_X = e[0][0] * vec.m_X + e[0][1] * vec.m_Y + e[0][2] * vec.m_Z;
    ret.m_Y = e[1][0] * vec.m_X + e[1][1] * vec.m_Y + e[1][2] * vec.m_Z;
    ret.m_Z = e[2][0] * vec.m_X + e[2][1] * vec.m_Y + e[2][2] * vec.m_Z;

    return ret;
}

CMatrix33 CMatrix33::operator*(const CMatrix33& other) const
{
    CMatrix33 ret;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            ret.e[i][j] = e[i][0] * other.e[0][j] + e[i][1] * other.e[1][j] + e[i][2] * other.e[2][j];

    return ret;
}

CMatrix33 CMatrix33::operator+(const CMatrix33& other) const
{
    CMatrix33 ret;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            ret.e[i][j] = e[i][j] + other.e[i][j];

    return ret;
}

CMatrix33 CMatrix33::operator-(const CMatrix33& other) const
{
    CMatrix33 ret;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            ret.e[i][j] = e[i][j] - other.e[i][j];

    return ret;
}


CMatrix33 CMatrix33::operator*(float val) const
{
    CMatrix33 ret;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            ret.e[i][j] = e[i][j] * val;

    return ret;
}

CMatrix33 CMatrix33::operator/(float val) const
{
    CMatrix33 ret;

    //assert(val != 0);

    float eps = 1e-10f;

    if ( 0 <= val && val <= eps )
        val += eps;
    else if ( -eps <= val && val <= 0 )
        val -= eps;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            ret.e[i][j] = e[i][j] / val;

    return ret;
}

CMatrix33& CMatrix33::operator*=(float val)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = val * e[i][j];

    return (*this);
}

CMatrix33& CMatrix33::operator-=(const CMatrix33& other)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = e[i][j] - other.e[i][j];

    return (*this);
}

CMatrix33& CMatrix33::operator+=(const CMatrix33& other)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = e[i][j] + other.e[i][j];

    return (*this);
}

CMatrix33& CMatrix33::operator=(const CMatrix33& other)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            e[i][j] = other.e[i][j];

    return (*this);
}

//CMatrix33& CMatrix33::operator=(float a)
//{
//  for ( int i = 0; i < 3; i++ )
//      for ( int j = 0; j < 3; j++ )
//          e[i][j] = a;
//
//  return (*this);
//}

bool CMatrix33::operator==(const CMatrix33& other)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
        {
            if ( e[i][j] != other.e[i][j] )
                return false;
        }

    return true;
}

bool CMatrix33::operator!=(const CMatrix33& other)
{
    return !(*this).operator==(other);
}

bool CMatrix33::operator==(float a)
{
    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
        {
            if ( e[i][j] != a )
                return false;
        }

    return true;
}

bool CMatrix33::operator!=(float a)
{
    return !(*this).operator==(a);
}

float& CMatrix33::operator()(int i, int j)
{
    assert(0 <= i && i < 3);
    assert(0 <= i && j < 3);

    return e[i][j];
}

const float& CMatrix33::operator()(int i, int j) const
{
    assert(0 <= i && i < 3);
    assert(0 <= i && j < 3);

    return e[i][j];
}

CMatrix33 operator*(float val, const CMatrix33& other)
{
    return other * val;
}

CMatrix33 operator-(const CMatrix33& other)
{
    CMatrix33 ret;

    for ( int i = 0; i < 3; i++ )
        for ( int j = 0; j < 3; j++ )
            ret.e[i][j] = -other.e[i][j] ;

    return ret;
}
