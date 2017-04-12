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

#ifndef _SU_COMMANDLIST_H_
#define _SU_COMMANDLIST_H_

#include "SuLog.h"
#include "SuCountedPtr.h"

//=================================================================================================================================
/// \brief Command list type.
/// 
//=================================================================================================================================

// tolua_begin
class SuCommandList
{
public:
	enum Type
	{
		DIRECT,    ///< A list that supports any command
		BUNDLE,    ///< A bundle of commands to be added to direct list
		COMPUTE,   ///< A list supporting compute and copy commands
		COPY       ///< A list supporting copy commands
	};

	virtual SU_DLL ~SuCommandList() {}
	virtual SU_DLL void Reset() = 0;
	virtual SU_DLL void Close() = 0;
};
// tolua_end

// tolua_begin
typedef SuCountedPtr<SuCommandList> SuCommandListPtr;
// tolua_end

// tolua_begin
class SuCommandListArray
{
public:
	virtual SU_DLL ~SuCommandListArray() {}
	virtual void TakeMutex() = 0;
	virtual void ReleaseMutex() = 0;
	virtual void Append(SuCommandListPtr pList) = 0;
	virtual void Clear() = 0;
	virtual uint32 GetNumCommandLists() = 0;
};
// tolua_end

// tolua_begin
typedef SuCountedPtr<SuCommandListArray> SuCommandListArrayPtr;
// tolua_end

#endif
