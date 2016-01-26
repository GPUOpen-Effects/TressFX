//-------------------------------------------------------------------------------------
// ParseMesh.cpp
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

#include "StdAfx.h"
#include "ParseMesh.h"
#include "ParseMaterial.h"
#include "ParseMisc.h"

using namespace ATG;
using namespace DirectX;

extern ATG::ExportScene* g_pScene;

class SkinData
{
public:
    std::vector<FbxNode*> InfluenceNodes;
    size_t dwVertexCount;
    DWORD dwVertexStride;
    std::unique_ptr<BYTE[]> pBoneIndices;
    std::unique_ptr<float[]> pBoneWeights;

    SkinData()
        : dwVertexCount( 0 ),
          dwVertexStride( 0 )
    {
    }

    ~SkinData()
    {
    }

    void Alloc( size_t dwCount, DWORD dwStride )
    {
        dwVertexCount = dwCount;
        dwVertexStride = dwStride;

        size_t dwBufferSize = dwVertexCount * dwVertexStride;
        pBoneIndices.reset( new BYTE[ dwBufferSize ] );
        ZeroMemory( pBoneIndices.get(), sizeof(BYTE) * dwBufferSize );

        pBoneWeights.reset( new float[ dwBufferSize ] );
        ZeroMemory( pBoneWeights.get(), sizeof(float) * dwBufferSize );
    }


    BYTE* GetIndices( size_t dwIndex ) 
    { 
        assert( dwIndex < dwVertexCount ); 
        return pBoneIndices.get() + ( dwIndex * dwVertexStride ); 
    }
    float* GetWeights( size_t dwIndex ) 
    { 
        assert( dwIndex < dwVertexCount ); 
        return pBoneWeights.get() + ( dwIndex * dwVertexStride ); 
    }

    DWORD GetBoneCount() const { return static_cast<DWORD>( InfluenceNodes.size() ); }

    void InsertWeight( size_t dwIndex, DWORD dwBoneIndex, float fBoneWeight )
    {
        assert( dwBoneIndex < 256 );

        auto pIndices = GetIndices( dwIndex );
        auto pWeights = GetWeights( dwIndex );

        for( DWORD i = 0; i < dwVertexStride; ++i )
        {
            if( fBoneWeight > pWeights[i] )
            {
                for( DWORD j = (dwVertexStride - 1); j > i; --j )
                {
                    pIndices[j] = pIndices[j - 1];
                    pWeights[j] = pWeights[j - 1];
                }
                pIndices[i] = static_cast<BYTE>( dwBoneIndex );
                pWeights[i] = fBoneWeight;
                break;
            }
        }
    }
};

void CaptureBindPoseMatrix( FbxNode* pNode, const FbxMatrix& matBindPose )
{
    PoseMap::iterator iter = g_BindPoseMap.find( pNode );
    if( iter != g_BindPoseMap.end() )
    {
        FbxMatrix matExisting = iter->second;
        if( matExisting != matBindPose )
        {
            // found the bind pose matrix, but it is different than what we prevoiusly encountered
            g_BindPoseMap[pNode] = matBindPose;
            g_bBindPoseFixupRequired = true;
            ExportLog::LogMsg( 4, "Updating bind pose matrix for frame \"%s\"", pNode->GetName() );
        }
    }
    else
    {
        // have not encountered this frame in the bind pose yet
        g_BindPoseMap[pNode] = matBindPose;
        g_bBindPoseFixupRequired = true;
        ExportLog::LogMsg( 4, "Adding bind pose matrix for frame \"%s\"", pNode->GetName() );
    }
}

