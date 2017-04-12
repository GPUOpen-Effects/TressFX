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

#ifndef _SU_GPU_VERTEX_BUFFER_H_
#define _SU_GPU_VERTEX_BUFFER_H_

#include "SuGPUBuffer.h"

//---------------------------------------------------------------------------------------------------------------------------------
/// \ingroup GPUResourceManager
/// 
/// \brief A class to represent a buffer which can be bound as a vertex buffer
/// 
/// Objects of this type support all of the buffer usage modes, and, in addition, may be bound as vertex buffers.
/// 
//---------------------------------------------------------------------------------------------------------------------------------
// tolua_begin
class SuGPUVertexBuffer : public SuGPUBuffer
{
public:
// tolua_end

	/// Constructor
	SU_DLL SuGPUVertexBuffer(Behavior eBehavior, uint32 nBindFlags, uint32 nCPUAccessFlags, uint32 nMiscHintFlags,
		uint32 nVertexSize, uint32 nVertexCount);

	/// Destructor
	virtual SU_DLL ~SuGPUVertexBuffer();

	/// Create the actual resource on the GPU based on creation settings
	///
	/// \param  pInitialData  Initialization data for the buffer
	/// 
	/// \return true if creation is succesful, false if any error occurred. See Sushi error log for specific error messages
	virtual bool Create(SuMemoryBufferPtr pInitialData) = 0;

	/// Destroy any actaul resources associated with this GPU resource
	virtual bool Destroy() = 0;

// tolua_begin
	/// Map the buffer resource and get a pointer to buffer memory to modify its contents. 
	virtual SuHandle Map(MapType eMapOption, bool bWaitForGPU = false) = 0;

	/// Unmap (and update) the resource that was previously mapped.
	/// Caller responsible for synchronizing with Transition before and after; transition to STATE_COPY_DEST before calling.
	/// 
	/// \return True if no errors occurred during unmapping, false if trying to unmap a resource that hasn't been created previously
	virtual bool Unmap() = 0;

	/// Update entire buffer.  
	/// Behavior must be GPU_RESOURCE_DYNAMIC.
	/// Caller must transition state to STATE_COPY_DEST.
	virtual SU_DLL bool UpdateBuffer(SuMemoryBuffer* pBuffer);

	/// Update a subrange of the buffer from element index beginIndex to endIndex-1
	/// Behavior must be GPU_RESOURCE_DYNAMIC.
	/// Caller must transition state to STATE_COPY_DEST.
	virtual SU_DLL bool UpdateBufferRange(SuMemoryBuffer* pBuffer, uint32 beginIndex, uint32 endIndex);

	/// Ensure completion of start state by GPU before transition to end state
	virtual SU_DLL bool Transition(StateType startState, StateType endState);

	/// Ensure completion of all previous UAV reads or writes, before subsequent UAV reads or writes occur
	virtual SU_DLL bool UAVBarrier();

	/// Returns the size of each vertex, in bytes
	inline uint32 GetVertexSize() const { return m_nVertexSize; }

	/// Returns the number of vertices that will fit in the buffer
	inline uint32 GetNumVertices() const { return m_nVertexCount; }

	/// Returns the stride of the structures stored in the buffer (in bytes)
	virtual uint32 GetStructSize() const { return m_nVertexSize; }

	/// Returns the number of elements stored in the buffer (in bytes)
	virtual uint32 GetStructCount() const { return m_nVertexCount; }

	/// Overrides the vertex size set at buffer creation time.
	SU_DLL void OverrideVertexSize(uint32 nNewVertexSize);

// tolua_end

	/// Can this buffer be used as a vertex buffer?
	virtual SU_DLL bool IsVertexBuffer() const { return true; };

	/// Can this buffer be used as an index buffer?
	virtual SU_DLL bool IsIndexBuffer() const { return false; };

protected:
	uint32 m_nVertexSize;         ///< Vertex size in bytes
	uint32 m_nVertexCount;        ///< Number of vertices in this vertex buffer
	uint32 m_nSizeInBytes;        ///< vertexSize * vertexCount at buffer creation time

private:

// tolua_begin
}; // End of SuGPUVertexBuffer class declaration
// tolua_end




#endif   // End of _SU_GPU_VERTEX_BUFFER_H_ definition