//-------------------------------------------------------------------------------------
// ExportMaterialDatabase.h
//
// Data structures representing a list of custom material types that can be exposed
// into the DCC package and assigned to objects.
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

#include "ExportBase.h"

namespace ATG
{

    enum ExportMaterialParameterType
    {
        MPT_STRING = 0,
        MPT_BOOL,
        MPT_INTEGER,
        MPT_FLOAT,
        MPT_FLOAT2,
        MPT_FLOAT3,
        MPT_FLOAT4,
        MPT_TEXTURE2D,
        MPT_TEXTURECUBE,
        MPT_TEXTUREVOLUME
    };

    static const CHAR* ExportMaterialParameterTypeNames[] = 
    {
        "string",
        "bool",
        "integer",
        "float",
        "float2",
        "float3",
        "float4",
        "texture2d",
        "texturecube",
        "texturevolume"
    };

    struct ExportMaterialParameterDefinition
    {
        ExportString                strName;
        ExportString                strDisplayName;
        ExportString                strDescription;
        ExportMaterialParameterType ParamType;
        ExportString                strDisplayHint;
        ExportString                strDefaultValue;
        ExportString                strLoaderHint;
        bool                        bVisibleInTool;
        bool                        bExportToContentFile;
        bool                        bDetectAlpha;
    };
    typedef std::vector<ExportMaterialParameterDefinition*> ExportMaterialParameterDefinitionVector;

    class ExportMaterialDefinition
    {
    public:
        ~ExportMaterialDefinition();
        ExportString        strName;
        ExportString        strDescription;
        ExportMaterialParameterDefinitionVector     Parameters;
    };
    typedef std::vector<ExportMaterialDefinition*> ExportMaterialDefinitionVector;

    class MaterialDatabaseReader : public ISAXCallback
    {
    public:
        MaterialDatabaseReader()
            : m_pCurrentMaterial( nullptr ),
            m_pCurrentParam( nullptr )
        { }
        virtual HRESULT  StartDocument() override { return S_OK; }
        virtual HRESULT  EndDocument() override { return S_OK; }

        virtual HRESULT  ElementBegin( const WCHAR* strName, UINT NameLen, 
            const XMLAttribute *pAttributes, UINT NumAttributes ) override;
        virtual HRESULT  ElementContent( const WCHAR *strData, UINT DataLen, bool More ) override;
        virtual HRESULT  ElementEnd( const WCHAR *strName, UINT NameLen ) override;

        virtual HRESULT  CDATABegin( ) override { return S_OK; }
        virtual HRESULT  CDATAData( const WCHAR *strCDATA, UINT CDATALen, bool bMore ) override { return S_OK; }
        virtual HRESULT  CDATAEnd( ) override { return S_OK; }

        virtual void     Error( HRESULT hError, const CHAR *strMessage ) override;
    protected:
        void             ParseAttributes( const XMLAttribute* pAttributes, size_t dwAttributeCount );
        const WCHAR*     FindAttribute( const WCHAR* strName );
        void             ProcessElementBeginContent();
        void             ProcessElementEnd();
    protected:
        struct ElementAttribute
        {
            WCHAR   strName[256];
            WCHAR   strValue[256];
        };
        typedef std::vector<ElementAttribute> ElementAttributeVector;

        WCHAR                   m_strCurrentElementName[256];
        bool                    m_bCurrentElementEndTag;
        ElementAttributeVector  m_CurrentElementAttributes;

        ExportMaterialDefinition*               m_pCurrentMaterial;
        ExportMaterialParameterDefinition*      m_pCurrentParam;
    };

    class ExportMaterialDatabase
    {
    public:
        static void Clear();
        static bool Initialize( const CHAR* strFileName );
        static const CHAR* GetDatabaseFileName();
        static size_t GetMaterialCount();
        static const ExportMaterialDefinition* GetMaterial( size_t dwIndex );
        static const ExportMaterialDefinition* FindMaterial( ExportString strName );
    };

}
