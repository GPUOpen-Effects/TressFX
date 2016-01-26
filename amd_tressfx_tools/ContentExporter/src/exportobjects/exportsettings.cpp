//-------------------------------------------------------------------------------------
// ExportSettings.cpp
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
#include "ExportSettings.h"

namespace ATG
{
    ExportSettingsManager   g_SettingsManager;
    ExportCoreSettings      g_ExportCoreSettings;

    ExportSettingsEntry::ExportSettingsEntry()
        : m_pFirstChild( nullptr ),
          m_pSibling( nullptr ),
          m_pLinkedCurrentValue( nullptr ),
          m_pEnumValues( nullptr ),
          m_dwEnumValueCount( 0 )
    {
    }

    ExportSettingsEntry::~ExportSettingsEntry()
    {
        if( m_pFirstChild )
        {
            delete m_pFirstChild;
            m_pFirstChild = nullptr;
        }
        if( m_pSibling )
        {
            delete m_pSibling;
            m_pSibling = nullptr;
        }
    }

    void ExportSettingsEntry::SetDefaultValue( bool bSetChildren, bool bSetSiblings )
    {
        if( m_pLinkedCurrentValue )
        {
            if( m_DefaultValue.m_Type == ExportVariant::VT_STRING )
                strcpy_s( (CHAR*)m_pLinkedCurrentValue, SETTINGS_STRING_LENGTH, m_DefaultValue.m_strValue );
            else if( m_DefaultValue.m_Type == ExportVariant::VT_BOOL )
                *reinterpret_cast<bool*>( m_pLinkedCurrentValue ) = m_DefaultValue.m_bValue ? true : false;
            else
                *reinterpret_cast<INT*>( m_pLinkedCurrentValue ) = m_DefaultValue.m_iValue;
        }
        else
        {
            m_CurrentValue = m_DefaultValue;
        }

        if( bSetSiblings && m_pSibling )
        {
            m_pSibling->SetDefaultValue( bSetChildren, true );
        }
        if( bSetChildren && m_pFirstChild )
        {
            m_pFirstChild->SetDefaultValue( true, true );
        }
    }

    void ExportSettingsEntry::ReverseChildOrder()
    {
        ExportSettingsEntry* pHead = nullptr;
        ExportSettingsEntry* pCurrent = m_pFirstChild;

        while( pCurrent )
        {
            ExportSettingsEntry* pNext = pCurrent->m_pSibling;
            pCurrent->m_pSibling = pHead;
            pHead = pCurrent;
            pCurrent = pNext;
        }

        m_pFirstChild = pHead;
    }

    void ExportSettingsEntry::CreateSettingName()
    {
        CHAR strSettingName[512];
        ZeroMemory( strSettingName, sizeof( strSettingName ) );
        const CHAR* strDisplayName = m_DisplayName.SafeString();
        assert( strlen( strDisplayName ) < ARRAYSIZE( strSettingName ) );
        CHAR* pDest = strSettingName;
        while( *strDisplayName != '\0' )
        {
            if( *strDisplayName != ' ' )
            {
                *pDest = *strDisplayName;
                ++pDest;
            }
            ++strDisplayName;
        }
        *pDest = '\0';
        m_SettingName = strSettingName;
    }

    bool ExportSettingsEntry::GetValueBool() const
    {
        assert( m_Type == CT_CHECKBOX );
        auto pValue = reinterpret_cast<const bool*>( GetCurrentValue() );
        return *pValue;
    }

    const CHAR* ExportSettingsEntry::GetValueString() const
    {
        assert( m_Type == CT_STRING );
        if( m_pLinkedCurrentValue )
            return reinterpret_cast<const CHAR*>( m_pLinkedCurrentValue );
        return m_CurrentValue.m_strValue;
    }

    INT ExportSettingsEntry::GetValueInt() const
    {
        assert( m_Type == CT_BOUNDEDINTSLIDER || m_Type == CT_ENUM );
        auto pValue = reinterpret_cast<const INT*>( GetCurrentValue() );
        return *pValue;
    }

    float ExportSettingsEntry::GetValueFloat() const
    {
        assert( m_Type == CT_BOUNDEDFLOATSLIDER );
        auto pValue = reinterpret_cast<const float*>( GetCurrentValue() );
        return *pValue;
    }

