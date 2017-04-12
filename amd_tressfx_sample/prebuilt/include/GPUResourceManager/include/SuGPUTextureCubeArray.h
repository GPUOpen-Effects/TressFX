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

#ifndef _SU_GPU_TEXTURE_CUBE_H_
#define _SU_GPU_TEXTURE_CUBE_H_

#include "SuGPUTexture.h"
#include "SuImageData2D.h"

class SuGPUTexture2DArray;

//---------------------------------------------------------------------------------------------------------------------------------
/// \brief System-independent representation of a generic GPU cubemap texture resource
///
/// \ingroup GPUResourceManager
/// SuGPUTextureCube provides an abstract interface for system-specific GPU texture resources routines and some common 
/// functionality for cubemap textures. 
//---------------------------------------------------------------------------------------------------------------------------------
// tolua_begin
class SuGPUTextureCubeArray: public SuGPUTexture
{

public:

   /// Enumeration of the faces in the cube map.  Note that the values are assigned to line up with D3D's face ordering
   enum CubeMapFace
   {
      CUBE_FACE_POSX,
      CUBE_FACE_NEGX,
      CUBE_FACE_POSY,
      CUBE_FACE_NEGY,
      CUBE_FACE_POSZ,
      CUBE_FACE_NEGZ,
      CUBE_FACE_COUNT = 6
   };

// tolua_end

   /// Constructor
   SU_DLL SuGPUTextureCubeArray( Behavior eBehavior, Type eType, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags,
                                 SuGPUResourceFormat eFormat, MipType eMipType, uint8 nMipLevels, uint8 nSampleCount,
                                 uint8 nQualityLevel, uint16 nEdgeWidth, uint16 nArraySize );

   /// Destructor
   virtual SU_DLL ~SuGPUTextureCubeArray();

   /// Create a new 3D texture resource with the specified parameters
   virtual SU_DLL bool Create( SuMemoryBufferPtr pInitialData );

   /// Destroy any actual resources associated with this GPU texture resource
   virtual SU_DLL bool Destroy();


   /// Map the resource and get a pointer to resource memory to modify its contents. Note that only the resources 
   /// that are created with appropriate CPU access flags will allow mapping with corresponding options (see MapType
   /// declaration for detailed description of correspondence).
   /// 
   /// \param eMapOption         Specifies read and write permissions for mapping this resource. Must match the CPU
   ///                           access options specified at resource creation time
   /// \param bWaitForGPU        Specifies whether need to wait to synchronize with the GPU ( = true) or not. Normally if contention exists 
   ///                           between the GPU and the CPU (detected during the call to Map), the CPU thread will have to block 
   ///                           and wait for the GPU to finish. This flag changes the typical behavior when contention is detected. 
   ///                           Note that this flag cannot be used with MAP_WRITE_DISCARD or MAP_WRITE_NOOVERWRITE.
   ///                           Defaults to false (i.e. don't synchronize)
   /// \param eFace              The cube map face to map
   /// \param nArrayIndex        The array slice to map
   /// \param nMipSlice          The mip level to map
   /// \param pMappedSurface     Pointer to a structure to be filled with information about the mapped surface
   /// \return true on success, false on error
   virtual SU_DLL bool Map( SuGPUResource::MapType eMapOption, bool bWaitForGPU, CubeMapFace eFace, uint16 nArrayIndex,
                            uint8 nMipSlice, SuMappedSurface* pMappedSurface );

   /// Unmap (and update) the texture resource that was previously mapped.
   /// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
   /// 
   /// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
   virtual SU_DLL bool Unmap();

// tolua_begin

   /// Retrieve the array size of this texture resource
   inline const uint16 GetArraySize() const    {   return m_nArraySize;   } 

   /// Copies part of one texture array into another, and performs multi-sample resolve as appropriate
   virtual SU_DLL bool Blit( SuGPUTextureCubeArray* pTarget, uint32 nArrayIndex=0, uint32 nMipSlice=0, 
                      uint32 nTargetArrayIndex=0, uint32 nTargetMipSlice=0, SuGPUResourceFormat eFormat=SU_FORMAT_UNKNOWN );

   /// A convenience function which returns a sampling view of the entire resource
   virtual SU_DLL SuGPUSamplingResourceViewPtr GetDefaultSamplingView();

   /// A convenience function which returns a resource view description of the entire resource
   virtual SU_DLL SuGPUResourceDescription GetDefaultResourceDesc();

   /// Reads back a specific mip level from the cube map
   virtual SU_DLL SuImageData2DPtr ReadMipLevel( uint16 nArrayIndex, CubeMapFace nFace, uint16 nMipLevel );

   /// A convenience function to compute the size of the resource
   virtual SU_DLL uint32 GetSizeInBytes() const;

   /// Generates miplevels for a texture resource
   virtual SU_DLL bool GenerateMips( uint32 nSourceMipLevel, uint32 nNumMipLevels = 0, uint32 nFirstArraySlice = 0,
                                     uint32 nArraySize = 1 ) { return true; }

   /// Copies a cube map into a texture2D array.  This is only supported under D3D10 
   virtual SU_DLL bool CopyToArray( SuGPUTexture2DArray* pArray );

// tolua_end
protected:

   /// Sub-class specific method to determine whether the resource can support a particular view
   virtual SU_DLL bool IsViewValid( const SuGPUResourceViewDescription& rDesc ) const;

   /// Interface for creating an API-specific sampling view
   virtual SU_DLL SuGPUSamplingResourceView* CreateSamplingView( const SuGPUResourceViewDescription& rDesc ) ;
   
   /// Interface for creating an API-specific renderable view
   virtual SU_DLL SuGPURenderableResourceView* CreateRenderableView( const SuGPUResourceViewDescription& rDesc ) ;

   /// Interface for creating an API-specific depth-stencil view
   virtual SU_DLL SuGPUDepthStencilResourceView* CreateDepthStencilView( const SuGPUResourceViewDescription& rDesc ) ;

private:

   uint16 m_nArraySize;       ///< Array size. Default value = 1 represents a simple cube map texture
   uint8* m_pTextureData;     ///< Actual texture data stored in this texture, stores the entire cubemap texture and its mip chain
   uint32 m_nTextureBytes;    ///< The size of the texture data in bytes

// tolua_begin
}; // End of SuGPUTextureCubeArray class declaration
// tolua_end

#endif   // End of _SU_GPU_TEXTURE_CUBE_H_