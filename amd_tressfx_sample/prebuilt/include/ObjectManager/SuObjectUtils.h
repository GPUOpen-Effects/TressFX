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

#ifndef _SU_OBJECTUTILS_H_
#define _SU_OBJECTUTILS_H_

#include "SuPoint3.h"
#include "SuCountedPtr.h"

class SuGPUTexture;
class SuGPUSamplingResourceView;
class SuModelDataManifestEntry;
class SuArchive;
class SuTextureSlot;
class SuEffect;
class SuGPUResource;
class SuModelData;

typedef SuCountedPtr<SuGPUTexture> SuGPUTexturePtr;
typedef SuCountedPtr<SuGPUSamplingResourceView> SuGPUSamplingResourceViewPtr;
typedef SuCountedPtr<SuEffect> SuEffectPtr;
typedef SuCountedPtr<SuGPUResource> SuGPUResourcePtr;
typedef SuCountedPtr<SuModelData> SuModelDataPtr;

/// A struct that maps a texture slot to a resource which is bound to it
struct SuSlotViewPair
{
   const SuTextureSlot* first;
   SuGPUSamplingResourceViewPtr second;
};


/// This is a utility function that creates an appropriate GPU resource from texture data stored in an SUO2 file
SU_DLL SuGPUTexturePtr CreateGPUTextureForModelTexture( const SuString& rNamePrefix, const SuArchive* pArchive, const SuModelDataManifestEntry* pEntry );



#endif // _SU_OBJECTUTILS_H_
