//-------------------------------------------------------------------------------------
// stdafx.h
//
// Precompiled header for the ExportObjects project.
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

#pragma warning( disable : 4100 4481 )

#define NOMINMAX
#include <stdio.h>
#include <tchar.h>
#include <windows.h>
#include <WindowsX.h>
#include <list>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <memory>
#include <commctrl.h>
#include <richedit.h>
#include <process.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>
#include <dxgiformat.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXPackedVector.h>

#include "ExportXmlParser.h"
#include "ExportPath.h"
#include "ExportMaterial.h"
#include "ExportObjects.h"
