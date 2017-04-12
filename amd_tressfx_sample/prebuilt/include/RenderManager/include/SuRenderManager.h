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

#ifndef _SURENDERMANAGER_H
#define _SURENDERMANAGER_H

#include "SuVector4.h"
#include "SuMatrix4.h"

#include "SuGPURenderableResourceView.h"
#include "SuGPUDepthStencilResourceView.h"
#include "SuGPUPerformanceCounter.h"
#include "SuRenderingPredicate.h"
#include "SuCommandList.h"
#include "SuCommandQueue.h"
#include "SuIndirectCommandSignature.h"
#include "SuFrameStats.h"

class SuOcclusionQuery;
class SuSOStatisticsQuery;
class SuGPUTimingQuery;
class SuGPUBuffer;
class SuGPUTimerResults;

// Tolua doesn't parse #includes, so it doesn't figure out that SuGPURenderableResourceViewPtr is the same type
// as SuCountedPtr<SuGPURenderableResourceView> and so on unless you re-declare the typedefs here
// tolua_begin
typedef SuCountedPtr<SuGPURenderableResourceView> SuGPURenderableResourceViewPtr;
typedef SuCountedPtr<SuGPUDepthStencilResourceView> SuGPUDepthStencilResourceViewPtr;
typedef SuCountedPtr<SuCommandList> SuCommandListPtr;
typedef SuCountedPtr<SuCommandListArray> SuCommandListArrayPtr;
typedef SuCountedPtr<SuCommandQueue> SuCommandQueuePtr;
typedef SuCountedPtr<SuFence> SuFencePtr;
typedef SuCountedPtr<SuIndirectCommandSignature> SuIndirectCommandSignaturePtr;
typedef int32 SuGPUTimerHandle;
// tolua_end

/// \defgroup RenderManager Render Manager

//==================================================================================================================================
/// \brief System-independent interface for low-level graphics rendering API 
///
/// \ingroup RenderManager
/// SuRenderManager provides an abstract interface for API-specific
/// low-level rendering commands.  It follows the singleton pattern
/// and instantiates the appropriate SuRenderManager subclass for the
/// platform.
//==================================================================================================================================

// tolua_begin
class SuRenderManager
{
// tolua_end
protected:
	/// Constructor
	SU_DLL SuRenderManager();

public:

	static const int32 MAX_WORKER_COUNT = 32;
	static const int32 MAIN_THREAD_INDEX = -1;

// tolua_begin

	/// Primitive Types
	enum PrimType
	{
		// Standard primitives
		POINT_LIST,
		LINE_LIST,
		LINE_STRIP,
		TRIANGLE_LIST,
		TRIANGLE_STRIP,

		// Adjacenecy primitives
		LINE_LIST_ADJ,
		LINE_STRIP_ADJ,
		TRIANGLE_LIST_ADJ,
		TRIANGLE_STRIP_ADJ,

		// Patch primitives.  PATCH_N,  where N is the control point count
		PATCH_1,
		PATCH_2,
		PATCH_3,
		PATCH_4,
		PATCH_5,
		PATCH_6,
		PATCH_7,
		PATCH_8,
		PATCH_9,
		PATCH_10,
		PATCH_11,
		PATCH_12,
		PATCH_13,
		PATCH_14,
		PATCH_15,
		PATCH_16,
		PATCH_17,
		PATCH_18,
		PATCH_19,
		PATCH_20,
		PATCH_21,
		PATCH_22,
		PATCH_23,
		PATCH_24,
		PATCH_25,
		PATCH_26,
		PATCH_27,
		PATCH_28,
		PATCH_29,
		PATCH_30,
		PATCH_31,
		PATCH_32,

		NUM_PRIM_TYPES
	};

	/// Types for SetRenderTargets   
	enum RenderTargetType
	{
		BACK_BUFFER = 0,
		TEXTURE_2D = 1,
		TEXTURE_CUBEMAP_POS_X = 2,
		TEXTURE_CUBEMAP_NEG_X = 3,
		TEXTURE_CUBEMAP_POS_Y = 4,
		TEXTURE_CUBEMAP_NEG_Y = 5,
		TEXTURE_CUBEMAP_POS_Z = 6,
		TEXTURE_CUBEMAP_NEG_Z = 7,

		TEXTURE_3D = 8, // these are is just here for completeness.  OGL doesn't support them, 
		BUFFER = 9,     // and D3D doesn't care what type of resource the target is

		NUM_RENDER_TARGET_TYPES
	};

	enum CommandQueue
	{
		GRAPHICS_QUEUE = 0,
		NUM_COMMAND_QUEUES
	};

