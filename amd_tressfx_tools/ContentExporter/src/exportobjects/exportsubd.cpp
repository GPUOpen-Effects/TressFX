//-------------------------------------------------------------------------------------
// ExportSubD.cpp
//
// Classes representing Catmull-Clark subdivision surfaces.
//  
// THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO
// THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
// PARTICULAR PURPOSE.
//  
// Advanced Technology Group (ATG)
// Copyright (C) Microsoft Corporation. All rights reserved.
//
// http://go.microsoft.com/fwlink/?LinkId=226208
//-------------------------------------------------------------------------------------

#include "stdafx.h"
#include "ExportSubD.h"

using namespace DirectX;

extern ATG::ExportScene* g_pScene;

namespace ATG
{
    ExportSubDProcessMesh::ExportSubDProcessMesh()
        : m_pPolyMesh( nullptr ),
          m_pQuadPatchDataVB( nullptr ),
          m_pTrianglePatchDataVB( nullptr ),
          m_pQuadPatchIB( nullptr ),
          m_pTrianglePatchIB( nullptr )
    {
    }

    void ExportSubDProcessMesh::Initialize( ExportMesh* pMesh )
    {
        m_pPolyMesh = pMesh;
        BuildMesh();
        BuildBoundaryEdgeTable();
        CreateDegenerateGeometry();
        ComputeAdjacency();
        SortPatches();
        BuildQuadPatchBuffer();
        BuildTriPatchBuffer();
        ConvertSubsets();

        ClearIntermediateBuffers();
    }

    void ExportSubDProcessMesh::ByteSwap()
    {
        if( m_pQuadPatchDataVB )
        {
            //m_pQuadPatchDataVB->ByteSwap( GetPatchDataDecl(), GetPatchDataDeclElementCount() );
        }
        if( m_pQuadPatchIB )
        {
            m_pQuadPatchIB->ByteSwap();
        }
        if( m_pTrianglePatchDataVB )
        {
            //m_pTrianglePatchDataVB->ByteSwap( GetPatchDataDecl(), GetPatchDataDeclElementCount() );
        }
        if( m_pTrianglePatchIB )
        {
            m_pTrianglePatchIB->ByteSwap();
        }
    }

    void ExportSubDProcessMesh::ClearIntermediateBuffers()
    {
        m_Quads.clear();
        m_Triangles.clear();
        m_Positions.clear();
        m_MeshVertexToPositionMapping.clear();
        m_PositionToMeshVertexMapping.clear();
        m_PositionToDegeneratePositionMapping.clear();
        m_IncidentBoundaryEdgesPerPosition.clear();
        m_BoundaryEdges.clear();
    }

    void ExportSubDProcessMesh::BuildMesh()
    {
        assert( m_pPolyMesh != nullptr );

        m_Positions.clear();
        m_MeshVertexToPositionMapping.clear();
        m_PositionToMeshVertexMapping.clear();

        const size_t dwVertexCount = m_pPolyMesh->GetVB()->GetVertexCount();
        m_MeshVertexToPositionMapping.resize( dwVertexCount, -1 );

        // compute triangle count
        const size_t dwIndexCount = m_pPolyMesh->GetIB()->GetIndexCount();
        assert( dwIndexCount % 3 == 0 );
        const size_t dwTriangleCount = dwIndexCount / 3;

        ExportLog::LogMsg( 4, "Processing %Iu verts and %Iu triangles into a subdivision surface control mesh.", dwVertexCount, dwTriangleCount );

        INT iCurrentPolyIndex = -1;
        INT iCurrentPolySize = 0;

        INT iCurrentPolyPositionIndices[4];
        INT iCurrentPolyMeshIndices[4];

        const size_t dwSubsetCount = m_pPolyMesh->GetSubsetCount();
        for( size_t dwSubsetIndex = 0; dwSubsetIndex < dwSubsetCount; ++dwSubsetIndex )
        {
            const ExportIBSubset* pSubset = m_pPolyMesh->GetSubset( dwSubsetIndex );
            const DWORD dwSubsetTriangleCount = pSubset->GetIndexCount() / 3;
            const DWORD dwSubsetBase = pSubset->GetStartIndex() / 3;
            iCurrentPolyIndex = -1;
            iCurrentPolySize = 0;

            for( DWORD dwSubsetTriangleIndex = 0; dwSubsetTriangleIndex < dwSubsetTriangleCount; ++dwSubsetTriangleIndex )
            {
                DWORD dwTriangleIndex = dwSubsetBase + dwSubsetTriangleIndex;
                INT iPolyIndex = m_pPolyMesh->GetPolygonForTriangle( dwTriangleIndex );

                // Poly indices should monotonically increase
                assert( iPolyIndex >= iCurrentPolyIndex );

                if( iPolyIndex > iCurrentPolyIndex )
                {
                    // moving to the next poly index

                    // do we have an accumulated triangle?
                    assert( iCurrentPolySize < 2 );
                    if( iCurrentPolySize == 1 )
                    {
                        AddTriangle( iCurrentPolyIndex, static_cast<INT>( dwSubsetIndex ), iCurrentPolyPositionIndices, iCurrentPolyMeshIndices );
                    }

                    // clear out the poly state
                    ZeroMemory( iCurrentPolyPositionIndices, sizeof( iCurrentPolyPositionIndices ) );
                    ZeroMemory( iCurrentPolyMeshIndices, sizeof( iCurrentPolyMeshIndices ) );
                    iCurrentPolySize = 1;

                    // increment the current poly index
                    iCurrentPolyIndex = iPolyIndex;
                }
                else
                {
                    // we have encountered the second triangle in a polygon
                    // increment the current poly size
                    assert( iCurrentPolySize < 2 );
                    ++iCurrentPolySize;
                }

                // make sure current poly size is either 1 (triangle) or 2 (quad)
                assert( iCurrentPolySize == 1 || iCurrentPolySize == 2 );

                // iterate through triangle corners
                INT iTriangleIndices[3] = { 0 };
                INT iTriangleMeshIndices[3] = { 0 };
                DWORD dwBaseIndex = dwTriangleIndex * 3;
                for( DWORD dwCornerIndex = 0; dwCornerIndex < 3; ++dwCornerIndex )
                {
                    INT iMeshIndex = static_cast<INT>( m_pPolyMesh->GetIB()->GetIndex( dwBaseIndex + dwCornerIndex ) );
                    auto pPosition = reinterpret_cast<const XMFLOAT3*>( m_pPolyMesh->GetVB()->GetVertex( iMeshIndex ) );

                    INT iPositionIndex = CreateOrAddPosition( *pPosition, iMeshIndex );
                    iTriangleIndices[dwCornerIndex] = iPositionIndex;
                    iTriangleMeshIndices[dwCornerIndex] = iMeshIndex;
                }

                // add the current triangle's position indices to the current poly position indices
                if( iCurrentPolySize == 1 )
                {
                    // triangle - just copy
                    iCurrentPolyPositionIndices[0] = iTriangleIndices[0];
                    iCurrentPolyPositionIndices[1] = iTriangleIndices[1];
                    iCurrentPolyPositionIndices[2] = iTriangleIndices[2];
                    iCurrentPolyPositionIndices[3] = -1;
                    iCurrentPolyMeshIndices[0] = iTriangleMeshIndices[0];
                    iCurrentPolyMeshIndices[1] = iTriangleMeshIndices[1];
                    iCurrentPolyMeshIndices[2] = iTriangleMeshIndices[2];
                    iCurrentPolyMeshIndices[3] = -1;
                }
                else if( iCurrentPolySize == 2 )
                {
                    // quad - add the unique vertex
                    for( DWORD i = 0; i < 3; ++i )
                    {
                        if( iTriangleIndices[i] != iCurrentPolyPositionIndices[0] && iTriangleIndices[i] != iCurrentPolyPositionIndices[1] && iTriangleIndices[i] != iCurrentPolyPositionIndices[2] )
                        {
                            iCurrentPolyPositionIndices[3] = iTriangleIndices[i];
                            iCurrentPolyMeshIndices[3] = iTriangleMeshIndices[i];
                            break;
                        }
                    }
                    assert( iCurrentPolyPositionIndices[3] != -1 );

                    // now add the quad
                    AddQuad( iCurrentPolyIndex, static_cast<INT>( dwSubsetIndex ), iCurrentPolyPositionIndices, iCurrentPolyMeshIndices );

                    // and reset the poly state
                    iCurrentPolySize = 0;
                }
            }

            // add the last remaining triangle, if one exists
            assert( iCurrentPolySize < 2 );
            if( iCurrentPolySize == 1 )
            {
                AddTriangle( iCurrentPolyIndex, static_cast<INT>( dwSubsetIndex ), iCurrentPolyPositionIndices, iCurrentPolyMeshIndices );
                iCurrentPolySize = 0;
            }
        }

        // sanity check our final triangle and quad counts
        assert( dwTriangleCount == ( m_Triangles.size() + m_Quads.size() * 2 ) );

        ExportLog::LogMsg( 3, "Subdivision surface control mesh complete; %Iu triangles and %Iu quads found, %Iu unique positions.", m_Triangles.size(), m_Quads.size(), m_Positions.size() );
    }

