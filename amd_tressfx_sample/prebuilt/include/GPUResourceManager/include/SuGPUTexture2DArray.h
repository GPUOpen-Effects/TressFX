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

#ifndef _SU_GPU_TEXTURE_2D_ARRAY_H_
#define _SU_GPU_TEXTURE_2D_ARRAY_H_

#include "SuGPUTexture.h"
#include "SuMemoryBuffer.h"
#include "SuImageData2D.h"
#include "SuVector4.h"

// tolua_begin
// repeated typedef for tolua's benefit
class SuImageData2D;
typedef SuCountedPtr<SuImageData2D> SuImageData2DPtr;
// tolua_end

// tolua_begin
class SuGPUTexture2DArrayClearInfo
{
public:
	SuGPUResourceFormat viewFormat;
	SuVector4 color;
	float depth;
	uint8 stencil;
	bool isColor;

	SuGPUTexture2DArrayClearInfo() :
		viewFormat(SU_FORMAT_R8G8B8A8_UNORM), color(0.0f, 0.0f, 0.0f, 1.0f), depth(1.0f), stencil(0), isColor(true) {}

	SuGPUTexture2DArrayClearInfo(
		SuGPUResourceFormat inViewFormat,
		const SuVector4& inColor,
		float inDepth,
		uint8 inStencil,
		bool inIsColor)
	{
		viewFormat = inViewFormat;
		color = inColor;
		depth = inDepth;
		stencil = inStencil;
		isColor = inIsColor;
	}
};
// tolua_end

//---------------------------------------------------------------------------------------------------------------------------------
/// \brief System-independent representation of a generic GPU 2D texture array resource
///
/// \ingroup GPUResourceManager
/// SuGPUTexture2DArray provides an abstract interface for system-specific GPU texture resources routines and some common 
/// functionality for 2D texture arrays. Note that regular texture images are represented as a 2D texture array with size = 1
/// A SuGPUTexture2DArray is a homogeneous array of 2D textures. Each texture in this array has the same data format and 
/// dimensions (including mip levels). 
//---------------------------------------------------------------------------------------------------------------------------------
// tolua_begin
class SuGPUTexture2DArray : public SuGPUTexture
{

public:
// tolua_end

	/// Constructor
	SU_DLL SuGPUTexture2DArray(
		Behavior eBehavior, Type eType, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags,
		SuGPUResourceFormat eFormat, MipType eMipType, uint8 nMipLevels, uint8 nSampleCount,
		uint8 nQualityLevel, uint16 nWidth, uint16 nHeight, uint16 nArraySize);

	/// Destructor
	virtual SU_DLL ~SuGPUTexture2DArray();

	/// Create a new 2D texture resource with the specified parameters
	virtual SU_DLL bool Create(SuMemoryBufferPtr pInitialData, const SuGPUTexture2DArrayClearInfo* pClearInfo = NULL);

	/// Destroy any actual resources associated with this GPU texture resource
	virtual SU_DLL bool Destroy();

	//=================================================================================================================================
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
	/// \param nArrayIndex        The array element to map
	/// \param nMipSlice          The mip level to map
	/// \param pMappedSurface     Pointer to a structure to be filled with information about the mapped surface
	/// \return true on success, false if the surface couldn't be mapped
	//=================================================================================================================================
	virtual SU_DLL bool Map(SuGPUResource::MapType eMapOption, bool bWaitForGPU, uint16 nArrayIndex, uint8 nMipSlice,
		SuMappedSurface* pMappedSurface);

	/// Unmap (and update) the texture resource that was previously mapped.
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	/// 
	/// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
	virtual SU_DLL bool Unmap();

	/// For given array index and mip slice, ensure completion of start state by GPU before transition to end state
	virtual SU_DLL bool Transition(uint16 nArrayIndex, uint8 nMipSlice, StateType startState, StateType endState);

	/// Ensure completion of all previous UAV reads or writes, before subsequent UAV reads or writes occur
	virtual SU_DLL bool UAVBarrier();

// tolua_begin