	/// Destructor
	virtual SU_DLL ~SuRenderManager() {}

	/// Singleton access
	///
	/// \return Pointer to render manager instance
	static SU_DLL SuRenderManager* GetPtr();

	/// Singleton access   
	///
	/// \return Reference to render manager instance
	static SU_DLL SuRenderManager& GetRef();

	/// Delete singleton instance
	static SU_DLL void ShutDown();

	/// Begin Scene
	///
	///      This method should be called prior to issuing any rendering calls
	virtual SU_DLL void BeginScene() {}

	/// End Scene
	///
	///      This method should be called after rendering and before presenting
	virtual SU_DLL void EndScene() {}

	/// Call at worker thread initialization to store per-thread index.
	virtual SU_DLL void SetWorkerIndex(int32 workerIndex);

	/// Create fence synchronization object.
	virtual SU_DLL SuFencePtr CreateFence(uint64 initialValue);

	/// Create or recycle empty command list in recording state.
	/// For DIRECT, COMPUTE or COPY types, command list is valid for one frame.
	/// For BUNDLE type, caller has ownership and is responsible for deletion.
	///
	/// \param bSetCurrent  Set the new command list as the current one in use by RenderManager.
	virtual SU_DLL SuCommandListPtr CreateCommandList(SuCommandList::Type type = SuCommandList::DIRECT, bool bSetCurrent = true);

	/// Close the given command list and append to an array to be executed at ExecuteCommandLists.  Must not be a bundle.
	///
	/// \param commandList  Command list to close and append to array
	virtual SU_DLL void CloseAndAppendCommandList(SuCommandListPtr commandList);

	/// Execute a created bundle, which means inserting into current command list.  Current command list should not itself be a bundle.
	///
	/// \param bundle   Bundle to execute.
	virtual SU_DLL void ExecuteBundle(SuCommandListPtr bundle);

	/// Execute the current command list array and clear.
	virtual SU_DLL void ExecuteCommandLists();

	/// Get the command list (direct or bundle) currently in use by RenderManager.
	virtual SU_DLL SuCommandListPtr GetCurrentCommandList();

	/// Set the command list in use by RenderManager.
	virtual SU_DLL void SetCurrentCommandList(SuCommandListPtr commandList);

	/// Create command queue of given type.
	virtual SU_DLL SuCommandQueuePtr CreateCommandQueue(SuCommandList::Type type = SuCommandList::DIRECT);

	// Get command queue used by default for execution and frame synchronization.
	virtual SU_DLL SuCommandQueuePtr GetFrameCommandQueue();

	// Create indirect command signature.
	virtual SU_DLL SuIndirectCommandSignaturePtr CreateIndirectCommandSignature();

	/// Wait for all executed commands on current queue to complete.
	virtual SU_DLL void WaitOnExecution();

	/// Draw indexed primitive
	///
	/// \pre Vertex and index buffers must already be bound
	/// \param primType Type of indexed primitive to draw
	/// \param numVertices Number of vertices affected by the call (required by D3D9)
	/// \param firstVertex First vertex affected by the call (required by D3D9)
	/// \param numIndices Number of indices in the bound index buffer
	/// \param firstIndex First index to start drawing at in the index buffer
	/// \param indexSize Size of each index in bytes (e.g. uint16 = 2, uint32 = 4)
	virtual SU_DLL void DrawIndexed(PrimType primType, uint32 nBaseVertexIndex, uint32 nNumVertices, uint32 nFirstVertex,
		uint32 nNumIndices, uint32 nFirstIndex, uint32 nIndexSize) {}

	/// Draw non-indexed primitive
	///
	/// \param primType  Primitive Type
	/// \param nNumVertices
	/// \param nFirstVertex
	virtual SU_DLL void DrawNonIndexed(PrimType primType, uint32 nNumVertices, uint32 nFirstVertex) {};

	/// Draw primitives using a streaming output buffer
	///
	/// \pre Vertex and index buffers must already be bound
	/// \param primType Type of indexed primitive to draw
	virtual SU_DLL void DrawAuto(PrimType primType) {}

	/// Dispatch call
	///
	/// \param nX   Dimension of thread group array
	/// \param nY   Dimension of thread group array
	/// \param nZ   Dimension of thread group array

	/// Perform a dispatch invocation
	virtual SU_DLL void Dispatch(int32 nX, int32 nY, int32 nZ) { SuLogError("Dispatch calls not supported for this API"); }

	/// Perform an indirect dispatch invocation
	virtual SU_DLL void DispatchIndirect(SuGPUBuffer* pBuffer, uint32 nByteOffset) { SuLogError("Dispatch indirect calls not supported for this API"); }

