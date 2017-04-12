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

#ifndef _SU_SUBCHUNKTREE_H_
#define _SU_SUBCHUNKTREE_H_

#include "SuArray.h"
#include "SuSubChunkTreeNode.h"
#include "SuModelSubChunk.h"

class SuFile;
class SuFrustum;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A tree data structure that can be used for efficient culling of subchunks
/// 
///   The subchunk tree is a tree of subchunks which share a common bone.  If an object has multiple bones, then
///   multiple subchunk trees are needed.
///
//=================================================================================================================================
class SuSubChunkTree
{
public:

   /// Structure that is used to describe the structure of a subchunk tree
   struct TreeDesc
   {
      TreeDesc* pLeftChild;  ///< Pointer to left child.  NULL if leaf
      TreeDesc* pRightChild; ///< Pointer to right child.  NULL if leaf
      uint32 nSubchunk;      ///< Subchunk index.  Should be ignored if not a leaf.
   };

   /// Creates a subchunk tree from a tree description and a set of subchunks
   static SuSubChunkTree* CreateTree( const SuArray<SuModelSubChunk>& rSubchunks, TreeDesc* pTreeDesc );

   /// Parses a subchunk tree file and creates a set of subchunk trees
   static bool ReadTreesFromFile( SuFile* pFile, SuArray<SuSubChunkTree*>& rTreesOut );

   /// Writes a set of subchunk trees to a subchunk tree file
   static bool WriteTreesToFile( SuFile* pFile, const SuArray<SuSubChunkTree*>& rTrees );

   /// Sanity checks the data in the subchunk tree
   bool Validate( ) const;

   /// Returns a pointer to the root node in the tree
   inline const SuSubChunkTreeNode* GetRoot() const { return &m_nodes[0]; };

   /// Returns a pointer to the root node in the tree
   inline SuSubChunkTreeNode* GetRoot() { return &m_nodes[0]; };

   /// Returns the number of nodes
   inline uint32 GetNumNodes() const { return (uint32)m_nodes.size(); };

   /// Returns a pointer to a particular node
   inline const SuSubChunkTreeNode* GetNode( uint32 nNode ) const { return &m_nodes[nNode]; };

   /// Returns a pointer to a particular node
   inline SuSubChunkTreeNode* GetNode( uint32 nNode ) { return &m_nodes[nNode]; } ;

   /// Returns the depth of a sub-tree
   uint32 GetDepth( uint32 nNode = 0 ) const;

private:

   /// Writes a description of the subchunk tree to a text file
   bool WriteToFile( SuFile* pFile ) const;

   /// Recursive helper method used during tree validation
   bool ValidateSubtree( uint32 nNode ) const;

   /// Recursive helper method used during tree construction
   static void Create( uint32 nRootIndex, const SuArray<SuModelSubChunk>& rSubChunks, TreeDesc* pTreeDesc, SuArray<SuSubChunkTreeNode>& rNodeArray  );

 
   /// Constructor is private to restrict instantiation to factory methods
   SuSubChunkTree( );

   SuArray<SuSubChunkTreeNode> m_nodes;   ///< Array of nodes.  The nodes are stored in a big global array to ensure that we have good cache coherence during traversal

};


#endif // _SU_SUBCHUNKTREE_H_
