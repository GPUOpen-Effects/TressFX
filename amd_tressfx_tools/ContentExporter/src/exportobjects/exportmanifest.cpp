//-------------------------------------------------------------------------------------
// ExportManifest.cpp
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
#include "ExportManifest.h"
#include "ExportObjects.h"

#include "DirectXTex.h"

extern ExportPath g_CurrentInputFileName;
extern ExportPath g_CurrentOutputFileName;

ExportPath g_TextureSubPath;
bool g_bIntermediateDDSFormat = true;
extern ATG::ExportScene* g_pScene;

using namespace DirectX;

namespace ATG
{
    size_t ExportManifest::AddFile( ExportString strSourceFileName, ExportString strIntermediateFileName, ExportFileType FileType )
    {
        ExportFileRecord Record;
        Record.strSourceFileName = strSourceFileName;
        Record.strIntermediateFileName = strIntermediateFileName;
        Record.FileType = FileType;
        return AddFile( Record );
    }

    size_t ExportManifest::AddFile( const ExportFileRecord& File )
    {
        size_t dwIndex = FindFile( File.strIntermediateFileName );
        if( dwIndex != (size_t)-1 )
            return dwIndex;
        dwIndex = m_Files.size();
        m_Files.push_back( File );
        return dwIndex;
    }

    size_t ExportManifest::FindFile( ExportString strIntermediateFileName ) const
    {
        for( size_t i = 0; i < m_Files.size(); i++ )
        {
            if( m_Files[i].strIntermediateFileName == strIntermediateFileName )
                return i;
        }
        return (size_t)-1;
    }

    void ExportManifest::ClearFilesOfType( ExportFileType FileType )
    {
        ExportFileRecordVector NewFileList;
        for( size_t i = 0; i < m_Files.size(); i++ )
        {
            if( m_Files[i].FileType != FileType )
                NewFileList.push_back( m_Files[i] );
        }
        m_Files.clear();
        m_Files = NewFileList;
    }

    void ExportTextureConverter::ProcessScene( ExportScene* pScene, ExportManifest* pManifest, const ExportPath& TextureSubPath, bool bIntermediateDDSFormat )
    {
        g_TextureSubPath = TextureSubPath;
        g_bIntermediateDDSFormat = bIntermediateDDSFormat;

        size_t dwMaterialCount = pScene->GetMaterialCount();
        for( size_t i = 0; i < dwMaterialCount; ++i )
        {
            ProcessMaterial( pScene->GetMaterial( i ), pManifest );
        }
    }

    void ExportTextureConverter::ProcessMaterial( ExportMaterial* pMaterial, ExportManifest* pManifest )
    {
        MaterialParameterList* pParameters = pMaterial->GetParameterList();
        MaterialParameterList::iterator iter = pParameters->begin();
        MaterialParameterList::iterator end = pParameters->end();
        while( iter != end )
        {
            ExportMaterialParameter* pParameter = &(*iter);
            if( pMaterial->IsTransparent() && pParameter->Flags & ExportMaterialParameter::EMPF_DIFFUSEMAP )
            {
                pParameter->Flags |= ExportMaterialParameter::EMPF_ALPHACHANNEL;
            }
            ProcessTextureParameter( pParameter, pManifest );
            ++iter;
        }
    }

    bool ExportManifest::FileExists( const ExportPath& Path )
    {
        HANDLE hFile = CreateFile( (const CHAR*)Path, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr );
        if( hFile != INVALID_HANDLE_VALUE )
        {
            CloseHandle( hFile );
            return true;
        }
        return false;
    }

