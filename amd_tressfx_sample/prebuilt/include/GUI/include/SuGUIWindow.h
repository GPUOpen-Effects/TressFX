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

#ifndef _SUGUIWINDOW_H_
#define _SUGUIWINDOW_H_

#include "SuGUIElement.h"
#include "SuTypes.h"
#include "SuVector4.h"
#include <vector>

/// Forward Declaration
class SuGUIButton;
class SuGUIText;
class SuGUICheckBox;
class SuGUIMenu;
class SuGUISlider;
class SuGUISeparator;
class SuGUILineGraph;

// tolua_begin
typedef int SuGUIWindowFlags;

class SuGUIWindow : public SuGUIElement
{
public:

    enum SuGUIWindowFlags_
    {
        // Default: 0
        NOTITLEBAR                = 1 << 0,         // Disable title-bar
        NORESIZE                  = 1 << 1,         // Disable user resizing with the lower-right grip
        NOMOVE                    = 1 << 2,         // Disable user moving the window
        NOSCROLLBAR               = 1 << 3,         // Disable scrollbars (window can still scroll with mouse or programatically)
        NOSCROLLWITHMOUSE         = 1 << 4,         // Disable user vertically scrolling with mouse wheel
        NOCOLLAPSE                = 1 << 5,         // Disable user collapsing window by double-clicking on it
        ALWAYSAUTORESIZE          = 1 << 6,         // Resize every window to its content every frame
        SHOWBORDERS               = 1 << 7,         // Show borders around windows and items
        NOSAVEDSETTINGS           = 1 << 8,         // Never load/save settings in .ini file
        NOINPUTS                  = 1 << 9,         // Disable catching mouse or keyboard inputs
        MENUBAR                   = 1 << 10,        // Has a menu-bar
        HORIZONTALSCROLLBAR       = 1 << 11,        // Allow horizontal scrollbar to appear (off by default). You may use SetNextWindowContentSize(ImVec2(width,0.0f)); prior to calling Begin() to specify width. Read code in imgui_demo in the "Horizontal Scrolling" section.
        NOFOCUSONAPPEARING        = 1 << 12,        // Disable taking focus when transitioning from hidden to visible state
        NOBRINGTOFRONTONFOCUS     = 1 << 13,        // Disable bringing window to front when taking focus (e.g. clicking on it or programatically giving it focus)
        ALWAYSVERTICALSCROLLBAR   = 1 << 14,        // Always show vertical scrollbar (even if ContentSize.y < Size.y)
        ALWAYSHORIZONTALSCROLLBAR = 1 << 15,        // Always show horizontal scrollbar (even if ContentSize.x < Size.x)
        ALWAYSUSEWINDOWPADDING    = 1 << 16,        // Ensure child windows without border uses style.WindowPadding (ignored by default for non-bordered child windows, because more convenient)
    };

    SU_DLL ~SuGUIWindow();

    /// ********************************************************************************************************************
    /// Static member used to create an instance of a SuGUIWindow*
    ///
    /// \param name The name of the window being created.
    /// \param size The Size of the window being created (width, height).
	/// \param fontScale The scale value for the font in this window.
    /// \param position The initial starting position of the window being created.
    /// \param flags The SuGUIWindowFlags to be intial set for the window being created.
    /// \param visible Whether the window being created is visible or not.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// \return Returns SuGUIWindow*, null if it failed.
    /// ********************************************************************************************************************
    static SU_DLL SuGUIWindow* CreateGUIWindow(SuString name = SuString(""), SuVector2 size = SuVector2(50,50), SuVector2 position = SuVector2(0,0), float fontScale = 1.0f, SuGUIWindowFlags flags = SuGUIWindow::GetDefaultWindowFlags(), bool visible = true);


	/// ********************************************************************************************************************
	/// Member used to set the scale of this windows font.
	///
	/// \param fontScale The value to set the fontScale too.
	/// return void.
	/// ********************************************************************************************************************
	SU_DLL void SetFontScale(float fontScale);

