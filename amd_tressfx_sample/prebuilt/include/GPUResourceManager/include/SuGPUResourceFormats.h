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

#ifndef _SU_GPURESOURCEFORMATS_H_
#define _SU_GPURESOURCEFORMATS_H_

#include "SuMemoryBuffer.h"
#include "SuArray.h"
#include "SuAlgorithm.h"

//....................................................................................................................
/// Resource data formats supported in Sushi3
//....................................................................................................................
// tolua_begin
enum SuGPUResourceFormat
{
   SU_FORMAT_UNKNOWN = 0,
   SU_FORMAT_R32G32B32A32_TYPELESS,
   SU_FORMAT_R32G32B32A32_FLOAT,
   SU_FORMAT_R32G32B32A32_UINT,
   SU_FORMAT_R32G32B32A32_SINT,
   SU_FORMAT_R32G32B32_TYPELESS,
   SU_FORMAT_R32G32B32_FLOAT,
   SU_FORMAT_R32G32B32_UINT,
   SU_FORMAT_R32G32B32_SINT,
   SU_FORMAT_R16G16B16A16_TYPELESS,
   SU_FORMAT_R16G16B16A16_FLOAT,
   SU_FORMAT_R16G16B16A16_UNORM,
   SU_FORMAT_R16G16B16A16_UINT,
   SU_FORMAT_R16G16B16A16_SNORM,
   SU_FORMAT_R16G16B16A16_SINT,
   SU_FORMAT_R32G32_TYPELESS,
   SU_FORMAT_R32G32_FLOAT,
   SU_FORMAT_R32G32_UINT,
   SU_FORMAT_R32G32_SINT,
   SU_FORMAT_R32G8X24_TYPELESS,
   SU_FORMAT_D32_FLOAT_S8X24_UINT,
   SU_FORMAT_R32_FLOAT_X8X24_TYPELESS,
   SU_FORMAT_X32_TYPELESS_G8X24_UINT,
   SU_FORMAT_R10G10B10A2_TYPELESS,
   SU_FORMAT_R10G10B10A2_UNORM,
   SU_FORMAT_R10G10B10A2_UINT,
   SU_FORMAT_R11G11B10_FLOAT,
   SU_FORMAT_R8G8B8A8_TYPELESS,
   SU_FORMAT_R8G8B8A8_UNORM,
   SU_FORMAT_R8G8B8A8_UNORM_SRGB,
   SU_FORMAT_R8G8B8A8_UINT,
   SU_FORMAT_R8G8B8A8_SNORM,
   SU_FORMAT_R8G8B8A8_SINT,
   SU_FORMAT_R16G16_TYPELESS,
   SU_FORMAT_R16G16_FLOAT,
   SU_FORMAT_R16G16_UNORM,
   SU_FORMAT_R16G16_UINT,
   SU_FORMAT_R16G16_SNORM,
   SU_FORMAT_R16G16_SINT,
   SU_FORMAT_R32_TYPELESS,
   SU_FORMAT_D32_FLOAT,
   SU_FORMAT_R32_FLOAT,
   SU_FORMAT_R32_UINT,
   SU_FORMAT_R32_SINT,
   SU_FORMAT_R24G8_TYPELESS,
   SU_FORMAT_D24_UNORM_S8_UINT,
   SU_FORMAT_R24_UNORM_X8_TYPELESS,
   SU_FORMAT_X24_TYPELESS_G8_UINT,
   SU_FORMAT_R8G8_TYPELESS,
   SU_FORMAT_R8G8_UNORM,
   SU_FORMAT_R8G8_UINT,
   SU_FORMAT_R8G8_SNORM,
   SU_FORMAT_R8G8_SINT,
   SU_FORMAT_R16_TYPELESS,
   SU_FORMAT_R16_FLOAT,
   SU_FORMAT_D16_UNORM,
   SU_FORMAT_R16_UNORM,
   SU_FORMAT_R16_UINT,
   SU_FORMAT_R16_SNORM,
   SU_FORMAT_R16_SINT,
   SU_FORMAT_R8_TYPELESS,
   SU_FORMAT_R8_UNORM,
   SU_FORMAT_R8_UINT,
   SU_FORMAT_R8_SNORM,
   SU_FORMAT_R8_SINT,
   SU_FORMAT_A8_UNORM,
   SU_FORMAT_R1_UNORM,
   SU_FORMAT_R9G9B9E5_SHAREDEXP,
   SU_FORMAT_R8G8_B8G8_UNORM,
   SU_FORMAT_G8R8_G8B8_UNORM,
   SU_FORMAT_BC1_TYPELESS,
   SU_FORMAT_BC1_UNORM,
   SU_FORMAT_BC1_UNORM_SRGB,
   SU_FORMAT_BC2_TYPELESS,
   SU_FORMAT_BC2_UNORM,
   SU_FORMAT_BC2_UNORM_SRGB,
   SU_FORMAT_BC3_TYPELESS,
   SU_FORMAT_BC3_UNORM,
   SU_FORMAT_BC3_UNORM_SRGB,
   SU_FORMAT_BC4_TYPELESS,
   SU_FORMAT_BC4_UNORM,
   SU_FORMAT_BC4_SNORM,
   SU_FORMAT_BC5_TYPELESS,
   SU_FORMAT_BC5_UNORM,
   SU_FORMAT_BC5_SNORM,
   SU_FORMAT_BC7_TYPELESS,
   SU_FORMAT_BC7_UNORM,
   SU_FORMAT_BC7_UNORM_SRGB,
   SU_FORMAT_B5G6R5_UNORM,
   SU_FORMAT_B5G5R5A1_UNORM,
   SU_FORMAT_B8G8R8A8_UNORM,
   SU_FORMAT_B8G8R8X8_UNORM,
   SU_FORMAT_ATITC_RGB,
   SU_FORMAT_ATITC_RGBA,
   SU_FORMAT_ETC1,
   SU_FORMAT_ETC3,
   SU_FORMAT_ETC5,
   SU_FORMAT_D24_COMPARISON_S8_UINT,
   SU_FORMAT_D16_COMPARISON,
   SU_FORMAT_COUNT      // number of GPU resource formats

}; // End of SuGPUResourceFormat
// tolua_end