    INT ExportSubDProcessMesh::CreateOrAddPosition( const XMFLOAT3& vPosition, INT iMeshVertexIndex )
    {
        assert( iMeshVertexIndex >= 0 && iMeshVertexIndex < static_cast<INT>( m_pPolyMesh->GetVB()->GetVertexCount() ) );
        INT iCurrentIndex = m_MeshVertexToPositionMapping[iMeshVertexIndex];
        if( iCurrentIndex == -1 )
        {
            XMVECTOR p0 = XMLoadFloat3( &vPosition );

            size_t dwPositionCount = m_Positions.size();
            for( size_t i = 0; i < dwPositionCount; ++i )
            {
                XMVECTOR pi = XMLoadFloat3( &m_Positions[i] );
                if ( XMVector3Equal( p0, pi ) )
                {
                    iCurrentIndex = static_cast<INT>( i );
                    m_MeshVertexToPositionMapping[iMeshVertexIndex] = iCurrentIndex;
                    return iCurrentIndex;
                }
            }
            iCurrentIndex = static_cast<INT>( dwPositionCount );
            m_Positions.push_back( vPosition );
            m_PositionToMeshVertexMapping.push_back( iMeshVertexIndex );
            m_MeshVertexToPositionMapping[iMeshVertexIndex] = iCurrentIndex;
            return iCurrentIndex;
        }
        else
        {
            return iCurrentIndex;
        }
    }

    void ExportSubDProcessMesh::AddTriangle( INT iPolyIndex, INT iSubsetIndex, const INT* pIndices, const INT* pMeshIndices )
    {
        Triangle tri;
        tri.iPolyIndex = iPolyIndex;
        tri.iMeshSubsetIndex = iSubsetIndex;
        tri.iIndices[0] = pIndices[0];
        tri.iIndices[1] = pIndices[1];
        tri.iIndices[2] = pIndices[2];
        tri.iMeshIndices[0] = pMeshIndices[0];
        tri.iMeshIndices[1] = pMeshIndices[1];
        tri.iMeshIndices[2] = pMeshIndices[2];

        m_Triangles.push_back( tri );
    }

    void ExportSubDProcessMesh::AddQuad( INT iPolyIndex, INT iSubsetIndex, const INT* pIndices, const INT* pMeshIndices )
    {
        Quad quad;
        quad.iPolyIndex = iPolyIndex;
        quad.iMeshSubsetIndex = iSubsetIndex;
        quad.bDegenerate = false;
        if( g_pScene->Settings().bFlipTriangles )
        {
            // the combination of two flipped triangles causes a bowtie
            // reorder the quad to fix it
            quad.iIndices[0] = pIndices[0];
            quad.iIndices[1] = pIndices[3];
            quad.iIndices[2] = pIndices[1];
            quad.iIndices[3] = pIndices[2];
            quad.iMeshIndices[0] = pMeshIndices[0];
            quad.iMeshIndices[1] = pMeshIndices[3];
            quad.iMeshIndices[2] = pMeshIndices[1];
            quad.iMeshIndices[3] = pMeshIndices[2];
        }
        else
        {
            quad.iIndices[0] = pIndices[0];
            quad.iIndices[1] = pIndices[1];
            quad.iIndices[2] = pIndices[2];
            quad.iIndices[3] = pIndices[3];
            quad.iMeshIndices[0] = pMeshIndices[0];
            quad.iMeshIndices[1] = pMeshIndices[1];
            quad.iMeshIndices[2] = pMeshIndices[2];
            quad.iMeshIndices[3] = pMeshIndices[3];
        }

        bool bDegenerate = false;
        for( DWORD i = 0; i < 4; ++i )
        {
            for( DWORD j = i + 1; j < 4; ++j )
            {
                if( pIndices[i] == pIndices[j] || pMeshIndices[i] == pMeshIndices[j] )
                {
                    bDegenerate = true;
                }
            }
        }
        if( bDegenerate )
        {
            ExportLog::LogWarning( "Degenerate quad detected with poly index %d.", iPolyIndex );
        }

        m_Quads.push_back( quad );
    }

