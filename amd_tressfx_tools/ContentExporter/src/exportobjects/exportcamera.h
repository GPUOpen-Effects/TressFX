//-------------------------------------------------------------------------------------
// ExportCamera.h
//
// A data structure representing a camera.
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
    class ExportCamera : public ExportBase
    {
    public:
        ExportCamera( ExportString strName );

        DirectX::XMFLOAT3   LocalPosition;
        DirectX::XMFLOAT3   Direction;
        DirectX::XMFLOAT3   Up;
        float               fFocalLength;
        float               fFieldOfView;
        float               fNearClip;
        float               fFarClip;
    };
}