    /// *******************************************************************************************************************
    /// Member used to create a SuGUIButton and add it to this window's
    /// element list.
    ///
    /// \param name Name of the button, cannot be NULL.
    /// \param luaScriptCallback The name of the lua function to be called when this button is pressed.
    /// \param size SuVector2 to determin the size of the button, will automatically adjust to fit text if (0,0) is used.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// ********************************************************************************************************************
    SU_DLL int CreateButton(SuString name, SuVector2 size = SuVector2(0, 0), bool isSameLine = false);
// tolua_end

    /// *******************************************************************************************************************
    /// Member used to create a SuGUIButton and add it to this window's
    /// element list.
    ///
    /// \param name Name of the button, cannot be NULL.
    /// \param watchBool Bool pointer to be toggled true / false when button is pressed.
    /// \param size SuVector2 to determin the size of the button, will automatically adjust to fit text if (0,0) is used.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// ********************************************************************************************************************
    SU_DLL int CreateButton(SuString name, bool* watchBool, SuVector2 size = SuVector2(0, 0), bool isSameLine = false);

    /// *******************************************************************************************************************
    /// Member used to create a SuGUIButton and add it to this window's
    /// element list.
    ///
    /// \param name Name of the button, cannot be NULL.
    /// \param func Function to be called when this button is pressed.
    /// \param size SuVector2 to determin the size of the button, will automatically adjust to fit text if (0,0) is used.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// ********************************************************************************************************************
    SU_DLL int CreateButton(SuString name, void(*func)(), SuVector2 size = SuVector2(0, 0), bool isSameLine = false);
// tolua_begin
    /// ***************************************************************
    /// Member used to create a SuGUISlider and add it to this window's
    /// element list.
    ///
    /// \param name Name Of this Element, Can not be NULL.
	/// \param width The width of the slider, 0 will automatically resize the slider based on window width.
    /// \param startingValue float used to set an intial value for the slider. Cannot be bigger then max or lower then min. default = 0.5.
    /// \param min Minimum value of the slider, default = 0.0.
    /// \param max Maximum value of the slider, defualt = 1.0.
    /// \param isSameLine Whether this element is on the same line as the previous element.
	/// \return A pointer to the SuGUISlider this creates.
    /// ***************************************************************
    SU_DLL SuGUISlider* CreateSliderFloat(SuString name,float width, float startingValue, float min, float max, bool isSameLine = false);

	/// ***************************************************************
	/// Member used to create a SuGUISlider and add it to this window's
	/// element list.
	///
	/// \param name Name Of this Element, Can not be NULL.
	/// \param width The width of the slider, 0 will automatically resize the slider based on window width.
	/// \param startingValue float used to set an intial value for the slider. Cannot be bigger then max or lower then min. default = 0.5.
	/// \param min Minimum value of the slider, default = 0.0.
	/// \param max Maximum value of the slider, defualt = 1.0.
	/// \param isSameLine Whether this element is on the same line as the previous element.
	/// \return A pointer to the SuGUISlider this creates.
	/// ***************************************************************
	SU_DLL SuGUISlider* CreateSliderInt(SuString name, float width, int startingValue, int min, int max, bool isSameLine = false);

    /// **************************************************************
    /// Member used to create an instance of a SuGUICheckBox and add it to
    /// this window's element list.
    ///
    /// \param name Name of this element, can not be NULL.
    /// \param isChecked bool used to keep trake of value, default to false.
    /// \param isSameLine Whether this element is on the same line as the previous element.
	/// \return A pointer to the SuGUICheckBox this created.
    /// **************************************************************
    SU_DLL SuGUICheckBox* CreateCheckBox(SuString name, bool isChecked = false, bool isSameLine = false);

    /// *****************************************************************
    /// Member used to create a menu withing this window then adds it to
    /// this window's element list
    ///
    /// \param name The name and Label of this menu
    /// *****************************************************************
    SU_DLL void CreateMenu(SuString name);

    /// **************************************************************************
    /// Member used to add a menu item to a already existing menu. Searches
    /// for the meny by name, then will add the item to that menu.
    ///
    /// \param menuName The name of the menu you want to add the item to.
    /// \param itemName The name of the item you are creating.
    /// \param itemShortcut The shortcut for the item you are adding. Can be NULL.
    /// \param itemSelected A bool you can link to this item to show whether its selected or not.
    /// \param itemEnabled A bool showing whether or not this item is enabled.
    /// **************************************************************************
    SU_DLL void AddMenuElement(SuString menuName, SuString itemName, SuString itemShortcut, bool* itemSelected, bool itemEnabled = true);

