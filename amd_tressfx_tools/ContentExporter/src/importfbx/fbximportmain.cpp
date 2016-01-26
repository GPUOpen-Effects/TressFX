//-------------------------------------------------------------------------------------
// FBXImportMain.cpp
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
#include "FBXImportMain.h"
#include "ParseMisc.h"
#include "ParseAnimation.h"

using namespace ATG;
using namespace DirectX;

FbxManager* g_pSDKManager = nullptr;
FbxImporter* g_pImporter = nullptr;
FbxScene* g_pFBXScene = nullptr;

std::vector<FbxPose*> g_BindPoses;
PoseMap g_BindPoseMap;
bool g_bBindPoseFixupRequired = false;

extern ATG::ExportScene* g_pScene;

extern ExportPath g_CurrentOutputFileName;

void FBXTransformer::Initialize( FbxScene* pScene )
{
    ExportLog::LogMsg( 4, "Identifying scene's coordinate system." );
    auto SceneAxisSystem = pScene->GetGlobalSettings().GetAxisSystem();

    // convert scene to Maya Y up coordinate system
    FbxAxisSystem::MayaYUp.ConvertScene( pScene );

    INT iUpAxisSign;
    auto UpVector = SceneAxisSystem.GetUpVector( iUpAxisSign );

    if( UpVector == FbxAxisSystem::eZAxis )
    {
        ExportLog::LogMsg( 4, "Converting from Z-up axis system to Y-up axis system." );
        m_bMaxConversion = true;
    }
    else
    {
        m_bMaxConversion = false;
    }

    SetUnitScale( (float)g_pScene->Settings().fExportScale );
    SetZFlip( g_pScene->Settings().bFlipZ );
}

void FBXTransformer::TransformMatrix( XMFLOAT4X4* pDestMatrix, const XMFLOAT4X4* pSrcMatrix ) const
{
    XMFLOAT4X4 SrcMatrix;
    if( pSrcMatrix == pDestMatrix )
    {
        memcpy( &SrcMatrix, pSrcMatrix, sizeof( XMFLOAT4X4 ) );
        pSrcMatrix = &SrcMatrix;
    }
    memcpy( pDestMatrix, pSrcMatrix, sizeof( XMFLOAT4X4 ) );

    // What we're doing here is premultiplying by a left hand -> right hand matrix,
    // and then postmultiplying by a right hand -> left hand matrix.
    // The end result of those multiplications is that the third row and the third
    // column are negated (so element _33 is left alone).  So instead of actually
    // carrying out the multiplication, we just negate the 6 matrix elements.

    if ( m_bFlipZ )
    {
        pDestMatrix->_13 = -pSrcMatrix->_13;
        pDestMatrix->_23 = -pSrcMatrix->_23;
        pDestMatrix->_43 = -pSrcMatrix->_43;

        pDestMatrix->_31 = -pSrcMatrix->_31;
        pDestMatrix->_32 = -pSrcMatrix->_32;
        pDestMatrix->_34 = -pSrcMatrix->_34;
    }

    // Apply the global unit scale to the translation components of the matrix.
    pDestMatrix->_41 *= m_fUnitScale;
    pDestMatrix->_42 *= m_fUnitScale;
    pDestMatrix->_43 *= m_fUnitScale;
}

void FBXTransformer::TransformPosition( XMFLOAT3* pDestPosition, const XMFLOAT3* pSrcPosition ) const
{
    XMFLOAT3 SrcVector;
    if( pSrcPosition == pDestPosition )
    {
        SrcVector = *pSrcPosition;
        pSrcPosition = &SrcVector;
    }

    if( m_bMaxConversion )
    {
        pDestPosition->x = pSrcPosition->x * m_fUnitScale;
        pDestPosition->y = pSrcPosition->z * m_fUnitScale;
        pDestPosition->z = pSrcPosition->y * m_fUnitScale;
    }
    else
    {
        const float flipZ = m_bFlipZ ? -1.0f : 1.0f;

        pDestPosition->x = pSrcPosition->x * m_fUnitScale;
        pDestPosition->y = pSrcPosition->y * m_fUnitScale;
        pDestPosition->z = pSrcPosition->z * m_fUnitScale * flipZ;
    }
}

