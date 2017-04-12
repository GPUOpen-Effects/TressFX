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

#ifndef _SU_RTSCAMERACONTROLLER_H_
#define _SU_RTSCAMERACONTROLLER_H_

#include "SuCameraController.h"
#include "SuInputListener.h"
#include "SuPoint3.h"
#include "SuVector2.h"

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A camera controller which implements simple RTS-like camera movement
/// 
//=================================================================================================================================
// tolua_begin
class SuRTSCameraController : public SuCameraController, public SuInputListener
{
public:

   SU_DLL SuRTSCameraController( );

   virtual SU_DLL ~SuRTSCameraController();

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

   /// Updates the camera position
   virtual SU_DLL void Update( SuTime t );

   /// Sets the translation speed for the camera
   inline void SetTranslationSpeed( float fSpeed ) { m_fTranslationSpeed = fSpeed; };

   /// Sets the rotation speed for the camera, in degrees
   inline void SetRotationSpeed( float fSpeed ) { m_fRotationSpeed = SuDegToRad(fSpeed); };

   /// Sets the orientation of the camera in the scene
   SU_DLL void SetViewParameters( float fViewingAngle, float fAltitude, float fFocusHeight );

   /// Restricts camera movement to a particular bounding square in the focus plane
   SU_DLL void RestrictMovement( float fXMin, float fXMax, float fZMin, float fZMax );

   /// Jumps the camera so that it is focused on the specified point in the focus plane
   SU_DLL void JumpToPosition( const SuVector2& rPosition );

   /// Sets the distance, in pixels, from the edge of the screen, at which to start scrolling the camera
   inline void SetScrollThreshold( uint16 nThresh ) { m_nScrollRegionSize = static_cast<int32>( nThresh ); };

// tolua_end
protected:

   /// A set of flags indicating what the user would like the camera to be doing
   struct Input
   {
      bool bLeft;    
      bool bRight;
      bool bUp;
      bool bDown;
      bool bSpinLeft;
      bool bSpinRight;
   };

   /// Obtains user movement commands from the keyboard and mouse
   SU_DLL void GetUserInput( Input& rInput );

   /// Clears tracked keyboard state
   SU_DLL void ResetKeyState();


private:


   float m_fTranslationSpeed; ///< Translation speed (WS units/sec)
   float m_fRotationSpeed;    ///< Rotation speed (radians/sec)

   float m_fRadius;           ///< Radius of cylinder on which the camera lives
   float m_fAltitude;         ///< Height of camera above the target position

   SuPoint3 m_vLookAt;        ///< Position being looked at
   float m_fPhi;              ///< Angle about the vertical axis (azimuth), relative to world-space positive Z axis
   float m_fFocusHeight;      ///< World-space Y coordinate of the camera's focus plane

   bool m_bFirstFrame;
   SuTime m_fLastFrameTime;

   bool m_bTranslateLeft;     ///< Flag which is set if a translate-left key is pressed
   bool m_bTranslateRight;    ///< Flag which is set if a translate-right key is pressed
   bool m_bTranslateUp;       ///< Flag which is set if a translate-up key is pressed
   bool m_bTranslateDown;     ///< Flag which is set if a translate-down key is pressed
   bool m_bSpinLeft;
   bool m_bSpinRight;

   int32 m_nScrollRegionSize; ///< Distance, in pixels, from window's edge at which to start scrolling the camera

   bool m_bRestrictMovement;  ///< Flag indicating whether movement restrictions are in place
   SuVector2 m_vRestrictMin;  ///< Bounding square in the focus plane to restrict the focus point to
   SuVector2 m_vRestrictMax;  ///< Bounding square in the focus plane to restrict the focus point to

// tolua_begin
};
// tolua_end

#endif // _SU_RTSCAMERACONTROLLER_H_
