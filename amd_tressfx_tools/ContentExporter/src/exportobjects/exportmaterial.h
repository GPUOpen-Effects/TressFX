//-------------------------------------------------------------------------------------
// ExportMaterial.h
//
// Data structures representing parameterized materials (surface shaders).
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

#include "ExportMaterialDatabase.h"

namespace ATG
{

class ExportMaterialParameter
{
public:
    enum ExportMaterialParameterFlags
    {
        EMPF_NONE = 0,
        EMPF_BUMPMAP = 1,
        EMPF_DIFFUSEMAP = 2,
        EMPF_NORMALMAP = 4,
        EMPF_SPECULARMAP = 8,
        EMPF_ALPHACHANNEL = 16,
        EMPF_AOMAP = 32,
    };
    ExportMaterialParameter()
    {
        ZeroMemory( this, sizeof( ExportMaterialParameter ) );
    }
    ExportString                    Name;
    ExportMaterialParameterType     ParamType;
    ExportString                    Hint;
    bool                            bInstanceParam;
    ExportString                    ValueString;
    float                           ValueFloat[16];
    INT                             ValueInt;
    DWORD                           Flags;
};
typedef std::list<ExportMaterialParameter> MaterialParameterList;

class ExportMaterial :
    public ExportBase
{
public:
    ExportMaterial();
    ExportMaterial( ExportString name );
    ~ExportMaterial();

    void SetMaterialDefinition( const ExportMaterialDefinition* pDef ) { m_pMaterialDefinition = pDef; }
    const ExportMaterialDefinition* GetMaterialDefinition() const { return m_pMaterialDefinition; }

    void SetDefaultMaterialName( const ExportString strDefaultName ) { m_DefaultMaterialName = strDefaultName; }
    ExportString GetDefaultMaterialName() const { return m_DefaultMaterialName; }

    void AddParameter( const ExportMaterialParameter& Param ) { m_Parameters.push_back( Param ); }
    MaterialParameterList* GetParameterList() { return &m_Parameters; }
    size_t GetParameterCount() const { return m_Parameters.size(); }
    ExportMaterialParameter* FindParameter( const ExportString strName );

    void SetTransparent( bool bTransparent ) { m_bTransparent = bTransparent; }
    bool IsTransparent() const { return m_bTransparent; }

    static ExportString GetDefaultDiffuseMapTextureName();
    static ExportString GetDefaultNormalMapTextureName();
    static ExportString GetDefaultSpecularMapTextureName();

protected:
    const ExportMaterialDefinition*     m_pMaterialDefinition;
    ExportString                        m_DefaultMaterialName;
    MaterialParameterList               m_Parameters;
    bool                                m_bTransparent;
};

};

