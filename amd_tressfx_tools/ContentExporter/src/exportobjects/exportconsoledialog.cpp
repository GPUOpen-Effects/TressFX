//-------------------------------------------------------------------------------------
// ExportConsoleDialog.cpp
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
#include "ExportConsoleDialog.h"
#include "ExportResources.h"

namespace ATG
{
    extern const CHAR* g_strTitle;
    extern HWND g_hParentWindow;
    extern HINSTANCE g_hInstance;

    ExportConsoleDialog::ExportConsoleDialog()
    {
        m_strTemplate = "ExportConsole";
    }

    LRESULT ExportConsoleDialog::OnInitDialog( HWND hwndFocusCtrl )
    {
        UNREFERENCED_PARAMETER( hwndFocusCtrl );
        m_hRichTextBox = GetDlgItem( m_hwnd, IDC_RICHTEXT );
        m_hProgressBar = GetDlgItem( m_hwnd, IDC_EXPORTPROGRESS );
        m_hProgressText = GetDlgItem( m_hwnd, IDC_EXPORTTEXT );
        m_hOKButton = GetDlgItem( m_hwnd, IDOK );
        SetWindowText( m_hwnd, g_strTitle );
        SetWindowText( m_hProgressText, g_strTitle );

        SendMessage( m_hwnd, WM_SIZE, 0, 0 );
        SetTimer( m_hwnd, 1, 0, nullptr );

        return false;
    }

    LRESULT ExportConsoleDialog::OnCommand( WORD wNotifyCode, WORD idCtrl, HWND hwndCtrl )
    {
        UNREFERENCED_PARAMETER( hwndCtrl );
        switch (idCtrl)
        {
        case IDCANCEL: // the little x in the upper right?
        case IDOK:
        case IDCLOSE:
            switch (wNotifyCode)
            {
            case BN_CLICKED:
                Hide();
                return true;
            }
            break;
        }
        return false;
    }

    LRESULT ExportConsoleDialog::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
    {
        UNREFERENCED_PARAMETER( wParam );
        UNREFERENCED_PARAMETER( lParam );

        switch( uMsg )
        {
        case WM_TIMER:
            Hide();
            KillTimer( m_hwnd, wParam );
            return false;
        case WM_SIZE:
            {
                GridLayout Layout( 3, 2 );
                Layout.SetRowSpec( 0, 1.0f );
                Layout.SetRowSpec( 1, 30 );
                Layout.SetRowSpec( 2, 30 );
                Layout.SetColumnSpec( 0, 1 );
                Layout.SetColumnSpec( 1, 100 );

                const DWORD dwOuterBorder = 4;
                const DWORD dwInnerBorder = 4;
                Layout.SetClientRect( m_hwnd, dwOuterBorder );
                Layout.PlaceWindow( m_hRichTextBox, 0, 0, 1, 2, dwInnerBorder );
                Layout.PlaceWindow( m_hProgressText, 1, 0, 1, 2, dwInnerBorder );
                Layout.PlaceWindow( m_hProgressBar, 2, 0, 1, 1, dwInnerBorder );
                Layout.PlaceWindow( m_hOKButton, 2, 1, 1, 1, dwInnerBorder );
                return false;
            }
        }
        return false;
    }

    void ExportConsoleDialog::ConsolePrint( COLORREF rgb, const CHAR* strText )
    {
        CHARRANGE cr = { -1, -2 };

        if( rgb != CLR_INVALID )
        {
            // Set whatever colors, etc. they want
            CHARFORMAT cf = {0};
            cf.cbSize = sizeof(cf);
            cf.dwMask = CFM_COLOR;
            cf.dwEffects = 0;
            cf.crTextColor = rgb;
            SendDlgItemMessage( m_hwnd, IDC_RICHTEXT, EM_SETCHARFORMAT, SCF_SELECTION, reinterpret_cast<LPARAM>( &cf ) );
        }

        // Move the selection to the end
        SendDlgItemMessage( m_hwnd, IDC_RICHTEXT, EM_EXSETSEL, 0, reinterpret_cast<LPARAM>( &cr ) );

        // Add the text and scroll it into view
        SendDlgItemMessage( m_hwnd, IDC_RICHTEXT, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>( strText ) );
        SendDlgItemMessage( m_hwnd, IDC_RICHTEXT, EM_SCROLLCARET, 0, 0L );
    }

