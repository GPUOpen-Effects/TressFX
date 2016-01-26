//-------------------------------------------------------------------------------------
// ExportMesh.cpp
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
#include "exportmesh.h"

#include "DirectXMesh.h"
#include "UVAtlas.h"

#include <conio.h>

using namespace DirectX;
using namespace DirectX::PackedVector;

#define SAFE_RELEASE(x) { if( (x) != nullptr ) { (x)->Release(); (x) = nullptr; } }

extern ATG::ExportScene* g_pScene;

namespace ATG
{

INT GetElementSizeFromDeclType(DWORD Type);

ExportMeshTriangleAllocator g_MeshTriangleAllocator;

ExportMeshBase::ExportMeshBase( ExportString name )
: ExportBase( name )
{ }

ExportMeshBase::~ExportMeshBase()
{ }

ExportIBSubset* ExportMeshBase::FindSubset( const ExportString Name )
{
    size_t dwSubsetCount = GetSubsetCount();
    for( size_t i = 0; i < dwSubsetCount; ++i )
    {
        ExportIBSubset* pSubset = GetSubset( i );
        if( pSubset->GetName() == Name )
            return pSubset;
    }
    return nullptr;
}

ExportMesh::ExportMesh( ExportString name )
: ExportMeshBase( name ),
  m_uDCCVertexCount( 0 ),
  m_pSubDMesh( nullptr )
{
    m_BoundingSphere.Center = XMFLOAT3( 0, 0, 0 );
    m_BoundingSphere.Radius = 0;
}

ExportMesh::~ExportMesh()
{
    ClearRawTriangles();
}

void ExportMesh::ClearRawTriangles()
{
    if( m_RawTriangles.size() > 0 )
    {
        g_MeshTriangleAllocator.ClearAllTriangles();
        m_RawTriangles.clear();
    }
}

void ExportMesh::ByteSwap()
{
    if( m_pIB )
        m_pIB->ByteSwap();
    if( m_pVB && m_VertexElements.size() > 0 )
        m_pVB->ByteSwap( &m_VertexElements[0], GetVertexDeclElementCount() );
    if( m_pSubDMesh )
    {
        m_pSubDMesh->ByteSwap();
    }
}

void ExportVB::Allocate()
{
    size_t uSize = GetVertexDataSize();
    m_pVertexData.reset( new uint8_t[ uSize ] );
    ZeroMemory( m_pVertexData.get(), uSize );
}

uint8_t* ExportVB::GetVertex( size_t uIndex )
{
    if( !m_pVertexData )
        return nullptr;
    if( uIndex >= m_uVertexCount )
        return nullptr;
    return m_pVertexData.get() + ( uIndex * m_uVertexSizeBytes );
}

const uint8_t* ExportVB::GetVertex( size_t uIndex ) const
{
    if( !m_pVertexData )
        return nullptr;
    if( uIndex >= m_uVertexCount )
        return nullptr;
    return m_pVertexData.get() + ( uIndex * m_uVertexSizeBytes );
}

void ExportVB::ByteSwap( const D3DVERTEXELEMENT9* pVertexElements, const size_t dwVertexElementCount )
{
    for( size_t dwVertexIndex = 0; dwVertexIndex < m_uVertexCount; dwVertexIndex++ )
    {
        auto pVB = GetVertex( dwVertexIndex );
        for( size_t i = 0; i < dwVertexElementCount; i++ )
        {
            auto pElement = reinterpret_cast<DWORD*>( pVB + pVertexElements[i].Offset );
            switch( pVertexElements[i].Type )
            {
            case D3DDECLTYPE_FLOAT4:
                *pElement = _byteswap_ulong( *pElement );
                pElement++;
            case D3DDECLTYPE_FLOAT3:
                *pElement = _byteswap_ulong( *pElement );
                pElement++;
            case D3DDECLTYPE_FLOAT2:
                *pElement = _byteswap_ulong( *pElement );
                pElement++;
            case D3DDECLTYPE_FLOAT1:
            case D3DDECLTYPE_D3DCOLOR:
            case D3DDECLTYPE_UBYTE4:
            case D3DDECLTYPE_UBYTE4N:
                *pElement = _byteswap_ulong( *pElement );
                break;
            case D3DDECLTYPE_SHORT4N:
            case D3DDECLTYPE_FLOAT16_4:
                {
                    auto pWord = reinterpret_cast<WORD*>( pElement );
                    *pWord = _byteswap_ushort( *pWord );
                    pWord++;
                    *pWord = _byteswap_ushort( *pWord );
                    pElement++;
                }
            case D3DDECLTYPE_FLOAT16_2:
                {
                    auto pWord = reinterpret_cast<WORD*>( pElement );
                    *pWord = _byteswap_ushort( *pWord );
                    pWord++;
                    *pWord = _byteswap_ushort( *pWord );
                    pElement++;
                    break;
                }
            }
        }
    }
}

void ExportIB::ByteSwap()
{
    if( m_dwIndexSize == 2 )
    {
        auto pIndexData16 = reinterpret_cast<WORD*>( m_pIndexData.get() );
        for( size_t i = 0; i < m_uIndexCount; i++ )
        {
            WORD wIndex = _byteswap_ushort( pIndexData16[ i ] );
            pIndexData16[ i ] = wIndex;
        }
    }
    else
    {
        auto pIndexData32 = reinterpret_cast<DWORD*>( m_pIndexData.get() );
        for( size_t i = 0; i < m_uIndexCount; i++ )
        {
            DWORD dwIndex = _byteswap_ulong( pIndexData32[ i ] );
            pIndexData32[ i ] = dwIndex;
        }
    }
}

void ExportIB::Allocate()
{
    if( m_dwIndexSize == 2 )
    {
        m_pIndexData.reset( reinterpret_cast<uint8_t*>( new WORD[ m_uIndexCount ] ) );
        ZeroMemory( m_pIndexData.get(), m_uIndexCount * sizeof( WORD ) );
    }
    else
    {
        m_pIndexData.reset( reinterpret_cast<uint8_t*>( new DWORD[ m_uIndexCount ] ) );
        ZeroMemory( m_pIndexData.get(), m_uIndexCount * sizeof( DWORD ) );
    }
}

void ExportMeshTriangleAllocator::Terminate()
{
    AllocationBlockList::iterator iter = m_AllocationBlocks.begin();
    AllocationBlockList::iterator end = m_AllocationBlocks.end();
    while( iter != end )
    {
        AllocationBlock& block = *iter;
        delete[] block.pTriangleArray;
        ++iter;
    }
    m_AllocationBlocks.clear();
    m_uTotalCount = 0;
    m_uAllocatedCount = 0;
}

void ExportMeshTriangleAllocator::SetSizeHint( UINT uAnticipatedSize )
{
    if( uAnticipatedSize <= m_uTotalCount )
        return;
    UINT uNewCount = std::max<UINT>( uAnticipatedSize - m_uTotalCount, 10000 );
    AllocationBlock NewBlock;
    NewBlock.m_uTriangleCount = uNewCount;
    NewBlock.pTriangleArray = new ExportMeshTriangle[ uNewCount ];
    m_AllocationBlocks.push_back( NewBlock );
    m_uTotalCount += uNewCount;
}

ExportMeshTriangle* ExportMeshTriangleAllocator::GetNewTriangle()
{
    if( m_uAllocatedCount == m_uTotalCount )
        SetSizeHint( m_uTotalCount + 10000 );
    UINT uIndex = m_uAllocatedCount;
    m_uAllocatedCount++;
    AllocationBlockList::iterator iter = m_AllocationBlocks.begin();
    AllocationBlockList::iterator end = m_AllocationBlocks.end();
    while( iter != end )
    {
        AllocationBlock& block = *iter;
        if( uIndex < block.m_uTriangleCount )
        {
            ExportMeshTriangle* pTriangle = &block.pTriangleArray[ uIndex ];
            pTriangle->Initialize();
            return pTriangle;
        }
        uIndex -= block.m_uTriangleCount;
        ++iter;
    }
    assert( false );
    return nullptr;
}

void ExportMeshTriangleAllocator::ClearAllTriangles()
{
    m_uAllocatedCount = 0;
}

bool ExportMeshVertex::Equals( const ExportMeshVertex* pOtherVertex ) const
{
    if ( !pOtherVertex )
        return false;

    if( pOtherVertex == this )
        return true;

    XMVECTOR v0 = XMLoadFloat3( &Position );
    XMVECTOR v1 = XMLoadFloat3( &pOtherVertex->Position );
    if ( XMVector3NotEqual( v0, v1 ) )
        return false;

    v0 = XMLoadFloat3( &Normal );
    v1 = XMLoadFloat3( &pOtherVertex->Normal );
    if ( XMVector3NotEqual( v0, v1 ) )
        return false;

    for( size_t i = 0; i < 8; i++ )
    {
        v0 = XMLoadFloat4( &TexCoords[i] );
        v1 = XMLoadFloat4( &pOtherVertex->TexCoords[i] );
        if ( XMVector4NotEqual( v0, v1 ) )
            return false;
    }

    v0 = XMLoadFloat4( &Color );
    v1 = XMLoadFloat4( &pOtherVertex->Color );
    if ( XMVector4NotEqual( v0, v1 ) )
        return false;

    return true;
}

UINT FindOrAddVertex( ExportMeshVertexArray& ExistingVertexArray, ExportMeshVertex* pTestVertex )
{
    for( size_t i = 0; i < ExistingVertexArray.size(); i++ )
    {
        ExportMeshVertex* pVertex = ExistingVertexArray[i];
        if( pVertex->Equals( pTestVertex ) )
            return static_cast<UINT>( i );
    }
    UINT index = static_cast<UINT>( ExistingVertexArray.size() );
    ExistingVertexArray.push_back( pTestVertex );
    return index;
}

void ExportMesh::AddRawTriangle( ExportMeshTriangle* pTriangle )
{
    m_RawTriangles.push_back( pTriangle );
    m_uDCCVertexCount = std::max<UINT>( m_uDCCVertexCount, pTriangle->Vertex[0].DCCVertexIndex + 1 );
    m_uDCCVertexCount = std::max<UINT>( m_uDCCVertexCount, pTriangle->Vertex[1].DCCVertexIndex + 1 );
    m_uDCCVertexCount = std::max<UINT>( m_uDCCVertexCount, pTriangle->Vertex[2].DCCVertexIndex + 1 );
}

UINT FindOrAddVertexFast( ExportMeshVertexArray& ExistingVertexArray, ExportMeshVertex* pTestVertex )
{
    UINT uIndex = pTestVertex->DCCVertexIndex;
    assert( uIndex < ExistingVertexArray.size() );
    ExportMeshVertex* pVertex = ExistingVertexArray[ uIndex ];
    if( pVertex )
    {
        ExportMeshVertex* pLastVertex = nullptr;
        while( pVertex )
        {
            uIndex = pVertex->DCCVertexIndex;
            if( pVertex->Equals( pTestVertex ) )
                return uIndex;
            pLastVertex = pVertex;
            pVertex = pVertex->pNextDuplicateVertex;
        }
        assert( pVertex == nullptr );
        uIndex = static_cast<UINT>( ExistingVertexArray.size() );
        ExistingVertexArray.push_back( pTestVertex );
        pTestVertex->DCCVertexIndex = uIndex;
        if( pLastVertex )
        {
            pLastVertex->pNextDuplicateVertex = pTestVertex;
        }
    }
    else
    {
        ExistingVertexArray[ uIndex ] = pTestVertex;
    }
    return uIndex;
}

void ExportMesh::SetVertexNormalCount( UINT uCount )
{
    m_VertexFormat.m_bNormal = ( uCount > 0 );
    m_VertexFormat.m_bTangent = ( uCount > 1 );
    m_VertexFormat.m_bBinormal = ( uCount > 2 );
}

bool SubsetLess( ExportMeshTriangle* pA, ExportMeshTriangle* pB )
{
    return pA->SubsetIndex < pB->SubsetIndex;
}

void ExportMesh::SortRawTrianglesBySubsetIndex()
{
    if( m_RawTriangles.empty() )
        return;

    std::stable_sort( m_RawTriangles.begin(), m_RawTriangles.end(), SubsetLess );
}

void ExportMesh::Optimize( DWORD dwFlags )
{
    if( m_RawTriangles.empty() )
        return;

    ExportLog::LogMsg( 4, "Optimizing mesh \"%s\" with %Iu triangles.", GetName().SafeString(), m_RawTriangles.size() );
    
    // Apply a AttributeSort optimization
    SortRawTrianglesBySubsetIndex();

    ExportIBSubset* pCurrentIBSubset = nullptr;
    INT iCurrentSubsetIndex = -1;
    std::vector<UINT> IndexData;
    IndexData.reserve( m_RawTriangles.size() * 3 );
    ExportMeshVertexArray VertexData;
    VertexData.resize( m_uDCCVertexCount, nullptr );

    bool bFlipTriangles = g_pScene->Settings().bFlipTriangles;
    if( dwFlags & FLIP_TRIANGLES )
        bFlipTriangles = !bFlipTriangles;

    // loop through raw triangles
    const size_t dwTriangleCount = m_RawTriangles.size();
    m_TriangleToPolygonMapping.clear();
    m_TriangleToPolygonMapping.reserve( dwTriangleCount );

    m_pAttributes.reset( new uint32_t[ dwTriangleCount ] );
    for( size_t i = 0; i < dwTriangleCount; i++ )
    {
        ExportMeshTriangle* pTriangle = m_RawTriangles[i];
        // create a new subset if one is encountered
        // note: subset index will be monotonically increasing
        assert( pTriangle->SubsetIndex >= iCurrentSubsetIndex );
        if( pTriangle->SubsetIndex > iCurrentSubsetIndex )
        {
            pCurrentIBSubset = new ExportIBSubset();
            pCurrentIBSubset->SetName( "Default" );
            pCurrentIBSubset->SetStartIndex( static_cast<UINT>( IndexData.size() ) );
            m_vSubsets.push_back( pCurrentIBSubset );
            iCurrentSubsetIndex = pTriangle->SubsetIndex;
        }
        // collapse the triangle verts into the final vertex list
        // this removes unnecessary duplicates, and retains necessary duplicates
        UINT uIndexA = FindOrAddVertexFast( VertexData, &pTriangle->Vertex[0] );
        UINT uIndexB = FindOrAddVertexFast( VertexData, &pTriangle->Vertex[1] );
        UINT uIndexC = FindOrAddVertexFast( VertexData, &pTriangle->Vertex[2] );
        // record final indices into the index list
        IndexData.push_back( uIndexA );
        if( bFlipTriangles )
        {
            IndexData.push_back( uIndexC );
            IndexData.push_back( uIndexB );
        }
        else
        {
            IndexData.push_back( uIndexB );
            IndexData.push_back( uIndexC );
        }
        m_pAttributes[i] = static_cast<uint32_t>( iCurrentSubsetIndex );
        m_TriangleToPolygonMapping.push_back( pTriangle->PolygonIndex );
        pCurrentIBSubset->IncrementIndexCount( 3 );
    }
    ExportLog::LogMsg( 3, "Triangle list mesh: %Iu verts, %Iu indices, %Iu subsets", VertexData.size(), IndexData.size(), m_vSubsets.size() );

    size_t nVerts = VertexData.size();
    if ( nVerts > 4294967295 )
    {
        ExportLog::LogError( "Mesh \"%s\" has more than 2^32-1 vertices.  Index buffer is invalid.", GetName().SafeString() );
    }
    else if ( !g_pScene->Settings().bLittleEndian )
    {
        if ( nVerts > 16777215 )
        {
            ExportLog::LogError( "Mesh \"%s\" has more than 16777215 vertices.  Index buffer is invalid for Xbox 360.", GetName().SafeString() );
        }
    }
    else if ( nVerts > 1048575 )
    {
        if ( g_pScene->Settings().dwFeatureLevel <= D3D_FEATURE_LEVEL_9_2 )
        {
            ExportLog::LogWarning("Mesh \"%s\" has more than 1048575 vertices.  Index buffer is invalid for Feature Level 9.1 or 9.2.", GetName().SafeString());
        }
    }
    else if ( nVerts > 65534 )
    {
        if ( g_pScene->Settings().dwFeatureLevel <= D3D_FEATURE_LEVEL_9_1 )
        {
            ExportLog::LogWarning("Mesh \"%s\" has more than 65534 vertices.  Index buffer is invalid for Feature Level 9.1.", GetName().SafeString());
        }
    }

    // Create real index buffer from index list
    m_pIB.reset(new ExportIB);
    m_pIB->SetIndexCount( IndexData.size() );
    if( VertexData.size() > 65535 || g_pScene->Settings().bForceIndex32Format )
    {
        m_pIB->SetIndexSize( 4 );
    }
    else
    {
        m_pIB->SetIndexSize( 2 );
    }
    m_pIB->Allocate();
    for( size_t i = 0; i < IndexData.size(); i++ )
    {
        m_pIB->SetIndex( i, IndexData[i] );
    }

    INT iUVAtlasTexCoordIndex = g_pScene->Settings().iGenerateUVAtlasOnTexCoordIndex;
    bool bComputeUVAtlas = ( iUVAtlasTexCoordIndex >= 0 );
    if( bComputeUVAtlas )
    {
        if( iUVAtlasTexCoordIndex < static_cast<INT>( m_VertexFormat.m_uUVSetCount ) )
        {
            ExportLog::LogWarning( "UV atlas being generated in existing texture coordinate set %d, which will overwrite its contents.", iUVAtlasTexCoordIndex );
        }
        else
        {
            // Save the index of a new UV set
            iUVAtlasTexCoordIndex = static_cast<INT>( m_VertexFormat.m_uUVSetCount );
            // add another UV set, it will be empty
            ++m_VertexFormat.m_uUVSetCount;
            ExportLog::LogMsg( 4, "Adding a new texture coordinate set for the UV atlas." );
        }
    }

    // Convert vertex data to final format
    BuildVertexBuffer( VertexData, dwFlags );

    // Check if we need to remap the UV atlas texcoord index
    if( bComputeUVAtlas && ( iUVAtlasTexCoordIndex != g_pScene->Settings().iGenerateUVAtlasOnTexCoordIndex ) )
    {
        // Scan the decl elements
        for( size_t i = 0; i < m_VertexElements.size(); ++i )
        {
            if( m_VertexElements[i].Usage == D3DDECLUSAGE_TEXCOORD && m_VertexElements[i].UsageIndex == iUVAtlasTexCoordIndex )
            {
                // Change the decl usage index to the desired usage index
                iUVAtlasTexCoordIndex = g_pScene->Settings().iGenerateUVAtlasOnTexCoordIndex;
                m_VertexElements[i].UsageIndex = static_cast<BYTE>( iUVAtlasTexCoordIndex );
                m_InputLayout[i].SemanticIndex = iUVAtlasTexCoordIndex;
            }
        }
    }

    if (dwFlags & CLEAN_MESHES)
    {
        CleanMesh(bComputeUVAtlas);
    }

    if( m_VertexFormat.m_bTangent || m_VertexFormat.m_bBinormal )
    {
        ComputeVertexTangentSpaces();
    }

    m_pVBNormals.reset();
    m_pVBTexCoords.reset();

    if (bComputeUVAtlas)
    {
        ComputeUVAtlas();
    }

    m_pVBPositions.reset();

    if ( dwFlags & VCACHE_OPT )
    {
        OptimizeVcache();
    }

    m_pAttributes.reset();
    m_pAdjacency.reset();
    ClearRawTriangles();
    ComputeBounds();

    ExportLog::LogMsg( 3, "Vertex size: %u bytes; VB size: %Iu bytes", m_pVB->GetVertexSize(), m_pVB->GetVertexDataSize() );

    if( ExportLog::GetLogLevel() >= 4 )
    {
        size_t dwDeclSize = GetVertexDeclElementCount();
        
        static const CHAR* strDeclUsages[] = { "Position", "BlendWeight", "BlendIndices", "Normal", "PSize", "TexCoord", "Tangent", "Binormal", "TessFactor", "PositionT", "Color", "Fog", "Depth", "Sample" };
        static const CHAR* strDeclTypes[] = { "Float1", "Float2", "Float3", "Float4", "D3DColor", "UByte", "Short2", "Short4", "UByte4N", "Short2N", "Short4N", "UShort2N", "UShort4N", "UDec3", "Dec3N", "Float16_2", "Float16_4", "Unused" };

        for( size_t i = 0; i < dwDeclSize; ++i )
        {
            const D3DVERTEXELEMENT9& Element = GetVertexDeclElement( i );

            ExportLog::LogMsg( 4, "Element %2Iu Stream %2u Offset %2u: %12s.%-2u Type %s (%d bytes)", i, Element.Stream, Element.Offset, strDeclUsages[Element.Usage], Element.UsageIndex, strDeclTypes[Element.Type], GetElementSizeFromDeclType( Element.Type ) );
        }
    }

    ExportLog::LogMsg( 4, "DCC stored %u verts; final vertex count is %Iu due to duplication.", m_uDCCVertexCount, m_pVB->GetVertexCount() );

    if( dwFlags & FORCE_SUBD_CONVERSION )
    {
        ExportLog::LogMsg( 2, "Converting mesh \"%s\" to a subdivision surface mesh.", GetName().SafeString() );
        m_pSubDMesh = new ExportSubDProcessMesh();
        m_pSubDMesh->Initialize( this );
    }
}

void ExportMesh::CleanMesh(bool breakBowTies)
{
    assert(m_pIB != 0);
    assert(m_pVB != 0);

    ExportLog::LogMsg(4, "Cleaning up mesh...");

    if (m_pVBPositions)
    {
        ComputeAdjacency();
    }

    size_t nFaces = m_pIB->GetIndexCount() / 3;
    size_t nVerts = m_pVB->GetVertexCount();

    HRESULT hr = E_FAIL; 
    std::vector<uint32_t> dups;
    if (m_pIB->GetIndexSize() == 2)
    {
        hr = Clean( reinterpret_cast<uint16_t*>(m_pIB->GetIndexData()), nFaces, nVerts, m_pAdjacency.get(), m_pAttributes.get(), dups, breakBowTies );
    }
    else
    {
        hr = Clean( reinterpret_cast<uint32_t*>(m_pIB->GetIndexData()), nFaces, nVerts, m_pAdjacency.get(), m_pAttributes.get(), dups, breakBowTies );
    }
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to clean (%08X).", GetName().SafeString(), hr );
        return;
    }

