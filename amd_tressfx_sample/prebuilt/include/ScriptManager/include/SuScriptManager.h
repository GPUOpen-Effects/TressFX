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

#ifndef _SUSCRIPTMANAGER_H
#define _SUSCRIPTMANAGER_H

#include "SuList.h"
#include "SuMutex.h"

struct lua_State; // forward declaration

class SuScriptManager
{
public:
   /// Destructor
   virtual SU_DLL ~SuScriptManager();

   /// Singleton access
   static SU_DLL SuScriptManager* GetPtr();

   /// Singleton access
   static SU_DLL SuScriptManager& GetRef();

   /// Shuts down the scripting layer and deletes the singleton
   static SU_DLL void ShutDown();

   /// Loads a script file and executes its Initialize() function
   SU_DLL bool LoadScript( const SuString& rFileName );

   /// Executes a Lua command
   SU_DLL bool ExecuteCommand( const SuString& rCmd );

   /// Executes a chunk of Lua code, possibly within a specific Lua environment
   SU_DLL bool ExecuteCode( const SuString& rCmd, const SuString* pEnvironmentName = NULL );

   /// Adds a path to the script include search path
   SU_DLL void AddIncludeDirectory( const SuString& rDir );

   /// Registers a Lua function library by calling the supplied registration function
   SU_DLL bool RegisterLibrary( int32 (*registerFunc)(lua_State *) );

   /// Sets a lua variable to an instance of a tolua-exposed type
   SU_DLL bool SetVariable( const SuString& rVarName, void* pValue, const SuString& rTypeName );

   /// Sets a lua variable to a string
   SU_DLL bool SetVariable( const SuString& rVarName, const SuString& rStr );

   /// Sets a lua variable to a bool
   SU_DLL bool SetBoolVariable( const SuString& rVarName, bool bVal );

   /// Returns the value of a Lua string variable
   SU_DLL SuString GetStringVariable( const SuString& rVarName, const SuString& rDefault, bool* pFound = NULL );

   /// Returns the value of a Lua boolean variable
   SU_DLL bool GetBoolVariable( const SuString& rVarName, bool bDefault, bool* pFound = NULL );

   /// Returns the value of a Lua number variable
   SU_DLL float32 GetFloatVariable( const SuString& rVarName, float32 fDefault, bool* pFound = NULL );

   /// Returns the value of a Lua userdata variable (and its type)
   SU_DLL void* GetUserDataVariable( const SuString& rVarName, SuString& rTypeName );

   /// Calls a script function, passing one argument
   SU_DLL bool CallFunction( const SuString& rFuncName, void* pArg, const SuString& rArgTypeName );

   /// Calls a script function, passing two arguments
   SU_DLL bool CallFunction( const SuString& rFuncName, void* pArg1, void* pArg2, const SuString& rArgType1, const SuString& rArgType2 );

   /// Returns the script manager's lua state
   ///
   /// USE THIS LUA STATE ONLY WHEN YOU KNOW WHAT YOU'RE DOING! Messing up the lua state can
   /// compromise the internal consistency of the script manager.
   inline lua_State* GetLuaState() { return m_luaState; }

   /*
   bool8 GetShaderParam(const char8 *paramName, SSParamType type, void *data);
   bool8 GetShaderParamRenderTargetID (const char8 *paramName, int32 *rtID);
   */

   /// Reloads all scripts
   SU_DLL bool ReloadAllScripts();
   
   /// Forces a full garbage collection
   SU_DLL void CollectGarbage();

   /// Disables automatic garbage collection
   SU_DLL void DisableGarbageCollection();

   /// Adds string message to message queue. See declaration on thread safety.
   SU_DLL bool AddMessage(SuString&);

   SU_DLL bool ClearQueue();

   // Process message queue
   SU_DLL void ProcessMessages();

   /// Adds string message to message queue. See declaration on thread safety.
   SU_DLL bool GetReturn(SuString&);

   SU_DLL void SetNumMsgPerFrame(uint32 num) { m_numMsgLoop = num; }

private:
   /// Constructor
   SuScriptManager();

   /// Initializes the Lua state
   bool InitializeLua();

   bool InitLuaJIT();

   bool LoadScriptFile( const SuString& rFileName );
   
   
   bool LoadIncludeFile( const SuString& rFileName );
   
   /// Mangles a string so it's suitable as a Lua variable name
   SuString MangleString( const SuString& rStr );

   bool CallScriptInitialize( const SuString& rFileName );
   
   bool InitializeAllRegisteredLibraries();

   /// Prepares the Lua stack for setting the value of a variable.
   bool PushNestedVariable( const SuString& rVarName );

   static int32 LoadScriptFileWrapper( lua_State* pLuaState );

   /// Lua memory allocation callback
   static void* LuaAllocator( void* pUD, void* pPtr, size_t nOrigSize, size_t nNewSize );

   // disallow copying by assignment
   SuScriptManager( const SuScriptManager & );
   int operator=( const SuScriptManager & );
	
   /// Parse and execute string
   void ParseExecute( SuString& rCmd );

   // Removes string message to message queue. See declaration on thread safety.
   SU_DLL bool RemoveMessage(SuString&);

private:
   static SuScriptManager* ms_pInstance;
   
   lua_State* m_luaState;

   SuStringList  m_loadedScriptFiles;
   SuStringArray m_drawScriptEntryPoints;
   SuStringList  m_includeDirs;
   SuList<int32 (*)(lua_State *)> m_libraryRegistrationFunctions;

   /// Message Queue for live connection
   SuStringList m_msgQueue;
   SuMutex      m_msgMutex;

   SuString     m_retString;
   SuMutex      m_retMutex;

   uint32       m_numMsgLoop;
};

/// Get Lua VM info.  This is useful when debugging in Visual Studio (C++).  Add as Watch variable with {,,SushiCoreD3D11_d.dll}GetInfo(). 
const char* GetLuaInfo();

#endif
