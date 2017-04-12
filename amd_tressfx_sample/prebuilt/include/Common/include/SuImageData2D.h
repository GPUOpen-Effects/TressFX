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

#ifndef _SU_IMAGEDATA2D_H__
#define _SU_IMAGEDATA2D_H__

#include "SuTypes.h"
#include "SuMath.h"
#include "SuAlgorithm.h"
#include "SuArray.h"
#include "SuCountedPtr.h"

// tolua_begin
class SuImageData2D;
typedef SuCountedPtr<SuImageData2D> SuImageData2DPtr;
// tolua_end

//=========================================================================================================
/// \ingroup Common
/// \brief A class to encapsulate a 2D array of image data
/// 
//=========================================================================================================
// tolua_begin
class SuImageData2D
{
public:
   /// Constructor
   SU_DLL SuImageData2D();

   /// Constructor
   SU_DLL SuImageData2D( uint32 nWidth, uint32 nHeight, uint32 nNumChannels );

   /// Destructor
   virtual SU_DLL ~SuImageData2D();

   /// Returns the image width
   inline uint32 GetWidth() const { return m_nWidth; }

   /// Returns the image height
   inline uint32 GetHeight() const { return m_nHeight; }

   /// Returns the number of channels in the image
   inline uint32 GetNumChannels() const { return m_nNumChannels; }

   /// Pixel data access
   inline float64 Pixel( uint32 nX, uint32 nY, uint32 nChannel ) const { return m_imageData[ CoordToIndex(nX, nY, nChannel) ]; }

   /// Pixel data access
   inline float64& Pixel( uint32 nX, uint32 nY, uint32 nChannel ) { return m_imageData[ CoordToIndex(nX, nY, nChannel) ]; }

   /// Applys a gamma to all pixels in the image
   SU_DLL void ApplyGamma( float64 fGamma );

   /// Creates a half-resolution image using a 2x2 box filter kernel
   SU_DLL SuImageData2DPtr CreateHalfResImage() const;

   /// Copies a channel from a source image
   SU_DLL void CopyChannelData( uint32 nDestChannelID, const SuImageData2D& rSrcImg, uint32 nSrcChannelID );

   /// Adds the contents of pInput to this image
   SU_DLL void AddPixels( SuImageData2DPtr pInput );

   /// Scales an image by a constant factor
   SU_DLL void ScalePixels( float64 fScaleFact );

   /// Averages two images
   SU_DLL void AveragePixels( SuImageData2DPtr pInput );

   /// Vertical flip
   SU_DLL void FlipVertical ( void );

// tolua_end


   /// Sets the image data by converting uint8 data to floats
   SU_DLL void FillFromUInt8Data( const uint8* pData );

   /// Sets the image data by converting uint16 data to floats
   SU_DLL void FillFromUInt16Data( const uint16* pData );

   /// Converts the image data to uint8
   SU_DLL void ConvertToUInt8( uint8* pData, uint32 nRedOffset = 0, uint32 nGreenOffset = 1, uint32 nBlueOffset = 2,
                                      uint32 nAlphaOffset = 3, int32 nDestPixelStride = -1 ) const;

   /// Converts the image data to uint16
   SU_DLL void ConvertToUInt16( uint16* pData, uint32 nRedOffset = 0, uint32 nGreenOffset = 1, uint32 nBlueOffset = 2,
                                       uint32 nAlphaOffset = 3, int32 nDestPixelStride = -1 ) const;

   
   /// Returns the raw image data
   inline const SuArray<float64>& GetImageBuffer() const { return m_imageData; }

private:

   /// Returns the number of floats in the data array
   uint32 GetNumDataElements() const { return m_nWidth * m_nHeight * m_nNumChannels; }

   /// Computes an index into the data buffer for a given pixel coordinate
   ///
   /// Implements clamp to edge if coord out of bounds
   inline uint32 CoordToIndex( uint32 nX, uint32 nY, uint32 nChannel ) const 
   { 
      SU_ASSERT( nChannel < m_nNumChannels );
      return SuMin(nY, m_nHeight-1) * m_nRowStride +
            SuMin(nX, m_nWidth-1) * m_nNumChannels + nChannel; 
   }

   uint32 m_nWidth;
   uint32 m_nHeight;
   uint32 m_nNumChannels;
   uint32 m_nRowStride;

   SuArray<float64> m_imageData;

   /// \brief helper functor for exponentiating a number
   class PowFunctor
   {
   public:
      /// Constructor
      explicit PowFunctor( float64 fExp ) : m_fExp( fExp ) {}

      /// Operator to exponentiate the input parameter
      void operator() ( float64 &fVal ) { fVal = SuPow( fVal, m_fExp ); }

   private:
      float64 m_fExp;
   };

// tolua_begin
};
// tolua_end

/// Creates an SuImageData2D object from raw input data
template<typename PerChannelDataType>
SuImageData2DPtr SuCreateImageDataFromRawData( uint32  nWidth, uint32  nHeight, uint32  nNumOutputChannels,
                                               uint32  nNumInputChannels, uint32  nPitch, PerChannelDataType* pSrcData,
                                               float32 fInputDataScaleFactor = 1.0f, uint32  nRedInputOffset = 0,
                                               uint32  nGreenInputOffset = 1, uint32  nBlueInputOffset = 2,
                                               uint32  nAlphaInputOffset = 3 );

#include "SuImageData2D.inl"

#endif // _SU_IMAGEDATA2D_H_
