//-------------------------------------------------------------------------------------
// SDKMeshFileWriter.cpp
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
#include "SDKmesh.h"

extern ATG::ExportScene*     g_pScene;

using namespace DirectX;

namespace ATG
{
    std::vector<ExportFrame*>                       g_FrameArray;
    std::vector<SDKMESH_FRAME>                      g_FrameHeaderArray;
    std::vector<ExportModel*>                       g_ModelArray;
    std::vector<SDKMESH_MESH>                       g_MeshHeaderArray;
    std::vector<ExportMeshBase*>                    g_ModelMeshArray;
    std::vector<ExportVB*>                          g_VBArray;
    std::vector<SDKMESH_VERTEX_BUFFER_HEADER>       g_VBHeaderArray;
    std::vector<ExportIB*>                          g_IBArray;
    std::vector<SDKMESH_INDEX_BUFFER_HEADER>        g_IBHeaderArray;
    std::vector<SDKMESH_SUBSET>                     g_SubsetArray;
    std::vector<UINT>                               g_SubsetIndexArray;
    std::vector<UINT>                               g_FrameInfluenceArray;
    std::vector<SDKMESH_MATERIAL>                   g_MaterialArray;

    typedef std::unordered_map<ExportMaterial*,DWORD> MaterialLookupMap;
    MaterialLookupMap                               g_ExportMaterialToSDKMeshMaterialMap;

    BYTE g_Padding4K[4096] = { 0 };

    bool WriteSDKMeshAnimationFile( const CHAR* strFileName, ExportManifest* pManifest );

    void ClearSceneArrays()
    {
        g_FrameArray.clear();
        g_FrameHeaderArray.clear();
        g_ModelArray.clear();
        g_MeshHeaderArray.clear();
        g_ModelMeshArray.clear();
        g_VBArray.clear();
        g_VBHeaderArray.clear();
        g_IBArray.clear();
        g_IBHeaderArray.clear();
        g_SubsetArray.clear();
        g_SubsetIndexArray.clear();
        g_FrameInfluenceArray.clear();
        g_MaterialArray.clear();
        g_ExportMaterialToSDKMeshMaterialMap.clear();
    }

    void ProcessTexture( CHAR* strDest, const DWORD dwDestLength, const CHAR* strSrc )
    {
        size_t dwLength = strlen( strSrc ) + 1;
        if( dwLength > dwDestLength )
        {
            ExportLog::LogWarning( "Truncating texture path \"%s\".", strSrc );
            strncpy_s( strDest, dwDestLength, strSrc, dwDestLength - 1 );
            strDest[dwDestLength - 1] = '\0';
        }
        else
        {
            strcpy_s( strDest, dwDestLength, strSrc );
        }
    }

    DWORD CaptureMaterial( ExportMaterial* pMaterial )
    {
        MaterialLookupMap::iterator iter = g_ExportMaterialToSDKMeshMaterialMap.find( pMaterial );
        if( iter != g_ExportMaterialToSDKMeshMaterialMap.end() )
        {
            DWORD dwIndex = iter->second;
            return dwIndex;
        }

        SDKMESH_MATERIAL Material;
        ZeroMemory( &Material, sizeof(SDKMESH_MATERIAL) );
        strcpy_s( Material.Name, pMaterial->GetName() );

        auto pColor = pMaterial->FindParameter( "DiffuseColor" );
        if( pColor )
        {
            Material.Diffuse = XMFLOAT4( pColor->ValueFloat[0], pColor->ValueFloat[1], pColor->ValueFloat[2], pColor->ValueFloat[3] );
        }
        else
        {
            Material.Diffuse = XMFLOAT4( 1.f, 1.f, 1.f, 1.f );
        }
          
        pColor = pMaterial->FindParameter( "AmbientColor" );
        if( pColor )
        {
            Material.Ambient = XMFLOAT4( pColor->ValueFloat[0], pColor->ValueFloat[1], pColor->ValueFloat[2], 0.f );
        }
        else
        {
            Material.Ambient = XMFLOAT4( 0.f, 0.f, 0.f, 0.f );
        }

        pColor = pMaterial->FindParameter( "EmissiveColor" );
        if( pColor )
        {
            Material.Emissive = XMFLOAT4( pColor->ValueFloat[0], pColor->ValueFloat[1], pColor->ValueFloat[2], 0.f );
        }
        else
        {
            Material.Emissive = XMFLOAT4( 0.f, 0.f, 0.f, 0.f );
        }

        pColor = pMaterial->FindParameter( "SpecularColor" );
        if( pColor )
        {
            Material.Specular  = XMFLOAT4( pColor->ValueFloat[0], pColor->ValueFloat[1], pColor->ValueFloat[2], 0.f );

            auto pPower = pMaterial->FindParameter( "SpecularPower" );
            Material.Power = (pPower) ? pPower->ValueFloat[0] : 16.f;
        }
        else
        {
            Material.Specular = XMFLOAT4( 0.f, 0.f, 0.f, 0.f );
            Material.Power = 1.f;
        }

        auto pDiffuse = pMaterial->FindParameter( "DiffuseTexture" );
        if( pDiffuse )
        {
            ProcessTexture( Material.DiffuseTexture, MAX_MATERIAL_NAME, pDiffuse->ValueString.SafeString() );
        }
        auto pNormal = pMaterial->FindParameter( "NormalMapTexture" );
        if( pNormal )
        {
            ProcessTexture( Material.NormalTexture, MAX_MATERIAL_NAME, pNormal->ValueString.SafeString() );
        }
        auto pSpecular = pMaterial->FindParameter( "SpecularMapTexture" );
        if( pSpecular )
        {
            ProcessTexture( Material.SpecularTexture, MAX_MATERIAL_NAME, pSpecular->ValueString.SafeString() );
        }
        DWORD dwIndex = static_cast<DWORD>( g_MaterialArray.size() );
        g_MaterialArray.push_back( Material );
        g_ExportMaterialToSDKMeshMaterialMap[pMaterial] = dwIndex;
        return dwIndex;
    }

