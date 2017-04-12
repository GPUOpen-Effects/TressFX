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

#ifndef _SU_GPU_TEXTURE_3D_H_
#define _SU_GPU_TEXTURE_3D_H_

#include "SuGPUTexture.h"
#include "SuImageData2D.h"
//---------------------------------------------------------------------------------------------------------------------------------
/// \brief System-independent representation of a generic GPU 3D texture resource
///
/// \ingroup GPUResourceManager
/// SuGPUTexture3D provides an abstract interface for system-specific GPU texture resources routines and some common 
/// functionality for 3D textures. In a 3D Texture, each mip level is an array of 2D Textures. The biggest and most 
/// detailed mip level has an array size equal to the depth of the 3D Texture
//---------------------------------------------------------------------------------------------------------------------------------
// tolua_begin
class SuGPUTexture3D: public SuGPUTexture
{

public:

// do not expose constructor/destructor to the script.  This way we force the script to go through the GRM interface
// tolua_end

   /// Constructor
   SU_DLL SuGPUTexture3D( Behavior eBehavior, Type eType, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags,
                          SuGPUResourceFormat eFormat, MipType eMipType, uint8 nMipLevels, uint8 nSampleCount, uint8 nQualityLevel, 
                          uint16 nWidth, uint16 nHeight, uint16 nDepth );

   /// Destructor
   virtual SU_DLL ~SuGPUTexture3D();


   /// Create a new 3D texture resource with the specified parameters
   virtual SU_DLL bool Create( SuMemoryBufferPtr pInitialData  );

   /// Destroy any actual resources associated with this GPU texture resource
   virtual SU_DLL bool Destroy();

   /// Map the resource and get a pointer to texture resource memory to modify its contents. 
   virtual SU_DLL bool Map( SuGPUResource::MapType eMapOption, bool bWaitForGPU, uint8 nMipSlice, SuMappedVolume* pMappedVolume );

   /// Unmap the texture resource that was previously mapped.
   /// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
   /// 
   /// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
   virtual SU_DLL bool Unmap();

// tolua_begin

   /// Reads back a mip level from the texture.
   /// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_SOURCE before calling.
   /// 
   /// Data is not ready until GPU finishes executing readback, so CPU/GPU synchronization is also needed.
   virtual SU_DLL bool ReadbackMipLevel(uint16 nMipLevel);

   /// Copy readback slice into SuImageData2D
   /// User is responsible for synchronization
   virtual SU_DLL SuImageData2DPtr GetReadbackImage(uint16 nSlice);

   /// Retrieve the depth of the 3D texture
   inline const uint16 GetDepth() const    {   return m_nDepth;   }

   /// A convenience function which returns a sampling view of the entire resource
   virtual SU_DLL SuGPUSamplingResourceViewPtr GetDefaultSamplingView() ;

   /// A convenience function which returns a resource view description of the entire resource
   virtual SU_DLL SuGPUResourceDescription GetDefaultResourceDesc();

   /// A convenience function to compute the size of the resource
   virtual SU_DLL uint32 GetSizeInBytes() const;

// tolua_end
protected:

   /// Sub-class specific method to determine whether the resource can support a particular view
   virtual SU_DLL bool IsViewValid( const SuGPUResourceViewDescription& rDesc ) const;

private:

   uint16 m_nDepth;           ///< Describes the depth of the texture in elements


// tolua_begin
}; // End of SuGPUTexture3D class declaration
// tolua_end

#endif   // End of _SU_GPU_TEXTURE_3D_H_ declaration