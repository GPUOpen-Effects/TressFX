//-------------------------------------------------------------------------------------
// ExportSubD.h
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
#pragma once

#include <deque>

namespace ATG
{
    struct ExportSubDPatchSubset
    {
        ExportString    Name;
        INT             iOriginalMeshSubset;
        bool            bQuadPatches;
        DWORD           dwStartPatch;
        DWORD           dwPatchCount;
    };

    class ExportSubDProcessMesh
    {
    public:
        enum 
        {
            MAX_POINT_COUNT = 32,
            MAX_QUAD_NEIGHBOR_COUNT = (MAX_POINT_COUNT - 4),
            MAX_TRIANGLE_NEIGHBOR_COUNT = (MAX_POINT_COUNT - 3)
        };
        struct PatchData
        {
            //UINT iMeshIndices[MAX_POINT_COUNT];
            BYTE bValence[4];
            BYTE bPrefix[4];
        };
        struct Triangle
        {
            INT iIndices[3];
            INT iMeshIndices[3];
            INT iNeighbors[MAX_TRIANGLE_NEIGHBOR_COUNT];
            BYTE bValence[3];
            BYTE bPrefix[3];
            INT iPolyIndex;
            INT iMeshSubsetIndex;
        };
        struct Quad
        {
            INT iIndices[4];
            INT iMeshIndices[4];
            INT iNeighbors[MAX_QUAD_NEIGHBOR_COUNT];
            BYTE bValence[4];
            BYTE bPrefix[4];
            INT iPolyIndex;
            INT iMeshSubsetIndex;
            bool bDegenerate;
        };
    protected:
        struct Edge
        {
            INT iTriangleIndex;
            INT iQuadIndex;
            INT iLocalIndex;
        };
        typedef std::unordered_map< UINT64, Edge > EdgeMap;

        std::vector< Triangle >             m_Triangles;
        std::vector< Quad >                 m_Quads;
        std::vector< DirectX::XMFLOAT3 >    m_Positions;
        std::vector< INT >                  m_MeshVertexToPositionMapping;
        std::vector< INT >                  m_PositionToMeshVertexMapping;
        std::vector< INT >                  m_PositionToDegeneratePositionMapping;
        std::vector< INT >                  m_IncidentBoundaryEdgesPerPosition;
        EdgeMap                             m_BoundaryEdges;

        ExportMesh*             m_pPolyMesh;
        ExportIB*               m_pQuadPatchIB;
        ExportIB*               m_pTrianglePatchIB;
        ExportVB*               m_pQuadPatchDataVB;
        ExportVB*               m_pTrianglePatchDataVB;

        std::vector< ExportSubDPatchSubset > m_Subsets;
    public:
        ExportSubDProcessMesh();
        void Initialize( ExportMesh* pMesh );

        void ByteSwap();

        size_t GetQuadPatchCount() const { return ( m_pQuadPatchDataVB ) ? m_pQuadPatchDataVB->GetVertexCount() : 0; }
        ExportVB* GetQuadPatchDataVB() const { return m_pQuadPatchDataVB; }
        ExportIB* GetQuadPatchIB() const { return m_pQuadPatchIB; }
        size_t GetTrianglePatchCount() const { return ( m_pTrianglePatchDataVB ) ? m_pTrianglePatchDataVB->GetVertexCount() : 0; }
        ExportVB* GetTrianglePatchDataVB() const { return m_pTrianglePatchDataVB; }
        ExportIB* GetTriPatchIB() const { return m_pTrianglePatchIB; }

        size_t GetSubsetCount() const { return m_Subsets.size(); }
        ExportSubDPatchSubset* GetSubset( size_t dwIndex ) { return &m_Subsets[dwIndex]; }
        ExportSubDPatchSubset* FindSubset( ExportString strName );

        static const D3DVERTEXELEMENT9* GetPatchDataDecl();
        static DWORD GetPatchDataDeclElementCount();

    protected:
        void BuildMesh();
        INT CreateOrAddPosition( const DirectX::XMFLOAT3& vPosition, INT iMeshVertexIndex );
        void AddTriangle( INT iPolyIndex, INT iSubsetIndex, const INT* pIndices, const INT* pMeshIndices );
        void AddQuad( INT iPolyIndex,  INT iSubsetIndex, const INT* pIndices, const INT* pMeshIndices );

        void BuildBoundaryEdgeTable();
        INT AddOrRemoveEdge( INT iPositionIndexA, INT iPositionIndexB, INT iTriangleIndex, INT iQuadIndex, INT iLocalIndex );

        void CreateDegenerateGeometry();
        INT CreateOrAddDegeneratePosition( INT iPositionIndex );

        void ComputeAdjacency();
        void ComputeTriangleAdjacency( const INT iTriangleIndex );
        void ComputeQuadAdjacency( const INT iQuadIndex );
        INT ExecuteSweep( INT iPivotPositionIndex, INT iSweepPositionIndex, INT iStopPositionIndex, INT iStartQuadIndex, INT iStartTriangleIndex, std::vector<INT>& Neighbors );
        INT FindTriangleWithEdge( INT iStartPositionIndex, INT iEndPositionIndex, INT iExcludeThisTriangle );
        INT FindQuadWithEdge( INT iStartPositionIndex, INT iEndPositionIndex, INT iExcludeThisQuad );
        INT FindLocalIndexInTriangle( INT iTriangleIndex, INT iPositionIndex );
        INT FindLocalIndexInQuad( INT iQuadIndex, INT iPositionIndex );
        INT GetNextPositionIndexInTriangle( INT iTriangleIndex, INT iPivotPositionIndex, INT iSweepPositionIndex );
        INT GetNextPositionIndexInQuad( INT iQuadIndex, INT iPivotPositionIndex, INT iSweepPositionIndex );
        INT GetOppositePositionIndexInQuad( INT iQuadIndex, INT iPositionIndex );

        void RemoveValenceTwoQuads( std::deque<INT>& BadQuads );

        DirectX::XMFLOAT3 GetQuadCenter( INT iQuadIndex );

        void SortPatches();

        void BuildQuadPatchBuffer();
        void BuildTriPatchBuffer();

        void ConvertSubsets();

        void ClearIntermediateBuffers();
    };
}
