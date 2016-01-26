//-------------------------------------------------------------------------------------
// ParseMaterial.cpp
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
#include <algorithm>
#include "ParseMaterial.h"

using namespace ATG;

extern ATG::ExportScene* g_pScene;

bool MaterialParameterSort( ExportMaterialParameter A, ExportMaterialParameter B )
{
    if( A.ParamType == MPT_TEXTURE2D && B.ParamType != MPT_TEXTURE2D )
        return true;
    return false;
}

void FixupGenericMaterial( ExportMaterial* pMaterial )
{
    ExportMaterialParameter OutputParam;
    OutputParam.ParamType = MPT_TEXTURE2D;
    OutputParam.bInstanceParam = true;

    ExportMaterialParameter* pParam = pMaterial->FindParameter( "DiffuseTexture" );
    if (!pParam)
    {
        OutputParam.ValueString = ExportMaterial::GetDefaultDiffuseMapTextureName();
        if (*OutputParam.ValueString)
        {
            ExportLog::LogMsg(2, "Material \"%s\" has no diffuse texture.  Assigning a default diffuse texture.", pMaterial->GetName().SafeString());
            OutputParam.Name = "DiffuseTexture";
            pMaterial->AddParameter(OutputParam);
        }
    }
    else if ( g_ExportCoreSettings.bMaterialColors )
    {
        auto pColor = pMaterial->FindParameter( "DiffuseColor" );

        if (pColor && pColor->ValueFloat[0] == 0 && pColor->ValueFloat[1] == 0 && pColor->ValueFloat[2] == 0)
        {
            ExportLog::LogWarning( "Material \"%s\" has a black DiffuseColor which will modulate a DiffuseTexture to black. Set a DiffuseColor or use -materialcolors-.", pMaterial->GetName().SafeString());
        }
    }

    pParam = pMaterial->FindParameter( "NormalMapTexture" );
    if (!pParam)
    {
        OutputParam.ValueString = ExportMaterial::GetDefaultNormalMapTextureName();
        if (*OutputParam.ValueString)
        {
            ExportLog::LogMsg(2, "Material \"%s\" has no normal map texture.  Assigning a default normal map texture.", pMaterial->GetName().SafeString());
            OutputParam.Name = "NormalMapTexture";
            pMaterial->AddParameter(OutputParam);
        }
    }

    pParam = pMaterial->FindParameter( "SpecularMapTexture" );
    if (!pParam)
    {
        if (g_ExportCoreSettings.bUseEmissiveTexture)
        {
            pParam = pMaterial->FindParameter( "EmissiveMapTexture" );
            if (pParam)
            {
                // Copy emissive to specular (SDKMESH's material doesn't have an emissive texture slot)
                ExportLog::LogMsg( 4, "EmissiveMapTexture encoded as SpecularMapTexture in material \"%s\".",  pMaterial->GetName().SafeString() );
                OutputParam.Name = "SpecularMapTexture";
                OutputParam.ValueString = pParam->ValueString;
                pMaterial->AddParameter(OutputParam);
            }
        }

        if (!pParam)
        {
            OutputParam.ValueString = ExportMaterial::GetDefaultSpecularMapTextureName();
            if (*OutputParam.ValueString)
            {
                ExportLog::LogMsg(2, "Material \"%s\" has no specular map texture.  Assigning a default specular map texture.", pMaterial->GetName().SafeString());
                OutputParam.Name = "SpecularMapTexture";
                pMaterial->AddParameter(OutputParam);
            }
        }
    }

    auto pParamList = pMaterial->GetParameterList();
    //std::reverse( pParamList->begin(), pParamList->end() );
    std::stable_sort( pParamList->begin(), pParamList->end(), MaterialParameterSort );
}

void AddTextureParameter( ExportMaterial* pMaterial, const CHAR* strParamName, DWORD dwIndex, const CHAR* strFileName, DWORD dwFlags )
{
    ExportMaterialParameter OutputParam;
    if( dwIndex == 0 )
    {
        OutputParam.Name = strParamName;
    }
    else
    {
        CHAR strDecoratedName[512];
        sprintf_s( strDecoratedName, "%s%u", strParamName, dwIndex );
        OutputParam.Name = strDecoratedName;
    }
    ExportLog::LogMsg( 4, "Material parameter \"%s\" = \"%s\"", OutputParam.Name.SafeString(), strFileName );
    OutputParam.ValueString = strFileName;
    OutputParam.ParamType = MPT_TEXTURE2D;
    OutputParam.bInstanceParam = true;
    OutputParam.Flags = dwFlags;
    pMaterial->AddParameter( OutputParam );
}

