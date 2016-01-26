//--------------------------------------------------------------------------------------
// File: Matrix33.h
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

#pragma once

class CVector3D;

class CMatrix33
{
    friend class CVector3D;
    friend class CQuaternion;

public:
    CMatrix33(void);
    CMatrix33(const CMatrix33& other);
    CMatrix33(float e00, float e01, float e02, float e10, float e11, float e12, float e20, float e21, float e22);
    CMatrix33(float a);
    ~CMatrix33(void);

private:
    float e[3][3];

public:
    static const CMatrix33 IDENTITY;
    static const CMatrix33 ZERO;

public:
    void SetIdentity();
    void Set(float e00, float e01, float e02, float e10, float e11, float e12, float e20, float e21, float e22);
    float GetElement(int i, int j) const;
    void SetElement(int i, int j, float val);
    void SetRotation(const CVector3D& axis, float ang);
    void Inverse();
    CMatrix33 InverseOther() const;
    void Transpose();
    CMatrix33 TransposeOther() const;

    CVector3D operator*(const CVector3D& vec) const;
    CMatrix33 operator*(const CMatrix33& other) const;
    CMatrix33 operator*(float val) const;
    CMatrix33 operator+(const CMatrix33& other) const;
    CMatrix33 operator-(const CMatrix33& other) const;
    CMatrix33 operator/(float val) const;
    CMatrix33& operator*=(float val);
    CMatrix33& operator-=(const CMatrix33& other);
    CMatrix33& operator+=(const CMatrix33& other);
    CMatrix33& operator=(const CMatrix33& other);
    //CMatrix33& operator=(float a);

    bool operator==(const CMatrix33& other);
    bool operator!=(const CMatrix33& other);

    bool operator==(float a);
    bool operator!=(float a);

    float& operator()(int i, int j);
    const float& operator()(int i, int j) const;

    friend CMatrix33 operator*(float val, const CMatrix33& other);
    friend CMatrix33 operator-(const CMatrix33& other);
};
