//-------------------------------------------------------------------------------------
// ExportManifest.h
//
// Data structures to track a list of files that are important to the export process.
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

namespace ATG
{
    enum ExportFileType
    {
        EFT_SCENEFILE_XML,
        EFT_SCENEFILE_BINARY,
        EFT_BINARY_RESOURCE,
        EFT_BUNDLED_RESOURCE,
        EFT_TEXTURE2D,
        EFT_TEXTURECUBE,
        EFT_TEXTUREVOLUME,
        EFT_SHADER,
    };

    enum ExportTextureOperation
    {
        ETO_NOTHING = 0,
        ETO_CONVERTFORMAT,
        ETO_BUMPMAP_TO_NORMALMAP
    };

    struct ExportFileRecord
    {
        ExportString            strSourceFileName;
        ExportString            strIntermediateFileName;
        ExportString            strResourceName;
        ExportString            strDevKitFileName;
        ExportFileType          FileType;
        DXGI_FORMAT             CompressedTextureFormat;
        ExportTextureOperation  TextureOperation;
    };
    typedef std::vector<ExportFileRecord> ExportFileRecordVector;

    class ExportManifest
    {
    public:
        void Clear() { m_Files.clear(); }
        void ClearFilesOfType( ExportFileType FileType );
        size_t AddFile( const ExportFileRecord& File );
        size_t AddFile( ExportString strSourceFileName, ExportString strIntermediateFileName, ExportFileType FileType = EFT_TEXTURE2D );
        size_t GetFileCount() const { return m_Files.size(); }
        ExportFileRecord& GetFile( size_t dwIndex ) { return m_Files[dwIndex]; }
        size_t FindFile( ExportString strFileName ) const;

        static bool FileExists( const ExportPath& Path );

    protected:
        ExportFileRecordVector  m_Files;
    };

    class ExportScene;
    class ExportMaterial;
    class ExportMaterialParameter;

    class ExportTextureConverter
    {
    public:
        static void ProcessScene( ExportScene* pScene, ExportManifest* pManifest, const ExportPath& TextureSubPath, bool bIntermediateDDSFormat );
        static void PerformTextureFileOperations( ExportManifest* pManifest );

    protected:
        static void ProcessMaterial( ExportMaterial* pMaterial, ExportManifest* pManifest );
        static void ProcessTextureParameter( ExportMaterialParameter* pParameter, ExportManifest* pManifest );
    };
}