/// \ingroup GPUResourceManager
/// Returns the number of bits per pixel for a pixel of the given format.  
inline uint8 SuGetBitsPerPixel( SuGPUResourceFormat eFormat )
{
   // big ugly lookup table of resource format sizes
   static const uint8 nElementSizesInBits[SU_FORMAT_COUNT] = {
      0,    // SU_FORMAT_UNKNOWN,
      128,  // SU_FORMAT_R32G32B32A32_TYPELESS,
      128,  // SU_FORMAT_R32G32B32A32_FLOAT,
      128,  // SU_FORMAT_R32G32B32A32_UINT,
      128,  // SU_FORMAT_R32G32B32A32_SINT,
      96,   // SU_FORMAT_R32G32B32_TYPELESS,
      96,   // SU_FORMAT_R32G32B32_FLOAT,
      96,   // SU_FORMAT_R32G32B32_UINT,
      96,   // SU_FORMAT_R32G32B32_SINT,
      64,   // SU_FORMAT_R16G16B16A16_TYPELESS,
      64,   // SU_FORMAT_R16G16B16A16_FLOAT,
      64,   // SU_FORMAT_R16G16B16A16_UNORM,
      64,   // SU_FORMAT_R16G16B16A16_UINT,
      64,   // SU_FORMAT_R16G16B16A16_SNORM,
      64,   // SU_FORMAT_R16G16B16A16_SINT,
      64,   // SU_FORMAT_R32G32_TYPELESS,
      64,   // SU_FORMAT_R32G32_FLOAT,
      64,   // SU_FORMAT_R32G32_UINT,
      64,   // SU_FORMAT_R32G32_SINT,
      64,   // SU_FORMAT_R32G8X24_TYPELESS,
      64,   // SU_FORMAT_D32_FLOAT_S8X24_UINT,
      64,   // SU_FORMAT_R32_FLOAT_X8X24_TYPELESS,
      64,   // SU_FORMAT_X32_TYPELESS_G8X24_UINT,
      32,   // SU_FORMAT_R10G10B10A2_TYPELESS,
      32,   // SU_FORMAT_R10G10B10A2_UNORM,
      32,   // SU_FORMAT_R10G10B10A2_UINT,
      32,   // SU_FORMAT_R11G11B10_FLOAT,
      32,   // SU_FORMAT_R8G8B8A8_TYPELESS,
      32,   // SU_FORMAT_R8G8B8A8_UNORM,
      32,   // SU_FORMAT_R8G8B8A8_UNORM_SRGB,
      32,   // SU_FORMAT_R8G8B8A8_UINT,
      32,   // SU_FORMAT_R8G8B8A8_SNORM,
      32,   // SU_FORMAT_R8G8B8A8_SINT,
      32,   // SU_FORMAT_R16G16_TYPELESS,
      32,   // SU_FORMAT_R16G16_FLOAT,
      32,   // SU_FORMAT_R16G16_UNORM,
      32,   // SU_FORMAT_R16G16_UINT,
      32,   // SU_FORMAT_R16G16_SNORM,
      32,   // SU_FORMAT_R16G16_SINT,
      32,   // SU_FORMAT_R32_TYPELESS,
      32,   // SU_FORMAT_D32_FLOAT,
      32,   // SU_FORMAT_R32_FLOAT,
      32,   // SU_FORMAT_R32_UINT,
      32,   // SU_FORMAT_R32_SINT,
      32,   // SU_FORMAT_R24G8_TYPELESS,
      32,   // SU_FORMAT_D24_UNORM_S8_UINT,
      32,   // SU_FORMAT_R24_UNORM_X8_TYPELESS,
      32,   // SU_FORMAT_X24_TYPELESS_G8_UINT,
      16,   // SU_FORMAT_R8G8_TYPELESS,
      16,   // SU_FORMAT_R8G8_UNORM,
      16,   // SU_FORMAT_R8G8_UINT,
      16,   // SU_FORMAT_R8G8_SNORM,
      16,   // SU_FORMAT_R8G8_SINT,
      16,   // SU_FORMAT_R16_TYPELESS,
      16,   // SU_FORMAT_R16_FLOAT,
      16,   // SU_FORMAT_D16_UNORM,
      16,   // SU_FORMAT_R16_UNORM,
      16,   // SU_FORMAT_R16_UINT,
      16,   // SU_FORMAT_R16_SNORM,
      16,   // SU_FORMAT_R16_SINT,
      8,    // SU_FORMAT_R8_TYPELESS,
      8,    // SU_FORMAT_R8_UNORM,
      8,    // SU_FORMAT_R8_UINT,
      8,    // SU_FORMAT_R8_SNORM,
      8,    // SU_FORMAT_R8_SINT,
      8,    // SU_FORMAT_A8_UNORM,
      1,    // SU_FORMAT_R1_UNORM,
      32,   // SU_FORMAT_R9G9B9E5_SHAREDEXP,
      16,   // SU_FORMAT_R8G8_B8G8_UNORM,
      16,   // SU_FORMAT_G8R8_G8B8_UNORM,
      4,    // SU_FORMAT_BC1_TYPELESS,
      4,    // SU_FORMAT_BC1_UNORM,
      4,    // SU_FORMAT_BC1_UNORM_SRGB,
      8,    // SU_FORMAT_BC2_TYPELESS,
      8,    // SU_FORMAT_BC2_UNORM,
      8,    // SU_FORMAT_BC2_UNORM_SRGB,
      8,    // SU_FORMAT_BC3_TYPELESS,
      8,    // SU_FORMAT_BC3_UNORM,
      8,    // SU_FORMAT_BC3_UNORM_SRGB,
      4,    // SU_FORMAT_BC4_TYPELESS,
      4,    // SU_FORMAT_BC4_UNORM,
      4,    // SU_FORMAT_BC4_SNORM,
      8,    // SU_FORMAT_BC5_TYPELESS,
      8,    // SU_FORMAT_BC5_UNORM,
      8,    // SU_FORMAT_BC5_SNORM,
	  8,    // SU_FORMAT_BC7_TYPELESS,
	  8,    // SU_FORMAT_BC7_UNORM,
	  8,    // SU_FORMAT_BC7_UNORM_SRGB,
      16,   // SU_FORMAT_B5G6R5_UNORM,
      16,   // SU_FORMAT_B5G5R5A1_UNORM,
      32,   // SU_FORMAT_B8G8R8A8_UNORM,
      32,   // SU_FORMAT_B8G8R8X8_UNORM,
      4,    // SU_FORMAT_ATITC_RGB,
      8,    // SU_FORMAT_ATITC_RGBA,
      4,    // SU_FORMAT_ETC1,
      8,    // SU_FORMAT_ETC3,
      8,    // SU_FORMAT_ETC5,
      32,   // SU_FORMAT_D24_COMPARISON_S8_UINT,
      16    // SU_FORMAT_D16_COMPARISON,
   };

   uint8 nElemSizeInBits = nElementSizesInBits[eFormat];
   return nElemSizeInBits;
};



