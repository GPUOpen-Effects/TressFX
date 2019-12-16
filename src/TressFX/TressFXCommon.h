//---------------------------------------------------------------------------------------
// Constant buffer layouts.
//-------------------------------------------------------------------------------------
//
// Copyright (c) 2019 Advanced Micro Devices, Inc. All rights reserved.
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

#ifndef TRESSFXCOMMON_H_
#define TRESSFXCOMMON_H_

#include "AMD_Types.h"

#define TRESSFX_COLLISION_CAPSULES 0
#define TRESSFX_MAX_NUM_COLLISION_CAPSULES 8

#define TRESSFX_SIM_THREAD_GROUP_SIZE 64

namespace AMD
{
#pragma warning(push)
#pragma warning(disable : 4201)  // disable warning C4201: nonstandard extension used : nameless struct/union
        AMD_DECLARE_BASIC_VECTOR_TYPE;
#pragma warning(pop)
}


#ifdef TRESSFX_NON_COPYABLE_MODERN_CPP

class TressFXNonCopyable
{
public:
    TressFXNonCopyable()  = default;
    ~TressFXNonCopyable() = default;

protected:
    TressFXNonCopyable(const TressFXNonCopyable&) = delete;
    void operator=(const TressFXNonCopyable&) = delete;
};

#else

class TressFXNonCopyable
{
public:
    TressFXNonCopyable() {}
    ~TressFXNonCopyable() {}

    TressFXNonCopyable(TressFXNonCopyable const&) = delete;
    TressFXNonCopyable& operator=(TressFXNonCopyable const&) = delete;
};

#endif

/// Computes the minimum
template<class Type>
inline Type TressFXMin(Type a, Type b)
{
    return (a < b) ? a : b;
}

#endif  // TRESSFXCOMMON_H_