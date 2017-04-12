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

#ifndef _SUMOUSEMOVEEVENT_H
#define _SUMOUSEMOVEEVENT_H

#include "SuTypes.h"
#include "SuWindowManager.h"

/// \brief Event class for mouse move events sent out by the window manager
///
/// \ingroup WindowManager

// tolua_begin
class SuMouseMoveEvent
{
public:
   /// Constuctor
   inline SuMouseMoveEvent( int32 nPosX, int32 nPosY )
      : m_nPosX( nPosX ), m_nPosY( nPosY ), m_bMouseLeftPressed(false), m_bMouseMiddlePressed(false), m_bMouseRightPressed(false)
   {
   }
  
   /// Returns the mouse cursor X coordinate in absolute window coordinates
   inline int32 GetX() const { return m_nPosX; }

   /// Returns the mouse cursor Y coordinate in absolute window coordinates
   inline int32 GetY() const { return m_nPosY; }

   /// Returns the mouse cursor X coordinate in relative coordinates.
   ///
   /// \return A number between 0 and 1: 0 means the cursor is at the left edge of the window, 1 means it is on the
   ///         right edge.
   inline float32 GetRelativeX() const { return static_cast<float32>( m_nPosX )/SuWindowManager::GetRef().GetWindowWidth(); }

   /// Returns the mouse cursor Y coordinate in relative coordinates.
   ///
   /// \return A number between 0 and 1: 0 means the cursor is at the top edge of the window, 1 means it is on the
   ///         bottom edge.
   inline float32 GetRelativeY() const { return static_cast<float32>( m_nPosY )/SuWindowManager::GetRef().GetWindowHeight(); }

   /// Checks whether the key event has been processed by an event handler
   inline bool IsAccepted() const { return m_bAccepted; }

   /// Set the event's accept flag
   inline void SetAccepted( bool bAccept ) { m_bAccepted = bAccept; }

   /// Convenience method to accept the event from within an event handler
   inline void Accept() { SetAccepted( true ); }

   /// Convenience method to ignore the event from within an event handler
   inline void Ignore() { SetAccepted( false ); }

   bool IsMouseLeftPressed() const { return m_bMouseLeftPressed; }
   bool IsMouseMiddlePressed() const { return m_bMouseMiddlePressed; }
   bool IsMouseRightPressed() const { return m_bMouseRightPressed; }

   void SetMouseLeftPressed(bool bPressed) { m_bMouseLeftPressed = bPressed; }
   void SetMouseMiddlePressed(bool bPressed) { m_bMouseMiddlePressed = bPressed; }
   void SetMouseRightPressed(bool bPressed) { m_bMouseRightPressed = bPressed; }

// tolua_end

private:
   int32 m_nPosX;
   int32 m_nPosY;
   bool  m_bAccepted;

   bool m_bMouseLeftPressed;
   bool m_bMouseMiddlePressed;
   bool m_bMouseRightPressed;

};    // tolua_export

#endif
