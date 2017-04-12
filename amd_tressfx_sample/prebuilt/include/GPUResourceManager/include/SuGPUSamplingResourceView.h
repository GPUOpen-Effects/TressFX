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

#ifndef  _SU_GPU_SAMPLING_RESOURCE_VIEW_H_
#define  _SU_GPU_SAMPLING_RESOURCE_VIEW_H_

#include "SuGPUResourceView.h"

// Forward declarations:
class SuGPUResource;
class SuGPUResourceViewDescription;


/// \brief Sampling Resource View
/// \ingroup GPUResourceManager
/// Encapsulates functionality necessary to create and use a view for sampling a static resource from any shader stage
class SuGPUSamplingResourceView : public SuGPUResourceView
{
public:

	/// Sampling view constructor
	SU_DLL SuGPUSamplingResourceView(SuGPUResource* pResource, const SuGPUResourceViewDescription& rDescription);

	/// Sampling view constructor
	SU_DLL SuGPUSamplingResourceView(
		SuGPUResource*                  pResource,
		SuGPUResourceFormat             eResourceFormat,
		SuGPUResource::Type             eResourceType,
		const SuGPUResourceDescription& rResourceDesc);

	/// Destructor
	virtual SU_DLL ~SuGPUSamplingResourceView();

	/// Create the view based on the existing description
	virtual SU_DLL bool Create();

	/// An entry point to destroy all of the API-specific resources
	virtual SU_DLL bool Destroy();

};    // End of SuGPUSamplingResourceView class declaration


#endif   // End of #define _SU_GPU_SAMPLING_RESOURCE_VIEW_H_