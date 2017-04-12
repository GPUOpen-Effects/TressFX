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

#include "PCH.h"
#include "SuAlgorithm.h"
#include "SuLight.h"
#include "SuLightController.h"

const SuVector4 SuLight::COLOR_TO_GRAY_WEIGHTS(0.3f, 0.59f, 0.11f, 0.0f);

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Computes the distance-based light attenuation
///
/// \param fDist The distance to the light source
//=================================================================================================================================
float32 SuLight::ComputeDistanceAttenuation(float32 fDist) const
{
    switch (GetFalloff())
    {
    case SuLight::FALLOFF_NONE:
        return 1.0f;

    case SuLight::FALLOFF_LINEAR:
        return 1.0f / fDist;

    case SuLight::FALLOFF_QUADRATIC:
        return 1.0f / (fDist * fDist);

    default:
        SU_ASSERT(!"Unsupported light falloff type");
    }

    return 0.0f;
}

//=================================================================================================================================
/// Does nothing if the current controller is NULL
///
/// \param fTime The time for which to update the light
//=================================================================================================================================
void SuLight::Update(SuTime fTime)
{
    if (m_pController == NULL)
    {
        return;
    }

    m_pController->Update(fTime, this);
}

//=========================================================================================================
/// Assigns a property
/// \param rStrProperty Property to be added
//=========================================================================================================
void SuLight::AddProperty(const SuString& rStrProperty)
{
    if (!HasProperty(rStrProperty))
    {
        m_properties.push_back(rStrProperty);
    }
}

//=========================================================================================================
/// Test if we have a particular property
/// \param rStrProperty Property to be tested
//=========================================================================================================
bool SuLight::HasProperty(const SuString& rStrProperty) const
{
    return SuFind(m_properties.begin(), m_properties.end(), rStrProperty) != m_properties.end();
}

//=================================================================================================================================
//
//          Protected methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Returns the light's brightness (grayscale color * intensity)
//=================================================================================================================================
float32 SuLight::GetBrightness() const
{
    return GetIntensity() * GetColor().Dot(COLOR_TO_GRAY_WEIGHTS);
}
