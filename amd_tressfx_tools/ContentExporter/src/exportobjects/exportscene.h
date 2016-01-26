//-------------------------------------------------------------------------------------
// ExportScene.h
//
// The root class of the scene hierarchy.  Exporter settings are also contained here.
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

#include "ExportSettings.h"

namespace ATG
{

class ExportMaterial;
class ExportMeshBase;
class ExportAnimation;

typedef std::vector<ExportMaterial*> ExportMaterialList;
typedef std::vector<ExportAnimation*> ExportAnimationList;
typedef std::vector<ExportMeshBase*> ExportMeshBaseList;

class ExportStatistics
{
public:
    ExportStatistics()
    {
        ZeroMemory( this, sizeof( ExportStatistics ) );
    }
    void StartExport() { StartExportTime = GetTickCount64(); }
    void StartSceneParse() { StartSceneParseTime = GetTickCount64(); }
    void StartSave() { StartSaveTime = GetTickCount64(); }
    void EndExport() { EndExportTime = GetTickCount64(); }
    ULONGLONG   StartExportTime;
    ULONGLONG   StartSceneParseTime;
    ULONGLONG   StartSaveTime;
    ULONGLONG   EndExportTime;
    size_t      TrisExported;
    size_t      VertsExported;
    size_t      MaterialsExported;
    size_t      VertexBytesExported;
    size_t      IndexBytesExported;
    size_t      MeshesExported;
    size_t      SubDMeshesProcessed;
    size_t      SubDQuadsProcessed;
    size_t      SubDTrisProcessed;
    void FinalReport();
};

class ExportInformation
{
public:
    ExportString        ExporterName;
    ExportString        DCCNameAndVersion;
    ExportString        UserName;
    ExportString        MachineName;
    ExportString        PlatformName;
    __time64_t          ExportTime;
};

class IDCCTransformer
{
public:
    virtual void TransformMatrix( DirectX::XMFLOAT4X4* pDestMatrix, const DirectX::XMFLOAT4X4* pSrcMatrix ) const = 0;
    virtual void TransformPosition( DirectX::XMFLOAT3* pDestPosition, const DirectX::XMFLOAT3* pSrcPosition ) const = 0;
    virtual void TransformDirection( DirectX::XMFLOAT3* pDestDirection, const DirectX:: XMFLOAT3* pSrcDirection ) const = 0;
    virtual float TransformLength( float fInputLength ) const = 0;
};

class ExportScene :
    public ExportFrame
{
public:
    ExportScene();
    virtual ~ExportScene();

    ExportStatistics& Statistics() { return m_Statistics; }
    ExportCoreSettings& Settings() { return g_ExportCoreSettings; }
    ExportInformation& Information() { return m_Information; }

    bool AddMaterial( ExportMaterial* pMaterial );
    bool AddMesh( ExportMeshBase* pMesh );
    bool AddAnimation( ExportAnimation* pAnimation );

    ExportMaterial* FindMaterial( ExportString name );
    ExportMeshBase* FindMesh( ExportString name );
    ExportAnimation* FindAnimation( ExportString name );
    ExportMaterial* FindMaterial( void* pDCCObject );
    ExportMeshBase* FindMesh( void* pDCCObject );
    ExportAnimation* FindAnimation( void* pDCCObject );

    size_t GetMaterialCount() const { return m_vMaterials.size(); }
    size_t GetMeshCount() const { return m_vMeshes.size(); }
    size_t GetAnimationCount() const { return m_vAnimations.size(); }

    ExportMaterial* GetMaterial( size_t uIndex ) { return m_vMaterials[ uIndex ]; }
    ExportMeshBase* GetMesh( size_t uIndex ) { return m_vMeshes[ uIndex ]; }
    ExportAnimation* GetAnimation( size_t uIndex ) { return m_vAnimations[ uIndex ]; }

    IDCCTransformer* GetDCCTransformer() { return m_pDCCTransformer; }
    void SetDCCTransformer( IDCCTransformer* pDCCTransformer ) { m_pDCCTransformer = pDCCTransformer; }

protected:    
    ExportMaterialList          m_vMaterials;
    ExportMeshBaseList          m_vMeshes;
    ExportAnimationList         m_vAnimations;
    ExportStatistics            m_Statistics;
    ExportInformation           m_Information;
    IDCCTransformer*            m_pDCCTransformer;
};

};

