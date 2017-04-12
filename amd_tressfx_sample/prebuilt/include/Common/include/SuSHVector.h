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

//=================================================================================================================================
//
//          Includes / defines / typedefs / static member variable initialization block
//
//=================================================================================================================================

#ifndef _SU_SHVECTOR_H_
#define _SU_SHVECTOR_H_

#include "SuTypes.h"
#include "SuVector3.h"
#include "SuMatrix4.h"

#define SU_SH_MINORDER 2
#define SU_SH_MAXORDER 6

//=================================================================================================================================
/// \ingroup Common
/// \brief A class to encapsulate a vector of spherical harmonic coefficients
/// 
//=================================================================================================================================
// tolua_begin
class SuSHVector
{
public:

   /// Constructor
   SU_DLL SuSHVector( uint32 nOrder=4 );   

   /// Copy constructor
   SU_DLL SuSHVector( const SuSHVector& rRHS ); 

   // tolua_end -- tolua doesn't support assignments.  Prevent warning
   /// Assignment
   SU_DLL const SuSHVector& operator=( const SuSHVector& rRHS ); 
   // tolua_begin


   /// Helper method to evaluate spherical harmonics for a particular direction
   static SU_DLL void EvalDirection( uint32 nOrder, const SuVector3& rDirection, SuSHVector& rSHOut, bool bApplyNormalizer = false );

   /// Helper method to project a directional light onto spherical harmonics
   static SU_DLL void EvalDirectionalLight( uint32 nOrder, const SuVector3& rDirection, const SuVector3& rColor, 
                                     SuSHVector& rRedOut, SuSHVector& rGreenOut, SuSHVector& rBlueOut );

   /// Helper function to project a hemispherical light onto spherical harmonics
   static SU_DLL void EvalHemisphereLight( uint32 nOrder, const SuVector3& rDirection, const SuVector3& rTopColor, const SuVector3& rBottomColor,
                                    SuSHVector& rRedOut, SuSHVector& rGreenOut, SuSHVector& rBlueOut );


   /// Helper function to project a cone light onto spherical harmonics
   static SU_DLL void EvalConeLight( uint32 nOrder, const SuVector3& rDirection, float fRadius, const SuVector3& rColor, 
                              SuSHVector& rRedOut, SuSHVector& rGreenOut, SuSHVector& rBlueOut );

   /// Get the cosine convolution coefficient for a given band (NOTE: nBandIndex != nCoefIndex)
   inline float32 GetCovolutionCoefficient ( uint32 nBandIndex ) const { return SuSHVector::m_fConvolutionCoef[nBandIndex]; };

   /// Accessor for SH coefficients
   /// \param i  Index of the SH coefficient desired.
   /// \return  The ith coefficient in the vector
   inline float32 GetCoefficient( uint32 i ) const { return m_coefficients[i]; };
   
   /// Changes the value of a particular SH coefficient
   /// \param i      Index of the SH coefficient
   /// \param fCoeff The new value for the coefficient
   inline void SetCoefficient( uint32 i, float32 fCoeff ) { m_coefficients[i] = fCoeff; };

   /// Returns the number of coefficients in the SH vector
   inline uint32 GetNumCoefficients() const { return (uint32) m_coefficients.size(); };

   /// Changes the order of the SH vector
   SU_DLL void SetOrder( uint32 nOrder ) ;

   /// Returns the order of the SH vector
   SU_DLL uint32 GetOrder() const;

   /// Addition operator
   SU_DLL SuSHVector operator+( const SuSHVector& rRHS ) const;

   /// Scaling operator
   SU_DLL SuSHVector operator*( float32 fScale ) const;

   /// Dot product
   SU_DLL float32 Dot( const SuSHVector& rRHS ) const;

   /// Spherical harmonic rotation
   SU_DLL void Rotate( const SuMatrix4& rMatrix, SuSHVector& rOutput ) const;

   /// Rotation about the Z axis
   SU_DLL void RotateZ( float32 fAngle, SuSHVector& rOutput ) const;

   /// Rotation about the X axis by 90 degrees
   SU_DLL void RotateX90( bool bPositive, SuSHVector& rOutput ) const;

// tolua_end
private:


   /// Array of SH coefficients
   SuArray<float32> m_coefficients;

   /// Directional lights get an extra normalization factor (to remain consitant with the D3DX way of doing things).  
   /// It makes the case where the normal is alligned with the light reflect light at unit intensity.
   static SU_DLL const float64 m_fLightBasisNormalizer[SU_SH_MAXORDER];

   /// Lookup table for renomalization constant for a given SH basis function
   static SU_DLL const float64 m_fRenormalizationConst[SU_SH_MAXORDER*SU_SH_MAXORDER];

   /// Convolution coeffients for cosine lobe
   static SU_DLL const float32 m_fConvolutionCoef[SU_SH_MAXORDER];

   /// Convert cartesian coordinates to spherical coordinates using notation and coord frame consitent with the rest of the SH helpers
   static SU_DLL void CartesianToSpherical ( const SuVector3& rCartesian, float64* pTheta, float64* pPhi );

   /// Evaludate the associated legendre polynomial for a particular SH basis function
   static SU_DLL float64 ComputeP ( int32 nL, int32 nM, float64 fX );

   /// Compute the renomalization constant for a given SH basis function using a look up table.
   static SU_DLL  float64 LookUpK ( int32 nL, int32 nM );

   /// Evaluate a SH basis function at a point on the sphere.
   static SU_DLL float64 EvaluateSH ( int32 nL, int32 nM, float64 fCosTheta, float64 fPhi );

// tolua_begin
};
// tolua_end


#endif // _SU_SHVECTOR_H_
