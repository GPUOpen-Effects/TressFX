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

#ifndef  _SU_GPU_DEPTH_STENCIL_RESOURCE_VIEW_H_
#define  _SU_GPU_DEPTH_STENCIL_RESOURCE_VIEW_H_

#include "SuGPUResourceView.h"

class SuGPUResource;
class SuGPUResourceViewDescription;


/// \brief Depth Stencil Resource View
/// \ingroup GPUResourceManager
/// Encapsulates functionality necessary to create and use a view for a depth stencil surface
// tolua_begin
class SuGPUDepthStencilResourceView: public SuGPUResourceView
{
public:
// tolua_end

   /// Depth / stencil view constructor
   SU_DLL SuGPUDepthStencilResourceView( SuGPUResource* pResource, const SuGPUResourceViewDescription& rDescription );

   /// Depth / stencil view constructor
   SU_DLL SuGPUDepthStencilResourceView( SuGPUResource*           pResource,
                                  SuGPUResourceFormat      eResourceFormat,
                                  SuGPUResource::Type      eResourceType,
                                  const SuGPUResourceDescription& rResourceDesc );

   /// Destructor
   virtual SU_DLL ~SuGPUDepthStencilResourceView();

   /// Create the view based on the existing description
   virtual SU_DLL bool Create();

   /// An entry point to destroy all of the API-specific resources
   virtual SU_DLL bool Destroy();

private:

// tolua_begin
};    // End of SuGPUDepthStencilResourceView class declaration
// tolua_end

#endif   // End of #define _SU_GPU_DEPTH_STENCIL_RESOURCE_VIEW_H_