    void CaptureVertexBuffer( ExportVB* pVB, const D3DVERTEXELEMENT9* pElements, size_t dwElementCount )
    {
        SDKMESH_VERTEX_BUFFER_HEADER VBHeader;
        ZeroMemory( &VBHeader, sizeof( SDKMESH_VERTEX_BUFFER_HEADER ) );
        VBHeader.DataOffset = 0;
        VBHeader.SizeBytes = pVB->GetVertexDataSize();
        VBHeader.StrideBytes = pVB->GetVertexSize();
        VBHeader.NumVertices = pVB->GetVertexCount();
        memcpy( VBHeader.Decl, pElements, dwElementCount * sizeof( D3DVERTEXELEMENT9 ) );
        static D3DVERTEXELEMENT9 EndElement = { 0xFF, 0, D3DDECLTYPE_UNUSED, 0, 0, 0 };
        VBHeader.Decl[ dwElementCount ] = EndElement;
        g_VBArray.push_back( pVB );
        g_VBHeaderArray.push_back( VBHeader );
    }

    void CaptureIndexBuffer( ExportIB* pIB )
    {
        SDKMESH_INDEX_BUFFER_HEADER IBHeader;
        ZeroMemory( &IBHeader, sizeof( SDKMESH_INDEX_BUFFER_HEADER ) );
        IBHeader.DataOffset = 0;
        IBHeader.IndexType = pIB->GetIndexSize() == 2 ? IT_16BIT : IT_32BIT;
        IBHeader.NumIndices = pIB->GetIndexCount();
        IBHeader.SizeBytes = (UINT64)pIB->GetIndexDataSize();
        g_IBArray.push_back( pIB );
        g_IBHeaderArray.push_back( IBHeader );
    }

    void CapturePolyMesh( ExportMesh* pMesh )
    {
        ExportSubDProcessMesh* pSubDMesh = pMesh->GetSubDMesh();
        if( pSubDMesh )
        {
            CaptureIndexBuffer( pSubDMesh->GetQuadPatchIB() );
            CaptureVertexBuffer( pMesh->GetVB(), &pMesh->GetVertexDeclElement( 0 ), pMesh->GetVertexDeclElementCount() );
            CaptureVertexBuffer( pSubDMesh->GetQuadPatchDataVB(), pSubDMesh->GetPatchDataDecl(), pSubDMesh->GetPatchDataDeclElementCount() );
            if( pSubDMesh->GetTrianglePatchDataVB() )
            {
                ExportLog::LogWarning( "Subdivision surface mesh \"%s\" contains triangle patches, which are not currently written to SDKMESH files.", pMesh->GetName().SafeString() );
            }
        }
        else
        {
            CaptureIndexBuffer( pMesh->GetIB() );
            CaptureVertexBuffer( pMesh->GetVB(), &pMesh->GetVertexDeclElement( 0 ), pMesh->GetVertexDeclElementCount() );
        }
    }

    void CaptureSubset( ExportMeshBase* pMeshBase, ExportMaterialSubsetBinding* pBinding, size_t dwMaxVertexCount )
    {
        ExportIBSubset* pIBSubset = pMeshBase->FindSubset( pBinding->SubsetName );
        assert( pIBSubset != nullptr );
        DWORD dwMaterialIndex = CaptureMaterial( pBinding->pMaterial );
        SDKMESH_SUBSET Subset;
        Subset.IndexStart = pIBSubset->GetStartIndex();
        Subset.IndexCount = pIBSubset->GetIndexCount();
        Subset.MaterialID = dwMaterialIndex;
        Subset.VertexStart = 0;
        Subset.VertexCount = static_cast<UINT64>( dwMaxVertexCount );
        switch( pIBSubset->GetPrimitiveType() )
        {
        case ExportIBSubset::TriangleList:
            Subset.PrimitiveType = PT_TRIANGLE_LIST;
            break;
        case ExportIBSubset::TriangleStrip:
            Subset.PrimitiveType = PT_TRIANGLE_STRIP;
            break;
        default:
            Subset.PrimitiveType = PT_TRIANGLE_LIST;
            break;
        }
        strcpy_s( Subset.Name, pIBSubset->GetName().SafeString() );
        g_SubsetArray.push_back( Subset );
    }

