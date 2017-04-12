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

#ifndef _SU_FENCE_H_
#define _SU_FENCE_H_

#include "SuCountedPtr.h"

//=================================================================================================================================
/// \brief Fence synchronization primitive.
/// Fence has a uint64 value associated with it.
/// Can add request on SuCommandQueue for GPU to set fence value.
/// Can wait for fence value on command queue or CPU.
/// See SuCommandQueue class
//=================================================================================================================================

#include "SuLog.h"

// tolua_begin
class SuFence
{
public:
	virtual ~SuFence() {}

	/// Get latest value
	virtual SU_DLL uint64 GetCompletedValue() { SU_NOT_IMPLEMENTED(); return 0; }

	/// Wait on CPU for value
	virtual SU_DLL bool Wait(uint64 value) { SU_NOT_IMPLEMENTED(); return false; }

	/// Signal value from CPU
	virtual SU_DLL bool Signal(uint64 value) { SU_NOT_IMPLEMENTED(); return false; }
};
// tolua_end

// tolua_begin
typedef SuCountedPtr<SuFence> SuFencePtr;
// tolua_end


#endif