	virtual SU_DLL uint32 GetNumTilesX() const;
	virtual SU_DLL uint32 GetNumTilesY() const;
	virtual SU_DLL void MapTile(uint32 i, uint32 j, uint32 k);
	virtual SU_DLL void UnmapTile(uint32 i, uint32 j, uint32 k);



	/// Reads back a mip level from the texture.
	/// Caller responsible for synchronizing with Transition before and after.
	/// Transition to STATE_COPY_SOURCE if sample count == 1
	/// Transition to STATE_MULTISAMPLE_RESOLVE_SOURCE if sample count > 1
	/// 
	/// Data is not ready until GPU finishes executing readback, so CPU/GPU synchronization is also needed.
	virtual SU_DLL bool ReadbackMipLevel(uint16 nArrayElement, uint16 nMipSlice);

	/// Copy readback data into SuImageData2D
    /// User is responsible for synchronization
    virtual SU_DLL SuImageData2DPtr GetReadbackImage();

	/// Retrieve the array size of this texture resource
	inline const uint16 GetArraySize() const { return m_nArraySize; }

	/// Copies part of one texture array into another, and performs multi-sample resolve as appropriate
	virtual SU_DLL bool Blit(SuGPUTexture2DArray* pTarget, uint32 nArrayIndex = 0, uint32 nMipSlice = 0,
		uint32 nTargetArrayIndex = 0, uint32 nTargetMipSlice = 0, SuGPUResourceFormat eFormat = SU_FORMAT_UNKNOWN);


	/// A convenience function which returns a sampling view of the entire resource
	virtual SU_DLL SuGPUSamplingResourceViewPtr GetDefaultSamplingView();

	/// A convenience function which returns a sampling view description of the entire resource
	virtual SU_DLL SuGPUResourceDescription GetDefaultResourceDesc();

	/// A convenience function to compute the size of the resource
	virtual SU_DLL uint32 GetSizeInBytes() const;

	/// Generates miplevels for a texture resource
	virtual SU_DLL bool GenerateMips(uint32 nSourceMipLevel, uint32 nNumMipLevels = 0, uint32 nFirstArraySlice = 0,
		uint32 nArraySize = 1) {
		return true;
	}


	/// Update a portion of this texture
	virtual SU_DLL bool Update(uint32 nDestinationMipLevel, uint32 nXOffset, uint32 nYOffset, uint32 nSliceOffset,
		uint32 nWidth, uint32 nHeight, uint32 nSlices, SuMemoryBufferPtr pInitialData);

	// Gets a copy of this texture in a memory buffer
	virtual SU_DLL SuMemoryBufferPtr GetMemoryBuffer(uint32 nSourceMipLevel, uint32 nXOffset, uint32 nYOffset, uint32 nSliceOffset,
		uint32 nWidth, uint32 nHeight, uint32 nSlices);
// tolua_end


protected:

	/// Sub-class specific method to determine whether the resource can support a particular view
	virtual SU_DLL bool IsViewValid(const SuGPUResourceViewDescription& rDesc) const;

	/// Interface for creating an API-specific sampling view
	virtual SU_DLL SuGPUSamplingResourceView* CreateSamplingView(const SuGPUResourceViewDescription& rDesc);

	/// InteSU_DLL rface for creating an API-specific renderable view
	virtual SU_DLL SuGPURenderableResourceView* CreateRenderableView(const SuGPUResourceViewDescription& rDesc);

	/// Interface for creating an API-specific depth-stencil view
	virtual SU_DLL SuGPUDepthStencilResourceView* CreateDepthStencilView(const SuGPUResourceViewDescription& rDesc);

private:

	uint16 m_nArraySize;       ///< Array size. Default value = 1 represents 2D texture images

	uint8** m_ppTextureData;   ///< Actual texture data stored in this texture. 
							   ///< Note that the data should be organized as an array
							   ///< of data blocks, each block encapsulating the entire mip chain for each 
							   ///< individual 2D texture 

	uint32 m_nArraySliceBytes; ///< The size of each individual texture data in bytes

// tolua_begin
}; // End of SuGPUTexture2DArray class declaration
// tolua_end

#endif   // End of _SU_GPU_TEXTURE_2D_ARRAY_H_ declaration