    void CaptureSubDSubset( ExportSubDProcessMesh* pSubDMesh, ExportMaterialSubsetBinding* pBinding, size_t dwMaxVertexCount )
    {
        DWORD dwMaterialIndex = CaptureMaterial( pBinding->pMaterial );
        ExportSubDPatchSubset* pSubset = pSubDMesh->FindSubset( pBinding->SubsetName );
        SDKMESH_SUBSET Subset;
        Subset.IndexStart = pSubset->dwStartPatch;
        Subset.IndexCount = pSubset->dwPatchCount;
        Subset.MaterialID = dwMaterialIndex;
        Subset.VertexStart = 0;
        Subset.VertexCount = static_cast<UINT64>( dwMaxVertexCount );
        Subset.PrimitiveType = pSubset->bQuadPatches ? PT_QUAD_PATCH_LIST : PT_TRIANGLE_PATCH_LIST;
        strcpy_s( Subset.Name, pSubset->Name.SafeString() );
        g_SubsetArray.push_back( Subset );
    }

    void CaptureModel( ExportModel* pModel )
    {
        g_ModelArray.push_back( pModel );
        ExportMeshBase* pMeshBase = pModel->GetMesh();
        g_ModelMeshArray.push_back( pMeshBase );

        SDKMESH_MESH MeshHeader;
        ZeroMemory( &MeshHeader, sizeof( SDKMESH_MESH ) );
        strcpy_s( MeshHeader.Name, pMeshBase->GetName().SafeString() );

        switch( pMeshBase->GetSmallestBound() )
        {
        case ExportMeshBase::SphereBound:
            {
                MeshHeader.BoundingBoxCenter = pMeshBase->GetBoundingSphere().Center;
                float fSize = pMeshBase->GetBoundingSphere().Radius;
                MeshHeader.BoundingBoxExtents = XMFLOAT3( fSize, fSize, fSize );
                break;
            }
        case ExportMeshBase::AxisAlignedBoxBound:
            MeshHeader.BoundingBoxCenter = pMeshBase->GetBoundingAABB().Center;
            MeshHeader.BoundingBoxExtents = pMeshBase->GetBoundingAABB().Extents;
            break;
        case ExportMeshBase::OrientedBoxBound:
            MeshHeader.BoundingBoxCenter = pMeshBase->GetBoundingOBB().Center;
            MeshHeader.BoundingBoxExtents = pMeshBase->GetBoundingOBB().Extents;
            break;
        }

        MeshHeader.NumFrameInfluences = static_cast<UINT>( pMeshBase->GetInfluenceCount() );

        ExportSubDProcessMesh* pSubDMesh = nullptr;
        size_t dwMaxVertexCount = 0;
        size_t dwMaxIndexCount = 0;
        MeshHeader.IndexBuffer = static_cast<UINT>( g_IBArray.size() );
        switch( pMeshBase->GetMeshType() )
        {
        case ExportMeshBase::PolyMesh:
            {
                auto pMesh = reinterpret_cast<ExportMesh*>( pMeshBase );
                pSubDMesh = pMesh->GetSubDMesh();
                if( pSubDMesh )
                {
                    MeshHeader.NumVertexBuffers = 2;
                    MeshHeader.VertexBuffers[0] = static_cast<UINT>( g_VBArray.size() );
                    MeshHeader.VertexBuffers[1] = static_cast<UINT>( g_VBArray.size() + 1 );
                }
                else
                {
                    MeshHeader.NumVertexBuffers = 1;
                    MeshHeader.VertexBuffers[0] = static_cast<UINT>( g_VBArray.size() );
                }
                CapturePolyMesh( pMesh );
                dwMaxVertexCount = pMesh->GetVB()->GetVertexCount();
                dwMaxIndexCount = pMesh->GetIB()->GetIndexCount();
            }
            break;
        }

        MeshHeader.NumSubsets = static_cast<UINT>( pModel->GetBindingCount() );
        for( DWORD i = 0; i < MeshHeader.NumSubsets; ++i )
        {
            g_SubsetIndexArray.push_back( static_cast<UINT>( g_SubsetArray.size() ) );
            if( pSubDMesh )
            {
                CaptureSubDSubset( pSubDMesh, pModel->GetBinding( i ), dwMaxVertexCount );
            }
            else
            {
                CaptureSubset( pMeshBase, pModel->GetBinding( i ), dwMaxVertexCount );
            }
        }

        if ( !MeshHeader.NumSubsets )
        {
            MeshHeader.NumSubsets = 1;

            ExportLog::LogWarning( "No model binding for mesh \"%s\" so creating a default subset", pMeshBase->GetName().SafeString() );

            g_SubsetIndexArray.push_back( static_cast<UINT>( g_SubsetArray.size() ) );
                
            SDKMESH_SUBSET Subset = {0};
            Subset.IndexStart = 0;
            Subset.IndexCount = static_cast<UINT64>( dwMaxIndexCount );
            Subset.MaterialID = 0;
            Subset.VertexStart = 0;
            Subset.VertexCount = static_cast<UINT64>( dwMaxVertexCount );
            Subset.PrimitiveType = PT_TRIANGLE_LIST;
            g_SubsetArray.push_back( Subset );
        }

        g_MeshHeaderArray.push_back( MeshHeader );
    }