    if (dups.empty())
    {
        ExportLog::LogMsg( 4, "Mesh cleanup did not duplicate any vertices" );
        return;
    }

    size_t nNewVerts = nVerts + dups.size();

    ExportLog::LogMsg( 4, "Mesh cleanup increased vertex count from %Iu to %Iu.", nVerts, nNewVerts );

    std::unique_ptr<XMFLOAT3[]> pos;
    if (m_pVBPositions)
    {
        pos.reset(new XMFLOAT3[nNewVerts]);

        hr = FinalizeVB( m_pVBPositions.get(), sizeof(XMFLOAT3), nVerts, dups.data(), dups.size(), nullptr, pos.get() );
        if (FAILED(hr))
        {
            ExportLog::LogError("Mesh \"%s\" failed clean of positions (%08X).", GetName().SafeString(), hr );
            return;
        }
    }

    std::unique_ptr<XMFLOAT3[]> normals;
    if (m_pVBNormals)
    {
        normals.reset(new XMFLOAT3[nNewVerts]);

        hr = FinalizeVB( m_pVBNormals.get(), sizeof(XMFLOAT3), nVerts, dups.data(), dups.size(), nullptr, normals.get() );
        if (FAILED(hr))
        {
            ExportLog::LogError("Mesh \"%s\" failed clean of normals (%08X).", GetName().SafeString(), hr );
            return;
        }
    }