    void ExportSubDProcessMesh::BuildBoundaryEdgeTable()
    {
        assert( m_pPolyMesh != nullptr );

        m_BoundaryEdges.clear();

        size_t dwTotalEdgeCount = m_Triangles.size() * 3 + m_Quads.size() * 4;
        ExportLog::LogMsg( 4, "Scanning %Iu edges in control mesh for boundary edges.", dwTotalEdgeCount );

        // add the edges in all of the triangles
        const size_t dwTriangleCount = m_Triangles.size();
        for( size_t dwTriangleIndex = 0; dwTriangleIndex < dwTriangleCount; ++dwTriangleIndex )
        {
            const Triangle& tri = m_Triangles[dwTriangleIndex];

            for( INT iEdgeIndex = 0; iEdgeIndex < 3; ++iEdgeIndex )
            {
                INT iPositionIndexA = tri.iIndices[ iEdgeIndex ];
                INT iPositionIndexB = tri.iIndices[ ( iEdgeIndex + 1 ) % 3 ];
                AddOrRemoveEdge( iPositionIndexA, iPositionIndexB, static_cast<INT>( dwTriangleIndex ), -1, iEdgeIndex );
            }
        }

        // add the edges in all of the quads
        const size_t dwQuadCount = m_Quads.size();
        for( size_t dwQuadIndex = 0; dwQuadIndex < dwQuadCount; ++dwQuadIndex )
        {
            const Quad& quad = m_Quads[dwQuadIndex];

            for( INT iEdgeIndex = 0; iEdgeIndex < 4; ++iEdgeIndex )
            {
                INT iPositionIndexA = quad.iIndices[ iEdgeIndex ];
                INT iPositionIndexB = quad.iIndices[ ( iEdgeIndex + 1 ) % 4 ];
                AddOrRemoveEdge( iPositionIndexA, iPositionIndexB, -1, static_cast<INT>( dwQuadIndex ), iEdgeIndex );
            }
        }

        const size_t dwBoundaryEdgeCount = m_BoundaryEdges.size();
        const size_t dwPositionCount = m_Positions.size();

        // anything left over in the boundary edge table is a boundary edge
        ExportLog::LogMsg( 3, "Control mesh has %Iu boundary edges.", dwBoundaryEdgeCount );

        // scan for more than 2 incident boundary edges on any position vertex
        m_IncidentBoundaryEdgesPerPosition.clear();
        m_IncidentBoundaryEdgesPerPosition.resize( dwPositionCount, 0 );

        EdgeMap::iterator iter = m_BoundaryEdges.begin();
        EdgeMap::iterator end = m_BoundaryEdges.end();
        while( iter != end )
        {
            const Edge& BoundaryEdge = (*iter).second;

            // decode the edge into start and end position indices
            INT iPositionIndexA = -1;
            INT iPositionIndexB = -1;
            if( BoundaryEdge.iTriangleIndex >= 0 )
            {
                const Triangle& tri = m_Triangles[BoundaryEdge.iTriangleIndex];
                iPositionIndexA = tri.iIndices[ BoundaryEdge.iLocalIndex ];
                iPositionIndexB = tri.iIndices[ ( BoundaryEdge.iLocalIndex + 1 ) % 3 ];
            }
            else
            {
                const Quad& quad = m_Quads[BoundaryEdge.iQuadIndex];
                iPositionIndexA = quad.iIndices[ BoundaryEdge.iLocalIndex ];
                iPositionIndexB = quad.iIndices[ ( BoundaryEdge.iLocalIndex + 1 ) % 4 ];
            }

            // increment the incident boundary edge counter for each position
            m_IncidentBoundaryEdgesPerPosition[iPositionIndexA]++;
            m_IncidentBoundaryEdgesPerPosition[iPositionIndexB]++;

            ++iter;
        }

        bool bInvalidData = false;
        for( DWORD i = 0; i < dwPositionCount; ++i )
        {
            if( m_IncidentBoundaryEdgesPerPosition[i] > 2 )
            {
                XMFLOAT3 vPos = m_Positions[i];
                ExportLog::LogWarning( "Detected %d incident boundary edges on position %u, which will result in invalid adjacency data.  Location: <%0.3f %0.3f %0.3f>", m_IncidentBoundaryEdgesPerPosition[i], i, vPos.x, vPos.y, vPos.z );
                bInvalidData = true;
            }
        }
        if( bInvalidData )
        {
            ExportLog::LogError( "Adjacency data is invalid due to non-manifold boundary edge geometry." );
        }
    }

    INT ExportSubDProcessMesh::AddOrRemoveEdge( INT iPositionIndexA, INT iPositionIndexB, INT iTriangleIndex, INT iQuadIndex, INT iLocalIndex )
    {
        assert( iPositionIndexA != iPositionIndexB );
        assert( iPositionIndexA >= 0 );
        assert( iPositionIndexB >= 0 );

        // compute a unique hash for this edge, with the smaller index in the MSW and the larger index in the LSW
        UINT64 EdgeHashKey = 0;
        if( iPositionIndexA < iPositionIndexB )
        {
            EdgeHashKey = ( (UINT64)iPositionIndexA << 32 ) | (UINT64)iPositionIndexB;
        }
        else
        {
            EdgeHashKey = ( (UINT64)iPositionIndexB << 32 ) | (UINT64)iPositionIndexA;
        }

        // search for the edge in the table
        EdgeMap::iterator iter = m_BoundaryEdges.find( EdgeHashKey );

        if( iter == m_BoundaryEdges.end() )
        {
            // we did not find the edge; add a new edge data structure to the table
            Edge e = { 0 };
            e.iTriangleIndex = iTriangleIndex;
            e.iQuadIndex = iQuadIndex;
            e.iLocalIndex = iLocalIndex;
            m_BoundaryEdges[EdgeHashKey] = e;
            return 1;
        }
        else
        {
            // we found the edge, meaning that this edge has been encountered twice
            // remove the edge from the table
            m_BoundaryEdges.erase( iter );
            return 2;
        }
    }

    void ExportSubDProcessMesh::CreateDegenerateGeometry()
    {
        if( m_BoundaryEdges.empty() )
            return;

        ExportLog::LogMsg( 4, "Creating %Iu degenerate quads from %Iu boundary edges.", m_BoundaryEdges.size(), m_BoundaryEdges.size() );

        const size_t dwPositionCount = m_Positions.size();

        // initialize the position to degenerate position map
        m_PositionToDegeneratePositionMapping.clear();
        m_PositionToDegeneratePositionMapping.resize( dwPositionCount, -1 );

        // walk through the boundary edges
        EdgeMap::iterator iter = m_BoundaryEdges.begin();
        EdgeMap::iterator end = m_BoundaryEdges.end();
        while( iter != end )
        {
            const Edge& BoundaryEdge = (*iter).second;

            // decode the edge into start and end position indices
            INT iPositionIndexA = -1;
            INT iPositionIndexB = -1;
            if( BoundaryEdge.iTriangleIndex >= 0 )
            {
                const Triangle& tri = m_Triangles[BoundaryEdge.iTriangleIndex];
                iPositionIndexA = tri.iIndices[ BoundaryEdge.iLocalIndex ];
                iPositionIndexB = tri.iIndices[ ( BoundaryEdge.iLocalIndex + 1 ) % 3 ];
            }
            else
            {
                const Quad& quad = m_Quads[BoundaryEdge.iQuadIndex];
                iPositionIndexA = quad.iIndices[ BoundaryEdge.iLocalIndex ];
                iPositionIndexB = quad.iIndices[ ( BoundaryEdge.iLocalIndex + 1 ) % 4 ];
            }

            assert( iPositionIndexA != -1 && iPositionIndexB != -1 );

            // create a degenerate position for each edge position
            INT iPositionIndexAPrime = CreateOrAddDegeneratePosition( iPositionIndexA );
            INT iPositionIndexBPrime = CreateOrAddDegeneratePosition( iPositionIndexB );

            // create a degenerate quad
            Quad quad;
            quad.bDegenerate = true;
            quad.iPolyIndex = -1;
            if( g_pScene->Settings().bFlipTriangles )
            {
                quad.iIndices[0] = iPositionIndexAPrime;
                quad.iIndices[1] = iPositionIndexA;
                quad.iIndices[2] = iPositionIndexB;
                quad.iIndices[3] = iPositionIndexBPrime;
            }
            else
            {
                quad.iIndices[0] = iPositionIndexAPrime;
                quad.iIndices[1] = iPositionIndexBPrime;
                quad.iIndices[2] = iPositionIndexB;
                quad.iIndices[3] = iPositionIndexA;
            }
            m_Quads.push_back( quad );

            ++iter;
        }

        // clear the boundary edges table
        m_BoundaryEdges.clear();

        ExportLog::LogMsg( 4, "Degenerate quads created." );
    }

