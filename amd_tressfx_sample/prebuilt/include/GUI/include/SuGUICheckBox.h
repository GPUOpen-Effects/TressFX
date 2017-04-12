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

#ifndef _SUGUICHECKBOX_H_
#define _SUGUICHECKBOX_H_

#include "SuGUIElement.h"

// tolua_begin
class SuGUICheckBox : public SuGUIElement
{
public:

    ~SuGUICheckBox();
// tolua_end
    /// **************************************************************
    /// Static member used to create an instance of a SuGUICheckBox
    ///
    /// \param name Name of this element, can not be NULL.
    /// \param isChecked bool used to keep trake of value, can be NULL.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// \return SuGUICheckBox*
    /// **************************************************************
    static SuGUICheckBox* CreateCheckBox(SuString name, bool isChecked = false, bool isSameLineAsLastElement = false);

// tolua_begin
	SU_DLL SuString GetName() { return m_sName; }

    SU_DLL bool GetIsChecked(){ return m_bIsChecked; }
// tolua_end
    /// Override from SuGUIElement to draw the CheckBox.
    virtual void Draw();

private:

    SuGUICheckBox();

	/// This bool is used to determine whether the checkbox has been checked or not
	/// in the last frame.
	bool m_bLastCheckValue;

    /// The bool determining whether its checked or not.
    bool m_bIsChecked;
// tolua_begin
};
// tolua_end
#endif