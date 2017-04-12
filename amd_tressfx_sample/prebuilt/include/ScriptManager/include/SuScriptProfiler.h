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

#ifndef _SU_LUAPROFILER_H_
#define _SU_LUAPROFILER_H_

extern "C" {
#include "lua.h"
};

#include "SuTypes.h"
#include "SuHashMap.h"
#include "SuRealTimeSource.h"

class SuCallGraphNode;

//=================================================================================================================================
/// \ingroup ScriptManager
/// \brief A profiler for lua scripts
/// 
//=================================================================================================================================
// tolua_begin
class SuScriptProfiler
{
public:

   /// Singleton access -- reference
   static SU_DLL SuScriptProfiler& GetRef();

   /// Singleton access -- pointer
   static SU_DLL SuScriptProfiler* GetPtr();
   
   /// Destructor  
   SU_DLL ~SuScriptProfiler( );

   /// Determines whether or not the profiler is profiling
   SU_DLL bool IsProfiling() const;

   /// Starts profiling
   SU_DLL void StartProfiling( ) ;

   /// Ends profiling
   SU_DLL void EndProfiling( );

// tolua_end

   /// Destroys the singleton instance
   static void Shutdown();

private:


   /// Constructor
   SuScriptProfiler( );

   /// Hook function to receive notification of lua calls and returns
   static void LuaFunctionHook( lua_State *pL, lua_Debug *pDebug );

   /// Helper method that is called on a lua function call
   void OnFunctionCall( const char* pFunctionName );

   /// Helper method that is called on a lua function return
   void OnFunctionReturn( const char* pFunctionName );

   /// Helper method to clean up any functions remaining on the profile stack when profiling is stopped
   void UnwindProfileStack();

   /// Destroys the call graph that the profiler has built
   void DestroyCallGraph();

   /// Hash map of lua function names to call graph nodes
   typedef SuHashMap< const char*, SuCallGraphNode*, SuRawStringHasher<char>, SuRawStringComparer<char> > FunctionNodeMap;

   /// The one true instance
   static SuScriptProfiler* ms_pInstance;

   /// Flag telling whether or not the profiler is profiling
   bool m_bProfiling;

   FunctionNodeMap m_callGraph;

   SuRealTimeSource m_timer;                 ///< A timer to use for profiling

   SuTime m_fProfileStart;                   ///< Profile start time
   SuArray< SuCallGraphNode* > m_callStack;  ///< Function call stack
   SuArray< SuTime > m_timeStack;            ///< Stack of function entry times


// tolua_begin
};
// tolua_end

#endif // _SU_LUAPROFILER_H_
