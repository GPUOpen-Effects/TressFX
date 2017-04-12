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

#ifndef _SU_MODEL_H_
#define _SU_MODEL_H_

#include "SuHashMap.h"
#include "SuGPUSamplingResourceView.h"
#include "SuEffect.h"
#include "SuPoint3.h"
#include "SuModelADB.h"
#include "SuEffectReloadListener.h"
#include "SuGPUIndexBuffer.h"
#include "SuGPUVertexBuffer.h"
#include "SuGPUTexture.h"
#include "SuModelChunk.h"

class SuArchive;
class SuModelData;
class SuModelDataManifest;
class SuModelVB;
class SuModelIB;
class SuModelTexture;
class SuSkeleton;
class SuMorphTargetInfo;


class SuGeometrySorter;
class SuCamera;
class SuMatrix4;


//tolua_begin
class SuGPUVertexBuffer;
class SuGPUIndexBuffer;
class SuGPUTexture;
typedef SuCountedPtr<SuGPUVertexBuffer> SuGPUVertexBufferPtr;
typedef SuCountedPtr<SuGPUIndexBuffer> SuGPUIndexBufferPtr;
typedef SuCountedPtr<SuGPUTexture> SuGPUTexturePtr;
typedef SuCountedPtr<SuSkeleton> SuSkeletonPtr;
//tolua_end

typedef SuCountedPtr< SuMorphTargetInfo > SuMorphTargetInfoPtr;


/// An array of model chunks
typedef SuArray<SuModelChunk*> SuModelChunkArray;

/// A mapping of names to GPU vertex buffers
typedef SuHashMap< SuString, SuGPUVertexBufferPtr, SuStringHasher, SuHashComparer<SuString> > SuGPUVBMap;

/// A mapping of names to GPU index buffers
typedef SuHashMap< SuString, SuGPUIndexBufferPtr, SuStringHasher, SuHashComparer<SuString> > SuGPUIBMap;

/// A mapping of names to GPU textures
typedef SuHashMap< SuString, SuGPUTexturePtr, SuStringHasher, SuHashComparer<SuString> > SuGPUTexMap;

/// A struct to store a set of chunks which share the same effect
struct SuModelEffectGroup
{
   SuEffectPtr            pEffect;   ///< The effect
   SuArray<uint32>        chunks;    ///< Array of chunk indices of chunks in this effect group
};

typedef SuList<SuModelEffectGroup> SuModelEffectGroupList;


//=========================================================================================================
/// \ingroup ObjectManager
/// \brief A class to store GPU resources and rendering state information for a model.
///
///   The SuModel class creates and maintains all of the runtime state that is used to render a model.  This includes:
///      - GPU Vertex Buffers
///      - GPU Index Buffers
///      - GPU Textures
/// 
//=========================================================================================================
// tolua_begin
class SuModel : public SuEffectReloadListener
{
public:

   /// Destructor
   SU_DLL ~SuModel( );

   /// Locates a vertex buffer with a particular name
   SU_DLL SuGPUVertexBufferPtr GetVBByName( const SuString& rName ) const;
   
   /// Locates an index buffer with a particular name
   SU_DLL SuGPUIndexBufferPtr GetIBByName( const SuString& rName ) const;

   /// Locates a texture with a particular name
   SU_DLL SuGPUTexturePtr GetTextureByName( const SuString& rName ) const;

   /// Returns the skeleton for this model, or NULL if there is no skeleton
   SU_DLL SuSkeleton* GetSkeleton() const;

   /// Returns the archive name from which this model was loaded
   inline const SuString& GetArchiveName() const { return m_archiveName; };

   /// Retrieves the bounding box for this model, if it has one
   SU_DLL bool GetBoundingBox( SuPoint3* pMinOut, SuPoint3* pMaxOut ) const;

   /// Retrieves the bounding sphere for this model, if it has one
   SU_DLL bool GetBoundingSphere( SuPoint3* pCenter, float* pfRadius ) const;

   /// Returns the number of chunks in the model
   inline uint32 GetChunkCount( ) const { return static_cast<uint32>(m_chunks.size()); };

   /// Returns the name of a particular chunk
   inline const SuString& GetChunkName( uint32 nChunkID ) const 
   {
      SU_ASSERT( nChunkID < GetChunkCount() );
      return m_chunks[nChunkID]->GetName();
   };

   /// Returns the vertex count for a particular chunk
   inline uint32 GetChunkVertexCount( uint32 nChunkID ) const 
   {
      SU_ASSERT( nChunkID < GetChunkCount() );
      return m_chunks[nChunkID]->GetVertexCount();
   };

   /// Returns the index count for a particular chunk
   inline uint32 GetChunkIndexCount( uint32 nChunkID ) const
   {
      SU_ASSERT( nChunkID < GetChunkCount() );
      return m_chunks[nChunkID]->GetIndexCount();
   };

   /// Returns the primitive type for a particular chunk
   inline SuRenderManager::PrimType GetChunkPrimitiveType( uint32 nChunkID ) const
   {
      SU_ASSERT( nChunkID < GetChunkCount() );
      return m_chunks[nChunkID]->GetPrimitiveType();
   };

// tolua_end

   /// Factory method to contruct a model by reading it from a zip archive with the given name
   static SU_DLL SuModel* ConstructFromArchive( const SuString& rArchiveName );

