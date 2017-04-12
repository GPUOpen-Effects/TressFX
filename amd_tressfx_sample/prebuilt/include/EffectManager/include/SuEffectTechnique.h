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

#ifndef _SUEFFECTTECHNIQUE_H
#define _SUEFFECTTECHNIQUE_H

#include "SuTypes.h"
#include "SuArray.h"

// forward declarations
class SuEffectPass;
class SuEffect;

/// Array with pointers to effect passes
/// \ingroup EffectManager
typedef SuArray<SuEffectPass *> SuEffectPassArray;

//=================================================================================================================================
/// \ingroup EffectManager
/// \brief Shader Technique
/// 
//=================================================================================================================================
class SuEffectTechnique
{
public:
   /// Constructor
   SU_DLL SuEffectTechnique( const SuString& rName, const SuEffect* pParent, const SuEffectTechnique* pBaseTechnique = NULL,
                      bool bIsAbstractBaseTechnique = false, bool bIsComputetechnique = false );

   /// Destructor
   virtual SU_DLL ~SuEffectTechnique();

   /// Returns the technique's name
   inline const SuString& GetName() const { return m_name; }

   /// Returns the technique's parent effect
   inline const SuEffect* GetParent() const { return m_pParent; }

   /// Adds a property string to the technique
   SU_DLL void AddProperty( const SuString& prop );

   /// Returns a vector with all the technique's properties
   inline const SuStringArray& GetProperties() const { return m_properties; }

   /// Returns a pass with a particular name
   SU_DLL const SuEffectPass* GetPass( const SuString& rName ) const;

   /// Returns a non-const pointer to a pass with a particular name
   SU_DLL SuEffectPass* GetPass( const SuString& rName );

   /// Returns a pass with the given index
   SU_DLL const SuEffectPass* GetPass( uint32 nPassIndex ) const;

   /// Returns a non-const pointer to the pass with the given index
   SU_DLL SuEffectPass* GetPass( uint32 nPassIndex );

   /// Returns an array with all the technique's passes
   inline const SuEffectPassArray& GetPasses() const { return m_passes; }

   /// Returns the number of passes in this technique
   SU_DLL uint32 GetNumPasses() const;

   /// Returns true if this technique is used exclusively to derive other techniques, never for rendering
   inline bool IsAbstractBaseTechnique() const { return m_bIsAbstractBaseTechnique; }

   /// Returns true if this technique is used exclusively to derive other techniques, never for rendering
   inline bool IsComputeTechnique() const { return m_bIsComputeTechnique; }

protected:
   friend class SuEffectBuilder;

   /// Adds a new effect pass to this technique
   SU_DLL void AppendPass( SuEffectPass* pPass );

   /// Adds a new override effect pass to this technique
   SU_DLL void AppendOverridePass( uint32 nOverrideID, SuEffectPass* pPass );

   /// Replaces an effect pass with a new one.
   SU_DLL bool ReplaceOverridePass( uint32 nOverrideID, SuEffectPass* pOldPass, SuEffectPass* pNewPass );

   /// Returns a non-const pointer to a pass with a particular name
   SU_DLL SuEffectPass* GetOverridePass( uint32 nOverrideID, const SuString& rName );

private:
   /// Copies the state of a technique to this technique
   void CopyStateFromTechnique( const SuEffectTechnique* pTechnique );

   bool m_bIsAbstractBaseTechnique;     ///< true if this technique is used exclusively to derive other techniques, never for rendering
   bool m_bIsComputeTechnique;          ///< true if this technique is used exclusively for compute shaders

   SuString m_name;
   const SuEffect* m_pParent;

   SuStringArray m_properties;
   
   /// Stores pointers to the main (effect-defined) passes for this technique
   SuEffectPassArray m_passes;         
   
   /// for each override technique, stores array of pointers to auxilliary override passes that need to be spliced in with the 
   /// main rendering passes when rendering this effect with a particular override enabled. The ID of the override technique
   /// acts as an index into this array.
   SuArray< SuEffectPassArray > m_overridePasses;
};

#endif