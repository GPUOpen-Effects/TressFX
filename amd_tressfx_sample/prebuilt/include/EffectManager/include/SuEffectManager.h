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

#ifndef _SUEFFECTMANAGER_H
#define _SUEFFECTMANAGER_H

#include "SuTypes.h"
#include "SuHashMap.h"
#include "SuSearchPath.h"
#include "SuCountedPtr.h"
#include "SuEffectParameter.h"
#include "SuUpdateHandlerFactory.h"
#include "SuTextureSlotBinderFactory.h"
#include "SuUAVSlotBinderFactory.h"
#include "SuStreamBinderFactory.h"

/// \defgroup EffectManager Effect Manager

class SuEffect;
class SuEffectBuilder;
class SuSampler;
class SuEffectPass;
class SuUpdateHandler;
class SuShaderCache;
class SuEffectPassOverrideData;
typedef SuCountedPtr<SuEffectPassOverrideData> SuEffectPassOverrideDataPtr;

class SuEffectReloadListener;

typedef SuHashMap< SuString, SuUpdateHandlerFactory*, SuStringHasher, SuHashComparer<SuString> > SuUpdateHandlerFactoryMap;
typedef SuCountedPtr<SuUpdateHandler> SuUpdateHandlerPtr;
typedef SuCountedPtr<SuEffect> SuEffectPtr;

typedef SuHashMap< SuString, SuTextureSlotBinderFactory*, SuStringHasher, SuHashComparer<SuString> > SuTextureSlotBinderFactoryMap;
typedef SuHashMap< SuString, SuUAVSlotBinderFactory*, SuStringHasher, SuHashComparer<SuString> > SuUAVSlotBinderFactoryMap;
typedef SuHashMap< SuString, SuStreamBinderFactory*, SuStringHasher, SuHashComparer<SuString> > SuStreamBinderFactoryMap;

/// \brief Effect manager
///
/// \ingroup EffectManager
/// Cool new Effect Manager.

// tolua_begin
class SuEffectManager : public SuSharedObjectOwner<SuEffect>, public SuUpdateHandlerFactory
{
// tolua_end
protected:
   /// Constructor
   SU_DLL SuEffectManager();

public:
   /// Singleton access
   static SU_DLL SuEffectManager* GetPtr();                // tolua_export

   /// Singleton access
   static SU_DLL SuEffectManager& GetRef();                // tolua_export

   /// Delete singleton instance
   static SU_DLL void ShutDown();

   /// Initialize the effect manager
   SU_DLL void SetShaderCachePath( const SuString& rShaderCachePath );

   /// Add effect search path
   SU_DLL void AddSearchPath( const SuString& rPath );     // tolua_export

   /// Add a #define
   SU_DLL void AddDefine( const SuString& rSymbol, const SuString& rValue ); // tolua_export

   /// Clear any and all search paths that have been set
   SU_DLL void ClearSearchPaths();

   /// Returns the search path for effect files
   inline const SuSearchPath& GetSearchPath() const { return m_searchPath; }

   /// Loads an effect file
   SU_DLL SuCountedPtr<SuEffect> LoadEffect( const SuString& rFileName, uint32 nLoadFlags = 0 );          // tolua_export

   /// Sets the default effect which will be used to determine the default render and sampler states.
   SU_DLL bool SetDefaultEffect( SuCountedPtr<SuEffect> pDefaultEffect );                           // tolua_export

   // tolua_begin
   /// Optional flags to pass into LoadEffect()
   enum EffectLoadFlags
   {
      LOAD_DEFAULT_EFFECT =  0x00000001,
      EFFECT_LOAD_FLAGS_FORCE_UINT32 = 0xFFFFFFFF
   };

   /// Reloads any effects whose effect files have changed
   SU_DLL void ReloadEffects( bool bForceReload );

   /// Loads a set of override techniques from an effect file. 
   SU_DLL bool LoadOverrideTechniqueEffect( const SuString& rFileName );

   inline const SuArray< SuEffectPassOverrideDataPtr >& GetEffectPassOverrideData() const { return m_pOverridePassInfos; }

