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

#ifndef _SU_TRIANGLEMESH_H_
#define _SU_TRIANGLEMESH_H_

#include "SuPoint3.h"


//=========================================================================================================
/// \ingroup Common
/// \brief A class to encapsulate a simple, position-only triangle mesh.
///
/// Simple meshes of this sort are useful for picking or collision detection
/// 
//=========================================================================================================

// tolua_begin
class SuTriangleMesh
{
public:

// tolua_end
   /// Constructs a triangle mesh and initializes it with vertex and index data
   static SU_DLL SuTriangleMesh* CreateMesh( const SuArray<SuPoint3>& rVertices, const SuArray<uint32>& rIndices );

// tolua_begin

   /// Constructs a triangle mesh with space for the specified number of vertices and triangles
   static SU_DLL SuTriangleMesh* CreateMesh( uint32 nVertexCount, uint32 nTriCount );
  
   /// Retrieves the position of a particular vertex
   inline const SuPoint3& GetVertexPosition( uint32 i ) const { return m_vertexPositions[i]; };

   /// Retrieves the ith vertex index for a particular triangle
   inline uint32 GetVertexIndex( uint32 nTri, uint32 i ) const { return m_indices[ (3*nTri)+i ]; };
   
   /// Sets a vertex position 
   inline void SetVertexPosition( uint32 i, const SuPoint3& rPos ) { m_vertexPositions[i] = rPos; };

   /// Sets the vertex indices for a triangle
   inline void SetTriangleIndices( uint32 nTri, uint32 n0, uint32 n1, uint32 n2 ) 
   {
      uint32 nJ = 3*nTri;
      m_indices[ nJ ] = n0;
      m_indices[ nJ+1 ] = n1;
      m_indices[ nJ+2 ] = n2;
   };

   /// Returns the vertex count
   inline size_t GetVertexCount() const { return m_vertexPositions.size(); };

   /// Returns the triangle count
   inline size_t GetTriangleCount() const { return m_indices.size()/3; };

// tolua_end

   inline const SuArray<SuPoint3>& GetVertexArray() const { return m_vertexPositions; };
   inline const SuArray<uint32>& GetIndexArray() const { return m_indices; };

private:

   /// Private constructor to restrict instantiation to 'Create' factory method
   inline SuTriangleMesh( ) {};

   /// Constructor to create a mesh with a copy of vertex and index arrays
   inline SuTriangleMesh( const SuArray<SuPoint3>& rVerts, const SuArray<uint32>& rIndices )
      : m_vertexPositions( rVerts ), m_indices( rIndices )
   {
   }

   SuArray<SuPoint3> m_vertexPositions;
   SuArray<uint32> m_indices;

// tolua_begin
};
// tolua_end


#endif // _SU_TRIANGLEMESH_H_