    INT ExportSubDProcessMesh::CreateOrAddDegeneratePosition( INT iPositionIndex )
    {
        assert( iPositionIndex >= 0 && iPositionIndex < static_cast<INT>( m_PositionToDegeneratePositionMapping.size() ) );

        // search for a degenerate position for this position index
        size_t iDegeneratePositionIndex = m_PositionToDegeneratePositionMapping[iPositionIndex];
        if( iDegeneratePositionIndex == -1 )
        {
            // create a new degenerate position, which is equivalent to its position
            iDegeneratePositionIndex = m_Positions.size();
            m_Positions.push_back( m_Positions[iPositionIndex] );
            m_PositionToMeshVertexMapping.push_back( m_PositionToMeshVertexMapping[iPositionIndex] );
            // update the position to degenerate position map
            m_PositionToDegeneratePositionMapping[iPositionIndex] = static_cast<UINT>( iDegeneratePositionIndex );
        }
        return static_cast<INT>( iDegeneratePositionIndex );
    }

    void ExportSubDProcessMesh::ComputeAdjacency()
    {
        const size_t dwTriangleCount = m_Triangles.size();
        if( dwTriangleCount > 0 )
        {
            ExportLog::LogMsg( 4, "Computing triangle adjacency." );

            for( size_t i = 0; i < dwTriangleCount; ++i )
            {
                ComputeTriangleAdjacency( static_cast<INT>( i ) );
            }
        }

        const size_t dwQuadCount = m_Quads.size();
        if( dwQuadCount > 0 )
        {
            ExportLog::LogMsg( 4, "Computing quad adjacency." );

            DWORD dwRegularQuadCount = 0;
            DWORD dwExtraordinaryQuadCount = 0;

            std::deque<INT> ValenceTwoQuads;

            for( size_t i = 0; i < dwQuadCount; ++i )
            {
                if( m_Quads[i].bDegenerate )
                    continue;

                ComputeQuadAdjacency( static_cast<INT>( i ) );

                const Quad& ProcessedQuad = m_Quads[i];
                if( ProcessedQuad.bValence[0] == 4 &&
                    ProcessedQuad.bValence[1] == 4 &&
                    ProcessedQuad.bValence[2] == 4 &&
                    ProcessedQuad.bValence[3] == 4 )
                {
                    ++dwRegularQuadCount;
                }
                else
                {
                    ++dwExtraordinaryQuadCount;
                }

                if( ProcessedQuad.bValence[0] == 2 ||
                    ProcessedQuad.bValence[1] == 2 ||
                    ProcessedQuad.bValence[2] == 2 ||
                    ProcessedQuad.bValence[3] == 2 )
                {
                    ValenceTwoQuads.push_back( static_cast<INT>( i ) );
                }
            }

            float fTotalQuadCount = (float)( dwRegularQuadCount + dwExtraordinaryQuadCount );
            float fRegularPercent = 100.0f * (float)dwRegularQuadCount / fTotalQuadCount;

            ExportLog::LogMsg( 3, "%u regular quads (%0.0f%% of total), %u extraordinary quads.", dwRegularQuadCount, fRegularPercent, dwExtraordinaryQuadCount );

            RemoveValenceTwoQuads( ValenceTwoQuads );
        }
    }

    void ExportSubDProcessMesh::ComputeTriangleAdjacency( const INT iTriangleIndex )
    {
        Triangle& tri = m_Triangles[iTriangleIndex];

        // This resizable array stores the neighbor list while we're computing adjacency
        std::vector< INT > Neighbors;

        ExportLog::LogMsg( 5, "Computing adjacency for triangle %d: <%d %d %d>", iTriangleIndex, tri.iIndices[0], tri.iIndices[1], tri.iIndices[2] );

        // Run a "sweep" for each corner
        for( INT iCornerIndex = 0; iCornerIndex < 3; ++iCornerIndex )
        {
            // Set up our position indices we'll be using to sweep through neighboring triangles and quads
            // The pivot is the triangle corner, and does not change
            // The sweep starts at the previous triangle corner, and stops at the next triangle corner
            INT iSweepPositionIndex = tri.iIndices[ ( iCornerIndex + 2 ) % 3 ];
            INT iPivotPositionIndex = tri.iIndices[ iCornerIndex ];
            INT iStopPositionIndex = tri.iIndices[ ( iCornerIndex + 1 ) % 3 ];

            ExportLog::LogMsg( 5, "Corner %d, position index %d", iCornerIndex, iPivotPositionIndex );

            tri.bValence[iCornerIndex] = static_cast<BYTE>( ExecuteSweep( iPivotPositionIndex, iSweepPositionIndex, iStopPositionIndex, -1, iTriangleIndex, Neighbors ) );

            // The prefix is the index of the end of this corner's neighbor list.
            // We add 3 since the neighbor list will be appended to the 3 triangle indices in the final per-triangle buffer.
            tri.bPrefix[iCornerIndex] = static_cast<BYTE>( Neighbors.size() + 3 );

            ExportLog::LogMsg( 5, "Valence %u, prefix %u", tri.bValence[iCornerIndex], tri.bPrefix[iCornerIndex] );
        }

        if( Neighbors.size() > MAX_TRIANGLE_NEIGHBOR_COUNT )
        {
            ExportLog::LogWarning( "Triangle %d contains more than %d neighbors.  Truncating neighbor list; this will create invalid data.", iTriangleIndex, MAX_TRIANGLE_NEIGHBOR_COUNT );
        }

        // Store the neighbor list
        for( DWORD i = 0; i < MAX_TRIANGLE_NEIGHBOR_COUNT; ++i )
        {
            if( i < Neighbors.size() )
            {
                tri.iNeighbors[i] = Neighbors[i];
            }
            else
            {
                tri.iNeighbors[i] = -1;
            }
        }
    }

