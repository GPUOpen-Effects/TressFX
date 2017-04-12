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

#ifndef _SU_GPU_TEXTURE_H_
#define _SU_GPU_TEXTURE_H_

#include "SuGPUResourceViewDescription.h"
#include "SuGPUResource.h"


/// \ingroup GPUResourceManager
/// A struct to hold information about a mapped 2D texture surface.
/// This structure contains a data pointer and a pitch.  Note that the pitch need not equal the byte width of the surface
struct SuMappedSurface
{
   uint32 nRowPitch; ///< Distance between successive rows, in bytes.
   void* pData;      ///< Pointer to the mapped surface data
};

/// \ingroup GPUResourceManager
/// A struct to hold information about a mapped 3D texture
/// This structure contains a data pointer and a row and slice pitch.  Note that the pitches need not equal the byte widths of
/// the surface.
struct SuMappedVolume
{
   uint32 nRowPitch;    ///< Distance between successive rows, in bytes.
   uint32 nSlicePitch;  ///< Distance between successive slices, in bytes.
   void* pData;         ///< Pointer ot the mapped volume
};

//---------------------------------------------------------------------------------------------------------------------------------
/// \brief System-independent representation of a generic GPU texture resource
///
/// \ingroup GPUResourceManager
/// SuGPUTexture provides an abstract interface for system-specific GPU texture resources routines and some common functionality
//---------------------------------------------------------------------------------------------------------------------------------
// tolua_begin
class SuGPUTexture: public SuGPUResource
{
public:

   enum MipType
   {
      MIP_NONE,            ///< Mip-mapping is disabled
      MIP_SPECIFIED,       ///< Mip levels for a given texture resource are computed and specified
      MIP_AUTO_GENERATE    ///< Request to automatically generate mip levels for a given texture resource. Note that this only applies
                           ///< to renderable texture surfaces (i.e. which can be used for render targets (not depth stencil targets)
   }; // End of enum MipType declaration

   /// Constructor
   SU_DLL SuGPUTexture( Behavior eBehavior, Type eType, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags, SuGPUResourceFormat eFormat,
                 uint16 nWidth, uint16 nHeight, MipType eMipType, uint8 nMipLevels, uint8 nSampleCount, uint8 nQualityLevel );

   /// Destructor
   virtual SU_DLL ~SuGPUTexture();
  

   /// Destroy any actual resources associated with this GPU texture resource
   ///
   /// \return true if release of resources is succesful, false if any error occurred. 
   /// See Sushi error log for specific error messages
   virtual bool Destroy() = 0;


   /// Unmap the texture resource that was previously mapped
   ///
   /// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
   virtual bool Unmap() = 0;

   /// Checks whether the given format is supported for a given resource type
   static SU_DLL bool CheckFormatForType( SuGPUResource::Type eType, SuGPUResourceFormat eFormat );
   
   /// Get mip type for this texture resource
   inline const MipType GetMipType() const {     return m_eMipType;    }

   /// Retrieve the width of this texture resource surface for the highest detail level
   inline const uint16 GetWidth() const  {  return m_nWidth;     } 

   /// Retrieve the height of this texture resource surface for the highest detail level
   inline const uint16 GetHeight() const {  return m_nHeight;    } 

   /// Get format of this texture resource
   inline const SuGPUResourceFormat GetFormat() const     {     return m_eFormat;    }

   /// Get the number of mip levels for this texture resource
   inline const uint8 GetNumMipLevels() const      {     return m_nMipLevels;       }

   /// Get number of multisamples per pixel
   inline const uint8 GetSampleCount()  const       {     return m_nSampleCount;     }

   /// Get image quality level
   inline const uint8 GetQualityLevel()    const    {     return m_nQualityLevel;    }

   /// For textures, GetHandle() returns NULL, because it is expected that views will be used
   inline SuHandle GetHandle() const { return 0; };

   /// Returns a depth-stencil view for this resource
   SU_DLL SuGPUDepthStencilResourceViewPtr GetDepthStencilView( const SuGPUResourceViewDescription& rDesc );

   /// A convenience function to compute the size of the resource
   virtual uint32 GetSizeInBytes() const = 0;

   /// A utility method which writes the texture to a DDS file.
   virtual SU_DLL bool SaveToDDS( const SuString& rDDSFileName ) const;

// tolua_end
protected:

   /// Interface for creating an API-specific depth-stencil view
   virtual SuGPUDepthStencilResourceView* CreateDepthStencilView( const SuGPUResourceViewDescription& rDesc ) = 0;


   bool m_bDepthStencil;      ///< This flag is set to true if the texture is used as depth/stencil output. Note that the 
                              /// supported types for that is texture 2D and texture cube

   MipType  m_eMipType;       ///< Describes whether mip levels will be generated or are specified for this resource
                              /// Note that only render targets have the option to generate mip maps automatically, thus only
                              /// 2D textures and cubemaps may have this option enabledx
                              
   SuGPUResourceFormat m_eFormat;   ///< Describes texture data format 

   uint8 m_nMipLevels;        ///< Number of mip levels (default = 0)
 
   uint8 m_nSampleCount;      ///< Number of multisamples per pixel (1 if MSAA is disabled, > 1 otherwise
   uint8 m_nQualityLevel;     ///< Image quality level (0 by default, the higher the number, the higher the quality and the lower the performance)

   uint16 m_nWidth;           ///< Describes the width dimension of the highest detail mip level for this texture resource
   uint16 m_nHeight;          ///< Describes the height dimension of the highest detail mip level for this texture resource

// tolua_begin
}; // End of SuGPUTexture class declaration
// tolua_end
#endif   // End of _SU_GPU_TEXTURE_H_