   /// Returns true if front faces are counter clockwise (the default)
   /// Returns false if front faces are not counter clockwise.
   inline bool IsFrontCounterClockwise() const { return m_bFrontCounterClockwise; };

   /// Sets whether front or back faces are counter-clockwise.  This method is virtual to allow 
   /// subclasses to perform API-specific state-twiddling when the value is changed.  Subclasses which override
   /// this method should make sure to call the base-class version.  It is illegal to call this method inside of an
   /// effect pass Begin/End pair. 
   virtual SU_DLL void SetFrontCounterClockwise( bool bFrontCCW ) { m_bFrontCounterClockwise = bFrontCCW; }

   /// Writes a list of loaded effect files to the Sushi log
   SU_DLL void ListLoadedEffects() const;

   /// Sets the currently active override technique. Empty string means no override technique.
   SU_DLL void SetOverrideTechnique( const SuString& rName );

   /// Returns the ID of the currently active override technique. -1 if none is active
   SU_DLL int32 GetActiveOverrideTechnique() const;

   /// Returns the name of the currently active override technique.
   SU_DLL SuString GetActiveOverrideTechniqueName() const;

   /// Returns the number of loaded override techniques
   inline uint32 GetNumOverrideTechniques() const { return static_cast<uint32>( m_pOverridePassInfos.size() ); }

   /// Returns the name of an override technique
   SU_DLL SuString GetOverrideTechniqueName( uint32 nIdx );

   /// Enable/disable forcing any active override technique off
   SU_DLL void ForceDisableOverrideTechniques( bool bDisable );

   /// Returns whether override techniques are disabled
   SU_DLL bool AreOverrideTechniquesDisabled() const;

   // tolua_end

   /// Returns the shader cache
   inline SuShaderCache* GetShaderCache() const { return m_pShaderCache; };

   /// Returns the default sampler state
   inline const SuSampler* GetDefaultSampler() const { return m_pDefaultSampler; }

   /// Returns a render pass object ptr containing the default render state
   inline const SuEffectPass* GetDefaultRenderPass() const { return m_pDefaultPass; }

   /// Registers a handler for an app-updatable effect parameter.
   SU_DLL void RegisterUpdateHandlerFactory( const SuString& rUpdateName, SuUpdateHandlerFactory* pHandler );

   /// Registers a factory for constructing Texture slot binders
   SU_DLL void RegisterTextureSlotBinderFactory( const SuString& rBinderType, SuTextureSlotBinderFactory* pFactory );

   /// Registers a factory for constructing UAV slot binders
   SU_DLL void RegisterUAVSlotBinderFactory( const SuString& rBinderType, SuUAVSlotBinderFactory* pFactory );

   /// Registers a factory for constructing vertex stream binders
   SU_DLL void RegisterStreamBinderFactory( const SuString& rBinderType, SuStreamBinderFactory* pFactory );

   /// Removes a previously registered update handler factory
   SU_DLL void DeregisterUpdateHandlerFactory( SuUpdateHandlerFactory* pFactory );

   /// Removes a previously registered texture slot binder factory
   SU_DLL void DeregisterTextureSlotBinderFactory( SuTextureSlotBinderFactory* pFactory );

   /// Removes a previously registered texture slot binder factory
   SU_DLL void DeregisterUAVSlotBinderFactory( SuUAVSlotBinderFactory* pFactory );

   /// Removes a previously registered stream binder factory
   SU_DLL void DeregisterStreamBinderFactory( SuStreamBinderFactory* pFactory );

   /// Sets a texture slot binder factory as the 'default' binder factory.  This factory is used if no other factories
   /// have been registered which match the string given in the slot annotations
   /// \param pFactory  The default binder factory.  The effect manager does not assume ownership
   inline void SetDefaultTextureSlotBinderFactory( SuTextureSlotBinderFactory* pFactory ) { m_pDefaultTextureSlotBinderFactory = pFactory; };