    void ExportSubDProcessMesh::ComputeQuadAdjacency( const INT iQuadIndex )
    {
        Quad& quad = m_Quads[iQuadIndex];

        // This resizable array stores the neighbor list while we're computing adjacency
        std::vector< INT > Neighbors;

        ExportLog::LogMsg( 5, "Computing adjacency for quad %d: <%d %d %d %d>", iQuadIndex, quad.iIndices[0], quad.iIndices[1], quad.iIndices[2], quad.iIndices[3] );

        // Run a "sweep" for each corner
        for( INT iCornerIndex = 0; iCornerIndex < 4; ++iCornerIndex )
        {
            // Set up our position indices we'll be using to sweep through neighboring triangles and quads
            // The pivot is the quad corner, and does not change
            // The sweep starts at the previous quad corner, and stops at the next quad corner
            INT iSweepPositionIndex = quad.iIndices[ ( iCornerIndex + 3 ) % 4 ];
            INT iPivotPositionIndex = quad.iIndices[ iCornerIndex ];
            INT iStopPositionIndex = quad.iIndices[ ( iCornerIndex + 1 ) % 4 ];

            ExportLog::LogMsg( 5, "Corner %d, position index %d", iCornerIndex, iPivotPositionIndex );

            quad.bValence[iCornerIndex] = static_cast<BYTE>( ExecuteSweep( iPivotPositionIndex, iSweepPositionIndex, iStopPositionIndex, iQuadIndex, -1, Neighbors ) );

            // The prefix is the index of the end of this corner's neighbor list.
            // We add 4 since the neighbor list will be appended to the 4 quad indices in the final per-quad buffer.
            quad.bPrefix[iCornerIndex] = static_cast<BYTE>( Neighbors.size() + 4 );

            ExportLog::LogMsg( 5, "Valence %u, prefix %u", quad.bValence[iCornerIndex], quad.bPrefix[iCornerIndex] );
        }

        if( Neighbors.size() > MAX_QUAD_NEIGHBOR_COUNT )
        {
            ExportLog::LogWarning( "Quad %d contains more than %d neighbors.  Truncating neighbor list; this will create invalid data.", iQuadIndex, MAX_QUAD_NEIGHBOR_COUNT );
        }

        // Store the neighbor list
        for( DWORD i = 0; i < MAX_QUAD_NEIGHBOR_COUNT; ++i )
        {
            if( i < Neighbors.size() )
            {
                quad.iNeighbors[i] = Neighbors[i];
            }
            else
            {
                quad.iNeighbors[i] = -1;
            }
        }
    }

    INT ExportSubDProcessMesh::ExecuteSweep( INT iPivotPositionIndex, INT iSweepPositionIndex, INT iStopPositionIndex, INT iStartQuadIndex, INT iStartTriangleIndex, std::vector<INT>& Neighbors )
    {
        INT iValence = 2;

        // The pivot, sweep, and stop position indices should all be valid
        assert( iPivotPositionIndex != -1 && iSweepPositionIndex != -1 && iStopPositionIndex != -1 );

        // Either the start quad or start triangle index should be valid, but not both
        assert( ( iStartQuadIndex == -1 ) ^ ( iStartTriangleIndex == -1 ) );

        // Determine the triangle or quad where the sweep for this corner stops
        INT iStopTriangleIndex = FindTriangleWithEdge( iPivotPositionIndex, iStopPositionIndex, iStartTriangleIndex );
        INT iStopQuadIndex = FindQuadWithEdge( iPivotPositionIndex, iStopPositionIndex, iStartQuadIndex );

        // Start the sweep at this polygon
        INT iCurrentQuadIndex = iStartQuadIndex;
        INT iCurrentTriangleIndex = iStartTriangleIndex;

        bool bSweepComplete = false;
        bool bFirstSweep = true;

        while( !bSweepComplete )
        {
            if( iValence >= MAX_QUAD_NEIGHBOR_COUNT )
            {
                ExportLog::LogError( "Maximum valence encountered.  Terminating sweep to prevent infinite loop." );
                bSweepComplete = true;
                continue;
            }

            // Look for a quad with the active edge, except for the current quad
            INT iNextQuadIndex = FindQuadWithEdge( iSweepPositionIndex, iPivotPositionIndex, iCurrentQuadIndex );
            if( iNextQuadIndex != -1 )
            {
                // Determine if we have reached the stop quad
                if( iNextQuadIndex == iStopQuadIndex )
                {
                    bSweepComplete = true;
                    continue;
                }

                ++iValence;

                // Move our current sweep quad to the next sweep quad we have identified
                iCurrentQuadIndex = iNextQuadIndex;
                iCurrentTriangleIndex = -1;

                // Look for the edge in the quad the contains the pivot point, but not the sweep point
                INT iNextSweepPositionIndex = GetNextPositionIndexInQuad( iCurrentQuadIndex, iPivotPositionIndex, iSweepPositionIndex );
                assert( iNextSweepPositionIndex != -1 );

                // If we're not in the first quad of the sweep, add the opposite point to the neighbor list
                if( !bFirstSweep )
                {
                    // Get the point that is opposite the pivot in the quad
                    INT iOppositePositionIndex = GetOppositePositionIndexInQuad( iCurrentQuadIndex, iPivotPositionIndex );
                    Neighbors.push_back( iOppositePositionIndex );
                    ExportLog::LogMsg( 5, "Neighbor %d", iOppositePositionIndex );
                }

                // Add the next sweep point to the neighbor list
                Neighbors.push_back( iNextSweepPositionIndex );
                ExportLog::LogMsg( 5, "Neighbor %d", iNextSweepPositionIndex );

                // Move our sweep point
                iSweepPositionIndex = iNextSweepPositionIndex;
                bFirstSweep = false;
                continue;
            }

            // Look for a triangle with the active edge, except for the current triangle
            INT iNextTriIndex = FindTriangleWithEdge( iSweepPositionIndex, iPivotPositionIndex, iCurrentTriangleIndex );
            if( iNextTriIndex != -1 )
            {
                // Determine if we have reached the stop triangle
                if( iNextTriIndex == iStopTriangleIndex )
                {
                    bSweepComplete = true;
                    continue;
                }

                ++iValence;

                // Move our current sweep triangle to the next sweep triangle we have identified
                iCurrentTriangleIndex = iNextTriIndex;
                iCurrentQuadIndex = -1;

                // Look for the edge in the triangle the contains the pivot point, but not the sweep point
                INT iNextSweepPositionIndex = GetNextPositionIndexInTriangle( iCurrentTriangleIndex, iPivotPositionIndex, iSweepPositionIndex );
                assert( iNextSweepPositionIndex != -1 );

                // Add the next sweep point to the neighbor list
                Neighbors.push_back( iNextSweepPositionIndex );
                ExportLog::LogMsg( 5, "Neighbor %d", iNextSweepPositionIndex );

                // Move our sweep point
                iSweepPositionIndex = iNextSweepPositionIndex;
                bFirstSweep = false;
                continue;
            }

            // Neither quad nor triangle was found
            // End the sweep for this corner
            bSweepComplete = true;
        }

        if( iValence <= 2 && iStartQuadIndex != -1 )
        {
            XMFLOAT3 vQuadCenter = GetQuadCenter( iStartQuadIndex );
            ExportLog::LogWarning( "Valence of 2 encountered in quad %d.  This indicates neighboring quads sharing 2 edges.  Quad will be merged with the adjacent quad.  Quad center: <%0.3f %0.3f %0.3f>", iStartQuadIndex, vQuadCenter.x, vQuadCenter.y, vQuadCenter.z );
        }

        return iValence;
    }

