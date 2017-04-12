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

#ifndef _SUGUIBUTTON_H_
#define _SUGUIBUTTON_H_

#include "SuGUIElement.h"
#include "SuVector4.h"

class SuGUIButton : public SuGUIElement
{
public:
    virtual ~SuGUIButton();

    /// Overrice from SuGUIElement to draw the button.
    virtual void Draw();

    /// ********************************************************************************************************************
    /// Create Button will initialize all the values and return a SuGUIButton*
    ///
    /// \param name The Name and Label of this button.
    /// \param watchbool The boolean to switch when the button is pressed.
    /// \param size The size of the button. Default is (0,0). Default size will automatically adjust based on text size.
    /// \param sameLineAsLastElemet Whether this element is on the same line as the previous element.
    /// \return A object of type SuGUIButton* will be returned with everything initialized.
    /// ********************************************************************************************************************
    static SuGUIButton* CreateButton(int buttonID, SuString name, bool* watchBool, SuVector2 size = SuVector2(0, 0), bool sameLineAsLastElemet = false);

    /// ********************************************************************************************************************
    /// Create Button will initialize all the values and return a SuGUIButton*
    ///
    /// \param name The Name and Label of this button.
    /// \param func The Function to get called when this button is pressed.
    /// \param size The size of the button. Default is (0,0). Default size will automatically adjust based on text size.
    /// \param sameLineAsLastElemet Whether this element is on the same line as the previous element.
    /// \return A object of type SuGUIButton* will be returned with everything initialized.
    /// ********************************************************************************************************************
    static SuGUIButton* CreateButton(int buttonID, SuString name, void(*func)(), SuVector2 size = SuVector2(0, 0), bool sameLineAsLastElemet = false);

    /// ********************************************************************************************************************
    /// Create Button will initialize all the values and return a SuGUIButton*
    /// I made this function during my attemps to get lua to play nice with the gui.
    ///
    /// \param name The Name and Label of this button.
    /// \param luaFunctionCallback The name of the lua function to be called when this button is pressed.
    /// \param size The size of the button. Default is (0,0). Default size will automatically adjust based on text size.
    /// \param sameLineAsLastElemet Whether this element is on the same line as the previous element.
    /// \return A object of type SuGUIButton* will be returned with everything initialized.
    /// ********************************************************************************************************************
    static SuGUIButton* CreateButton(int buttonID, SuString name, SuVector2 size = SuVector2(0, 0), bool sameLineAsLastElemet = false);

private:

    SuGUIButton();

    /// The size of the button.
    SuVector2 m_vSize;

    /// Function used to either call function of switch bool when button is pressed.
    void ButtonClicked();

    /// The bool that is to be switched on pressed.
    bool* m_bWatchBool;

    /// The function that is to be called on pressed.
    void (*functionPtr)();

    SuGUIEvent m_Event;
};

#endif