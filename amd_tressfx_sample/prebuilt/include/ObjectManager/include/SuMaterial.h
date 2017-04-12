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

#ifndef _SUMATERIAL_H
#define _SUMATERIAL_H

#include "SuEffect.h"
#include "SuParameterSetting.h"
#include "SuGPUIndexBuffer.h"
#include "SuAnimationSet.h"
#include "SuModelChunk.h"

class SuParameterSetting;
class SuTextureSlot;
struct SuSlotViewPair;


//=================================================================================================================================
/// \brief Material class
///
/// The engine's material class. 
//=================================================================================================================================
// tolua_begin
class SuMaterial
{
public:
   /// Default Constructor
   SU_DLL SuMaterial();

   /// Parameterized Constructor 1
   SU_DLL SuMaterial( const SuString& rName, int id );

   /// Destructor
   virtual SU_DLL ~SuMaterial();

   SU_DLL bool SetFloatParameter( const SuString& rParameterName, const float& rParameterValue ); 
   SU_DLL bool SetFloatVectorParameter( const SuString& rParameterName, const SuVector4& rParameterValue ); 

// tolua_end

   SU_DLL void SetName( const SuString& rName );
   SU_DLL void PrintMaterialInfoToLog();
   SU_DLL void SetEffect( SuEffectPtr pEffect );
   SU_DLL void SetTextureBindings( SuSlotViewArray& rTexBindings );
   SU_DLL const SuSlotViewArray& GetTextureBindings() { return m_textureBindings; }
   SU_DLL void SetParameterSettings( SuParameterBindingList& rParamSettings );
   SU_DLL void Apply();
   SU_DLL void UnApply();
   
   // @materialedit - these functions don't necessarily belong here. Putting these here until 
   // we have a better material model 
   SU_DLL void SetAnimationSet( SuAnimationSet* pAnimations ) { m_pCurrentAnimationSet = pAnimations; }
   SU_DLL void SetAnimationTime( float fAnimationTime ) { m_fAnimationTime = fAnimationTime; }

private:

   SuEffectPtr m_pEffect;                          ///< Pointer to the effect used for rendering
   SuParameterBindingList m_paramSettings;         ///< Effect parameter settings for this material
   SuSlotViewArray m_textureBindings;///< Mapping of texture slots to sampling resource views for this material
   SuAnimationSet* m_pCurrentAnimationSet;
   float m_fAnimationTime;

   SuString m_name;
   int m_id;

// tolua_begin
};
// tolua_end

#endif