	/// Sets the tessellation levels to be used for subsequent draw calls
	virtual SU_DLL void SetTessellationLevel(float32 fMin, float32 fMax);

	/// Draw instanced geometry
	/// \param primType              Type of indexed primitive to draw
	/// \param numVertices           Number of vertices affected by the call (required by D3D9)
	/// \param firstVertex           First vertex affected by the call (required by D3D9)
	/// \param numIndices            Number of indices in the bound index buffer
	/// \param firstIndex            First index to start drawing at in the index buffer
	/// \param indexSize             Size of each index in bytes (e.g. uint16 = 2, uint32 = 4)
	/// \param nInstanceCount        Number of instances to draw
	/// \param nFirstInstance        Index of first instance to draw (used as an offset for the instance data)
	virtual SU_DLL void DrawIndexedInstanced(PrimType primType,
		uint32 nBaseVertexIndex,
		uint32 nNumVertices,
		uint32 nFirstVertex,
		uint32 nNumIndices,
		uint32 nFirstIndex,
		uint32 nIndexSize,
		uint32 nInstanceCount,
		uint32 nFirstInstance = 0);

	/// Draw non-indexed instanced geometry
	/// \param primType  Primitive Type
	/// \param nNumVertices
	/// \param nFirstVertex
	/// \param nInstances 
	virtual SU_DLL void DrawNonIndexedInstanced(PrimType ePrimType, uint32 nNumVertices, uint32 nFirstVertex, uint32 nInstances);


	/// Set the viewport - uses coordinate system with (0,0) in upper left x (e.g. D3D-style)
	///
	/// \param nX x-offset of viewport
	/// \param nY y-offset of viewport
	/// \param nWidth width of viewport
	/// \param nHeight height of viewport
	virtual SU_DLL void Viewport(float32 nX, float32 nY, float32 nWidth, float32 nHeight) {}

	/// Returns the current viewport
	///
	/// \param pX  (output) Pointer to a variable to receive the X coordinate.  May NOT be NULL
	/// \param pY  (output) Pointer to a variable to receive the Y coordinate.  May NOT be NULL
	/// \param pWidth (output) Pointer to a variable to receive the width.  May NOT be NULL
	/// \param pHeight (output) Pointer to a variable to receive the height.  May NOT be NULL
	virtual SU_DLL void GetViewport(float32 *pX, float32* pY, float32* pWidth, float32* pHeight) {}

	/// Set the scissor region - uses coordinate system with (0,0) in upper left x (e.g. D3D-style)
	///
	/// \param nX x-offset of scissor
	/// \param nY y-offset of scissor
	/// \param nWidth width of scissor
	/// \param nHeight height of scissor
	virtual SU_DLL void Scissor(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight) {}

	/// Returns the current scissor rectangle.
	///
	/// \param pX  (output) Pointer to a variable to receive the X coordinate.  May NOT be NULL
	/// \param pY  (output) Pointer to a variable to receive the Y coordinate.  May NOT be NULL
	/// \param pWidth (output) Pointer to a variable to receive the width.  May NOT be NULL
	/// \param pHeight (output) Pointer to a variable to receive the height.  May NOT be NULL
	virtual SU_DLL void GetScissor(uint32 *pX, uint32* pY, uint32* pWidth, uint32* pHeight) {}

	/// Sets the viewport and scissor to the same rectangle.  
	SU_DLL void ViewportAndScissor(uint32 nX, uint32 nY, uint32 nWidth, uint32 nHeight);

	/// Set the Clear color
	///
	/// \param rColor 4-channel clear color
	virtual SU_DLL void SetClearColor(const SuVector4& rColor) { m_clearColor = rColor; }

	/// Set the Clear color
	///
	/// \param rColor 4-channel clear color
	virtual SU_DLL void SetClearColor(float32 fR, float32 fG, float32 fB, float32 fA) { m_clearColor = SuVector4(fR, fG, fB, fA); }

	/// Get the Clear color
	inline const SuVector4& GetClearColor() const { return m_clearColor; }

	/// Set the Clear depth value
	///
	/// \param fDepth 32-bit float depth clear value
	virtual SU_DLL void SetClearDepth(float32 fDepth) { m_fClearDepth = fDepth; }

	/// Get the Clear depth
	inline float32 GetClearDepth() const { return m_fClearDepth; }

	/// Set the Clear stencil value
	///
	/// \param nStencil 8-bit stencil clear value
	virtual SU_DLL void SetClearStencil(uint8 nStencil) { m_nClearStencil = nStencil; }

