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

#ifndef  _SU_GPU_RENDERABLE_RESOURCE_VIEW_H_
#define  _SU_GPU_RENDERABLE_RESOURCE_VIEW_H_

#include "SuGPUResourceView.h"

class SuGPUResource;
class SuGPUResourceViewDescription;


/// \brief Renderable Resource View
/// \ingroup GPUResourceManager
/// Encapsulates functionality necessary to create and use a view for a renderable resource
// tolua_begin
class SuGPURenderableResourceView: public SuGPUResourceView
{
public:
// tolua_end

	/// Renderable view constructor
	SU_DLL SuGPURenderableResourceView(SuGPUResource* pResource, const SuGPUResourceViewDescription& rDescription);

	/// Renderable view constructor
	SU_DLL SuGPURenderableResourceView(
		SuGPUResource*                  pResource,
		SuGPUResourceFormat             eResourceFormat,
		SuGPUResource::Type             eResourceType,
		const SuGPUResourceDescription& rResourceDesc);

	/// Destructor
	virtual SU_DLL ~SuGPURenderableResourceView();

	/// Create the view based on the existing description
	virtual SU_DLL bool Create();

	/// An entry point to destroy all of the API-specific resources
	virtual SU_DLL bool Destroy();

	/// Returns the RenderManager rendertarget type value that corresponds to this view.  This is only needed by the OGL RM
	// tolua_begin
	SU_DLL uint32 GetRMTargetType() const;

	/// Set flag indicating that override techniques can be used on this render target
	///
	/// This flag is used by the Sushi engine to enable or disable override techniques (such as wireframe overlay) using
	/// SuEffectManager::ForceDisableOverrideTechniques(...). This functionality is handled in a Lua version of the
	/// SuRenderManager::SetRenderTargets method, which you can find in the file RenderManager.pkg.
	///
	/// \sa AreOverrideTechniquesAllowed, SuEffectManager::ForceDisableOverrideTechniques
	///
	/// \param bAllow Whether to allow use of override techniques on this RT
	void AllowOverrideTechniques(bool bAllow) { m_bAllowOverrideTechniques = bAllow; }

	/// Returns the flag indicating whether override techniques can be used on this render target
	///
	/// \sa AllowOverrideTechniques, SuEffectManager::ForceDisableOverrideTechniques
	///
	/// \return Whether the use of override techniques is allowed on this RT
	bool AreOverrideTechniquesAllowed() const { return m_bAllowOverrideTechniques; }

// tolua_end

private:
	bool m_bAllowOverrideTechniques;

// tolua_begin
};    // End of SuGPURenderableResourceView class declaration
// tolua_end

#endif   // End of #define _SU_GPU_RENDERABLE_RESOURCE_VIEW_H_