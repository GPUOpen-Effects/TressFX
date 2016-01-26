//-------------------------------------------------------------------------------------
// ExportSettingsDialog.cpp
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
#include "ExportSettingsDialog.h"
#include "ExportResources.h"

#define IDC_DYNAMICCONTROL   4000
#define IDC_DYNAMICDROPLIST   4001

namespace ATG
{
    extern const CHAR* g_strTitle;
    extern HWND g_hParentWindow;
    extern HINSTANCE g_hInstance;

    ExportSettingsDialog::ExportSettingsDialog()
    {
        m_strTemplate = "ExportSettings";
        m_pCurrentCategory = nullptr;
        m_DialogState = DS_HIDDEN_UNKNOWN;
        m_bControlDataUpdate = false;
    }

    void ExportSettingsDialog::Show()
    {
        ExportDialogBase::Show();
        m_DialogState = DS_VISIBLE;
        TreeView_SelectItem( m_hCategoryTree, nullptr );
        TreeView_SelectItem( m_hCategoryTree, TreeView_GetRoot( m_hCategoryTree ) );
    }

    LRESULT CALLBACK ExportSettingsDialog::ScrollPaneWndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
    {
        switch( message )
        {
        case WM_CREATE:
        case WM_DESTROY:
        case WM_ENABLE:
        case WM_GETDLGCODE:
        case WM_GETFONT:
        case WM_GETTEXT:
        case WM_GETTEXTLENGTH:
        case WM_SETFONT:
        case WM_SETTEXT:
        case WM_PAINT:
        case WM_ERASEBKGND:
            {
                DWORD dwID = static_cast<DWORD>( GetWindowLongPtr( hWnd, GWLP_ID ) );
                if( dwID != IDC_DYNAMICCONTROL )
                {
                    auto pDlg = reinterpret_cast<ExportSettingsDialog*>( GetWindowLongPtr( hWnd, GWLP_USERDATA ) );
                    return CallWindowProc( pDlg->m_pStaticWndProc, hWnd, message, wParam, lParam );
                }
                return DefWindowProc( hWnd, message, wParam, lParam );
            }
        case WM_COMMAND:
        case WM_VSCROLL:
        case WM_NOTIFY:
        case WM_HSCROLL:
            return DlgProc( hWnd, message, wParam, lParam );
        default:
            return DefWindowProc( hWnd, message, wParam, lParam );
        }
    }


    LRESULT ExportSettingsDialog::OnInitDialog( HWND hwndFocusCtrl )
    {
        UNREFERENCED_PARAMETER( hwndFocusCtrl );
        m_hCategoryTree = GetDlgItem( m_hwnd, IDC_CATEGORIES );
        m_hOKButton = GetDlgItem( m_hwnd, IDOK );
        m_hCancelButton = GetDlgItem( m_hwnd, IDCANCEL );

        m_hScrollingPane = CreateWindowEx( 
            WS_EX_CLIENTEDGE | WS_EX_CONTROLPARENT,                          
            "Static",       // window class 
            nullptr,        // text for window title bar 
            WS_CHILD |      // window styles 
            WS_CLIPCHILDREN |
            WS_VSCROLL |
            WS_VISIBLE |
            SS_NOTIFY, 
            300,
            0,
            100,
            100,
            m_hwnd,             
            nullptr,        // window class menu 
            g_hInstance,    // instance owning this window 
            nullptr         // pointer not needed 
            ); 

        SetWindowLongPtr( m_hScrollingPane, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( this ) );
        m_pStaticWndProc = reinterpret_cast<WNDPROC>( SetWindowLongPtr( m_hScrollingPane, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>( ScrollPaneWndProc ) ) );

        SetWindowText( m_hwnd, g_strTitle );

        PopulateCategories();
        TreeView_SelectItem( m_hCategoryTree, TreeView_GetRoot( m_hCategoryTree ) );
        //PopulateControls();

        SendMessage( m_hwnd, WM_SIZE, 0, 0 );
        SetTimer( m_hwnd, 1, 0, nullptr );

        return false;
    }


    BOOL CALLBACK DestroyWindowCallback( HWND hWnd, LPARAM lParam )
    {
        UNREFERENCED_PARAMETER( lParam );
        DestroyWindow( hWnd );
        return TRUE;
    }


