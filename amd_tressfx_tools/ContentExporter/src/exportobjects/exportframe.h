//-------------------------------------------------------------------------------------
// ExportFrame.h
//
// A class representing a node in a scenegraph, containing a local transform, children,
// and a parent pointer.
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

class ExportModel;
class ExportLight;
class ExportCamera;

class ExportFrame :
    public ExportBase
{
public:
    ExportFrame();
    ExportFrame( ExportString name );
    virtual ~ExportFrame();

    void AddChild( ExportFrame* pFrame ) { m_vChildren.push_back( pFrame ); }
    void RemoveChild( ExportFrame* pFrame );
    size_t GetChildCount() const { return m_vChildren.size(); }
    ExportFrame* GetChildByIndex( size_t uIndex ) { return m_vChildren[ uIndex ]; }

    size_t GetModelCount() const { return m_vModels.size(); }
    void AddModel( ExportModel* pModel ) { m_vModels.push_back( pModel ); }
    ExportModel* GetModelByIndex( size_t uIndex ) { return m_vModels[ uIndex ]; }

    size_t GetLightCount() const { return m_vLights.size(); }
    void AddLight( ExportLight* pLight ) { m_vLights.push_back( pLight ); }
    ExportLight* GetLightByIndex( size_t uIndex ) { return m_vLights[ uIndex ]; }

    size_t GetCameraCount() const { return m_vCameras.size(); }
    void AddCamera( ExportCamera* pCamera ) { m_vCameras.push_back( pCamera ); }
    ExportCamera* GetCameraByIndex( size_t uIndex ) { return m_vCameras[ uIndex ]; }

    ExportFrame* FindFrameByName( ExportString name );
    ExportFrame* FindFrameByDCCObject( void* pObject );

    ExportTransform& Transform() { return m_Transform; }
    void NormalizeTransform();
protected:
    ExportTransform                 m_Transform;
    std::vector< ExportFrame* >     m_vChildren;
    std::vector< ExportModel* >     m_vModels;
    std::vector< ExportLight* >     m_vLights;
    std::vector< ExportCamera* >    m_vCameras;
};

};

