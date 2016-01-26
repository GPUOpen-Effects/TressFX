//-------------------------------------------------------------------------------------
// ExportProgress.h
//
// A DCC-agnostic system for reporting progress of the export operation.
// The export front-end implements a subclass of ExportProgress that manipulates the
// DCC user interface, and sets up the g_pProgress global pointer so the DCC-agnostic
// code can report progress updates.
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
    class ExportProgress
    {
    public:
        virtual void Initialize( const CHAR* strTitle );
        virtual void Terminate();

        virtual void SetCaption( const CHAR* strCaption );
        virtual void StartNewTask( const CHAR* strCaption, float fTaskPercentOfWhole );
        virtual void SetProgress( float fTaskRelativeProgress );
    };

    extern ExportProgress* g_pProgress;
}

