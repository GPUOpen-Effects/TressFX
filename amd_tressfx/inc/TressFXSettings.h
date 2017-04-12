// ----------------------------------------------------------------------------
// Wrappers for setting values that end up in constant buffers.
// ----------------------------------------------------------------------------
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

#ifndef _TRESSFXSIMULATIONPARAMS_H_
#define _TRESSFXSIMULATIONPARAMS_H_

// Probably want to unify or rename these.

// tolua_begin
class TressFXSimulationSettings
{
public:
    TressFXSimulationSettings()
        : m_vspCoeff(0.8f)
        , m_vspAccelThreshold(1.4f)

        , m_localConstraintStiffness(0.9f)
        , m_localConstraintsIterations(2)

        , m_globalConstraintStiffness(0.0f)
        , m_globalConstraintsRange(0.0f)

        , m_lengthConstraintsIterations(2)

        , m_damping(0.08f)

        , m_gravityMagnitude(10.0)
        , m_tipSeparation(0.0f)

        , m_windMagnitude(0)
        , m_windAngleRadians(3.1415926f / 180.0f * 40.0f)
    {
        m_windDirection[0] = 1.0f;
        m_windDirection[1] = 0.0f;
        m_windDirection[2] = 0.0f;
    }

    // VSPf
    float m_vspCoeff;
    float m_vspAccelThreshold;

    // local constraint
    float m_localConstraintStiffness;
    int   m_localConstraintsIterations;

    // global constraint
    float m_globalConstraintStiffness;
    float m_globalConstraintsRange;

    // length constraint
    int m_lengthConstraintsIterations;

    // damping
    float m_damping;

    // gravity
    float m_gravityMagnitude;

    // tip separation for follow hair from its guide
    float m_tipSeparation;

    // wind
    float m_windMagnitude;
    float m_windDirection[3];
    float m_windAngleRadians;
};
// tolua_end

#endif
