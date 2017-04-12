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

#ifndef _SU_MATERIAL_MANAGER_H_
#define _SU_MATERIAL_MANAGER_H_

#include "SuCommon.h"
#include "SuHashMap.h"
#include "SuUpdateHandlerFactory.h"


class SuMaterial;

//=================================================================================================================================
/// \ingroup ObjectManager 
/// \brief  A class which creates and manages material objects
/// 
//=================================================================================================================================

class SuMaterialUpdateHandler : public SuUpdateHandler 
{
public:
   /// Implements SuUpdateHandler
   SU_DLL virtual bool GetUpdateValue( void* pBuffer, uint32 nBufferSize );
};



// tolua_begin
class SuMaterialManager: public SuUpdateHandlerFactory
{
public:
   
   /// Singleton access: reference
   static SU_DLL SuMaterialManager& GetRef();

   /// Singleton access: pointer
   static SU_DLL SuMaterialManager* GetPtr();

   /// Performs any one-time initialization work that is needed by the object manager
   SU_DLL void Initialize();

   /// Destroy the object manager singleton, and all objects that it contains.
   static SU_DLL void ShutDown();

   /// Creates a material.
   SU_DLL SuMaterial* CreateMaterial( const SuString& rMaterialName );

   /// Returns a pointer to the Material with the given name
   SU_DLL SuMaterial* GetMaterialByName( const SuString& rMaterialName ) const;

   /// Returns the name for a Material
   SU_DLL SuString GetMaterialName( const SuMaterial* pMaterial ) const;

   /// Prints a list of materials in the engine
   SU_DLL SuString GetMaterialInfoString() const;

// tolua_end

private:

   /// Constructor.  It is private to restrict instantiation to the singleton access methods.
   SuMaterialManager();

   /// Destructor.  It is private to restrict destruction to the ShutDown method.
   ~SuMaterialManager();

   /// Singleton instance
   static SuMaterialManager* ms_pInstance;

   /// Map of material names to materials
   typedef SuHashMap< SuString, SuMaterial*, SuStringHasher, SuHashComparer<SuString> > MaterialNameMap;
   MaterialNameMap m_Materials;

   int m_idCounter;


   // Start UpdateHandler functionality. This follows the same
   // pattern as in SuObjectManager and SuCameraManager.

public:

   // Materials set their value when they are used.
   SU_DLL void SetCurrentMaterialID(int id) { m_idCurrent = id; }
   SU_DLL int GetCurrentMaterialID() { return m_idCurrent; }

   // Implements SuUpdateHandlerFactory
   SU_DLL virtual SuUpdateHandlerPtr GetUpdateHandler( const SuString& rAppUpdateName, SuEffectParameter::DataType eType,
      uint32 nArraySize, const SuAnnotationMap* pAnnotations );


private:
   int m_idCurrent;
   SuUpdateHandlerPtr m_pUpdateHandler;
 
// tolua_begin
};
//tolua_end


#endif