bool ParseMeshSkinning( FbxMesh* pMesh, SkinData* pSkinData )
{
    DWORD dwDeformerCount = pMesh->GetDeformerCount( FbxDeformer::eSkin );
    if( dwDeformerCount == 0 )
        return false;

    ExportLog::LogMsg( 4, "Parsing skin weights on mesh %s", pMesh->GetName() );

    const DWORD dwVertexCount = pMesh->GetControlPointsCount();
    const DWORD dwStride = 4;
    pSkinData->Alloc( dwVertexCount, dwStride );

    for( DWORD dwDeformerIndex = 0; dwDeformerIndex < dwDeformerCount; ++dwDeformerIndex )
    {
        auto pSkin = reinterpret_cast<FbxSkin*>( pMesh->GetDeformer( dwDeformerIndex, FbxDeformer::eSkin ) );
        DWORD dwClusterCount = pSkin->GetClusterCount();

        for( DWORD dwClusterIndex = 0; dwClusterIndex < dwClusterCount; ++dwClusterIndex )
        {
            auto pCluster = pSkin->GetCluster( dwClusterIndex );
            DWORD dwClusterSize = pCluster->GetControlPointIndicesCount();
            if( dwClusterSize == 0 )
                continue;

            auto pLink = pCluster->GetLink();

            DWORD dwBoneIndex = pSkinData->GetBoneCount();
            pSkinData->InfluenceNodes.push_back( pLink );
            ExportLog::LogMsg( 4, "Influence %u: %s", dwBoneIndex, pLink->GetName() );
            
            FbxAMatrix matXBindPose;
            pCluster->GetTransformLinkMatrix( matXBindPose );
            FbxAMatrix matReferenceGlobalInitPosition;
            pCluster->GetTransformMatrix(matReferenceGlobalInitPosition);
 
            FbxMatrix matBindPose = matReferenceGlobalInitPosition.Inverse() * matXBindPose;

            CaptureBindPoseMatrix( pLink, matBindPose );

            INT* pIndices = pCluster->GetControlPointIndices();
            DOUBLE* pWeights = pCluster->GetControlPointWeights();

            for( DWORD i = 0; i < dwClusterSize; ++i )
            {
                pSkinData->InsertWeight( pIndices[i], dwBoneIndex, (float)pWeights[i] );
            }
        }
    }

    return true;
}