    void ExportTextureConverter::ProcessTextureParameter( ExportMaterialParameter* pParameter, ExportManifest* pManifest )
    {
        switch( pParameter->ParamType )
        {
        case MPT_TEXTURE2D:
            break;
        case MPT_TEXTURECUBE:
            break;
        case MPT_TEXTUREVOLUME:
            break;
        default:
            return;
        }

        // Skip processing if it's the default texture.
        if( pParameter->ValueString == ExportMaterial::GetDefaultDiffuseMapTextureName() )
            return;
        if( pParameter->ValueString == ExportMaterial::GetDefaultNormalMapTextureName() )
            return;

        if ( !*pParameter->ValueString.SafeString() )
            return;

        // Look for our texture in the location specified by the content file.
        ExportPath SourceFileName( pParameter->ValueString.SafeString() );
        if( !ExportManifest::FileExists( SourceFileName ) )
        {
            ExportPath DifferentSourceFileName( g_CurrentInputFileName );
            DifferentSourceFileName.ChangeFileNameWithExtension( SourceFileName );

            if( !ExportManifest::FileExists( DifferentSourceFileName ) )
            {
                ExportLog::LogWarning( "Source texture file \"%s\" could not be found.", (const CHAR*)SourceFileName );
            }
            else
            {
                SourceFileName = DifferentSourceFileName;
            }
        }

        // At this point we have a valid file in SourceFileName.
        // Next we build an intermediate file name, which is the content file copied to a location parallel to the output scene file.
        ExportPath IntermediateFileName( g_CurrentOutputFileName );
        IntermediateFileName.Append( g_TextureSubPath );
        IntermediateFileName.ChangeFileNameWithExtension( SourceFileName );

        // IntermediateFileName is now complete, but we may need to perform a texture format conversion.
        const CHAR* strSourceExtension = IntermediateFileName.GetExtension();

        // Check if the texture is already in the desired file format; otherwise, we will need to convert.
        ExportTextureOperation TexOperation = ETO_NOTHING;
        const CHAR* strDesiredTextureFileFormat = "dds";
        if( !g_bIntermediateDDSFormat )
        {
            strDesiredTextureFileFormat = "tga";
        }
        if( _stricmp( strSourceExtension, strDesiredTextureFileFormat ) != 0 )
        {
            TexOperation = ETO_CONVERTFORMAT;
            IntermediateFileName.ChangeExtension( strDesiredTextureFileFormat );
            size_t dwFoundFile = pManifest->FindFile( (const CHAR*)IntermediateFileName );
            if( dwFoundFile != (size_t)-1 )
            {
                const ExportFileRecord& fr = pManifest->GetFile( dwFoundFile );
                if( fr.strSourceFileName != ExportString( (const CHAR*)SourceFileName ) )
                {
                    // File collision detected.
                    // For example, texture.jpg and texture.bmp are both being converted to texture.dds.
                    // But they are different textures.
                    // Therefore, give a unique label to this conversion, to avoid the name collision.
                    CHAR strUniqueLabel[10];
                    sprintf_s( strUniqueLabel, "_%s", strSourceExtension );
                    IntermediateFileName.AppendToFileName( strUniqueLabel );
                }
            }
        }

        // If the file requires processing, that will change the name of the intermediate
        // file and possibly the extension too.
        if( pParameter->Flags & ExportMaterialParameter::EMPF_BUMPMAP )
        {
            IntermediateFileName.AppendToFileName( "_normal" );
            TexOperation = ETO_BUMPMAP_TO_NORMALMAP;
        }

        // IntermediateFileName is now ready.
        // Next we build the resource file name, which is the filename used by the title to load this texture.
        // We will use the intermediate file name without its path.

        ExportPath ResourceFileName( IntermediateFileName.GetFileName() );
        pParameter->ValueString = (const CHAR*)ResourceFileName;

        // Determine the proper texture compression format.
        DXGI_FORMAT CompressedTextureFormat = DXGI_FORMAT_BC1_UNORM;
        if( pParameter->Flags & ExportMaterialParameter::EMPF_ALPHACHANNEL )
        {
            CompressedTextureFormat = DXGI_FORMAT_BC3_UNORM;
        }
        if( !g_pScene->Settings().bTextureCompression )
        {
            CompressedTextureFormat = g_pScene->Settings().bBGRvsRGB ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;
        }

        // Build the export file record for the manifest.
        ExportFileRecord fr;
        fr.strSourceFileName = SourceFileName;
        fr.strIntermediateFileName = IntermediateFileName;
        fr.strResourceName = ResourceFileName;
        fr.TextureOperation = TexOperation;
        fr.CompressedTextureFormat = CompressedTextureFormat;
        ExportPath DevKitFileName( g_TextureSubPath );
        DevKitFileName.ChangeFileNameWithExtension( ResourceFileName );
        fr.strDevKitFileName = DevKitFileName;

        switch( pParameter->ParamType )
        {
        case MPT_TEXTURE2D:
            fr.FileType = EFT_TEXTURE2D;
            break;
        case MPT_TEXTURECUBE:
            fr.FileType = EFT_TEXTURECUBE;
            break;
        case MPT_TEXTUREVOLUME:
            fr.FileType = EFT_TEXTUREVOLUME;
            break;
        }

        pManifest->AddFile( fr );
    }

