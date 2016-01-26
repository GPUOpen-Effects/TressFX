//--------------------------------------------------------------------------------------
// File: Transform.h
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

#include "Vector3D.h"
#include "Quaternion.h"

class CTransform
{
public:
    CTransform(void);
    CTransform(const CTransform& other);
    CTransform(const CVector3D& translation, const CQuaternion& rotation);
    ~CTransform(void);

private:
    CVector3D m_Translation;
    CQuaternion m_Rotation;

public:
    const CVector3D& GetTranslation() const { return m_Translation; }
    const CQuaternion& GetRotation() const { return m_Rotation; }
    CVector3D& GetTranslation() { return m_Translation; }
    CQuaternion& GetRotation() { return m_Rotation; }
    void Inverse();
    CTransform InverseOther() const;

    CVector3D operator*(const CVector3D& vector) const;
    CTransform operator*(const CTransform& transform) const;
    CTransform& operator=(const CTransform& other);
};