inline bool SuIsFormatTypeless( SuGPUResourceFormat eFormat )
{
   // this lookup table contains 1 if a resource is typeless, 0 otherwise
   static const uint8 nIsResourceTypeless[SU_FORMAT_COUNT] = {
      0,    // SU_FORMAT_UNKNOWN,
      1,  // SU_FORMAT_R32G32B32A32_TYPELESS,
      0,  // SU_FORMAT_R32G32B32A32_FLOAT,
      0,  // SU_FORMAT_R32G32B32A32_UINT,
      0,  // SU_FORMAT_R32G32B32A32_SINT,
      1,   // SU_FORMAT_R32G32B32_TYPELESS,
      0,   // SU_FORMAT_R32G32B32_FLOAT,
      0,   // SU_FORMAT_R32G32B32_UINT,
      0,   // SU_FORMAT_R32G32B32_SINT,
      1,   // SU_FORMAT_R16G16B16A16_TYPELESS,
      0,   // SU_FORMAT_R16G16B16A16_FLOAT,
      0,   // SU_FORMAT_R16G16B16A16_UNORM,
      0,   // SU_FORMAT_R16G16B16A16_UINT,
      0,   // SU_FORMAT_R16G16B16A16_SNORM,
      0,   // SU_FORMAT_R16G16B16A16_SINT,
      1,   // SU_FORMAT_R32G32_TYPELESS,
      0,   // SU_FORMAT_R32G32_FLOAT,
      0,   // SU_FORMAT_R32G32_UINT,
      0,   // SU_FORMAT_R32G32_SINT,
      1,   // SU_FORMAT_R32G8X24_TYPELESS,
      0,   // SU_FORMAT_D32_FLOAT_S8X24_UINT,
      0,   // SU_FORMAT_R32_FLOAT_X8X24_TYPELESS,
      0,   // SU_FORMAT_X32_TYPELESS_G8X24_UINT,
      1,   // SU_FORMAT_R10G10B10A2_TYPELESS,
      0,   // SU_FORMAT_R10G10B10A2_UNORM,
      0,   // SU_FORMAT_R10G10B10A2_UINT,
      0,   // SU_FORMAT_R11G11B10_FLOAT,
      1,   // SU_FORMAT_R8G8B8A8_TYPELESS,
      0,   // SU_FORMAT_R8G8B8A8_UNORM,
      0,   // SU_FORMAT_R8G8B8A8_UNORM_SRGB,
      0,   // SU_FORMAT_R8G8B8A8_UINT,
      0,   // SU_FORMAT_R8G8B8A8_SNORM,
      0,   // SU_FORMAT_R8G8B8A8_SINT,
      1,   // SU_FORMAT_R16G16_TYPELESS,
      0,   // SU_FORMAT_R16G16_FLOAT,
      0,   // SU_FORMAT_R16G16_UNORM,
      0,   // SU_FORMAT_R16G16_UINT,
      0,   // SU_FORMAT_R16G16_SNORM,
      0,   // SU_FORMAT_R16G16_SINT,
      1,   // SU_FORMAT_R32_TYPELESS,
      0,   // SU_FORMAT_D32_FLOAT,
      0,   // SU_FORMAT_R32_FLOAT,
      0,   // SU_FORMAT_R32_UINT,
      0,   // SU_FORMAT_R32_SINT,
      0,   // SU_FORMAT_R24G8_TYPELESS,
      0,   // SU_FORMAT_D24_UNORM_S8_UINT,
      0,   // SU_FORMAT_R24_UNORM_X8_TYPELESS,
      0,   // SU_FORMAT_X24_TYPELESS_G8_UINT,
      1,   // SU_FORMAT_R8G8_TYPELESS,
      0,   // SU_FORMAT_R8G8_UNORM,
      0,   // SU_FORMAT_R8G8_UINT,
      0,   // SU_FORMAT_R8G8_SNORM,
      0,   // SU_FORMAT_R8G8_SINT,
      1,   // SU_FORMAT_R16_TYPELESS,
      0,   // SU_FORMAT_R16_FLOAT,
      0,   // SU_FORMAT_D16_UNORM,
      0,   // SU_FORMAT_R16_UNORM,
      0,   // SU_FORMAT_R16_UINT,
      0,   // SU_FORMAT_R16_SNORM,
      0,   // SU_FORMAT_R16_SINT,
      1,    // SU_FORMAT_R8_TYPELESS,
      0,    // SU_FORMAT_R8_UNORM,
      0,    // SU_FORMAT_R8_UINT,
      0,    // SU_FORMAT_R8_SNORM,
      0,    // SU_FORMAT_R8_SINT,
      0,    // SU_FORMAT_A8_UNORM,
      0,    // SU_FORMAT_R1_UNORM,
      0,    // SU_FORMAT_R9G9B9E5_SHAREDEXP,
      0,    // SU_FORMAT_R8G8_B8G8_UNORM,
      0,    // SU_FORMAT_G8R8_G8B8_UNORM,
      1,    // SU_FORMAT_BC1_TYPELESS,
      0,    // SU_FORMAT_BC1_UNORM,
      0,    // SU_FORMAT_BC1_UNORM_SRGB,
      1,    // SU_FORMAT_BC2_TYPELESS,
      0,    // SU_FORMAT_BC2_UNORM,
      0,    // SU_FORMAT_BC2_UNORM_SRGB,
      1,    // SU_FORMAT_BC3_TYPELESS,
      0,    // SU_FORMAT_BC3_UNORM,
      0,    // SU_FORMAT_BC3_UNORM_SRGB,
      1,    // SU_FORMAT_BC4_TYPELESS,
      0,    // SU_FORMAT_BC4_UNORM,
      0,    // SU_FORMAT_BC4_SNORM,
      1,    // SU_FORMAT_BC5_TYPELESS,
      0,    // SU_FORMAT_BC5_UNORM,
      0,    // SU_FORMAT_BC5_SNORM,
	  1,	// SU_FORMAT_BC7_TYPELESS,
	  0,	// SU_FORMAT_BC7_UNORM,
	  0,	// SU_FORMAT_BC7_UNORM_SRGB,
      0,    // SU_FORMAT_B5G6R5_UNORM,
      0,    // SU_FORMAT_B5G5R5A1_UNORM,
      0,    // SU_FORMAT_B8G8R8A8_UNORM,
      0,    // SU_FORMAT_B8G8R8X8_UNORM,
      0,    // SU_FORMAT_ATITC_RGB,
      0,    // SU_FORMAT_ATITC_RGBA,
      0,    // SU_FORMAT_ETC1,
      0,    // SU_FORMAT_ETC3,
      0,    // SU_FORMAT_ETC5,
      0,    // SU_FORMAT_D24_COMPARISON_S8_UINT,
      0     // SU_FORMAT_D16_COMPARISON,
   };

   return (nIsResourceTypeless[ eFormat ] != 0);
}


