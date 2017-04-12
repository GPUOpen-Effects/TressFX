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

#ifndef _SU_ANIMATIONUTILS_H_
#define _SU_ANIMATIONUTILS_H_

class SuAnimationSet;
class SuAnimationCurve;

/// \ingroup ObjectManager
/// \brief  Appends one animation set to another
SU_DLL bool SuAppendAnimationSet( SuAnimationSet* pSet1, const SuAnimationSet* pSet2 );

/// \ingroup ObjectManager
/// \brief  Appends one animation curve to another
SU_DLL bool SuAppendAnimationCurve( SuAnimationCurve* pCurve1, const SuAnimationCurve* pCurve2 );

/// \ingroup ObjectManager
/// \brief  Extends an animation set, repeating the last value of the curve at that end.
SU_DLL bool SuExtendAnimationSet( SuAnimationSet* pSet, float fAdditionalTime );

/// \ingroup ObjectManager
/// \brief  Extends an animation curve, repeating the last value of the curve at that end.
SU_DLL bool SuExtendAnimationCurve( SuAnimationCurve* pCurve, float fAdditionalTime );





#endif // _SU_ANIMATIONUTILS_H_
