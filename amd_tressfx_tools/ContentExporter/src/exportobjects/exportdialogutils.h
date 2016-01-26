//-------------------------------------------------------------------------------------
// ExportDialogUtils.h
// 
// Wrapper code to build Win32 dialog boxes.
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
    class ThinDialog
    {
    public:
        static LRESULT CALLBACK DlgProc( HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam );

        HWND Hwnd() 
        { 
            return m_hwnd; 
        };

        ThinDialog() : 
            m_hwnd( nullptr ),
            m_strTemplate( nullptr )
        {
        };

        virtual ~ThinDialog() 
        {
            ::DestroyWindow( m_hwnd );
        };

        INT DoModal( HINSTANCE hinst, HWND hwndParent )
        {
            return static_cast<INT>( ::DialogBoxParam( hinst, 
                m_strTemplate, 
                hwndParent, 
                reinterpret_cast<DLGPROC>( DlgProc ),
                reinterpret_cast<LPARAM>( this ) ) );
        };

        bool DoModeless( HINSTANCE hinst, HWND hwndParent )
        {
            m_hwnd = ::CreateDialogParam( hinst,
                m_strTemplate, 
                hwndParent, 
                reinterpret_cast<DLGPROC>( DlgProc ),
                reinterpret_cast<LPARAM>( this ) );
            return (m_hwnd != 0);
        };

        void Destroy()
        {
            ::DestroyWindow( m_hwnd );
        }

    protected:
        virtual LRESULT OnMessage( UINT uMsg, WPARAM wParam, LPARAM lParam );
        virtual LRESULT OnNcActivate( BOOL fDrawActive );
        virtual LRESULT OnActivate( BOOL fActivated, DWORD idThread );
        virtual LRESULT OnInitDialog( HWND hwndFocusCtrl );
        virtual LRESULT OnDestroyDialog( );
        virtual LRESULT OnNotify( INT idCtrl, LPNMHDR pnmh );
        virtual LRESULT OnCommand( WORD wNotifyCode, WORD idCtrl, HWND hwndCtrl ); 

        void CenterOnParent();
        HWND m_hwnd;
        LPTSTR m_strTemplate;

    private:
        
        static void SetInstance( HWND hwndDlg, ThinDialog* pDlg )
        {
            if (pDlg)   
            {
                ::SetWindowLongPtr( hwndDlg, GWLP_USERDATA, reinterpret_cast<LONG_PTR>( pDlg ) );
                pDlg->m_hwnd = hwndDlg;
            }
        };

        static  ThinDialog* GetInstance( HWND hwndDlg )
        {
            return reinterpret_cast<ThinDialog*>( ::GetWindowLongPtr( hwndDlg, GWLP_USERDATA ) );
        }
    };

    /*

    GridLayout manages a grid of rectangles, where the column and row sizes are
    based on user-provided specifications combined with the size of an input
    rectangle, which spcifies the size of the entire grid.  Its operation is much like
    a TABLE object in HTML.  

    This class is really useful for laying out a window of UI elements that needs to be 
    arbitrarily resizable.

    Specifications for rows and columns come in the form of FLOATs.  If the float is
    less than or equal to 1, it is INTerpreted as a percentage of the adjusted size of
    the grid.  If the float is greater than 1, it is INTerpreted as an exact pixel size
    that is expected in the final pixel allocation.

    After the specs have been set, an input rect is passed in.  A border amount (also a parameter)
    is subtracted from the input rect, making it smaller.  Then, all of the exact pixel amounts
    in the row and column specs are evaluated, and each subtracts their amount from the remaining
    height and width, respectively.  The remaining amount is called the adjusted height and width.
    Now, the percentage specs are evaluated, for example a row spec of 0.5 gets 50% of the adjusted
    height.

    Once the allocated amounts are computed, the user can now query cells in the grid or ranges of
    cells in the grid.  The PlaceWindow method is where a user can pass in a window handle, a cell range,
    and an alignment specification, and GridLayout will move the window INTo the proper place.

    Sample usage:

    // create a 2x2 grid
    GridLayout layout(2, 2);
    // set the height of row 0 to be the current height of the window m_toolbarHwnd
    layout.SetRowSpec(0, m_toolbarHwnd);
    // set the height of row 1 to be the rest of the height
    layout.SetRowSpec(1, 1);
    // set the width of column 0 to 200 pixels
    layout.SetColumnSpec(0, 200);
    // set the width of column 1 to be the rest of the width
    layout.SetColumnSpec(1, 1);
    // allocate the grid sizes based on the client rect of m_hwnd
    layout.SetClientRect(m_hwnd);
    // place the toolbar in cell 0,0, spanning two columns with a border of 0
    layout.PlaceWindow(m_toolbarHwnd, 0, 0, 1, 2, 0);
    // place the tree view in cell 1,0 with a border of 5 pixels
    layout.PlaceWindow(m_treeViewHwnd, 1, 0, 1, 1, 5);
    // place the tab control in cell 1,1 with a border of 2 pixels
    layout.PlaceWindow(m_tabCtrlHwnd, 1, 1, 1, 1, 2);

    */


