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

#ifndef _SU_GPUTIMERRESULTS_H_
#define _SU_GPUTIMERRESULTS_H_

#include <SuCommon.h>

// tolua_begin
class SuGPUTimerResults
{
public:
	virtual SU_DLL uint32      GetNumTimers() const;
	virtual SU_DLL bool        IsTimedThisFrame(uint32 timerIndex) const;
	virtual SU_DLL const char* GetTimerName(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetFrameTotal(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetMinFrameTotal(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetMaxFrameTotal(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetAvgFrameTotal(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetMinTimespan(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetMaxTimespan(uint32 timerIndex) const;
	virtual SU_DLL SuTime      GetAvgTimespan(uint32 timerIndex) const;
};
// tolua_end

#endif