	/// Get the Clear stencil
	inline uint8 GetClearStencil() const { return m_nClearStencil; }

	/// Clear the selected buffers
	///
	/// \param bColor Clear color buffer
	/// \param bDepth Clear depth buffer
	/// \param bStencil Clear stencil buffer
	/// \param nRenderTarget Optional render target to clear, -1 means clear all bound render targets
	virtual SU_DLL void Clear(bool bColor, bool bDepth, bool bStencil, int32 nRenderTarget = -1) {}

	/// Perform a pipeline flush
	virtual SU_DLL void Flush();

	// TOLUA hackery:
	//  Tolua cannot bind arrays if they're passed as pointers, as is the case here.  
	//  It can, however, bind array declarations like the one you see below, which is not valid C++
	//  so, we're going to try and make tolua see one declaration, and make C++ see another one
#if 0
	virtual void SetRenderTargets(uint32 nNumTargets, const SuGPURenderableResourceViewPtr colorViews[nNumTargets],
		const SuGPUDepthStencilResourceViewPtr pDepthView) {}

#endif

// tolua_end

	/// Set the render targets
	///
	/// \param nNumTargets Number of render targets
	/// \param colorViews Array of pointers to render target views (must have at least numTargets number of views)
	/// \param depthHandle Single pointer to depth-stencil view
	virtual SU_DLL void SetRenderTargets(uint32 nNumTargets, SuGPURenderableResourceViewPtr* colorViews, SuGPUDepthStencilResourceViewPtr pDepthView);

	/// Returns all currently bound render targets
	SU_DLL void GetRenderTargets(SuArray<SuGPURenderableResourceViewPtr>* pColorViews, SuGPUDepthStencilResourceViewPtr* ppDepthView);

	/// Returns the number of currently bound render targets
	SU_DLL uint32 GetNumRenderTargets() const { return static_cast<uint32>(m_pColorViews.size()); }

// tolua_begin

	/// Returns a reference to the SuFrameStats object that is maintained by the render manager
	inline SuFrameStats& GetFrameStats() { return m_frameStats; };

	/// Queries whether or not draw calls have been blocked
	/// \return True if draw calls are blocked, false otherwise
	inline bool AreDrawsBlocked() const { return m_bDrawsBlocked; };

	/// Turns draw call blocking on or off
	SU_DLL void SetDrawBlocking(bool bBlock);

	/// Factory method for creating occlusion query objects
	virtual SU_DLL SuOcclusionQuery* CreateOcclusionQuery() const;

	/// Factory method for creating stream out statistics query objects
	virtual SU_DLL SuSOStatisticsQuery* CreateSOStatisticsQuery() const;

	/// Factory method for creating GPU performance counter objects
	virtual SU_DLL SuGPUPerformanceCounter* CreateGPUPerformanceCounter(SuGPUCounterType eType) const;

	/// Factory method for creating rendering rendering predicate objects
	virtual SU_DLL SuRenderingPredicate* CreateRenderingPredicate(SuRenderingPredicateType eType, bool bPredicateHint) const;

	/// Sets a predicate to use for conditional rendering
	virtual SU_DLL void SetPredication(SuRenderingPredicate* pPredicate, bool bPredicateValue = true) const;

	/// Create timer and return handle, valid for current frame only
	virtual SU_DLL SuGPUTimerHandle CreateGPUTimer(const char* name);

	/// Start a timer created this frame (multiple start/stops allowed)
	virtual SU_DLL bool StartGPUTimer(SuGPUTimerHandle timer);

	/// Stop a timer created this frame (multiple start/stops allowed)
	virtual SU_DLL bool StopGPUTimer(SuGPUTimerHandle timer);

	/// Get timer results from last completed frame
	virtual SU_DLL SuGPUTimerResults* GetGPUTimerResults();

// tolua_end

protected:

	/// Helper method to adjust frame stats in response to a draw call
	void IncrementDrawStats(SuRenderManager::PrimType primType, bool bIndexed, uint32 nIndices);

	SuFrameStats m_frameStats;    ///< Frame statistics

private:
	static SuRenderManager* m_pInstance;       // singleton instance
	SuVector4               m_clearColor;
	float32                 m_fClearDepth;
	uint8                   m_nClearStencil;

	bool                    m_bDrawsBlocked;  ///< Flag indicating whether or not draw calls are blocked

	SuArray<SuGPURenderableResourceViewPtr> m_pColorViews;
	SuGPUDepthStencilResourceViewPtr m_pDepthStencilView;

// tolua_begin
};
// tolua_end

#endif // _SURENDERMANAGER_H
