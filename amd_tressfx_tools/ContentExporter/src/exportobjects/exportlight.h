//-------------------------------------------------------------------------------------
// ExportLight.h
//
// Classes to represent various types of lights found in a scene.
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
    class ExportLight : public ExportBase
    {
    public:
        ExportLight( ExportString strName );
        enum LightType
        {
            LT_AMBIENT = 0,
            LT_DIRECTIONAL,
            LT_POINT,
            LT_SPOT,
        };
        enum LightFalloff
        {
            LF_NONE = 0,
            LF_LINEAR,
            LF_SQUARED
        };

        LightType           Type;
        DirectX::XMFLOAT3   LocalPosition;
        DirectX::XMFLOAT4   Color;
        DirectX::XMFLOAT3   Direction;
        float               fRange;
        LightFalloff        Falloff;
        LightFalloff        SpotFalloff;
        float               fInnerAngle;
        float               fOuterAngle;
    };
}
