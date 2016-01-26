//-------------------------------------------------------------------------------------
// ExportDialogUtils.cpp
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

#include "ExportDialogUtils.h"

namespace ATG
{
    void ThinDialog::CenterOnParent()
    {
        // center ourself in our parent
        RECT rcParent;
        RECT rcChild;
        INT x;
        INT y;
        HWND hwndParent = ::GetParent( m_hwnd );
        // center in the desktop if our parent is iconified
        if (::IsIconic( hwndParent ))
        {
            hwndParent = ::GetDesktopWindow();
        }
        ::GetWindowRect( hwndParent, &rcParent );
        ::GetWindowRect( m_hwnd, &rcChild );
        x = ((rcParent.right - rcParent.left) - (rcChild.right - rcChild.left)) / 2;
        y = ((rcParent.bottom - rcParent.top) - (rcChild.bottom - rcChild.top)) / 2;
        ::SetWindowPos( m_hwnd, nullptr, x, y, 0, 0, SWP_NOACTIVATE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER );
    }


    LRESULT CALLBACK ThinDialog::DlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam )
    {
        LRESULT lrt = FALSE;

        auto pDlg = GetInstance( hwndDlg );

        switch (uMsg)
        {
        case WM_NCACTIVATE:
            if ( pDlg )
            {
                lrt = pDlg->OnNcActivate( static_cast<BOOL>( wParam ) );
            }
            break;
        case WM_ACTIVATE:
            if (pDlg)
            {
                lrt = pDlg->OnActivate( static_cast<BOOL>( wParam ), static_cast<DWORD>( lParam ) );
            }
            break;
        case WM_INITDIALOG:
            pDlg = reinterpret_cast<ThinDialog*>( lParam );
            SetInstance( hwndDlg, pDlg );
            lrt = pDlg->OnInitDialog( (HWND)wParam );
            break;

        case WM_NOTIFY:
            assert( pDlg );
            lrt = pDlg->OnNotify( static_cast<INT>( wParam ), reinterpret_cast<LPNMHDR>( lParam ) );
            break;

        case WM_DESTROY:
            assert( pDlg );
            lrt = pDlg->OnDestroyDialog();
            pDlg->m_hwnd = nullptr;
            break;

        case WM_COMMAND:
            assert( pDlg );
            lrt = pDlg->OnCommand( HIWORD( wParam ), LOWORD( wParam ), (HWND)lParam );
            break;

        case WM_ENABLE: // 0x000a
        case WM_ACTIVATEAPP: // 0x001c
        case WM_CANCELMODE: // 0x001f
        case WM_DRAWITEM: // 002b
        case WM_MEASUREITEM: // 0x002c
        case WM_SETFONT: // 0x0030
        case WM_WINDOWPOSCHANGING: // 0x0046
        case WM_WINDOWPOSCHANGED: // 0x0047
        case WM_NOTIFYFORMAT: // 0x0055
        case 0x0129:  // mystery message
        case WM_CTLCOLOREDIT: // 0x0133
            if (pDlg) // if it is sent before we know who we are, nothing we can do
            {
                lrt = pDlg->OnMessage( uMsg, wParam, lParam );
            }
            break;

        default:
            assert( pDlg );
            lrt = pDlg->OnMessage( uMsg, wParam, lParam );
            break;
        }
        return lrt;
    };

    LRESULT ThinDialog::OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam )
    {
        UNREFERENCED_PARAMETER( uMsg );
        UNREFERENCED_PARAMETER( wParam );
        UNREFERENCED_PARAMETER( lParam );
        return false;
    };

    LRESULT ThinDialog::OnNcActivate( BOOL fDrawActive )
    {
        UNREFERENCED_PARAMETER( fDrawActive );
        return false;
    };

    LRESULT ThinDialog::OnActivate( BOOL fActivated, DWORD idThread )
    {
        UNREFERENCED_PARAMETER( fActivated );
        UNREFERENCED_PARAMETER( idThread );
        return false;
    };

    LRESULT ThinDialog::OnInitDialog( HWND hwndFocusCtrl )
    {
        UNREFERENCED_PARAMETER( hwndFocusCtrl );
        return false;
    };

    LRESULT ThinDialog::OnDestroyDialog( )
    {
        return false;
    };

    LRESULT ThinDialog::OnNotify( INT idCtrl, LPNMHDR pnmh )
    {
        UNREFERENCED_PARAMETER( idCtrl );
        UNREFERENCED_PARAMETER( pnmh );
        return false;
    };

    LRESULT ThinDialog::OnCommand( WORD wNotifyCode, WORD idCtrl, HWND hwndCtrl )
    {
        UNREFERENCED_PARAMETER( wNotifyCode );
        UNREFERENCED_PARAMETER( idCtrl );
        UNREFERENCED_PARAMETER( hwndCtrl );
        return false;
    };

    void GridLayout::SetRect(const LPRECT rect, INT border)
    {
        m_xOffset = rect->left + border;
        m_yOffset = rect->top + border;

        INT width = rect->right - rect->left - border * 2;
        INT height = rect->bottom - rect->top - border * 2;

        if (width < 0)
            width = 0;
        if (height < 0)
            height = 0;

        // find spec entries that request pixel counts
        for (INT i = 0; i < GRIDMAX; i++)
        {
            if (m_rowSpec[i] > 1)
            {
                INT amount = static_cast<INT>( m_rowSpec[i] );
                if (amount > height)
                    amount = height;
                m_rowAlloc[i] = amount;
                height -= amount;
            }
            if (m_columnSpec[i] > 1)
            {
                INT amount = static_cast<INT>( m_columnSpec[i] );
                if (amount > width)
                    amount = width;
                m_columnAlloc[i] = amount;
                width -= amount;
            }
        }

        // evenly distribute remaining height and width amongst fractional entries
        // note: if fractional entries total > 1, the last entries get nothing
        INT remHeight = height;
        INT remWidth = width;
        for (INT i = 0; i < GRIDMAX; i++)
        {
            if (m_rowSpec[i] > 0 && m_rowSpec[i] <= 1)
            {
                INT amount = static_cast<INT>(m_rowSpec[i] * (float)height);
                if (amount > remHeight)
                    amount = remHeight;
                m_rowAlloc[i] = amount;
                remHeight -= amount;
            }
            if (m_columnSpec[i] > 0 && m_columnSpec[i] <= 1)
            {
                INT amount = static_cast<INT>(m_columnSpec[i] * (float)width);
                if (amount > remWidth)
                    amount = remWidth;
                m_columnAlloc[i] = amount;
                remWidth -= amount;
            }
        }
    }

    void GridLayout::SetClientRect(HWND hwnd, INT border)
    {
        RECT rect;
        GetClientRect(hwnd, &rect);
        SetRect(&rect, border);
    }

    void GridLayout::SetWindowRect(HWND hwnd, INT border)
    {
        RECT rect;
        GetWindowRect(hwnd, &rect);
        SetRect(&rect, border);
    }

    void GridLayout::GetRect(LPRECT destRect, INT rowStart, INT colStart, INT rowSpan, INT colSpan, INT border)
    {
        destRect->left = m_xOffset;
        destRect->top = m_yOffset;

        // sum across to get the start coords
        for (INT i = 0; i < rowStart; i++)
            destRect->top += m_rowAlloc[i];
        for (INT i = 0; i < colStart; i++)
            destRect->left += m_columnAlloc[i];

        // set right and bottom to left and top
        destRect->bottom = destRect->top;
        destRect->right = destRect->left;

        // sum across to get widths
        for (INT i = rowStart; i < rowStart + rowSpan; i++)
            destRect->bottom += m_rowAlloc[i];
        for (INT i = colStart; i < colStart + colSpan; i++)
            destRect->right += m_columnAlloc[i];

        // subtract out a border
        destRect->left += border;
        destRect->top += border;
        destRect->right -= border;
        destRect->bottom -= border;
    }

    void GridLayout::PlaceWindow(HWND hwnd, INT rowStart, INT colStart, INT rowSpan, INT colSpan, INT border, DWORD alignment)
    {
        RECT rect;
        GetRect(&rect, rowStart, colStart, rowSpan, colSpan, border);
        INT cy = rect.bottom - rect.top;
        INT cx = rect.right - rect.left;

        // early out if the control is supposed to be resized to fit the rect
        if (alignment == ALIGN_FILLRECT)
        {
            MoveWindow(hwnd, rect.left, rect.top, cx, cy, true);
            return;
        }

        INT px = rect.left, py = rect.top;

        // get current size of window
        RECT currentRect;
        GetWindowRect(hwnd, &currentRect);
        INT sizeX = currentRect.right - currentRect.left;
        INT sizeY = currentRect.bottom - currentRect.top;

        INT newSizeX = sizeX;
        INT newSizeY = sizeY;

        // figure out horizontal placement
        if (alignment & ALIGN_HCENTER)
        {
            px = rect.left + (cx / 2) - (sizeX / 2);
        }
        else if (alignment & ALIGN_LEFT)
        {
            px = rect.left;
        }
        else if (alignment & ALIGN_RIGHT)
        {
            px = rect.right - sizeX;
        }
        else if (alignment & ALIGN_HJUSTIFY)
        {
            px = rect.left;
            newSizeX = cx;
        }

        // figure out vertical placement
        if (alignment & ALIGN_VCENTER)
        {
            py = rect.top + (cy / 2) - (sizeY / 2);
        }
        else if (alignment & ALIGN_TOP)
        {
            py = rect.top;
        }
        else if (alignment & ALIGN_BOTTOM)
        {
            py = rect.bottom - sizeY;
        }
        else if (alignment & ALIGN_VJUSTIFY)
        {
            py = rect.top;
            newSizeY = cy;
        }

        // move window INTo place
        if (newSizeX == sizeX && newSizeY == sizeY)
            SetWindowPos(hwnd, nullptr, px, py, 0, 0, SWP_NOZORDER|SWP_NOSIZE);
        else
            SetWindowPos(hwnd, nullptr, px, py, newSizeX, newSizeY, SWP_NOZORDER);
    }

    void GridLayout::PlaceLayout(GridLayout& layout, INT rowStart, INT colStart, INT rowSpan, INT colSpan, INT border)
    {
        RECT cellRect;
        GetRect(&cellRect, rowStart, colStart, rowSpan, colSpan, border);
        layout.SetRect(&cellRect);
    }
}