    void ExportSettingsEntry::SetValue( INT iValue )
    {
        assert( m_Type == CT_BOUNDEDINTSLIDER || m_Type == CT_CHECKBOX || m_Type == CT_ENUM );

        if ( m_CurrentValue.m_Type == ExportVariant::VT_BOOL )
        {
            auto pValue = reinterpret_cast<bool*>( GetCurrentValue() );
            *pValue = iValue ? true : false;
        }
        else
        {
            auto pValue = reinterpret_cast<INT*>( GetCurrentValue() );
            *pValue = iValue;
        }
    }

    void ExportSettingsEntry::SetValue( float fValue )
    {
        assert( m_Type == CT_BOUNDEDFLOATSLIDER );
        auto pValue = reinterpret_cast<float*>( GetCurrentValue() );
        *pValue = fValue;
    }

    void ExportSettingsEntry::SetValue( const CHAR* strValue )
    {
        assert( m_Type == CT_STRING );
        if( m_pLinkedCurrentValue )
        {
            strcpy_s( (CHAR*)m_pLinkedCurrentValue, SETTINGS_STRING_LENGTH, strValue );
        }
        else
        {
            m_CurrentValue.m_strValue = strValue;
        }
    }

    ExportSettingsManager::~ExportSettingsManager()
    {
        size_t dwCount = GetRootCategoryCount();
        for( size_t i = 0; i < dwCount; ++i )
        {
            delete GetRootCategory( i );
        }
        m_RootCategories.clear();
    }

    ExportSettingsEntry* ExportSettingsManager::AddRootCategory( ExportString Caption )
    {
        return AddCategory( nullptr, Caption );
    }

    ExportSettingsEntry* ExportSettingsManager::AddCategory( ExportSettingsEntry* pParentCategory, ExportString Caption )
    {
        ExportSettingsEntry* pNewCategory = new ExportSettingsEntry();
        pNewCategory->m_DisplayName = Caption;
        pNewCategory->m_Type = ExportSettingsEntry::CT_CATEGORY;
        if( pParentCategory )
        {
            pParentCategory->AddChild( pNewCategory );
        }
        else
        {
            m_RootCategories.push_back( pNewCategory );
        }

        return pNewCategory;
    }

    ExportSettingsEntry* ExportSettingsManager::AddBool( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, bool bDefaultValue, bool* pLinkedValue )
    {
        assert( pCategory != nullptr );
        ExportSettingsEntry* pNewEntry = new ExportSettingsEntry();
        pNewEntry->m_DisplayName = Caption;
        pNewEntry->CreateSettingName();
        pNewEntry->m_CommandLineOptionName = CmdLine;
        pNewEntry->m_DefaultValue.m_bValue = bDefaultValue;
        pNewEntry->m_DefaultValue.m_Type = ExportVariant::VT_BOOL;
        pNewEntry->m_pLinkedCurrentValue = pLinkedValue;
        pNewEntry->m_CurrentValue = pNewEntry->m_DefaultValue;
        pNewEntry->m_Type = ExportSettingsEntry::CT_CHECKBOX;

        pCategory->AddChild( pNewEntry );
        return pNewEntry;
    }

    ExportSettingsEntry* ExportSettingsManager::AddIntBounded( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, INT iDefaultValue, INT iMin, INT iMax, INT* pLinkedValue )
    {
        assert( pCategory != nullptr );
        ExportSettingsEntry* pNewEntry = new ExportSettingsEntry();
        pNewEntry->m_DisplayName = Caption;
        pNewEntry->CreateSettingName();
        pNewEntry->m_CommandLineOptionName = CmdLine;
        pNewEntry->m_DefaultValue.m_iValue = iDefaultValue;
        pNewEntry->m_DefaultValue.m_Type = ExportVariant::VT_INT;
        pNewEntry->m_pLinkedCurrentValue = pLinkedValue;
        pNewEntry->m_CurrentValue = pNewEntry->m_DefaultValue;
        pNewEntry->m_Type = ExportSettingsEntry::CT_BOUNDEDINTSLIDER;

        pNewEntry->m_MinValue.m_iValue = iMin;
        pNewEntry->m_MaxValue.m_iValue = iMax;

        pCategory->AddChild( pNewEntry );
        return pNewEntry;
    }

