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

#ifndef _SU_GPUSTRUCTUREDBUFFER_H_
#define _SU_GPUSTRUCTUREDBUFFER_H_

#include "SuGPUBuffer.h"
#include "SuMemoryBuffer.h"

//=================================================================================================================================
/// \ingroup GPUResourceManager
/// \brief A 'structured' buffer is a special buffer which supports atomic reads and writes of fixed-sized memory locations
///
///  The 'Structured buffer' is a particular type of resource class which is designed to emulate the structured buffer
///     functionality that is offered by DX11.  DX11 structured buffers can be used bound via sampling views or UAVs
///     and exist mainly for use as unordered append buffers.  
///
///  The reason for creating a new class, as opposed to overloading the SuGPUVertexBuffer class, is that structured buffers
///    in DX11 may not be bound to the pipeline as VBs.  
///
///  Structured buffers are fairly restricted compared to other GPU resources:
///    - They can only be bound as shader resources or UAVs
///    - CPU access is not supported for structured buffers.
///    - Behavior is always SuGPUResource::DYNAMIC
/// 
//=================================================================================================================================
// tolua_begin
class SuGPUStructuredBuffer : public SuGPUBuffer
{
public:
// tolua_end

	/// Constructor
	SU_DLL SuGPUStructuredBuffer(uint32 nStructSize, uint32 nStructCount, uint32 nBindFlags, uint32 nMiscHintFlags);

	/// Destructor
	virtual SU_DLL ~SuGPUStructuredBuffer();


// tolua_begin

	/// Can this buffer be used as a vertex buffer?
	virtual SU_DLL bool IsVertexBuffer() const { return false; };

	/// Can this buffer be used as an index buffer?
	virtual SU_DLL bool IsIndexBuffer() const { return false; };

	/// Returns the stride of the structures stored in the buffer (in bytes)
	virtual uint32 GetStructSize() const { return m_nStructSize; };

	/// Returns the number of vertex elements stored in the buffer (in bytes)
	virtual uint32 GetStructCount() const { return m_nStructCount; };

	/// A convenience function which returns a resource view description of the entire resource
	virtual SU_DLL SuGPUResourceDescription GetDefaultResourceDesc();

	/// Add command to readback buffer
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_SOURCE before calling.
	/// 
	/// Data is not ready until GPU finishes executing readback, so CPU/GPU synchronization is also needed.
	virtual SU_DLL bool ReadbackBuffer();

	/// Get memory buffer pointer to the readback data
	/// User is responsible for synchronization
	virtual SU_DLL SuMemoryBufferPtr GetReadbackData();

	virtual SU_DLL SuHandle Map(MapType eMapOption, bool bWaitForGPU = false);

	/// Unmap (and update) the resource that was previously mapped.
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	virtual SU_DLL bool Unmap();

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

// tolua_end

protected:

	/// Performs subclass specific view validation
	virtual SU_DLL bool IsViewValid(const SuGPUResourceViewDescription& rDesc) const;


private:

	uint32 m_nStructSize;
	uint32 m_nStructCount;

// tolua_begin
};
// tolua_end


#endif // _SU_GPUSTRUCTUREDBUFFER_H_
