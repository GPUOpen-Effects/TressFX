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

#ifndef _SU_INDIRECTCOMMANDSIGNATURE_H_
#define _SU_INDIRECTCOMMANDSIGNATURE_H_

#include "SuCommon.h"
#include "SuArray.h"
#include "SuCountedPtr.h"

/// Signature of an indirect command.
/// After adding all desired arguments call Finalize().
///
/// TODO:
/// Currently the signature supports one draw, draw indexed, or dispatch call.
/// DX12 supports additional arguments to set constants or views, but these 
/// will require changes to the effect system.

// tolua_begin
class SuIndirectCommandSignature
{
public:
// tolua_end
	virtual SU_DLL ~SuIndirectCommandSignature() {}

	// tolua_begin
	virtual SU_DLL void AddDraw();
	virtual SU_DLL void AddDrawIndexed();
	virtual SU_DLL void AddDispatch();
	virtual SU_DLL bool Finalize();
	virtual SU_DLL void Reset();
	virtual SU_DLL uint32 GetCommandSize() const;
	virtual SU_DLL uint32 GetDrawOrDispatchOffset() const;
// tolua_end
protected:
	SU_DLL SuIndirectCommandSignature();

// tolua_begin
};
// tolua_end

// tolua_begin
typedef SuCountedPtr<SuIndirectCommandSignature> SuIndirectCommandSignaturePtr;
// tolua_end

#endif