    ExportSettingsEntry* ExportSettingsManager::AddFloatBounded( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, float fDefaultValue, float fMin, float fMax, float* pLinkedValue )
    {
        assert( pCategory != nullptr );
        ExportSettingsEntry* pNewEntry = new ExportSettingsEntry();
        pNewEntry->m_DisplayName = Caption;
        pNewEntry->CreateSettingName();
        pNewEntry->m_CommandLineOptionName = CmdLine;
        pNewEntry->m_DefaultValue.m_fValue = fDefaultValue;
        pNewEntry->m_DefaultValue.m_Type = ExportVariant::VT_FLOAT;
        pNewEntry->m_pLinkedCurrentValue = pLinkedValue;
        pNewEntry->m_CurrentValue = pNewEntry->m_DefaultValue;
        pNewEntry->m_Type = ExportSettingsEntry::CT_BOUNDEDFLOATSLIDER;

        pNewEntry->m_MinValue.m_fValue = fMin;
        pNewEntry->m_MaxValue.m_fValue = fMax;

        pCategory->AddChild( pNewEntry );
        return pNewEntry;
    }

    ExportSettingsEntry* ExportSettingsManager::AddString( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, const CHAR* strDefaultValue, CHAR* pLinkedValue )
    {
        assert( pCategory != nullptr );
        ExportSettingsEntry* pNewEntry = new ExportSettingsEntry();
        pNewEntry->m_DisplayName = Caption;
        pNewEntry->CreateSettingName();
        pNewEntry->m_CommandLineOptionName = CmdLine;
        pNewEntry->m_DefaultValue.m_strValue = strDefaultValue;
        pNewEntry->m_DefaultValue.m_Type = ExportVariant::VT_STRING;
        pNewEntry->m_pLinkedCurrentValue = pLinkedValue;
        pNewEntry->m_CurrentValue = pNewEntry->m_DefaultValue;
        pNewEntry->m_Type = ExportSettingsEntry::CT_STRING;

        pCategory->AddChild( pNewEntry );
        return pNewEntry;
    }

    ExportSettingsEntry* ExportSettingsManager::AddEnum( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, INT iDefaultValue, const ExportEnumValue* pEnumValues, DWORD dwEnumValueCount, INT* pLinkedValue )
    {
        assert( pCategory != nullptr );
        ExportSettingsEntry* pNewEntry = new ExportSettingsEntry();
        pNewEntry->m_DisplayName = Caption;
        pNewEntry->CreateSettingName();
        pNewEntry->m_CommandLineOptionName = CmdLine;
        pNewEntry->m_DefaultValue.m_iValue = iDefaultValue;
        pNewEntry->m_DefaultValue.m_Type = ExportVariant::VT_INT;
        pNewEntry->m_pLinkedCurrentValue = pLinkedValue;
        pNewEntry->m_CurrentValue = pNewEntry->m_DefaultValue;
        pNewEntry->m_Type = ExportSettingsEntry::CT_ENUM;
        pNewEntry->m_pEnumValues = pEnumValues;
        pNewEntry->m_dwEnumValueCount = dwEnumValueCount;

        pCategory->AddChild( pNewEntry );
        return pNewEntry;
    }

