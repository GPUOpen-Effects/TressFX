//-------------------------------------------------------------------------------------
// FBXImportMain.h
//
// Entry points for FBX scene parsing.
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

class FBXTransformer : public ATG::IDCCTransformer
{
public:
    FBXTransformer()
        : m_fUnitScale( 1.0f ),
          m_bFlipZ( true )
    { }

    void Initialize( FbxScene* pScene );

    virtual void TransformMatrix( DirectX::XMFLOAT4X4* pDestMatrix, const DirectX::XMFLOAT4X4* pSrcMatrix ) const override;
    virtual void TransformPosition( DirectX::XMFLOAT3* pDestPosition, const DirectX::XMFLOAT3* pSrcPosition ) const override;
    virtual void TransformDirection( DirectX::XMFLOAT3* pDestDirection, const DirectX::XMFLOAT3* pSrcDirection ) const override;
    virtual float TransformLength( float fInputLength ) const override;

    // Sets unit scale for exporting all geometry - works with characters too.
    void SetUnitScale( const float fScale )
    {
        m_fUnitScale = fScale;
    }

    void SetZFlip( const bool bFlip )
    {
        m_bFlipZ = bFlip;
    }

protected:
    float m_fUnitScale;
    bool  m_bMaxConversion;
    bool  m_bFlipZ;
};

class FBXImport
{
public:
    static HRESULT Initialize();
    static void ClearScene();

    static HRESULT ImportFile( const CHAR* strFileName );

private:
};