/// \ingroup GPUResourceManager
/// Tests whether a resource format is SRGB
/// \return True if the resource format is an SRGB format, false otherwise.
inline bool SuIsFormatSRGB( SuGPUResourceFormat eFormat )
{
   return ( eFormat ==  SU_FORMAT_R8G8B8A8_UNORM_SRGB || eFormat == SU_FORMAT_BC1_UNORM_SRGB || 
            eFormat == SU_FORMAT_BC2_UNORM_SRGB || eFormat == SU_FORMAT_BC3_UNORM_SRGB || eFormat == SU_FORMAT_BC7_UNORM_SRGB );
};


/// \ingroup GPUResourceManager
/// Returns the number of color channels for a given format
inline uint8 SuGetNumChannels( SuGPUResourceFormat eFormat )
{
   // big ugly lookup table of resource format channel counts
   static const uint8 nChannelCounts[SU_FORMAT_COUNT] = {
      0,    // SU_FORMAT_UNKNOWN,
      4,  // SU_FORMAT_R32G32B32A32_TYPELESS,
      4,  // SU_FORMAT_R32G32B32A32_FLOAT,
      4,  // SU_FORMAT_R32G32B32A32_UINT,
      4,  // SU_FORMAT_R32G32B32A32_SINT,
      3,   // SU_FORMAT_R32G32B32_TYPELESS,
      3,   // SU_FORMAT_R32G32B32_FLOAT,
      3,   // SU_FORMAT_R32G32B32_UINT,
      3,   // SU_FORMAT_R32G32B32_SINT,
      4,   // SU_FORMAT_R16G16B16A16_TYPELESS,
      4,   // SU_FORMAT_R16G16B16A16_FLOAT,
      4,   // SU_FORMAT_R16G16B16A16_UNORM,
      4,   // SU_FORMAT_R16G16B16A16_UINT,
      4,   // SU_FORMAT_R16G16B16A16_SNORM,
      4,   // SU_FORMAT_R16G16B16A16_SINT,
      2,   // SU_FORMAT_R32G32_TYPELESS,
      2,   // SU_FORMAT_R32G32_FLOAT,
      2,   // SU_FORMAT_R32G32_UINT,
      2,   // SU_FORMAT_R32G32_SINT,
      2,   // SU_FORMAT_R32G8X24_TYPELESS,
      1,   // SU_FORMAT_D32_FLOAT_S8X24_UINT,
      1,   // SU_FORMAT_R32_FLOAT_X8X24_TYPELESS,
      1,   // SU_FORMAT_X32_TYPELESS_G8X24_UINT,
      4,   // SU_FORMAT_R10G10B10A2_TYPELESS,
      4,   // SU_FORMAT_R10G10B10A2_UNORM,
      4,   // SU_FORMAT_R10G10B10A2_UINT,
      3,   // SU_FORMAT_R11G11B10_FLOAT,
      4,   // SU_FORMAT_R8G8B8A8_TYPELESS,
      4,   // SU_FORMAT_R8G8B8A8_UNORM,
      4,   // SU_FORMAT_R8G8B8A8_UNORM_SRGB,
      4,   // SU_FORMAT_R8G8B8A8_UINT,
      4,   // SU_FORMAT_R8G8B8A8_SNORM,
      4,   // SU_FORMAT_R8G8B8A8_SINT,
      2,   // SU_FORMAT_R16G16_TYPELESS,
      2,   // SU_FORMAT_R16G16_FLOAT,
      2,   // SU_FORMAT_R16G16_UNORM,
      2,   // SU_FORMAT_R16G16_UINT,
      2,   // SU_FORMAT_R16G16_SNORM,
      2,   // SU_FORMAT_R16G16_SINT,
      1,   // SU_FORMAT_R32_TYPELESS,
      1,   // SU_FORMAT_D32_FLOAT,
      1,   // SU_FORMAT_R32_FLOAT,
      1,   // SU_FORMAT_R32_UINT,
      1,   // SU_FORMAT_R32_SINT,
      2,   // SU_FORMAT_R24G8_TYPELESS,
      1,   // SU_FORMAT_D24_UNORM_S8_UINT,
      1,   // SU_FORMAT_R24_UNORM_X8_TYPELESS,
      2,   // SU_FORMAT_X24_TYPELESS_G8_UINT,
      2,   // SU_FORMAT_R8G8_TYPELESS,
      2,   // SU_FORMAT_R8G8_UNORM,
      2,   // SU_FORMAT_R8G8_UINT,
      2,   // SU_FORMAT_R8G8_SNORM,
      2,   // SU_FORMAT_R8G8_SINT,
      1,   // SU_FORMAT_R16_TYPELESS,
      1,   // SU_FORMAT_R16_FLOAT,
      1,   // SU_FORMAT_D16_UNORM,
      1,   // SU_FORMAT_R16_UNORM,
      1,   // SU_FORMAT_R16_UINT,
      1,   // SU_FORMAT_R16_SNORM,
      1,   // SU_FORMAT_R16_SINT,
      1,   // SU_FORMAT_R8_TYPELESS,
      1,   // SU_FORMAT_R8_UNORM,
      1,   // SU_FORMAT_R8_UINT,
      1,   // SU_FORMAT_R8_SNORM,
      1,   // SU_FORMAT_R8_SINT,
      1,   // SU_FORMAT_A8_UNORM,
      1,   // SU_FORMAT_R1_UNORM,
      3,   // SU_FORMAT_R9G9B9E5_SHAREDEXP,
      3,   // SU_FORMAT_R8G8_B8G8_UNORM,
      3,   // SU_FORMAT_G8R8_G8B8_UNORM,
      4,    // SU_FORMAT_BC1_TYPELESS,
      4,    // SU_FORMAT_BC1_UNORM,
      4,    // SU_FORMAT_BC1_UNORM_SRGB,
      4,    // SU_FORMAT_BC2_TYPELESS,
      4,    // SU_FORMAT_BC2_UNORM,
      4,    // SU_FORMAT_BC2_UNORM_SRGB,
      4,    // SU_FORMAT_BC3_TYPELESS,
      4,    // SU_FORMAT_BC3_UNORM,
      4,    // SU_FORMAT_BC3_UNORM_SRGB,
      1,    // SU_FORMAT_BC4_TYPELESS,
      1,    // SU_FORMAT_BC4_UNORM,
      1,    // SU_FORMAT_BC4_SNORM,
      2,    // SU_FORMAT_BC5_TYPELESS,
      2,    // SU_FORMAT_BC5_UNORM,
      2,    // SU_FORMAT_BC5_SNORM,
	  4,	// SU_FORMAT_BC7_TYPELESS,
	  4,	// SU_FORMAT_BC7_UNORM,
	  4,	// SU_FORMAT_BC7_UNORM_SRGB,
      3,   // SU_FORMAT_B5G6R5_UNORM,
      4,   // SU_FORMAT_B5G5R5A1_UNORM,
      4,   // SU_FORMAT_B8G8R8A8_UNORM,
      4,   // SU_FORMAT_B8G8R8X8_UNORM,
      3,   // SU_FORMAT_ATITC_RGB,
      4,   // SU_FORMAT_ATITC_RGBA,
      3,    // SU_FORMAT_ETC1,
      4,    // SU_FORMAT_ETC3,
      4,    // SU_FORMAT_ETC5,
      1,   // SU_FORMAT_D24_COMPARISON_S8_UINT,
      1    // SU_FORMAT_D16_COMPARISON,
   };

   return nChannelCounts[eFormat];
};


