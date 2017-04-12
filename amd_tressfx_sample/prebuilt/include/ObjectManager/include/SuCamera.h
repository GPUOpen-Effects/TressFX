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

#ifndef _SUCAMERA_H
#define _SUCAMERA_H

#include "SuTypes.h"
#include "SuVector3.h"
#include "SuPoint3.h"
#include "SuVector4.h"
#include "SuMatrix4.h"
#include "SuFrustum.h"
#include "SuRay.h"

class SuQuat;

//=================================================================================================================================
/// \brief Camera class
///
/// The engine's camera class. Works according to a right-handed coordinate system, which
/// means the camera points down the local negative Z axis, conforming to OpenGL conventions.
//=================================================================================================================================
// tolua_begin
class SuCamera
{
public:
   /// Constructor
   SU_DLL SuCamera();

   /// Destructor
   virtual SU_DLL ~SuCamera();

   /// Sets the camera position
   SU_DLL void SetPosition( const SuPoint3& rPos );

   /// Returns the camera position
   inline const SuPoint3& GetPosition() const { return m_pos; }

   /// Sets the camera orientation
   SU_DLL void SetOrientation( const SuVector3& rDirection, const SuVector3& rUp, const SuVector3& rRight );

   /// Sets the camera orientation from a quaternion
   SU_DLL void SetOrientation( const SuQuat& rQuat );

   /// Sets the camera orientation using a lookat point
   SU_DLL void SetLookAt( const SuPoint3& rLookAtPt, const SuVector3& rUpDir );

   /// Returns the direction the camera is facing in
   inline const SuVector3& GetDirection() const { return m_viewDir; }

   /// Returns the camera up direction
   inline const SuVector3& GetUpVector() const { return m_up; }

   /// returns the camera right direction
   inline const SuVector3& GetRightVector() const { return m_right; }

   /// Sets the camera frustum using an angle and aspect ratio
   SU_DLL void SetFrustum( float32 fHorizFOVAngle, float32 fAspectRatio, float32 fNear, float32 fFar );

   /// Sets the camera frustum
   SU_DLL void SetFrustum( float32 fLeft, float32 fRight, float32 fBottom, float32 fTop, float32 fNear, float32 fFar );

   /// Set the camera frustum to an orthogonal projection using width and aspect ratio
   SU_DLL void SetOrthoFrustum( float32 fWidth, float32 fAspectRatio, float32 fNear, float32 fFar );

   /// Sets the camera frustum to an orthogonal projection
   SU_DLL void SetOrthoFrustum( float32 fLeft, float32 fRight, float32 fBottom, float32 fTop, float32 fNear, float32 fFar );

   /// Returns the current frustum parameters
   SU_DLL void GetFrustum( float32* pLeft, float32* pRight, float32* pBottom, float32* pTop, float32* pNear, float32* pFar ) const;

   /// Returns the current frustum parameters in a vector SuVector4(left, right, bottom, top).  This is useful for calling from lua.
   SU_DLL SuVector4 GetFrustumParams( void ) const { return SuVector4(m_fLeft, m_fRight, m_fBottom, m_fTop); }

   /// Returns whether the camera is using an orthogonal frustum or not
   inline bool IsOrthogonalProjection() const { return m_bOrtho; }

   /// Returns the camera's aspect ratio
   inline float32 GetAspectRatio() const { return m_fAspectRatio; }

   /// Returns the camera's horizontal field of view angle in degrees.
   inline float32 GetFOV() const { return m_fFOV; }

   /// Returns the camera's near plane distance
   inline float32 GetNearPlaneDistance() const { return m_fNear; }

   /// Returns the camera's far plane distance
   inline float32 GetFarPlaneDistance() const { return m_fFar; }

   /// Returns the camera's view matrix
   ///
   /// The view matrix transforms world space to view space
   /// if something is multiplied from the right.
   inline const SuMatrix4& GetViewMatrix() const { return m_viewMat; }

   /// Returns the camera's projection matrix
   ///
   /// The projection matrix turns view space vectors to NDC (normalized device coordinate)
   /// space vectors if they are multiplied from the right.
   inline const SuMatrix4& GetProjectionMatrix() const { return m_projMat; }

   /// Returns the camera's view-projection matrix
   inline const SuMatrix4 GetViewProjectionMatrix() const { return m_viewProjMat; }

   /// Returns the camera's frustum
   inline const SuFrustum& GetFrustum() const { return m_frustum; };

   /// Turns this camera into a camera for rendering into the specified cube map face
   SU_DLL void SetCubemapCamera( uint32 nCubeFace, const SuPoint3& rPosition, float fNear, float fFar );

