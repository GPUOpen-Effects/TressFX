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
#include "SuBoundingBox.h"
#include "SuPointLight.h"

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuPointLight::SuPointLight(const SuString& rName)
    : SuLight(SuLight::POINT_LIGHT, rName), m_position(0.0f, 0.0f, 0.0f), m_fRadius(0.0f)
{
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuPointLight::~SuPointLight() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Tests whether the point light casts any light on a bounding box
///
/// \param rBB The bounding box to test
///
/// \return true if the bounding box is hit by the light, false otherwise
//=================================================================================================================================
bool SuPointLight::IlluminatesBoundingBox(const SuBoundingBox& rBB) const
{
    // TODO:  Check radius.

    float32 fIllumination =
        GetBrightness() * ComputeDistanceAttenuation((m_position - rBB.GetCenter()).Length());

    // Note: For a huge bounding volume, this test could fail. The center could be far away from
    // the light, but the
    // edge of the BV could be really close.
    return IsVisible() && (fIllumination >= 0.001f);
}