void ParseMesh( FbxNode* pNode, FbxMesh* pFbxMesh, ExportFrame* pParentFrame, bool bSubDProcess, const CHAR* strSuffix )
{
    if( !g_pScene->Settings().bExportMeshes )
        return;

    if( !pNode || !pFbxMesh )
        return;

    const CHAR* strName = pFbxMesh->GetName();
    if( !strName || strName[0] == '\0' )
        strName = pParentFrame->GetName().SafeString();

    if( !strSuffix )
    {
        strSuffix = "";
    }
    CHAR strDecoratedName[512];
    sprintf_s( strDecoratedName, "%s_%s%s", g_pScene->Settings().strMeshNameDecoration, strName, strSuffix );
    ExportMesh* pMesh = new ExportMesh( strDecoratedName );
    pMesh->SetDCCObject( pFbxMesh );

    bool bSmoothMesh = false;

    auto Smoothness = pFbxMesh->GetMeshSmoothness();
    if( Smoothness != FbxMesh::eHull && g_pScene->Settings().bConvertMeshesToSubD )
    {
        bSubDProcess = true;
        bSmoothMesh = true;
    }

    ExportLog::LogMsg( 2, "Parsing %s mesh \"%s\", renamed to \"%s\"", bSmoothMesh ? "smooth" : "poly", strName, strDecoratedName );

    SkinData skindata;
    bool bSkinnedMesh = ParseMeshSkinning( pFbxMesh, &skindata );
    if( bSkinnedMesh )
    {
        DWORD dwBoneCount = skindata.GetBoneCount();
        for( DWORD i = 0; i < dwBoneCount; ++i )
        {
            pMesh->AddInfluence( skindata.InfluenceNodes[i]->GetName() );
        }
    }

    bool bExportColors = g_pScene->Settings().bExportColors;
    pMesh->SetVertexColorCount( 0 );

    // Vertex normals and tangent spaces
    if( !g_pScene->Settings().bExportNormals )
    {
        pMesh->SetVertexNormalCount( 0 );
    }
    else if( g_pScene->Settings().bComputeVertexTangentSpace )
    {
        if( g_pScene->Settings().bExportBinormal )
            pMesh->SetVertexNormalCount( 3 );
        else
            pMesh->SetVertexNormalCount( 2 );
    }
    else
    {
        pMesh->SetVertexNormalCount( 1 );
    }

    DWORD dwLayerCount = pFbxMesh->GetLayerCount();
    ExportLog::LogMsg( 4, "%u layers in FBX mesh", dwLayerCount );

    if (!dwLayerCount || !pFbxMesh->GetLayer(0)->GetNormals())
    {
        ExportLog::LogMsg( 4, "Generating normals..." );
        pFbxMesh->InitNormals();
#if (FBXSDK_VERSION_MAJOR >= 2015)
        pFbxMesh->GenerateNormals();
#else
        pFbxMesh->ComputeVertexNormals();
#endif
    }

    DWORD dwVertexColorCount = 0;
    FbxLayerElementVertexColor* pVertexColorSet = nullptr;
    DWORD dwUVSetCount = 0;
    FbxLayerElementMaterial* pMaterialSet = nullptr;
    std::vector<FbxLayerElementUV*> VertexUVSets;
    for( DWORD dwLayerIndex = 0; dwLayerIndex < dwLayerCount; ++dwLayerIndex )
    {
        if( pFbxMesh->GetLayer(dwLayerIndex)->GetVertexColors() && bExportColors )
        {
            if( dwVertexColorCount == 0 )
            {
                dwVertexColorCount++;
                pVertexColorSet = pFbxMesh->GetLayer(dwLayerIndex)->GetVertexColors();
            }
            else
            {
                ExportLog::LogWarning( "Only one vertex color set is allowed; ignoring additional vertex color sets." );
            }
        }
        if( pFbxMesh->GetLayer(dwLayerIndex)->GetUVs() )
        {
            dwUVSetCount++;
            VertexUVSets.push_back( pFbxMesh->GetLayer(dwLayerIndex)->GetUVs() );
        }
        if( pFbxMesh->GetLayer(dwLayerIndex)->GetMaterials() )
        {
            if( pMaterialSet )
            {
                ExportLog::LogWarning( "Multiple material layers detected on mesh %s.  Some will be ignored.", pMesh->GetName().SafeString() );
            }
            pMaterialSet = pFbxMesh->GetLayer(dwLayerIndex)->GetMaterials();
        }
    }

    std::vector<ExportMaterial*> MaterialList;
    for( int dwMaterial = 0; dwMaterial < pNode->GetMaterialCount(); ++dwMaterial )
    {
        auto pMat = pNode->GetMaterial( dwMaterial );
        if ( !pMat )
            continue;

        auto pMaterial = ParseMaterial( pMat );
        MaterialList.push_back( pMaterial );
    }

    ExportLog::LogMsg( 4, "Found %u UV sets", dwUVSetCount );
    dwUVSetCount = std::min<DWORD>( dwUVSetCount, g_pScene->Settings().iMaxUVSetCount );
    ExportLog::LogMsg( 4, "Using %u UV sets", dwUVSetCount );

    pMesh->SetVertexColorCount( dwVertexColorCount );
    pMesh->SetVertexUVCount( dwUVSetCount );
    // TODO: Does FBX only support 2D texture coordinates?
    pMesh->SetVertexUVDimension( 2 );

    DWORD dwMeshOptimizationFlags = 0;
    if( g_pScene->Settings().bCompressVertexData )
        dwMeshOptimizationFlags |= ExportMesh::COMPRESS_VERTEX_DATA;

    DWORD dwPolyCount = pFbxMesh->GetPolygonCount();
    // Assume that polys are usually quads.
    g_MeshTriangleAllocator.SetSizeHint( dwPolyCount * 2 );

    DWORD dwVertexCount = pFbxMesh->GetControlPointsCount();
    auto pVertexPositions = pFbxMesh->GetControlPoints();

    if( bSkinnedMesh )
    {
        assert( skindata.dwVertexCount == dwVertexCount );
    }
    
    ExportLog::LogMsg( 4, "%u vertices, %u polygons", dwVertexCount, dwPolyCount );

    DWORD dwNonConformingSubDPolys = 0;

    // Compute total transformation
    FbxAMatrix vertMatrix;
    FbxAMatrix normMatrix;
    {
        auto trans = pNode->GetGeometricTranslation( FbxNode::eSourcePivot );
        auto rot = pNode->GetGeometricRotation( FbxNode::eSourcePivot );
        auto scale = pNode->GetGeometricScaling( FbxNode::eSourcePivot );

        FbxAMatrix geom;
        geom.SetT( trans );
        geom.SetR( rot );
        geom.SetS( scale );

        if ( g_pScene->Settings().bExportAnimations || !g_pScene->Settings().bApplyGlobalTrans )
        {
            vertMatrix = geom;
        }
        else
        {
            auto global = pNode->EvaluateGlobalTransform();
            vertMatrix = global * geom;
        }

        // Calculate the normal transform matrix (inverse-transpose)
        normMatrix = vertMatrix;
        normMatrix = normMatrix.Inverse();
        normMatrix = normMatrix.Transpose();
    }

    const bool bInvertTexVCoord = g_pScene->Settings().bInvertTexVCoord;
    
    // Loop over polygons.
    DWORD basePolyIndex = 0;
    for( DWORD dwPolyIndex = 0; dwPolyIndex < dwPolyCount; ++dwPolyIndex )
    {
        // Triangulate each polygon into one or more triangles.
        DWORD dwPolySize = pFbxMesh->GetPolygonSize( dwPolyIndex );
        assert( dwPolySize >= 3 );
        DWORD dwTriangleCount = dwPolySize - 2;
        assert( dwTriangleCount > 0 );

        if( dwPolySize > 4 )
        {
            ++dwNonConformingSubDPolys;
        }

        DWORD dwMaterialIndex = 0;
        if( pMaterialSet )
        {
            switch( pMaterialSet->GetMappingMode() )
            {
            case FbxLayerElement::eByPolygon:
                switch( pMaterialSet->GetReferenceMode() )
                {
                case FbxLayerElement::eDirect:
                    dwMaterialIndex = dwPolyIndex;
                    break;
                case FbxLayerElement::eIndex:
                case FbxLayerElement::eIndexToDirect:
                    dwMaterialIndex = pMaterialSet->GetIndexArray().GetAt( dwPolyIndex );
                    break;
                }
            }
        }

        DWORD dwCornerIndices[3];
        // Loop over triangles in the polygon.
        for( DWORD dwTriangleIndex = 0; dwTriangleIndex < dwTriangleCount; ++dwTriangleIndex )
        {
            dwCornerIndices[0] = pFbxMesh->GetPolygonVertex( dwPolyIndex, 0 );
            dwCornerIndices[1] = pFbxMesh->GetPolygonVertex( dwPolyIndex, dwTriangleIndex + 1 );
            dwCornerIndices[2] = pFbxMesh->GetPolygonVertex( dwPolyIndex, dwTriangleIndex + 2 );

            //ExportLog::LogMsg( 4, "Poly %d Triangle %d: %d %d %d", dwPolyIndex, dwTriangleIndex, dwCornerIndices[0], dwCornerIndices[1], dwCornerIndices[2] );

            FbxVector4 vNormals[3];
            ZeroMemory( vNormals, 3 * sizeof(FbxVector4) );
            INT iPolyIndex = static_cast<INT>( dwPolyIndex );
            INT iVertIndex[3] = { 0, static_cast<INT>( dwTriangleIndex + 1 ), static_cast<INT>( dwTriangleIndex + 2 ) };
            pFbxMesh->GetPolygonVertexNormal( iPolyIndex, iVertIndex[0], vNormals[0] );
            pFbxMesh->GetPolygonVertexNormal( iPolyIndex, iVertIndex[1], vNormals[1] );
            pFbxMesh->GetPolygonVertexNormal( iPolyIndex, iVertIndex[2], vNormals[2] );

            // Build the raw triangle.
            auto pTriangle = g_MeshTriangleAllocator.GetNewTriangle();

            // Store polygon index
            pTriangle->PolygonIndex = static_cast<INT>( dwPolyIndex );

            // Store material subset index
            pTriangle->SubsetIndex = dwMaterialIndex;

            for( DWORD dwCornerIndex = 0; dwCornerIndex < 3; ++dwCornerIndex )
            {
                const DWORD& dwDCCIndex = dwCornerIndices[dwCornerIndex];
                // Store DCC vertex index (this helps the mesh reduction/VB generation code)
                pTriangle->Vertex[dwCornerIndex].DCCVertexIndex = dwDCCIndex;

                // Store vertex position
                auto finalPos = vertMatrix.MultT( pVertexPositions[dwDCCIndex] );

                pTriangle->Vertex[dwCornerIndex].Position.x = (float)finalPos.mData[0];
                pTriangle->Vertex[dwCornerIndex].Position.y = (float)finalPos.mData[1];
                pTriangle->Vertex[dwCornerIndex].Position.z = (float)finalPos.mData[2];

                // Store vertex normal
                auto finalNorm = vNormals[dwCornerIndex];
                finalNorm.mData[3] = 0.0;
                finalNorm = normMatrix.MultT( finalNorm );
                finalNorm.Normalize();

                pTriangle->Vertex[dwCornerIndex].Normal.x = (float)finalNorm.mData[0];
                pTriangle->Vertex[dwCornerIndex].Normal.y = (float)finalNorm.mData[1];
                pTriangle->Vertex[dwCornerIndex].Normal.z = (float)finalNorm.mData[2];

                // Store UV sets
                for( DWORD dwUVIndex = 0; dwUVIndex < dwUVSetCount; ++dwUVIndex )
                {
                    // Crack apart the FBX dereferencing system for UV coordinates
                    FbxLayerElementUV* pUVSet = VertexUVSets[dwUVIndex];
                    FbxVector2 Value( 0, 0 );
                    switch( pUVSet->GetMappingMode() )
                    {
                    case FbxLayerElement::eByControlPoint:
                        switch (pUVSet->GetReferenceMode())
                        {
                        case FbxLayerElement::eDirect:
                            Value = pUVSet->GetDirectArray().GetAt(dwDCCIndex);
                            break;

                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                            {
                                int iUVIndex = pUVSet->GetIndexArray().GetAt(dwDCCIndex);
                                Value = pUVSet->GetDirectArray().GetAt(iUVIndex);
                            }
                            break;
                        }
                        break;

                    case FbxLayerElement::eByPolygonVertex:
                        switch (pUVSet->GetReferenceMode())
                        {
                        case FbxLayerElement::eDirect:
                            Value = pUVSet->GetDirectArray().GetAt( basePolyIndex + iVertIndex[dwCornerIndex] );
                            break;

                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                            {
                                int iUVIndex = pUVSet->GetIndexArray().GetAt( basePolyIndex + iVertIndex[dwCornerIndex] );
#ifdef _DEBUG
                                if (!dwUVIndex)
                                {
                                    // Warning: pFbxMesh->GetTextureUVIndex only works for the first layer of the mesh
                                    int iUVIndex2 = pFbxMesh->GetTextureUVIndex(iPolyIndex, iVertIndex[dwCornerIndex]);
                                    assert(iUVIndex == iUVIndex2);
                                }
#endif
                                Value = pUVSet->GetDirectArray().GetAt( iUVIndex );
                            }
                            break;
                        }
                        break;
                    }

                    // Store a single UV set
                    pTriangle->Vertex[dwCornerIndex].TexCoords[dwUVIndex].x = (float)Value.mData[0];
                    if( bInvertTexVCoord )
                    {
                        pTriangle->Vertex[dwCornerIndex].TexCoords[dwUVIndex].y = 1.0f - (float) Value.mData[1];
                    }
                    else
                    {
                        pTriangle->Vertex[dwCornerIndex].TexCoords[dwUVIndex].y = (float)Value.mData[1];
                    }
                }

                // Store vertex color set
                if( dwVertexColorCount > 0 && pVertexColorSet )
                {
                    // Crack apart the FBX dereferencing system for Color coordinates
                    FbxColor Value( 1, 1, 1, 1 );
                    switch( pVertexColorSet->GetMappingMode() )
                    {
                    case FbxLayerElement::eByControlPoint:
                        switch( pVertexColorSet->GetReferenceMode() )
                        {
                        case FbxLayerElement::eDirect:
                            Value = pVertexColorSet->GetDirectArray().GetAt( dwDCCIndex );
                            break;
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                            {
                                int iColorIndex = pVertexColorSet->GetIndexArray().GetAt(dwDCCIndex);
                                Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
                            }
                            break;
                        }
                        break;

                    case FbxLayerElement::eByPolygonVertex:
                        switch( pVertexColorSet->GetReferenceMode() )
                        {
                        case FbxLayerElement::eDirect:
                            Value = pVertexColorSet->GetDirectArray().GetAt( basePolyIndex + iVertIndex[dwCornerIndex] );
                            break;
                        case FbxLayerElement::eIndex:
                        case FbxLayerElement::eIndexToDirect:
                            {
                                int iColorIndex = pVertexColorSet->GetIndexArray().GetAt( basePolyIndex + iVertIndex[dwCornerIndex] );
                                Value = pVertexColorSet->GetDirectArray().GetAt(iColorIndex);
                            }
                            break;
                        }
                        break;
                    }

                    // Store a single vertex color set
                    pTriangle->Vertex[dwCornerIndex].Color.x = (float)Value.mRed;
                    pTriangle->Vertex[dwCornerIndex].Color.y = (float)Value.mGreen;
                    pTriangle->Vertex[dwCornerIndex].Color.z = (float)Value.mBlue;
                    pTriangle->Vertex[dwCornerIndex].Color.w = (float)Value.mAlpha;
                }

                // Store skin weights
                if( bSkinnedMesh )
                {
                    memcpy( &pTriangle->Vertex[dwCornerIndex].BoneIndices, skindata.GetIndices( dwDCCIndex ), sizeof(PackedVector::XMUBYTE4) );
                    memcpy( &pTriangle->Vertex[dwCornerIndex].BoneWeights, skindata.GetWeights( dwDCCIndex ), sizeof(XMFLOAT4) );
                }
            }

            // Add raw triangle to the mesh.
            pMesh->AddRawTriangle( pTriangle );
        }

        basePolyIndex += dwPolySize;
    }

    if( bSubDProcess )
    {
        dwMeshOptimizationFlags |= ExportMesh::FORCE_SUBD_CONVERSION;
    }

    if ( g_pScene->Settings().bCleanMeshes )
    {
        dwMeshOptimizationFlags |= ExportMesh::CLEAN_MESHES;
    }

    if ( g_pScene->Settings().bOptimizeVCache )
    {
        dwMeshOptimizationFlags |= ExportMesh::CLEAN_MESHES | ExportMesh::VCACHE_OPT;
    }

    pMesh->Optimize( dwMeshOptimizationFlags );

    ExportModel* pModel = new ExportModel( pMesh );
    size_t dwMaterialCount = MaterialList.size();
    if( !pMesh->GetSubDMesh() )
    {
        for( size_t dwSubset = 0; dwSubset < dwMaterialCount; ++dwSubset )
        {
            auto pMaterial = MaterialList[dwSubset];
            auto pSubset = pMesh->GetSubset( dwSubset );
            CHAR strUniqueSubsetName[100];
            sprintf_s( strUniqueSubsetName, "subset%Iu_%s", dwSubset, pMaterial->GetName().SafeString() );
            pSubset->SetName( strUniqueSubsetName );
            pModel->SetSubsetBinding( pSubset->GetName(), pMaterial );
        }
    }
    else
    {
        auto pSubDMesh = pMesh->GetSubDMesh();
        size_t dwSubsetCount = pSubDMesh->GetSubsetCount();
        for( size_t dwSubset = 0; dwSubset < dwSubsetCount; ++dwSubset )
        {
            auto pSubset = pSubDMesh->GetSubset( dwSubset );
            assert( pSubset != nullptr );
            assert( pSubset->iOriginalMeshSubset < static_cast<INT>( dwMaterialCount ) );
            auto pMaterial = MaterialList[pSubset->iOriginalMeshSubset];
            CHAR strUniqueSubsetName[100];
            sprintf_s( strUniqueSubsetName, "subset%Iu_%s", dwSubset, pMaterial->GetName().SafeString() );
            pSubset->Name = strUniqueSubsetName;
            pModel->SetSubsetBinding( pSubset->Name, pMaterial, true );
        }
    }

    if( bSubDProcess && ( dwNonConformingSubDPolys > 0 ) )
    {
        ExportLog::LogWarning( "Encountered %u polygons with 5 or more sides in mesh \"%s\", which were subdivided into quad and triangle patches.  Mesh appearance may have been affected.", dwNonConformingSubDPolys, pMesh->GetName().SafeString() );
    }

    // update statistics
    if( pMesh->GetSubDMesh() )
    {
        g_pScene->Statistics().SubDMeshesProcessed++;
        g_pScene->Statistics().SubDQuadsProcessed += pMesh->GetSubDMesh()->GetQuadPatchCount();
        g_pScene->Statistics().SubDTrisProcessed += pMesh->GetSubDMesh()->GetTrianglePatchCount();
    }
    else
    {
        g_pScene->Statistics().TrisExported += pMesh->GetIB()->GetIndexCount() / 3;
        g_pScene->Statistics().VertsExported += pMesh->GetVB()->GetVertexCount();
        g_pScene->Statistics().MeshesExported++;
    }

    pParentFrame->AddModel( pModel );
    g_pScene->AddMesh( pMesh );
}

