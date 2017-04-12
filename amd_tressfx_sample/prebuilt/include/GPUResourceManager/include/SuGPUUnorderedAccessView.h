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

#ifndef _SU_GPUUNORDEREDACCESSVIEW_H_
#define _SU_GPUUNORDEREDACCESSVIEW_H_

#include "SuGPUResourceView.h"

class SuGPUResource;
class SuGPUResourceViewDescription;

//=================================================================================================================================
/// \ingroup GPUResourceManager
/// \brief Base class for unordered access views
/// 
/// An Unordered Access View is a view type which allows scatter writes.  For buffer resources, UnorderedAccessViews are also
///   used for 'append' buffer resources.  Sushi support for these features closely mirrors the DX11 API
///
//=================================================================================================================================
// tolua_begin
class SuGPUUnorderedAccessView : public SuGPUResourceView
{
public:

   /// Constructor for unordered access views
   SU_DLL SuGPUUnorderedAccessView( SuGPUResource* pResource, const SuGPUResourceViewDescription& rDesc );

   /// Set flag indicating that override techniques can be used on this render target
   ///
   /// This flag is used by the Sushi engine to enable or disable override techniques (such as wireframe overlay) using
   /// SuEffectManager::ForceDisableOverrideTechniques(...). This functionality is handled in a Lua version of the
   /// SuRenderManager::SetRenderTargets method, which you can find in the file RenderManager.pkg.
   ///
   /// \sa AreOverrideTechniquesAllowed, SuEffectManager::ForceDisableOverrideTechniques
   ///
   /// \param bAllow Whether to allow use of override techniques on this RT
   SU_DLL void AllowOverrideTechniques( bool bAllow ) { m_bAllowOverrideTechniques = bAllow; }

   /// Returns the flag indicating whether override techniques can be used on this render target
   ///
   /// \sa AllowOverrideTechniques, SuEffectManager::ForceDisableOverrideTechniques
   ///
   /// \return Whether the use of override techniques is allowed on this RT
   SU_DLL bool AreOverrideTechniquesAllowed() const { return m_bAllowOverrideTechniques; }

   /// Set the intial count for the UAV. This count will be used the very next time this UAV is bound.
   /// A value of -1 indicates that this UAV's current counter should be preserved when it is bound.
   /// Each time the UAV is bound, the intial count is querried and then implicitly set to -1. This
   /// way the UAV will preserve its counter on subsequent bind events unless the user explicitly
   /// calls this function again.
   SU_DLL void SetInitialCount ( uint32 nInitialCount ) { m_nInitialCount = nInitialCount; m_bIsFirstRun = true;}

   /// Get the intial count for the UAV. This count will be used the very next time this UAV is bound.
   /// A value of -1 indicates that this UAV's current counter should be preserved when it is bound.
   SU_DLL uint32 GetInitialCount () const  { return m_nInitialCount; }

   SU_DLL bool GetIsFirstRun () const { return m_bIsFirstRun; }

   SU_DLL void SetIsFirstRun ( bool bIsFirstRun ) { m_bIsFirstRun = bIsFirstRun; }


   /// Clear the UAV to a uint32 value
   virtual SU_DLL void ClearUInt ( const uint32 pValue[4] ) {}

   /// Clear the UAV to a float value
   virtual SU_DLL void ClearFloat ( const float32 pValue[4] ) {}

// tolua_end

private:

   bool  m_bAllowOverrideTechniques;
   uint32 m_nInitialCount;
   bool  m_bIsFirstRun;

// tolua_begin
};
// tolua_end

typedef SuCountedPtr< SuGPUUnorderedAccessView > SuGPUUnorderedAccessViewPtr;


#endif // _SU_GPUUNORDEREDACCESSVIEW_H_
