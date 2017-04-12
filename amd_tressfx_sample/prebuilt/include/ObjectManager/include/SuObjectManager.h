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

#ifndef _SU_OBJECT_MANAGER_H_
#define _SU_OBJECT_MANAGER_H_

#include "SuCommon.h"
#include "SuHashMap.h"
#include "SuSearchPath.h"

#include "SuEffectReloadListener.h"
#include "SuUpdateHandlerFactory.h"

#include "SuDefaultParticleBufferManager.h"

/// \defgroup ObjectManager Object Manager

class SuModelData;
class SuModel;

class SuCamera;
class SuObject;
class SuAnimatedModel;
class SuAnimationSet;
class SuParticleGroup;

typedef SuCountedPtr<SuParticleGroup> SuParticleGroupPtr;
typedef SuCountedPtr<SuAnimationSet>  SuAnimationSetPtr;

// tolua_begin

/// \ingroup ObjectManager
/// \brief This is the object manager
class SuObjectManager : public SuUpdateHandlerFactory, public SuEffectReloadListener
{
public:
   
   /// Singleton access: reference
   static SU_DLL SuObjectManager& GetRef();

   /// Singleton access: pointer
   static SU_DLL SuObjectManager* GetPtr();

   /// Performs any one-time initialization work that is needed by the object manager
   SU_DLL void Initialize();

   /// Destroy the object manager singleton, and all objects that it contains.
   static SU_DLL void ShutDown();

   /// Method to find an object with a given name.
   SU_DLL SuObject* GetObjectByName( const SuString& rName );

   /// Creates an animated model
   SU_DLL SuAnimatedModel* CreateAnimatedModel( const SuString& rObjectName, const SuString& rArchiveFile, const SuString& rAnimationFile );

   /// Creates a particle group
   SU_DLL SuParticleGroup* CreateParticleGroup( const SuString& rGroupName, const SuString& rGroupArchive,
                                         const SuString& rAnimationFile,
                                         SuParticleBufferManager* pBufferMgr = &SuObjectManager::ms_defaultParticleBufferMgr );

   /// Adds a directory to the object file search path
   SU_DLL void AddSearchPath( const SuString& rSearchPath );

   /// Re-loads objects
   SU_DLL void ReloadObjects( bool bForce );

   /// Signals the object manager to begin tracking all objects that are drawn
   SU_DLL void StartDrawTracking();

   /// Signals the object manager to stop tracking objects, and to print a list of drawn objects
   SU_DLL SuString EndDrawTracking();

   /// Performs GPU cache warming for all loaded objects
   SU_DLL void WarmGPUCaches();

   // TODO:  Object reloading support for plugin objects

   /// Registers an object that was created by a sushi plugin, and transfers ownership to the object manager
   SU_DLL bool RegisterPluginObject( SuObject* pObject );

   // Don't expose this to the script
// tolua_end

   /// Method to access the app update handler for the object manager
   SU_DLL SuCountedPtr<SuUpdateHandler> GetUpdateHandler( const SuString& rAnnotationName, SuEffectParameter::DataType eType,
                                                   uint32 nArraySize, const SuAnnotationMap* pAnnotations );

   /// Notifies the OM that an object is being drawn.  
   SU_DLL void SetObjectBeingDrawn( SuObject* pObject );

   /// Returns the object that is currently being drawn.  This method is required for use by app-update handlers
   inline SuObject* GetObjectBeingDrawn() const { return m_pDrawingObject; };

   // implementation of the SuEffectReloadListener interface

   /// Called in the event of an effect reload
   virtual void OnEffectReload( SuEffectPtr pOldEffect, SuEffectPtr pNewEffect );

   /// Looks up a path in the object manager's search path
   SU_DLL bool LookupObjectPath( const SuString& rPath, SuString* pFullPathOut );

   // WARNING: This *should* be private, but then poor tolua gets confused when creating the bindings, so it's public instead.
   // Don't get any stupid ideas just because of that...
   static SU_DLL SuDefaultParticleBufferManager ms_defaultParticleBufferMgr;


private:

   /// Constructor.  It is private to restrict instantiation to the singleton access methods.
   SuObjectManager();