    std::unique_ptr<XMFLOAT2[]> texcoords;
    if (m_pVBTexCoords)
    {
        texcoords.reset(new XMFLOAT2[nNewVerts]);

        hr = FinalizeVB( m_pVBTexCoords.get(), sizeof(XMFLOAT2), nVerts, dups.data(), dups.size(), nullptr, texcoords.get() );
        if (FAILED(hr))
        {
            ExportLog::LogError("Mesh \"%s\" failed clean of texcoords (%08X).", GetName().SafeString(), hr );
            return;
        }
    }

    DWORD stride = m_pVB->GetVertexSize();

    std::unique_ptr<ExportVB> newVB(new ExportVB);
    newVB->SetVertexCount( nNewVerts );
    newVB->SetVertexSize( stride );
    newVB->Allocate();

    hr = FinalizeVB( m_pVB->GetVertexData(), stride, nVerts, dups.data(), dups.size(), nullptr, newVB->GetVertexData() );
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed clean of vertices (%08X).", GetName().SafeString(), hr );
        return;
    }

    // Commit changes
    m_pVB.swap(newVB);
    m_pVBPositions.swap(pos);
    m_pVBNormals.swap(normals);
    m_pVBTexCoords.swap(texcoords);
}

void ExportMesh::ComputeVertexTangentSpaces()
{
    assert( m_pIB != 0 );
    assert( m_pVB != 0 );

    if ( !m_pVBPositions )
    {
        ExportLog::LogError("Mesh \"%s\" missing positions needed for tangent space computation.", GetName().SafeString() );
        return;
    }

    if ( !m_pVBNormals )
    {
        ExportLog::LogError("Mesh \"%s\" missing normals needed for tangent space computation.", GetName().SafeString() );
        return;
    }

    if ( m_VertexFormat.m_uUVSetCount <= static_cast<UINT>( g_pScene->Settings().iTangentSpaceIndex )
         || !m_pVBTexCoords )
    {
        ExportLog::LogError("Mesh \"%s\" missing texture coordinate %d needed for tangent space computation.", GetName().SafeString(), g_pScene->Settings().iTangentSpaceIndex );
        return;
    }

    if (m_VertexFormat.m_uUVSetSize < 1)
    {
        ExportLog::LogError("Mesh \"%s\" texture coordinate %d must be have at least U & V for tangent space computation.", GetName().SafeString(), g_pScene->Settings().iTangentSpaceIndex );
        return;
    }

    size_t nVerts = m_pVB->GetVertexCount();

    std::unique_ptr<XMFLOAT3 []> tan1(new XMFLOAT3[nVerts]);
    std::unique_ptr<XMFLOAT3 []> tan2(new XMFLOAT3[nVerts]);

    HRESULT hr = E_FAIL;
    if (m_pIB->GetIndexSize() == 2)
    {
        hr = ComputeTangentFrame(reinterpret_cast<const uint16_t*>(m_pIB->GetIndexData()), m_pIB->GetIndexCount() / 3, m_pVBPositions.get(), m_pVBNormals.get(), m_pVBTexCoords.get(), nVerts,
                                 tan1.get(), tan2.get());
    }
    else
    {
        hr = ComputeTangentFrame(reinterpret_cast<const uint32_t*>(m_pIB->GetIndexData()), m_pIB->GetIndexCount() / 3, m_pVBPositions.get(), m_pVBNormals.get(), m_pVBTexCoords.get(), nVerts,
                                 tan1.get(), tan2.get());
    }
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to compute tangent space (%08X).", GetName().SafeString(), hr );
        return;
    }

    std::unique_ptr<VBWriter> writer( new VBWriter() );
    hr = writer->Initialize( m_InputLayout.data(), m_InputLayout.size() );
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to create VBWriter (%08X).", GetName().SafeString(), hr );
        return;
    }

    hr = writer->AddStream( m_pVB->GetVertexData(), nVerts, 0, m_pVB->GetVertexSize() );
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to initialize VBWriter (%08X).", GetName().SafeString(), hr );
        return;
    }

    if ( m_VertexFormat.m_bTangent )
    {
        hr = writer->Write( tan1.get(), "TANGENT", 0, nVerts );
        if (FAILED(hr))
        {
            ExportLog::LogError("Mesh \"%s\" failed to write tangents (%08X).", GetName().SafeString(), hr );
        }
    }

    if ( m_VertexFormat.m_bBinormal )
    {
        hr = writer->Write( tan2.get(), "BINORMAL", 0, nVerts );
        if (FAILED(hr))
        {
            ExportLog::LogError("Mesh \"%s\" failed to write bi-normals (%08X).", GetName().SafeString(), hr );
        }
    }
}

