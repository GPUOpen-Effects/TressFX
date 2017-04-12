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

#ifndef _SU_CAMERAMANAGER_H_
#define _SU_CAMERAMANAGER_H_

#include "SuTypes.h"
#include "SuHashMap.h"
#include "SuEffectParameter.h"
#include "SuUpdateHandlerFactory.h"

class SuCamera;

//=================================================================================================================================
/// \ingroup ObjectManager 
/// \brief  A class which creates and manages camera objects
/// 
//=================================================================================================================================

// tolua_begin
class SuCameraManager : public SuUpdateHandlerFactory
{
public:

   virtual SU_DLL ~SuCameraManager();

   /// Singleton access
   static SU_DLL SuCameraManager* GetPtr();

   /// Singleton access
   static SU_DLL SuCameraManager& GetRef();

   /// Initializes the camera manager and registers camera-specific app updates
   SU_DLL void Initialize();

   /// Destroys the camera manager and all managed cameras
   static SU_DLL void ShutDown();

   /// Creates a camera.
   SU_DLL SuCamera* CreateCamera( const SuString& rCameraName );

   /// Returns a pointer to the camera with the given name
   SU_DLL SuCamera* GetCameraByName( const SuString& rCameraName ) const;

   /// Returns the name for a camera
   SU_DLL SuString GetCameraName( const SuCamera* pCamera ) const;

   /// Specifies the current camera which objects should use when drawing themselves
   SU_DLL void SetActiveCamera( SuCamera* pCamera );

   /// Returns a pointer to the current camera which objects should use when drawing themselves
   SU_DLL SuCamera* GetActiveCamera( ) const;

   /// Signals the camera manager to destroy the specified camera
   SU_DLL void DestroyCamera( SuCamera* pCamera );

   // tolua_end

   /// Method to access the app update handler for the camera manager
   SU_DLL SuCountedPtr<SuUpdateHandler> GetUpdateHandler( const SuString& rAnnotationName, SuEffectParameter::DataType eType,
                                                          uint32 nArraySize, const SuAnnotationMap* pAnnotations );
 


private:

   static SuCameraManager* ms_pInstance;

   /// Default constructor.  Private to restrict instantiation to singleton access method
   SuCameraManager();

   /// Helper method which locates an existing update handler
   SuUpdateHandlerPtr FindExistingUpdateHandler( const SuString& rCameraName, uint32 nUpdateType );


   /// Map of camera names to cameras
   typedef SuHashMap< SuString, SuCamera*, SuStringHasher, SuHashComparer<SuString> > CameraNameMap;
   CameraNameMap m_cameras;

   /// The 'default' camera which should be used by objects to draw themselves
   SuCamera* m_pActiveCamera;

   
   /// Hash table containing a list of update handlers for each named camera.  The empty string "" indicates handlers for the current active camera
   typedef SuList< SuUpdateHandlerPtr > UpdateHandlerList;
   typedef SuHashMap< SuString, UpdateHandlerList, SuStringHasher, SuHashComparer<SuString> > UpdateHandlerMap;
   UpdateHandlerMap m_updateHandlers;


// tolua_begin
};
// tolua_end


#endif // _SU_CAMERAMANAGER_H_
