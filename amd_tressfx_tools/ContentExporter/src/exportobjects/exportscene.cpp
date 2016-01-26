//-------------------------------------------------------------------------------------
// ExportScene.cpp
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
#include "exportscene.h"

namespace ATG
{

void ExportStatistics::FinalReport()
{
    ULONGLONG ExportTotalTime = EndExportTime - StartExportTime;
    ULONGLONG ExportParseTime = StartSaveTime - StartSceneParseTime;
    ULONGLONG ExportSaveTime = EndExportTime - StartSaveTime;

    ExportLog::LogMsg( 2, "%Iu poly meshes consisting of %Iu vertices, %Iu triangles, and %Iu materials exported.", MeshesExported, VertsExported, TrisExported, MaterialsExported );
    if( SubDMeshesProcessed > 0 )
    {
        ExportLog::LogMsg( 2, "%Iu subdivision surface meshes processed, including %Iu quads and %Iu triangles.", SubDMeshesProcessed, SubDQuadsProcessed, SubDTrisProcessed );
    }
    ExportLog::LogMsg( 2, "Export complete in %0.2f seconds; %0.2f seconds for scene parse and %0.2f seconds for file writing.", 
        (float)ExportTotalTime / 1000.0f, (float)ExportParseTime / 1000.0f, (float)ExportSaveTime / 1000.0f );
}

ExportScene::ExportScene()
: ExportFrame(),
  m_pDCCTransformer( nullptr )
{
    m_Information.ExportTime = _time64( nullptr );
    CHAR strDomain[50];
    size_t BufferSize = ARRAYSIZE(strDomain);
    getenv_s( &BufferSize, strDomain, ARRAYSIZE(strDomain), "USERDOMAIN" );
    CHAR strUsername[50];
    getenv_s( &BufferSize, strUsername, ARRAYSIZE(strUsername), "USERNAME" );
    CHAR strTemp[256];
    sprintf_s( strTemp, "%s\\%s", strDomain, strUsername );
    m_Information.UserName = strTemp;
    CHAR strComputerName[100];
    BufferSize = ARRAYSIZE(strComputerName);
    getenv_s( &BufferSize, strComputerName, ARRAYSIZE(strComputerName), "COMPUTERNAME" );
    m_Information.MachineName = strComputerName;
    OSVERSIONINFO OSVersion = { 0 };
    OSVersion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
#pragma prefast (disable : 28159)
#pragma warning (suppress : 4996)
    GetVersionEx( &OSVersion );
    sprintf_s( strTemp, "Windows NT %u.%u build %u", OSVersion.dwMajorVersion, OSVersion.dwMinorVersion, OSVersion.dwBuildNumber );
    m_Information.PlatformName = strTemp;
}

ExportScene::~ExportScene()
{
    {
        ExportAnimationList::iterator iter = m_vAnimations.begin();
        ExportAnimationList::iterator end = m_vAnimations.end();
        while( iter != end )
        {
            delete *iter;
            ++iter;
        }
        m_vAnimations.clear();
    }
    {
        ExportMeshBaseList::iterator iter = m_vMeshes.begin();
        ExportMeshBaseList::iterator end = m_vMeshes.end();
        while( iter != end )
        {
            delete *iter;
            ++iter;
        }
        m_vMeshes.clear();
    }
    {
        ExportMaterialList::iterator iter = m_vMaterials.begin();
        ExportMaterialList::iterator end = m_vMaterials.end();
        while( iter != end )
        {
            delete *iter;
            ++iter;
        }
        m_vMaterials.clear();
    }
}

bool ExportScene::AddAnimation( ExportAnimation* pAnimation )
{
    if( !pAnimation )
        return false;
    if( FindAnimation( pAnimation->GetName() ) )
        return false;
    m_vAnimations.push_back( pAnimation );
    return true;
}

bool ExportScene::AddMaterial( ExportMaterial* pMaterial )
{
    if( !pMaterial )
        return false;
    if( FindMaterial( pMaterial->GetDCCObject() ) )
        return false;
    m_vMaterials.push_back( pMaterial );
    return true;
}

bool ExportScene::AddMesh( ExportMeshBase* pMesh )
{
    if( !pMesh )
        return false;
    if( FindMesh( pMesh->GetName() ) )
        return false;
    m_vMeshes.push_back( pMesh );
    return true;
}

ExportAnimation* ExportScene::FindAnimation( ExportString name )
{
    for( UINT i = 0; i < m_vAnimations.size(); i++ )
    {
        if( m_vAnimations[i]->GetName() == name )
            return m_vAnimations[i];
    }
    return nullptr;
}

ExportMaterial* ExportScene::FindMaterial( ExportString name )
{
    for( UINT i = 0; i < m_vMaterials.size(); i++ )
    {
        if( m_vMaterials[i]->GetName() == name )
            return m_vMaterials[i];
    }
    return nullptr;
}

ExportMeshBase* ExportScene::FindMesh( ExportString name )
{
    for( UINT i = 0; i < m_vMeshes.size(); i++ )
    {
        if( m_vMeshes[i]->GetName() == name )
            return m_vMeshes[i];
    }
    return nullptr;
}

ExportAnimation* ExportScene::FindAnimation( void* pDCCObject )
{
    for( UINT i = 0; i < m_vAnimations.size(); i++ )
    {
        if( m_vAnimations[i]->GetDCCObject() == pDCCObject )
            return m_vAnimations[i];
    }
    return nullptr;
}

ExportMaterial* ExportScene::FindMaterial( void* pDCCObject )
{
    for( UINT i = 0; i < m_vMaterials.size(); i++ )
    {
        if( m_vMaterials[i]->GetDCCObject() == pDCCObject )
            return m_vMaterials[i];
    }
    return nullptr;
}

ExportMeshBase* ExportScene::FindMesh( void* pDCCObject )
{
    for( UINT i = 0; i < m_vMeshes.size(); i++ )
    {
        if( m_vMeshes[i]->GetDCCObject() == pDCCObject )
            return m_vMeshes[i];
    }
    return nullptr;
}

};