    void CaptureScene( ExportFrame* pRootFrame, UINT dwParentIndex )
    {
        SDKMESH_FRAME Frame;
        ZeroMemory( &Frame, sizeof( SDKMESH_FRAME ) );
        strcpy_s( Frame.Name, pRootFrame->GetName().SafeString() );
        Frame.Matrix = pRootFrame->Transform().Matrix();
        Frame.ParentFrame = dwParentIndex;
        Frame.AnimationDataIndex = INVALID_ANIMATION_DATA;
        size_t dwCurrentIndex = g_FrameArray.size();

        size_t dwModelCount = pRootFrame->GetModelCount();
        if ( dwModelCount == 0 )
        {
            Frame.Mesh = INVALID_MESH;
        }
        else
        {
            // Only one mesh per frame is supported in the SDKMesh format.
            if( dwModelCount > 1 )
            {
                ExportLog::LogWarning( "Frame \"%s\" has %Iu meshes.  Only one mesh per frame is supported in the SDKMesh format.", pRootFrame->GetName().SafeString(), dwModelCount );
            }
            Frame.Mesh = static_cast<UINT>( g_MeshHeaderArray.size() );
            ExportModel* pModel = pRootFrame->GetModelByIndex( 0 );
            CaptureModel( pModel );
        }

        UINT dwChildIndex = INVALID_FRAME;
        size_t dwChildCount = pRootFrame->GetChildCount();
        if( dwChildCount > 0 )
            dwChildIndex = static_cast<DWORD>( g_FrameHeaderArray.size() + 1 );
        Frame.ChildFrame = static_cast<UINT>( dwChildIndex );
        Frame.SiblingFrame = INVALID_FRAME;

        g_FrameHeaderArray.push_back( Frame );
        g_FrameArray.push_back( pRootFrame );

        DWORD dwPreviousSiblingIndex = INVALID_FRAME;
        DWORD dwCurrentSiblingIndex = INVALID_FRAME;
        for( size_t i = 0; i < dwChildCount; ++i )
        {
            dwPreviousSiblingIndex = dwCurrentSiblingIndex;
            dwCurrentSiblingIndex = static_cast<DWORD>( g_FrameHeaderArray.size() );
            if( dwPreviousSiblingIndex != INVALID_FRAME )
            {
                g_FrameHeaderArray[ dwPreviousSiblingIndex ].SiblingFrame = dwCurrentSiblingIndex;
            }
            CaptureScene( pRootFrame->GetChildByIndex( i ), static_cast<UINT>( dwCurrentIndex ) );
        }
    }

    UINT FindFrame( ExportString Name )
    {
        size_t dwFrameCount = g_FrameArray.size();
        for( size_t i = 0; i < dwFrameCount; ++i )
        {
            if( g_FrameArray[i]->GetName() == Name )
                return static_cast<UINT>( i );
        }
        return INVALID_FRAME;
    }

    void CaptureSecondPass()
    {
        // Create frame influence lists
        assert( g_MeshHeaderArray.size() == g_ModelMeshArray.size() );
        size_t dwMeshCount = g_MeshHeaderArray.size();
        for( size_t i = 0; i < dwMeshCount; ++i )
        {
            SDKMESH_MESH& Mesh = g_MeshHeaderArray[i];
            const ExportMeshBase* pMeshBase = g_ModelMeshArray[i];
            for( DWORD j = 0; j < Mesh.NumFrameInfluences; ++j )
            {
                ExportString InfluenceName = pMeshBase->GetInfluence( j );
                DWORD dwFrameIndex = FindFrame( InfluenceName );
                g_FrameInfluenceArray.push_back( static_cast<UINT>( dwFrameIndex ) );
            }

            if ( !Mesh.NumFrameInfluences )
            {
                Mesh.NumFrameInfluences = 1;
                g_FrameInfluenceArray.push_back( 0 );
            }
        }
    }

    inline DWORD RoundUp4K( DWORD dwValue )
    {
        return ( ( dwValue + 4095 ) / 4096 ) * 4096;
    }

    inline DWORD RoundUp32B( DWORD dwValue )
    {
        return ( ( dwValue + 31 ) / 32 ) * 32;
    }

    size_t ComputeMeshHeaderIndexDataSize()
    {
        return ( ( g_SubsetIndexArray.size() + g_FrameInfluenceArray.size() ) * sizeof( UINT ) ); 
    }

    DWORD ComputeBufferDataSize()
    {
        DWORD dwDataSize = 0;
        size_t dwVBCount = g_VBHeaderArray.size();
        for( size_t i = 0; i < dwVBCount; ++i )
        {
            SDKMESH_VERTEX_BUFFER_HEADER& VBHeader = g_VBHeaderArray[i];
            dwDataSize += RoundUp4K( static_cast<DWORD>( VBHeader.SizeBytes ) );
        }
        size_t dwIBCount = g_IBHeaderArray.size();
        for( size_t i = 0; i < dwIBCount; ++i )
        {
            SDKMESH_INDEX_BUFFER_HEADER& IBHeader = g_IBHeaderArray[i];
            dwDataSize += RoundUp4K( static_cast<DWORD>( IBHeader.SizeBytes ) );
        }
        return dwDataSize;
    }

    void WriteVertexBufferHeaders( HANDLE hFile, UINT64& DataOffset )
    {
        size_t dwVBCount = g_VBHeaderArray.size();
        for( size_t i = 0; i < dwVBCount; ++i )
        {
            SDKMESH_VERTEX_BUFFER_HEADER& VBHeader = g_VBHeaderArray[i];
            VBHeader.DataOffset = DataOffset;
            DataOffset += RoundUp4K( static_cast<DWORD>( VBHeader.SizeBytes ) );
            DWORD dwBytesWritten = 0;
            WriteFile( hFile, &VBHeader, sizeof( SDKMESH_VERTEX_BUFFER_HEADER ), &dwBytesWritten, nullptr );
        }
    }

