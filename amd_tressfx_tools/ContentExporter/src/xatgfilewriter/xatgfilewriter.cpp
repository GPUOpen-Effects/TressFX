//-------------------------------------------------------------------------------------
// XATGFileWriter.cpp
//
// Writes files in the XATG file format, from data stored in the export objects hierarchy.
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
#include "XATGFileWriter.h"
#include "xmlwriter.h"

extern ATG::ExportScene*     g_pScene;
extern ExportPath g_CurrentInputFileName;

using namespace DirectX;
using namespace DirectX::PackedVector;

namespace ATG
{

static const CHAR* g_strTextureOutputSubPath = "\\textures";
static const CHAR* g_strSceneOutputSubPath = "\\scenes";

XATGExportSettings      g_XATGSettings;

XMLWriter*              g_pXMLWriter = nullptr;
ExportManifest*         g_pManifest = nullptr;
CHAR                    g_strFileName[MAX_PATH];
CHAR                    g_strOutputFileBasePath[MAX_PATH];
CHAR                    g_strOutputFileScenePath[MAX_PATH];

INT                     g_iWorkSize = 0;
INT                     g_iCompletedWork = 0;

HANDLE                  g_hBinaryBlobFile = INVALID_HANDLE_VALUE;
ExportManifest          g_DefaultManifest;
ExportFileRecord        g_TextureBundledFile;

void WriteAnimations();
void WriteMaterials();
void WriteMeshes();
void WriteFrame( ExportFrame* pFrame );
void WriteInformation();
void PrepareDestination();
void PrepareBinaryBlob();
void PrepareBundledFile();
DWORD GetBinaryBlobCurrentOffset();
void WriteBinaryBlobData( BYTE* pData, size_t dwDataSizeBytes );

void XATGInitializeSettings()
{
    ExportSettingsEntry* pCategoryXATG = g_SettingsManager.AddRootCategory( "XATG File Export" );
    g_SettingsManager.AddBool( pCategoryXATG, "Bundle Textures", "bundletextures", true, &g_XATGSettings.bBundleTextures );
    g_SettingsManager.AddBool( pCategoryXATG, "Use Existing Bundle", "useexistingbundle", false, &g_XATGSettings.bUseExistingBundle );
    g_SettingsManager.AddBool( pCategoryXATG, "Write .pmem Fast Load File", "packmeshdata", true, &g_XATGSettings.bBinaryBlobExport );
    pCategoryXATG->SetDefaultValue( true, false );
}

void UpdateProgress()
{
    if( g_iWorkSize <= 0 )
        return;

    g_pProgress->SetProgress( (float)g_iCompletedWork / (float)g_iWorkSize );
}

void ConvertSlashes( CHAR* strString )
{
    CHAR* pChar = strString;
    while( *pChar != '\0' )
    {
        if( *pChar == '/' )
            *pChar = '\\';
        ++pChar;
    }
}

bool WriteXATGFile( const CHAR* strFileName, ExportManifest* pManifest )
{
    if( !g_pScene || !strFileName )
        return false;

    if( g_pXMLWriter )
        return false;

    g_pManifest = pManifest;
    if( !g_pManifest )
    {
        g_pManifest = &g_DefaultManifest;
    }

    strcpy_s( g_strFileName, strFileName );
    ConvertSlashes( g_strFileName );

    PrepareDestination();

    bool retValue = true;

    g_iWorkSize = 0;
    g_iCompletedWork = 0;
    if( g_pScene->Settings().bExportScene )
        g_iWorkSize += 50;
    if( g_pScene->Settings().bExportMeshes )
        g_iWorkSize += static_cast<INT>( g_pScene->GetMeshCount() * 10 );
    if( g_pScene->Settings().bExportMaterials )
        g_iWorkSize += static_cast<INT>( g_pScene->GetMaterialCount() * 3 );

    g_pXMLWriter = new XMLWriter( g_strOutputFileScenePath );
    if( !g_pXMLWriter->IsValid() )
    {
        ExportLog::LogError( "Could not write to destination file \"%s\".  Verify that the path is valid and the file is not marked read-only.", g_strOutputFileScenePath );
        return false;
    }

    g_pXMLWriter->StartElement( "XFileATG" );
    g_pXMLWriter->AddAttribute( "Version", "1.7" );

    WriteInformation();
    PrepareBinaryBlob();
    PrepareBundledFile();

    if( g_pScene->Settings().bExportMeshes )
    {
        g_pProgress->SetCaption( "Writing Meshes..." );
        WriteMeshes();
    }

    if( g_pScene->Settings().bExportMaterials )
    {
        g_pProgress->SetCaption( "Writing Materials..." );
        WriteMaterials();
    }

    if( g_pScene->Settings().bExportAnimations )
    {
        g_pProgress->SetCaption( "Writing Animations..." );
        WriteAnimations();
    }

    if( g_pScene->Settings().bExportScene && g_pScene->GetChildCount() > 0 )
    {
        g_pProgress->SetCaption( "Writing Scene..." );
        ExportFrame* pRootFrame = g_pScene->GetChildByIndex( 0 );
        if( pRootFrame )
            WriteFrame( pRootFrame );
        g_iCompletedWork += 50;
        UpdateProgress();
    }

    g_pXMLWriter->EndElement();

    g_pXMLWriter->Close();
    delete g_pXMLWriter;
    g_pXMLWriter = nullptr;

    if( g_hBinaryBlobFile != INVALID_HANDLE_VALUE )
    {
        CloseHandle( g_hBinaryBlobFile );
        g_hBinaryBlobFile = INVALID_HANDLE_VALUE;
    }
    g_strFileName[0] = '\0';

    return retValue;
}

CHAR g_strBuffer[512];
const CHAR* WriteMatrix( const XMFLOAT4X4& m )
{
    sprintf_s( g_strBuffer, "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f, %f",
        m._11, m._12, m._13, m._14,
        m._21, m._22, m._23, m._24,
        m._31, m._32, m._33, m._34,
        m._41, m._42, m._43, m._44 );
    return g_strBuffer;
}

const CHAR* WriteVec3( const XMFLOAT3& vec3 )
{
    sprintf_s( g_strBuffer, "%f, %f, %f", vec3.x, vec3.y, vec3.z );
    return g_strBuffer;
}

const CHAR* WriteColor( const XMFLOAT4& color )
{
    sprintf_s( g_strBuffer, "%f, %f, %f, %f", color.w, color.x, color.y, color.z );
    return g_strBuffer;
}

const CHAR* WriteQuaternion( const XMFLOAT4& quat )
{
    sprintf_s( g_strBuffer, "%f, %f, %f, %f", quat.x, quat.y, quat.z, quat.w );
    return g_strBuffer;
}

void WriteAnimations()
{
    for( size_t i = 0; i < g_pScene->GetAnimationCount(); i++ )
    {
        ExportAnimation* pAnim = g_pScene->GetAnimation( i );
        size_t dwTrackCount = pAnim->GetTrackCount();
        if( dwTrackCount == 0 )
            continue;
        g_pXMLWriter->StartElement( "Animation" );
        g_pXMLWriter->AddAttribute( "Name", pAnim->GetName() );
        g_pXMLWriter->AddAttribute( "Duration", pAnim->fEndTime - pAnim->fStartTime );
        for( size_t dwTrack = 0; dwTrack < dwTrackCount; dwTrack++ )
        {
            ExportAnimationTrack* pTrack = pAnim->GetTrack( dwTrack );
            g_pXMLWriter->StartElement( "AnimationTrack" );
            g_pXMLWriter->AddAttribute( "Name", pTrack->GetName() );
            
            {
                size_t dwKeyCount = pTrack->TransformTrack.PositionKeys.size();
                g_pXMLWriter->StartElement( "PositionKeys" );
                g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwKeyCount ) );
                for( size_t dwKey = 0; dwKey < dwKeyCount; dwKey++ )
                {
                    ExportAnimationPositionKey& Key = pTrack->TransformTrack.PositionKeys[dwKey];
                    g_pXMLWriter->StartElement( "PositionKey" );
                    g_pXMLWriter->AddAttribute( "Time", Key.fTime );
                    g_pXMLWriter->AddAttribute( "Position", WriteVec3( Key.Position ) );
                    g_pXMLWriter->EndElement();
                }
                g_pXMLWriter->EndElement();
            }

            {
                size_t dwKeyCount = pTrack->TransformTrack.OrientationKeys.size();
                g_pXMLWriter->StartElement( "OrientationKeys" );
                g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwKeyCount ) );
                for( size_t dwKey = 0; dwKey < dwKeyCount; dwKey++ )
                {
                    ExportAnimationOrientationKey& Key = pTrack->TransformTrack.OrientationKeys[dwKey];
                    g_pXMLWriter->StartElement( "OrientationKey" );
                    g_pXMLWriter->AddAttribute( "Time", Key.fTime );
                    g_pXMLWriter->AddAttribute( "Orientation", WriteQuaternion( Key.Orientation ) );
                    g_pXMLWriter->EndElement();
                }
                g_pXMLWriter->EndElement();
            }

            {
                size_t dwKeyCount = pTrack->TransformTrack.ScaleKeys.size();
                g_pXMLWriter->StartElement( "ScaleKeys" );
                g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwKeyCount ) );
                for( size_t dwKey = 0; dwKey < dwKeyCount; dwKey++ )
                {
                    ExportAnimationScaleKey& Key = pTrack->TransformTrack.ScaleKeys[dwKey];
                    g_pXMLWriter->StartElement( "ScaleKey" );
                    g_pXMLWriter->AddAttribute( "Time", Key.fTime );
                    g_pXMLWriter->AddAttribute( "Scale", WriteVec3( Key.Scale ) );
                    g_pXMLWriter->EndElement();
                }
                g_pXMLWriter->EndElement();
            }

            g_pXMLWriter->StartElement( "AnnotationTrack" );
            g_pXMLWriter->EndElement();

            g_pXMLWriter->EndElement();
        }
        g_pXMLWriter->EndElement();
    }
}

