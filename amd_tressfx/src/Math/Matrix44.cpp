//--------------------------------------------------------------------------------------
// File: Matrix44.cpp
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

#include <cmath>
#include "Matrix44.h"
#include "Vector3D.h"

#define vsin(x) ((1.0f) - cos(x))

CMatrix44::CMatrix44()
{
    int i = 0;
    int j = 0;

    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < 4; j++ )
            Row[i][j] = 0.0f;
    }
}

CMatrix44::CMatrix44(const CMatrix44& other)
{
    int i = 0;
    int j = 0;

    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < 4; j++ )
            Row[i][j] = other.Row[i][j];
    }
}

CMatrix44::CMatrix44(float R1[4], float R2[4], float R3[4], float R4[4])
{
    int i;

    for ( i = 0; i < 4; i++ )
    {
        Row[0][i] = R1[i];
    }

    for ( i = 0; i < 4; i++ )
    {
        Row[1][i] = R2[i];
    }

    for ( i = 0; i < 4; i++ )
    {
        Row[2][i] = R3[i];
    }

    for ( i = 0; i < 4; i++ )
    {
        Row[3][i] = R4[i];
    }
}

CMatrix44::CMatrix44(float e00, float e01, float e02, float e03,
              float e10, float e11, float e12, float e13,
              float e20, float e21, float e22, float e23,
              float e30, float e31, float e32, float e33)
{
    Row[0][0] = e00; Row[0][1] = e01; Row[0][2] = e02; Row[0][3] = e03;
    Row[1][0] = e10; Row[1][1] = e11; Row[1][2] = e12; Row[1][3] = e13;
    Row[2][0] = e20; Row[2][1] = e21; Row[2][2] = e22; Row[2][3] = e23;
    Row[3][0] = e30; Row[3][1] = e31; Row[3][2] = e32; Row[3][3] = e33;
}

CMatrix44::~CMatrix44()
{
}

void CMatrix44::SetIdentity()
{
    Row[0][0] = 1.0f; Row[0][1] = 0.0f; Row[0][2] = 0.0f; Row[0][3] = 0.0f;
    Row[1][0] = 0.0f; Row[1][1] = 1.0f; Row[1][2] = 0.0f; Row[1][3] = 0.0f;
    Row[2][0] = 0.0f; Row[2][1] = 0.0f; Row[2][2] = 1.0f; Row[2][3] = 0.0f;
    Row[3][0] = 0.0f; Row[3][1] = 0.0f; Row[3][2] = 0.0f; Row[3][3] = 1.0f;
}

void CMatrix44::SetRotation(const CVector3D& axis, float ang)
{
    float nx = axis.m_X;
    float ny = axis.m_Y;
    float nz = axis.m_Z;

    Row[0][0] = nx*nx*vsin(ang) + cos(ang);
    Row[0][1] = nx*ny*vsin(ang) - nz*sin(ang);
    Row[0][2] = nx*nz*vsin(ang) + ny*sin(ang);

    Row[1][0] = nx*ny*vsin(ang) + nz*sin(ang);
    Row[1][1] = ny*ny*vsin(ang) + cos(ang);
    Row[1][2] = ny*nz*vsin(ang) - nx*sin(ang);

    Row[2][0] = nx*nz*vsin(ang) - ny*sin(ang);
    Row[2][1] = ny*nz*vsin(ang) + nx*sin(ang);
    Row[2][2] = nz*nz*vsin(ang) + cos(ang);

    Row[3][0] = 0.0f;
    Row[3][1] = 0.0f;
    Row[3][2] = 0.0f;
    Row[3][3] = 1.0f;
}

void CMatrix44::SetTranslate(float x, float y, float z)
{
    Row[0][3] = x;
    Row[1][3] = y;
    Row[2][3] = z;
}

float CMatrix44::GetElement(int nRow, int nCol) const
{
    return Row[nRow][nCol];
}

CVector3D CMatrix44::operator*(const CVector3D &vec) const
{
    CVector3D retVal;

    retVal.m_X = Row[0][0]*vec.m_X + Row[0][1]*vec.m_Y + Row[0][2]*vec.m_Z + Row[0][3];
    retVal.m_Y = Row[1][0]*vec.m_X + Row[1][1]*vec.m_Y + Row[1][2]*vec.m_Z + Row[1][3];
    retVal.m_Z = Row[2][0]*vec.m_X + Row[2][1]*vec.m_Y + Row[2][2]*vec.m_Z + Row[2][3];

    return retVal;
}

CMatrix44 CMatrix44::operator*(const CMatrix44& other) const
{
    CMatrix44 retMat;

    for ( int i = 0; i < 4; i++ )
    {
        for ( int j = 0; j < 4; j++ )
            retMat.Row[i][j] = Row[i][0] * other.Row[0][j] + Row[i][1] * other.Row[1][j] + Row[i][2] * other.Row[2][j] + Row[i][3] * other.Row[3][j];
    }

    return retMat;
}

CMatrix44& CMatrix44::operator=(const CMatrix44& other)
{
    int i = 0;
    int j = 0;

    for ( i = 0; i < 4; i++ )
    {
        for ( j = 0; j < 4; j++ )
            Row[i][j] = other.Row[i][j];
    }

    return *this;
}