    void WriteIndexBufferHeaders( HANDLE hFile, UINT64& DataOffset )
    {
        size_t dwIBCount = g_IBHeaderArray.size();
        for( size_t i = 0; i < dwIBCount; ++i )
        {
            SDKMESH_INDEX_BUFFER_HEADER& IBHeader = g_IBHeaderArray[i];
            IBHeader.DataOffset = DataOffset;
            DataOffset += RoundUp4K( static_cast<DWORD>( IBHeader.SizeBytes ) );
            DWORD dwBytesWritten = 0;
            WriteFile( hFile, &IBHeader, sizeof( SDKMESH_INDEX_BUFFER_HEADER ), &dwBytesWritten, nullptr );
        }
    }

    void WriteMeshes( HANDLE hFile, UINT64& DataOffset )
    {
        size_t dwMeshCount = g_MeshHeaderArray.size();
        DWORD dwBytesWritten = 0;
        for( size_t i = 0; i < dwMeshCount; ++i )
        {
            SDKMESH_MESH& Mesh = g_MeshHeaderArray[i];
            Mesh.SubsetOffset = DataOffset;
            DataOffset += Mesh.NumSubsets * sizeof( UINT );
            Mesh.FrameInfluenceOffset = DataOffset;
            DataOffset += Mesh.NumFrameInfluences * sizeof( UINT );
            WriteFile( hFile, &Mesh, sizeof( SDKMESH_MESH ), &dwBytesWritten, nullptr );
        }
    }

    void WriteSubsetIndexAndFrameInfluenceData( HANDLE hFile )
    {
        size_t dwMeshCount = g_MeshHeaderArray.size();
        DWORD dwSubsetIndexCount = 0;
        DWORD dwFrameInfluenceCount = 0;
        DWORD dwBytesWritten = 0;
        for( size_t i = 0; i < dwMeshCount; ++i )
        {
            SDKMESH_MESH& Mesh = g_MeshHeaderArray[i];
            if( Mesh.NumSubsets > 0 )
            {
                WriteFile( hFile, &g_SubsetIndexArray[ dwSubsetIndexCount ], Mesh.NumSubsets * sizeof( UINT ), &dwBytesWritten, nullptr );
                dwSubsetIndexCount += Mesh.NumSubsets;
            }
            if( Mesh.NumFrameInfluences > 0 )
            {
                WriteFile( hFile, &g_FrameInfluenceArray[ dwFrameInfluenceCount ], Mesh.NumFrameInfluences * sizeof( UINT ), &dwBytesWritten, nullptr );
                dwFrameInfluenceCount += Mesh.NumFrameInfluences;
            }
        }
    }

    void WriteVertexBufferData( HANDLE hFile )
    {
        assert( g_VBHeaderArray.size() == g_VBArray.size() );
        size_t dwVBCount = g_VBHeaderArray.size();
        DWORD dwBytesWritten = 0;
        for( size_t i = 0; i < dwVBCount; ++i )
        {
            const ExportVB* pVB = g_VBArray[i];
            DWORD dwDataSize = static_cast<DWORD>( pVB->GetVertexDataSize() );
            WriteFile( hFile, pVB->GetVertexData(), dwDataSize, &dwBytesWritten, nullptr );
            DWORD dwPaddingSize = RoundUp4K( dwDataSize ) - dwDataSize;
            assert( dwPaddingSize < 4096 );
            WriteFile( hFile, g_Padding4K, dwPaddingSize, &dwBytesWritten, nullptr );
        }
    }

    void WriteIndexBufferData( HANDLE hFile )
    {
        assert( g_IBHeaderArray.size() == g_IBArray.size() );
        size_t dwIBCount = g_IBHeaderArray.size();
        DWORD dwBytesWritten = 0;
        for( size_t i = 0; i < dwIBCount; ++i )
        {
            const ExportIB* pIB = g_IBArray[i];
            DWORD dwDataSize = static_cast<DWORD>( pIB->GetIndexDataSize() );
            WriteFile( hFile, pIB->GetIndexData(), dwDataSize, &dwBytesWritten, nullptr );
            DWORD dwPaddingSize = RoundUp4K( dwDataSize ) - dwDataSize;
            assert( dwPaddingSize < 4096 );
            WriteFile( hFile, g_Padding4K, dwPaddingSize, &dwBytesWritten, nullptr );
        }
    }