void FBXTransformer::TransformDirection( XMFLOAT3* pDestDirection, const XMFLOAT3* pSrcDirection ) const
{
    XMFLOAT3 SrcVector;
    if( pSrcDirection == pDestDirection )
    {
        SrcVector = *pSrcDirection;
        pSrcDirection = &SrcVector;
    }

    if( m_bMaxConversion )
    {
        pDestDirection->x = pSrcDirection->x;
        pDestDirection->y = pSrcDirection->z;
        pDestDirection->z = pSrcDirection->y;
    }
    else
    {
        const float flipZ = m_bFlipZ ? -1.0f : 1.0f;

        pDestDirection->x = pSrcDirection->x;
        pDestDirection->y = pSrcDirection->y;
        pDestDirection->z = pSrcDirection->z * flipZ;
    }
}

float FBXTransformer::TransformLength( float fInputLength ) const
{
    return fInputLength * m_fUnitScale;
}


HRESULT FBXImport::Initialize()
{
    if( !g_pSDKManager )
    {
        g_pSDKManager = FbxManager::Create();
        if( !g_pSDKManager )
            return E_FAIL;

        auto ios = FbxIOSettings::Create( g_pSDKManager, IOSROOT );
        if( !ios )
            return E_FAIL;

        g_pSDKManager->SetIOSettings(ios);
    }

    if( !g_pImporter )
    {
        g_pImporter = FbxImporter::Create( g_pSDKManager, "" );
        if ( !g_pImporter )
            return E_FAIL;
    }

    g_pFBXScene = FbxScene::Create( g_pSDKManager, "" );
    if ( !g_pFBXScene )
        return E_FAIL;

    return S_OK;
}

void FBXImport::ClearScene()
{
    g_pFBXScene->Clear();
}

void SetBindPose()
{
    assert( g_pFBXScene != nullptr );

    g_BindPoses.clear();
    INT iPoseCount = g_pFBXScene->GetPoseCount();
    for( INT i = 0; i < iPoseCount; ++i )
    {
        auto pPose = g_pFBXScene->GetPose( i );
        INT iNodeCount = pPose->GetCount();
        ExportLog::LogMsg( 4, "Found %spose: \"%s\" with %d nodes", pPose->IsBindPose() ? "bind " : "", pPose->GetName(), iNodeCount );
        for( INT j = 0; j < iNodeCount; ++j )
        {
            auto pPoseNode = pPose->GetNode( j );
            ExportLog::LogMsg( 5, "Pose node %d: %s", j, pPoseNode->GetName() );
        }
        if( pPose->IsBindPose() )
        {
            g_BindPoses.push_back( pPose );
        }
    }
    if( g_BindPoses.empty() )
    {
        if( g_pScene->Settings().bExportAnimations )
        {
            ExportLog::LogWarning( "No valid bind pose found; will export scene using the default pose." );
        }
        return;
    }

    size_t dwPoseCount = g_BindPoses.size();
    for( size_t i = 0; i < dwPoseCount; ++i )
    {
        FbxPose* pPose = g_BindPoses[i];
        INT iNodeCount = pPose->GetCount();
        for( INT j = 0; j < iNodeCount; ++j )
        {
            auto pNode = pPose->GetNode( j );
            auto matNode = pPose->GetMatrix( j );

            PoseMap::iterator iter = g_BindPoseMap.find( pNode );
            if( iter != g_BindPoseMap.end() )
            {
                FbxMatrix matExisting = iter->second;
                if( matExisting != matNode )
                {
                    ExportLog::LogWarning( "Node \"%s\" found in more than one bind pose, with conflicting transforms.", pNode->GetName() );
                }
            }

            g_BindPoseMap[pNode] = matNode;
        }
    }

    ExportLog::LogMsg( 3, "Created bind pose map with %Iu nodes.", g_BindPoseMap.size() );
}

