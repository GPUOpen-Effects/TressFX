//-------------------------------------------------------------------------------------
// ExportSettings.h
//
// Generic data structures for export settings, including the settings themselves and
// data structures for annotating, organizing, and modifying the settings variables.
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
    static const size_t SETTINGS_STRING_LENGTH = 256;

    class ExportVariant
    {
    public:
        enum VariantType
        {
            VT_NONE,
            VT_BOOL,
            VT_FLOAT,
            VT_INT,
            VT_STRING            
        };
    public:
        ExportVariant()
            : m_Type( VT_NONE )
        { }
        union
        {
            float           m_fValue;
            INT             m_iValue;
            bool            m_bValue;
        };
        ExportMutableString m_strValue;
        VariantType         m_Type;
    };

    struct ExportEnumValue
    {
        const CHAR* strLabel;
        const CHAR* strCommandLine;
        INT         iValue;
    };

    class ExportSettingsEntry
    {
    public:
        enum ControlType
        {
            CT_CATEGORY,
            CT_CHECKBOX,
            CT_BOUNDEDINTSLIDER,
            CT_BOUNDEDFLOATSLIDER,
            CT_STRING,
            CT_ENUM,
        };
    public:
        ExportString        m_DisplayName;
        ExportString        m_SettingName;
        ExportString        m_CommandLineOptionName;
        ControlType         m_Type;

        ExportVariant       m_DefaultValue;
        ExportVariant       m_CurrentValue;
        void*               m_pLinkedCurrentValue;

        ExportVariant       m_MinValue;
        ExportVariant       m_MaxValue;

        const ExportEnumValue*  m_pEnumValues;
        DWORD                   m_dwEnumValueCount;

        ExportSettingsEntry*    m_pFirstChild;
        ExportSettingsEntry*    m_pSibling;
    public:
        ExportSettingsEntry();
        ~ExportSettingsEntry();

        void SetDefaultValue( bool bSetChildren = false, bool bSetSiblings = false );
        float GetValueFloat() const;
        bool GetValueBool() const;
        INT GetValueInt() const;
        const CHAR* GetValueString() const;
        void SetValue( float fValue );
        void SetValue( INT iValue );
        void SetValue( const CHAR* strValue );

        void AddChild( ExportSettingsEntry* pNewChild )
        {
            pNewChild->m_pSibling = m_pFirstChild;
            m_pFirstChild = pNewChild;
        }
        void ReverseChildOrder();
        void CreateSettingName();

    protected:
        void* GetCurrentValue()
        {
            return m_pLinkedCurrentValue ? m_pLinkedCurrentValue : &m_CurrentValue;
        }
        const void* GetCurrentValue() const
        {
            return m_pLinkedCurrentValue ? m_pLinkedCurrentValue : &m_CurrentValue;
        }
    };

    class ExportSettingsManager
    {
    public:
        ExportSettingsManager() {}
        ~ExportSettingsManager();

        ExportSettingsEntry* AddRootCategory( ExportString Caption );
        ExportSettingsEntry* AddCategory( ExportSettingsEntry* pParentCategory, ExportString Caption );
        bool                 DeleteRootCategoryAndChildren( ExportSettingsEntry* pCategory );

        ExportSettingsEntry* AddBool( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, bool bDefaultValue, bool* pLinkedValue = nullptr );
        ExportSettingsEntry* AddFloatBounded( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, float fDefaultValue, float fMin, float fMax, float* pLinkedValue = nullptr );
        ExportSettingsEntry* AddIntBounded( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, INT iDefaultValue, INT iMin, INT iMax, INT* pLinkedValue = nullptr );
        ExportSettingsEntry* AddString( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, const CHAR* strDefaultValue, CHAR* pLinkedValue = nullptr );
        ExportSettingsEntry* AddEnum( ExportSettingsEntry* pCategory, ExportString Caption, ExportString CmdLine, INT iDefaultValue, const ExportEnumValue* pEnumValues, DWORD dwEnumValueCount, INT* pLinkedValue = nullptr );

        void SetDefaultValues();

        size_t GetRootCategoryCount() const { return m_RootCategories.size(); }
        ExportSettingsEntry* GetRootCategory( size_t dwIndex ) { return m_RootCategories[ dwIndex ]; }

        bool LoadSettings( const CHAR* strFileName );
        bool SaveSettings( const CHAR* strFileName );

        bool MarshalAllSettings( CHAR* strDestBuffer, DWORD dwBufferSize, bool bNewLines, ExportSettingsEntry* pRoot = nullptr );
        bool UnMarshalAllSettings( const CHAR* strSrcBuffer );

    protected:
        ExportSettingsEntry* FindSettingsEntry( ExportString SettingName, bool bCommandLineName, ExportSettingsEntry* pRoot );

        typedef std::vector<ExportSettingsEntry*> SettingsEntryArray;
        SettingsEntryArray  m_RootCategories;
    };

    extern ExportSettingsManager        g_SettingsManager;

    class ExportCoreSettings
    {
    public:
        ExportCoreSettings();
        void SetDefaultSettings();
    public:
        bool        bFlipTriangles;
        bool        bInvertTexVCoord;
        bool        bFlipZ;
        bool        bApplyGlobalTrans;
        bool        bExportScene;
        bool        bExportLights;
        bool        bExportCameras;
        bool        bExportMaterials;
        bool        bExportMeshes;
        bool        bExportHiddenObjects;
        bool        bExportAnimations;
        BOOL        bLittleEndian;
        DWORD       dwFeatureLevel;
        bool        bExportNormals;
        bool        bExportColors;
        bool        bForceIndex32Format;
        INT         iMaxUVSetCount;
        bool        bExportSkinWeights;
        bool        bForceExportSkinWeights;
        bool        bComputeVertexTangentSpace;
        INT         iTangentSpaceIndex;
        bool        bExportBinormal;
        bool        bSetBindPoseBeforeSceneParse;
        INT         iAnimSampleCountPerFrame;
        INT         iAnimPositionExportQuality;
        INT         iAnimOrientationExportQuality;
        bool        bRenameAnimationsToFileName;
        CHAR        strDefaultMaterialName[SETTINGS_STRING_LENGTH];
        CHAR        strDefaultDiffuseMapTextureName[SETTINGS_STRING_LENGTH];
        CHAR        strDefaultNormalMapTextureName[SETTINGS_STRING_LENGTH];
        CHAR        strDefaultSpecMapTextureName[SETTINGS_STRING_LENGTH];
        bool        bCompressVertexData;
        DWORD       dwNormalCompressedType;
        DWORD       dwTexCoordCompressedType;
        DWORD       dwPositionCompressedType;
        bool        bTextureCompression;
        bool        bMaterialColors;
        bool        bBGRvsRGB;
        bool        bGenerateTextureMipMaps;
        bool        bForceTextureOverwrite;
        bool        bUseEmissiveTexture;
        bool        bConvertMeshesToSubD;
        bool        bGeometricAdjacency;
        INT         iGenerateUVAtlasOnTexCoordIndex;
        float       fUVAtlasMaxStretch;
        float       fUVAtlasGutter;
        INT         iUVAtlasTextureSize;
        float       fLightRangeScale;
        CHAR        strMeshNameDecoration[SETTINGS_STRING_LENGTH];
        CHAR        strAnimationRootNodeName[SETTINGS_STRING_LENGTH];
        bool        bOptimizeAnimations;
        bool        bCleanMeshes;
        bool        bOptimizeVCache;
        INT         iVcacheSize;
        INT         iStripRestart;
        float       fExportScale;
    };

    extern ExportCoreSettings       g_ExportCoreSettings;
}