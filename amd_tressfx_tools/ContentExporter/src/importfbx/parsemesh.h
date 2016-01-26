//-------------------------------------------------------------------------------------
// ParseMesh.h
//
// Entry points for FBX mesh parsing.
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

void ParseMesh( FbxNode* pNode, FbxMesh* pFbxMesh, ATG::ExportFrame* pParentFrame, bool bSubDProcess = false, const CHAR* strSuffix = nullptr );
void ParseSubDiv( FbxNode* pNode, FbxSubDiv* pFbxSubD, ATG::ExportFrame* pParentFrame );