    void ConvertImageFormat( const CHAR* strSourceFileName, const CHAR* strDestFileName, DXGI_FORMAT CompressedFormat, bool bNormalMap )
    {
        bool iscompressed = IsCompressed( CompressedFormat ) && g_bIntermediateDDSFormat;

        if ( bNormalMap )
        {
            ExportLog::LogMsg( 4, "Converting bump map file \"%s\" to normal map file %s.", strSourceFileName, strDestFileName );
        }
        else if( iscompressed )
        {
            ExportLog::LogMsg( 4, "Compressing and converting file \"%s\" to file \"%s\".", strSourceFileName, strDestFileName );
        }
        else
        {
            ExportLog::LogMsg( 4, "Converting file \"%s\" to file \"%s\".", strSourceFileName, strDestFileName );
        }

        // Load texture image
        WCHAR wSource[MAX_PATH];
        if ( !MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, strSourceFileName, -1, wSource, MAX_PATH) )
            *wSource = 0;

        WCHAR wDest[MAX_PATH];
        if ( !MultiByteToWideChar( CP_ACP, MB_PRECOMPOSED, strDestFileName, -1, wDest, MAX_PATH ) )
            *wDest = 0;

        char ext[_MAX_EXT];
        _splitpath_s( strSourceFileName, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT );

        TexMetadata info;
        std::unique_ptr<ScratchImage> image( new ScratchImage );

        bool isdds = false;
        if ( _stricmp( ext, ".dds" ) == 0 )
        {
            isdds = true;
            HRESULT hr = LoadFromDDSFile( wSource, DDS_FLAGS_NONE, &info, *image );
            if ( FAILED(hr) )
            {
                ExportLog::LogError( "Could not load texture \"%s\" (DDS: %08X).", strSourceFileName, hr );
                return;
            }
        }
        else if ( _stricmp( ext, ".tga" ) == 0 )
        {
            HRESULT hr = LoadFromTGAFile( wSource, &info, *image );
            if ( FAILED(hr) )
            {
                ExportLog::LogError( "Could not load texture \"%s\" (TGA: %08X).", strSourceFileName, hr );
                return;
            }
        }
        else
        {
            HRESULT hr = LoadFromWICFile( wSource, TEX_FILTER_DEFAULT, &info, *image );
            if ( FAILED(hr) )
            {
                ExportLog::LogError( "Could not load texture \"%s\" (WIC: %08X).", strSourceFileName, hr );
                return;
            }
        }

        switch( g_pScene->Settings().dwFeatureLevel )
        {
        default: // 11.0 or greater
            if ( info.width > 16384 || info.height > 16384 )
            {
                ExportLog::LogWarning( "Texture size (%Iu,%Iu) too large for feature level 11.0 or later (16384) \"%s\".", info.width, info.height, strSourceFileName );
            }
            break;

        case D3D_FEATURE_LEVEL_10_1:
        case D3D_FEATURE_LEVEL_10_0:
            if ( info.width > 8192 || info.height > 8192 )
            {
                ExportLog::LogWarning( "Texture size (%Iu,%Iu) too large for feature level 10.0 or 10.1 (8192) \"%s\".", info.width, info.height, strSourceFileName );
            }
            break;

        case D3D_FEATURE_LEVEL_9_3:
            if ( info.width > 4096 || info.height > 4096 )
            {
                ExportLog::LogWarning( "Texture size (%Iu,%Iu) too large for feature level 9.3 (4096) \"%s\".", info.width, info.height, strSourceFileName );
            }
            break;

        case D3D_FEATURE_LEVEL_9_2:
        case D3D_FEATURE_LEVEL_9_1:
            if ( info.width > 2048 || info.height > 2048 )
            {
                ExportLog::LogWarning( "Texture size (%Iu,%Iu) too large for feature level 9.1 or 9.2 (2048) \"%s\".", info.width, info.height, strSourceFileName );
            }
            break;
        }

        // Handle normal maps
        DXGI_FORMAT tformat = g_pScene->Settings().bBGRvsRGB ? DXGI_FORMAT_B8G8R8A8_UNORM : DXGI_FORMAT_R8G8B8A8_UNORM;

        if ( bNormalMap )
        {
            std::unique_ptr<ScratchImage> timage( new ScratchImage );

            HRESULT hr = ComputeNormalMap( image->GetImages(), image->GetImageCount(), image->GetMetadata(),
                                           CNMAP_CHANNEL_LUMINANCE, 10.f, tformat, *timage );
            if ( FAILED(hr) )
            {
                ExportLog::LogError( "Could not compute normal map for \"%s\" (%08X).", strSourceFileName, hr );
            }
            else
            {
                image.swap( timage );
                info.format = tformat;
            }
        }
        // Handle conversions
        else if ( !isdds
                  && !iscompressed
                  && info.format != tformat )
        {
            std::unique_ptr<ScratchImage> timage( new ScratchImage );

            HRESULT hr = Convert( image->GetImages(), image->GetImageCount(), image->GetMetadata(),
                                  tformat, TEX_FILTER_DEFAULT, 0.5f, *timage );
            if ( FAILED(hr) )
            {
                ExportLog::LogError( "Could not convert \"%s\" (%08X).", strSourceFileName, hr );
            }
            else
            {
                image.swap( timage );
                info.format = tformat;
            }
        }