    bool ExportSettingsManager::MarshalAllSettings( CHAR* strDestBuffer, DWORD dwBufferSize, bool bNewLines, ExportSettingsEntry* pRoot )
    {
        bool bResult = true;
        if( !pRoot )
        {
            size_t dwRootCount = GetRootCategoryCount();
            for( size_t i = 0; i < dwRootCount; ++i )
            {
                bResult &= MarshalAllSettings( strDestBuffer, dwBufferSize, bNewLines, GetRootCategory( i ) );
            }
            return bResult;
        }

        if( pRoot->m_Type == ExportSettingsEntry::CT_CATEGORY && pRoot->m_pFirstChild )
        {
            bResult &= MarshalAllSettings( strDestBuffer, dwBufferSize, bNewLines, pRoot->m_pFirstChild );
        }
        else
        {
            size_t dwCurrentLen = strlen( strDestBuffer );
            CHAR* strCurrentSpot = strDestBuffer + dwCurrentLen;
            const CHAR* strSettingName = pRoot->m_CommandLineOptionName.SafeString();
            size_t dwNameLen = strlen( strSettingName );
            if( dwCurrentLen + dwNameLen + 1 > dwBufferSize )
                return false;
            strcat_s( strDestBuffer, dwBufferSize, strSettingName );
            strcat_s( strDestBuffer, dwBufferSize, "=" );
            dwCurrentLen += ( dwNameLen + 1 );

            switch( pRoot->m_CurrentValue.m_Type )
            {
            case ExportVariant::VT_STRING:
                {
                    const CHAR* strValue = pRoot->GetValueString();
                    size_t dwValueLen = strlen( strValue );
                    if( dwCurrentLen + dwValueLen + 1 > dwBufferSize )
                    {
                        *strCurrentSpot = '\0';
                        return false;
                    }
                    strcat_s( strDestBuffer, dwBufferSize, strValue );
                    break;
                }
            case ExportVariant::VT_INT:
            case ExportVariant::VT_BOOL:
                {
                    INT iValue = 0;
                    if( pRoot->m_CurrentValue.m_Type == ExportVariant::VT_BOOL )
                        iValue = pRoot->GetValueBool();
                    else
                        iValue = pRoot->GetValueInt();
                    CHAR strValue[32] = {0};
                    _itoa_s( iValue, strValue, 10 );
                    size_t dwValueLen = strlen( strValue );
                    if( dwCurrentLen + dwValueLen + 1 > dwBufferSize )
                    {
                        *strCurrentSpot = '\0';
                        return false;
                    }
                    strcat_s( strDestBuffer, dwBufferSize, strValue );
                    break;
                }
            case ExportVariant::VT_FLOAT:
                {
                    float fValue = pRoot->GetValueFloat();
                    CHAR strValue[32];
                    sprintf_s( strValue, "%0.5f", fValue );
                    size_t dwValueLen = strlen( strValue );
                    if( dwCurrentLen + dwValueLen + 1 > dwBufferSize )
                    {
                        *strCurrentSpot = '\0';
                        return false;
                    }
                    strcat_s( strDestBuffer, dwBufferSize, strValue );
                    break;
                }
            }
            if( bNewLines )
            {
                strcat_s( strDestBuffer, dwBufferSize, ";\n" );
            }
            else
            {
                strcat_s( strDestBuffer, dwBufferSize, ";" );
            }
        }

        if( pRoot->m_pSibling )
        {
            bResult &= MarshalAllSettings( strDestBuffer, dwBufferSize, bNewLines, pRoot->m_pSibling );
        }

        return bResult;
    }

    bool ExportSettingsManager::UnMarshalAllSettings( const CHAR* strSrcBuffer )
    {
        size_t dwSrcLen = strlen( strSrcBuffer );
        if( dwSrcLen == 0 )
            return true;

        std::unique_ptr<CHAR[]> strSrcTokenized( new CHAR[dwSrcLen + 1] );
        strcpy_s( strSrcTokenized.get(), dwSrcLen + 1, strSrcBuffer );

        static const CHAR* strDelimiters = ";\n";

        CHAR* strNextToken = nullptr;
        CHAR* pToken = strtok_s( strSrcTokenized.get(), strDelimiters, &strNextToken );
        while( pToken )
        {
            bool bProcess = true;
            if( strlen( pToken ) == 0 )
                bProcess = false;
            if( *pToken == '#' )
                bProcess = false;

            if( bProcess )
            {
                CHAR* strSettingName = pToken;
                CHAR* strEquals = strchr( pToken, '=' );
                if( !strEquals )
                    break;
                CHAR* strValue = strEquals + 1;
                *strEquals = '\0';

                ExportSettingsEntry* pEntry = FindSettingsEntry( strSettingName, true, nullptr );
                if( pEntry )
                {
                    ExportLog::LogMsg( 4, "Setting \"%s\" = \"%s\"", strSettingName, strValue );
                    switch( pEntry->m_CurrentValue.m_Type )
                    {
                    case ExportVariant::VT_STRING:
                        pEntry->SetValue( strValue );
                        break;
                    case ExportVariant::VT_INT:
                    case ExportVariant::VT_BOOL:
                        pEntry->SetValue( atoi( strValue ) );
                        break;
                    case ExportVariant::VT_FLOAT:
                        pEntry->SetValue( (float)atof( strValue ) );
                        break;
                    }
                }
                else
                {
                    ExportLog::LogWarning( "Did not find setting \"%s\"", strSettingName );
                }
            }
            pToken = strtok_s( nullptr, strDelimiters, &strNextToken );
        }

        return true;
    }

