//
// Copyright (c) 2016 Advanced Micro Devices, Inc. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#ifndef _SUGUIMENUITEM_H_
#define _SUGUIMENUITEM_H_
#include "SuGUIElement.h"

class SuGUIMenuItem : public SuGUIElement
{

public:
    ~SuGUIMenuItem();

    /// ***************************************************************************
    /// CreateMenuItem will create a new MenuItem and return a pointer to it.
    ///
    /// \param name The name / Label of this menu item.
    /// \param shortcut The shortcut of the item.
    /// \param selected The bool this item toggles True/False.
    /// \param enabled Whether this item is enabled or not.
    /// \return Returns a pointer to the newly created SuGUIMenuItem to be used in an SuGUIElement list.
    /// ***************************************************************************
    static SuGUIMenuItem* CreateMenuItem(SuString name, SuString shortcut, bool* selected, bool enabled = true);

    /// Returns the m_Selected bool
    bool GetSelected();

    /// Overrides base class draw function.
    void Draw();

private:
    SuGUIMenuItem();

    /// The shortcut for the MenuItem
    SuString m_sShortcut;

    /// Bool to be toggled when the item is clicked
    bool* m_bSelected;

    /// Bool to determine whether this item is selectable.
    bool m_bEnabled;
};

#endif