void WriteMaterialParameterString( const CHAR* strName, const CHAR* strValue, const CHAR* strTypeHint, bool bInstanceParam, const CHAR* strHint )
{
    g_pXMLWriter->StartElement( "ParamString" );
    g_pXMLWriter->AddAttribute( "Name", strName );
    if( strTypeHint )
    {
        g_pXMLWriter->AddAttribute( "Type", strTypeHint );
    }
    if( bInstanceParam )
        g_pXMLWriter->AddAttribute( "InstanceParam", "TRUE" );
    if( strHint && strlen( strHint ) > 0 )
        g_pXMLWriter->AddAttribute( "Hint", strHint );
    g_pXMLWriter->WriteElement( "Value", strValue );
    g_pXMLWriter->EndElement();
}

void WriteMaterialParameterFloatArray( const CHAR* strName, const float* pValues, size_t dwCount, bool bInstanceParam, const CHAR* strHint )
{
    g_pXMLWriter->StartElement( "ParamFloat" );
    g_pXMLWriter->AddAttribute( "Name", strName );
    g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwCount ) );
    if( bInstanceParam )
        g_pXMLWriter->AddAttribute( "InstanceParam", "TRUE" );
    if( strHint && strlen( strHint ) > 0 )
        g_pXMLWriter->AddAttribute( "Hint", strHint );
    for( size_t i = 0; i < dwCount; ++i )
    {
        g_pXMLWriter->WriteElement( "Value", pValues[i] );
    }
    g_pXMLWriter->EndElement();
}

void WriteMaterialParameterInteger( const CHAR* strName, const INT iValue, bool bInstanceParam, const CHAR* strHint )
{
    g_pXMLWriter->StartElement( "ParamInt" );
    g_pXMLWriter->AddAttribute( "Name", strName );
    if( bInstanceParam )
        g_pXMLWriter->AddAttribute( "InstanceParam", "TRUE" );
    if( strHint && strlen( strHint ) > 0 )
        g_pXMLWriter->AddAttribute( "Hint", strHint );
    g_pXMLWriter->WriteElement( "Value", iValue );
    g_pXMLWriter->EndElement();
}

void WriteMaterialParameterBool( const CHAR* strName, const bool bValue, bool bInstanceParam, const CHAR* strHint )
{
    g_pXMLWriter->StartElement( "ParamBool" );
    g_pXMLWriter->AddAttribute( "Name", strName );
    if( bInstanceParam )
        g_pXMLWriter->AddAttribute( "InstanceParam", "TRUE" );
    if( strHint && strlen( strHint ) > 0 )
        g_pXMLWriter->AddAttribute( "Hint", strHint );
    if( bValue )
        g_pXMLWriter->WriteElement( "Value", "TRUE" );
    else
        g_pXMLWriter->WriteElement( "Value", "FALSE" );
    g_pXMLWriter->EndElement();
}

