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

#ifndef _SU_SPOTLIGHT_H_
#define _SU_SPOTLIGHT_H_

#include "SuCamera.h"
#include "SuLight.h"

//=========================================================================================================
/// \ingroup Engine
/// \brief Spot light
///
//=========================================================================================================
// tolua_begin
class SuSpotLight : public SuLight
{
public:
    /// Constructor
    SuSpotLight(const SuString& rName);

    /// Destructor
    virtual ~SuSpotLight();

    /// Returns the spotlight dropoff value
    float32 GetDropOff() const { return m_fDropOff; }

    /// Sets the spotlight dropoff value
    void SetDropOff(float32 fDropOff) { m_fDropOff = fDropOff; }

    /// Sets the light position
    void SetPosition(const SuPoint3& rPos) { m_camera.SetPosition(rPos); }

    /// Returns the light position
    const SuPoint3& GetPosition() const { return m_camera.GetPosition(); }

    /// Sets the direction the spot light points faces
    void SetDirection(const SuVector3& rDirection);

    /// Sets the spot light orientation
    inline void SetOrientation(const SuVector3& rDirection,
                               const SuVector3& rUp,
                               const SuVector3& rRight);

    /// Sets the spot light orientation using a lookat point
    inline void SetLookAt(const SuPoint3& rLookAtPt);

    /// Returns the direction the spot light is facing in
    const SuVector3& GetDirection() const { return m_camera.GetDirection(); }

    /// Returns the spot light up direction
    const SuVector3& GetUpVector() const { return m_camera.GetUpVector(); }

    /// returns the spot light right direction
    const SuVector3& GetRightVector() const { return m_camera.GetRightVector(); }

    /// Sets the spot light's (outer) cone angle
    void SetConeAngle(float32 fAngle);

    /// Returns the spot light's (outer) cone angle
    float32 GetConeAngle() const { return m_fConeAngle; }

    /// Sets the spot light's hot spot (inner cone) angle
    ///
    /// \param fAngle Angle in degrees
    void SetHotSpotAngle(float32 fAngle) { m_fHotSpotAngle = fAngle; }

    /// Returns the spot light's hot spot (inner cone) angle
    float32 GetHotSpotAngle() const { return m_fHotSpotAngle; }

    /// Sets the spot light's near plane distance.
    void SetNearPlaneDistance(float32 fDist);

    /// Returns the spot light's near plane distance
    float32 GetNearPlaneDistance() const { return m_camera.GetNearPlaneDistance(); }

    /// Sets the spot light's far plane distance.
    void SetFarPlaneDistance(float32 fDist);

    /// Returns the spot light's far plane distance
    float32 GetFarPlaneDistance() const { return m_camera.GetFarPlaneDistance(); }

    /// Returns the spot light's view matrix
    ///
    /// The view matrix transforms world space to view space
    /// if something is multiplied from the right.
    const SuMatrix4& GetViewMatrix() const { return m_camera.GetViewMatrix(); }

    /// Returns the spot light's projection matrix
    ///
    /// The projection matrix turns view space vectors to NDC (normalized device coordinate)
    /// space vectors if they are multiplied from the right.
    const SuMatrix4& GetProjectionMatrix() const { return m_camera.GetProjectionMatrix(); }

    /// Returns the spot light's view-projection matrix
    const SuMatrix4 GetViewProjectionMatrix() const { return m_camera.GetViewProjectionMatrix(); }

    /// Returns the spot light's frustum
    const SuFrustum& GetFrustum() const { return m_camera.GetFrustum(); };

    /// Tests whether the spot light casts any light on a bounding box
    virtual bool IlluminatesBoundingBox(const SuBoundingBox& rBV) const;

    // tolua_end

protected:
    friend class SuSpotLightCameraController;

    /// Returns the spot light's internal camera representation
    const SuCamera& GetSpotLightCamera() const { return m_camera; }

private:
    float32  m_fDropOff;
    float32  m_fConeAngle;
    float32  m_fHotSpotAngle;
    SuCamera m_camera;  ///< A camera that represents the spot light shape
    // tolua_begin
};
// tolua_end

#include "SuSpotLight.inl"

#endif  // _SU_SPOTLIGHT_H_
