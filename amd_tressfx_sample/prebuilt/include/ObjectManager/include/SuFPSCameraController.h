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

#ifndef _SU_FPSCAMERACONTROLLER_H_
#define _SU_FPSCAMERACONTROLLER_H_


//=========================================================================================================
/// \brief Camera controller for first-person-shooter user control
//=========================================================================================================

#include "SuTypes.h"
#include "SuInputListener.h"
#include "SuVector2.h"
#include "SuVector3.h"
#include "SuMatrix4.h"
#include "SuCameraController.h"

class SuCamera;

// tolua_begin
class SuFPSCameraController : public SuCameraController, public SuInputListener
{
public:
   SU_DLL SuFPSCameraController();

   virtual SU_DLL ~SuFPSCameraController();

   /// The controller is about to get input events
   virtual SU_DLL void Focused();

   /// The controller lost input event focus
   virtual SU_DLL void FocusLost();

   /// Callback handler for key press events
   virtual SU_DLL void KeyPressed( SuKeyEvent& rKE );

   /// Callback handler for key release events
   virtual SU_DLL void KeyReleased( SuKeyEvent& rKE );
   
   /// Callback handler for mouse move events
   virtual SU_DLL void MouseMoved( SuMouseMoveEvent& rME );

   virtual SU_DLL void Update( SuTime t );

   /// Stops all camera motion by setting velocities to zero
   virtual SU_DLL void FreezeCamera();

   /// Sets FPS camera translation (walk) speed to the given value 
   inline void SetTranslationSpeed( float32 fTranslationSpeed ) {  m_fTranslationSpeed = fTranslationSpeed; }

   /// Sets FPS camera rotation (look-at) speed to the given value
   inline void SetRotationSpeed( float32 fRotationSpeed ) {  m_fRotationSpeed = fRotationSpeed; }

   /// Set FPS camera mouse translation (walk) acceleration
   inline void SetTranslationAccel( float32 fTranslationAccel ) { m_fTranslationAccel = fTranslationAccel; }

   /// Set FPS camera mouse rotation (look-at) acceleration
   inline void SetRotationAccel( float32 fRotationAccel ) { m_fRotationAccel = fRotationAccel; }

   /// Set translational drag
   inline void SetTranslationDrag ( float32 fDrag ) { m_fTranslationDrag = fDrag; }

   /// Set rotational drag
   inline void SetRotationDrag ( float32 fDrag ) { m_fRotationDrag = fDrag; }

   /// Set translation ramp up time
   inline void SetTranslationRampUpTime ( float32 fTime ) { m_fTranslationRampUpTime = fTime; }

   /// Set translation ramp down time
   inline void SetTranslationRampDownTime ( float32 fTime ) { m_fTranslationRampDownTime = fTime; }

// tolua_end

protected:

   /// Returns the translational acceleration from the input
   virtual SU_DLL SuVector3 GetTranslationAccelerationFromInput() const;

   /// Returns the rotational acceleration from the input
   virtual SU_DLL SuVector2 GetRotationAccelerationFromInput() const;

   virtual SU_DLL void UpdateOrientation( float32 fDt );

   virtual SU_DLL void UpdatePosition( float32 fDt );

   /// Changes the camera's FOV by fDelta degrees
   void SU_DLL ChangeFOV ( float32 fDelta );

   // input controller state
   SuVector2 m_mouseDelta;
   bool m_bWalkForwardKeyDown;
   bool m_bWalkBackwardKeyDown;
   bool m_bStrafeLeftKeyDown;
   bool m_bStrafeRightKeyDown;
   bool m_bMoveUpKeyDown;
   bool m_bMoveDownKeyDown;
   bool m_bControlPushed;
   bool m_bLeftMouseButtonDown;
   bool m_bRightMouseButtonDown;

   bool m_bFirstTime;
   SuTime m_fPrevTime;     ///< Time during previous call

   SuTime m_fAvgFrameTime; ///< A running average of previous frame times

   SuVector3 m_vTransVel;   ///< Camera's translational velocity in world space
   SuVector2 m_vRotVel;     ///< Camera's rotational velocity

   float32 m_fTranslationSpeed;  ///< Controls camera translation speed
   float32 m_fRotationSpeed;     ///< Controls camera rotation speed

   bool m_bHasFocus;

   float32 m_fInitialTranslationSpeed;  ///< A setting for camera translation speed
   float32 m_fInitialRotationSpeed;     ///< A setting for camera rotation speed

   float32 m_fTranslationAccel;    ///< Input translation acceleration parameter
   float32 m_fRotationAccel;       ///< Input rotation acceleration parameter

   float32 m_fTranslationDrag;
   float32 m_fRotationDrag;
   float32 m_fTranslationRampUpTime;
   float32 m_fTranslationRampDownTime;

   static const float32 DEFAULT_TRANSLATION_ACCEL;
   static const float32 DEFAULT_ROTATION_ACCEL;
   static const float32 DEFAULT_TRANSLATION_DRAG;
   static const float32 DEFAULT_ROTATION_DRAG;
   static const float32 DEFAULT_TRANSLATION_RAMP_UP_TIME;
   static const float32 DEFAULT_TRANSLATION_RAMP_DOWN_TIME;

// tolua_begin
};
// tolua_end

#endif // _SU_FPSCAMERACONTROLLER_H
