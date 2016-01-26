//-------------------------------------------------------------------------------------
// ParseMisc.h
//
// Entry points for parsing less complex scene items in a FBX scene, including nodes,
// cameras, lights, etc.
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

void ParseNode( FbxNode* pNode, ATG::ExportFrame* pParentFrame, DirectX::CXMMATRIX matParentWorld );

void ParseCamera( FbxCamera* pFbxCamera, ATG::ExportFrame* pParentFrame );
void ParseLight( FbxLight* pFbxLight, ATG::ExportFrame* pParentFrame );

void FixupNode( ATG::ExportFrame* pFrame, DirectX::CXMMATRIX matParentWorld );

typedef std::unordered_map<FbxNode*,FbxMatrix> PoseMap;
extern PoseMap g_BindPoseMap;
extern bool g_bBindPoseFixupRequired;