    XMFLOAT3 ExportSubDProcessMesh::GetQuadCenter( INT iQuadIndex )
    {
        const Quad& CurrentQuad = m_Quads[iQuadIndex];
        
        XMVECTOR vCenter = XMVectorZero();

        for( DWORD i = 0; i < 4; ++i )
        {
            XMVECTOR pi = XMLoadFloat3( &m_Positions[ CurrentQuad.iIndices[i] ] );
            vCenter = XMVectorAdd( vCenter, pi );
        }

        vCenter *= 0.25f;

        XMFLOAT3 v;
        XMStoreFloat3( &v, vCenter );
        return v;
    }

    INT ExportSubDProcessMesh::FindTriangleWithEdge( INT iStartPositionIndex, INT iEndPositionIndex, INT iExcludeThisTriangle )
    {
        INT iTriangleCount = static_cast<INT>( m_Triangles.size() );
        for( INT i = 0; i < iTriangleCount; ++i )
        {
            if( i == iExcludeThisTriangle )
                continue;

            const Triangle& tri = m_Triangles[i];

            if( tri.iIndices[0] != iStartPositionIndex && tri.iIndices[1] != iStartPositionIndex && tri.iIndices[2] != iStartPositionIndex )
                continue;

            if( tri.iIndices[0] != iEndPositionIndex && tri.iIndices[1] != iEndPositionIndex && tri.iIndices[2] != iEndPositionIndex )
                continue;

            return i;
        }
        return -1;
    }

    INT ExportSubDProcessMesh::FindQuadWithEdge( INT iStartPositionIndex, INT iEndPositionIndex, INT iExcludeThisQuad )
    {
        INT iQuadCount = static_cast<INT>( m_Quads.size() );
        for( INT i = 0; i < iQuadCount; ++i )
        {
            if( i == iExcludeThisQuad )
                continue;

            const Quad& quad = m_Quads[i];
            for( INT iEdge = 0; iEdge < 4; ++iEdge )
            {
                INT iCornerA = quad.iIndices[iEdge];
                INT iCornerB = quad.iIndices[ ( iEdge + 1 ) % 4 ];
                if( iCornerA == iStartPositionIndex && iCornerB == iEndPositionIndex )
                    return i;
                if( iCornerB == iStartPositionIndex && iCornerA == iEndPositionIndex )
                    return i;
            }
        }
        return -1;
    }

    INT ExportSubDProcessMesh::FindLocalIndexInTriangle( INT iTriangleIndex, INT iPositionIndex )
    {
        const Triangle& tri = m_Triangles[iTriangleIndex];
        for( INT i = 0; i < 3; ++i )
        {
            if( tri.iIndices[i] == iPositionIndex )
                return i;
        }
        return -1;
    }

    INT ExportSubDProcessMesh::FindLocalIndexInQuad( INT iQuadIndex, INT iPositionIndex )
    {
        const Quad& quad = m_Quads[iQuadIndex];
        for( INT i = 0; i < 4; ++i )
        {
            if( quad.iIndices[i] == iPositionIndex )
                return i;
        }
        return -1;
    }

    INT ExportSubDProcessMesh::GetNextPositionIndexInTriangle( INT iTriangleIndex, INT iPivotPositionIndex, INT iSweepPositionIndex )
    {
        const Triangle& tri = m_Triangles[iTriangleIndex];
        for( INT i = 0; i < 3; ++i )
        {
            INT iPositionIndex = tri.iIndices[i];
            if( iPositionIndex != iPivotPositionIndex && iPositionIndex != iSweepPositionIndex )
                return iPositionIndex;
        }
        return -1;
    }

    INT ExportSubDProcessMesh::GetNextPositionIndexInQuad( INT iQuadIndex, INT iPivotPositionIndex, INT iSweepPositionIndex )
    {
        const Quad& quad = m_Quads[iQuadIndex];
        for( INT iEdge = 0; iEdge < 4; ++iEdge )
        {
            INT iPositionIndexA = quad.iIndices[ iEdge ];
            INT iPositionIndexB = quad.iIndices[ ( iEdge + 3 ) % 4 ];

            if( iPositionIndexA == iPivotPositionIndex && iPositionIndexB != iSweepPositionIndex )
            {
                return iPositionIndexB;
            }
            if( iPositionIndexB == iPivotPositionIndex && iPositionIndexA != iSweepPositionIndex )
            {
                return iPositionIndexA;
            }
        }
        return -1;
    }

    INT ExportSubDProcessMesh::GetOppositePositionIndexInQuad( INT iQuadIndex, INT iPositionIndex )
    {
        const Quad& quad = m_Quads[iQuadIndex];
        for( INT iCorner = 0; iCorner < 4; ++iCorner )
        {
            if( quad.iIndices[iCorner] == iPositionIndex )
            {
                return quad.iIndices[ ( iCorner + 2 ) % 4 ];
            }
        }
        return -1;
    }

    bool QuadsHaveTwoMatchingEdges( const ExportSubDProcessMesh::Quad& QuadA, const ExportSubDProcessMesh::Quad& QuadB )
    {
        DWORD dwMatchingVertexCount = 0;
        for( DWORD i = 0; i < 4; ++i )
        {
            for( DWORD j = 0; j < 4; ++j )
            {
                if( QuadA.iIndices[i] == QuadB.iIndices[j] )
                {
                    ++dwMatchingVertexCount;
                }
            }
        }

        return dwMatchingVertexCount == 3;
    }

