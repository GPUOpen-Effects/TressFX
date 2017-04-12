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

#ifndef _SUGUIMANAGER_H_
#define _SUGUIMANAGER_H_

//#include <Windows.h>
#include <vector>
#include "SuTypes.h"
#include "SuInputListener.h"
#include "SuEffect.h"
#include "SuGPUResourceManager.h"
#include "SuGUIElement.h"
#include "SuGUIListener.h"

//Forwad Declaration
class SuVector2;
class SuButton;
struct ImDrawData;
class SuCamera;
class SuGUIWindow;


// tolua_begin
class SuGUIManager : public SuInputListener
{
    
public:
    /// Deconstructor
    virtual SU_DLL ~SuGUIManager();

    /// *****************************************************************
    /// Creates a new window and adds it to the element list.
    ///
    /// \param name The name of the window.
    /// \param size The Size of the window in pixels.
    /// \param position The Position of the window in window space.
    /// \return Returns a pointer to the newly created SuGUIWindow.
    ///*******************************************************************
    SU_DLL SuGUIWindow* CreateGUIWindow(SuString name, SuVector2 size, SuVector2 position);

    /// Singleton Access
    static SU_DLL SuGUIManager* GetPtr();

    /// Singleton Access
    static SU_DLL SuGUIManager&  GetRef();

	/// Delete Singleton Instance
	static SU_DLL void Shutdown();

	/// Initialize The Manager
	SU_DLL bool Initialize();
// tolua_end

	/// *****************************************************************
	/// Increments Event ID and returns it
	///
	/// \return The next available EventID
	///*******************************************************************
	int GetEventID();

    /// *********************************************************************
    /// Registers a SuGUIListener to receive SuGUIEvents
    ///
    /// \param listener The listener you will be adding to the list.
    /// *********************************************************************
    SU_DLL void RegisterGUIListener(SuGUIListener* listener);

    /// What the Render Manager should call in order to draw the GUI properly.
	void SU_DLL DrawGUI();

    /// Receive a SuGUIEvent from the GUIElements and passes it on to the listeners
    void GUIEvent(SuGUIEvent& rGE);

protected:

    /// Do not call this render funtion, it is used internal yet has to be public due to other issues
    void SU_DLL Render();

	///**********************************************************
	///     Overrides for the KeyListener
	///**********************************************************
	virtual void Focused();

	virtual void FocusLost();

	virtual void KeyPressed(SuKeyEvent& rKE);

	virtual void KeyReleased(SuKeyEvent& rKE);

	virtual void MouseMoved(SuMouseMoveEvent& rME);

private:
    /// Constructor
    SuGUIManager();

	/// EventID keeps track of what event a GUIObject will fire by assigning this value to it and incrementing 
	/// the value to avoid duplicate eventID's
	int m_EventIDCounter;

    /// **************************************************************************************************
    /// This function is used in order to properly connect ImGui with Sushi. Because it is static it wont have
    /// access to the SuGUIManager therefor it merely calls the render function of the manager and passes the
    /// data along.
    ///
    /// \param drawData The draw data acuumulated from ImGui that needs to be rendered this frame.
    /// **************************************************************************************************
    static void RenderDrawList(ImDrawData* drawData);

    /// **************************************************************************************************
    /// If the buffers aren't big enough to hold all the data they will be resized with this function.
    ///
    /// \param numVerts The number of vertices that are needed in the buffer.
    /// \param numIdx The number of indices that are needed in the buffer.
    /// **************************************************************************************************
	void ResizeBuffers(int numVerts, int numIdx);

	void BuildGUI();

	void PreRender();

    /// Boolean determining whether to allow input on the GUI.
    bool m_bInputEnabled;

    /// Keeps a count of how many times the buffers where resized.
    int m_nResizeCount;

    /// singleton instance
	static SuGUIManager* ms_Instance;       

    /// Camera Used for Orthographic projection of the GUI Elements into the backbuffer.
    SuCamera* m_pGUICam;

    /// Effect used to render the GUI.
    SuEffectPtr m_Effect;

    /// Vertex buffer used to hold on the GUI data in a given frame.
    SuGPUVertexBufferPtr m_VertexBuffer;

    /// Index buffer used to hold the GUI data in a given fram.
    SuGPUIndexBufferPtr m_IndexBuffer;

    /// A array of all the GUIElements to be drawn in a frame. Use AddGUIElement to populate.
    SuArray<SuGUIElement*> m_GUIElements;

    /// An array of all the SuGUIListeners that want to handle GUIEvents
    SuArray<SuGUIListener*> m_GUIListeners;

    /// The texture view used for the font during rendering.
    SuGPUSamplingResourceViewPtr m_GUIFontTexture;

    /// The texture of the font.
    SuGPUTexture2DArrayPtr m_Tex;

	/// The command list to render the GUI
	ImDrawData* m_im_drawData;

	bool m_DrawGUI;

// tolua_begin
};
// tolua_end
#endif