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

#ifndef _SUKEYEVENT_H
#define _SUKEYEVENT_H

#include "SuTypes.h"

/// \brief Event class for keyboard events sent out by the window manager
///
/// \ingroup WindowManager

// tolua_begin
class SuKeyEvent
{
public:
   /// Constuctor
   SU_DLL SuKeyEvent( int32 nKeyID, int32 nKeyChar, int32 nModifiers );

   /// Keyboard modifier flags
   enum Modifier
   {
      MODIFIER_SHIFT = 0x01,
      MODIFIER_ALT   = 0x02,
      MODIFIER_CTRL  = 0x04
   };

   /// Key codes of special keys
   enum Key
   {
      KEY_0 = '0',
      KEY_1 = '1',
      KEY_2 = '2',
      KEY_3 = '3',
      KEY_4 = '4',
      KEY_5 = '5',
      KEY_6 = '6',
      KEY_7 = '7',
      KEY_8 = '8',
      KEY_9 = '9',

      KEY_A = 'A',
      KEY_B = 'B',
      KEY_C = 'C',
      KEY_D = 'D',
      KEY_E = 'E',
      KEY_F = 'F',
      KEY_G = 'G',
      KEY_H = 'H',
      KEY_I = 'I',
      KEY_J = 'J',
      KEY_K = 'K',
      KEY_L = 'L',
      KEY_M = 'M',
      KEY_N = 'N',
      KEY_O = 'O',
      KEY_P = 'P',
      KEY_Q = 'Q',
      KEY_R = 'R',
      KEY_S = 'S',
      KEY_T = 'T',
      KEY_U = 'U',
      KEY_V = 'V',
      KEY_W = 'W',
      KEY_X = 'X',
      KEY_Y = 'Y',
      KEY_Z = 'Z',

      KEY_SEMICOLON = ';',
      KEY_PLUS = '+',
      KEY_MINUS = '-',
      KEY_COMMA = 44, // ',' Stupid tolua chokes on the comma character literal
      KEY_PERIOD = '.',
      KEY_SLASH = '/',
      KEY_TILDE = '~', // BACKTICK?
      KEY_LEFT_BRACKET = '[',
      KEY_RIGHT_BRACKET = ']',
      KEY_BACKSLASH = '\\',
      KEY_QUOTE = '\"',
      KEY_SPACE = ' ',

      // We pass keys back as a full 32 bit int, by ignoring the lower
      // 16 bits we can avoid any problems with the virtual keys and UNICODE keys.
      KEY_ADD             = 0x00010000,
      KEY_ALT             = 0x00020000,
      KEY_BACKSPACE       = 0x00030000,
      KEY_CAPS_LOCK       = 0x00040000,
      KEY_CONTROL         = 0x00050000,
      KEY_DECIMAL         = 0x00060000,
      KEY_DELETE          = 0x00070000,
      KEY_DIVIDE          = 0x00080000,
      KEY_DOWN            = 0x00090000,
      KEY_END             = 0x000a0000,
      KEY_ENTER           = 0x000b0000,
      KEY_ESCAPE          = 0x000c0000,
      KEY_F1              = 0x000d0000,
      KEY_F2              = 0x000e0000,
      KEY_F3              = 0x000f0000,
      KEY_F4              = 0x00100000,
      KEY_F5              = 0x00110000,
      KEY_F6              = 0x00120000,
      KEY_F7              = 0x00130000,
      KEY_F8              = 0x00140000,
      KEY_F9              = 0x00150000,
      KEY_F10             = 0x00160000,
      KEY_F11             = 0x00170000,
      KEY_F12             = 0x00180000,
      KEY_HOME            = 0x00190000,
      KEY_INSERT          = 0x001a0000,
      KEY_LEFT            = 0x001b0000,
      KEY_META            = 0x001c0000,
      KEY_MULTIPLY        = 0x001d0000,
      KEY_NUM_LOCK        = 0x001e0000,
      KEY_NUMPAD0         = 0x001f0000,
      KEY_NUMPAD1         = 0x00200000,
      KEY_NUMPAD2         = 0x00210000,
      KEY_NUMPAD3         = 0x00220000,
      KEY_NUMPAD4         = 0x00230000,
      KEY_NUMPAD5         = 0x00240000,
      KEY_NUMPAD6         = 0x00250000,
      KEY_NUMPAD7         = 0x00260000,
      KEY_NUMPAD8         = 0x00270000,
      KEY_NUMPAD9         = 0x00280000,
      KEY_PAGE_DOWN       = 0x00290000,
      KEY_PAGE_UP         = 0x002a0000,
      KEY_PAUSE           = 0x002b0000,
      KEY_PRINTSCREEN     = 0x002c0000,
      KEY_RIGHT           = 0x002d0000,
      KEY_SCROLL_LOCK     = 0x002e0000,
      KEY_SHIFT           = 0x002f0000,
      KEY_SUBTRACT        = 0x00300000,
      KEY_TAB             = 0x00310000,
      KEY_UP              = 0x00320000,
      KEY_MOUSE_LEFT      = 0x00330000,
      KEY_MOUSE_RIGHT     = 0x00340000,
      KEY_MOUSE_MIDDLE    = 0x00350000,
      KEY_MOUSE_WHEEL_UP  = 0x00360000,
      KEY_MOUSE_WHEEL_DOWN= 0X00370000
   };

   /// Get the keycode of the key that triggered the event
   inline int32 GetKey() const { return m_nKeyID; }

   /// Get the character generated by the key
   ///
   /// \return The ASCII code of the character or -1 if the keypress couldn't be translated
   ///         into an ASCII character.
   inline int32 GetKeyChar() const { return m_nTranslatedChar; }
   
   /// Get the modifiers that were pressed together with the key
   inline int32 GetModifiers() const { return m_nModifiers; }

   /// Returns whether the SHIFT key was pressed when the key event happened
   SU_DLL bool IsShiftPressed() const;

   /// Returns whether the ALT key was pressed when the key event happened
   SU_DLL bool IsAltPressed() const;

   /// Returns whether the CTRL key was pressed when the key event happened
   SU_DLL bool IsCtrlPressed() const;
   
   /// Checks whether the key event has been processed by an event handler
   inline bool IsAccepted() const { return m_bAccepted; }

   /// Set the event's accept flag
   inline void SetAccepted( bool bAccept ) { m_bAccepted = bAccept; }

   /// Convenience method to accept the event from within an event handler
   inline void Accept() { SetAccepted( true ); }

   /// Convenience method to ignore the event from within an event handler
   inline void Ignore() { SetAccepted( false ); }

   /// In case of mouse press, release or wheel event, the following methods give access to mouse position and wheel delta value. 
   inline int32 GetMousePosX() const { return m_nMousePosX; }
   inline int32 GetMousePosY() const { return m_nMousePosY; }
   inline int32 GetMouseWheelDelta() const { return m_nMouseWheelDelta; }
   inline void SetMousePosX(int32 posX) { m_nMousePosX = posX; }
   inline void SetMousePosY(int32 posY) { m_nMousePosY = posY; }
   inline void SetMouseWheelDelta(int32 wheelDelta) { m_nMouseWheelDelta = wheelDelta; }
// tolua_end

private:
   int32 m_nKeyID;
   TCHAR m_nTranslatedChar;
   int32 m_nModifiers;
   bool  m_bAccepted;

   /// If the key event was thrown by mouse press, release or wheel, mouse position (x, y) and/or wheel delta 
   /// value will be set to the following variables. 
   int32 m_nMousePosX;
   int32 m_nMousePosY;
   int32 m_nMouseWheelDelta;
};    // tolua_export

#endif