void ExportMesh::ComputeAdjacency()
{
    assert( m_pIB != 0 );
    assert( m_pVB != 0 );

    if (m_pAdjacency)
        return;

    if (!m_pVBPositions)
    {
        ExportLog::LogError("Mesh \"%s\" failed to compute adjacency; requires positions", GetName().SafeString());
        return;
    }

    size_t nFaces = m_pIB->GetIndexCount() / 3;
    size_t nVerts = m_pVB->GetVertexCount();

    m_pAdjacency.reset( new uint32_t[ nFaces * 3 ] );

    float epsilon = (g_pScene->Settings().bGeometricAdjacency) ? 1e-5f : 0.f;

    HRESULT hr = E_FAIL;
    if (m_pIB->GetIndexSize() == 2)
    {
        hr = GenerateAdjacencyAndPointReps(reinterpret_cast<const uint16_t*>(m_pIB->GetIndexData()), nFaces, m_pVBPositions.get(), nVerts, epsilon,
                                           nullptr, m_pAdjacency.get());
    }
    else
    {
        hr = GenerateAdjacencyAndPointReps(reinterpret_cast<const uint32_t*>(m_pIB->GetIndexData()), nFaces, m_pVBPositions.get(), nVerts, epsilon,
                                           nullptr, m_pAdjacency.get());
    }

    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to compute adjacency (%08X).", GetName().SafeString(), hr );
        m_pAdjacency.reset();
        return;
    }
}

static HRESULT __cdecl UVAtlasCallback( float fPercentDone  )
{
    static ULONGLONG s_lastTick = 0;

    ULONGLONG tick = GetTickCount64();

    if ( ( tick - s_lastTick ) > 1000 )
    {
        wprintf( L"%.2f%%   \r", fPercentDone * 100 );
        s_lastTick = tick;
    }

    if ( _kbhit() )
    {
        if ( _getch() == 27 )
        {
            return E_ABORT;
        }
    }

    return S_OK;
}

