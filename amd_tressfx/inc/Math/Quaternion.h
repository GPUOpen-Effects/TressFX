//--------------------------------------------------------------------------------------
// File: Quaternion.h
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

#include "Matrix33.h"
#include "Vector3D.h"

#pragma warning(push)
#pragma warning(                                                                                  \
    disable : 4201)  // disable warning C4201: nonstandard extension used : nameless struct/union

namespace AMD
{
    class tressfx_quat
    {
    public:
        tressfx_quat(float x = 0.0, float y = 0.0, float z = 0.0, float w = 1.0);
        tressfx_quat(const tressfx_quat& other);
        tressfx_quat(const tressfx_mat33& rotMat);
        tressfx_quat(const tressfx_vec3& axis, float angle_radian);
        tressfx_quat(float* xyz);
        ~tressfx_quat(void);

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

        tressfx_quat& Normalize();

        void SetRotation(const tressfx_vec3& axis, float angle_radian);
        void SetRotation(const tressfx_mat33& rotMat);
        void SetRotation(const tressfx_quat& quaternion);
        void GetRotation(tressfx_vec3* pAxis, float* pAngle_radian) const;
        void GetRotation(tressfx_mat33* pMat33) const;
        tressfx_mat33 GetMatrix33() const;
        float         Length() const;

        void         SetIdentity();
        void         Inverse();
        tressfx_quat InverseOther() const;

        tressfx_quat& operator=(const tressfx_quat& other);
        tressfx_quat& operator=(float* xyz);
        tressfx_quat operator+(const tressfx_quat& other) const;
        tressfx_quat operator+(const tressfx_vec3& vec) const;
        tressfx_quat operator*(const tressfx_quat& other) const;
        tressfx_vec3 operator*(const tressfx_vec3& vec) const;

        friend tressfx_vec3 operator*(const tressfx_vec3& vec, const tressfx_quat& q);
    };

}  // namespace AMD

#pragma warning(pop)