/// \ingroup GPUResourceManager
/// Determines whether or not a given format has an alpha channel
inline bool SuHasAlphaChannel( SuGPUResourceFormat eFormat )
{
   // big ugly lookup table of resource format channel counts
   static const bool bHasAlpha[SU_FORMAT_COUNT] = {
      false,    // SU_FORMAT_UNKNOWN,
      true,  // SU_FORMAT_R32G32B32A32_TYPELESS,
      true,  // SU_FORMAT_R32G32B32A32_FLOAT,
      true,  // SU_FORMAT_R32G32B32A32_UINT,
      true,  // SU_FORMAT_R32G32B32A32_SINT,
      false,   // SU_FORMAT_R32G32B32_TYPELESS,
      false,   // SU_FORMAT_R32G32B32_FLOAT,
      false,   // SU_FORMAT_R32G32B32_UINT,
      false,   // SU_FORMAT_R32G32B32_SINT,
      true,   // SU_FORMAT_R16G16B16A16_TYPELESS,
      true,   // SU_FORMAT_R16G16B16A16_FLOAT,
      true,   // SU_FORMAT_R16G16B16A16_UNORM,
      true,   // SU_FORMAT_R16G16B16A16_UINT,
      true,   // SU_FORMAT_R16G16B16A16_SNORM,
      true,   // SU_FORMAT_R16G16B16A16_SINT,
      false,   // SU_FORMAT_R32G32_TYPELESS,
      false,   // SU_FORMAT_R32G32_FLOAT,
      false,   // SU_FORMAT_R32G32_UINT,
      false,   // SU_FORMAT_R32G32_SINT,
      false,   // SU_FORMAT_R32G8X24_TYPELESS,
      false,   // SU_FORMAT_D32_FLOAT_S8X24_UINT,
      false,   // SU_FORMAT_R32_FLOAT_X8X24_TYPELESS,
      false,   // SU_FORMAT_X32_TYPELESS_G8X24_UINT,
      true,   // SU_FORMAT_R10G10B10A2_TYPELESS,
      true,   // SU_FORMAT_R10G10B10A2_UNORM,
      true,   // SU_FORMAT_R10G10B10A2_UINT,
      false,   // SU_FORMAT_R11G11B10_FLOAT,
      true,   // SU_FORMAT_R8G8B8A8_TYPELESS,
      true,   // SU_FORMAT_R8G8B8A8_UNORM,
      true,   // SU_FORMAT_R8G8B8A8_UNORM_SRGB,
      true,   // SU_FORMAT_R8G8B8A8_UINT,
      true,   // SU_FORMAT_R8G8B8A8_SNORM,
      true,   // SU_FORMAT_R8G8B8A8_SINT,
      false,   // SU_FORMAT_R16G16_TYPELESS,
      false,   // SU_FORMAT_R16G16_FLOAT,
      false,   // SU_FORMAT_R16G16_UNORM,
      false,   // SU_FORMAT_R16G16_UINT,
      false,   // SU_FORMAT_R16G16_SNORM,
      false,   // SU_FORMAT_R16G16_SINT,
      false,   // SU_FORMAT_R32_TYPELESS,
      false,   // SU_FORMAT_D32_FLOAT,
      false,   // SU_FORMAT_R32_FLOAT,
      false,   // SU_FORMAT_R32_UINT,
      false,   // SU_FORMAT_R32_SINT,
      false,   // SU_FORMAT_R24G8_TYPELESS,
      false,   // SU_FORMAT_D24_UNORM_S8_UINT,
      false,   // SU_FORMAT_R24_UNORM_X8_TYPELESS,
      false,   // SU_FORMAT_X24_TYPELESS_G8_UINT,
      false,   // SU_FORMAT_R8G8_TYPELESS,
      false,   // SU_FORMAT_R8G8_UNORM,
      false,   // SU_FORMAT_R8G8_UINT,
      false,   // SU_FORMAT_R8G8_SNORM,
      false,   // SU_FORMAT_R8G8_SINT,
      false,   // SU_FORMAT_R16_TYPELESS,
      false,   // SU_FORMAT_R16_FLOAT,
      false,   // SU_FORMAT_D16_UNORM,
      false,   // SU_FORMAT_R16_UNORM,
      false,   // SU_FORMAT_R16_UINT,
      false,   // SU_FORMAT_R16_SNORM,
      false,   // SU_FORMAT_R16_SINT,
      false,   // SU_FORMAT_R8_TYPELESS,
      false,   // SU_FORMAT_R8_UNORM,
      false,   // SU_FORMAT_R8_UINT,
      false,   // SU_FORMAT_R8_SNORM,
      false,   // SU_FORMAT_R8_SINT,
      true,    // SU_FORMAT_A8_UNORM,
      false,   // SU_FORMAT_R1_UNORM,
      false,   // SU_FORMAT_R9G9B9E5_SHAREDEXP,
      false,   // SU_FORMAT_R8G8_B8G8_UNORM,
      false,   // SU_FORMAT_G8R8_G8B8_UNORM,
      false,    // SU_FORMAT_BC1_TYPELESS,
      false,    // SU_FORMAT_BC1_UNORM,
      false,    // SU_FORMAT_BC1_UNORM_SRGB,
      true,    // SU_FORMAT_BC2_TYPELESS,
      true,    // SU_FORMAT_BC2_UNORM,
      true,    // SU_FORMAT_BC2_UNORM_SRGB,
      true,    // SU_FORMAT_BC3_TYPELESS,
      true,    // SU_FORMAT_BC3_UNORM,
      true,    // SU_FORMAT_BC3_UNORM_SRGB,
      false,    // SU_FORMAT_BC4_TYPELESS,
      false,    // SU_FORMAT_BC4_UNORM,
      false,    // SU_FORMAT_BC4_SNORM,
      false,    // SU_FORMAT_BC5_TYPELESS,
      false,    // SU_FORMAT_BC5_UNORM,
      false,    // SU_FORMAT_BC5_SNORM,
	  false,	// SU_FORMAT_BC7_TYPELESS,
	  false,	// SU_FORMAT_BC7_UNORM,
	  false,	// SU_FORMAT_BC7_UNORM_SRGB,
      false,   // SU_FORMAT_B5G6R5_UNORM,
      true,   // SU_FORMAT_B5G5R5A1_UNORM,
      true,   // SU_FORMAT_B8G8R8A8_UNORM,
      false,   // SU_FORMAT_B8G8R8X8_UNORM,
      false,   // SU_FORMAT_ATITC_RGB,
      true,   // SU_FORMAT_ATITC_RGBA,
      false,    // SU_FORMAT_ETC1,
      true,    // SU_FORMAT_ETC3,
      true,    // SU_FORMAT_ETC5,
      false,   // SU_FORMAT_D24_COMPARISON_S8_UINT,
      false    // SU_FORMAT_D16_COMPARISON,
   };

   return bHasAlpha[eFormat];
}