HRESULT FBXImport::ImportFile( const CHAR* strFileName )
{
    assert( g_pSDKManager != nullptr );
    assert( g_pImporter != nullptr );
    assert( g_pFBXScene != nullptr );

    assert( g_pScene != nullptr );

    CHAR strTemp[200];
    g_pScene->Information().ExporterName = g_strExporterName;
    INT iMajorVersion, iMinorVersion, iRevision;
    g_pSDKManager->GetFileFormatVersion( iMajorVersion, iMinorVersion, iRevision );


    sprintf_s( strTemp, "FBX SDK %d.%d.%d", iMajorVersion, iMinorVersion, iRevision );
    g_pScene->Information().DCCNameAndVersion = strTemp;

    ExportLog::LogMsg( 2, "Compiled against %s", strTemp );
    ExportLog::LogMsg( 1, "Loading FBX file \"%s\"...", strFileName );

    INT iFileFormat = -1;
    bool bResult = g_pImporter->Initialize( strFileName, iFileFormat, g_pSDKManager->GetIOSettings() );

    if( !bResult )
    {
        ExportLog::LogError( "Could not initialize FBX importer." );
        ExportLog::LogError( g_pImporter->GetStatus().GetErrorString() );
        return E_FAIL;
    }

    bResult = g_pImporter->Import( g_pFBXScene );

    if( !bResult )
    {
        ExportLog::LogError( "Could not load FBX file \"%s\".", strFileName );
        ExportLog::LogError( g_pImporter->GetStatus().GetErrorString() );
        return E_FAIL;
    }

    ExportLog::LogMsg( 1, "FBX file \"%s\" was successfully loaded.", strFileName );
    g_pImporter->GetFileVersion( iMajorVersion, iMinorVersion, iRevision );
    ExportLog::LogMsg( 2, "FBX file version: %d.%d.%d", iMajorVersion, iMinorVersion, iRevision );

    ExportLog::LogMsg( 2, "Parsing scene." );

    auto pTransformer = reinterpret_cast<FBXTransformer*>( g_pScene->GetDCCTransformer() );
    pTransformer->Initialize( g_pFBXScene );

    SetBindPose();
    g_bBindPoseFixupRequired = false;

    assert( g_pFBXScene->GetRootNode() != nullptr );
    XMMATRIX matIdentity = XMMatrixIdentity();
    ParseNode( g_pFBXScene->GetRootNode(), g_pScene, matIdentity );

    if( g_bBindPoseFixupRequired )
    {
        ExportLog::LogMsg( 2, "Fixing up frames with updated bind pose." );
        FixupNode( g_pScene, matIdentity );
    }

    if( g_pScene->Settings().bExportAnimations )
    {
        ParseAnimation( g_pFBXScene );
        if( g_pScene->Settings().bRenameAnimationsToFileName )
        {
            auto AnimName = g_CurrentOutputFileName.GetFileNameWithoutExtension();

            size_t dwAnimCount = g_pScene->GetAnimationCount();
            for( size_t i = 0; i < dwAnimCount; ++i )
            {
                CHAR strCurrentAnimName[MAX_PATH];
                if( i > 0 )
                {
                    sprintf_s( strCurrentAnimName, "%s%Iu", (const CHAR*)AnimName, i );
                }
                else
                {
                    strcpy_s( strCurrentAnimName, (const CHAR*)AnimName );
                }
                ExportAnimation* pAnim = g_pScene->GetAnimation( i );
                ExportLog::LogMsg( 4, "Renaming animation \"%s\" to \"%s\".", pAnim->GetName().SafeString(), strCurrentAnimName );
                pAnim->SetName( strCurrentAnimName );
            }
        }
    }

    return S_OK;
}
