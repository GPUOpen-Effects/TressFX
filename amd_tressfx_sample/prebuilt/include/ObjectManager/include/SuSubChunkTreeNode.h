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

#ifndef _SU_SUBCHUNKTREENODE_H_
#define _SU_SUBCHUNKTREENODE_H_

#include "SuBoundingBox.h"

class SuModelSubChunk;

//=================================================================================================================================
/// \ingroup ObjectManager
/// \brief A node in a subchunk tree
/// 
//=================================================================================================================================
class SuSubChunkTreeNode
{
public:

   /// Constructor
   SuSubChunkTreeNode();

   /// Tests whether or not the node is a leaf
   inline bool IsLeaf() const { return ( m_nLeftChild == m_nRightChild ) && ( m_nRightChild == 0) ; };

   /// Returns the node's bounding box
   inline const SuBoundingBox& GetBoundingBox() const { return m_box; };

   inline uint32 GetFirstVertex() const { return m_nFirstVertex; };
   inline uint32 GetLastVertex() const { return m_nLastVertex; };
   inline uint32 GetFirstIndex() const { return m_nFirstIndex; };
   inline uint32 GetLastIndex() const { return m_nLastIndex; };

   /// Returns the vertex range covered by this node
   inline void GetVertexRange( uint32* pnFirstVertex, uint32* pnLastVertex ) const
   {
      *pnFirstVertex = m_nFirstVertex;
      *pnLastVertex = m_nLastVertex;
   };

   /// Returns the index range covered by this node
   inline void GetIndexRange( uint32* pnFirstIndex, uint32* pnLastIndex ) const
   {
      *pnFirstIndex = m_nFirstIndex;
      *pnLastIndex = m_nLastIndex;
   };

   /// Returns the index of the left child node, for an inner node.
   inline uint32 GetLeftChild() const { return m_nLeftChild; };

   /// Returns the index of the right child node, for an inner node.
   inline uint32 GetRightChild() const { return m_nRightChild; };
   
   /// Turns the node into a leaf node
   void MakeLeaf( const SuModelSubChunk& rSubChunk );

   /// Turns the node into an inner node
   void MakeInner( const SuSubChunkTreeNode& rLeftChild, const SuSubChunkTreeNode& rRightChild, uint32 nLeftChild, uint32 nRightChild );

   /// Generates a string describing this node
   void ToString( SuString* pStringOut ) const;

   /// Turns the node into the node described by a given string
   bool SetFromString( const SuString& rNode );

   /// Changes the vertex range
   inline void SetVertexRange( uint32 nFirst, uint32 nLast )
   {
      m_nFirstVertex = nFirst;
      m_nLastVertex  = nLast;
   };

   /// Changes the index range
   inline void SetIndexRange( uint32 nFirst, uint32 nLast )
   {
      m_nFirstIndex = nFirst;
      m_nLastIndex = nLast;
   };


private:

   SuBoundingBox m_box;          ///< Bounding box of subtree
   uint32 m_nFirstVertex;        ///< First vertex covered by the node
   uint32 m_nLastVertex;         ///< Last vertex covered by the node
   uint32 m_nFirstIndex;         ///< First index covered by the node
   uint32 m_nLastIndex;          ///< Last index covered by the node
   uint32 m_nLeftChild;          ///< Index of left child in subchunk tree.  0 if leaf
   uint32 m_nRightChild;         ///< Index of right child in subchunk tree.  0 if leaf

};


#endif // _SU_SUBCHUNKTREENODE_H_