static void CheckUVSettings(FbxFileTexture* texture, ExportMaterial* pMaterial)
{
    if (texture->GetSwapUV())
    {
        ExportLog::LogWarning( "Material \"%s\" has swapped UVs which are not exported as such", pMaterial->GetName().SafeString() );
    }
   
    if ( texture->GetWrapModeU() != FbxTexture::eRepeat
         || texture->GetWrapModeV() != FbxTexture::eRepeat )
    {
        ExportLog::LogWarning( "Material \"%s\" has set to clamp wrap U/V mode which is not exported", pMaterial->GetName().SafeString() );
    }

    auto& uvScaling = texture->GetUVScaling();
    auto& uvTrans = texture->GetUVTranslation();
    if ( uvScaling[0] != 1.0
         || uvScaling[1] != 1.0
         || uvTrans[0] != 0
         || uvTrans[1] != 0 )
    {
        ExportLog::LogWarning( "Material \"%s\" has UV transforms which are not exported", pMaterial->GetName().SafeString() );
    }
}

bool ExtractTextures( FbxProperty Property, const CHAR* strParameterName, ExportMaterial* pMaterial, DWORD dwFlags )
{
    bool bResult = false;
    DWORD dwLayeredTextureCount = Property.GetSrcObjectCount( FbxLayeredTexture::ClassId );
    if( dwLayeredTextureCount > 0 )
    {
        DWORD dwTextureIndex = 0;
        for( DWORD i = 0; i < dwLayeredTextureCount; ++i )
        {
            auto pFbxLayeredTexture = FbxCast<FbxLayeredTexture>( Property.GetSrcObject( FbxLayeredTexture::ClassId, i ) );
            DWORD dwTextureCount = pFbxLayeredTexture->GetSrcObjectCount( FbxFileTexture::ClassId );
            for( DWORD j = 0; j < dwTextureCount; ++j )
            {
                auto pFbxTexture = FbxCast<FbxFileTexture>( pFbxLayeredTexture->GetSrcObject( FbxFileTexture::ClassId, j ) );
                if( !pFbxTexture )
                    continue;

                CheckUVSettings(pFbxTexture, pMaterial);

                AddTextureParameter( pMaterial, strParameterName, dwTextureIndex, pFbxTexture->GetFileName(), dwFlags );
                ++dwTextureIndex;
                bResult = true;
            }
        }
    }
    else
    {
        DWORD dwTextureCount = Property.GetSrcObjectCount( FbxFileTexture::ClassId );
        for( DWORD i = 0; i < dwTextureCount; ++i )
        {
            auto pFbxTexture = FbxCast<FbxFileTexture>( Property.GetSrcObject( FbxFileTexture::ClassId, i ) );
            if( !pFbxTexture )
                continue;

            CheckUVSettings(pFbxTexture, pMaterial);

            AddTextureParameter( pMaterial, strParameterName, i, pFbxTexture->GetFileName(), dwFlags );
            bResult = true;
        }
    }
    return bResult;
}