void ExportMesh::ComputeUVAtlas()
{
    assert( m_pIB != 0 );
    assert( m_pVB != 0 );

    ExportLog::LogMsg( 4, "Generating UV atlas..." );

    if (!m_pVBPositions)
    {
        ExportLog::LogError("UV atlas creation failed for mesh \"%s\"; requires position data.", GetName().SafeString());
        return;
    }

    ComputeAdjacency();

    if (!m_pAdjacency)
    {
        ExportLog::LogError("UV atlas creation failed for mesh \"%s\"; requires adjacency.", GetName().SafeString());
        return;
    }

    INT iDestUVIndex = g_pScene->Settings().iGenerateUVAtlasOnTexCoordIndex;
    assert( iDestUVIndex >= 0 && iDestUVIndex < 8 );

    size_t nFaces = m_pIB->GetIndexCount() / 3;
    size_t nVerts = m_pVB->GetVertexCount();

    DXGI_FORMAT indexFormat = (m_pIB->GetIndexSize() == 2) ? DXGI_FORMAT_R16_UINT : DXGI_FORMAT_R32_UINT;

    size_t texSize = g_pScene->Settings().iUVAtlasTextureSize;

    std::vector<UVAtlasVertex> vb;
    std::vector<uint8_t> ib;
    float outStretch = 0.f;
    size_t outCharts = 0;
    std::vector<uint32_t> vertexRemapArray;
    HRESULT hr = UVAtlasCreate( m_pVBPositions.get(), nVerts,
                                m_pIB->GetIndexData(), indexFormat, nFaces,
                                0, g_pScene->Settings().fUVAtlasMaxStretch, texSize, texSize,
                                g_pScene->Settings().fUVAtlasGutter,
                                m_pAdjacency.get(), nullptr,
                                nullptr,
                                UVAtlasCallback, UVATLAS_DEFAULT_CALLBACK_FREQUENCY,
                                UVATLAS_DEFAULT, vb, ib,
                                nullptr,
                                &vertexRemapArray,
                                &outStretch, &outCharts );
    if( FAILED( hr ) )
    {
        ExportLog::LogError( "UV atlas creation failed for mesh \"%s\" (%08X).", GetName().SafeString(), hr );
        return;
    }

    ExportLog::LogMsg( 4, "Created UV atlas with %Iu charts in texcoord %d.", outCharts, iDestUVIndex );

    // Update vertex buffer from UVAtlas
    size_t nNewVerts = vertexRemapArray.size();

    if ( nNewVerts != nVerts )
    {
        ExportLog::LogMsg( 4, "UV atlas increased vertex count from %Iu to %Iu.", nVerts, nNewVerts );
    }

    std::unique_ptr<XMFLOAT3[]> pos(new XMFLOAT3[nNewVerts]);
    hr = UVAtlasApplyRemap(m_pVBPositions.get(), sizeof(XMFLOAT3), nVerts, nNewVerts, vertexRemapArray.data(), pos.get());
    if (FAILED(hr))
    {
        ExportLog::LogError( "UV atlas remap failed for mesh \"%s\" (%08X).", GetName().SafeString(), hr );
        return;
    }

    DWORD stride = m_pVB->GetVertexSize();

    std::unique_ptr<ExportVB> newVB(new ExportVB);
    newVB->SetVertexCount( nNewVerts );
    newVB->SetVertexSize( stride );
    newVB->Allocate();

    hr = UVAtlasApplyRemap( m_pVB->GetVertexData(), stride, nVerts, nNewVerts, vertexRemapArray.data(), newVB->GetVertexData() );
    if (FAILED(hr))
    {
        ExportLog::LogError( "UV atlas remap failed for mesh \"%s\" (%08X).", GetName().SafeString(), hr );
        return;
    }

    // Update UVs
    std::unique_ptr<VBWriter> writer( new VBWriter() );
    hr = writer->Initialize( m_InputLayout.data(), m_InputLayout.size() );
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to create VBWriter (%08X).", GetName().SafeString(), hr );
        return;
    }

    hr = writer->AddStream( newVB->GetVertexData(), nNewVerts, 0, stride );
    if (FAILED(hr))
    {
        ExportLog::LogError("Mesh \"%s\" failed to initialize VBWriter (%08X).", GetName().SafeString(), hr );
        return;
    }

    {
        std::unique_ptr<XMFLOAT2[]> uvs(new XMFLOAT2[nNewVerts]);

        auto txptr = uvs.get();
        size_t j = 0;
        for (auto it = vb.cbegin(); it != vb.cend() && j < nNewVerts; ++it, ++txptr)
        {
            *txptr = it->uv;
        }

        hr = writer->Write(uvs.get(), "TEXCOORD", iDestUVIndex, nNewVerts);
        if (FAILED(hr))
        {
            ExportLog::LogError("Mesh \"%s\" failed to write new UV atlas texcoords (%08X).", GetName().SafeString(), hr);
            return;
        }
    }

    // Commit changes
    m_pVBPositions.swap(pos);
    m_pVB.swap(newVB);

    if (indexFormat == DXGI_FORMAT_R16_UINT)
    {
        assert((ib.size() / sizeof(uint16_t)) == nFaces * 3);
        memcpy(m_pIB->GetIndexData(), ib.data(), sizeof(uint16_t) * 3 * nFaces);
    }
    else
    {
        assert((ib.size() / sizeof(uint32_t)) == nFaces * 3);
        memcpy(m_pIB->GetIndexData(), ib.data(), sizeof(uint32_t) * 3 * nFaces);
    }

    // Invalidate other data
    m_pVBNormals.reset();
    m_pVBTexCoords.reset();
}

