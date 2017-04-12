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

#ifndef _SU_OBJECTLISTENER_H_
#define _SU_OBJECTLISTENER_H_

class SuObject;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A base class for a listener object which can be notified when things happen to a sushi object
/// 
///  The purpose of the SuObjectListener class is to provide a mechanism by which Sushi plugins may hook into the object system
///   An object listener can be 'attached' to an object by a plugin, and when so attached, it will receive notifications
///   whenever interesting things happen to the object.
///
///  It is assumed that the listener possesses a priori knowledge about the type of objects to which it will be attached.  
///   Because Sushi does not use RTTI, there no way to determine the type of object at runtime.  If the same
///   listener class is to be attached to multiple different SuObject types, then the code attaching the listener
///   must deal with it in some way.
///
//=================================================================================================================================
class SuObjectListener
{
public:

   /// Method which is called when the listener is attached to the object via a call to 'AttachListener'
   virtual SU_DLL void OnAttach( SuObject* pObject ) {};

   /// Method which is called when the listener is detached from the object using a call to 'DetachListener'
   virtual SU_DLL void OnDetach( SuObject* pObject ) {};

   /// Method which is called when an object to which the listener is attached has been destroyed.
   ///  This method will be called by the object's destructor.  It should not make any calls to the object
   virtual SU_DLL void OnDestroy( SuObject* pObject ) {};

   /// Method which is called whenever the state of the object is changed.  Note that this is not only when UpdateObject is called.
   ///  For example, SuAnimatedModel will also call this method whenever the world transform or visibility of an object has been changed
   virtual SU_DLL void OnUpdate( SuObject* pObject, SuTime fTime ) {};

   /// Method which is called whenever the object is reloaded, after reloading is complete
   ///  It is assumed that the listener will respond by completely re-building any 
   ///   internal state which depends on the object
   virtual SU_DLL void OnReload( SuObject* pObject ) {};


};


#endif // _SU_OBJECTLISTENER_H_
