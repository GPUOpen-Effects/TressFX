//-------------------------------------------------------------------------------------
// ExportProgress.cpp
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
#include "exportprogress.h"

namespace ATG
{
    void ExportProgress::Initialize( const CHAR* strTitle )
    {
    }

    void ExportProgress::Terminate()
    {
    }

    void ExportProgress::StartNewTask( const CHAR* strCaption, float fTaskPercentOfWhole )
    {
    }

    void ExportProgress::SetCaption( const CHAR* strCaption )
    {
    }

    void ExportProgress::SetProgress( float fTaskRelativeProgress )
    {
    }

    ExportProgress g_NullProgressBar;
    ExportProgress* g_pProgress = &g_NullProgressBar;
}