   /// Destructor.  It is private to restrict destruction to the ShutDown method.
   ~SuObjectManager();

   /// Constructs a model object from an archive file. 
   SuModel* CreateModel( const SuString& rArchiveFileName );

   /// Construct an animation set from an archive file.
   SuAnimationSetPtr CreateAnimationSet( const SuString& rArchiveFileName );

   /// Constructs an update handler for a particular app-update type
   SuUpdateHandlerPtr CreateUpdateHandler( const SuString& rAppUpdateName );

   /// A mapping of object names to objects
   typedef SuHashMap< SuString, SuObject*, SuStringHasher, SuHashComparer<SuString> > ObjectMap;
   ObjectMap m_objects;

   /// A hashmap identifying the objects in m_objects which are SuAnimatedModels.  These get some special treatment
   typedef SuHashMap< SuObject*, SuAnimatedModel*, SuPointerHasher<SuObject>, SuHashComparer<SuObject*> > AnimatedModelMap;
   AnimatedModelMap m_animatedModels;


   struct SuModelInfo
   {
      SuModel* pModel;
      time_t   nTimeStamp;
   };

   /// A mapping of model archive names to loaded model objects
   typedef SuHashMap< SuString, SuModelInfo, SuStringHasher, SuHashComparer<SuString> > ModelMap;
   ModelMap m_models;


   /// A struct which holds information about each animation set that the object manager owns
   struct SuAnimationSetInfo
   {
      SuAnimationSetPtr pSet;
      time_t            nTimeStamp;
   };

   /// A mapping of animation set file names to loaded animation sets
   typedef SuHashMap< SuString, SuAnimationSetInfo, SuStringHasher, SuHashComparer<SuString> > AnimationSetMap;
   AnimationSetMap m_animationSets;

   struct SuParticleGroupInfo
   {
      SuParticleGroup*  pParticleGroup;
      SuString          fullPath;      ///< Full path of particle file from whence this group came
      SuAnimationSetPtr pAnimations;   ///< The animation set from which this group pulled its animations.  Needed for re-loading to work
      time_t   nTimeStamp;
   };

   /// A mapping of particle group archive names to loaded particle groups
   typedef SuHashMap< SuString, SuParticleGroupInfo, SuStringHasher, SuHashComparer<SuString> > ParticleMap;
   ParticleMap m_particleGroups;

   /// Singleton instance
   static SuObjectManager* ms_pManager ;

   /// The object currently being drawn, used to facilitate app updates
   SuObject* m_pDrawingObject;


   typedef SuHashMap< SuString, SuUpdateHandlerPtr, SuStringHasher, SuHashComparer<SuString> > UpdateHandlerMap;
   UpdateHandlerMap m_updateHandlers;   ///< Set of cached update handlers for various app-update types
   
   /// Search path for object files
   SuSearchPath m_searchPath;

   bool m_bTrackDraws; ///< Flag to tell whether or not to track object draws

   struct ObjectDrawStats
   {
      uint32 nDrawIndexed;       ///< Total number of draw calls generated
      uint32 nDrawNonIndexed;    ///< Total number of nonindexed draw calls generated
      uint32 nPoints;            ///< Total number of point primitives generated
      uint32 nLines;             ///< Total number of line primitives generated
      uint32 nTriangles;         ///< Total number of triangle primitives generated
   
      uint32 nDrawIndexedPrev;   ///< Value of draw call counter at the time object drawing began
      uint32 nDrawNonIndexedPrev;///< Value of draw call counter at the time object drawing began
      uint32 nPointsPrev;        ///< Value of point counter at the time object drawing began
      uint32 nLinesPrev;         ///< Value of point counter at the time object drawing began
      uint32 nTrianglesPrev;     ///< Value of triangles counter at the time object drawing began
   };

   /// Map of every object to its drawing statistics, used to implemented draw call tracking
   typedef SuHashMap< SuString, ObjectDrawStats, SuStringHasher, SuHashComparer<SuString> > ObjectDrawMap;
   ObjectDrawMap m_objectDrawMap;

// tolua_begin
};
//tolua_end


#endif