    /// ***********************************************************************************************
    /// Member used to create text and then add it to this window's element list.
    /// You cannot have text that is both colored and wrapped. The drawer will favor
    /// wrapped text if both are set true.
    ///
    /// \param text The text to be displayed
    /// \param color The color of the text, will set m_IsColored to be true (overriding if wrapped is also true)
    /// \param isWrapped Sets this text to be wrapped should it extend past its bounds.
    /// \param isSameLine Whether this element is on the same line as the previous element.
    /// ***********************************************************************************************
	SU_DLL SuGUIText* CreateText(SuString text, bool isSameLine = false);

	/// ********************************************************************************************************************
	/// CreateLineGraph will initialize all the values, add it to this windows element list and return a SuGUILineGraph*
	///
	/// \param name The Name and Label of this button.
	/// \param values A Pointer to the array data
	/// \param values_count The amount of values in the array
	/// \param values_offset An offset to display the values
	/// \param overlay_text You can choose to have text overlay the graph
	/// \param scale_min The minimum scale value
	/// \param scale_max The Maximum scale value
	/// \param graph_size The Size of the graph
	/// \stride The size of the values in the graph
	///
	/// \return An Object of type SuGUILineGraph
	/// ********************************************************************************************************************
	SU_DLL SuGUILineGraph* CreateLineGraph(SuString name, float* values, int values_count, int values_offset = 0, const char* overlay_text = NULL, float scale_min = FLT_MIN, float scale_max = FLT_MAX, SuVector2 graph_size = SuVector2(100, 100), int stride = 4);

    /// ******************************************************************************
    /// Member variable used to create a space in the window of default size. (Does not apear to be changable)
    /// ******************************************************************************
    SU_DLL void CreateSpace();

	/// ******************************************************************************
	/// Member variable used to create a space in the window of default size. (Does not apear to be changable)
	///
	/// \param numberOfSpaces This value will create multiple spacess at one time for convinience.
	/// ******************************************************************************
	SU_DLL void CreateSpace(float numberOfSpaces);

    /// ******************************************************************************
    /// Member variable used to create a line between GUIElements.
    /// ******************************************************************************
    SU_DLL void CreateSeparator();

    /// Sets the position of the window
    SU_DLL void SetPosition(SuVector2 newPos);

    /// Sets the size of the window.
    SU_DLL void SetSize(SuVector2 newSize);

    /// Sets weather this window is visible or not.
    SU_DLL void SetVisible(bool newVisibility);

    /// Sets the window flags for the window.
    SU_DLL void SetWindowFlags(SuGUIWindowFlags flags);

    /// Returns default window flags to be used.
    SU_DLL static SuGUIWindowFlags GetDefaultWindowFlags();

	/// Sets the minimun size this window can be.
	SU_DLL void SetMinimunSize(SuVector2 minSize);
// tolua_end

    void Draw();

protected:

private:
    /// Constructor
    SuGUIWindow();

    /// Used to add an element to the list of elements.
    void AddElement(SuGUIElement* element);

    /// The elements this window contains.
    SuArray<SuGUIElement*> windowElements;
    
    /// Whether the window is collapsed or not.
    bool m_bIsVisible;

	/// The scale of the font being displayed in this window.
	float m_nFontScale;

    /// The name of this window, displayed on the top bar.
    SuString m_sName;

    /// The size of the window.
    SuVector2 m_vSize;

    /// The position of the window. Can be changed by draggin the window if the proper flags are set.
    SuVector2 m_vPosition;

    /// The window flags
    SuGUIWindowFlags m_flags;

    /// If this window has never been drawn befor set its position.
    bool m_bFirstTimeDraw;

    /// If someone specifically set the windows position it has to be moved.
    bool m_bNewWindowPosition;

	/// The minimun size the window can be. Default is (0,0)
	SuVector2 m_vMinimumSize;

// tolua_begin
};
// tolua_end
#endif