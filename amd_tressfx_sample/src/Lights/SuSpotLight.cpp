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
#include "SuSpotLight.h"

//=================================================================================================================================
//
//          Constructor(s) / Destructor(s) Block
//
//=================================================================================================================================

//=================================================================================================================================
/// Constructor
//=================================================================================================================================
SuSpotLight::SuSpotLight(const SuString& rName)
    : SuLight(SuLight::SPOT_LIGHT, rName)
    , m_fDropOff(0.0f)
    , m_fConeAngle(90.0f)
    , m_fHotSpotAngle(45.0f)
    , m_camera()
{
    // init camera frustum
    SetConeAngle(m_fConeAngle);
}

//=================================================================================================================================
/// Destructor
//=================================================================================================================================
SuSpotLight::~SuSpotLight() {}

//=================================================================================================================================
//
//          Public methods block
//
//=================================================================================================================================

//=================================================================================================================================
/// Sets the direction the spot light faces
///
/// \param rDirection The new direction
//=================================================================================================================================
void SuSpotLight::SetDirection(const SuVector3& rDirection)
{
    SuVector3 dir(rDirection);
    dir.Normalize();
    SuVector3 up(0.0f, 1.0f, 0.0f);

    if ((1.0f - SuAbs(dir.Dot(up))) < SU_EPSILON)
    {
        // vectors are co-linear, choose a different up vector
        up = SuVector3(1.0f, 0.0f, 0.0f);
    }

    // orthogonalize up vector
    up = up - dir.Dot(up) * dir;
    up.Normalize();

    SetOrientation(dir, up, dir.Cross(up));
}

//=================================================================================================================================
/// Sets the spot light's (outer) cone angle
///
/// \param fAngle Angle in degrees
//=================================================================================================================================
void SuSpotLight::SetConeAngle(float32 fAngle)
{
    m_fConeAngle = fAngle;
    m_camera.SetFrustum(
        fAngle, 1.0, m_camera.GetNearPlaneDistance(), m_camera.GetFarPlaneDistance());
}

//=================================================================================================================================
/// The near plane distance is only important if the spot light is being used for projective
/// texturing.
///
/// \param fDist The new near plane distance
//=================================================================================================================================
void SuSpotLight::SetNearPlaneDistance(float32 fDist)
{
    m_camera.SetFrustum(m_fConeAngle, 1.0f, fDist, m_camera.GetFarPlaneDistance());
}

//=================================================================================================================================
/// The far plane distance is only important if the spot light is being used for projective
/// texturing.
///
/// \param fDist The new far plane distance
//=================================================================================================================================
void SuSpotLight::SetFarPlaneDistance(float32 fDist)
{
    m_camera.SetFrustum(m_fConeAngle, 1.0f, m_camera.GetNearPlaneDistance(), fDist);
}

//=================================================================================================================================
/// Tests whether the spot light casts any light on a bounding box
///
/// \param rBB The bounding box to test
///
/// \return true if the bounding box is hit by the light, false otherwise
//=================================================================================================================================
bool SuSpotLight::IlluminatesBoundingBox(const SuBoundingBox& rBB) const
{
    float32 fIllumination =
        GetBrightness() * ComputeDistanceAttenuation((GetPosition() - rBB.GetCenter()).Length());

    // Note: For a huge bounding volume, this test could fail. The center could be far away from
    // the light, but the
    // edge of the BV could be really close.

    if (!IsVisible() || fIllumination <= 0.001f)
    {
        return false;
    }

    // Check against view frustum. Note that this isn't ideal that that we should check against a
    // cone instead of a
    // rectangular frustum.
    return GetFrustum().CullAxisAlignedBox(rBB.GetMinCorner(), rBB.GetMaxCorner()) !=
           SuFrustum::OUTSIDE;
}