   /// Sets a texture slot binder factory as the 'default' binder factory.  This factory is used if no other factories
   /// have been registered which match the string given in the slot annotations
   /// \param pFactory  The default binder factory.  The effect manager does not assume ownership
   inline void SetDefaultUAVSlotBinderFactory( SuUAVSlotBinderFactory* pFactory ) { m_pDefaultUAVSlotBinderFactory = pFactory; };

   /// Sets a texture slot binder factory as the 'default' binder factory.  This factory is used if no other factories
   /// have been registered which match the string given in the stream annotations
   /// \param pFactory  The default binder factory.  The effect manager does not assume ownership
   inline void SetDefaultStreamBinderFactory( SuStreamBinderFactory* pFactory ) { m_pDefaultStreamBinderFactory = pFactory; };

   /// Locates the app update handler factory for a particular app-update type
   SU_DLL SuUpdateHandlerFactory* GetUpdateHandlerFactory( const SuString& rAnnotationText ) const;

   /// Locates the texture slot binder factory for a particular binding type
   SU_DLL SuTextureSlotBinderFactory* GetTextureSlotBinderFactory( const SuString& rBinderType ) const;

   /// Locates the texture slot binder factory for a particular binding type
   SU_DLL SuUAVSlotBinderFactory* GetUAVSlotBinderFactory( const SuString& rBinderType ) const;

   /// Locates the stream binder factory for a particular binding type
   SU_DLL SuStreamBinderFactory* GetStreamBinderFactory( const SuString& rBinderType ) const;

   /// Registers an effect reload listener with the effect manager
   SU_DLL void AddReloadListener( SuEffectReloadListener* pListener );

   /// Un-registers an effect reload listener with the effect manager
   SU_DLL void RemoveReloadListener( SuEffectReloadListener* pListener );

   /// Notifies the effect manager that it now holds the last reference to an effect
   virtual SU_DLL void NotifyLastReference( SuEffect* pEffect );

   /// Returns the current pass number (0-based).  Effects are responsible for setting this in order to support the 'PassNumber' app-update 
   /// This method should not be called outside the effect code base
   inline uint32 GetCurrentEffectPassNumber() const { return m_nEffectPassNumber; }

   /// Sets the current pass number (0-based).  Effects are responsible for setting this in order to support the 'PassNumber' app-update
   /// This method should not be called outside the effect code base.
   inline void SetCurrentEffectPassNumber( uint32 nPass ) { m_nEffectPassNumber = nPass; }

   /// Returns the current effect that is being rendered with.
   /// This method should not be called outside the effect code base.
   inline SuEffect* GetCurrentEffect() const { return m_pCurrentEffect; }

   /// Sets the current effect that is being rendered with.  Effects are responsible for setting this in order to support effect manager app-updates.
   /// This method should not be called outside the effect code base.
   inline void SetCurrentEffect( SuEffect* pEffect ) { m_pCurrentEffect = pEffect; }

   /// The effect manager is, itself, an update handler factory, because it makes handlers for the PassNumber app-update
   virtual SU_DLL SuUpdateHandlerPtr GetUpdateHandler( const SuString& rAppUpdateName, SuEffectParameter::DataType eType,
                                                uint32 nArraySize, const SuAnnotationMap* pAnnotations );

   /// Returns true if effect parameter app-update handlers are enabled, false otherwise
   inline bool AreAppUpdatesEnabled() const { return m_bHandlersEnabled; };

   /// Enables or disables effect parameter app-update handlers
   inline void SetAppUpdatesEnabled( bool bEnable ) { m_bHandlersEnabled = bEnable; }

   /// Enables or disables effect file logging
   inline void SetEffectFileLogging( bool bLog ) { m_bLogEffectLoading = bLog; };   // tolua_export

   /// Override effect options
   SU_DLL void OverrideEffectOption ( const SuString& rKey, const SuString& rValue );

protected:
   /// Destructor
   virtual SU_DLL ~SuEffectManager ();

   /// Factory method to create an effect builder
   virtual SuEffectBuilder* CreateEffectBuilder( bool bBuildDefaultEffect = false ) const = 0;

   /// Uses SuEffectParser interface to set API-specific effect #define definitions
   virtual void AddAPISpecificEffectParserDefines() const = 0;