/// \ingroup GPUResourceManager
/// Returns whether a format is using DXT-style texture compression
inline bool SuIsTextureFormatBlockCompressed( SuGPUResourceFormat eFormat )
{
   switch( eFormat )
   {
   case SU_FORMAT_BC1_TYPELESS:
   case SU_FORMAT_BC1_UNORM:
   case SU_FORMAT_BC1_UNORM_SRGB:
   case SU_FORMAT_BC2_TYPELESS:
   case SU_FORMAT_BC2_UNORM:
   case SU_FORMAT_BC2_UNORM_SRGB:
   case SU_FORMAT_BC3_TYPELESS:
   case SU_FORMAT_BC3_UNORM:
   case SU_FORMAT_BC3_UNORM_SRGB:
   case SU_FORMAT_BC4_TYPELESS:
   case SU_FORMAT_BC4_UNORM:
   case SU_FORMAT_BC4_SNORM:
   case SU_FORMAT_BC5_TYPELESS:
   case SU_FORMAT_BC5_UNORM:
   case SU_FORMAT_BC5_SNORM:
   case SU_FORMAT_BC7_TYPELESS:
   case SU_FORMAT_BC7_UNORM:
   case SU_FORMAT_BC7_UNORM_SRGB:
   case SU_FORMAT_ATITC_RGB:
   case SU_FORMAT_ATITC_RGBA:
   case SU_FORMAT_ETC1:
   case SU_FORMAT_ETC3:
   case SU_FORMAT_ETC5:
      return true;
   }

   return false;
}