   /// Aligns the camera so that it is zoomed in on a sphere at the specified position, with the specified radius
   SU_DLL void ZoomOnSphere( const SuPoint3& rSphereCenter, float fRadius );

   /// \brief Returns a flag indicating whether or not DOF is enabled
   inline bool IsDOFEnabled() const { return m_bDOFEnabled; };

   /// \brief Sets the flag indicating whether or not DOF is enabled
   inline void SetDOFEnabled( bool bEnable ) { m_bDOFEnabled = bEnable; };

   /// \brief Returns the focus distance for the camera, useful for depth-of-field
   inline float32 GetFocusDistance() const { return m_fFocusDistance; };

   /// Sets the focus distance for the camera, useful for depth of field
   inline void SetFocusDistance( float32 fDistance ) { m_fFocusDistance = fDistance; };

   /// \brief Returns the F-stop setting for the camera, useful for depth-of-field
   /// Note that this need not be a physically accurate F number.  The precise meaning of this parameter depends on the particular DOF implementation being used
   inline float32 GetFStop() const { return m_fFStop; };

   /// \brief Sets the F-stop setting for the camera, useful for depth-of-field
   /// Note that this need not be a physically accurate F number.  The precise meaning of this parameter depends on the particular DOF implementation being used
   inline void SetFStop( float32 fStop ) { m_fFStop = fStop; };

   /// \brief Returns the focal length for the camera lens, useful for depth-of-field
   /// The default focal length is 
   /// Note that this need not be a physically accurate value.  The precise meaning of this parameter depends on the particular DOF implementation being used.
   inline float32 GetFocalLength() const { return m_fFocalLength; };

   /// \brief Sets the focal length for the camera lens, useful for depth-of-field
   /// Note that this need not be a physically accurate value.  The precise meaning of this parameter depends on the particular DOF implementation being used.
   inline void SetFocalLength( float32 fFocalLength ) { m_fFocalLength = fFocalLength; };

   /// Returns the camera's current visibility state
   bool IsVisible() const { return m_bVisible; }

   /// Sets the camera's visibility
   void SetVisible( bool bVisible ) { m_bVisible = bVisible; }

   /// Returns the entire list of properties
   inline const SuStringArray& GetProperties() const { return m_properties; } 

   /// Assigns a property
   SU_DLL void AddProperty( const SuString& rStrProperty );

   /// Test if we have particular property
   SU_DLL bool HasProperty( const SuString& rStrProperty ) const; 

   /// Returns number of properties. This is convenient for Lua access.
   uint32 GetNumProperties() const { return static_cast<uint32>(m_properties.size()); }

   /// Returns the ith property. This is convenient for Lua access.
   const SuString& GetProperty(uint32 i) const { return m_properties[i]; }

   /// Computes a picking ray passing through a specified point on the image plane
   SU_DLL SuRay GetPickRay( float s, float t ) const;

// tolua_end

private:
   /// Updates the world->view matrix from the vectors stored in the camera object
   void UpdateViewMatrix();

   /// Updates the projection matrix from the frustum parameters
   void UpdateProjectionMatrix();

   /// Updates the frustum from the view-proj matrix
   void UpdateFrustum();


   SuMatrix4 m_viewMat;          ///< Camera's current world->view matrix
   SuMatrix4 m_projMat;          ///< Camera's current view->projection matrix
   SuMatrix4 m_viewProjMat;      ///< Camera's current world->projection matrix

   SuPoint3  m_pos;              ///< Camera position
   SuVector3 m_viewDir;          ///< Direction camera is facing in
   SuVector3 m_up;               ///< Camera up vector
   SuVector3 m_right;            ///< Camera right vector
   float32   m_fAspectRatio;     ///< Camera aspect ratio
   float32   m_fFOV;             ///< Camera's horizontal field of view
   bool      m_bVisible;         ///< Camera visibility

   // depth of field parameters
   float32   m_fFStop;           ///< Camera F-stop
   float32   m_fFocusDistance;   ///< Camera focus distance 
   float32   m_fFocalLength;     ///< 
   bool      m_bDOFEnabled;      ///< DOF enable/disable flag

   // Frustum information
   float32   m_fNear;            ///< Camera's near plane distance
   float32   m_fFar;             ///< Camera's far plane distance
   float32   m_fLeft;
   float32   m_fRight;
   float32   m_fBottom;
   float32   m_fTop;
   bool      m_bOrtho;           ///< Flag indicating whether this is an orthogonal projection frustum

   SuFrustum m_frustum;          ///< View frustum object to use for culling
   SuStringArray m_properties;   ///< Properties array

// tolua_begin
};
// tolua_end

#endif