    ExportSettingsEntry* ExportSettingsManager::FindSettingsEntry( ExportString SettingName, bool bCommandLineName, ExportSettingsEntry* pRoot )
    {
        static ExportSettingsEntry* pCachedNextEntry = nullptr;
        if( pCachedNextEntry )
        {
            if( ( bCommandLineName && pCachedNextEntry->m_CommandLineOptionName == SettingName ) ||
                ( !bCommandLineName && pCachedNextEntry->m_SettingName == SettingName ) )
            {
                ExportSettingsEntry* pFound = pCachedNextEntry;
                pCachedNextEntry = pCachedNextEntry->m_pSibling;
                return pFound;
            }
        }

        if( !pRoot )
        {
            size_t dwRootCount = GetRootCategoryCount();
            for( size_t i = 0; i < dwRootCount; ++i )
            {
                ExportSettingsEntry* pFound = FindSettingsEntry( SettingName, bCommandLineName, GetRootCategory( i ) );
                if( pFound )
                {
                    pCachedNextEntry = pFound->m_pSibling;
                    return pFound;
                }
            }
        }
        else
        {
            if( pRoot->m_Type == ExportSettingsEntry::CT_CATEGORY )
            {
                if( pRoot->m_pFirstChild )
                {
                    ExportSettingsEntry* pEntry = FindSettingsEntry( SettingName, bCommandLineName, pRoot->m_pFirstChild );
                    if( pEntry )
                    {
                        return pEntry;
                    }
                }
                if( pRoot->m_pSibling )
                {
                    return FindSettingsEntry( SettingName, bCommandLineName, pRoot->m_pSibling );
                }
            }
            if( ( bCommandLineName && pRoot->m_CommandLineOptionName == SettingName ) ||
                ( !bCommandLineName && pRoot->m_SettingName == SettingName ) )
            {
                return pRoot;
            }
            if( pRoot->m_pSibling )
            {
                return FindSettingsEntry( SettingName, bCommandLineName, pRoot->m_pSibling );
            }
        }
        return nullptr;
    }

    void ExportSettingsManager::SetDefaultValues()
    {
        size_t dwCount = GetRootCategoryCount();
        for( size_t i = 0; i < dwCount; ++i )
        {
            m_RootCategories[i]->SetDefaultValue( true, true );
        }
    }

    bool ExportSettingsManager::SaveSettings( const CHAR* strFileName )
    {
        const size_t dwBufferSize = 32 * 1024;
        std::unique_ptr<CHAR[]> strBuffer( new CHAR[dwBufferSize] );
        ZeroMemory( strBuffer.get(), dwBufferSize * sizeof( CHAR ) );

        bool bSuccess = MarshalAllSettings( strBuffer.get(), dwBufferSize, true, nullptr );
        if( !bSuccess )
        {
            return false;
        }

        FILE* fp = nullptr;
        fopen_s( &fp, strFileName, "w" );
        if( fp )
        {
            fputs( strBuffer.get(), fp );
            fclose( fp );
            return true;
        }
        else
        {
            return false;
        }
    }

    bool ExportSettingsManager::LoadSettings( const CHAR* strFileName )
    {
        FILE* fp = nullptr;
        fopen_s( &fp, strFileName, "r" );
        if( !fp )
        {
            return false;
        }

        const size_t dwBufferSize = 32 * 1024;
        std::unique_ptr<CHAR[]> strBuffer( new CHAR[dwBufferSize] );
        ZeroMemory( strBuffer.get(), dwBufferSize * sizeof( CHAR ) );

        size_t dwReadBytes = fread( strBuffer.get(), sizeof(CHAR), dwBufferSize, fp );
        fclose( fp );

        if( dwReadBytes == 0 )
        {
            return false;
        }
        else
        {
            assert( dwReadBytes < dwBufferSize );
            strBuffer[dwReadBytes] = '\0';
            bool bSuccess = UnMarshalAllSettings( strBuffer.get() );
            return bSuccess;
        }
    }

