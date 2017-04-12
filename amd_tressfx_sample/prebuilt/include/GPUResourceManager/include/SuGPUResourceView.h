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

#ifndef _SU_GPU_RESOURCE_VIEW_H_
#define _SU_GPU_RESOURCE_VIEW_H_

#include "SuGPUResourceViewDescription.h"

// Forward declarations
// tolua_begin
class SuGPUResource;
// tolua_end

/// \ingroup GPUResourceManager
/// \brief An abstract base class for GPU resource view
// tolua_begin
class SuGPUResourceView
{
public:
// tolua_end


   /// Constructor
   SU_DLL SuGPUResourceView();

   /// Constructor
   SU_DLL SuGPUResourceView( SuGPUResource* pResource, const SuGPUResourceViewDescription& rDescription );

   /// Constructor
   SU_DLL SuGPUResourceView( 
	   SuGPUResource*                  pResource,
	   SuGPUResourceViewType           eViewType,
	   SuGPUResourceFormat             eResourceFormat,
	   SuGPUResource::Type             eResourceType,
	   const SuGPUResourceDescription& rResourceDesc);

   /// Destructor
   virtual SU_DLL ~SuGPUResourceView();

   // tolua_begin

   /// Retrieve the resource for which this view is created
   inline const SuGPUResource* GetResource() const  {   return m_pResource;   }  // tolua_export

   /// Retrieve this view's description
   inline const SuGPUResourceViewDescription* GetDescription() const  {     return &m_description;   }

 
   // tolua_end

   /// Retrieve this view's handle.  This handle is 
   inline const SuHandle GetHandle() const  {   return m_hResource;  }  // tolua_export




protected:

   /// Method that is to be used by subclasses to set the API-agnostic handle for this view
   inline void SetHandle( SuHandle hResource ) { m_hResource = hResource; };

private:
   
   SuGPUResourceViewDescription m_description;  ///< Resource view description

   SuGPUResource* m_pResource;                  ///< A GPU resource object linked to this view

   SuHandle       m_hResource;                  ///< An API-agnostic handle to the resource

// tolua_begin
};  // End of class SuGPUResourceView declaration
// tolua_end


#endif   // End of _SU_GPU_RESOURCE_VIEW_H_
