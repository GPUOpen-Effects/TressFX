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

#ifndef _SU_BONEANIMATOR_H_
#define _SU_BONEANIMATOR_H_

#include "SuCountedPtr.h"

class SuAnimationCurve;
class SuBone;
class SuAnimationSet;
class SuMatrix4;
class SuVector3;
class SuQuat;

typedef SuCountedPtr<SuAnimationCurve> SuAnimationCurvePtr;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief An object which samples animation curves for a bone
/// 
//=================================================================================================================================
class SuBoneAnimator
{
public:

   /// Constructs a bone animator by pulling curves from the specified animation set
   static SU_DLL SuBoneAnimator* Create( const SuBone* pBone, const SuAnimationSet* pAnimationSet );

   /// Samples the bone animation
   SU_DLL void GetTransform( float32 fTime, SuMatrix4* pMatrixOut ) const;

   /// Extrapolates a bone transformation for an earlier time, based on the current bone and the properties of the bone curves
   SU_DLL void ExtrapolateTransform( float32 fCurrentTime, float32 fDT, SuMatrix4* pPrevBone ) const;

   /// Checks for the existance of an animation cut over a particular time interval
   SU_DLL bool HasCutInTimeInterval( float32 fPreviousTime, float32 fCurrentTime ) const;

private:

   SuBoneAnimator(); // restrict instantiation to Create() factory method
   
 
   // the animator supports pulling animation curves either from a single matrix-valued curve, 
   // or from a set of curves that represent a decomposed transformation matrix.

   float32 m_fAnimationLength;            ///< Length of the animation for this animator
   SuAnimationCurvePtr m_pSingleCurve;    ///< Single curve, (Matrix)
   SuAnimationCurvePtr m_pTranslateCurve; ///< Translation curve (float3)
   SuAnimationCurvePtr m_pRotateCurve;    ///< Rotation Curve (Quat)
   SuAnimationCurvePtr m_pScaleCurve;     ///< Scaling curve (float3)

};


#endif // _SU_BONEANIMATOR_H_