    ExportCoreSettings::ExportCoreSettings()
    {
        auto pCategoryPlatform = g_SettingsManager.AddRootCategory( "Target Platform" );
        static const ExportEnumValue EndianEnums[] = {
            { "Big-Endian (PowerPC)", "ppc", 0 },
            { "Little-Endian (Intel)", "intel", 1 },
        };
        g_SettingsManager.AddEnum( pCategoryPlatform, "Data Endianness", "endian", 1, EndianEnums, ARRAYSIZE( EndianEnums ), &bLittleEndian );
        static const ExportEnumValue FLTypes[] = {
            { "Feature Level 12.1", "12.1", 0xc100 /*D3D_FEATURE_LEVEL_12_1*/ },
            { "Feature Level 12.0", "12.0", 0xc000 /*D3D_FEATURE_LEVEL_12_0*/ },
            { "Feature Level 11.1", "11.1", D3D_FEATURE_LEVEL_11_1 },
            { "Feature Level 11.0", "11.0", D3D_FEATURE_LEVEL_11_0 },
            { "Feature Level 10.1", "10.1", D3D_FEATURE_LEVEL_10_1 },
            { "Feature Level 10.0", "10.0", D3D_FEATURE_LEVEL_10_0 },
            { "Feature Level 9.3",  "9.3",  D3D_FEATURE_LEVEL_9_3 },
            { "Feature Level 9.2",  "9.2",  D3D_FEATURE_LEVEL_9_2 },
            { "Feature Level 9.1",  "9.1",  D3D_FEATURE_LEVEL_9_1 },
        };
        g_SettingsManager.AddEnum( pCategoryPlatform, "Target Feature Level", "fl", D3D_FEATURE_LEVEL_9_3, FLTypes, ARRAYSIZE( FLTypes ), (INT*)&dwFeatureLevel );
        pCategoryPlatform->ReverseChildOrder();
        
        auto pCategoryScene = g_SettingsManager.AddRootCategory( "Scene" );
        g_SettingsManager.AddBool( pCategoryScene, "Export Hidden Objects", "exporthiddenobjects", false, &bExportHiddenObjects );
        g_SettingsManager.AddBool( pCategoryScene, "Export Frames", "exportframes", true, &bExportScene );
        g_SettingsManager.AddBool( pCategoryScene, "Export Lights", "exportlights", true, &bExportLights );
        g_SettingsManager.AddFloatBounded( pCategoryScene, "Light Range Scale", "lightrangescale", 1.0f, 0.0f, 1000.0f, &fLightRangeScale );
        g_SettingsManager.AddBool( pCategoryScene, "Export Cameras", "exportcameras", true, &bExportCameras );
        g_SettingsManager.AddBool( pCategoryScene, "Export in Bind Pose", "exportbindpose", true, &bSetBindPoseBeforeSceneParse );
        g_SettingsManager.AddFloatBounded( pCategoryScene, "Export Scene Scale (1.0 = default)", "exportscale", 1.0f, 0.0f, 1000000.f, &fExportScale );
        pCategoryScene->ReverseChildOrder();

        auto pCategoryMeshes = g_SettingsManager.AddRootCategory( "Meshes" );
        g_SettingsManager.AddBool( pCategoryMeshes, "Export Meshes", "exportmeshes", true, &bExportMeshes );
        g_SettingsManager.AddBool( pCategoryMeshes, "Compress Vertex Data", "compressvertexdata", false, &bCompressVertexData );
        g_SettingsManager.AddBool( pCategoryMeshes, "Compute Vertex Tangent Space", "computevertextangents", true, &bComputeVertexTangentSpace );
        g_SettingsManager.AddIntBounded( pCategoryMeshes, "Generate Tangents on Texture Coordinate Index", "tangentsindex", 0, 0, 7, &iTangentSpaceIndex );
        g_SettingsManager.AddBool( pCategoryMeshes, "Export Binormals", "exportbinormals", true, &bExportBinormal );
        static const ExportEnumValue VertexNormalTypes[] = {
            { "FLOAT3 (12 bytes)", "float3", D3DDECLTYPE_FLOAT3 },
            { "UBYTE4N Biased (4 bytes)", "ubyte4n", D3DDECLTYPE_UBYTE4N },
            { "SHORT4N (8 bytes)", "short4n", D3DDECLTYPE_SHORT4N },
            { "FLOAT16_4 (8 bytes)", "float16_4", D3DDECLTYPE_FLOAT16_4 },
        };
        g_SettingsManager.AddEnum( pCategoryMeshes, "Compressed Type for Normals", "compressednormaltype", D3DDECLTYPE_FLOAT16_4, VertexNormalTypes, ARRAYSIZE( VertexNormalTypes ), (INT*)&dwNormalCompressedType );
        g_SettingsManager.AddBool( pCategoryMeshes, "Export Normals", "exportnormals", true, &bExportNormals );
        g_SettingsManager.AddBool( pCategoryMeshes, "Export Vertex Colors", "exportcolors", true, &bExportColors );
        g_SettingsManager.AddBool( pCategoryMeshes, "Force 32 Bit Index Buffers", "force32bitindices", false, &bForceIndex32Format );
        g_SettingsManager.AddIntBounded( pCategoryMeshes, "Max UV Set Count", "maxuvsetcount", 8, 0, 8, &iMaxUVSetCount );
        g_SettingsManager.AddBool( pCategoryMeshes, "Export Bone Weights & Indices for Skinned Meshes", "exportboneweights", true, &bExportSkinWeights );
        g_SettingsManager.AddBool( pCategoryMeshes, "Always Export Bone Weights & Indices for Skinned Meshes (even if no data present)", "forceboneweights", false, &bForceExportSkinWeights );
        g_SettingsManager.AddBool( pCategoryMeshes, "Flip Triangle Winding", "fliptriangles", true, &bFlipTriangles );
        g_SettingsManager.AddBool( pCategoryMeshes, "Apply global transformation (if not animated)", "applyglobaltrans", false, &bApplyGlobalTrans );
        g_SettingsManager.AddBool( pCategoryMeshes, "Invert V Texture Coordinates", "invertvtexcoord", true, &bInvertTexVCoord );
        g_SettingsManager.AddBool( pCategoryMeshes, "Invert Z Coordinates", "flipz", true, &bFlipZ );
        g_SettingsManager.AddString( pCategoryMeshes, "Mesh Name Decoration, applied as a prefix to mesh names", "meshnamedecoration", "Mesh", strMeshNameDecoration );

        auto pCategoryOpt = g_SettingsManager.AddCategory( pCategoryMeshes, "Mesh optimization" );
        g_SettingsManager.AddBool( pCategoryOpt, "Use geometric rather than topographic adjacency", "gadjacency", false, &bGeometricAdjacency );
        g_SettingsManager.AddBool( pCategoryOpt, "Perform vertex cache optimization of meshes", "optimizemeshes", false, &bOptimizeVCache );
        g_SettingsManager.AddIntBounded( pCategoryOpt, "Vertex cache size for optimizemesh", "vcache", 12, 0, 64, &iVcacheSize );
        g_SettingsManager.AddIntBounded( pCategoryOpt, "Strip restart length for optimizemesh", "restart", 7, 0, 64, &iStripRestart );
        g_SettingsManager.AddBool( pCategoryOpt, "Clean up meshes (implied by optimizemeshes)", "cleanmeshes", false, &bCleanMeshes );
        pCategoryOpt->ReverseChildOrder();

        auto pCategoryUVAtlas = g_SettingsManager.AddCategory( pCategoryMeshes, "UV Atlas Generation" );
        g_SettingsManager.AddIntBounded( pCategoryUVAtlas, "Generate UV Atlas on Texture Coordinate Index", "generateuvatlas", -1, -1, 7, &iGenerateUVAtlasOnTexCoordIndex );
        g_SettingsManager.AddFloatBounded( pCategoryUVAtlas, "UV Atlas Max Stretch Factor", "uvatlasstretch", 0.75f, 0.0f, 1.0f, &fUVAtlasMaxStretch );
        g_SettingsManager.AddFloatBounded( pCategoryUVAtlas, "UV Atlas Gutter Size", "uvatlasgutter", 2.5f, 0.0f, 10.0f, &fUVAtlasGutter );
        g_SettingsManager.AddIntBounded( pCategoryUVAtlas, "UV Atlas Texture Size", "uvatlastexturesize", 1024, 64, 4096, &iUVAtlasTextureSize );
        pCategoryUVAtlas->ReverseChildOrder();

        auto pCategorySubD = g_SettingsManager.AddCategory( pCategoryMeshes, "Subdivision Surfaces" );
        g_SettingsManager.AddBool( pCategorySubD, "Convert Poly Meshes to Subdivision Surfaces", "convertmeshtosubd", false, &bConvertMeshesToSubD );
        pCategorySubD->ReverseChildOrder();

        pCategoryMeshes->ReverseChildOrder();

        auto pCategoryMaterials = g_SettingsManager.AddRootCategory( "Materials" );
        g_SettingsManager.AddBool( pCategoryMaterials, "Export Materials", "exportmaterials", true, &bExportMaterials );
        g_SettingsManager.AddString( pCategoryMaterials, "Default Material Name", "defaultmaterialname", "Default", strDefaultMaterialName );
        g_SettingsManager.AddBool( pCategoryMaterials, "Export material colors", "materialcolors", true, &bMaterialColors );
        g_SettingsManager.AddBool( pCategoryMaterials, "Use Texture Compression", "texturecompression", true, &bTextureCompression );
        g_SettingsManager.AddBool( pCategoryMaterials, "Use BGRA texture format", "texturebgra", false, &bBGRvsRGB );
        g_SettingsManager.AddBool( pCategoryMaterials, "Generate Texture Mip Maps", "generatetexturemips", true, &bGenerateTextureMipMaps );
        g_SettingsManager.AddBool( pCategoryMaterials, "Force Texture File Overwriting", "forcetextureoverwrite", true, &bForceTextureOverwrite );
        g_SettingsManager.AddBool( pCategoryMaterials, "Use emissive texture as specular texture", "useemissivetexture", true, &bUseEmissiveTexture );
        g_SettingsManager.AddString( pCategoryMaterials, "Default Diffuse Map Texture Filename", "defaultdiffusemap", "", strDefaultDiffuseMapTextureName );
        g_SettingsManager.AddString( pCategoryMaterials, "Default Normal Map Texture Filename", "defaultnormalmap", "", strDefaultNormalMapTextureName );
        g_SettingsManager.AddString( pCategoryMaterials, "Default Specular Map Texture Filename", "defaultspecmap", "", strDefaultSpecMapTextureName );
        pCategoryMaterials->ReverseChildOrder();

        auto pCategoryAnimation = g_SettingsManager.AddRootCategory( "Animation" );
        g_SettingsManager.AddBool( pCategoryAnimation, "Export Animations", "exportanimations", true, &bExportAnimations );
        g_SettingsManager.AddBool( pCategoryAnimation, "Optimize Animations", "optimizeanimations", true, &bOptimizeAnimations );
        g_SettingsManager.AddBool( pCategoryAnimation, "Rename Animations To Match Output File Name", "renameanimations", true, &bRenameAnimationsToFileName );
        g_SettingsManager.AddIntBounded( pCategoryAnimation, "Animation Baking Sample Count Per Frame", "animsamplecount", 1, 1, 10, &iAnimSampleCountPerFrame );
        g_SettingsManager.AddIntBounded( pCategoryAnimation, "Position Curve Quality", "positioncurvequality", 50, 0, 100, &iAnimPositionExportQuality );
        g_SettingsManager.AddIntBounded( pCategoryAnimation, "Orientation Curve Quality", "orientationcurvequality", 50, 0, 100, &iAnimOrientationExportQuality );
        g_SettingsManager.AddString( pCategoryAnimation, "Animation Root Node Name (default includes all nodes)", "animationrootnode", "", strAnimationRootNodeName );
        pCategoryAnimation->ReverseChildOrder();

        SetDefaultSettings();
    }

    void ExportCoreSettings::SetDefaultSettings()
    {
        g_SettingsManager.SetDefaultValues();
    }
}