void ExportMesh::OptimizeVcache()
{
    assert( m_pIB != 0 );
    assert( m_pVB != 0 );

    uint32_t vertexCache = static_cast<uint32_t>( g_pScene->Settings().iVcacheSize );
    uint32_t restart = std::min<uint32_t>( vertexCache, g_pScene->Settings().iStripRestart );

    if (!vertexCache)
    {
        ExportLog::LogMsg( 4, "Optimize mesh for strip order..." );
    }
    else
    {
        ExportLog::LogMsg( 4, "Optimize mesh for vertex cache (vcache: %u, restart: %u)...", vertexCache, restart );
    }

    if (!m_pAttributes)
    {
        ExportLog::LogError("Optimize mesh for vertex cache failed for mesh \"%s\"; requires attributes.", GetName().SafeString());
        return;
    }

    ComputeAdjacency();

    if (!m_pAdjacency)
    {
        ExportLog::LogError("Optimize mesh for vertex cache failed for mesh \"%s\"; requires adjacency.", GetName().SafeString());
        return;
    }

    size_t nFaces = m_pIB->GetIndexCount() / 3;
    size_t nVerts = m_pVB->GetVertexCount();
    DWORD indexSize = m_pIB->GetIndexSize();

    // Compute stats before optimization
    float acmr, atvr;
    if (indexSize == 2)
    {
        ComputeVertexCacheMissRate(reinterpret_cast<const uint16_t*>(m_pIB->GetIndexData()), nFaces, nVerts, vertexCache, acmr, atvr);
    }
    else
    {
        ComputeVertexCacheMissRate(reinterpret_cast<const uint32_t*>(m_pIB->GetIndexData()), nFaces, nVerts, vertexCache, acmr, atvr);
    }

    ExportLog::LogMsg( 4, "Vcache miss rate before optimization - ACMR %f, ATVR %f", acmr, atvr );

    // Optimize faces for pre-transform vertex cache
    HRESULT hr = S_OK;
    std::unique_ptr<uint32_t[]> faceRemap( new uint32_t[ nFaces ] );
    if (indexSize == 2)
    {
        hr = OptimizeFacesEx( reinterpret_cast<const uint16_t*>(m_pIB->GetIndexData()), nFaces, m_pAdjacency.get(), m_pAttributes.get(),
                              faceRemap.get(), vertexCache, restart );
    }
    else
    {
        hr = OptimizeFacesEx( reinterpret_cast<const uint32_t*>(m_pIB->GetIndexData()), nFaces, m_pAdjacency.get(), m_pAttributes.get(),
                              faceRemap.get(), vertexCache, restart );
    }
    if (FAILED(hr))
    {
        ExportLog::LogError("Optimize mesh for pre-transform vertex cache failed for mesh \"%s\" (%08X)", GetName().SafeString(), hr );
        return;
    }

    std::unique_ptr<ExportIB> newIB( new ExportIB );
    newIB->SetIndexSize( indexSize );
    newIB->SetIndexCount( nFaces * 3 );
    newIB->Allocate();

    if (indexSize == 2)
    {
        hr = ReorderIB( reinterpret_cast<const uint16_t*>(m_pIB->GetIndexData()), nFaces, faceRemap.get(), reinterpret_cast<uint16_t*>(newIB->GetIndexData()) );
    }
    else
    {
        hr = ReorderIB( reinterpret_cast<const uint32_t*>(m_pIB->GetIndexData()), nFaces, faceRemap.get(), reinterpret_cast<uint32_t*>(newIB->GetIndexData()) );
    }
    if (FAILED(hr))
    {
        ExportLog::LogError("Reorder IB for pre-transform vertex cache failed for mesh \"%s\" (%08X)", GetName().SafeString(), hr );
        return;
    }

    // Optimize vertices for post-transform vertex cache
    std::unique_ptr<uint32_t> vertRemap( new uint32_t[ nVerts ] );
    if (indexSize == 2)
    {
        hr = OptimizeVertices( reinterpret_cast<const uint16_t*>(newIB->GetIndexData()), nFaces, nVerts,
                               vertRemap.get() );
    }
    else
    {
        hr = OptimizeVertices( reinterpret_cast<const uint32_t*>(newIB->GetIndexData()), nFaces, nVerts,
                               vertRemap.get() );
    }
    if (FAILED(hr))
    {
        ExportLog::LogError("Optimize mesh for post-transform vertex cache failed for mesh \"%s\" (%08X)", GetName().SafeString(), hr );
        return;
    }

    if (indexSize == 2)
    {
        hr = FinalizeIB( reinterpret_cast<uint16_t*>(newIB->GetIndexData()), nFaces, vertRemap.get(), nVerts );
    }
    else
    {
        hr = FinalizeIB( reinterpret_cast<uint32_t*>(newIB->GetIndexData()), nFaces, vertRemap.get(), nVerts );
    }
    if (FAILED(hr))
    {
        ExportLog::LogError("Finalize IB for post-transform vertex cache failed for mesh \"%s\" (%08X)", GetName().SafeString(), hr );
        return;
    }

    DWORD stride = m_pVB->GetVertexSize();

    std::unique_ptr<ExportVB> newVB(new ExportVB);
    newVB->SetVertexCount( nVerts );
    newVB->SetVertexSize( stride );
    newVB->Allocate();

    hr = FinalizeVB( m_pVB->GetVertexData(), stride, nVerts, nullptr, 0, vertRemap.get(), newVB->GetVertexData() );
    if (FAILED(hr))
    {
        ExportLog::LogError("Finalize VB for post-transform vertex cache failed for mesh \"%s\" (%08X)", GetName().SafeString(), hr );
        return;
    }

    // Compute stats after optimization
    float acmr2, atvr2;
    if (indexSize == 2)
    {
        ComputeVertexCacheMissRate(reinterpret_cast<const uint16_t*>(newIB->GetIndexData()), nFaces, nVerts, vertexCache, acmr2, atvr2);
    }
    else
    {
        ComputeVertexCacheMissRate(reinterpret_cast<const uint32_t*>(newIB->GetIndexData()), nFaces, nVerts, vertexCache, acmr2, atvr2);
    }

    ExportLog::LogMsg( 4, "Vcache miss rate after optimization - ACMR %f, ATVR %f", acmr2, atvr2 );

    if ( ( (acmr2 > acmr) && fabs (acmr2 - acmr) > 0.0001f )
         || ( (atvr2 > atvr) && fabs (atvr2 - atvr) > 0.0001f ) )
    {
        ExportLog::LogWarning("Vertex cache optimization resulted in worse ACMR/ATVR for mesh \"%s\"; ignored results", GetName().SafeString());
        return;
    }

    // Commit changes
    m_pIB.swap( newIB );
    m_pVB.swap( newVB );

    // Invalidate other data
    m_pVBPositions.reset();
    m_pVBNormals.reset();
    m_pVBTexCoords.reset();
    m_pAttributes.reset();
    m_pAdjacency.reset();
}

void NormalizeBoneWeights( BYTE* pWeights )
{
    DWORD dwSum = static_cast<DWORD>( pWeights[0] ) + static_cast<DWORD>( pWeights[1] ) + static_cast<DWORD>( pWeights[2] ) + static_cast<DWORD>( pWeights[3] );
    if( dwSum == 255 )
        return;

    INT iDifference = 255 - static_cast<INT>( dwSum );
    for( DWORD i = 0; i < 4; ++i )
    {
        if( pWeights[i] == 0 )
            continue;
        INT iValue = static_cast<INT>( pWeights[i] );
        if( iValue + iDifference > 255 )
        {
            iDifference -= ( 255 - iValue );
            iValue = 255;
        }
        else
        {
            iValue += iDifference;
            iDifference = 0;
        }
        pWeights[i] = static_cast<BYTE>( iValue );
    }

    dwSum = static_cast<DWORD>( pWeights[0] ) + static_cast<DWORD>( pWeights[1] ) + static_cast<DWORD>( pWeights[2] ) + static_cast<DWORD>( pWeights[3] );
    assert( dwSum == 255 );
}


__inline INT GetElementSizeFromDeclType(DWORD Type)
{
    switch (Type)
    {
    case D3DDECLTYPE_FLOAT1:
        return 4;
    case D3DDECLTYPE_FLOAT2:
        return 8;
    case D3DDECLTYPE_FLOAT3:
        return 12;
    case D3DDECLTYPE_FLOAT4:
        return 16;
    case D3DDECLTYPE_D3DCOLOR:
        return 4;
    case D3DDECLTYPE_UBYTE4:
        return 4;
    case D3DDECLTYPE_UBYTE4N:
        return 4;
    case D3DDECLTYPE_SHORT4N:
        return 8;
    case D3DDECLTYPE_FLOAT16_2:
        return 4;
    case D3DDECLTYPE_FLOAT16_4:
        return 8;
    case D3DDECLTYPE_UNUSED:
        return 0;
    default:
        assert(false);
        return 0;
    }
}


void TransformAndWriteVector( BYTE* pDest, XMFLOAT3* normal, const XMFLOAT3& Src, DWORD dwDestFormat )
{
    XMFLOAT3 SrcTransformed;
    g_pScene->GetDCCTransformer()->TransformDirection( &SrcTransformed, &Src );

    if (normal)
    {
        memcpy(normal, &SrcTransformed, sizeof(XMFLOAT3));
    }

    switch( dwDestFormat )
    {
    case D3DDECLTYPE_FLOAT3:
        {
            *reinterpret_cast<XMFLOAT3*>(pDest) = SrcTransformed;
            break;
        }
    case D3DDECLTYPE_UBYTE4N:
        {
            XMVECTOR v = XMLoadFloat3( &SrcTransformed );
            v = v * g_XMOneHalf;
            v += g_XMOneHalf;
            v = XMVectorSelect( g_XMOne, v, g_XMSelect1110 );

            XMUBYTEN4 UB4;
            XMStoreUByteN4( &UB4, v );
            
            *reinterpret_cast<XMUBYTEN4*>(pDest) = UB4;
            break;
        }
    case D3DDECLTYPE_SHORT4N:
        {
            *reinterpret_cast<XMSHORTN4*>(pDest) = XMSHORTN4( SrcTransformed.x, SrcTransformed.y, SrcTransformed.z, 1 );
            break;
        }
    case D3DDECLTYPE_FLOAT16_4:
        {
            *reinterpret_cast<XMHALF4*>(pDest) = XMHALF4( SrcTransformed.x, SrcTransformed.y, SrcTransformed.z, 1 );
            break;
        }
    }
}


