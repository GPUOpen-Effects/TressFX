//-------------------------------------------------------------------------------------
// XATGFileWriter.h
//  
// Writes files in the XATG file format, from data stored in the export objects hierarchy.
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

bool WriteXATGFile( const CHAR* strFileName, ExportManifest* pManifest );

void BundleTextures();

struct XATGExportSettings
{
    bool    bBundleTextures;
    bool    bUseExistingBundle;
    bool    bBinaryBlobExport;
};

void XATGInitializeSettings();

}