#define GRIDMAX 10

    class GridLayout
    {
    public:
        enum Alignments
        {
            ALIGN_HCENTER = 0x0001,
            ALIGN_LEFT = 0x0002,
            ALIGN_RIGHT = 0x0004,
            ALIGN_HJUSTIFY = 0x0008,
            ALIGN_VCENTER = 0x0010,
            ALIGN_TOP = 0x0020,
            ALIGN_BOTTOM = 0x0040,
            ALIGN_VJUSTIFY = 0x0080,
            ALIGN_FILLRECT = ALIGN_HJUSTIFY | ALIGN_VJUSTIFY,
            ALIGN_CENTERED = ALIGN_HCENTER | ALIGN_VCENTER,
            ALIGN_UPPERLEFT = ALIGN_LEFT | ALIGN_TOP,
            ALIGN_CENTERLEFT = ALIGN_LEFT | ALIGN_VCENTER,
            ALIGN_CENTERRIGHT = ALIGN_RIGHT | ALIGN_VCENTER,
            ALIGN_FORCE_DWORD = 0x7fffffff
        };
    public:

        GridLayout(INT rows = 1, INT columns = 1)
        {
            assert(rows > 0 && columns > 0);
            Reset();
            float share = 1.0f / (float)rows;
            for (INT i = 0; i < rows; i++)
                m_rowSpec[i] = share;
            share = 1.0f / (float)columns;
            for (INT i = 0; i < columns; i++)
                m_columnSpec[i] = share;
        }

        ~GridLayout()
        {
        }

        // spec methods
        // ------------------------------

        // SetRowSpec sets the allocation spec for a row.
        // if the value is less than 1, the allocation will be a percentage of the adjusted height.
        // if the value is 1, the allocation will be 100% of the adjusted height.
        // if the value is greater than 1, the allocation will be that amount in pixels.
        void SetRowSpec(UINT index, float s = 1)
        {
            assert(index < GRIDMAX);
            m_rowSpec[index] = s;
        }

        // convenience method that gets the current height of window
        void SetRowSpec(UINT index, HWND window)
        {
            assert(index < GRIDMAX);
            RECT rect;
            GetWindowRect(window, &rect);
            m_rowSpec[index] = (float)(rect.bottom - rect.top);
        }

        // SetColumnSpec sets the allocation spec for a column.
        // if the value is less than 1, the allocation will be a percentage of the adjusted width.
        // if the value is 1, the allocation will be 100% of the adjusted width.
        // if the value is greater than 1, the allocation will be that amount in pixels.
        void SetColumnSpec(UINT index, float s = 1)
        {
            assert(index < GRIDMAX);
            m_columnSpec[index] = s;
        }

        // convenience method that gets the current width of window
        void SetColumnSpec(UINT index, HWND window)
        {
            assert(index < GRIDMAX);
            RECT rect;
            GetWindowRect(window, &rect);
            m_columnSpec[index] = (float)(rect.right - rect.left);
        }

        // allocation methods
        // ------------------------------

        void SetRect(const LPRECT rect, INT border = 0);
        void SetWindowRect(HWND hwnd, INT border = 0);
        void SetClientRect(HWND hwnd, INT border = 0);

        // query methods
        // ------------------------------

        void GetRect(LPRECT destRect, INT rowStart, INT colStart, INT rowSpan = 1, INT colSpan = 1, INT border = 0);
        void PlaceWindow(HWND hwnd, INT rowStart, INT colStart, INT rowSpan = 1, INT colSpan = 1, INT border = 0, DWORD alignment = ALIGN_FILLRECT);
        void PlaceLayout(GridLayout& layout, INT rowStart, INT colStart, INT rowSpan = 1, INT colSpan = 1, INT border = 0);

    private:
        void Reset()
        {
            ZeroMemory(m_columnSpec, GRIDMAX * sizeof(float));
            ZeroMemory(m_rowSpec, GRIDMAX * sizeof(float));
            ZeroMemory(m_columnAlloc, GRIDMAX * sizeof(INT));
            ZeroMemory(m_rowAlloc, GRIDMAX * sizeof(INT));
        }

    private:
        float m_columnSpec[GRIDMAX];
        float m_rowSpec[GRIDMAX];

        INT m_columnAlloc[GRIDMAX];
        INT m_rowAlloc[GRIDMAX];

        INT m_xOffset;
        INT m_yOffset;
    };

}