void ParseSubDiv( FbxNode* pNode, FbxSubDiv* pFbxSubD, ExportFrame* pParentFrame )
{
    if( !g_pScene->Settings().bExportMeshes )
        return;

    if( !pFbxSubD )
    {
        return;
    }

    const CHAR* strName = pFbxSubD->GetName();
    if( !strName || strName[0] == '\0' )
        strName = pParentFrame->GetName().SafeString();

    DWORD dwLevelCount = static_cast<DWORD>( pFbxSubD->GetLevelCount() );
    ExportLog::LogMsg( 2, "Parsing subdivision surface \"%s\" with %u levels", strName, dwLevelCount );
    if( dwLevelCount == 0 )
    {
        ExportLog::LogWarning( "Subdivision surface \"%s\" has no levels.", strName );
        return;
    }

    FbxMesh* pLevelMesh = nullptr;
    DWORD dwCurrentLevel = dwLevelCount - 1;
    while( !pLevelMesh && dwCurrentLevel > 0 )
    {
        pLevelMesh = pFbxSubD->GetMesh( dwCurrentLevel );
        if( !pLevelMesh )
        {
            --dwCurrentLevel;
        }
    }
    if( !pLevelMesh )
    {
        pLevelMesh = pFbxSubD->GetBaseMesh();
    }

    assert( pLevelMesh != nullptr );

    ExportLog::LogMsg( 3, "Parsing level %u", dwCurrentLevel );
    CHAR strSuffix[32];
    sprintf_s( strSuffix, "_level%u", dwCurrentLevel );
    ParseMesh( pNode, pLevelMesh, pParentFrame, true, strSuffix );
}