void WriteMaterialParameter( const ExportMaterialParameter& ParamDesc )
{
    switch( ParamDesc.ParamType )
    {
    case MPT_TEXTURE2D:
        WriteMaterialParameterString( ParamDesc.Name, ParamDesc.ValueString.SafeString(), "Texture2D", ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_TEXTURECUBE:
        WriteMaterialParameterString( ParamDesc.Name, ParamDesc.ValueString.SafeString(), "TextureCube", ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_TEXTUREVOLUME:
        WriteMaterialParameterString( ParamDesc.Name, ParamDesc.ValueString.SafeString(), "Texture3D", ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_FLOAT:
        WriteMaterialParameterFloatArray( ParamDesc.Name, ParamDesc.ValueFloat, 1, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_FLOAT2:
        WriteMaterialParameterFloatArray( ParamDesc.Name, ParamDesc.ValueFloat, 2, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_FLOAT3:
        WriteMaterialParameterFloatArray( ParamDesc.Name, ParamDesc.ValueFloat, 3, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_FLOAT4:
        WriteMaterialParameterFloatArray( ParamDesc.Name, ParamDesc.ValueFloat, 4, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_BOOL:
        WriteMaterialParameterBool( ParamDesc.Name, ParamDesc.ValueInt ? true : false, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_INTEGER:
        WriteMaterialParameterInteger( ParamDesc.Name, ParamDesc.ValueInt, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    case MPT_STRING:
        WriteMaterialParameterString( ParamDesc.Name, ParamDesc.ValueString, nullptr, ParamDesc.bInstanceParam, ParamDesc.Hint );
        break;
    }
}

void WriteMaterials()
{
    const CHAR* strMaterialDBName = ExportMaterialDatabase::GetDatabaseFileName();
    if( strlen( strMaterialDBName ) != 0 )
    {
        g_pXMLWriter->StartElement( "MaterialDatabase" );
        g_pXMLWriter->AddAttribute( "FileName", strMaterialDBName );
        g_pXMLWriter->AddAttribute( "MaterialCount", static_cast<INT>( ExportMaterialDatabase::GetMaterialCount() ) );
        g_pXMLWriter->EndElement();
    }
    for( size_t i = 0; i < g_pScene->GetMaterialCount(); i++ )
    {
        ExportMaterial* pMaterial = g_pScene->GetMaterial( i );
        g_pXMLWriter->StartElement( "MaterialInstance" );
        g_pXMLWriter->AddAttribute( "Name", pMaterial->GetName().SafeString() );
        if( pMaterial->IsTransparent() )
            g_pXMLWriter->AddAttribute( "Transparent", "TRUE" );
        if( pMaterial->GetParameterCount() > 0 )
        {
            const CHAR* strMaterialName = pMaterial->GetDefaultMaterialName();
            if( pMaterial->GetMaterialDefinition() )
                strMaterialName = pMaterial->GetMaterialDefinition()->strName.SafeString();
            g_pXMLWriter->AddAttribute( "MaterialName", strMaterialName );
            // serialize effect parameters
            const MaterialParameterList* pParameters = pMaterial->GetParameterList();
            MaterialParameterList::const_iterator iter = pParameters->begin();
            MaterialParameterList::const_iterator end = pParameters->end();
            while( iter != end )
            {
                const ExportMaterialParameter& ParamDesc = *iter;
                WriteMaterialParameter( ParamDesc );
                ++iter;
            }
        }
        g_pXMLWriter->EndElement();

        g_iCompletedWork += 3;
        UpdateProgress();
    }
}

void WriteVertexElement( const D3DVERTEXELEMENT9& Element )
{
    static const CHAR* s_strVertexDeclType[] = {
        "FLOAT1",
        "FLOAT2",
        "FLOAT3",
        "FLOAT4",
        "D3DCOLOR",
        "UBYTE4",
        "SHORT2",
        "SHORT4",
        "UBYTE4N",
        "SHORT2N",
        "SHORT4N",
        "USHORT2N",
        "USHORT4N",
        "UDEC3",
        "DEC3N",
        "FLOAT16_2",
        "FLOAT16_4",
        "UNUSED",
        "DEC_11_11_10N",
    };

    static const CHAR* s_strVertexDeclMethod[] = {
        "DEFAULT",
        "PARTIALU",
        "PARTIALV",
        "CROSSUV",
        "UV",
        "LOOKUP",
        "LOOKUPPRESAMPLED"
    };

    static const CHAR* s_strVertexDeclUsage[] = {
        "POSITION",
        "BLENDWEIGHT",
        "BLENDINDICES",
        "NORMAL",
        "PSIZE",
        "TEXCOORD",
        "TANGENT",
        "BINORMAL",
        "TESSFACTOR",
        "POSITIONT",
        "COLOR",
        "FOG",
        "DEPTH",
        "SAMPLE",
    };

    g_pXMLWriter->StartElement( "VertexDecl" );
    g_pXMLWriter->AddAttributeFormat( "Offset", "%d", Element.Offset );
    g_pXMLWriter->AddAttribute( "Type", s_strVertexDeclType[ Element.Type ] );
    if( Element.Method != 0 )
    {
        g_pXMLWriter->AddAttribute( "Method", s_strVertexDeclMethod[ Element.Method ] );
    }
    g_pXMLWriter->AddAttribute( "Usage", s_strVertexDeclUsage[ Element.Usage ] );
    if( Element.UsageIndex != 0 )
    {
        g_pXMLWriter->AddAttributeFormat( "UsageIndex", "%d", Element.UsageIndex );
    }
    g_pXMLWriter->EndElement();
}


XMFLOAT3 CrackCompressedVector( DWORD dwCompressedVector )
{
    XMXDECN4 DecN4( dwCompressedVector );
    XMFLOAT3 Vec3;
    Vec3.x = (float)DecN4.x / 511.0f;
    Vec3.y = (float)DecN4.y / 511.0f;
    Vec3.z = (float)DecN4.z / 511.0f;

    return Vec3;
}


void WriteVertexBufferVerbose( ExportVB* pVB, const D3DVERTEXELEMENT9* pVertexElements, size_t dwVertexElementCount )
{
    size_t uVertexCount = pVB->GetVertexCount();

    for( size_t i = 0; i < uVertexCount; i++ )
    {
        auto pVertex = pVB->GetVertex( i );

        g_pXMLWriter->StartElement( "E" );
        const CHAR* strComma = ", ";
        for( UINT d = 0; d < dwVertexElementCount; d++ )
        {
            if( d == dwVertexElementCount - 1 )
                strComma = "";
            uint8_t* pVertexData = pVertex + pVertexElements[d].Offset;
            switch( pVertexElements[d].Type )
            {
            case D3DDECLTYPE_FLOAT4:
                g_pXMLWriter->WriteStringFormat( "%f, %f, %f, %f%s", 
                    *reinterpret_cast<float*>( pVertexData ),
                    *reinterpret_cast<float*>( pVertexData + 4 ),
                    *reinterpret_cast<float*>( pVertexData + 8 ),
                    *reinterpret_cast<float*>( pVertexData + 12 ),
                    strComma );
                break;
            case D3DDECLTYPE_FLOAT3:
                g_pXMLWriter->WriteStringFormat( "%f, %f, %f%s", 
                    *reinterpret_cast<float*>( pVertexData ),
                    *reinterpret_cast<float*>( pVertexData + 4 ),
                    *reinterpret_cast<float*>( pVertexData + 8 ),
                    strComma );
                break;
            case D3DDECLTYPE_FLOAT2:
                g_pXMLWriter->WriteStringFormat( "%f, %f%s", 
                    *reinterpret_cast<float*>( pVertexData ),
                    *reinterpret_cast<float*>( pVertexData + 4 ),
                    strComma );
                break;
            case D3DDECLTYPE_FLOAT1:
                g_pXMLWriter->WriteStringFormat( "%f%s", *reinterpret_cast<float*>( pVertexData ), strComma );
                break;
            case D3DDECLTYPE_D3DCOLOR:
            case D3DDECLTYPE_UBYTE4:
            case D3DDECLTYPE_UBYTE4N:
                g_pXMLWriter->WriteStringFormat( "0x%08x%s", *reinterpret_cast<DWORD*>( pVertexData ), strComma );
                break;
            case D3DDECLTYPE_FLOAT16_2:
                {
                    float fData[2];
                    XMConvertHalfToFloatStream( fData, sizeof(float), reinterpret_cast<HALF*>(pVertexData), sizeof(HALF), 2 );
                    g_pXMLWriter->WriteStringFormat( "%f, %f%s", fData[0], fData[1], strComma );
                    break;
                }
            case D3DDECLTYPE_FLOAT16_4:
                {
                    float fData[4];
                    XMConvertHalfToFloatStream( fData, sizeof(float), reinterpret_cast<HALF*>(pVertexData), sizeof(HALF), 4 );
                    g_pXMLWriter->WriteStringFormat( "%f, %f, %f, %f%s", 
                        fData[0], fData[1], fData[2], fData[3], strComma );
                    break;
                }
            case D3DDECLTYPE_SHORT4N:
                {
                    auto pWords = reinterpret_cast<short*>( pVertexData );
                    g_pXMLWriter->WriteStringFormat( "%f, %f, %f, %f%s", (float)pWords[0] / 32767.0f, (float)pWords[1] / 32767.0f, (float)pWords[2] / 32767.0f, (float)pWords[3] / 32767.0f, strComma );
                    break;
                }
            default:
                g_pXMLWriter->WriteStringFormat( "UNSUPPORTED_TYPE%s", strComma );
                break;
            }
        }
        g_pXMLWriter->EndElement();
    }

}

void WriteVertexData( ExportVB* pVB, const D3DVERTEXELEMENT9* pVertexElements, size_t dwVertexElementCount )
{
    size_t dwVertexCount = pVB->GetVertexCount();

    if( g_XATGSettings.bBinaryBlobExport )
    {
        DWORD dwBlobLocation = GetBinaryBlobCurrentOffset();
        WriteBinaryBlobData( pVB->GetVertexData(), pVB->GetVertexDataSize() );
        g_pXMLWriter->StartElement( "PhysicalBinaryData" );
        g_pXMLWriter->AddAttribute( "Offset", static_cast<INT>( dwBlobLocation ) );
        g_pXMLWriter->AddAttribute( "Size", static_cast<INT>( pVB->GetVertexDataSize() ) );
        g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwVertexCount ) );
        g_pXMLWriter->EndElement();
        return;
    }

    g_pXMLWriter->StartElement( "Vertices" );
    g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwVertexCount ) );

    WriteVertexBufferVerbose( pVB, pVertexElements, dwVertexElementCount );

    g_pXMLWriter->EndElement();
}

void WriteSphereBound( const BoundingSphere& sphere )
{
    g_pXMLWriter->StartElement( "SphereBound" );
    g_pXMLWriter->AddAttribute( "Center", WriteVec3( sphere.Center ) );
    g_pXMLWriter->AddAttribute( "Radius", sphere.Radius );
    g_pXMLWriter->EndElement();
}

void WriteAxisAlignedBoxBound( const BoundingBox& aabb )
{
    g_pXMLWriter->StartElement( "AxisAlignedBoxBound" );
    g_pXMLWriter->AddAttribute( "Center", WriteVec3( aabb.Center ) );
    g_pXMLWriter->AddAttribute( "Extents", WriteVec3( aabb.Extents ) );
    g_pXMLWriter->EndElement();
}

void WriteOrientedBoxBound( const BoundingOrientedBox& obb )
{
    g_pXMLWriter->StartElement( "OrientedBoxBound" );
    g_pXMLWriter->AddAttribute( "Center", WriteVec3( obb.Center ) );
    g_pXMLWriter->AddAttribute( "Extents", WriteVec3( obb.Extents ) );
    g_pXMLWriter->AddAttribute( "Orientation", WriteQuaternion( obb.Orientation ) );
    g_pXMLWriter->EndElement();
}


void WriteIndexBufferVerbose( ExportIB* pIB )
{
    for( size_t uIndex = 0; uIndex < pIB->GetIndexCount(); uIndex++ )
    {
        g_pXMLWriter->WriteElement( "E", static_cast<INT>( pIB->GetIndex( uIndex ) ) );
    }
}


void WriteIBSubset( ExportIBSubset* pSubset )
{
    const CHAR* strPrimitiveTypes[] =
    {
        "TriangleList",
        "TriangleStrip",
        "QuadList"
    };
    g_pXMLWriter->StartElement( "IBSubset" );
    g_pXMLWriter->AddAttribute( "Name", pSubset->GetName() );
    assert( static_cast<size_t>( pSubset->GetPrimitiveType() ) < ARRAYSIZE( strPrimitiveTypes ) );
    g_pXMLWriter->AddAttribute( "PrimitiveType", strPrimitiveTypes[ pSubset->GetPrimitiveType() ] );
    g_pXMLWriter->AddAttributeFormat( "StartIndex", "%d", pSubset->GetStartIndex() );
    g_pXMLWriter->AddAttributeFormat( "IndexCount", "%d", pSubset->GetIndexCount() );
    g_pXMLWriter->EndElement();
}


void WritePatchSubset( ExportSubDPatchSubset* pSubset )
{
    const CHAR* strPrimitiveTypes[] =
    {
        "TrianglePatchList",
        "QuadPatchList"
    };
    g_pXMLWriter->StartElement( "IBSubset" );
    g_pXMLWriter->AddAttribute( "Name", pSubset->Name );
    g_pXMLWriter->AddAttribute( "PrimitiveType", strPrimitiveTypes[ pSubset->bQuadPatches ] );
    g_pXMLWriter->AddAttributeFormat( "StartIndex", "%d", pSubset->dwStartPatch );
    g_pXMLWriter->AddAttributeFormat( "IndexCount", "%d", pSubset->dwPatchCount );
    g_pXMLWriter->EndElement();
}


void WriteVertexBuffer( ExportVB* pVB, DWORD dwStreamIndex, const D3DVERTEXELEMENT9* pElements, size_t dwElementCount )
{
    g_pXMLWriter->StartElement( "VertexBuffer" );
    g_pXMLWriter->AddAttribute( "Name", "Default" );
    g_pXMLWriter->AddAttribute( "Stream", static_cast<INT>( dwStreamIndex ) );
    g_pXMLWriter->AddAttribute( "Stride", static_cast<INT>( pVB->GetVertexSize() ) );
    {
        g_pXMLWriter->StartElement( "VertexDecls" );
        g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( dwElementCount ) );
        for( size_t i = 0; i < dwElementCount; i++ )
        {
            WriteVertexElement( pElements[i] );
        }
        g_pXMLWriter->EndElement();
    }

    WriteVertexData( pVB, pElements, dwElementCount );
    g_pXMLWriter->EndElement(); // VertexBuffer
}


void WriteIndexBuffer( ExportIB* pIB )
{
    g_pXMLWriter->StartElement( "IndexBuffer" );
    g_pXMLWriter->AddAttribute( "IndexSize", static_cast<INT>( pIB->GetIndexSize() * 8 ) );
    if( g_XATGSettings.bBinaryBlobExport )
    {
        DWORD dwBlobLocation = GetBinaryBlobCurrentOffset();
        WriteBinaryBlobData( pIB->GetIndexData(), pIB->GetIndexDataSize() );
        g_pXMLWriter->StartElement( "PhysicalBinaryData" );
        g_pXMLWriter->AddAttribute( "Offset", static_cast<INT>( dwBlobLocation ) );
        g_pXMLWriter->AddAttribute( "Size", static_cast<INT>( pIB->GetIndexDataSize() ) );
        g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( pIB->GetIndexCount() ) );
        g_pXMLWriter->EndElement();
    }
    else
    {
        g_pXMLWriter->AddAttributeFormat( "IndexCount", "%d", pIB->GetIndexCount() );
        g_pXMLWriter->StartElement( "Indices" );
        WriteIndexBufferVerbose( pIB );
        g_pXMLWriter->EndElement(); // Indices
    }
    g_pXMLWriter->EndElement(); // IndexBuffer
}


void WritePolyMesh( ExportMesh* pMesh )
{
    ExportSubDProcessMesh* pSubDMesh = pMesh->GetSubDMesh();

    if( !g_pScene->Settings().bLittleEndian && g_XATGSettings.bBinaryBlobExport )
    {
        pMesh->ByteSwap();
    }

    g_pXMLWriter->StartElement( "Mesh" );
    g_pXMLWriter->AddAttribute( "Name", pMesh->GetName() );
    if( pMesh->GetInfluenceCount() > 0 )
    {
        g_pXMLWriter->AddAttribute( "Skinned", L"TRUE" );
    }

    g_pXMLWriter->StartElement( "MeshTopology" );
    INT iVBCount = 1;
    if( pSubDMesh )
    {
        iVBCount += 1;
        g_pXMLWriter->AddAttribute( "QuadPatches", L"TRUE" );
    }
    g_pXMLWriter->AddAttribute( "VertexBufferCount", iVBCount );

    if( pMesh->GetInfluenceCount() > 0 )
    {
        g_pXMLWriter->StartElement( "InfluenceObjects" );
        g_pXMLWriter->AddAttribute( "Count", static_cast<INT>( pMesh->GetInfluenceCount() ) );
        for( UINT j = 0; j < pMesh->GetInfluenceCount(); j++ )
        {
            g_pXMLWriter->WriteElement( "E", pMesh->GetInfluence( j ) );
        }
        g_pXMLWriter->EndElement();
    }

    if( !pSubDMesh )
    {
        WriteVertexBuffer( pMesh->GetVB(), 0, &pMesh->GetVertexDeclElement( 0 ), pMesh->GetVertexDeclElementCount() );
        WriteIndexBuffer( pMesh->GetIB() );
        size_t uSubsetCount = pMesh->GetSubsetCount();
        for( size_t i = 0; i < uSubsetCount; i++ )
        {
            ExportIBSubset* pSubset = pMesh->GetSubset( i );
            WriteIBSubset( pSubset );
        }
    }
    else
    {
        WriteVertexBuffer( pMesh->GetVB(), 0, &pMesh->GetVertexDeclElement( 0 ), pMesh->GetVertexDeclElementCount() );
        WriteVertexBuffer( pSubDMesh->GetQuadPatchDataVB(), 1, pSubDMesh->GetPatchDataDecl(), pSubDMesh->GetPatchDataDeclElementCount() );
        WriteIndexBuffer( pSubDMesh->GetQuadPatchIB() );
        size_t uSubsetCount = pSubDMesh->GetSubsetCount();
        for( size_t i = 0; i < uSubsetCount; i++ )
        {
            WritePatchSubset( pSubDMesh->GetSubset( i ) );
        }
        if( pSubDMesh->GetTrianglePatchDataVB() )
        {
            ExportLog::LogWarning( "Subdivision surface mesh \"%s\" contains triangle patches, which are not currently written to XATG files.", pMesh->GetName().SafeString() );
        }
    }

    g_pXMLWriter->EndElement(); // MeshTopology

    g_pXMLWriter->EndElement(); // Mesh
}


void WriteMeshes()
{
    size_t uMeshCount = g_pScene->GetMeshCount();
    for( size_t i = 0; i < uMeshCount; i++ )
    {
        ExportMeshBase* pMeshBase = g_pScene->GetMesh( i );
        switch( pMeshBase->GetMeshType() )
        {
        case ExportMeshBase::PolyMesh:
            WritePolyMesh( reinterpret_cast<ExportMesh*>( pMeshBase ) );
            break;
        }

        g_iCompletedWork += 10;
        UpdateProgress();
    }
}

void WriteModel( ExportModel* pModel )
{
    g_pXMLWriter->StartElement( "Model" );
    g_pXMLWriter->AddAttribute( "Mesh", pModel->GetMesh()->GetName().SafeString() );

    if( !pModel->IsShadowCaster() )
        g_pXMLWriter->AddAttribute( "ShadowCaster", "FALSE" );
    if( !pModel->IsShadowReceiver() )
        g_pXMLWriter->AddAttribute( "ShadowReceiver", "FALSE" );

    switch( pModel->GetMesh()->GetSmallestBound() )
    {
    case ExportMesh::SphereBound:
        WriteSphereBound( pModel->GetMesh()->GetBoundingSphere() );
        break;
    case ExportMesh::AxisAlignedBoxBound:
        WriteAxisAlignedBoxBound( pModel->GetMesh()->GetBoundingAABB() );
        break;
    case ExportMesh::OrientedBoxBound:
        WriteOrientedBoxBound( pModel->GetMesh()->GetBoundingOBB() );
        break;
    }
    for( UINT i = 0; i < pModel->GetBindingCount(); i++ )
    {
        ExportMaterialSubsetBinding* pBinding = pModel->GetBinding( i );
        g_pXMLWriter->StartElement( "SubsetMaterialMapping" );
        g_pXMLWriter->AddAttribute( "SubsetName", pBinding->SubsetName );
        g_pXMLWriter->AddAttribute( "MaterialName", pBinding->pMaterial->GetName().SafeString() );
        g_pXMLWriter->EndElement();
    }
    g_pXMLWriter->EndElement();
}

void WriteLight( ExportLight* pLight )
{
    static const CHAR* s_strFalloffNames[] = {
        "NONE",
        "LINEAR",
        "SQUARED"
    };

    switch( pLight->Type )
    {
    case ExportLight::LT_AMBIENT:
        g_pXMLWriter->StartElement( "AmbientLight" );
        g_pXMLWriter->AddAttribute( "Name", pLight->GetName().SafeString() );
        g_pXMLWriter->AddAttribute( "Color", WriteColor( pLight->Color ) );
        g_pXMLWriter->EndElement();
        break;
    case ExportLight::LT_DIRECTIONAL:
        g_pXMLWriter->StartElement( "DirectionalLight" );
        g_pXMLWriter->AddAttribute( "Name", pLight->GetName().SafeString() );
        g_pXMLWriter->AddAttribute( "Color", WriteColor( pLight->Color ) );
        g_pXMLWriter->AddAttribute( "Direction", WriteVec3( pLight->Direction ) );
        g_pXMLWriter->EndElement();
        break;
    case ExportLight::LT_POINT:
        g_pXMLWriter->StartElement( "PointLight" );
        g_pXMLWriter->AddAttribute( "Name", pLight->GetName().SafeString() );
        g_pXMLWriter->AddAttribute( "Color", WriteColor( pLight->Color ) );
        g_pXMLWriter->AddAttribute( "Position", WriteVec3( pLight->LocalPosition ) );
        g_pXMLWriter->AddAttribute( "Range", pLight->fRange );
        g_pXMLWriter->AddAttribute( "Falloff", s_strFalloffNames[ pLight->Falloff ] );
        g_pXMLWriter->EndElement();
        break;
    case ExportLight::LT_SPOT:
        g_pXMLWriter->StartElement( "SpotLight" );
        g_pXMLWriter->AddAttribute( "Name", pLight->GetName().SafeString() );
        g_pXMLWriter->AddAttribute( "Color", WriteColor( pLight->Color ) );
        g_pXMLWriter->AddAttribute( "Position", WriteVec3( pLight->LocalPosition ) );
        g_pXMLWriter->AddAttribute( "Direction", WriteVec3( pLight->Direction ) );
        g_pXMLWriter->AddAttribute( "Range", pLight->fRange );
        g_pXMLWriter->AddAttribute( "Falloff", s_strFalloffNames[ pLight->Falloff ] );
        g_pXMLWriter->AddAttribute( "SpotlightFalloff", s_strFalloffNames[ pLight->SpotFalloff ] );
        g_pXMLWriter->AddAttribute( "InnerAngle", pLight->fInnerAngle );
        g_pXMLWriter->AddAttribute( "OuterAngle", pLight->fOuterAngle );
        g_pXMLWriter->EndElement();
        break;
    }
}

void WriteCamera( ExportCamera* pCamera )
{
    g_pXMLWriter->StartElement( "PerspectiveCamera" );
    g_pXMLWriter->AddAttribute( "Name", pCamera->GetName() );

    g_pXMLWriter->AddAttribute( "EyePoint", WriteVec3( pCamera->LocalPosition ) );
    g_pXMLWriter->AddAttribute( "LookDirection", WriteVec3( pCamera->Direction ) );
    g_pXMLWriter->AddAttribute( "UpDirection", WriteVec3( pCamera->Up ) );
    g_pXMLWriter->AddAttribute( "FieldOfView", pCamera->fFieldOfView );
    g_pXMLWriter->AddAttribute( "Aspect", 4.0f / 3.0f );
    g_pXMLWriter->AddAttribute( "ZNear", pCamera->fNearClip );
    g_pXMLWriter->AddAttribute( "ZFar", pCamera->fFarClip );

    g_pXMLWriter->AddAttribute( "FocalLength", pCamera->fFocalLength );

    g_pXMLWriter->EndElement();
}

void WriteFrame( ExportFrame* pFrame )
{
    g_pXMLWriter->StartElement( "Frame" );
    g_pXMLWriter->AddAttribute( "Name", pFrame->GetName() );
    g_pXMLWriter->AddAttribute( "Matrix", WriteMatrix( pFrame->Transform().Matrix() ) );

    size_t uModelCount = pFrame->GetModelCount();
    for( size_t i = 0; i < uModelCount; i++ )
    {
        WriteModel( pFrame->GetModelByIndex( i ) );
    }

    if( g_pScene->Settings().bExportLights )
    {
        size_t uLightCount = pFrame->GetLightCount();
        for( size_t i = 0; i < uLightCount; i++ )
        {
            WriteLight( pFrame->GetLightByIndex( i ) );
        }
    }

    if( g_pScene->Settings().bExportCameras )
    {
        size_t uCameraCount = pFrame->GetCameraCount();
        for( size_t i = 0; i < uCameraCount; i++ )
        {
            WriteCamera( pFrame->GetCameraByIndex( i ) );
        }
    }

    size_t uChildCount = pFrame->GetChildCount();
    for( size_t i = 0; i < uChildCount; i++ )
    {
        WriteFrame( pFrame->GetChildByIndex( i ) );
    }

    g_pXMLWriter->EndElement();
}


size_t GetFrameCount( ATG::ExportFrame* pFrame )
{
    if( !pFrame )
        return 0;
    size_t dwCount = 1;
    size_t dwChildCount = pFrame->GetChildCount();
    for( size_t i = 0; i < dwChildCount; ++i )
    {
        dwCount += GetFrameCount( pFrame->GetChildByIndex( i ) );
    }
    return dwCount;
}


void WriteInformation()
{
    g_pXMLWriter->StartComment();
    CHAR strTime[200] = {0};
    _ctime64_s( strTime, &g_pScene->Information().ExportTime );
    CHAR* pLF = strchr( strTime, '\n' );
    if( pLF )
        *pLF = '\0';
    g_pXMLWriter->WriteStringFormat( "Time: %s", strTime );
    g_pXMLWriter->EndComment();

    g_pXMLWriter->StartComment();
    g_pXMLWriter->WriteStringFormat( "Exporter: %s", g_pScene->Information().ExporterName );
    g_pXMLWriter->EndComment();

    g_pXMLWriter->StartComment();
    g_pXMLWriter->WriteStringFormat( "DCC: %s", g_pScene->Information().DCCNameAndVersion );
    g_pXMLWriter->EndComment();

    /*
    g_pXMLWriter->StartComment();
    g_pXMLWriter->WriteStringFormat( "User: %s on machine %s (%s)", 
        g_pScene->Information().UserName,
        g_pScene->Information().MachineName,
        g_pScene->Information().PlatformName );
    g_pXMLWriter->EndComment();
    */

    ULONGLONG dwParseTime = g_pScene->Statistics().StartSaveTime - g_pScene->Statistics().StartExportTime;
    float fParseTime = (float)dwParseTime * 0.001f;
    g_pXMLWriter->StartComment();
    g_pXMLWriter->WriteStringFormat( "Stats: %d vertices, %d triangles, %d materials, %0.3f seconds to parse scene.",
        g_pScene->Statistics().VertsExported,
        g_pScene->Statistics().TrisExported,
        g_pScene->Statistics().MaterialsExported,
        fParseTime );
    g_pXMLWriter->EndComment();

    g_pXMLWriter->StartElement( "LoadHint" );

    if( g_pScene->Settings().bExportMeshes )
    {
        g_pXMLWriter->AddAttribute( "MeshCount", static_cast<INT>( g_pScene->GetMeshCount() ) );
    }

    if( g_pScene->Settings().bExportMaterials )
    {
        g_pXMLWriter->AddAttribute( "MaterialCount", static_cast<INT>( g_pScene->GetMaterialCount() ) );
    }

    if( g_pScene->Settings().bExportAnimations )
    {
        g_pXMLWriter->AddAttribute( "AnimationCount", static_cast<INT>( g_pScene->GetAnimationCount() ) );
    }

    if( g_pScene->Settings().bExportScene && g_pScene->GetChildCount() > 0 )
    {
        g_pXMLWriter->AddAttribute( "FrameCount", static_cast<INT>( GetFrameCount( g_pScene->GetChildByIndex( 0 ) ) ) );
    }

    g_pXMLWriter->EndElement();
}

void PrepareDestination()
{
    // trim the filename off the output XATG file path
    CHAR* pLastSlash = strrchr( g_strFileName, '\\' );
    assert( pLastSlash != nullptr );
    INT iPathSize = static_cast<INT>( pLastSlash - g_strFileName );

    // copy base path (not including filename) into g_strOutputFileBasePath
    assert( iPathSize >= 0 );
    strncpy_s( g_strOutputFileBasePath, g_strFileName, iPathSize );
    g_strOutputFileBasePath[ iPathSize ] = '\0';
    // convert base path to lowercase
    _strlwr_s( g_strOutputFileBasePath );

    bool bCreateSceneDir = true;
    // look for scenes directory at the end of the output file path
    CHAR* strSceneDir = strstr( g_strOutputFileBasePath, g_strSceneOutputSubPath );
    if( strSceneDir && 
        ( strSceneDir - g_strOutputFileBasePath ) == static_cast<INT>( strlen( g_strOutputFileBasePath ) - strlen( g_strSceneOutputSubPath ) ) )
    {
        // we found a trailing scenes directory, chop it off.  this affects g_strOutputFileBasePath
        *strSceneDir = '\0';
        bCreateSceneDir = false;
    }

    // compose scene output file path
    strcpy_s( g_strOutputFileScenePath, g_strOutputFileBasePath );
    strcat_s( g_strOutputFileScenePath, g_strSceneOutputSubPath );
    if( bCreateSceneDir )
    {
        // create scenes directory if one doesn't exist already
        CreateDirectory( g_strOutputFileScenePath, nullptr );
    }
    // add filename back onto g_strOutputFileScenePath
    strcat_s( g_strOutputFileScenePath, pLastSlash );

    ExportLog::LogMsg( 1, "Writing to scene file \"%s\".", g_strOutputFileScenePath );

    size_t dwIndex = g_pManifest->AddFile( g_strOutputFileScenePath, g_strOutputFileScenePath, EFT_SCENEFILE_XML );
    const CHAR* strSubPath = strstr( g_strOutputFileScenePath, g_strSceneOutputSubPath );
    assert( strSubPath != nullptr );
    strSubPath++;
    g_pManifest->GetFile( dwIndex ).strDevKitFileName = strSubPath;

    // compose and create texture directory
    CHAR strTextureOutputPath[MAX_PATH];
    strcpy_s( strTextureOutputPath, g_strOutputFileBasePath );
    strcat_s( strTextureOutputPath, g_strTextureOutputSubPath );
    CreateDirectory( strTextureOutputPath, nullptr );
}

void PrepareBinaryBlob()
{
    // Check if we're exporting the binary blob.
    g_hBinaryBlobFile = INVALID_HANDLE_VALUE;
    if( !g_XATGSettings.bBinaryBlobExport )
        return;

    // Compose the binary blob filename.
    CHAR strBlobFilename[MAX_PATH];
    strcpy_s( strBlobFilename, g_strOutputFileScenePath );
    CHAR* pExt = strstr( strBlobFilename, ".xatg" );
    if ( !pExt )
        return;
    *pExt = '\0';
    strcat_s( strBlobFilename, ".pmem" );

    // Create the file.
    g_hBinaryBlobFile = CreateFile( strBlobFilename, FILE_WRITE_DATA, 0, nullptr, CREATE_ALWAYS, FILE_FLAG_SEQUENTIAL_SCAN, nullptr );
    if( g_hBinaryBlobFile == INVALID_HANDLE_VALUE )
    {
        ExportLog::LogError( "Could not create physical memory file \"%s\".  Verify that the destination file is not read-only.", strBlobFilename );
        g_XATGSettings.bBinaryBlobExport = false;
        return;
    }
    assert( g_hBinaryBlobFile != INVALID_HANDLE_VALUE );

    ExportLog::LogMsg( 4, "Writing to physical memory file \"%s\".", strBlobFilename );

    // Add a reference to the file in the XATG scene file.
    CHAR* pFilename = strrchr( strBlobFilename, '\\' );
    assert( pFilename );
    CHAR strMediaRelativeBlobFilename[MAX_PATH];
    strcpy_s( strMediaRelativeBlobFilename, g_strSceneOutputSubPath + 1 );
    strcat_s( strMediaRelativeBlobFilename, pFilename );
    g_pXMLWriter->StartElement( "PhysicalMemoryFile" );
    g_pXMLWriter->AddAttribute( "BigEndian", !g_pScene->Settings().bLittleEndian );
    g_pXMLWriter->WriteString( strMediaRelativeBlobFilename );
    g_pXMLWriter->EndElement();

    // Add the binary blob file to the manifest.
    size_t dwIndex = g_pManifest->AddFile( strBlobFilename, strBlobFilename, EFT_BINARY_RESOURCE );
    g_pManifest->GetFile( dwIndex ).strDevKitFileName = strMediaRelativeBlobFilename;
}

void PrepareBundledFile()
{
    if( !g_XATGSettings.bBundleTextures )
        return;

    CHAR* pFilename = strrchr( g_strOutputFileScenePath, '\\' );
    assert( pFilename != nullptr );

    // Compose the bundled texture media-relative file name.
    CHAR strMediaRelativeBundleFilename[MAX_PATH];
    strcpy_s( strMediaRelativeBundleFilename, g_strTextureOutputSubPath + 1 );
    strcat_s( strMediaRelativeBundleFilename, pFilename );
    CHAR* pExt = strstr( strMediaRelativeBundleFilename, ".xatg" );
    if ( !pExt )
        return;
    *pExt = '\0';
    strcat_s( strMediaRelativeBundleFilename, ".xpr" );

    // Compose the bundled texture file name (the .xpr file).
    CHAR strTextureBundleFilename[MAX_PATH];
    strcpy_s( strTextureBundleFilename, g_strOutputFileBasePath );
    strcat_s( strTextureBundleFilename, "\\" );
    strcat_s( strTextureBundleFilename, strMediaRelativeBundleFilename );

    // Compose the bundled texture spec file name (the .rdf file).
    CHAR strTextureBundleSpecFilename[MAX_PATH];
    strcpy_s( strTextureBundleSpecFilename, strTextureBundleFilename );
    pExt = strstr( strTextureBundleSpecFilename, ".xpr" );
    if ( !pExt )
        return;
    *pExt = '\0';
    strcat_s( strTextureBundleSpecFilename, ".rdf" );

    // Add a reference to the file in the XATG scene file.
    g_pXMLWriter->WriteElement( "BundledResources", strMediaRelativeBundleFilename );

    // Add the bundled file name to the manifest.
    size_t dwIndex = g_pManifest->AddFile( strTextureBundleSpecFilename, strTextureBundleFilename, EFT_BUNDLED_RESOURCE );
    g_pManifest->GetFile( dwIndex ).strDevKitFileName = strMediaRelativeBundleFilename;
    g_TextureBundledFile = g_pManifest->GetFile( dwIndex );

    ExportLog::LogMsg( 4, "Writing to bundler .RDF file \"%s\".", strTextureBundleSpecFilename );
    ExportLog::LogMsg( 4, "Writing to bundler .XPR file \"%s\".", strTextureBundleFilename );
}

DWORD GetBinaryBlobCurrentOffset()
{
    if( g_hBinaryBlobFile == INVALID_HANDLE_VALUE )
        return 0;
    return GetFileSize( g_hBinaryBlobFile, nullptr );
}

void WriteBinaryBlobData( BYTE* pData, size_t dwDataSizeBytes )
{
    DWORD dwBytesWritten = 0;
    WriteFile( g_hBinaryBlobFile, pData, static_cast<DWORD>( dwDataSizeBytes ), &dwBytesWritten, nullptr );
    const DWORD dwPadSize = 32;
    if( ( dwDataSizeBytes % dwPadSize ) != 0 )
    {
        DWORD dwZeroPadSize = dwPadSize - ( dwDataSizeBytes % dwPadSize );
        BYTE bZeros[dwPadSize];
        ZeroMemory( bZeros, dwPadSize );
        WriteFile( g_hBinaryBlobFile, bZeros, dwZeroPadSize, &dwBytesWritten, nullptr );
    }
}


const CHAR* GetCompressedTextureFormatString( DXGI_FORMAT format )
{
    switch( format )
    {
    case DXGI_FORMAT_BC1_UNORM:
        return "D3DFMT_DXT1";
    case DXGI_FORMAT_BC3_UNORM:
        return "D3DFMT_DXT5";
    case DXGI_FORMAT_R8G8B8A8_UNORM:
        return "D3DFMT_A8B8G8R8";
    default:
        return "D3DFMT_A8R8G8B8";
    }
}

void BundleTextures()
{
    assert( g_pManifest != nullptr );

    if( g_XATGSettings.bUseExistingBundle )
    {
        if( ExportManifest::FileExists( g_TextureBundledFile.strIntermediateFileName.SafeString() ) )
        {
            // Clear all of the 2D textures from the manifest.
            g_pManifest->ClearFilesOfType( EFT_TEXTURE2D );
            return;
        }
    }

    // Open an XML writer to the RDF file.
    XMLWriter BundleFileWriter;
    BundleFileWriter.Initialize( g_TextureBundledFile.strSourceFileName );

    // Write RDF header.
    BundleFileWriter.StartElement( "RDF" );
    BundleFileWriter.AddAttribute( "Version", "XPR2" );

    ExportLog::LogMsg( 4, "Creating bundler RDF file." );
    g_pProgress->SetCaption( "Converting textures..." );

    ExportFileRecordVector BundledFiles;

    for( size_t i = 0; i < g_pManifest->GetFileCount(); i++ )
    {
        // Only process textures.
        ExportFileRecord& File = g_pManifest->GetFile( i );
        if( File.FileType != EFT_TEXTURE2D &&
            File.FileType != EFT_TEXTURECUBE &&
            File.FileType != EFT_TEXTUREVOLUME )
            continue;

        ExportLog::LogMsg( 4, "Bundling resource %s from texture intermediate file %s.", File.strResourceName.SafeString(), File.strIntermediateFileName.SafeString() );

        // Write the RDF entry to the file.
        switch( File.FileType )
        {
        case EFT_TEXTURE2D:
            BundleFileWriter.StartElement( "Texture" );
            BundleFileWriter.AddAttribute( "Name", File.strResourceName );
            BundleFileWriter.AddAttribute( "Source", File.strIntermediateFileName );
            BundleFileWriter.AddAttribute( "Format", GetCompressedTextureFormatString( File.CompressedTextureFormat ) );
            BundleFileWriter.AddAttribute( "Levels", 0 );
            BundleFileWriter.EndElement();
            BundledFiles.push_back( File );
            break;
        }
    }

    // Close the RDF file.
    BundleFileWriter.EndElement(); // RDF
    BundleFileWriter.Close();

    // If we didn't have any textures to bundle, return.
    if( BundledFiles.empty() )
    {
        DeleteFile( g_TextureBundledFile.strSourceFileName );
        g_pManifest->ClearFilesOfType( EFT_BUNDLED_RESOURCE );
        return;
    }

    g_pProgress->SetCaption( "Bundling textures..." );
    // Use the XEDK env variable to figure out where the bundler is.
    CHAR strBundlerExe[MAX_PATH];
    CHAR strXEDKLocation[MAX_PATH];
    size_t BufferSize = ARRAYSIZE(strXEDKLocation);
    getenv_s( &BufferSize, strXEDKLocation, BufferSize, "XEDK" );
    strcpy_s( strBundlerExe, strXEDKLocation );
    strcat_s( strBundlerExe, "\\bin\\win32\\bundler.exe" );

    // Compose the command line for the bundler.
    CHAR strBundlerCmdLine[MAX_PATH * 5];
    strcpy_s( strBundlerCmdLine, "\"" );
    strcat_s( strBundlerCmdLine, g_TextureBundledFile.strSourceFileName );
    strcat_s( strBundlerCmdLine, "\" -o \"");
    strcat_s( strBundlerCmdLine, g_TextureBundledFile.strIntermediateFileName );
    strcat_s( strBundlerCmdLine, "\"" );

    bool bExecuteBundler = true;

    if( bExecuteBundler )
    {
        // Run the bundler.
        ExportLog::LogMsg( 4, "Running the bundler to create %s.", g_TextureBundledFile.strIntermediateFileName.SafeString() );
        SHELLEXECUTEINFO sei;
        ZeroMemory( &sei, sizeof( SHELLEXECUTEINFO ) );
        sei.cbSize = sizeof( SHELLEXECUTEINFO );
        sei.fMask = SEE_MASK_NOCLOSEPROCESS;
        sei.hwnd = nullptr;
        sei.lpFile = strBundlerExe;
        sei.lpParameters = strBundlerCmdLine;
        sei.lpDirectory = nullptr;
        sei.lpVerb = nullptr;
        sei.nShow = SW_SHOW;

        ShellExecuteEx( &sei );
        if( sei.hProcess )
            WaitForSingleObject( sei.hProcess, 600000 );
    }

    // Clear all of the 2D textures from the manifest.
    g_pManifest->ClearFilesOfType( EFT_TEXTURE2D );
}

}