ExportMaterial* ParseMaterial( FbxSurfaceMaterial* pFbxMaterial )
{
    if ( !pFbxMaterial )
        return nullptr;

    auto pExistingMaterial = g_pScene->FindMaterial( pFbxMaterial );
    if( pExistingMaterial )
    {
        ExportLog::LogMsg( 4, "Found existing material \"%s\".", pFbxMaterial->GetName() );
        return pExistingMaterial;
    }

    ExportLog::LogMsg( 2, "Parsing material \"%s\".", pFbxMaterial->GetName() );

    bool bRenameMaterial = false;
    ExportString MaterialName( pFbxMaterial->GetName() );
    ExportMaterial* pSameNameMaterial = nullptr;
    DWORD dwRenameIndex = 0;
    do 
    {
        pSameNameMaterial = g_pScene->FindMaterial( MaterialName );
        if( pSameNameMaterial )
        {
            bRenameMaterial = true;
            CHAR strName[200];
            sprintf_s( strName, "%s_%u", pFbxMaterial->GetName(), dwRenameIndex++ );
            MaterialName = strName;
        }
    } while ( pSameNameMaterial );

    if( bRenameMaterial )
    {
        ExportLog::LogMsg( 2, "Found duplicate material name; renaming material \"%s\" to \"%s\".", pFbxMaterial->GetName(), MaterialName.SafeString() );
    }

    ExportMaterial* pMaterial = new ExportMaterial( MaterialName );
    pMaterial->SetDCCObject( pFbxMaterial );
    pMaterial->SetDefaultMaterialName( g_pScene->Settings().strDefaultMaterialName );

    if ( g_ExportCoreSettings.bMaterialColors )
    {
        auto pFbxLambert = FbxCast<FbxSurfaceLambert>(pFbxMaterial);
        if ( pFbxLambert )
        {
            // Diffuse Color
            {
                FbxDouble3 color = pFbxLambert->Diffuse.Get();
                double factor = pFbxLambert->DiffuseFactor.Get();

                ExportMaterialParameter OutputParam;
                OutputParam.Name = "DiffuseColor";
                OutputParam.ValueFloat[0] = static_cast<float>( color[0] * factor );
                OutputParam.ValueFloat[1] = static_cast<float>( color[1] * factor );
                OutputParam.ValueFloat[2] = static_cast<float>( color[2] * factor );
                OutputParam.ValueFloat[3] = static_cast<float>( 1.0 - pFbxLambert->TransparencyFactor.Get() );
                OutputParam.ParamType = MPT_FLOAT4;
                OutputParam.bInstanceParam = true;
                OutputParam.Flags = 0;
                pMaterial->AddParameter( OutputParam );

                ExportLog::LogMsg( 4, "Material parameter \"%s\" = \"%f %f %f %f\"", OutputParam.Name.SafeString(),
                                   OutputParam.ValueFloat[0], OutputParam.ValueFloat[1], OutputParam.ValueFloat[2], OutputParam.ValueFloat[3] );
            }

            // Ambient Color
            {
                FbxDouble3 color = pFbxLambert->Ambient.Get();
                double factor = pFbxLambert->AmbientFactor.Get();

                ExportMaterialParameter OutputParam;
                OutputParam.Name = "AmbientColor";
                OutputParam.ValueFloat[0] = static_cast<float>( color[0] * factor );
                OutputParam.ValueFloat[1] = static_cast<float>( color[1] * factor );
                OutputParam.ValueFloat[2] = static_cast<float>( color[2] * factor );
                OutputParam.ParamType = MPT_FLOAT3;
                OutputParam.bInstanceParam = true;
                OutputParam.Flags = 0;
                pMaterial->AddParameter( OutputParam );

                ExportLog::LogMsg( 4, "Material parameter \"%s\" = \"%f %f %f\"", OutputParam.Name.SafeString(),
                                   OutputParam.ValueFloat[0], OutputParam.ValueFloat[1], OutputParam.ValueFloat[2] );
            }

            // Emissive Color
            {
                FbxDouble3 color = pFbxLambert->Emissive.Get();
                double factor = pFbxLambert->EmissiveFactor.Get();

                ExportMaterialParameter OutputParam;
                OutputParam.Name = "EmissiveColor";
                OutputParam.ValueFloat[0] = static_cast<float>( color[0] * factor );
                OutputParam.ValueFloat[1] = static_cast<float>( color[1] * factor );
                OutputParam.ValueFloat[2] = static_cast<float>( color[2] * factor );
                OutputParam.ParamType = MPT_FLOAT3;
                OutputParam.bInstanceParam = true;
                OutputParam.Flags = 0;
                pMaterial->AddParameter( OutputParam );

                ExportLog::LogMsg( 4, "Material parameter \"%s\" = \"%f %f %f\"", OutputParam.Name.SafeString(),
                                   OutputParam.ValueFloat[0], OutputParam.ValueFloat[1], OutputParam.ValueFloat[2] );
            }

            auto pFbxPhong = FbxCast<FbxSurfacePhong>(pFbxLambert);
            if (pFbxPhong)
            {
                // Specular Color
                {
                    FbxDouble3 color = pFbxPhong->Specular.Get();
                    double factor = pFbxPhong->SpecularFactor.Get();

                    ExportMaterialParameter OutputParam;
                    OutputParam.Name = "SpecularColor";
                    OutputParam.ValueFloat[0] = static_cast<float>( color[0] * factor );
                    OutputParam.ValueFloat[1] = static_cast<float>( color[1] * factor );
                    OutputParam.ValueFloat[2] = static_cast<float>( color[2] * factor );
                    OutputParam.ParamType = MPT_FLOAT3;
                    OutputParam.bInstanceParam = true;
                    OutputParam.Flags = 0;
                    pMaterial->AddParameter( OutputParam );

                    ExportLog::LogMsg( 4, "Material parameter \"%s\" = \"%f %f %f\"", OutputParam.Name.SafeString(),
                                       OutputParam.ValueFloat[0], OutputParam.ValueFloat[1], OutputParam.ValueFloat[2] );
                }

                // Specular Power
                {
                    ExportMaterialParameter OutputParam;
                    OutputParam.Name = "SpecularPower";
                    OutputParam.ValueFloat[0] = static_cast<float>( pFbxPhong->Shininess.Get() );
                    OutputParam.ParamType = MPT_FLOAT;
                    OutputParam.bInstanceParam = true;
                    OutputParam.Flags = 0;
                    pMaterial->AddParameter( OutputParam );

                    ExportLog::LogMsg( 4, "Material parameter \"%s\" = \"%f\"", OutputParam.Name.SafeString(), OutputParam.ValueFloat[0] );
                }
            }
        }
    }

    enum ParameterPostOperations
    {
        PPO_Nothing = 0,
        PPO_TransparentMaterial = 1,
    };

    struct TextureParameterExtraction
    {
        const CHAR* strFbxPropertyName;
        const CHAR* strParameterName;
        DWORD dwPostOperations;
        DWORD dwParameterFlags;
    };

    TextureParameterExtraction ExtractionList[] =
    {
        { FbxSurfaceMaterial::sTransparentColor,   "AlphaTexture",                 PPO_TransparentMaterial,    ExportMaterialParameter::EMPF_ALPHACHANNEL },
        { FbxSurfaceMaterial::sDiffuse,            "DiffuseTexture",               PPO_Nothing,                ExportMaterialParameter::EMPF_DIFFUSEMAP },
        { FbxSurfaceMaterial::sAmbient,            "AOTexture",	                   PPO_Nothing,                ExportMaterialParameter::EMPF_AOMAP },
        { FbxSurfaceMaterial::sBump,               "NormalMapTexture",             PPO_Nothing,                0 /*ExportMaterialParameter::EMPF_BUMPMAP*/ },
        { FbxSurfaceMaterial::sNormalMap,          "NormalMapTexture",             PPO_Nothing,                ExportMaterialParameter::EMPF_NORMALMAP },
        { FbxSurfaceMaterial::sSpecular,           "SpecularMapTexture",           PPO_Nothing,                ExportMaterialParameter::EMPF_SPECULARMAP },
        { FbxSurfaceMaterial::sEmissive,           "EmissiveMapTexture",           PPO_Nothing,                0 },
    };

    for( DWORD dwExtractionIndex = 0; dwExtractionIndex < ARRAYSIZE(ExtractionList); ++dwExtractionIndex )
    {
        const TextureParameterExtraction& tpe = ExtractionList[dwExtractionIndex];

        auto Property = pFbxMaterial->FindProperty( tpe.strFbxPropertyName );
        if( !Property.IsValid() )
            continue;

        bool bFound = ExtractTextures( Property, tpe.strParameterName, pMaterial, tpe.dwParameterFlags );
        if( bFound )
        {
            if( tpe.dwPostOperations & PPO_TransparentMaterial )
            {
                ExportLog::LogMsg( 4, "Material \"%s\" is transparent.", pMaterial->GetName().SafeString() );
                pMaterial->SetTransparent( true );
            }
        }
    }

    FixupGenericMaterial( pMaterial );

    bool bResult = g_pScene->AddMaterial( pMaterial );
    assert( bResult );
    if( !bResult )
    {
        ExportLog::LogError( "Could not add material \"%s\" to scene.", pMaterial->GetName().SafeString() );
    }
    g_pScene->Statistics().MaterialsExported++;

    return pMaterial;
}
