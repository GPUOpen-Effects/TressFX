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

#ifndef _SU_COMPUTE_OBJECT_H_
#define _SU_COMPUTE_OBJECT_H_

#include "SuDispatchableObject.h"
#include "SuEffectReloadListener.h"

// For DispatchIndirect
class SuGPUBuffer;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A genereric compute object class, which can dispatch an an arbitrary compute shader
//=================================================================================================================================
// tolua_begin
class SuComputeObject : public SuDispatchableObject, public SuEffectReloadListener
{
public:

   /// Factory method for quad creation
   static SU_DLL SuComputeObject* CreateObject( const SuString& rEffectFileName );

   /// Quad destructor
   SU_DLL ~SuComputeObject();

   virtual SU_DLL bool HasMatchingComputeTechnique( uint32 nProperties, const SuString* pProperties ) ;

   /// Implements the SuDispatchableObject interface.
   virtual SU_DLL bool Dispatch( const SuString& rEffectTechniqueName, int nX, int nY, int nZ );

   /// Perform an indirect dispatch invocation. Not yet part of SuDispatchbleObject.
   virtual SU_DLL bool DispatchIndirect( const SuString& rEffectTechniqueName, SuGPUBuffer* pBuffer, uint32 nByteOffset );

 // tolua_end

   /// Handles an effect re-load
   virtual SU_DLL void OnEffectReload( SuEffectPtr pOldEffect, SuEffectPtr pNewEffect );


private:

   /// Constructor is private to restrict instantiation to CreateObject() factory method
   SuComputeObject();

   /// Tests whether or not an effect is a valid effect for an SuComputeObject
   static bool IsValidComputeEffect( SuEffectPtr pEffect );

   SuEffectPtr m_pEffect;        ///< The effect that is used by this quad

// tolua_begin
};
// tolua_end


#endif // _SU_QUAD_H_
