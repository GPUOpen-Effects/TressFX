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

#ifndef _SU_GPU_BUFFER_H_
#define _SU_GPU_BUFFER_H_

#include "SuGPUResource.h"
#include "SuMemoryBuffer.h"

//---------------------------------------------------------------------------------------------------------------------------------
/// \ingroup GPUResourceManager
/// \brief A base class for GPU buffer resources.
///
/// A GPU buffer resource is a block of GPU memory which is used as a vertex buffer, index buffer, or
/// generic buffer.  All buffers (including vertex and index buffers) can support any combination of the 
/// following three usage modes:
///    - Renderable buffers support the creation of render target views
///    - Shader resource buffers may be accessed from shaders using load instructions
///    - Stream out buffers may be used as stream output targets. 
//---------------------------------------------------------------------------------------------------------------------------------
// tolua_begin
class SuGPUBuffer : public SuGPUResource
{
public:
	/// Constructor
	SU_DLL SuGPUBuffer(SuGPUResource::Behavior eBehavior, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags, uint32 nSizeInBytes);

	/// Destructor
	virtual SU_DLL ~SuGPUBuffer();

	/// Can this buffer be used as a vertex buffer?
	virtual bool IsVertexBuffer() const = 0;

	/// Can this buffer be used as an index buffer?
	virtual bool IsIndexBuffer() const = 0;

	/// Returns the buffer size in bytes
	inline uint32 GetBufferSize() const { return m_nBufferSize; };

	/// Returns the stride of the structures stored in the buffer (in bytes)
	virtual uint32 GetStructSize() const { return 1; }

	/// Returns the number of elements stored in the buffer (in bytes)
	virtual uint32 GetStructCount() const { return m_nBufferSize; }

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
	virtual SuHandle Map(MapType eMapOption, bool bWaitForGPU = false);

	/// Map a subrange of the buffer for update, from element index beginIndex to endIndex-1
	/// Behavior must be GPU_RESOURCE_DYNAMIC.
	virtual SuHandle MapRange(uint32 beginIndex, uint32 endIndex);

	/// Unmap (and update) the resource that was previously mapped
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	///
	/// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
	virtual bool Unmap();

	/// Update entire buffer.  
	/// Behavior must be GPU_RESOURCE_DYNAMIC.
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	virtual SU_DLL bool UpdateBuffer(SuMemoryBuffer* pBuffer);

	/// Update a subrange of the buffer from element index beginIndex to endIndex-1
	/// Behavior must be GPU_RESOURCE_DYNAMIC.
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	virtual SU_DLL bool UpdateBufferRange(SuMemoryBuffer* pBuffer, uint32 beginIndex, uint32 endIndex);

	/// Ensure completion of start state by GPU before transition to end state
	virtual SU_DLL bool Transition(StateType startState, StateType endState);

	/// Ensure completion of all previous UAV reads or writes, before subsequent UAV reads or writes occur
	virtual SU_DLL bool UAVBarrier();

	/// Copy buffer region
	virtual SU_DLL bool CopyBufferRegion(uint64 nDstByteOffset, SuGPUBuffer& srcBuffer, uint64 nSrcBufferOffset, uint64 nNumBytes);

// tolua_end

	/// A convenience function which returns a sampling view of the entire resource
	virtual SU_DLL SuGPUSamplingResourceViewPtr GetDefaultSamplingView();

	/// A convenience function which returns a resource view description of the entire resource
	virtual SU_DLL SuGPUResourceDescription GetDefaultResourceDesc();

protected:

	/// Performs subclass specific view validation
	virtual SU_DLL bool IsViewValid(const SuGPUResourceViewDescription& rDesc) const;

	/// Sub-class specific method to construct a sampling view with the specified description.  
	virtual SU_DLL SuGPUSamplingResourceView* CreateSamplingView(const SuGPUResourceViewDescription& rDesc);

	/// Interface for creating an API-specific renderable view
	virtual SU_DLL SuGPURenderableResourceView* CreateRenderableView(const SuGPUResourceViewDescription& rDesc);

	uint32 m_nBufferSize;      ///< Describes the size of this buffer in bytes

private:

// tolua_begin
}; // End of SuGPUBuffer class declaration
// tolua_end

#endif   // End of _SU_GPU_BUFFER_H_ definition