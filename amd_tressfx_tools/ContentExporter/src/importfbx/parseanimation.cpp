//-------------------------------------------------------------------------------------
// ParseAnimation.cpp
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
#include "ParseAnimation.h"

using namespace ATG;
using namespace DirectX;

extern ATG::ExportScene* g_pScene;

struct AnimationScanNode
{
    INT iParentIndex;
    FbxNode* pNode;
    ExportAnimationTrack* pTrack;
    DWORD dwFlags;
    XMFLOAT4X4 matGlobal;
};

typedef std::vector<AnimationScanNode> ScanList;

void ParseNode( FbxNode* pNode, ScanList& scanlist, DWORD dwFlags, INT iParentIndex, bool bIncludeNode )
{
    INT iCurrentIndex = iParentIndex;

    if( !bIncludeNode )
    {
        const CHAR* strNodeName = pNode->GetName();
        if( _stricmp( strNodeName, g_pScene->Settings().strAnimationRootNodeName ) == 0 )
        {
            bIncludeNode = true;
        }
    }

    if( bIncludeNode )
    {
        iCurrentIndex = static_cast<INT>( scanlist.size() );

        // add node to anim list
        AnimationScanNode asn = { 0 };
        asn.iParentIndex = iParentIndex;
        asn.pNode = pNode;
        asn.dwFlags = dwFlags;
        scanlist.push_back( asn );
    }

    DWORD dwChildCount = pNode->GetChildCount();
    for( DWORD i = 0; i < dwChildCount; ++i )
    {
        ParseNode( pNode->GetChild( i ), scanlist, dwFlags, iCurrentIndex, bIncludeNode );
    }
}

static XMFLOAT4X4 ConvertMatrix( const FbxMatrix& matrix )
{
    XMFLOAT4X4 matResult;
    auto fData = reinterpret_cast<float*>( &matResult );
    auto pSrcData = reinterpret_cast<const DOUBLE*>( &matrix );
    for( DWORD i = 0; i < 16; ++i )
    {
        fData[i] = (float)pSrcData[i];
    }
    return matResult;
}


void AddKey( AnimationScanNode& asn, const AnimationScanNode* pParent, FbxAMatrix& matFBXGlobal, float fTime )
{
    XMFLOAT4X4 matGlobal = ConvertMatrix( matFBXGlobal );
    asn.matGlobal = matGlobal;
    XMFLOAT4X4 matLocal = matGlobal;
    if( pParent )
    {
        XMMATRIX m = XMLoadFloat4x4( &pParent->matGlobal );
        XMMATRIX matInvParentGlobal = XMMatrixInverse( nullptr, m );

        m = XMLoadFloat4x4( &matGlobal );
        m = XMMatrixMultiply( m, matInvParentGlobal );

        XMStoreFloat4x4( &matLocal, m );
    }

    XMMATRIX matLocalFinal;
    g_pScene->GetDCCTransformer()->TransformMatrix( reinterpret_cast<XMFLOAT4X4*>( &matLocalFinal ), &matLocal );

    XMVECTOR vScale;
    XMVECTOR qRotation;
    XMVECTOR vTranslation;
    XMMatrixDecompose( &vScale, &qRotation, &vTranslation, matLocalFinal );

    XMFLOAT3 scale;
    XMStoreFloat3( &scale, vScale );

    XMFLOAT4 rot;
    XMStoreFloat4( &rot, qRotation );

    XMFLOAT3 trans;
    XMStoreFloat3( &trans, vTranslation );

    asn.pTrack->TransformTrack.AddKey( fTime, trans, rot, scale );
}

void CaptureAnimation( ScanList& scanlist, ExportAnimation* pAnim, FbxScene* pFbxScene )
{
    const float fDeltaTime = pAnim->fSourceSamplingInterval;
    const float fStartTime = pAnim->fStartTime;
    const float fEndTime = pAnim->fEndTime;
    float fCurrentTime = fStartTime;

    size_t dwNodeCount = scanlist.size();

    ExportLog::LogMsg( 2, "Capturing animation data from %Iu nodes, from time %0.3f to %0.3f, at an interval of %0.3f seconds.", dwNodeCount, fStartTime, fEndTime, fDeltaTime );

    while( fCurrentTime <= fEndTime )
    {
        FbxTime CurrentTime;
        CurrentTime.SetSecondDouble( fCurrentTime );
        for( size_t i = 0; i < dwNodeCount; ++i )
        {
            AnimationScanNode& asn = scanlist[i];

#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
            auto pAnimEvaluator = pFbxScene->GetAnimationEvaluator();
#else
            auto pAnimEvaluator = pFbxScene->GetEvaluator();
#endif

            auto matGlobal = pAnimEvaluator->GetNodeGlobalTransform( asn.pNode, CurrentTime );
            AnimationScanNode* pParent = nullptr;
            if( asn.iParentIndex >= 0 )
                pParent = &scanlist[asn.iParentIndex];
            AddKey( asn, pParent, matGlobal, fCurrentTime - fStartTime );
        }
        fCurrentTime += fDeltaTime;
    }
}