   /// Returns the set of chunks contained in this model.
   SU_DLL const SuModelChunkArray& GetChunks() const;

   /// Returns a chunk in the model
   SU_DLL SuModelChunk* GetChunk( uint32 nIndex ) ;

   /// Retrieves all morph target infos
   inline const SuArray<SuMorphTargetInfoPtr>& GetMorphTargetInfos() const { return m_morphTargetInfos; }

   /// Returns a list of effect groups for this model
   inline const SuModelEffectGroupList& GetEffectGroups() const { return m_effectGroups; };

   /// Determines whether the shader(s) for this model require inverse-transposes for bone matrices
   inline bool NeedsBoneInverseTranspose() const { return m_bShaderNeedsBoneInverses; };

   /// Determines whether the shader(s) for this model require previous-frame bone matrices
   inline bool NeedsLastFrameBones() const { return m_bShaderNeedsLastFrameBones; };

   /// Determines whether the shader(s) for this model require previous-frame bone inverse-transpose matrices
   inline bool NeedsLastFrameBonesInverseTranspose() const { return m_bShaderNeedsLastFrameBonesIT; };

   /// Called on effect reload
   virtual SU_DLL void OnEffectReload( SuEffectPtr pOldEffect, SuEffectPtr pNewEffect );

   /// Called to trigger a model re-load.
   SU_DLL bool Reload( );

   /// Performs view-dependent sorting on the model, for techniques which support it
   SU_DLL bool DoViewDependentSorting( const SuCamera* pCamera, const SuArray<SuMatrix4>* pBones );

private:
   
   /// Constructor -- It is private to restrict instantiation to the ConstructFromArchive() factory method
   SuModel();

   /// Helper method to create a model from an SuModelData object
   bool Create( SuArchive* pArchive );

   /// Helper method to create the model's chunks
   bool CreateChunks( SuModelData* pModelData );

   /// Helper method to load all vertex buffers
   bool CreateAllVertexBuffers( SuArchive* pArchive, const SuModelDataManifest& rManifest, SuModelData* pModelData );
   
   /// Helper method to load all index buffers
   bool CreateAllIndexBuffers( SuArchive* pArchive, const SuModelDataManifest& rManifest, SuModelData* pModelData );

   /// Helper method to create textures
   bool CreateAllTextures( SuArchive* pArchive, const SuModelDataManifest& rManifest );

   /// Helper method to load a single vertex buffer
   bool CreateGPUVB( SuModelVB* pModelVB, bool bFetchable );
   
   /// Helper method to load a single index buffer
   bool CreateGPUIB( SuModelIB* pModelIB, bool bDynamic, bool bFetchable );

   /// Helper method to construct effect groups
   bool BuildEffectGroups( SuModelData* pModelData );

   /// Determines whether any shaders in this model have a particular app-update
   bool DoShadersHaveAppUpdate( const SuString& rAppUpdateName ) const;

   /// Frees the entire internal state of this model
   void Destroy();

   /// Destroys the chunks in this model
   void DestroyChunks();

   /// Destroys the effect groups in this model
   void DestroyEffectGroups();

   /// Loads bounding volume ADBs from archive, if any are present
   bool InitBoundingVolumeADBs( SuArchive* pArchive, const SuModelDataManifest& rManifest );

   /// Creates the morph target info array
   bool InitMorphTargetInfos( SuArchive* pArchive, const SuModelDataManifest& rManifest );

   /// Helper method which determines whether or not an effect uses view-dependent sorting
   bool DoesEffectUseRuntimeSort( SuEffectPtr pEffect ) const;

   /// Performs required initialization for view-dependent sorting
   void InitSorting( SuModelData* pModelData );

   /// Destroys the sorters, and all structures thereunto pertaining
   void DestroySorters();

   /// Makes sure that chunk index ranges are sane
   bool ValidateChunkIndexRanges() const;


   SuModelADBPtr m_pBoundingBoxADB;    ///< ADB containing information about the object's bounding box
   SuModelADBPtr m_pBoundingSphereADB; ///< ADB containing information about the object's bounding sphere

   SuModelEffectGroupList m_effectGroups; ///< Set of effect groups in this model

   SuGPUVBMap m_gpuVBs;       ///< GPU vertex buffers used by the model, indexed by their names
   SuGPUIBMap m_gpuIBs;       ///< GPU index buffers used by the model, indexed by their names
   SuGPUTexMap m_gpuTextures; ///< GPU textures, indexed by their names

   SuString m_archiveName;    ///< Name of the archive from whence this model came

   SuModelChunkArray m_chunks; ///< Array of chunks that make up this object

   SuArray<SuMorphTargetInfoPtr> m_morphTargetInfos;  ///< Array with information on all the morph target data

   SuSkeletonPtr m_pSkeleton;  ///< The model's skeleton, if it has one
   
   bool m_bShaderNeedsBoneInverses; ///< Flag indicating whether or not the shaders in this model require bone inverses
   bool m_bShaderNeedsLastFrameBones; ///< Flag indicating whether or not the shaders in this model require previous frame bones
   bool m_bShaderNeedsLastFrameBonesIT; ///< Flag indicating whether or not the shaders in this model require previous frame bone inverse-transpose

   /// Array of geometry sorters (for deleting)
   SuArray<SuGeometrySorter*> m_sorters;

// tolua_begin
};
// tolua_end


#endif // _SU_MODEL_H_