    void MergeQuads( ExportSubDProcessMesh::Quad& QuadA, const ExportSubDProcessMesh::Quad& QuadB )
    {
        INT iUniqueIndexA = -1;
        INT iUniquePositionInA = -1;
        for( INT i = 0; i < 4; ++i )
        {
            bool bFound = false;
            for( INT j = 0; j < 4; ++j )
            {
                if( QuadA.iIndices[i] == QuadB.iIndices[j] )
                {
                    bFound = true;
                }
            }
            if( !bFound )
            {
                iUniqueIndexA = QuadA.iIndices[i];
                iUniquePositionInA = i;
            }
        }
        assert( iUniqueIndexA != -1 );
        assert( iUniquePositionInA != -1 );

        INT iUniqueIndexB = -1;
        INT iUniquePositionInB = -1;
        for( INT i = 0; i < 4; ++i )
        {
            bool bFound = false;
            for( INT j = 0; j < 4; ++j )
            {
                if( QuadB.iIndices[i] == QuadA.iIndices[j] )
                {
                    bFound = true;
                }
            }
            if( !bFound )
            {
                iUniqueIndexB = QuadB.iIndices[i];
                iUniquePositionInB = i;
            }
        }
        assert( iUniqueIndexB != -1 );
        assert( iUniquePositionInB != -1 );

        INT iReplacementPositionInA = ( iUniquePositionInA + 2 ) % 4;

        QuadA.iIndices[iReplacementPositionInA] = iUniqueIndexB;
        QuadA.iMeshIndices[iReplacementPositionInA] = QuadB.iMeshIndices[iUniquePositionInB];

        // Erase the valence, prefix, and neighbor data for quad A, since it needs to be recomputed
        for( DWORD i = 0; i < 4; ++i )
        {
            QuadA.bValence[i] = 0;
            QuadA.bPrefix[i] = 0;
        }
        for( DWORD i = 0; i < ExportSubDProcessMesh::MAX_QUAD_NEIGHBOR_COUNT; ++i )
        {
            QuadA.iNeighbors[i] = 0;
        }
    }

    void ExportSubDProcessMesh::RemoveValenceTwoQuads( std::deque<INT>& BadQuads )
    {
        if( BadQuads.empty() )
        {
            return;
        }

        // The valence 2 quads (bad quads) should come in pairs.
        assert( BadQuads.size() % 2 == 0 );

        std::vector<INT> QuadsForAdjacency;

        // Find pairs of quads that share two edges.
        while( !BadQuads.empty() )
        {
            // Remove the first quad (quad A) from the bad quad list.
            INT iQuadAIndex = BadQuads.front();
            BadQuads.pop_front();
            Quad& QuadA = m_Quads[iQuadAIndex];
            
            // Find a quad in the bad quad list that adjoins quad A.
            bool bMatchedQuad = false;
            std::deque<INT>::iterator iter = BadQuads.begin();
            while( iter != BadQuads.end() )
            {
                int iQuadBIndex = *iter;
                Quad& QuadB = m_Quads[iQuadBIndex];

                if( QuadsHaveTwoMatchingEdges( QuadA, QuadB ) )
                {
                    ExportLog::LogMsg( 4, "Merging quad %d into quad %d, and eliminating quad %d.", iQuadBIndex, iQuadAIndex, iQuadBIndex );

                    // Merge the second quad into the first quad.
                    bMatchedQuad = true;
                    MergeQuads( QuadA, QuadB );

                    // The first quad needs to have its adjacency recomputed.
                    QuadsForAdjacency.push_back( iQuadAIndex );

                    // The second quad needs to be removed from the quad array.
                    // Setting the degenerate flag will effectively do this.
                    QuadB.bDegenerate = true;

                    // Remove the second quad from the bad quad list.
                    BadQuads.erase( iter );
                    break;
                }

                ++iter;
            }

            if( !bMatchedQuad )
            {
                // Can this error condition ever be encountered?
                ExportLog::LogError( "Quad %d has a vertex with valence 2, but was not matched with an adjacent quad.  Quad will be removed.", iQuadAIndex );
                QuadA.bDegenerate = true;
                break;
            }
        }

        // Recompute the adjacency of the quads that were modified.
        size_t dwCount = QuadsForAdjacency.size();
        for( DWORD i = 0; i < dwCount; ++i )
        {
            ComputeQuadAdjacency( QuadsForAdjacency[i] );
        }
    }

    bool QuadPatchSortPredicate( const ExportSubDProcessMesh::Quad& QuadA, const ExportSubDProcessMesh::Quad& QuadB )
    {
        INT iScoreA = 0;
        if( !QuadA.bDegenerate )
        {
            iScoreA += QuadA.iMeshSubsetIndex;
            if( QuadA.bValence[0] != 4 || QuadA.bValence[1] != 4 || QuadA.bValence[2] != 4 || QuadA.bValence[3] != 4 )
            {
                iScoreA += 1000000;
            }
        }

        INT iScoreB = 0;
        if( !QuadB.bDegenerate )
        {
            iScoreB += QuadB.iMeshSubsetIndex;
            if( QuadB.bValence[0] != 4 || QuadB.bValence[1] != 4 || QuadB.bValence[2] != 4 || QuadB.bValence[3] != 4 )
            {
                iScoreB += 1000000;
            }
        }

        return iScoreA < iScoreB;
    }

    bool TrianglePatchSortPredicate( const ExportSubDProcessMesh::Triangle& TriangleA, const ExportSubDProcessMesh::Triangle& TriangleB )
    {
        INT iScoreA = TriangleA.iMeshSubsetIndex;
        if( TriangleA.bValence[0] != 4 || TriangleA.bValence[1] != 4 || TriangleA.bValence[2] != 4 )
        {
            iScoreA += 1000000;
        }

        INT iScoreB = TriangleB.iMeshSubsetIndex;
        if( TriangleB.bValence[0] != 4 || TriangleB.bValence[1] != 4 || TriangleB.bValence[2] != 4 )
        {
            iScoreB += 1000000;
        }

        return iScoreA < iScoreB;
    }

    void ExportSubDProcessMesh::SortPatches()
    {
        std::stable_sort( m_Quads.begin(), m_Quads.end(), QuadPatchSortPredicate );
        std::stable_sort( m_Triangles.begin(), m_Triangles.end(), TrianglePatchSortPredicate );
    }

    void ExportSubDProcessMesh::BuildQuadPatchBuffer()
    {
        size_t dwQuadCount = m_Quads.size();

        size_t dwActiveQuadCount = 0;
        for( size_t i = 0; i < dwQuadCount; ++i )
        {
            if( !m_Quads[i].bDegenerate )
            {
                ++dwActiveQuadCount;
            }
        }

        if( dwActiveQuadCount == 0 )
            return;

        assert( m_pQuadPatchDataVB == nullptr );
        m_pQuadPatchDataVB = new ExportVB();
        m_pQuadPatchDataVB->SetVertexCount( static_cast<UINT>( dwActiveQuadCount ) );
        m_pQuadPatchDataVB->SetVertexSize( sizeof( PatchData ) );
        m_pQuadPatchDataVB->Allocate();

        m_pQuadPatchIB = new ExportIB();
        m_pQuadPatchIB->SetIndexCount( static_cast<UINT>( MAX_POINT_COUNT * dwActiveQuadCount ) );
        m_pQuadPatchIB->SetIndexSize( 4 );
        m_pQuadPatchIB->Allocate();

        auto pIndexData = reinterpret_cast<DWORD*>( m_pQuadPatchIB->GetIndexData() );

        DWORD dwIndex = 0;
        for( size_t i = 0; i < dwQuadCount; ++i )
        {
            const Quad& quad = m_Quads[i];
            if( quad.bDegenerate )
                continue;

            auto pPatchData = reinterpret_cast<PatchData*>( m_pQuadPatchDataVB->GetVertex( dwIndex ) );
            ZeroMemory( pPatchData, sizeof( PatchData ) );

            for( DWORD j = 0; j < 4; ++j )
            {
                pPatchData->bValence[j] = quad.bValence[j];
                pPatchData->bPrefix[j] = quad.bPrefix[j];
            }

            memcpy( pIndexData, quad.iMeshIndices, 4 * sizeof(INT) );

            for( DWORD j = 0; j < MAX_QUAD_NEIGHBOR_COUNT; ++j )
            {
                INT iPositionIndex = quad.iNeighbors[j];
                if( iPositionIndex != -1 )
                {
                    INT iMeshIndex = m_PositionToMeshVertexMapping[iPositionIndex];
                    assert( iMeshIndex != -1 );
                    pIndexData[ j + 4 ] = static_cast<DWORD>( iMeshIndex );
                }
            }

            ++dwIndex;
            pIndexData += MAX_POINT_COUNT;
        }
    }

