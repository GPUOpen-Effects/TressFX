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

#ifndef _SU_FLYPATHCAMERACONTROLLER_H_
#define _SU_FLYPATHCAMERACONTROLLER_H_

#include "SuCameraController.h"
#include "SuCountedPtr.h"

class SuAnimationCurve;
typedef SuCountedPtr<SuAnimationCurve> SuAnimationCurvePtr;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A camera controller which uses artist-defined animation curves to control the camera
/// 
//=================================================================================================================================
// tolua_begin
class SuFlypathCameraController : public SuCameraController
{
public:
   /// Constructor 
   SU_DLL SuFlypathCameraController( SuAnimationCurvePtr pPositionCurve, 
                              SuAnimationCurvePtr pOrientationCurve, 
                              SuAnimationCurvePtr pFOVCurve,
                              SuAnimationCurvePtr pNearPlaneCurve,
                              SuAnimationCurvePtr pFarPlaneCurve,
                              SuAnimationCurvePtr pDOFEnableCurve,
                              SuAnimationCurvePtr pFocalLengthCurve,
                              SuAnimationCurvePtr pFocusDistanceCurve,
                              SuAnimationCurvePtr pFStopCurve,
                              SuAnimationCurvePtr pVisibilityCurve,
                              bool bLoop = true );

   /// Destructor
   SU_DLL ~SuFlypathCameraController();

   /// Updates the camera
   virtual SU_DLL void Update( SuTime fTime );

   /// Scales the speed of flight
   SU_DLL void SetAnimationSpeed( float fSpeedScaler );

   /// Determines whether or not a camera cut occurs between two time points
   SU_DLL bool DoesCameraCut( float fFirstTime, float fSecondTime );

   /// Accessor for flypath position curve
   inline SuAnimationCurvePtr GetPositionCurve() const { return m_pPositionCurve; }

   /// Accessor for flypath orientation curve
   inline SuAnimationCurvePtr GetOrientationCurve() const { return m_pOrientationCurve; }

   /// \brief Returns the length of the flypath animation
   /// \return The length of the flypath, or -1 if all of the flypath animation curves are NULL
   inline float32 GetFlypathLength() const { return m_fLength; };

// tolua_end

private:

   SuAnimationCurvePtr m_pPositionCurve;     ///< Animation curve for the camera position
   SuAnimationCurvePtr m_pOrientationCurve;  ///< Animation curve for the camera orientation
   SuAnimationCurvePtr m_pFOVCurve;          ///< Animation curve for the camera field of view
   SuAnimationCurvePtr m_pNearPlaneCurve;    ///< Animation curve for the camera near plane
   SuAnimationCurvePtr m_pFarPlaneCurve;     ///< Animation curve for the camera far plane
   
   SuAnimationCurvePtr m_pDOFEnableCurve;       ///< Animation curve for DOF enable/disable
   SuAnimationCurvePtr m_pFocalLengthCurve;     ///< Animation curve for focal length
   SuAnimationCurvePtr m_pFocusDistanceCurve;   ///< Animation curve for focus distance
   SuAnimationCurvePtr m_pFStopCurve;           ///< Animation curve for F-Stop

   SuAnimationCurvePtr m_pVisibilityCurve;   ///< Animation curve for camera visibility

   /// Multiplier to scale the animation speed
   float32 m_fSpeedScaler;

   /// Length of the animation
   float32 m_fLength;

   /// Should the animation loop?
   bool m_bLoop;

// tolua_begin
};
// tolua_end


#endif // _SU_FLYPATHCAMERACONTROLLER_H_
