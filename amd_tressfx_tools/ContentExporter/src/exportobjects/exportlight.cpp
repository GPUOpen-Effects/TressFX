//-------------------------------------------------------------------------------------
// ExportLight.cpp
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
#include "ExportLight.h"

namespace ATG
{

    ExportLight::ExportLight( ExportString strName )
      : ExportBase( strName ),
        Type( LT_AMBIENT ),
        LocalPosition( 0, 0, 0 ),
        Color( 1, 1, 1, 1 ),
        Direction( 0, 0, -1 ),
        fRange( 0 ),
        Falloff( LF_NONE ),
        fInnerAngle( 0 ),
        fOuterAngle( 0 )
    {
    }

}

