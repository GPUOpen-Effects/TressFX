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

#ifndef _SUMATH_H
#define _SUMATH_H

/// \defgroup Math Math support

/// \ingroup Math
/// @{

/// Definition of PI
#define SU_PI        3.14159265358979323846

/// Definition of FLT_MAX
#define SU_FLT_MAX   3.402823466e+38F

/// Definition of FLT_MIN
#define SU_FLT_MIN   1.175494351e-38F

/// Definition of EPSILON
#define SU_EPSILON   1e-06f

/// Converts angle from degrees to radians
inline float32 SuDegToRad ( float32 fAngleInDegrees );
inline float64 SuDegToRad ( float64 fAngleInDegrees );

/// Converts angle from radians to degrees
inline float32 SuRadToDeg ( float32 fAngleInRadians );
inline float64 SuRadToDeg ( float64 fAngleInRadians );

/// Returns the absolute value of the passed in number
inline float32 SuAbs ( float32 fVal );
inline float64 SuAbs ( float64 fVal );

/// Returns the sign of the passed in number
inline int SuSign( float32 fVal );
inline int SuSign( float64 fVal );

/// Performs the trigonometric arc cosine operation on fVal and returns an angle in the range from 
/// 0 to PI expressed in radians. 
inline float32 SuACos ( float32 fVal );
inline float64 SuACos ( float64 fVal );

/// Performs the trigonometric arc sine operation on fVal and returns an angle in the range from 
/// -PI/2 to PI/2 expressed in radians. 
inline float32 SuASin ( float32 fVal );
inline float64 SuASin ( float64 fVal );

/// Performs the trigonometric arctangent operation on fVal and returns an angle in the range from 
/// -PI/2 to PI/2 expressed in radians. 
inline float32 SuATan ( float32 fVal );
inline float64 SuATan ( float64 fVal );

/// Performs the trigonometric arctangent operation on y/x and returns an angle in the range from 
/// -PI to PI expressed in radians, using the signs of the parameters to determine the quadrant.
inline float32 SuATan2 ( float32 y, float32 x );
inline float64 SuATan2 ( float64 y, float64 x );

/// Returns the smallest integer that is greater or equal to fVal
inline float32 SuCeil ( float32 fVal );
inline float64 SuCeil ( float64 fVal );

/// Performs the trigonometric cosine operation on fVal returning a value between -1 and 1. 
inline float32 SuCos ( float32 fVal );
inline float64 SuCos ( float64 fVal );

/// Returns hyperbolic cosine of fVal.
inline float32 SuCosH ( float32 fVal );
inline float64 SuCosH ( float64 fVal );

/// Returns the exponential value of parameter fVal. 
inline float32 SuExp ( float32 fVal );
inline float64 SuExp ( float64 fVal );

/// Returns the largest integer that is less than or equal to fVal
inline float32 SuFloor ( float32 fVal );
inline float64 SuFloor ( float64 fVal );

/// Returns the natural logarithm of parameter fVal 
inline float32 SuLog ( float32 fVal );
inline float64 SuLog ( float64 fVal );

/// Returns the logarithm base 10 of parameter fVal
inline float32 SuLog10 ( float32 fVal );
inline float64 SuLog10 ( float64 fVal );

/// Returns x raised to the power of y
inline float32 SuPow ( float32 x, float32 y );
inline float64 SuPow ( float64 x, float64 y );

/// Performs the trigonometric sine operation on fVal returning a value between -1 and 1. 
inline float32 SuSin ( float32 fVal );
inline float64 SuSin ( float64 fVal );

/// Returns hyperbolic sine of x
inline float32 SuSinH ( float32 fVal );
inline float64 SuSinH ( float64 fVal );

/// Returns the square root of parameter fVal
inline float32 SuSqrt ( float32 fVal );
inline float64 SuSqrt ( float64 fVal );

/// Performs the trigonometric tangent operation on fVal
inline float32 SuTan ( float32 fVal );
inline float64 SuTan ( float64 fVal );

/// Returns hyperbolic tangent of fVal
inline float32 SuTanH ( float32 fVal );
inline float64 SuTanH ( float64 fVal );

/// Returns the square root of the fVal
inline float32 SuSqrt ( float32 fVal );
inline float64 SuSqrt ( float64 fVal );

/// Clamps the parameter from 0.0 to 1.0
inline float32 SuSaturate ( float32 fVal );
inline float64 SuSaturate ( float64 fVal );

/// Clamps the parameter
inline float32 SuClamp ( float32 fVal, float32 fMin, float32 fMax );
inline float64 SuClamp ( float64 fVal, float64 fMin, float64 fMax );

/// Returns the closest power of to that greater or equal to n
inline uint32 SuRoundUpToNearestPowerOfTwo( uint32 n );

/// Checks whether a number is a power of 2
inline bool SuIsPowerOfTwo( uint32 n );

/// Takes the log of an integer power of two
inline uint32 SuLog2( uint32 nInt );



// Include the platform-specific implementation of the math functions.
#ifdef __SYMBIAN32__
   #include "../Symbian/SuMathSymbian.inl"
#else
   #include "SuMath.inl"
#endif


/// @}

#endif //_SUMATH_H
