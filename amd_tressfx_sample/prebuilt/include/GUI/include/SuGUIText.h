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

#ifndef _SUGUITEXT_H_
#define _SUGUITEXT_H_

#include "SuGUIElement.h"
#include "SuVector4.h"

// tolua_begin
class SuGUIText : public SuGUIElement
{

public:

    ~SuGUIText();
// tolua_end
    /// **********************************************************************
    /// Creates a text element. Cannot have wrapped and colored text.
    /// If you have both selected it will choose to wrappe the text over coloring it.
    ///
    /// \param text The text to display.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// \return Returns a SuGUIText*
    /// **********************************************************************
    static SuGUIText* CreateText(SuString text, bool isSameLineAsLastElement = false);

// tolua_begin
    /// *******************************************************************
    /// SetWrapped will set the value of m_Wrapped. Wrapped text will not
    /// overflow past the bounds of the text area but will move down to a
    /// new line.
    ///
    /// \param wrap The new value of m_Wrappped.
    /// *******************************************************************
    SU_DLL void SetWrapped(bool wrap);

    /// ******************************************************************
    /// SetColor will set the color of the text. If the text is also wrapped
    /// the color of the text will have no effect.
    ///
    /// \param color The new color of the text.
    /// ******************************************************************
	SU_DLL void SetColor(SuVector4 color);

	/// ******************************************************************
	/// SetColor will set the color of the text. If the text is also wrapped
    /// the color of the text will have no effect.
	///
	/// \param r The amount of red in the color
	/// \param g The amount of green in the color
	/// \param b The amount of blue in the color
	/// \param a The alpha value of this color
	/// ******************************************************************
	SU_DLL void SetColor(float r, float g, float b, float a);

	SU_DLL void SetText(SuString text);

// tolua_end
    /// Overrides the base class draw function
    virtual void Draw();

private:

    SuGUIText();
       
    /// The text to be displayed
    SuString m_sText;

    /// Whether the text is wrapped or not.
    bool m_bWrapped;

    /// Whether the text is colored or not.
    bool m_bIsColored;

    /// The color of the text.
    SuVector4 m_vColor;

// tolua_begin
};
// tolua_end

#endif