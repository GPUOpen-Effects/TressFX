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

#ifndef _SU_EFFECTUTILS_H_
#define _SU_EFFECTUTILS_H_

#include "SuGPUSamplingResourceView.h"
#include "SuGPUUnorderedAccessView.h"

typedef SuCountedPtr< SuGPUSamplingResourceView > SuGPUSamplingResourceViewPtr;
typedef SuCountedPtr< SuGPUUnorderedAccessView > SuGPUUnorderedAccessViewPtr;

class SuGPUResource;
class SuAnnotationMap;
class SuUAVSlot;
class SuTextureSlot;


/// Determines whether or not the 'SuCreateTextureSlotView' function will always create the exact same view of a particular resource
SU_DLL bool SuAreViewParametersDynamic( const SuAnnotationMap* pSlotAnnotations );

/// Utility function which creates a resource view of a resource to bind to a texture slot
SU_DLL SuGPUSamplingResourceViewPtr SuCreateTextureSlotView( const SuTextureSlot* pSlot, SuGPUResource* pResource );

/// Utility function which creates a resource view of a resource to bind to a UAV slot
SU_DLL SuGPUUnorderedAccessViewPtr SuCreateUAVSlotView( const SuUAVSlot* pSlot, SuGPUResource* pResource );
#endif // _SU_EFFECTUTILS_H_
