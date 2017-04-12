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

#ifndef _SU_SUBCHUNKSTATE_H_
#define _SU_SUBCHUNKSTATE_H_

#include "SuTypes.h"
#include "SuMatrix4.h"
#include "SuBoundingBox.h"

class SuModelSubChunk;
class SuBoundingVolumeVisualizer;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief An object which manages a set of subchunk bounding volumes for a subchunk renderer
///
///   This class is used to store the current subchunk bounding volumes for a particular model instance.
///   It is designed to be tightly coupled with the subchunk renderer.
///
///   The SuSubChunkState object contains a list of bones that are referenced by a set of subchunks,
///    a set of current transforms for those bones, and a mapping of subchunks to the bones. 
/// 
//=================================================================================================================================
class SuSubChunkState
{
public:

   /// Constructs a subchunk state for the given set of subchunks
   SuSubChunkState( const SuArray<SuModelSubChunk>* pSubChunks );

   /// Destructor
   ~SuSubChunkState( );

   /// Updates subchunk bounding volumes, based on the current bone transforms for an object
   void UpdateBoundingVolumes( const SuArray<SuMatrix4>& rBones );

   /// Renders the subchunk bounding volumes
   void DrawBoundingVolumes( const SuBoundingVolumeVisualizer& rVisualizer );

   /// Returns the current transformation matrix for a particular subchunk
   inline const SuMatrix4& GetSubchunkTransform( uint32 i ) const { return m_boneXForms[ m_subChunkBoneGroups[i] ]; };

   /// Returns the index of the bone group to which a particular subchunk belongs
   inline uint32 GetSubchunkBoneGroup( uint32 i ) const { return m_subChunkBoneGroups[i]; };

   /// Returns the current transform for a bone group
   const SuMatrix4& GetBoneGroupTransform( uint32 i ) const { return m_boneXForms[i]; };

   /// Returns the number of bone groups.  That is, the number of bones which influence the set of subchunks
   inline uint32 GetBoneGroupCount() const { return (uint32) m_boneIDs.size(); };

private:

   const SuArray<SuModelSubChunk>* m_pSubChunks;   ///< Array of subchunks whose bounding volumes are managed by this subchunk state
   SuArray<uint32> m_subChunkBoneGroups;           ///< Index of subchunk's bone in m_boneIDs
   SuArray<uint32> m_boneIDs;                      ///< Index of each bone in the model that is referenced by a subchunk
   SuArray<SuMatrix4> m_boneXForms;                ///< Current transform for each bone in m_boneIDs
};


#endif // _SU_SUBCHUNKSTATE_H_