    HWND CreateStaticLabel( HWND hParent, const CHAR* strText, HFONT hFont, DWORD dwX, DWORD dwY, DWORD dwWidth )
    {
        HWND hLabel = CreateWindow( "Static", 
            strText, 
            WS_CHILD | WS_VISIBLE, 
            dwX, dwY,
            dwWidth, 25,
            hParent,
            nullptr, g_hInstance, nullptr );

        SendMessage( hLabel, WM_SETFONT, reinterpret_cast<WPARAM>( hFont ), TRUE );

        return hLabel;
    }


    HWND CreateCheckbox( HWND hParent, DWORD dwX, DWORD dwY, ExportSettingsEntry* pData )
    {
        HWND hCheckbox = CreateWindow( "Button",
            "",
            WS_CHILD | WS_VISIBLE | BS_CHECKBOX,
            dwX, dwY, 25, 25,
            hParent, 
            nullptr, g_hInstance, nullptr );
        SetWindowLongPtr( hCheckbox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pData ) );
        SetWindowLongPtr( hCheckbox, GWLP_ID, IDC_DYNAMICCONTROL );
        return hCheckbox;
    }


    HWND CreateEditBox( HWND hParent, DWORD dwX, DWORD dwY, DWORD dwWidth, ExportSettingsEntry* pData )
    {
        HWND hEditBox = CreateWindowEx( WS_EX_CLIENTEDGE,
            "Edit",
            "",
            WS_CHILD | WS_VISIBLE,
            dwX, dwY, dwWidth, 25,
            hParent,
            nullptr, g_hInstance, nullptr );
        SetWindowLongPtr( hEditBox, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pData ) );
        SetWindowLongPtr( hEditBox, GWLP_ID, IDC_DYNAMICCONTROL );
        return hEditBox;
    }


    HWND CreateTrackBar( HWND hParent, DWORD dwX, DWORD dwY, DWORD dwWidth, ExportSettingsEntry* pData )
    {
        HWND hTrackBar = CreateWindowEx( 0, TRACKBAR_CLASS, nullptr, 
            TBS_AUTOTICKS | TBS_HORZ | TBS_TOOLTIPS | WS_VISIBLE | WS_CHILD, 
            dwX, dwY, dwWidth, 30,
            hParent,
            nullptr, g_hInstance, nullptr );
        SetWindowLongPtr( hTrackBar, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pData ) );
        SetWindowLongPtr( hTrackBar, GWLP_ID, IDC_DYNAMICCONTROL );
        SendMessage( hTrackBar, TBM_SETRANGE, TRUE, MAKELONG( pData->m_MinValue.m_iValue, pData->m_MaxValue.m_iValue ) );
        SendMessage( hTrackBar, TBM_SETTICFREQ, 10, 0 );
        return hTrackBar;
    }


    HWND CreateTrackBarFloat( HWND hParent, DWORD dwX, DWORD dwY, DWORD dwWidth, ExportSettingsEntry* pData )
    {
        HWND hTrackBar = CreateWindowEx( 0, TRACKBAR_CLASS, nullptr, 
            TBS_AUTOTICKS | TBS_HORZ | WS_VISIBLE | WS_CHILD, 
            dwX, dwY, dwWidth, 30,
            hParent,
            nullptr, g_hInstance, nullptr );
        SetWindowLongPtr( hTrackBar, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pData ) );
        SetWindowLongPtr( hTrackBar, GWLP_ID, IDC_DYNAMICCONTROL );
        SendMessage( hTrackBar, TBM_SETRANGE, TRUE, MAKELONG( 0, 1000 ) );
        SendMessage( hTrackBar, TBM_SETTICFREQ, 100, 0 );
        return hTrackBar;
    }


    HWND CreateDropList( HWND hParent, DWORD dwX, DWORD dwY, DWORD dwWidth, ExportSettingsEntry* pData )
    {
        assert( pData->m_Type == ExportSettingsEntry::CT_ENUM );
        HWND hDropList = CreateWindowEx( 0, WC_COMBOBOXEX, "",
            CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE,
            dwX, dwY, dwWidth, 300,
            hParent, nullptr, g_hInstance, nullptr );
        SetWindowLongPtr( hDropList, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pData ) );
        SetWindowLongPtr( hDropList, GWLP_ID, IDC_DYNAMICDROPLIST );

        for( DWORD i = 0; i < pData->m_dwEnumValueCount; ++i )
        {
            const ExportEnumValue* pEnumValue = &pData->m_pEnumValues[i];
            COMBOBOXEXITEMA CBItem = {0};
            CBItem.mask = CBEIF_TEXT;
            CBItem.pszText = const_cast<LPSTR>( pEnumValue->strLabel );
            CBItem.iItem = static_cast<INT>( i );
            SendMessageA( hDropList, CBEM_INSERTITEM, 0, reinterpret_cast<LPARAM>( &CBItem ) );
        }
        return hDropList;
    }


    void ExportSettingsDialog::PopulateControls()
    {
        auto DlgFont = reinterpret_cast<HFONT>( SendMessage( m_hOKButton, WM_GETFONT, 0, 0 ) );

        EnumChildWindows( m_hScrollingPane, DestroyWindowCallback, 0 );

        if( !m_pCurrentCategory )
            return;

        RECT ClientRect;
        GetClientRect( m_hScrollingPane, &ClientRect );
        const DWORD dwClientWidth = ClientRect.right - ClientRect.left;
        const DWORD dwClientMargin = 8;

        DWORD dwYPos = dwClientMargin;
        const DWORD dwXPosLabel = dwClientMargin;
        const DWORD dwXPosControl = dwClientWidth / 3;
        const DWORD dwLabelWidth = ( dwXPosControl - dwXPosLabel - 5 );
        ExportSettingsEntry* pSetting = m_pCurrentCategory->m_pFirstChild;

        const DWORD dwControlWidth = dwClientWidth - dwXPosControl;

        while( pSetting )
        {
            HWND hwndControl = nullptr;
            DWORD dwControlHeight = 30;
            CreateStaticLabel( m_hScrollingPane, pSetting->m_DisplayName, DlgFont, dwXPosLabel, dwYPos, dwLabelWidth );
            switch( pSetting->m_Type )
            {
            case ExportSettingsEntry::CT_CHECKBOX:
                hwndControl = CreateCheckbox( m_hScrollingPane, dwXPosControl, dwYPos, pSetting );
                SynchronizeControlUI( hwndControl );
                break;
            case ExportSettingsEntry::CT_STRING:
                hwndControl = CreateEditBox( m_hScrollingPane, dwXPosControl, dwYPos, dwControlWidth, pSetting );
                SendMessage( hwndControl, WM_SETFONT, reinterpret_cast<WPARAM>( DlgFont ), TRUE );
                SynchronizeControlUI( hwndControl );
                break;
            case ExportSettingsEntry::CT_BOUNDEDINTSLIDER:
                hwndControl = CreateTrackBar( m_hScrollingPane, dwXPosControl, dwYPos, dwControlWidth, pSetting );
                SynchronizeControlUI( hwndControl );
                dwControlHeight = 40;
                break;
            case ExportSettingsEntry::CT_BOUNDEDFLOATSLIDER:
                hwndControl = CreateTrackBarFloat( m_hScrollingPane, dwXPosControl, dwYPos, dwControlWidth, pSetting );
                SynchronizeControlUI( hwndControl );
                dwControlHeight = 40;
                break;
            case ExportSettingsEntry::CT_ENUM:
                hwndControl = CreateDropList( m_hScrollingPane, dwXPosControl, dwYPos, dwControlWidth, pSetting );
                SendMessage( hwndControl, WM_SETFONT, reinterpret_cast<WPARAM>( DlgFont ), TRUE);
                SynchronizeControlUI( hwndControl );
                break;
            }
            dwYPos += dwControlHeight;
            pSetting = pSetting->m_pSibling;
        }

        m_dwControlsHeight = dwYPos;
        UpdateVScroll();
        InvalidateRect( m_hScrollingPane, nullptr, TRUE );
    }


    void ExportSettingsDialog::PopulateCategories( const ExportSettingsEntry* pEntry, void* hParentItem )
    {
        if( !pEntry )
        {
            TreeView_DeleteAllItems( m_hCategoryTree );
            size_t dwRootCount = g_SettingsManager.GetRootCategoryCount();
            for( size_t i = 0; i < dwRootCount; ++i )
            {
                PopulateCategories( g_SettingsManager.GetRootCategory( i ), nullptr );
            }
            return;
        }

        if( pEntry->m_Type != ExportSettingsEntry::CT_CATEGORY )
            return;

        TVINSERTSTRUCT TreeItem;
        ZeroMemory( &TreeItem, sizeof( TVINSERTSTRUCT ) );
        CHAR strText[MAX_PATH];
        strcpy_s( strText, pEntry->m_DisplayName.SafeString() );
        TreeItem.item.pszText = strText;
        TreeItem.item.mask = TVIF_TEXT | TVIF_PARAM;
        TreeItem.item.lParam = reinterpret_cast<LPARAM>( pEntry );
        TreeItem.hParent = reinterpret_cast<HTREEITEM>( hParentItem );
        TreeItem.hInsertAfter = TVI_LAST;

        HTREEITEM hCurrentItem = TreeView_InsertItem( m_hCategoryTree, &TreeItem );

        if( pEntry->m_pFirstChild )
        {
            PopulateCategories( pEntry->m_pFirstChild, hCurrentItem );
        }
        if( pEntry->m_pSibling )
        {
            PopulateCategories( pEntry->m_pSibling, hParentItem );
        }
    }


    INT GetNormalizedIntValue( ExportSettingsEntry* pEntry )
    {
        float fValue = pEntry->GetValueFloat();
        float fNorm = ( fValue - pEntry->m_MinValue.m_fValue ) / ( pEntry->m_MaxValue.m_fValue - pEntry->m_MinValue.m_fValue );
        return static_cast<INT>( fNorm * 1000.0f );
    }


    float GetFloatFromNormalizedInt( ExportSettingsEntry* pEntry, INT iValue )
    {
        float fNorm = (float)iValue / 1000.0f;
        float fValue = ( fNorm * ( pEntry->m_MaxValue.m_fValue - pEntry->m_MinValue.m_fValue ) ) + pEntry->m_MinValue.m_fValue;
        return fValue;
    }


    void ExportSettingsDialog::SynchronizeControlUI( HWND hwndControl )
    {
        if( !hwndControl )
            return;
        auto pEntry = reinterpret_cast<ExportSettingsEntry*>( GetWindowLongPtr( hwndControl, GWLP_USERDATA ) );
        if( !pEntry )
            return;
        m_bControlDataUpdate = true;
        switch( pEntry->m_Type )
        {
        case ExportSettingsEntry::CT_CHECKBOX:
            {
                bool bValue = pEntry->GetValueBool();
                SendMessage( hwndControl, BM_SETCHECK, bValue ? BST_CHECKED : BST_UNCHECKED, 0 );
                break;
            }
        case ExportSettingsEntry::CT_STRING:
            {
                SetWindowText( hwndControl, pEntry->GetValueString() );
                break;
            }
        case ExportSettingsEntry::CT_BOUNDEDINTSLIDER:
            {
                INT iValue = pEntry->GetValueInt();
                SendMessage( hwndControl, TBM_SETPOS, TRUE, iValue );
                break;
            }
        case ExportSettingsEntry::CT_BOUNDEDFLOATSLIDER:
            {
                INT iValue = GetNormalizedIntValue( pEntry );
                SendMessage( hwndControl, TBM_SETPOS, TRUE, iValue );
                break;
            }
        case ExportSettingsEntry::CT_ENUM:
            {
                DWORD dwSelectionIndex = 0;
                INT iCurrentValue = pEntry->GetValueInt();
                for( DWORD i = 0; i < pEntry->m_dwEnumValueCount; ++i )
                {
                    const ExportEnumValue* pEnumValue = &pEntry->m_pEnumValues[i];
                    if( pEnumValue->iValue == iCurrentValue )
                        dwSelectionIndex = i;
                }
                ComboBox_SetCurSel( hwndControl, dwSelectionIndex );
                break;
            }
        }
        m_bControlDataUpdate = false;
    }


    void ExportSettingsDialog::UpdateVScroll()
    {
        RECT PageRect;
        GetWindowRect( m_hScrollingPane, &PageRect );
        SCROLLINFO si;
        si.cbSize = sizeof( SCROLLINFO );
        si.nMin = 0;
        si.nMax = m_dwControlsHeight;
        si.nPage = PageRect.bottom - PageRect.top;
        si.nPos = 0;
        si.nTrackPos = 0;
        si.fMask = SIF_ALL;
        SetScrollInfo( m_hScrollingPane, SB_VERT, &si, TRUE );
    }

    LRESULT ExportSettingsDialog::OnCommand( WORD wNotifyCode, WORD idCtrl, HWND hwndCtrl )
    {
        if( idCtrl == 0 )
        {
            idCtrl = static_cast<WORD>( GetWindowLongPtr( hwndCtrl, GWLP_ID ) );
        }
        switch (idCtrl)
        {
        case IDCANCEL: // the little x in the upper right?
        case IDOK:
        case IDCLOSE:
            switch (wNotifyCode)
            {
            case BN_CLICKED:
                Hide();
                if( idCtrl == IDOK )
                    m_DialogState = DS_HIDDEN_OK;
                else
                    m_DialogState = DS_HIDDEN_CANCELED;
                return 0;
            }
            break;
        case IDC_DYNAMICCONTROL:
            {
                if( m_bControlDataUpdate )
                    return true;
                auto pEntry = reinterpret_cast<ExportSettingsEntry*>( GetWindowLongPtr( hwndCtrl, GWLP_USERDATA ) );
                if( wNotifyCode == BN_CLICKED && pEntry->m_Type == ExportSettingsEntry::CT_CHECKBOX )
                {
                    pEntry->SetValue( !pEntry->GetValueBool() );
                    SynchronizeControlUI( hwndCtrl );
                }
                if( wNotifyCode == EN_CHANGE && pEntry->m_Type == ExportSettingsEntry::CT_STRING )
                {
                    CHAR strText[256];
                    GetWindowText( hwndCtrl, strText, 256 );
                    pEntry->SetValue( strText );
                }
                return true;
            }
        case IDC_DYNAMICDROPLIST:
            {
                if( m_bControlDataUpdate )
                    return true;
                auto pEntry = reinterpret_cast<ExportSettingsEntry*>( GetWindowLongPtr( hwndCtrl, GWLP_USERDATA ) );
                assert( pEntry->m_Type == ExportSettingsEntry::CT_ENUM );
                if( wNotifyCode == CBN_SELCHANGE )
                {
                    INT iEnumIndex = ComboBox_GetCurSel( hwndCtrl );
                    if( iEnumIndex < 0 )
                        iEnumIndex = 0;
                    if( iEnumIndex >= static_cast<INT>( pEntry->m_dwEnumValueCount ) )
                        iEnumIndex = static_cast<INT>( pEntry->m_dwEnumValueCount - 1 );
                    pEntry->SetValue( pEntry->m_pEnumValues[iEnumIndex].iValue );
                }
                return true;
            }
        }
        return false;
    }

    LRESULT ExportSettingsDialog::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
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
                const DWORD dwBorderSize = 4;

                GridLayout RootLayout( 2, 1 );
                RootLayout.SetRowSpec( 0, 1.0f );
                RootLayout.SetRowSpec( 1, 30 );
                RootLayout.SetClientRect( m_hwnd, dwBorderSize );

                const float fSplitterWidth = 5.0f;
                GridLayout PanesLayout( 1, 3 );
                PanesLayout.SetColumnSpec( 0, 200 );
                PanesLayout.SetColumnSpec( 1, fSplitterWidth );
                PanesLayout.SetColumnSpec( 2, 1.0f );
                RootLayout.PlaceLayout( PanesLayout, 0, 0, 1, 1, dwBorderSize );

                const float fButtonWidth = 100.0f;
                GridLayout ButtonsLayout( 1, 3 );
                ButtonsLayout.SetColumnSpec( 0, 1.0f );
                ButtonsLayout.SetColumnSpec( 1, fButtonWidth );
                ButtonsLayout.SetColumnSpec( 2, fButtonWidth );
                RootLayout.PlaceLayout( ButtonsLayout, 1, 0, 1, 1, 0 );

                PanesLayout.PlaceWindow( m_hCategoryTree, 0, 0, 1, 1, 0 );
                PanesLayout.PlaceWindow( m_hScrollingPane, 0, 2, 1, 1, 0 );

                ButtonsLayout.PlaceWindow( m_hOKButton, 0, 1, 1, 1, dwBorderSize );
                ButtonsLayout.PlaceWindow( m_hCancelButton, 0, 2, 1, 1, dwBorderSize );

                PopulateControls();

                return false;
            }
        case WM_VSCROLL:
            {
                RECT PageRect;
                GetWindowRect( m_hScrollingPane, &PageRect );
                SCROLLINFO si;
                // Get all the vertial scroll bar information
                si.cbSize = sizeof (si);
                si.fMask  = SIF_ALL;
                GetScrollInfo( m_hScrollingPane, SB_VERT, &si );
                si.nPage = PageRect.bottom - PageRect.top;
                // Save the position for comparison later on
                INT iCurrentScrollPos = si.nPos;
                switch (LOWORD (wParam))
                {
                    // user clicked the HOME keyboard key
                case SB_TOP:
                    si.nPos = si.nMin;
                    break;

                    // user clicked the END keyboard key
                case SB_BOTTOM:
                    si.nPos = si.nMax;
                    break;

                    // user clicked the top arrow
                case SB_LINEUP:
                    si.nPos -= 10;
                    break;

                    // user clicked the bottom arrow
                case SB_LINEDOWN:
                    si.nPos += 10;
                    break;

                    // user clicked the scroll bar shaft above the scroll box
                case SB_PAGEUP:
                    si.nPos -= si.nPage;
                    break;

                    // user clicked the scroll bar shaft below the scroll box
                case SB_PAGEDOWN:
                    si.nPos += si.nPage;
                    break;

                    // user dragged the scroll box
                case SB_THUMBTRACK:
                    si.nPos = si.nTrackPos;
                    break;

                default:
                    break; 
                }
                // Set the position and then retrieve it.  Due to adjustments
                //   by Windows it may not be the same as the value set.
                si.fMask = SIF_POS;
                SetScrollInfo (m_hScrollingPane, SB_VERT, &si, TRUE);
                GetScrollInfo (m_hScrollingPane, SB_VERT, &si);
                // If the position has changed, scroll window and update it
                if( si.nPos != iCurrentScrollPos )
                {                    
                    ScrollWindow(m_hScrollingPane, 0, ( iCurrentScrollPos - si.nPos ), nullptr, nullptr);
                    UpdateWindow (m_hScrollingPane);
                }
                return false;
            }
        case WM_HSCROLL:
            {
                if( m_bControlDataUpdate )
                    return false;
                HWND hwndControl = (HWND)lParam;
                if( GetWindowLongPtr( hwndControl, GWLP_ID ) != IDC_DYNAMICCONTROL )
                    return 1;
                auto pEntry = reinterpret_cast<ExportSettingsEntry*>( GetWindowLongPtr( hwndControl, GWLP_USERDATA ) );
                if( !pEntry )
                    return false;
                INT iPos = static_cast<INT>( SendMessage( hwndControl, TBM_GETPOS, 0, 0 ) );
                if( pEntry->m_Type == ExportSettingsEntry::CT_BOUNDEDINTSLIDER )
                    pEntry->SetValue( iPos );
                else
                    pEntry->SetValue( GetFloatFromNormalizedInt( pEntry, iPos ) );
                return false;
            }
        }
        return false;
    }

    LRESULT ExportSettingsDialog::OnNotify( INT idCtrl, LPNMHDR pnmh )
    {
        switch( idCtrl )
        {
        case IDC_CATEGORIES:
            switch( pnmh->code )
            {
            case TVN_SELCHANGED:
                {
                    auto pNMTV = reinterpret_cast<NM_TREEVIEW*>( pnmh );
                    auto pEntry = reinterpret_cast<ExportSettingsEntry*>( pNMTV->itemNew.lParam );
                    if( !pEntry )
                        return true;
                    assert( pEntry != nullptr );
                    assert( pEntry->m_Type == ExportSettingsEntry::CT_CATEGORY );
                    m_pCurrentCategory = pEntry;
                    PopulateControls();
                    return true;
                }
            }
            return true;
        }
        return false;
    }
}