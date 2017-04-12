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

#ifndef _SU_GPU_RESOURCE_VIEW_DESCRIPTION_H_
#define _SU_GPU_RESOURCE_VIEW_DESCRIPTION_H_

#include "SuGPUResource.h"
#include "SuGPUResourceFormats.h"

//....................................................................................................................
/// Describes resource view type 
//....................................................................................................................
// tolua_begin
enum SuGPUResourceViewType
{
	SU_RENDERABLE_VIEW = 0,    ///< A renderable resource view
	SU_DEPTH_STENCIL_VIEW,     ///< This resource will be used for depth / stencil view
	SU_SAMPLING_VIEW,          ///< A sampling resource view
	SU_UNORDERED_ACCESS_VIEW,  ///< A 'UAV' view.  Used for scatter and append buffering
	SU_CONSTANT_BUFFER_VIEW    ///< A constant buffer view
};  // End of enum SuGPUResourceViewType declaration

enum SuGPUResourceViewFlags
{
	SU_READ_ONLY_DEPTH    = 0x1L,     ///< Depth will not be written (only valid for depth-stencil views)
	SU_READ_ONLY_STENCIL  = 0x2L,     ///< Stencil will not be written (only valid for depth-stencil views)
	SU_RAW_VIEW           = 0x4L,     ///< Indicates that this is a raw view
	SU_APPEND_BUFFER_VIEW = 0x8L,     ///< Indicates that this view will be used for an unordered append buffer
	SU_COUNTER_VIEW      = 0x10L,     ///< Adds a counter to the unordered-access-view buffer
};  

enum SuGPUStereoBackBufferEye
{
	SU_STEREO_LEFT       = 0,         ///< Left eye
	SU_STEREO_RIGHT      = 1          ///< Right eye
};  // End of enum SuGPUStereoBackBufferEye declaration

// tolua_end

//....................................................................................................................
/// Description of a buffer for a resource view
//....................................................................................................................
// tolua_begin
struct SuBufferResourceDesc
{
	///< Offset from the beginning of the buffer in elements, where the view's format determines the size of each element
	int32 nElementOffset;

	///< The number of elements to be part of the view
	int32 nElementWidth;

};  // End of SuBufferResourceDesc
// tolua_end

//....................................................................................................................
/// Description of a 2D texture array for a resource view
//....................................................................................................................
// tolua_begin
struct SuTexture2DArrayResourceDesc
{
	uint16 nMipSlice;          ///< The starting mip slice where 0 is biggest and most detailed for viewing

	uint16 nMipLevels;         ///< The number of viewable levels of detail (only used for sampling view)

	uint16 nFirstArraySlice;   ///< The starting array slice from the beginning of the texture

	uint16 nArraySize;         ///< The number of array slices to view from the first array slice

};  // End of SuTexture2DArrayResourceDesc
// tolua_end

//....................................................................................................................
/// Description of a 3D texture for a resource view
//....................................................................................................................
// tolua_begin
struct SuTexture3DResourceDesc
{
	uint16 nMipSlice;       ///< The starting mip slice where 0 is biggest and most detailed

	uint16 nMipLevels;      ///< The number of viewable levels of detail (only used for sampling view)

	uint16 nFirstWSlice;    ///< The starting viewable slice along the w axis

	uint16 nWSize;          ///< The number of viewable slices starting at first w slice

};  // End of SuTexture3DResourceDesc
// tolua_end

//....................................................................................................................
/// Description of a cubemap texture for a resource view
//....................................................................................................................
// tolua_begin
struct SuTextureCubeArrayResourceDesc
{
	uint16 nMipSlice;          ///< The starting mip slice where 0 is biggest and most detailed

	uint16 nMipLevels;         ///< The number of viewable levels of detail (only used for sampling view)

	uint16 nFirstArraySlice;   ///< The starting array slice from the beginning of the texture

	uint16 nArraySize;         ///< The number of array slices to view from the first array slice

};  // End of SuTextureCubeResourceDesc
// tolua_end

//....................................................................................................................
/// Union of values describing a particular resource based on its type
//....................................................................................................................
//
#if 0

// TOLUA HACK:  Tolua won't instantiate SuGPUResourceDescrpition objects properly unless you declare a constructor
// and since you can't have a constructor in a union, we have to make tolua think that the thing is a struct

// tolua_begin
struct SuGPUResourceDescription
 {
    SuGPUResourceDescription() {};
    SuBufferResourceDesc         Buffer;
    SuTexture2DArrayResourceDesc Texture2D;
    SuTexture3DResourceDesc      Texture3D;
    SuTextureCubeArrayResourceDesc    TextureCube;
    uint16 nFlags;

 };
// tolua_end

#endif


struct SuGPUResourceDescription
{

	union {
		SuBufferResourceDesc         Buffer;
		SuTexture2DArrayResourceDesc Texture2D;
		SuTexture3DResourceDesc      Texture3D;
		SuTextureCubeArrayResourceDesc    TextureCube;
	};

	uint16 nFlags; ///< A bitwise combination of flags from the SuGPUResourceViewFlags enum
};


//....................................................................................................................
/// \ingroup GPUResourceManager
/// \brief A class for storing description of a GPU resource view
//....................................................................................................................
// tolua_begin
 class SuGPUResourceViewDescription
{
public:

	/// Constructor
	SU_DLL SuGPUResourceViewDescription();

	/// Constructor
	SU_DLL SuGPUResourceViewDescription(
		SuGPUResourceViewType    eViewType,
		SuGPUResourceFormat      eResourceFormat,
		SuGPUResource::Type        eResourceType,
		const SuGPUResourceDescription& resourceDesc);

	/// Copy constructor
	SU_DLL SuGPUResourceViewDescription(const SuGPUResourceViewDescription& rDescription);

	/// Destructor
	SU_DLL ~SuGPUResourceViewDescription();

	/// Equality comparison
	SU_DLL bool operator==(const SuGPUResourceViewDescription& rDesc) const;

	/// Retrieve resource view type
	inline const SuGPUResourceViewType GetViewType() const { return m_eViewType; }

	/// Retrieve resource format 
	inline const SuGPUResourceFormat GetResourceFormat() const { return m_eFormat; }

	/// Retrieve resource type
	inline const SuGPUResource::Type GetResourceType() const { return m_eResourceType; }

	/// Retrieve resource description
	inline const SuGPUResourceDescription GetResourceDescription() const { return m_resourceDesc; }

// tolua_end
private:

	SuGPUResourceViewType m_eViewType;        ///< Describes the type of view for this resource
	SuGPUResourceFormat   m_eFormat;          ///< Data format described by this view
	SuGPUResource::Type   m_eResourceType;    ///< Resource type description

	SuGPUResourceDescription m_resourceDesc;  ///< Detailed description of each resource for a specific view based on the resource type

// tolua_begin
};  // End of SuGPUResourceViewDescription class declaration
// tolua_end

 struct SuGPUResourceAndViewDescription
 {
	 SuGPUResource* pResource;
	 SuGPUResourceViewDescription viewDesc;
 };

#endif // End of #define _SU_GPU_RESOURCE_VIEW_DESCRIPTION_H_