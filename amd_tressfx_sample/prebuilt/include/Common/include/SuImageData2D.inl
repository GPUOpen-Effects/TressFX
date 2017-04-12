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
/// \ingroup Common
///
/// \param nWidth, nHeight The texture dimension of both the input data and the returned image data object
/// \param nNumOutputChannels The number of channels in the returned image data object
/// \param nNumInputChannels The number of channels (components per pixel) of the input data
/// \param nPitch The row pitch of the input data in bytes
/// \param pSrcData Pointer to the input data
/// \param fInputDataScaleFactor factor to scale input data with after it is converted to float
/// \param nRedInputOffset   The offset for the red channel data from the beginning of a pixel
/// \param nGreenInputOffset The offset for the green channel data from the beginning of a pixel
/// \param nBlueInputOffset  The offset for the blue channel data from the beginning of a pixel
/// \param nAlphaInputOffset The offset for the alpha channel data from the beginning of a pixel
///
/// \return A shared ptr to an image data object that contains a copy the input texture data
//=================================================================================================================================
template<typename PerChannelDataType>
SuImageData2DPtr SuCreateImageDataFromRawData( uint32  nWidth,
                                               uint32  nHeight,
                                               uint32  nNumOutputChannels,
                                               uint32  nNumInputChannels,
                                               uint32  nPitch,
                                               PerChannelDataType* pSrcData,
                                               float32 fInputDataScaleFactor,
                                               uint32  nRedInputOffset,
                                               uint32  nGreenInputOffset,
                                               uint32  nBlueInputOffset,
                                               uint32  nAlphaInputOffset )
{
   SuImageData2D* pImg = new SuImageData2D( nWidth, nHeight, nNumOutputChannels );
   SU_ASSERT( pImg );

   uint32 nInputChannelOffset[4] = 
   {
      nRedInputOffset,
      nGreenInputOffset,
      nBlueInputOffset, 
      nAlphaInputOffset
   };

   for( uint32 nY = 0; nY < nHeight; nY++ )
   {
      // Get ptr to start of row data
      PerChannelDataType* pData = reinterpret_cast<PerChannelDataType*>( reinterpret_cast<uint8*>(pSrcData) + nY * nPitch );

      for( uint32 nX = 0; nX < nWidth; nX++ )
      {
         for( uint32 nCh = 0; nCh < nNumOutputChannels; nCh++ )
         {
            pImg->Pixel( nX, nY, nCh ) = static_cast<float32>( pData[ nInputChannelOffset[nCh] ] ) * fInputDataScaleFactor;
         } // end for( each channel )
         pData += nNumInputChannels;
      } // end for( each pixel )
   } // end for( each row )

   return SuImageData2DPtr( pImg );
}
