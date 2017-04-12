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

#ifndef _SU_MODELCHUNKDESC_H_
#define _SU_MODELCHUNKDESC_H_

#include "SuRenderManager.h"
#include "SuCountedPtr.h"
#include "SuModelSubChunk.h"
#include "SuParameterSetting.h"

class SuSubChunkTree;

class SuModelChunkDesc;
typedef SuCountedPtr<SuModelChunkDesc> SuModelChunkDescPtr;

//=========================================================================================================
/// \ingroup ObjectManager
/// \brief A class to encapsulate the description of a model chunk
///
///   An SuModelChunkDesc is a collection of information about a model chunk
///   The information stored here includes:
///      
///      - Name of the effect file used to render the chunk
///      - Set of effect parameter values
///      - Mapping of texture slot names to model texture names
///      - Mapping of stream names to model VB names
///      - Index buffer name (can be the empty string "" to indicate no change to index buffer)
///      - IB and VB range
///      - Primitive type
///      - Optional set of subchunks
///      - Optional set of subchunk trees
///      
///   If parameter settings, vertex buffer names, and index buffer names are not specified, then
///   the chunk will not modify the corresponding effect state during rendering
///
///
//========================================================================================================= 
class SuModelChunkDesc
{
public:

   /// A structure used to store the name of each VB to be bound to an effect stream
   struct SuVBMapping
   {
      SuString effectStreamName; ///< Name of the effect stream to bind to
      SuString modelVBName;      ///< Name of the corresponding VB to be bound.  If the string is "NULL", this indicates that no VB is bound
   };
   typedef SuArray<SuVBMapping> SuVBMappingArray;

   /// An array that identifies which bone indices should go into the constant store
   typedef SuArray<uint32> SuBoneMapArray;

   /// A structure used to store the name of each texture to be bound to an effect texture slot
   struct SuTexMapping
   {
      SuString effectSlotName;   ///< Name of the texture slot to bind to
      SuString modelTextureName; ///< Name of the corresponding model texture to bind.  If the string is "NULL", this indicates that no Texture is bound
   };

   typedef SuArray<SuTexMapping> SuTexMappingArray;

   /// Constructs a chunk description for a chunk with a particular name
   SU_DLL SuModelChunkDesc( const SuString& rChunkName );

   /// Destructor
   SU_DLL ~SuModelChunkDesc();

   /// Returns the name of the chunk
   inline const SuString& GetChunkName() const { return m_chunkName; };

   /// Returns the effect filename used by the chunk
   inline const SuString& GetEffectName() const { return m_effectName; };
   
   /// Returns the name of the default technique used by the chunk
   inline const SuString& GetTechniqueName() const { return m_techniqueName; };

   /// Returns the name of the index buffer used by the chunk
   inline const SuString& GetIBName() const { return m_ibName; };

   /// Returns the name of the visibility curve used by the chunk
   inline const SuString& GetVisibilityCurveName() const { return m_visCurveName; }

   /// Accessor for the index ranges used to render the chunk.  
   inline void GetIndexRanges( uint32& rMin, uint32& rMax ) const { rMin = m_nIBMin; rMax = m_nIBMax; };

   /// Accessor for the vertex ranges used to render the chunk
   inline void GetVertexRanges( uint32& rMin, uint32& rMax )const { rMin = m_nVBMin; rMax = m_nVBMax; };

   /// Helper function that computes the number of vertices in the chunk
   inline uint32 GetVertexCount() const { return m_nVBMax - m_nVBMin + 1; }

   /// Helper function that computes the number of indices in the chunk
   inline uint32 GetIndexCount() const { return m_nIBMax - m_nIBMin + 1; };

   /// Accessor for the primitive type
   inline SuRenderManager::PrimType GetPrimType() const { return m_ePrimType; };

   /// Returns a list containing the VB names to bind to each effect stream
   inline const SuVBMappingArray& GetVBMappings() const { return m_vbMappings; };

   /// Returns a list containing the texture names to bind to each effect texture slot
   inline const SuTexMappingArray& GetTexMappings() const { return m_texMappings; };

   /// Returns an array of effect parameter settings for the chunk
   inline const SuParameterSettingArray& GetParameterSettings() const { return m_parameterSettings; };

