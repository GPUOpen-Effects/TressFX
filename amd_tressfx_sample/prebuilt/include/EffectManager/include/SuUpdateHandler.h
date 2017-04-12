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

#ifndef _SU_APPUPDATEHANDLER_H_
#define _SU_APPUPDATEHANDLER_H_

class SuUpdateHandlerFactory;
class SuAnnotationMap;

//=========================================================================================================
/// \ingroup EffectManager 
/// \brief An interface for an object which retrieves the value of an app-updatable effect parameter
//=========================================================================================================
class SuUpdateHandler
{
public:

   /// Virtual Destructor
   virtual SU_DLL ~SuUpdateHandler() {};

   //=========================================================================================================
   /// Callback method which is used to retrieve the value of an app-updatable parameter.
   ///    \param pBuffer
   ///       A buffer into which the value should be copied.
   ///    \param nBufferSize
   ///       The size of the buffer.  Not more than nBufferSize bytes will be copied.  The effect pass is responsible
   ///       for ensuring that the buffer is large enough to hold the parameter value
   ///    \return True on success, false on ~success
   //=========================================================================================================
   virtual bool GetUpdateValue( void* pBuffer, uint32 nBufferSize ) = 0;


};


#endif // _SU_APPUPDATEHANDLER_H_
