//--------------------------------------------------------------------------------------
// File: Transform.h
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

#include "Quaternion.h"
#include "Vector3D.h"

namespace AMD
{
    class tressfx_transform
    {
    public:
        tressfx_transform(void);
        tressfx_transform(const tressfx_transform& other);
        tressfx_transform(const tressfx_vec3& translation, const tressfx_quat& rotation);
        ~tressfx_transform(void);

    private:
        tressfx_vec3 m_Translation;
        tressfx_quat m_Rotation;

    public:
        const tressfx_vec3& GetTranslation() const { return m_Translation; }
        const tressfx_quat& GetRotation() const { return m_Rotation; }
        tressfx_vec3&       GetTranslation() { return m_Translation; }
        tressfx_quat&       GetRotation() { return m_Rotation; }
        void                Inverse();
        tressfx_transform   InverseOther() const;

        tressfx_vec3 operator*(const tressfx_vec3& vector) const;
        tressfx_transform operator*(const tressfx_transform& transform) const;
        tressfx_transform& operator=(const tressfx_transform& other);
    };

}  // namespace AMD
