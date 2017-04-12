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

#ifndef _SU_SAMPLER_H_
#define _SU_SAMPLER_H_

#include "SuTypes.h"
#include "SuAnnotationMap.h"
#include "SuVector4.h"
#include "SuRenderStateEnums.h"

// forward declarations
class SuEffect;
class SuTextureSlot;

/// \brief Sampler declaration
///
/// \ingroup EffectManager
class SuSampler
{
public:
   /// Constructor
   SU_DLL SuSampler( const SuString& rName, const SuEffect* pParent, SuAnnotationMap* pAnnotations = NULL );

   /// Destructor
   virtual SU_DLL ~SuSampler();

   /// Returns the sampler name
   inline const SuString& GetName() const { return m_name; }

   /// Returns the parent effect
   inline const SuEffect* GetParent() const { return m_pParent; }

   /// Returns the sampler annotations
   inline const SuAnnotationMap* GetAnnotations () const { return m_pAnnotations; }

   /// Return the texture for this sampler
   inline const SuTextureSlot* GetTextureSlot () const { return m_pTexture; }

   /// Sets the texture for this sampler to use
   virtual SU_DLL void AddTexture( const SuTextureSlot* pTexture );

   /// Sets a sampler state that takes a bool parameter
   virtual bool AddSamplerState( SuRenderStateId stateID, bool bEnabled ) = 0;

   /// Sets a sampler state that takes an int parameter
   virtual bool AddSamplerState( SuRenderStateId stateID, int32 nValue ) = 0;

   /// Sets a sampler state that takes a uint or enum parameter
   virtual bool AddSamplerState( SuRenderStateId stateID, SuRenderStateValue value ) = 0;
   
   /// Sets a sampler state that takes a float parameter
   virtual bool AddSamplerState( SuRenderStateId stateID, float32 value ) = 0;

   /// Sets a sampler state that takes a float parameter
   virtual bool AddSamplerState( SuRenderStateId stateID, const SuVector4& rValue ) = 0;

   /// Creates API-specific resources associated with this sampler, if necessary:
   virtual SU_DLL bool CreateResources();

private:
   SuString m_name;
   const SuEffect* m_pParent;
   const SuTextureSlot* m_pTexture;
   SuAnnotationMap* m_pAnnotations;
};

#endif   // End of _SU_SAMPLER_H_
