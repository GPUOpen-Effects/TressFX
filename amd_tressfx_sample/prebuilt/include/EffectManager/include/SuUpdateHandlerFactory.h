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

#ifndef _SU_UPDATEHANDLERFACTORY_H_
#define _SU_UPDATEHANDLERFACTORY_H_

#include "SuAnnotationMap.h"
#include "SuCountedPtr.h"
#include "SuEffectParameter.h"

class SuUpdateHandler;


typedef SuCountedPtr<SuUpdateHandler> SuUpdateHandlerPtr;

//=========================================================================================================
/// \ingroup EffectManager
/// \brief An interface for an object which constructs app-update handlers
/// 
//=========================================================================================================
class SuUpdateHandlerFactory
{
public:

   //=========================================================================================================
   /// Method for creating update handlers for effect parameters.  
   ///    This method is called to create an app-update handler from a set of annotations.  The caller is responsible for
   ///     calling Release() on the returned handler when finished with it.
   /// 
   ///     \param rAppUpdateName
   ///       The name of the AppUpdate annotation for which we are creating the handler.  For example, if the
   ///       effect parameter was declared as:
   ///          <i> float foo <AppUpdate = bar> </i> 
   ///             Then this string would be "bar"
   ///    \param eType
   ///       The data type of the effect parameter
   ///    \param pAnnotations
   ///       The full annotation map.  This map may contain additional annotations that are needed to create the update handler
   ///    \return A ref-counted pointer to an update handler to use to update this parameter value.  NULL if there is a type mismatch
   //=========================================================================================================

   virtual SuUpdateHandlerPtr GetUpdateHandler( const SuString& rAppUpdateName, SuEffectParameter::DataType eType,
                                                uint32 nArraySize, const SuAnnotationMap* pAnnotations ) = 0;

};


#endif // _SU_UPDATEHANDLERFACTORY_H_