void ExportMesh::BuildVertexBuffer( ExportMeshVertexArray& VertexArray, DWORD dwFlags )
{
    UINT uVertexSize = 0;
    INT iCurrentVertexOffset = 0;
    INT iPositionOffset = -1;
    INT iNormalOffset = -1;
    INT iTangentOffset = -1;
    INT iBinormalOffset = -1;
    INT iSkinDataOffset = -1;
    INT iColorOffset = -1;
    INT iUVOffset = -1;

    // create a vertex element struct and set default values
    D3DVERTEXELEMENT9 VertexElement;
    ZeroMemory( &VertexElement, sizeof( D3DVERTEXELEMENT9 ) );

    D3D11_INPUT_ELEMENT_DESC InputElement;
    ZeroMemory( &InputElement, sizeof( D3D11_INPUT_ELEMENT_DESC ) );

    bool bCompressVertexData = ( dwFlags & COMPRESS_VERTEX_DATA );

    DWORD dwNormalType = D3DDECLTYPE_FLOAT3;
    DXGI_FORMAT dwNormalTypeDXGI = DXGI_FORMAT_R32G32B32_FLOAT;
    if( bCompressVertexData )
    {
        dwNormalType = g_pScene->Settings().dwNormalCompressedType;

        switch(dwNormalType)
        {
        case D3DDECLTYPE_UBYTE4N:   dwNormalTypeDXGI = DXGI_FORMAT_R8G8B8A8_UNORM;      break;
        case D3DDECLTYPE_SHORT4N:   dwNormalTypeDXGI = DXGI_FORMAT_R16G16B16A16_SNORM;  break;
        case D3DDECLTYPE_FLOAT16_4: dwNormalTypeDXGI = DXGI_FORMAT_R16G16B16A16_FLOAT;  break;
        default:                    assert(false);                                      break;
        }
    }

    m_VertexElements.clear();
    m_InputLayout.clear();

    // check each vertex format option, and create a corresponding decl element
    if( m_VertexFormat.m_bPosition )
    {
        iPositionOffset = iCurrentVertexOffset;

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_POSITION;
        VertexElement.Type = D3DDECLTYPE_FLOAT3;
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "SV_Position";
        InputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
    }
    if( ( m_VertexFormat.m_bSkinData && g_pScene->Settings().bExportSkinWeights ) || g_pScene->Settings().bForceExportSkinWeights )
    {
        iSkinDataOffset = iCurrentVertexOffset;

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_BLENDWEIGHT;
        VertexElement.Type = D3DDECLTYPE_UBYTE4N;
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "BLENDWEIGHT";
        InputElement.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_BLENDINDICES;
        VertexElement.Type = D3DDECLTYPE_UBYTE4;
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "BLENDINDICES";
        InputElement.Format = DXGI_FORMAT_R8G8B8A8_UINT;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
    }
    if( m_VertexFormat.m_bNormal )
    {
        iNormalOffset = iCurrentVertexOffset;

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_NORMAL;
        VertexElement.Type = static_cast<BYTE>( dwNormalType );
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "NORMAL";
        InputElement.Format = dwNormalTypeDXGI;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
    }
    if( m_VertexFormat.m_bVertexColor )
    {
        iColorOffset = iCurrentVertexOffset;

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_COLOR;
        VertexElement.Type = D3DDECLTYPE_D3DCOLOR;
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "COLOR";
        InputElement.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
    }
    if( m_VertexFormat.m_uUVSetCount > 0 )
    {
        iUVOffset = iCurrentVertexOffset;
        for( UINT t = 0; t < m_VertexFormat.m_uUVSetCount; t++ )
        {
            VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
            VertexElement.Usage = D3DDECLUSAGE_TEXCOORD;

            InputElement.SemanticName = "TEXCOORD";
            InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );

            switch( m_VertexFormat.m_uUVSetSize )
            {
            case 1:
                VertexElement.Type = D3DDECLTYPE_FLOAT1;
                InputElement.Format = DXGI_FORMAT_R32_FLOAT;
                break;

            case 2:
                if( bCompressVertexData )
                {
                    VertexElement.Type = D3DDECLTYPE_FLOAT16_2;
                    InputElement.Format = DXGI_FORMAT_R16G16_FLOAT;
                }
                else
                {
                    VertexElement.Type = D3DDECLTYPE_FLOAT2;
                    InputElement.Format = DXGI_FORMAT_R32G32_FLOAT;
                }
                break;

            case 3:
                if( bCompressVertexData )
                {
                    VertexElement.Type = D3DDECLTYPE_FLOAT16_4;
                    InputElement.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                }
                else
                {
                    VertexElement.Type = D3DDECLTYPE_FLOAT3;
                    InputElement.Format = DXGI_FORMAT_R32G32B32_FLOAT;
                }
                break;

            case 4:
                if( bCompressVertexData )
                {
                    VertexElement.Type = D3DDECLTYPE_FLOAT16_4;
                    InputElement.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
                }
                else
                {
                    VertexElement.Type = D3DDECLTYPE_FLOAT4;
                    InputElement.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
                }
                break;

            default:
                continue;
            }

            VertexElement.UsageIndex = static_cast<BYTE>( t );
            m_VertexElements.push_back( VertexElement );

            InputElement.SemanticIndex = t;
            m_InputLayout.push_back( InputElement );

            iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
        }
        VertexElement.UsageIndex = 0;
        InputElement.SemanticIndex = 0;
    }
    if( m_VertexFormat.m_bTangent )
    {
        iTangentOffset = iCurrentVertexOffset;

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_TANGENT;
        VertexElement.Type = static_cast<BYTE>( dwNormalType );
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "TANGENT";
        InputElement.Format = dwNormalTypeDXGI;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
    }
    if( m_VertexFormat.m_bBinormal )
    {
        iBinormalOffset = iCurrentVertexOffset;

        VertexElement.Offset = static_cast<WORD>( iCurrentVertexOffset );
        VertexElement.Usage = D3DDECLUSAGE_BINORMAL;
        VertexElement.Type = static_cast<BYTE>( dwNormalType );
        m_VertexElements.push_back( VertexElement );

        InputElement.SemanticName = "BINORMAL";
        InputElement.Format = dwNormalTypeDXGI;
        InputElement.AlignedByteOffset = static_cast<UINT>( iCurrentVertexOffset );
        m_InputLayout.push_back( InputElement );

        iCurrentVertexOffset += GetElementSizeFromDeclType( VertexElement.Type );
    }

    assert( m_VertexElements.size() == m_InputLayout.size() );

    // save vertex size
    uVertexSize = iCurrentVertexOffset;
    if( uVertexSize == 0 )
        return;

    // create vertex buffer and allocate storage
    size_t nVerts = VertexArray.size();

    m_pVB.reset( new ExportVB );
    m_pVB->SetVertexCount( nVerts );
    m_pVB->SetVertexSize( uVertexSize );
    m_pVB->Allocate();

    if (iPositionOffset != -1)
    {
        m_pVBPositions.reset(new XMFLOAT3[nVerts]);
        memset(m_pVBPositions.get(), 0, sizeof(XMFLOAT3) * nVerts);
    }

    if (iNormalOffset != -1)
    {
        m_pVBNormals.reset(new XMFLOAT3[nVerts]);
        memset(m_pVBNormals.get(), 0, sizeof(XMFLOAT3) * nVerts);
    }

    if (iUVOffset != -1)
    {
        m_pVBTexCoords.reset(new XMFLOAT2[nVerts]);
        memset(m_pVBTexCoords.get(), 0, sizeof(XMFLOAT2) * nVerts);
    }

    // copy raw vertex data into the packed vertex buffer
    for( size_t i = 0; i < nVerts; i++ )
    {
        auto pDestVertex = m_pVB->GetVertex( i );
        ExportMeshVertex* pSrcVertex = VertexArray[i];
        if( !pSrcVertex )
        {
            continue;
        }

        if( iPositionOffset != -1 )
        {
            auto pDest = reinterpret_cast<XMFLOAT3*>( pDestVertex + iPositionOffset );
            g_pScene->GetDCCTransformer()->TransformPosition( pDest, &pSrcVertex->Position );

            memcpy(&m_pVBPositions[i], pDest, sizeof(XMFLOAT3) );
        }
        if( iNormalOffset != -1 )
        {
            TransformAndWriteVector( pDestVertex + iNormalOffset, &m_pVBNormals[i], pSrcVertex->Normal, dwNormalType );
        }
        if( iSkinDataOffset != -1 )
        {
            BYTE* pDest = pDestVertex + iSkinDataOffset;
            BYTE* pBoneWeights = pDest;
            *pDest++ = static_cast<BYTE>( pSrcVertex->BoneWeights.x * 255.0f );
            *pDest++ = static_cast<BYTE>( pSrcVertex->BoneWeights.y * 255.0f );
            *pDest++ = static_cast<BYTE>( pSrcVertex->BoneWeights.z * 255.0f );
            *pDest++ = static_cast<BYTE>( pSrcVertex->BoneWeights.w * 255.0f );
            NormalizeBoneWeights( pBoneWeights );
            *pDest++ = pSrcVertex->BoneIndices.x;
            *pDest++ = pSrcVertex->BoneIndices.y;
            *pDest++ = pSrcVertex->BoneIndices.z;
            *pDest++ = pSrcVertex->BoneIndices.w;
        }
        if( iTangentOffset != -1 )
        {
            TransformAndWriteVector( pDestVertex + iTangentOffset, nullptr, pSrcVertex->Tangent, dwNormalType );
        }
        if( iBinormalOffset != -1 )
        {
            TransformAndWriteVector( pDestVertex + iBinormalOffset, nullptr, pSrcVertex->Binormal, dwNormalType );
        }
        if( iColorOffset != -1 )
        {
            UINT uColor = 0;
            uColor |= ( static_cast<BYTE>( pSrcVertex->Color.w * 255.0f ) ) << 24;
            uColor |= ( static_cast<BYTE>( pSrcVertex->Color.x * 255.0f ) ) << 16;
            uColor |= ( static_cast<BYTE>( pSrcVertex->Color.y * 255.0f ) ) << 8;
            uColor |= ( static_cast<BYTE>( pSrcVertex->Color.z * 255.0f ) );
            memcpy( pDestVertex + iColorOffset, &uColor, 4 );
        }
        if( iUVOffset != -1 )
        {
            UINT iTangentSpaceIndex = g_pScene->Settings().iTangentSpaceIndex;
            if (m_VertexFormat.m_uUVSetCount > iTangentSpaceIndex)
            {
                if (m_VertexFormat.m_uUVSetSize > 1)
                {
                    memcpy(&m_pVBTexCoords[i], &pSrcVertex->TexCoords[iTangentSpaceIndex], sizeof(XMFLOAT2) );
                }
            }

            if( bCompressVertexData )
            {
                auto pDest = reinterpret_cast<DWORD*>( pDestVertex + iUVOffset ); 
                for( size_t t = 0; t < m_VertexFormat.m_uUVSetCount; t++ )
                {
                    switch( m_VertexFormat.m_uUVSetSize )
                    {
                    case 1:
                        {
                            memcpy( pDest, &pSrcVertex->TexCoords[t], sizeof(float) );
                            pDest++;
                            break;
                        }
                    case 2:
                        {
                            auto pFloat16 = reinterpret_cast<HALF*>(pDest);
                            XMConvertFloatToHalfStream( pFloat16, sizeof(HALF), reinterpret_cast<const float*>( &pSrcVertex->TexCoords[t] ), sizeof(float), 2 );
                            pDest++;
                            break;
                        }
                    case 3:
                        {
                            pDest[1] = 0;
                            auto pFloat16 = reinterpret_cast<HALF*>(pDest);
                            XMConvertFloatToHalfStream( pFloat16, sizeof(HALF), reinterpret_cast<const float*>( &pSrcVertex->TexCoords[t] ), sizeof(float), 3 );
                            pDest += 2;
                            break;
                        }
                    case 4:
                        {
                            auto pFloat16 = reinterpret_cast<HALF*>(pDest);
                            XMConvertFloatToHalfStream( pFloat16, sizeof(HALF), reinterpret_cast<const float*>( &pSrcVertex->TexCoords[t] ), sizeof(float), 4 );
                            pDest += 2;
                            break;
                        }
                    default:
                        assert( false );
                        break;
                    }
                }
            }
            else
            {
                size_t uStride = m_VertexFormat.m_uUVSetSize * sizeof( float );
                BYTE* pDest = pDestVertex + iUVOffset;
                for( UINT t = 0; t < m_VertexFormat.m_uUVSetCount; t++ )
                {
                    memcpy( pDest, &pSrcVertex->TexCoords[t], uStride );
                    pDest += uStride;
                }
            }
        }
    }
}