    void ExportConsoleDialog::ConsoleNewline()
    {
        ConsolePrint( 0, "\n" );
        SendMessage( m_hRichTextBox, EM_LINESCROLL, 0, 1 );
    }

    void ExportConsoleDialog::ConsoleClear()
    {
        SETTEXTEX stx;
        stx.codepage = CP_ACP;
        stx.flags = ST_DEFAULT;
        SendDlgItemMessage( m_hwnd, IDC_RICHTEXT, EM_SETTEXTEX, reinterpret_cast<WPARAM>( &stx ), reinterpret_cast<LPARAM>( "" ) );
    }

    void ExportConsoleDialog::LogCommand( DWORD dwCommand, void* pData )
    {
        UNREFERENCED_PARAMETER( pData );

        switch( dwCommand )
        {
        case ExportLog::ELC_CLEAR:
            ConsoleClear();
            Show();
            break;
        case ExportLog::ELC_STARTEXPORT:
        case ExportLog::ELC_ENDEXPORT:
            Show();
            break;
        }
    }

    void ExportConsoleDialog::LogMessage( const CHAR* strMessage )
    {
        ConsolePrint( RGB(0, 0, 0), strMessage );
        ConsoleNewline();
    }

    void ExportConsoleDialog::LogWarning( const CHAR* strMessage )
    {
        ConsolePrint( RGB(255, 128, 0), strMessage );
        ConsoleNewline();
    }

    void ExportConsoleDialog::LogError( const CHAR* strMessage )
    {
        ConsolePrint( RGB(255, 0, 0), strMessage );
        ConsoleNewline();
    }

    void ExportConsoleDialog::Initialize( const CHAR *strTitle )
    {
        UNREFERENCED_PARAMETER( strTitle );
        m_fCurrentTaskMin = 0;
        m_fCurrentTaskSize = 0;

        SendDlgItemMessage( m_hwnd, IDC_EXPORTPROGRESS, PBM_SETRANGE32, 0, 1000 );
        SendDlgItemMessage( m_hwnd, IDC_EXPORTPROGRESS, PBM_SETPOS, 0, 0 );
        SetCaption( "" );
    }

    void ExportConsoleDialog::Terminate()
    {
        SetCaption( "Export complete." );
        SendDlgItemMessage( m_hwnd, IDC_EXPORTPROGRESS, PBM_SETPOS, 1000, 0 );
    }

    void ExportConsoleDialog::SetCaption( const CHAR* strCaption )
    {
        SetWindowText( m_hProgressText, strCaption );
    }

    void ExportConsoleDialog::StartNewTask( const CHAR* strCaption, float fTaskPercentOfWhole )
    {
        SetCaption( strCaption );
        m_fCurrentTaskMin += m_fCurrentTaskSize;
        m_fCurrentTaskSize = fTaskPercentOfWhole;
        SetProgress( 0 );
    }

    void ExportConsoleDialog::SetProgress( float fTaskRelativeProgress )
    {
        if( fTaskRelativeProgress > 1 )
            fTaskRelativeProgress = 1;
        else if( fTaskRelativeProgress < 0 )
            fTaskRelativeProgress = 0;
        float fAbsoluteProgress = ( fTaskRelativeProgress * m_fCurrentTaskSize ) + m_fCurrentTaskMin;
        INT iProgress = static_cast<INT>( fAbsoluteProgress * 1000.0f );
        SendDlgItemMessage( m_hwnd, IDC_EXPORTPROGRESS, PBM_SETPOS, iProgress, 0 );
    }

}