/// \ingroup GPUResourceManager
/// A structure to hold information about a mip level
struct SuMipLevelInfo
{
   uint32 nRowPitch;    ///< Distance in bytes between consecutive rows
   uint32 nSlicePitch;  ///< Distance in bytes between consecutive slices
   uint32 nSize;        ///< Size of the mip level data in bytes
   void*  pData;         ///< Pointer to mip level data
};


//=================================================================================================================================
/// Counts the number of mip levels in a fully mipped texture with the specified dimensions
/// \param nWidth   Width of texture
/// \param nHeight  Height of texture
/// \param nDepth   Slice count in a 3D texture (use 1 for non-3D textures)
//=================================================================================================================================
inline uint8 SuComputeMipChainSize( uint32 nWidth, uint32 nHeight, uint32 nDepth )
{
   uint8 nCount=1;
   uint32 nSize =  SuMax( SuMax( nHeight,nDepth ), nWidth );
   
   while( nSize > 1 )
   {
      nSize /= 2;
      nCount++;
   }

   return nCount;
}

//=================================================================================================================================
/// Converts any untyped formats to a default fetchable format that can be used for a sampling view.
///
/// Returns the input format for any fully typed format.
//=================================================================================================================================
inline SuGPUResourceFormat SuGetDefaultFetchableFormat( SuGPUResourceFormat eFormat )
{
   // big ugly lookup table of resource format channel counts
   static const SuGPUResourceFormat eTranslatedFormat[ SU_FORMAT_COUNT ] = {
      SU_FORMAT_UNKNOWN,
      SU_FORMAT_R32G32B32A32_UINT, //SU_FORMAT_R32G32B32A32_TYPELESS,
      SU_FORMAT_R32G32B32A32_FLOAT,
      SU_FORMAT_R32G32B32A32_UINT,
      SU_FORMAT_R32G32B32A32_SINT,
      SU_FORMAT_R32G32B32_UINT, //SU_FORMAT_R32G32B32_TYPELESS,
      SU_FORMAT_R32G32B32_FLOAT,
      SU_FORMAT_R32G32B32_UINT,
      SU_FORMAT_R32G32B32_SINT,
      SU_FORMAT_R16G16B16A16_UNORM, //SU_FORMAT_R16G16B16A16_TYPELESS,
      SU_FORMAT_R16G16B16A16_FLOAT,
      SU_FORMAT_R16G16B16A16_UNORM,
      SU_FORMAT_R16G16B16A16_UINT,
      SU_FORMAT_R16G16B16A16_SNORM,
      SU_FORMAT_R16G16B16A16_SINT,
      SU_FORMAT_R32G32_UINT, //SU_FORMAT_R32G32_TYPELESS,
      SU_FORMAT_R32G32_FLOAT,
      SU_FORMAT_R32G32_UINT,
      SU_FORMAT_R32G32_SINT,
      SU_FORMAT_R32_FLOAT_X8X24_TYPELESS, //SU_FORMAT_R32G8X24_TYPELESS,
      SU_FORMAT_D32_FLOAT_S8X24_UINT,
      SU_FORMAT_R32_FLOAT_X8X24_TYPELESS,
      SU_FORMAT_X32_TYPELESS_G8X24_UINT,
      SU_FORMAT_R10G10B10A2_UNORM, //SU_FORMAT_R10G10B10A2_TYPELESS,
      SU_FORMAT_R10G10B10A2_UNORM,
      SU_FORMAT_R10G10B10A2_UINT,
      SU_FORMAT_R11G11B10_FLOAT,
      SU_FORMAT_R8G8B8A8_UNORM, //SU_FORMAT_R8G8B8A8_TYPELESS,
      SU_FORMAT_R8G8B8A8_UNORM,
      SU_FORMAT_R8G8B8A8_UNORM_SRGB,
      SU_FORMAT_R8G8B8A8_UINT,
      SU_FORMAT_R8G8B8A8_SNORM,
      SU_FORMAT_R8G8B8A8_SINT,
      SU_FORMAT_R16G16_UNORM, //SU_FORMAT_R16G16_TYPELESS,
      SU_FORMAT_R16G16_FLOAT,
      SU_FORMAT_R16G16_UNORM,
      SU_FORMAT_R16G16_UINT,
      SU_FORMAT_R16G16_SNORM,
      SU_FORMAT_R16G16_SINT,
      SU_FORMAT_R32_FLOAT, //SU_FORMAT_R32_TYPELESS,
      SU_FORMAT_D32_FLOAT,
      SU_FORMAT_R32_FLOAT,
      SU_FORMAT_R32_UINT,
      SU_FORMAT_R32_SINT,
      SU_FORMAT_R24_UNORM_X8_TYPELESS, //SU_FORMAT_R24G8_TYPELESS,
      SU_FORMAT_D24_UNORM_S8_UINT,
      SU_FORMAT_R24_UNORM_X8_TYPELESS,
      SU_FORMAT_X24_TYPELESS_G8_UINT,
      SU_FORMAT_R8G8_UNORM, //SU_FORMAT_R8G8_TYPELESS,
      SU_FORMAT_R8G8_UNORM,
      SU_FORMAT_R8G8_UINT,
      SU_FORMAT_R8G8_SNORM,
      SU_FORMAT_R8G8_SINT,
      SU_FORMAT_R16_UNORM, //SU_FORMAT_R16_TYPELESS,
      SU_FORMAT_R16_FLOAT,
      SU_FORMAT_D16_UNORM,
      SU_FORMAT_R16_UNORM,
      SU_FORMAT_R16_UINT,
      SU_FORMAT_R16_SNORM,
      SU_FORMAT_R16_SINT,
      SU_FORMAT_R8_UNORM, //SU_FORMAT_R8_TYPELESS,
      SU_FORMAT_R8_UNORM,
      SU_FORMAT_R8_UINT,
      SU_FORMAT_R8_SNORM,
      SU_FORMAT_R8_SINT,
      SU_FORMAT_A8_UNORM,
      SU_FORMAT_R1_UNORM,
      SU_FORMAT_R9G9B9E5_SHAREDEXP,
      SU_FORMAT_R8G8_B8G8_UNORM,
      SU_FORMAT_G8R8_G8B8_UNORM,
      SU_FORMAT_BC1_UNORM, //SU_FORMAT_BC1_TYPELESS,
      SU_FORMAT_BC1_UNORM,
      SU_FORMAT_BC1_UNORM_SRGB,
      SU_FORMAT_BC2_UNORM, //SU_FORMAT_BC2_TYPELESS,
      SU_FORMAT_BC2_UNORM,
      SU_FORMAT_BC2_UNORM_SRGB,
      SU_FORMAT_BC3_UNORM, //SU_FORMAT_BC3_TYPELESS,
      SU_FORMAT_BC3_UNORM,
      SU_FORMAT_BC3_UNORM_SRGB,
      SU_FORMAT_BC4_UNORM, //SU_FORMAT_BC4_TYPELESS,
      SU_FORMAT_BC4_UNORM,
      SU_FORMAT_BC4_SNORM,
      SU_FORMAT_BC5_UNORM, //SU_FORMAT_BC5_TYPELESS,
      SU_FORMAT_BC5_UNORM,
      SU_FORMAT_BC5_SNORM,
      SU_FORMAT_BC7_UNORM, //SU_FORMAT_BC7_TYPELESS
      SU_FORMAT_BC7_UNORM,
      SU_FORMAT_BC7_UNORM_SRGB,
      SU_FORMAT_B5G6R5_UNORM,
      SU_FORMAT_B5G5R5A1_UNORM,
      SU_FORMAT_B8G8R8A8_UNORM,
      SU_FORMAT_B8G8R8X8_UNORM,
      SU_FORMAT_ATITC_RGB,
      SU_FORMAT_ATITC_RGBA,
      SU_FORMAT_ETC1,
      SU_FORMAT_ETC3,
      SU_FORMAT_ETC5,
      SU_FORMAT_D24_COMPARISON_S8_UINT,
      SU_FORMAT_D16_COMPARISON
   };

   return eTranslatedFormat[ eFormat ];
}

/// \ingroup GPUResourceManager
/// Determines the sizes and locations of each individual mip level in a texture image
SU_DLL bool SuGetMipLevelInfo( uint32 nWidth, uint32 nHeight, uint32 nSlices, uint32 nMipLevels, SuGPUResourceFormat eFormat, 
                        SuMemoryBufferPtr pMemory, uint32 nOffset, SuArray<SuMipLevelInfo>& rLevelsOut );


/// \ingroup GPUResourceManager
/// Computes the full packed size of a texture with the specified format, dimensions, and mip count
SU_DLL uint32 SuComputeTextureSize( uint32 nWidth, uint32 nHeight, uint32 nDepth, uint32 nMipLevels, SuGPUResourceFormat eFormat );

#endif // _SU_GPURESOURCEFORMAT_H_
