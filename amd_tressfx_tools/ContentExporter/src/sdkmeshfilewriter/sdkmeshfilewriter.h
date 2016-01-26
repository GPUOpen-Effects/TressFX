//-------------------------------------------------------------------------------------
// SDKMeshFileWriter.h
//
// Entry point for writing SDKMESH files.  This file writer takes data from the
// ExportScene stored in a global variable (g_pScene).
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

class ExportManifest;

bool WriteSDKMeshFile( const CHAR* strFileName, ExportManifest* pManifest );

}
