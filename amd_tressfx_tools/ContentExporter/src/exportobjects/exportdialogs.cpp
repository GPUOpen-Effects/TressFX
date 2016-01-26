//-------------------------------------------------------------------------------------
// ExportDialogs.cpp
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

#include "ExportDialogs.h"
#include "ExportConsoleDialog.h"
#include "ExportSettingsDialog.h"

namespace ATG
{
    const CHAR* g_strTitle = nullptr;
    HWND g_hParentWindow = nullptr;
    HINSTANCE g_hInstance = nullptr;
    HMODULE g_hRichEdit = nullptr;

    ExportConsoleDialog g_ConsoleDlg;
    ExportSettingsDialog g_SettingsDlg;

    UINT WINAPI ExportConsoleDialog::ThreadEntry( void* pData )
    {
        auto pDlg = reinterpret_cast<ExportConsoleDialog*>( pData );
        pDlg->DoModal( g_hInstance, g_hParentWindow );
        return 0;
    }

    UINT WINAPI ExportSettingsDialog::ThreadEntry( void* pData )
    {
        auto pDlg = reinterpret_cast<ExportSettingsDialog*>( pData );
        pDlg->DoModal( g_hInstance, g_hParentWindow );
        return 0;
    }

    void InitializeExportDialogs( const CHAR* strTitle, HWND hParentWindow, HINSTANCE hInst )
    {
        g_strTitle = strTitle;
        g_hInstance = hInst;
        g_hParentWindow = hParentWindow;

        // Pull in common and rich edit controls
        INITCOMMONCONTROLSEX ICEX;
        ICEX.dwSize = sizeof( INITCOMMONCONTROLSEX );
        ICEX.dwICC = ICC_WIN95_CLASSES | ICC_COOL_CLASSES | ICC_USEREX_CLASSES;
        InitCommonControlsEx( &ICEX );
        InitCommonControls();
        g_hRichEdit = LoadLibrary( TEXT( "Riched32.dll" ) );
        assert( g_hRichEdit != nullptr );

        ExportLog::AddListener( &g_ConsoleDlg );
        g_pProgress = &g_ConsoleDlg;

        const DWORD dwStackSize = 8192;

        _beginthreadex( nullptr, dwStackSize, ExportConsoleDialog::ThreadEntry, &g_ConsoleDlg, 0, nullptr );
        _beginthreadex( nullptr, dwStackSize, ExportSettingsDialog::ThreadEntry, &g_SettingsDlg, 0, nullptr );
    }

    void TerminateExportDialogs()
    {
        FreeLibrary( g_hRichEdit );
    }

    void ShowConsoleDialog()
    {
        g_ConsoleDlg.Show();
    }

    bool ShowSettingsDialog( bool bModal )
    {
        g_SettingsDlg.Show();
        if( bModal )
        {
            while( g_SettingsDlg.GetDialogState() == ExportSettingsDialog::DS_VISIBLE ) { Sleep(0); }
            if( g_SettingsDlg.GetDialogState() != ExportSettingsDialog::DS_HIDDEN_OK )
                return false;
        }
        return true;
    }

    void ExportDialogBase::Show()
    {
        ShowWindow( m_hwnd, SW_SHOWNORMAL );
        SetWindowPos( m_hwnd, HWND_TOP, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE );
        UpdateWindow( m_hwnd );
    }

    void ExportDialogBase::Hide()
    {
        ShowWindow( m_hwnd, SW_HIDE );
    }
}