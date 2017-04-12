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

#ifndef _SUWINDOWMANAGER_H
#define _SUWINDOWMANAGER_H

#include "SuList.h"

// forward declarations
class SuInputListener;
class SuRenderListener;
class SuKeyEvent;
class SuMouseMoveEvent;
class SuDisplaySettings;

/// \defgroup WindowManager Window Manager

/// System-independent interface for window and input handling
///
/// \ingroup WindowManager
/// SuWindowManager provides an abstract interface for system-specific
/// window and input handling tasks. It follows the singleton pattern
/// and instantiates the appropriate SuWindowManager subclass for the
/// platform.

// tolua_begin
class SuWindowManager
{
// tolua_end
protected:
   /// Constructor
   SU_DLL SuWindowManager();

public:
   /// Destructor
   virtual SU_DLL ~SuWindowManager() {}

   /// Singleton access
   static SU_DLL SuWindowManager* GetPtr();      // tolua_export

   /// Singleton access
   static SU_DLL SuWindowManager& GetRef();      // tolua_export

   /// Delete singleton instance
   static SU_DLL void ShutDown();

   /// Tests whether chosen 3D API is working properly
   virtual bool Test3DAPI() const = 0;

   /// Creates a 3D window for a specific 3D API and platform
   virtual bool Create3DWindow( const SuString& rStrCardName, const SuDisplaySettings& rConfig, bool bCreateConsoleWindow = true, bool bShowWindow = true ) = 0;

   /// Shows a message box informing the user that the 3D API or window creation initialization failed
   virtual void ShowFailedWndCreationErrorMessage( const SuString& rStrCardName, const SuDisplaySettings& rConfig ) const = 0;

   /// Present the window
   virtual void PresentWindow() = 0;      // tolua_export

   /// Enters the message loop
   virtual void MessageLoop() = 0;

   /// Tells the message loop to end and exits the application
   virtual void Exit() = 0;               // tolua_export
   
   /// Shows or hides the mouse cursor
   virtual void ShowMouse( bool bShow ) = 0;       // tolua_export

   /// Returns the current mouse cursor position in absolute window coordinates
   virtual void GetMousePosition( int32* pnPosX, int32* pnPosY ) = 0;   // tolua_export

   /// Returns the current mouse cursor position in relative coordinates
   virtual void GetMousePositionRelative( float32* pfPosX, float32* pfPosY ) = 0;   // tolua_export

   /// Sets the current mouse cursor position in absolute window coordinates
   virtual void SetMousePosition( int32 nPosX, int32 nPosY ) = 0;   // tolua_export

   /// Sets the current mouse cursor position in relative coordinates
   virtual void SetMousePositionRelative( float32 fPosX, float32 fPosY ) = 0;   // tolua_export

   /// Returns a platform specific window ID to be used for nefarious purposes
   virtual uint32 GetWindowID() const = 0;         // tolua_export

   /// Returns the width of the window
   virtual uint32 GetWindowWidth() = 0;            // tolua_export

   /// Returns the height of the window
   virtual uint32 GetWindowHeight() = 0;           // tolua_export

   /// Adds a listener to receive input events
   SU_DLL void AddInputListener( SuInputListener* pListener );   // tolua_export

   /// Removes a listener to stop it from receiving input events
   SU_DLL void RemoveInputListener( SuInputListener* pListener );   // tolua_export

   /// Sets a listener to be notified when a new frame needs to be rendered
   SU_DLL void SetRenderListener( SuRenderListener* pListener );

   /// Removes a listener to stop it from receiving frame render events
   SU_DLL void RemoveRenderListener( SuRenderListener* pListener );

   /// Returns the aspect ratio of the desktop.  This is NOT necessarily the aspect ratio of the physical screen. 
   SU_DLL virtual float32 GetDesktopAspectRatio( ) const; // tolua_export

   /// Returns the width of the desktop, in pixels
   SU_DLL virtual uint32 GetDesktopWidth( ) const;        // tolua_export

   /// Returns the height of the desktop, in pixels
   SU_DLL virtual uint32 GetDesktopHeight( ) const;       // tolua_export

   /// Returns a string describing the GPU
   virtual const SuString& GetGPUDescription() const = 0; // tolua_export

   /// Returns a string describing the GPU vendor
   virtual const SuString& GetGPUVendor() const = 0; // tolua_export

   /// Returns a string describing the GPU driver
   virtual const SuString& GetGPUDriverVersion() const = 0; // tolua_export

   /// Returns the PCI Vendor ID
   virtual const uint32 GetPCIVendorID() const = 0; // tolua_export

   /// Returns the PCI Device ID
   virtual const uint32 GetPCIDeviceId() const = 0; // tolua_export

   /// Returns the PCI SubSys ID
   virtual const uint32 GetPCISubSysId() const = 0; // tolua_export

   /// Shows a platform-dependent modal message box.  This function blocks until the user dismisses the message box
   virtual SU_DLL void ShowMessageBox( const SuString& rTitle, const SuString& rMessage ) const ; // tolua_export

   /// Determines whether or not the monitor supports a particular full-screen display mode
   virtual SU_DLL bool IsDisplayModeSupported( uint32 nWidth, uint32 nHeight, uint32 nBackBufferBits, uint32 nRefreshRate = 0, bool bSRGB = false ) const; // tolua_export

   /// Determines wheter or not the window is a fullscreen window
   virtual SU_DLL bool IsFullScreen() const ; // tolua_export

   /// Toggles mouse movement blocking
   virtual SU_DLL void BlockMouseMovement( bool bBlock )  ; // tolua_export

protected:
   enum KeyEventType
   {
      KEY_PRESSED,
      KEY_RELEASED
   };

   /// Processes a key event by notifying the registered listeners
   SU_DLL void ProcessKeyEvent( SuKeyEvent& rKE, KeyEventType type );

   /// Processes a mouse move event by notifying the registered listeners
   SU_DLL void ProcessMouseMoveEvent( SuMouseMoveEvent& rME );

   /// Processes a render event by notifying the registered listener
   virtual SU_DLL void ProcessRenderEvent();

private:
   static SuWindowManager* ms_instance;       // singleton instance

   SuList<SuInputListener *> m_inputListeners;
   SuRenderListener *m_pRenderListener;
// tolua_begin
};
//tolua_end

#endif