void ExportMesh::ComputeBounds()
{
    if( !m_pVB )
        return;

    BoundingSphere::CreateFromPoints( m_BoundingSphere,
                                      m_pVB->GetVertexCount(), reinterpret_cast<const XMFLOAT3*>( m_pVB->GetVertexData() ), m_pVB->GetVertexSize() );

    BoundingBox::CreateFromPoints( m_BoundingAABB,
                                   m_pVB->GetVertexCount(), reinterpret_cast<const XMFLOAT3*>( m_pVB->GetVertexData() ), m_pVB->GetVertexSize() );

    BoundingOrientedBox::CreateFromPoints( m_BoundingOBB,
                                           m_pVB->GetVertexCount(), reinterpret_cast<const XMFLOAT3*>( m_pVB->GetVertexData() ), m_pVB->GetVertexSize() );

    float fVolumeSphere = XM_PI * ( 4.0f / 3.0f ) * 
                          m_BoundingSphere.Radius * 
                          m_BoundingSphere.Radius * 
                          m_BoundingSphere.Radius;

    float fVolumeAABB = m_BoundingAABB.Extents.x * 
                        m_BoundingAABB.Extents.y * 
                        m_BoundingAABB.Extents.z * 8.0f;

    float fVolumeOBB = m_BoundingOBB.Extents.x *
                       m_BoundingOBB.Extents.y *
                       m_BoundingOBB.Extents.z * 8.0f;

    if( fVolumeAABB <= fVolumeSphere && fVolumeAABB <= fVolumeOBB )
        m_SmallestBound = AxisAlignedBoxBound;
    else if( fVolumeOBB <= fVolumeAABB && fVolumeOBB <= fVolumeSphere )
        m_SmallestBound = OrientedBoxBound;
    else
        m_SmallestBound = SphereBound;
}

bool ExportModel::SetSubsetBinding( ExportString SubsetName, ExportMaterial* pMaterial, bool bSkipValidation )
{
    assert( m_pMesh != nullptr );
    if( !bSkipValidation )
    {
        bool bResult = false;
        for( UINT i = 0; i < m_pMesh->GetSubsetCount(); i++ )
        {
            ExportIBSubset* pSubset = m_pMesh->GetSubset( i );
            if( pSubset->GetName() == SubsetName )
                bResult = true;
        }
        if( !bResult )
            return false;
    }
    for( size_t i = 0; i < m_vBindings.size(); i++ )
    {
        ExportMaterialSubsetBinding* pBinding = m_vBindings[i];
        if( pBinding->SubsetName == SubsetName )
        {
            pBinding->pMaterial = pMaterial;
            return true;
        }
    }
    ExportMaterialSubsetBinding* pBinding = new ExportMaterialSubsetBinding();
    pBinding->SubsetName = SubsetName;
    pBinding->pMaterial = pMaterial;
    m_vBindings.push_back( pBinding );
    return true;
}

ExportModel::~ExportModel()
{
    for( UINT i = 0; i < m_vBindings.size(); i++ )
    {
        delete m_vBindings[i];
    }
    m_vBindings.clear();
    m_pMesh = nullptr;
}

};
