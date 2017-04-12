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

#ifndef _SU_COMMANDQUEUE_H_
#define _SU_COMMANDQUEUE_H_

#include "SuCountedPtr.h"
#include "SuFence.h"
#include "SuCommandList.h"

//=================================================================================================================================
/// \brief Command queue type
//=================================================================================================================================

// tolua_begin
class SuCommandQueue
{
public:
	virtual SU_DLL ~SuCommandQueue() {}

	/// Submit command list on queue for execution.
	virtual SU_DLL void ExecuteCommandList(SuCommandListPtr commandList) = 0;

	/// Submit array of command lists on queue for execution.
	virtual SU_DLL void ExecuteCommandListArray(SuCommandListArrayPtr commandListArray) = 0;

	/// Put a wait for fence on queue.
	virtual SU_DLL void Wait(SuFencePtr pFence, uint64 fenceValue) = 0;

	/// Put a signal of fence on queue.
	virtual SU_DLL void Signal(SuFencePtr pFence, uint64 fenceValue) = 0;

	/// Wait for submitted command queue execution to finish.  
	/// Places a fence signal on queue and waits on CPU until reached.
	virtual SU_DLL void WaitOnExecution() = 0;
};
// tolua_end

// tolua_begin
typedef SuCountedPtr<SuCommandQueue> SuCommandQueuePtr;
// tolua_end

#endif