void ParseAnimStack( FbxScene* pFbxScene, FbxString* strAnimStackName )
{
    // TODO - Ignore "Default"? FBXSDK_TAKENODE_DEFAULT_NAME

    auto curAnimStack = pFbxScene->FindMember<FbxAnimStack>( strAnimStackName->Buffer() );
    if ( !curAnimStack )
        return;

#if (FBXSDK_VERSION_MAJOR > 2014 || ((FBXSDK_VERSION_MAJOR==2014) && (FBXSDK_VERSION_MINOR>1) ) )
    pFbxScene->GetAnimationEvaluator()->Reset();
#else
    pFbxScene->GetEvaluator()->SetContext( curAnimStack );
#endif

    auto pTakeInfo = pFbxScene->GetTakeInfo( *strAnimStackName  );

    ExportLog::LogMsg( 2, "Parsing animation \"%s\"", strAnimStackName->Buffer() );

    auto pAnim = new ExportAnimation();
    pAnim->SetName( strAnimStackName->Buffer() );
    pAnim->SetDCCObject( pTakeInfo );
    g_pScene->AddAnimation( pAnim );

    FbxTime FrameTime;
    FrameTime.SetTime( 0, 0, 0, 1, 0, pFbxScene->GetGlobalSettings().GetTimeMode() );

    float fFrameTime = (float)FrameTime.GetSecondDouble();
    float fSampleTime = fFrameTime / (float)g_pScene->Settings().iAnimSampleCountPerFrame;
    assert( fSampleTime > 0 );

    float fStartTime, fEndTime;
    if( pTakeInfo )
    {
        fStartTime = (float)pTakeInfo->mLocalTimeSpan.GetStart().GetSecondDouble();
        fEndTime = (float)pTakeInfo->mLocalTimeSpan.GetStop().GetSecondDouble();
    }
    else
    {
        FbxTimeSpan tlTimeSpan;
        pFbxScene->GetGlobalSettings().GetTimelineDefaultTimeSpan( tlTimeSpan );

        fStartTime = (float)tlTimeSpan.GetStart().GetSecondDouble();
        fEndTime = (float)tlTimeSpan.GetStop().GetSecondDouble();

        ExportLog::LogWarning( "Animation take \"%s\" has no takeinfo; using defaults.", pAnim->GetName().SafeString() );
    }

    pAnim->fStartTime = fStartTime;
    pAnim->fEndTime = fEndTime;
    pAnim->fSourceFrameInterval = fFrameTime;
    pAnim->fSourceSamplingInterval = fSampleTime;

    bool bIncludeAllNodes = true;
    if( strlen( g_pScene->Settings().strAnimationRootNodeName ) > 0 )
    {
        bIncludeAllNodes = false;
    }

    ScanList scanlist;
    ParseNode( pFbxScene->GetRootNode(), scanlist, 0, -1, bIncludeAllNodes );

    size_t dwTrackCount = scanlist.size();
    for( size_t i = 0; i < dwTrackCount; ++i )
    {
        const CHAR* strTrackName = scanlist[i].pNode->GetName();
        ExportLog::LogMsg( 4, "Track: %s", strTrackName );
        auto pTrack = new ExportAnimationTrack();
        pTrack->SetName( strTrackName );
        pTrack->TransformTrack.pSourceFrame = g_pScene->FindFrameByDCCObject( scanlist[i].pNode );
        pAnim->AddTrack( pTrack );
        scanlist[i].pTrack = pTrack;
    }

    CaptureAnimation( scanlist, pAnim, pFbxScene );

    pAnim->Optimize();
}

void ParseAnimation( FbxScene* pFbxScene )
{
    assert( pFbxScene != nullptr );

    // set animation quality settings
    ExportAnimation::SetAnimationExportQuality( g_pScene->Settings().iAnimPositionExportQuality, g_pScene->Settings().iAnimOrientationExportQuality, 50 );

    FbxArray<FbxString*> AnimStackNameArray;
    pFbxScene->FillAnimStackNameArray( AnimStackNameArray );

    DWORD dwAnimStackCount = static_cast<DWORD>( AnimStackNameArray.GetCount() );
    for( DWORD i = 0; i < dwAnimStackCount; ++i )
    {
        ParseAnimStack( pFbxScene, AnimStackNameArray.GetAt(i) );
    }
}
