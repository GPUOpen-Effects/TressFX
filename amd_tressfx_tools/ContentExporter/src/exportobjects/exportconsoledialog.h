//-------------------------------------------------------------------------------------
// ExportConsoleDialog.h
//
// Implements a Win32 console dialog UI for exporter output spew.
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

#include "ExportDialogs.h"

namespace ATG
{
    class ExportConsoleDialog : public ILogListener, public ExportProgress, public ExportDialogBase
    {
    public:
        ExportConsoleDialog();

        virtual LRESULT OnCommand( WORD wNotifyCode, WORD idCtrl, HWND hwndCtrl ) override;
        virtual LRESULT OnInitDialog( HWND hwndFocusCtrl ) override;
        virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam ) override;

        virtual void LogMessage( const CHAR* strMessage ) override;
        virtual void LogWarning( const CHAR* strMessage ) override;
        virtual void LogError( const CHAR* strMessage ) override;
        virtual void LogCommand( DWORD dwCommand, void* pData ) override;

        virtual void Initialize( const CHAR* strTitle ) override;
        virtual void Terminate() override;
        virtual void SetCaption( const CHAR* strCaption ) override;
        virtual void StartNewTask( const CHAR* strCaption, float fTaskPercentOfWhole ) override;
        virtual void SetProgress( float fTaskRelativeProgress ) override;

        void ConsolePrint( COLORREF rgb, const CHAR* strText );
        void ConsoleNewline();
        void ConsoleClear();

        static UINT WINAPI ThreadEntry( void* pData );

    protected:
        void RecordWarning( const CHAR* strMessage );
        void RecordError( const CHAR* strMessage );
        void PrintWarningsAndErrors();
        void ClearWarningsAndErrors();

    protected:
        HWND    m_hRichTextBox;
        HWND    m_hProgressText;
        HWND    m_hProgressBar;
        HWND    m_hOKButton;
        float   m_fCurrentTaskMin;
        float   m_fCurrentTaskSize;
    };
}