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

#ifndef _SU_EFFECTRELOADLISTENER_H_
#define _SU_EFFECTRELOADLISTENER_H_

#include "SuCountedPtr.h"

class SuEffect;
typedef SuCountedPtr<SuEffect> SuEffectPtr;

//=================================================================================================================================
/// \ingroup EffectManager
/// \brief An interface for an object which can be notified when an effect is re-loaded
/// 
///  When the effect manager re-loads an effect, it will notify all of the listeners that it knows about, 
///  and signal them to use the new effect in place of the old one.  Listeners should respond by discarding any
///  references to the old effect and using the new one instead.
/// 
//=================================================================================================================================
class SuEffectReloadListener
{
public:

   /// Virtual destructor for reload listeners
   virtual SU_DLL ~SuEffectReloadListener() {};

   //=================================================================================================================================
   /// This method is called to notify the listener that an effect has been re-loaded.  The listener should respond
   ///  by replacing any references to the old effect with the new effect, and updating its internal state accordingly.
   ///
   /// \param pOldEffect
   ///   The old effect
   /// \param pNewEffect
   ///   The new effect, which should replace the old effect
   /// 
   //=================================================================================================================================
   virtual void OnEffectReload( SuEffectPtr pOldEffect, SuEffectPtr pNewEffect ) = 0;

};


#endif // _SU_EFFECTRELOADLISTENER_H_
