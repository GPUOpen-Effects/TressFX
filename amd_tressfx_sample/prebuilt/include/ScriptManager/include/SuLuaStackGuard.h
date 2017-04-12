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

#ifndef _SULUASTACKGUARD_H
#define _SULUASTACKGUARD_H

extern "C" {
#include "lua.h"
};

//=================================================================================================================================
/// \brief A utility class that helps keeping the lua stack balanced within a C++ function.
///
/// SuLuaStackGuard keeps track of the height of the lua stack at the beginning of a
/// function and resets it to the original height when the function terminates.
/// Usage:
/// \code
/// int foo()
/// {
///    SuLuaStackGuard guard( pMyLuaState );
///
///    // do stuff with lua stack here
///    // guard will reset the stack height whenever the function terminates.
/// }
/// \endcode
//=================================================================================================================================
class SuLuaStackGuard
{
public:
   /// Constructor
   SuLuaStackGuard( lua_State* pL )
      : m_pLuaState( pL ),
        m_nSavedStackHeight( 0 )
   {
      SU_ASSERT( m_pLuaState != NULL );
      m_nSavedStackHeight = lua_gettop( m_pLuaState );
   }

   /// Destructor
   ~SuLuaStackGuard ()
   {
      ResetStack();
   }

   /// Resets the Lua stack to the size it was when the stack guard object was instantiated
   void ResetStack ()
   {
      lua_settop( m_pLuaState, m_nSavedStackHeight );
   }

private:
   lua_State* m_pLuaState;
   int32 m_nSavedStackHeight;
};

#endif
