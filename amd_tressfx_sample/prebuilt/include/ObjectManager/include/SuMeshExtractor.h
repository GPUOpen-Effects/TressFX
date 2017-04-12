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

#ifndef _SU_MESHEXTRACTOR_H_
#define _SU_MESHEXTRACTOR_H_

#include "SuCountedPtr.h"
#include "SuPoint3.h"
#include "SuVector3.h"

class SuArchive;
class SuModelData;
class SuModelVB;
class SuModelIB;
class SuModelChunkDesc;
class SuVertexStreamDecl;
class SuVertexElement;
class SuMeshExtractor;
class SuEffect;

typedef SuCountedPtr<SuModelChunkDesc> SuModelChunkDescPtr;
typedef SuCountedPtr<SuModelData> SuModelDataPtr;
typedef SuCountedPtr<SuMeshExtractor> SuMeshExtractorPtr;
typedef SuCountedPtr<const SuMeshExtractor> SuMeshExtractorConstPtr;
typedef SuCountedPtr<SuModelIB> SuModelIBPtr;
typedef SuCountedPtr<SuModelVB> SuModelVBPtr;
typedef SuCountedPtr<SuEffect> SuEffectPtr;

/// Helper class for extracting meshes from models and model chunks
class SuMeshExtractor
{
public:

   /// Destructor
   SU_DLL ~SuMeshExtractor();

   /// Creat an extractor for just a chunk of a mesh
   static SU_DLL SuMeshExtractorPtr CreateChunkExtractor( const SuArchive* pArchive, SuModelDataPtr pModelData, SuModelChunkDescPtr pChunk, const SuVertexStreamDecl* pStream = NULL );

   /// Create an extractor for the entire mesh (all chunks)
   static SU_DLL SuMeshExtractorPtr CreateMeshExtractor( const SuArchive* pArchive, SuModelDataPtr pModelData, const SuVertexStreamDecl* pStream = NULL );

   /// Concatenate an extracted mesh with this one
   SU_DLL bool Concatenate ( const SuMeshExtractor* pSource );

   // Check if the extracted mesh has certain vertex elements
   SU_DLL bool HasPositions() const { return m_positions.size() == 0 ? false : true; }
   SU_DLL bool HasNormals() const { return m_normals.size() == 0 ? false : true; }
   SU_DLL bool HasTangents() const { return m_tangents.size() == 0 ? false : true; }
   SU_DLL bool HasBinormals() const { return m_binormals.size() == 0 ? false : true; }
   SU_DLL bool HasSkinningWeights() const { return m_skinningWeights.size() == 0 ? false : true; }
   SU_DLL bool HasSkinningIndices() const { return m_skinningIndices.size() == 0 ? false : true; }
      
   // Geometric elements
   SU_DLL const SuArray<SuPoint3>&  GetPositions() const { return m_positions; } ///< Get positions
   SU_DLL const SuArray<SuVector3>& GetNormals()   const { return m_normals;   } ///< Get normals
   SU_DLL const SuArray<SuVector3>& GetTangents()  const { return m_tangents;  } ///< Get tangents
   SU_DLL const SuArray<SuVector3>& GetBinormals() const { return m_binormals; } ///< Get binormals

   // Skinning elements
   SU_DLL const SuArray< SuArray<float32> >& GetSkinningWeights() const { return m_skinningWeights; } ///< Bone weights
   SU_DLL const SuArray< SuArray<uint32> >&  GetSkinningIndices() const { return m_skinningIndices; } ///< Bone indices

   // Index buffer
   SU_DLL const SuArray<uint32>& GetIndices() const { return m_indices; }    ///< Get index buffer

private:

   /// Returns the "default" geometry stream (first one it finds with a particular element type)
   static const SuVertexStreamDecl* GetDefaultStream ( SuEffectPtr pEffect, const SuString& rSemantic );

   /// Scans a model and locates the vertex buffer that is mapped to a particular stream declaration
   static bool LocateVBForStream ( const SuArchive* pArchive, SuModelDataPtr pModelData,
                                   SuModelChunkDescPtr pChunk, const SuVertexStreamDecl* pStream, 
                                   SuModelVBPtr& rVBOut );

   /// Scans an index buffer and extracts triangle vertex indices
   static bool ExtractTriangleIndices( SuModelChunkDescPtr pChunk, SuModelIBPtr pIB, SuArray<uint32>& rIndicesOut, int nIndexOffset );

   /// Extracts elements from a VB and packs them into a buffer
   template<class T> static bool ExtractVertexElements ( const SuArchive* pArchive, SuModelDataPtr pModelData,
                                                         SuModelChunkDescPtr pChunk, const SuVertexStreamDecl* pStream, 
                                                         const SuVertexElement* pElement, SuArray<T>& rOutputBuffer );

   /// Extracts skinning weights from a VB and packs them into a buffer
   static bool ExtractSkinWeights ( const SuArchive* pArchive, SuModelDataPtr pModelData,
                                    SuModelChunkDescPtr pChunk, const SuVertexStreamDecl* pStream, 
                                    const SuVertexElement* pElement, SuArray< SuArray<float32> >& rSkinningWeights );

   /// Extracts skinning weights from a VB and packs them into a buffer
   static bool ExtractSkinIndices ( const SuArchive* pArchive, SuModelDataPtr pModelData,
                                    SuModelChunkDescPtr pChunk, const SuVertexStreamDecl* pStream, 
                                    const SuVertexElement* pElement, SuArray< SuArray<uint32> >& rSkinningIndices );

   SuMeshExtractor(); ///< Private constructor, use factory method

   SuArray<SuPoint3> m_positions;  ///< Positions
   SuArray<SuVector3> m_normals;   ///< Normals
   SuArray<SuVector3> m_tangents;  ///< Tangents
   SuArray<SuVector3> m_binormals; ///< Binormals

   SuArray< SuArray<float32> > m_skinningWeights; ///< Bone weights
   SuArray< SuArray<uint32> > m_skinningIndices;  ///< Bone indices

   SuArray<uint32> m_indices; ///< Indices
   
};


/// Legacy: Extracts a simple triangle mesh for a model
SU_DLL bool SuExtractTriangleMesh( const SuArchive* pArchive, SuModelDataPtr pModelData, SuArray<SuPoint3>& rVerticesOut, SuArray<uint32>& rIndicesOut );

/// Legacy: Extracts a simple triangle mesh for a single chunk in a model.  
SU_DLL bool SuExtractTriangleMeshForChunk( const SuArchive* pArchive, 
                                           SuModelDataPtr pModelData, 
                                           SuModelChunkDescPtr pChunk,
                                           SuArray<SuPoint3>& rVerticesOut, 
                                           SuArray<uint32>& rIndicesOut );


/// Legacy: Extracts skinning information for a single chunk in a model
SU_DLL bool SuExtractSkinInfoForChunk( const SuArchive* pArchive, 
                                       SuModelDataPtr pModelData, 
                                       SuModelChunkDescPtr pChunk,
                                       SuArray< SuArray<float> >& rWeightsOut, 
                                       SuArray< SuArray<uint32> >& rIndicesOut );

#endif // _SU_MESHEXTRACTOR_H_

