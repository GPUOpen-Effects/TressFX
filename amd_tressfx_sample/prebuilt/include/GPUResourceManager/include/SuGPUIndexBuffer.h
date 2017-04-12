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

#ifndef _SU_GPU_INDEX_BUFFER_H_
#define _SU_GPU_INDEX_BUFFER_H_

#include "SuGPUBuffer.h"

//---------------------------------------------------------------------------------------------------------------------------------
/// \ingroup GPUResourceManager
/// 
/// \brief A class to represent a buffer which can be bound as a index buffer
/// 
/// Objects of this type support all of the buffer usage modes, and, in addition, may be bound as index buffers.
//---------------------------------------------------------------------------------------------------------------------------------
class SuGPUIndexBuffer : public SuGPUBuffer
{
public:

	/// Constructor
	SU_DLL SuGPUIndexBuffer(Behavior eBehavior, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags, uint32 nIndexSize, uint32 nIndexCount);

	/// Destructor
	virtual SU_DLL ~SuGPUIndexBuffer();

	/// Create the actual resource on the GPU based on creation settings
	///
	/// \param  pInitialData  Initialization data for the buffer
	/// 
	/// \return true if creation is succesful, false if any error occurred. See Sushi error log for specific error messages
	virtual SU_DLL bool Create(SuMemoryBufferPtr pInitialData) = 0;

	/// Destroy any actaul resources associated with this GPU resource
	virtual SU_DLL bool Destroy() = 0;

	/// Map the buffer resource and get a pointer to buffer memory to modify its contents. 
	virtual SU_DLL SuHandle Map(MapType eMapOption, bool bWaitForGPU = false) = 0;

	/// Unmap (and update) the resource that was previously mapped
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	/// 
	/// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
	virtual SU_DLL bool Unmap() = 0;

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

	inline uint32 GetIndexSize()  const { return m_nIndexSize; };
	inline uint32 GetNumIndices() const { return m_nIndexCount; };

	/// Returns the stride of the structures stored in the buffer (in bytes)
	virtual uint32 GetStructSize() const { return m_nIndexSize; };

	/// Returns the number of elements stored in the buffer (in bytes)
	virtual uint32 GetStructCount() const { return m_nIndexCount; };

	/// Can this buffer be used as a vertex buffer?
	virtual SU_DLL bool IsVertexBuffer() const { return false; };

	/// Can this buffer be used as an index buffer?
	virtual SU_DLL bool IsIndexBuffer() const { return true; };

private:

	uint32 m_nIndexSize;       ///< Individual index element size 
	uint32 m_nIndexCount;      ///< Number of indices in this index buffer

}; // End of SuGPUIndexBuffer class declaration


#endif   // End of _SU_GPU_INDEX_BUFFER_H_ definition