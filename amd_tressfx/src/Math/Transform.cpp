//--------------------------------------------------------------------------------------
// File: Transform.cpp
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

#include "Math/Transform.h"

namespace AMD
{
    tressfx_transform::tressfx_transform(void) {}

    tressfx_transform::tressfx_transform(const tressfx_transform& other)
    {
        m_Translation = other.m_Translation;
        m_Rotation    = other.m_Rotation;
    }

    tressfx_transform::tressfx_transform(const tressfx_vec3& translation,
                                         const tressfx_quat& rotation)
    {
        m_Translation = translation;
        m_Rotation    = rotation;
    }

    tressfx_transform::~tressfx_transform(void) {}

    void tressfx_transform::Inverse()
    {
        m_Rotation.Inverse();
        m_Translation = m_Rotation * (-m_Translation);
    }

    tressfx_transform tressfx_transform::InverseOther() const
    {
        tressfx_transform other(*this);
        other.Inverse();
        return other;
    }

    tressfx_vec3 tressfx_transform::operator*(const tressfx_vec3& vector) const
    {
        return m_Rotation * vector + m_Translation;
    }

    tressfx_transform tressfx_transform::operator*(const tressfx_transform& transform) const
    {
        return tressfx_transform(m_Rotation * transform.GetTranslation() + m_Translation,
                                 m_Rotation * transform.GetRotation());
    }

    tressfx_transform& tressfx_transform::operator=(const tressfx_transform& other)
    {
        if (this == &other)
        {
            return (*this);
        }

        m_Translation = other.m_Translation;
        m_Rotation    = other.m_Rotation;

        return (*this);
    }

}  // namespace AMD