    bool WriteSDKMeshFile( const CHAR* strFileName, ExportManifest* pManifest )
    {
        if( !g_pScene )
            return false;

        ClearSceneArrays();

        CaptureScene( g_pScene, INVALID_FRAME );
        CaptureSecondPass();

        if ( !g_SubsetArray.empty() && g_MaterialArray.empty() )
        {
            ExportLog::LogWarning( "No materials defined, so creating a default material" );
            SDKMESH_MATERIAL defMaterial;
            memset(&defMaterial, 0, sizeof(SDKMESH_MATERIAL));
            defMaterial.Ambient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.f);
            defMaterial.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.f);
            defMaterial.Power = 1.f;
            g_MaterialArray.push_back( defMaterial );
        }

        if ( !g_SubsetArray.empty() && g_FrameHeaderArray.empty() )
        {
            ExportLog::LogWarning( "No frames defined, so creating root frame" );
            SDKMESH_FRAME defFrame;
            memset(&defFrame, 0, sizeof(SDKMESH_FRAME));
            strcpy_s( defFrame.Name, "root");
            defFrame.ParentFrame = defFrame.ChildFrame = defFrame.SiblingFrame = DWORD(-1);
            defFrame.AnimationDataIndex = INVALID_ANIMATION_DATA;
            XMMATRIX id = XMMatrixIdentity();
            XMStoreFloat4x4( &defFrame.Matrix, id );
            g_FrameHeaderArray.push_back( defFrame );
        }

        HANDLE hFile = CreateFileA( strFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );
        if( hFile == INVALID_HANDLE_VALUE )
        {
            ExportLog::LogError( "Could not write to file \"%s\".  Check that the file is not read-only and that the path exists.", strFileName );
            return false;
        }

        ExportLog::LogMsg( 1, "Writing to SDKMESH file \"%s\"", strFileName );

        SDKMESH_HEADER FileHeader;
        ZeroMemory( &FileHeader, sizeof( SDKMESH_HEADER ) );

        FileHeader.Version = SDKMESH_FILE_VERSION;
        FileHeader.IsBigEndian = static_cast<BYTE>(!g_pScene->Settings().bLittleEndian);

        FileHeader.NumFrames = static_cast<UINT>( g_FrameArray.size() );
        FileHeader.NumMaterials = static_cast<UINT>( g_MaterialArray.size() );
        FileHeader.NumMeshes = static_cast<UINT>( g_MeshHeaderArray.size() );
        FileHeader.NumTotalSubsets = static_cast<UINT>( g_SubsetArray.size() );
        FileHeader.NumIndexBuffers = static_cast<UINT>( g_IBHeaderArray.size() );
        FileHeader.NumVertexBuffers = static_cast<UINT>( g_VBHeaderArray.size() );

        FileHeader.HeaderSize = sizeof( SDKMESH_HEADER ) +
                                FileHeader.NumVertexBuffers * sizeof( SDKMESH_VERTEX_BUFFER_HEADER ) + 
                                FileHeader.NumIndexBuffers * sizeof( SDKMESH_INDEX_BUFFER_HEADER );

        UINT64 StaticDataSize = FileHeader.NumMeshes * sizeof( SDKMESH_MESH ) +
                                 FileHeader.NumTotalSubsets * sizeof( SDKMESH_SUBSET ) +
                                 FileHeader.NumFrames * sizeof( SDKMESH_FRAME ) + 
                                 FileHeader.NumMaterials * sizeof( SDKMESH_MATERIAL );

        FileHeader.NonBufferDataSize = StaticDataSize + ComputeMeshHeaderIndexDataSize();

        FileHeader.BufferDataSize = ComputeBufferDataSize();


        FileHeader.VertexStreamHeadersOffset = sizeof( SDKMESH_HEADER );
        FileHeader.IndexStreamHeadersOffset = FileHeader.VertexStreamHeadersOffset + FileHeader.NumVertexBuffers * sizeof( SDKMESH_VERTEX_BUFFER_HEADER );
        FileHeader.MeshDataOffset = FileHeader.IndexStreamHeadersOffset + FileHeader.NumIndexBuffers * sizeof( SDKMESH_INDEX_BUFFER_HEADER );
        FileHeader.SubsetDataOffset = FileHeader.MeshDataOffset + FileHeader.NumMeshes * sizeof(SDKMESH_MESH);
        FileHeader.FrameDataOffset = FileHeader.SubsetDataOffset + FileHeader.NumTotalSubsets * sizeof(SDKMESH_SUBSET);
        FileHeader.MaterialDataOffset = FileHeader.FrameDataOffset + FileHeader.NumFrames * sizeof(SDKMESH_FRAME);

        // Write header to file
        DWORD dwBytesWritten = 0;
        WriteFile( hFile, &FileHeader, sizeof( SDKMESH_HEADER ), &dwBytesWritten, nullptr );

        UINT64 BufferDataOffset = FileHeader.HeaderSize + FileHeader.NonBufferDataSize;

        // Write VB headers
        WriteVertexBufferHeaders( hFile, BufferDataOffset );

        // Write IB headers
        WriteIndexBufferHeaders( hFile, BufferDataOffset );

        // Write meshes
        UINT64 SubsetListOffset = FileHeader.HeaderSize + StaticDataSize;
        WriteMeshes( hFile, SubsetListOffset );

        // Write subsets
        size_t dwSubsetCount = g_SubsetArray.size();
        for( size_t i = 0; i < dwSubsetCount; ++i )
        {
            SDKMESH_SUBSET& Subset = g_SubsetArray[i];
            WriteFile( hFile, &Subset, sizeof( SDKMESH_SUBSET ), &dwBytesWritten, nullptr );
        }

        // Write frames
        size_t dwFrameCount = g_FrameHeaderArray.size();
        for( size_t i = 0; i < dwFrameCount; ++i )
        {
            SDKMESH_FRAME& Frame = g_FrameHeaderArray[i];
            WriteFile( hFile, &Frame, sizeof( SDKMESH_FRAME ), &dwBytesWritten, nullptr );
        }

        // Write materials
        size_t dwMaterialCount = g_MaterialArray.size();
        for( size_t i = 0; i < dwMaterialCount; ++i )
        {
            SDKMESH_MATERIAL& Material = g_MaterialArray[i];
            WriteFile( hFile, &Material, sizeof( SDKMESH_MATERIAL ), &dwBytesWritten, nullptr );
        }

        // Write subset index lists and frame influence lists
        WriteSubsetIndexAndFrameInfluenceData( hFile );

        // Write VB data
        WriteVertexBufferData( hFile );
        
        // Write IB data
        WriteIndexBufferData( hFile );

        CloseHandle( hFile );

        ClearSceneArrays();

        WriteSDKMeshAnimationFile( strFileName, pManifest );

        return true;
    }

    bool SamplePositionData( ExportAnimationPositionKey* pKeys, size_t dwKeyCount, SDKANIMATION_DATA* pDestKeys, size_t dwDestKeyCount, float fKeyInterval )
    {
        if( dwKeyCount == 0 )
            return false;

        DWORD dwCurrentSrcKey = 0;
        bool bEndKey = false;
        ExportAnimationPositionKey StartKey = pKeys[dwCurrentSrcKey];
        ExportAnimationPositionKey EndKey;
        if( dwKeyCount > 1 )
        {
            bEndKey = true;
            EndKey = pKeys[dwCurrentSrcKey + 1];
        }

        float fTime = 0;
        for( size_t i = 0; i < dwDestKeyCount; ++i )
        {
            while( bEndKey && fTime >= EndKey.fTime )
            {
                StartKey = EndKey;
                ++dwCurrentSrcKey;
                if( dwCurrentSrcKey >= dwKeyCount )
                    bEndKey = false;
                else
                    EndKey = pKeys[dwCurrentSrcKey + 1];
            }
            if( !bEndKey )
            {
                pDestKeys[i].Translation = StartKey.Position;
            }
            else
            {
                assert( fTime <= EndKey.fTime );
                float fLerpFactor = ( fTime - StartKey.fTime ) / ( EndKey.fTime - StartKey.fTime );
                fLerpFactor = std::min( std::max( 0.0f, fLerpFactor ), 1.0f );

                XMVECTOR v = XMLoadFloat3( &StartKey.Position );
                XMVECTOR vEnd = XMLoadFloat3( &EndKey.Position );

                v = XMVectorLerp( v, vEnd, fLerpFactor );

                XMStoreFloat3( &pDestKeys[i].Translation, v );
            }
            fTime += fKeyInterval;
        }
        return true;
    }

    bool SampleOrientationData( ExportAnimationOrientationKey* pKeys, size_t dwKeyCount, SDKANIMATION_DATA* pDestKeys, size_t dwDestKeyCount, float fKeyInterval )
    {
        if( dwKeyCount == 0 )
            return false;

        DWORD dwCurrentSrcKey = 0;
        bool bEndKey = false;
        ExportAnimationOrientationKey StartKey = pKeys[dwCurrentSrcKey];
        ExportAnimationOrientationKey EndKey;
        if( dwKeyCount > 1 )
        {
            bEndKey = true;
            EndKey = pKeys[dwCurrentSrcKey + 1];
        }

        float fTime = 0;
        for( size_t i = 0; i < dwDestKeyCount; ++i )
        {
            while( bEndKey && fTime >= EndKey.fTime )
            {
                StartKey = EndKey;
                ++dwCurrentSrcKey;
                if( dwCurrentSrcKey >= dwKeyCount )
                    bEndKey = false;
                else
                    EndKey = pKeys[dwCurrentSrcKey + 1];
            }
            if( !bEndKey )
            {
                pDestKeys[i].Orientation = (XMFLOAT4)StartKey.Orientation;
            }
            else
            {
                assert( fTime <= EndKey.fTime );
                float fLerpFactor = ( fTime - StartKey.fTime ) / ( EndKey.fTime - StartKey.fTime );
                fLerpFactor = std::min( std::max( 0.0f, fLerpFactor ), 1.0f );

                XMVECTOR v = XMLoadFloat4( &StartKey.Orientation );
                XMVECTOR vEnd = XMLoadFloat4( &EndKey.Orientation );

                v = XMVectorLerp( v, vEnd, fLerpFactor );

                XMStoreFloat4( &pDestKeys[i].Orientation, v );
            }
            fTime += fKeyInterval;
        }
        return true;
    }

    bool SampleScaleData( ExportAnimationScaleKey* pKeys, size_t dwKeyCount, SDKANIMATION_DATA* pDestKeys, size_t dwDestKeyCount, float fKeyInterval )
    {
        if( dwKeyCount == 0 )
            return false;

        DWORD dwCurrentSrcKey = 0;
        bool bEndKey = false;
        ExportAnimationScaleKey StartKey = pKeys[dwCurrentSrcKey];
        ExportAnimationScaleKey EndKey;
        if( dwKeyCount > 1 )
        {
            bEndKey = true;
            EndKey = pKeys[dwCurrentSrcKey + 1];
        }

        float fTime = 0;
        for( size_t i = 0; i < dwDestKeyCount; ++i )
        {
            while( bEndKey && fTime >= EndKey.fTime )
            {
                StartKey = EndKey;
                ++dwCurrentSrcKey;
                if( dwCurrentSrcKey >= dwKeyCount )
                    bEndKey = false;
                else
                    EndKey = pKeys[dwCurrentSrcKey + 1];
            }
            if( !bEndKey )
            {
                pDestKeys[i].Scaling = StartKey.Scale;
            }
            else
            {
                assert( fTime <= EndKey.fTime );
                float fLerpFactor = ( fTime - StartKey.fTime ) / ( EndKey.fTime - StartKey.fTime );
                fLerpFactor = std::min( std::max( 0.0f, fLerpFactor ), 1.0f );

                XMVECTOR v = XMLoadFloat3( &StartKey.Scale );
                XMVECTOR vEnd = XMLoadFloat3( &EndKey.Scale );

                v = XMVectorLerp( v, vEnd, fLerpFactor );

                XMStoreFloat3( &pDestKeys[i].Scaling, v );
            }
            fTime += fKeyInterval;
        }
        return true;
    }

    bool WriteSDKMeshAnimationFile( const CHAR* strFileName, ExportManifest* pManifest )
    {
        if( !g_pScene || g_pScene->GetAnimationCount() == 0 )
            return false;

        if( !g_pScene->Settings().bExportAnimations )
            return false;

        ExportAnimation* pAnim = g_pScene->GetAnimation( 0 );
        const size_t dwTrackCount = pAnim->GetTrackCount();
        if ( dwTrackCount == 0 )
            return false;

        CHAR strAnimFileName[MAX_PATH];
        strcpy_s( strAnimFileName, strFileName );
        strcat_s( strAnimFileName, "_anim" );

        HANDLE hFile = CreateFileA( strAnimFileName, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr );
        if( hFile == INVALID_HANDLE_VALUE )
        {
            ExportLog::LogError( "Could not write to file \"%s\".  Check that the file is not read-only and that the path exists.", strAnimFileName );
            return false;
        }

        ExportLog::LogMsg( 1, "Writing to SDKMESH animation file \"%s\"", strAnimFileName );

        SDKANIMATION_FILE_HEADER AnimHeader;
        ZeroMemory( &AnimHeader, sizeof( SDKANIMATION_FILE_HEADER ) );

        const size_t dwKeyCount = size_t( ( pAnim->GetDuration() + 0.5f*pAnim->fSourceFrameInterval ) / pAnim->fSourceFrameInterval );
        size_t dwTrackHeadersDataSize = dwTrackCount * sizeof( SDKANIMATION_FRAME_DATA );
        size_t dwSingleTrackDataSize = dwKeyCount * sizeof( SDKANIMATION_DATA );

        AnimHeader.Version = SDKMESH_FILE_VERSION;
        AnimHeader.IsBigEndian = !g_pScene->Settings().bLittleEndian;
        AnimHeader.FrameTransformType = FTT_RELATIVE;
        AnimHeader.NumAnimationKeys = static_cast<UINT>( dwKeyCount );
        AnimHeader.AnimationFPS = static_cast<UINT>( 1.001f / pAnim->fSourceFrameInterval );
        AnimHeader.NumFrames = static_cast<UINT>( dwTrackCount );
        AnimHeader.AnimationDataSize = dwTrackHeadersDataSize + dwTrackCount * dwSingleTrackDataSize;
        AnimHeader.AnimationDataOffset = sizeof( SDKANIMATION_FILE_HEADER );

        DWORD dwBytesWritten = 0;
        WriteFile( hFile, &AnimHeader, sizeof( SDKANIMATION_FILE_HEADER ), &dwBytesWritten, nullptr );

        for( size_t i = 0; i < dwTrackCount; ++i )
        {
            ExportAnimationTrack* pTrack = pAnim->GetTrack( i );
            if ( !pTrack )
                return false;

            ExportFrame* pSourceFrame = pTrack->TransformTrack.pSourceFrame;
            SDKANIMATION_FRAME_DATA FrameData;
            ZeroMemory( &FrameData, sizeof( SDKANIMATION_FRAME_DATA ) );
            FrameData.DataOffset = dwTrackHeadersDataSize + i * dwSingleTrackDataSize;
            if( !pSourceFrame )
            {
                strncpy_s( FrameData.FrameName, pTrack->GetName().SafeString(), MAX_FRAME_NAME );
            }
            else
            {
                strncpy_s( FrameData.FrameName, pSourceFrame->GetName().SafeString(), MAX_FRAME_NAME );
            }
            WriteFile( hFile, &FrameData, sizeof( SDKANIMATION_FRAME_DATA ), &dwBytesWritten, nullptr );
        }

        std::unique_ptr<SDKANIMATION_DATA[]> pTrackData( new SDKANIMATION_DATA[ dwKeyCount ] );

        for( size_t i = 0; i < dwTrackCount; ++i )
        {
            ExportAnimationTrack* pTrack = pAnim->GetTrack( i );
            if ( !pTrack )
                return false;

            ExportAnimationTransformTrack* pTT = &pTrack->TransformTrack;

            ZeroMemory( pTrackData.get(), dwKeyCount * sizeof( SDKANIMATION_DATA ) );
            SamplePositionData( pTT->GetPositionKeys(), pTT->GetPositionKeyCount(), pTrackData.get(), dwKeyCount, pAnim->fSourceFrameInterval );
            SampleOrientationData( pTT->GetOrientationKeys(), pTT->GetOrientationKeyCount(), pTrackData.get(), dwKeyCount, pAnim->fSourceFrameInterval );
            SampleScaleData( pTT->GetScaleKeys(), pTT->GetScaleKeyCount(), pTrackData.get(), dwKeyCount, pAnim->fSourceFrameInterval );

            WriteFile( hFile, pTrackData.get(), static_cast<DWORD>( dwKeyCount * sizeof( SDKANIMATION_DATA ) ), &dwBytesWritten, nullptr );
        }

        pTrackData.reset();

        CloseHandle( hFile );

        return true;
    }
}