        // Handle mipmaps
        if( g_pScene->Settings().bGenerateTextureMipMaps
            && ( info.mipLevels == 1 )
            && ( !IsCompressed( info.format ) ) )
        {
            std::unique_ptr<ScratchImage> timage( new ScratchImage );

            HRESULT hr = GenerateMipMaps( image->GetImages(), image->GetImageCount(), image->GetMetadata(), TEX_FILTER_DEFAULT, 0, *timage );
            if ( FAILED(hr) )
            {
                ExportLog::LogError( "Failing generating mimaps for \"%s\" (WIC: %08X).", strSourceFileName, hr );
            }
            else
            {
                image.swap( timage );
            }
        }

        // Handle compression
        if ( iscompressed
             && info.format != CompressedFormat )
        {
            if ( (info.width % 4) != 0 || (info.height % 4) != 0 )
            {
                ExportLog::LogWarning( "Texture size (%Iux%Iu) not a multiple of 4 \"%s\", so skipping compress", info.width, info.height, strSourceFileName );
            }
            else
            {
                std::unique_ptr<ScratchImage> timage( new ScratchImage );

                HRESULT hr = Compress( image->GetImages(), image->GetImageCount(), image->GetMetadata(), CompressedFormat, TEX_COMPRESS_DEFAULT, 0.5f, *timage );
                if ( FAILED(hr) )
                {
                    ExportLog::LogError( "Failing compressing \"%s\" (WIC: %08X).", strSourceFileName, hr );
                }
                else
                {
                    image.swap( timage );
                }
            }
        }

        // Save DDS
        _splitpath_s( strDestFileName, nullptr, 0, nullptr, 0, nullptr, 0, ext, _MAX_EXT );

        HRESULT hr;
        if ( _stricmp( ext, ".tga" ) == 0 )
        {
            auto img = image->GetImage( 0, 0, 0 );
            if ( img )
                hr = SaveToTGAFile( *img, wDest );
            else
                hr = E_FAIL;
        }
        else
        {
            hr = SaveToDDSFile( image->GetImages(), image->GetImageCount(), image->GetMetadata(), DDS_FLAGS_NONE, wDest );
        }

        if( FAILED( hr ) )
        {
            ExportLog::LogError( "Could not write texture to file \"%s\" (%08X).", strDestFileName, hr );
        }
    }

    void ExportTextureConverter::PerformTextureFileOperations( ExportManifest* pManifest )
    {
        if( g_pScene->Settings().bForceTextureOverwrite )
        {
            ExportLog::LogMsg( 4, "Reprocessing and overwriting all destination textures." );
        }

        for( size_t i = 0; i < pManifest->GetFileCount(); i++ )
        {
            ExportFileRecord& File = pManifest->GetFile( i );
            if( File.FileType != EFT_TEXTURE2D &&
                File.FileType != EFT_TEXTURECUBE &&
                File.FileType != EFT_TEXTUREVOLUME )
                continue;

            if( File.strSourceFileName == File.strIntermediateFileName )
                continue;

            if( ExportManifest::FileExists( File.strIntermediateFileName.SafeString() ) && !g_pScene->Settings().bForceTextureOverwrite )
            {
                ExportLog::LogMsg( 4, "Destination texture file \"%s\" already exists.", File.strIntermediateFileName.SafeString() );
                continue;
            }

            switch( File.TextureOperation )
            {
            case ETO_NOTHING:
                // Copy file to intermediate location.
                ExportLog::LogMsg( 4, "Copying texture \"%s\" to \"%s\"...", File.strSourceFileName.SafeString(), File.strIntermediateFileName.SafeString() );
                CopyFile( File.strSourceFileName, File.strIntermediateFileName, false );
                ExportLog::LogMsg( 4, "Texture copy complete." );
                break;
            case ETO_CONVERTFORMAT:
                // Convert source file to intermediate location.
                ConvertImageFormat( File.strSourceFileName, File.strIntermediateFileName, File.CompressedTextureFormat, false );
                break;
            case ETO_BUMPMAP_TO_NORMALMAP:
                // Convert source file to a normal map, copy to intermediate file location.
                ConvertImageFormat( File.strSourceFileName, File.strIntermediateFileName, File.CompressedTextureFormat, true );
                break;
            }
        }
    }
}