   /// Returns the bone mapping array for the chunk.  If this is NULL, it indicates that all bones
   /// Should be packed sequentially into the constant store.
   inline const SuBoneMapArray* GetBoneMap() const { return m_pBoneMap; };

   /// Sets the bone map array for the chunk.  The chunk will delete the bone map array that is passed
   inline void SetBoneMap( SuBoneMapArray* pMap ) { m_pBoneMap = pMap; };

   /// Sets the name of the effect file used by the chunk
   inline void SetEffectName( const SuString& rName ) { m_effectName = rName; };

   /// Sets the name of the default technique used by the chunk
   inline void SetTechniqueName( const SuString& rName ) { m_techniqueName = rName; };

   /// Sets the name of the index buffer used by the chunk
   inline void SetIBName( const SuString& rName ) { m_ibName = rName; };

   /// Sets the name of the visibility curve used by the chunk
   inline void SetVisibilityCurveName( const SuString& rName ) { m_visCurveName = rName; }

   /// Sets the index ranges used to render the chunk
   inline void SetIndexRanges( uint32 nMin, uint32 nMax ) { m_nIBMin = nMin; m_nIBMax = nMax; };

   /// Sets the vertex ranges used to render the chunk
   inline void SetVertexRanges( uint32 nMin, uint32 nMax ) { m_nVBMin = nMin; m_nVBMax = nMax; };

   /// Sets the primitive type
   inline void SetPrimType( SuRenderManager::PrimType eType ) { m_ePrimType = eType; };

   /// Adds a vertex buffer binding to the chunk
   SU_DLL void AddVBMapping( const SuString& rStreamName, const SuString& rVBName );

   /// Adds a texture binding to the chunk
   SU_DLL void AddTexMapping( const SuString& rSlotName, const SuString& rTexName );

   /// Adds an effect parameter setting to the chunk
   SU_DLL void AddParameterSetting( SuParameterSettingPtr pSetting );

   /// Removes vertex buffer mappings from the chunk description
   SU_DLL void ClearVBMappings();

   /// Removes effect parameter settings from the chunk
   SU_DLL void ClearParameterSettings();

   /// Removes texture mappings from the chunk
   SU_DLL void ClearTexMappings();

   /// Adds a subchunk to the chunk description
   inline void AddSubChunk( const SuModelSubChunk& rSubchunk ) { m_subchunks.push_back( rSubchunk ); };

   /// Returns an array containing the sub-chunks in this chunk
   inline const SuArray<SuModelSubChunk>& GetSubChunks() const { return m_subchunks; };

   /// Non-const access to sub-chunks
   inline SuArray<SuModelSubChunk>& GetSubChunks() { return m_subchunks; };

   /// Adds a subchunk tree to the chunk description
   SU_DLL void AddSubChunkTree( SuSubChunkTree* pTree );

   /// Returns an array containing the subchunk trees for this chunk (if any)
   inline const SuArray< SuSubChunkTree* >& GetSubChunkTrees() const { return m_subChunkTrees; };

   /// Tests whether or not the chunk is indexed
   inline bool IsIndexed() const { return m_bIndexed; };

   /// Sets the flag indicating whether or not the chunk is indexed
   void SetIsIndexed( bool bIndexed ) { m_bIndexed = bIndexed; };

   /// Removes all subchunking related information from the chunk description
   SU_DLL void RemoveSubchunkInformation();

private:

   SuString m_chunkName;
   SuString m_ibName;
   SuString m_effectName;
   SuString m_techniqueName;
   SuString m_visCurveName;

   uint32 m_nVBMin;
   uint32 m_nVBMax;
   uint32 m_nIBMin;
   uint32 m_nIBMax;
   bool   m_bIndexed;

   SuRenderManager::PrimType m_ePrimType;

   SuVBMappingArray m_vbMappings;
   SuTexMappingArray m_texMappings;

   SuBoneMapArray* m_pBoneMap;

   SuParameterSettingArray m_parameterSettings;

   SuArray<SuModelSubChunk> m_subchunks;

   SuArray<SuSubChunkTree*> m_subChunkTrees;
};


#endif // _SU_CHUNKDESC_H_