    void ExportSubDProcessMesh::BuildTriPatchBuffer()
    {
        size_t dwTriangleCount = m_Triangles.size();
        if( dwTriangleCount == 0 )
            return;

        assert( m_pTrianglePatchDataVB == nullptr );
        m_pTrianglePatchDataVB = new ExportVB();
        m_pTrianglePatchDataVB->SetVertexCount( static_cast<UINT>( dwTriangleCount ) );
        m_pTrianglePatchDataVB->SetVertexSize( sizeof( PatchData ) );
        m_pTrianglePatchDataVB->Allocate();

        m_pTrianglePatchIB = new ExportIB();
        m_pTrianglePatchIB->SetIndexCount( static_cast<UINT>( dwTriangleCount * MAX_POINT_COUNT ) );
        m_pTrianglePatchIB->SetIndexSize( 4 );
        m_pTrianglePatchIB->Allocate();

        auto pIndexData = reinterpret_cast<DWORD*>( m_pTrianglePatchIB->GetIndexData() );
        for( size_t i = 0; i < dwTriangleCount; ++i )
        {
            const Triangle& Triangle = m_Triangles[i];
            auto pPatchData = reinterpret_cast<PatchData*>( m_pTrianglePatchDataVB->GetVertex( i ) );
            ZeroMemory( pPatchData, sizeof( PatchData ) );

            for( DWORD j = 0; j < 3; ++j )
            {
                pPatchData->bValence[j] = Triangle.bValence[j];
                pPatchData->bPrefix[j] = Triangle.bPrefix[j];
            }

            memcpy( pIndexData, Triangle.iMeshIndices, 3 * sizeof(INT) );

            for( DWORD j = 0; j < MAX_TRIANGLE_NEIGHBOR_COUNT; ++j )
            {
                INT iPositionIndex = Triangle.iNeighbors[j];
                if( iPositionIndex != -1 )
                {
                    INT iMeshIndex = m_PositionToMeshVertexMapping[iPositionIndex];
                    assert( iMeshIndex != -1 );
                    pIndexData[ j + 3 ] = static_cast<DWORD>( iMeshIndex );
                }
            }

            pIndexData += MAX_POINT_COUNT;
        }
    }

    void ExportSubDProcessMesh::ConvertSubsets()
    {
        size_t dwQuadCount = m_Quads.size();

        ExportSubDPatchSubset CurrentSubset = { 0 };
        CurrentSubset.bQuadPatches = true;
        CurrentSubset.iOriginalMeshSubset = -1;

        INT iPatchCount = 0;
        for( size_t i = 0; i < dwQuadCount; ++i )
        {
            if( m_Quads[i].bDegenerate )
                continue;

            INT iMeshSubsetIndex = m_Quads[i].iMeshSubsetIndex;
            if( iMeshSubsetIndex != CurrentSubset.iOriginalMeshSubset )
            {
                if( CurrentSubset.iOriginalMeshSubset != -1 )
                {
                    m_Subsets.push_back( CurrentSubset );
                }
                CurrentSubset.iOriginalMeshSubset = iMeshSubsetIndex;
                CurrentSubset.dwPatchCount = 0;
                CurrentSubset.dwStartPatch = iPatchCount;
                CurrentSubset.Name = m_pPolyMesh->GetSubset( iMeshSubsetIndex )->GetName();
            }
            ++CurrentSubset.dwPatchCount;
            ++iPatchCount;
        }
        if( CurrentSubset.dwPatchCount > 0 )
        {
            m_Subsets.push_back( CurrentSubset );
        }

        iPatchCount = 0;
        size_t dwTriCount = m_Triangles.size();
        CurrentSubset.bQuadPatches = false;
        CurrentSubset.dwStartPatch = 0;
        CurrentSubset.dwPatchCount = 0;
        CurrentSubset.iOriginalMeshSubset = -1;

        for( size_t i = 0; i < dwTriCount; ++i )
        {
            INT iMeshSubsetIndex = m_Triangles[i].iMeshSubsetIndex;
            if( iMeshSubsetIndex != CurrentSubset.iOriginalMeshSubset )
            {
                if( CurrentSubset.iOriginalMeshSubset != -1 )
                {
                    m_Subsets.push_back( CurrentSubset );
                }
                CurrentSubset.iOriginalMeshSubset = iMeshSubsetIndex;
                CurrentSubset.dwPatchCount = 0;
                CurrentSubset.dwStartPatch = iPatchCount;
                CurrentSubset.Name = m_pPolyMesh->GetSubset( iMeshSubsetIndex )->GetName();
            }
            ++CurrentSubset.dwPatchCount;
            ++iPatchCount;
        }
        if( CurrentSubset.dwPatchCount > 0 )
        {
            m_Subsets.push_back( CurrentSubset );
        }
    }

    const D3DVERTEXELEMENT9 g_PatchDataElements[] =
    {
        { 0, 0, D3DDECLTYPE_UBYTE4, 0, D3DDECLUSAGE_POSITION, 0 },
        { 0, 4, D3DDECLTYPE_UBYTE4, 0, D3DDECLUSAGE_POSITION, 1 }
    };

    const D3DVERTEXELEMENT9* ExportSubDProcessMesh::GetPatchDataDecl()
    {
        return g_PatchDataElements;
    }

    DWORD ExportSubDProcessMesh::GetPatchDataDeclElementCount()
    {
        return ARRAYSIZE( g_PatchDataElements );
    }

    ExportSubDPatchSubset* ExportSubDProcessMesh::FindSubset( ExportString strName )
    {
        size_t dwCount = GetSubsetCount();
        for( size_t i = 0; i < dwCount; ++i )
        {
            if( m_Subsets[i].Name == strName )
                return &m_Subsets[i];
        }
        return nullptr;
    }
}