   /// Constructs an effect from a file
   SU_DLL SuEffect* DoEffectLoad( const SuString& rFile, SuEffectBuilder* pBuilder );

   /// Computes the dependency timestamps for an effect
   SU_DLL void GetDependencyTimestamps( SuEffectPtr pEffect, SuArray<time_t>& rTimes );


private:
   /// The signleton instance
   static SuEffectManager* ms_instance;

   /// The path in which to search for effect files
   SuSearchPath m_searchPath;

   /// The default effect that contains the default sampler and render pass settings
   SuCountedPtr<SuEffect> m_pDefaultEffect;

   /// The default sampler settings that all other samplers will be cloned from
   const SuSampler* m_pDefaultSampler;

   /// The default effect pass that defines all default render state
   const SuEffectPass* m_pDefaultPass;

   /// Update handler for the pass number app-update
   SuUpdateHandlerPtr m_pPassNumHandler;

   /// Mapping of app-update strings to update handler factories
   SuUpdateHandlerFactoryMap m_updateHandlers;

   /// Mapping of texture slot binder types to texture slot binders
   SuTextureSlotBinderFactoryMap m_textureSlotBinderFactories;

   SuTextureSlotBinderFactory* m_pDefaultTextureSlotBinderFactory; ///< Default texture slot binder factory.  This factory makes binders for binding named resources to 'BindRT' slots   
   
   /// Mapping of uav slot binder types to texture slot binders
   SuUAVSlotBinderFactoryMap m_uavSlotBinderFactories;

   SuUAVSlotBinderFactory* m_pDefaultUAVSlotBinderFactory; ///< Default texture slot binder factory.  This factory makes binders for binding named resources to 'BindRT' slots   

   /// Mapping of stream binder types to stream binders
   SuStreamBinderFactoryMap m_streamBinderFactories;

   SuStreamBinderFactory* m_pDefaultStreamBinderFactory; ///< Default stream binder factory.  Makes binders for binding named resources to 'BindDynamicBuffer' streams


   /// Hash map for effects that we've already loaded
   struct SuEffectInfo
   {
      SuCountedPtr<SuEffect> pEffect;
      time_t                 nTimeStamp;
      SuArray<time_t>        dependencyTimeStamps;
   };

   typedef SuHashMap< SuString, SuEffectInfo, SuStringHasher, SuHashComparer<SuString> > SuEffectFileMap;
   SuEffectFileMap m_effectFileMap;

   /// Set of symbols which should be #define'ed for all loaded effects
   struct Define
   {
      SuString symbol;
      SuString value;
   };

   SuList<Define> m_defines;

   /// Set of objects which are listening for effect re-load events
   SuArray<SuEffectReloadListener*> m_ReloadListeners;

   /// If this flag is set, then the effect manager will cull faces in the opposite way that the effect specifies.
   /// This is necessary to support cases such as planar reflection, in which the face winding can change
   bool m_bFrontCounterClockwise;

   /// Flag to indicate whether or not an effect reload is in progress
   bool m_bDoingReload;

   /// The shader cache
   SuShaderCache* m_pShaderCache;

   uint32 m_nEffectPassNumber;   ///< Current effect pass index.  Used to implement the PassNumber App-Update
   SuEffect* m_pCurrentEffect;   ///< The current effect being drawn.  Used to implement effect manager app-updates

   bool m_bHandlersEnabled; ///< Flag indicating whether or not update handlers are enabled

   bool m_bLogEffectLoading; ///< Flag indicating that effect filenames should be printed to the log as they are loaded

   int32 m_nActiveOverrideTechniqueID;
   int32 m_nForceOverrideDisableCount;
   SuArray< SuEffectPassOverrideDataPtr > m_pOverridePassInfos;

   typedef SuHashMap< SuString, uint32, SuStringHasher, SuHashComparer<SuString> > SuOverridePassNameMap;
   SuOverridePassNameMap m_overridePassNameMap;

   SuAnnotationMap m_overrideEffectOptions; ///< Global effect option overrides
// tolua_begin
};
// tolua_end

#endif