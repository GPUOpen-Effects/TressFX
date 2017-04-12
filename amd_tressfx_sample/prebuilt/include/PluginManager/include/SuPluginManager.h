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

#ifndef _SU_PLUGINMANAGER_H_
#define _SU_PLUGINMANAGER_H_

#include "SuSearchPath.h"
#include "SuHashMap.h"

/// \defgroup PluginManager Plugin Manager

class SuPlugin;

//=================================================================================================================================
/// \ingroup PluginManager
/// \brief The plugin manager is a singleton class which is responsible for loading and unloading plugins
/// 
///   Plugins are standalone shared libraries (DLLs on windows) which are able to register script functions with the engine.  
///   Plugin implementations will implement their own subclasses of the SuPlugin class, and the draw script will be able to use
///    functions exposed by the plugin by issuing method calls on the SuPlugin object.  
///
///   For example, here is a class definition that might exist in a plugin DLL
///
///     // tolua_begin
///     class myPlugin : public SuPlugin
///     {
///         void Foo();
///         void Bar();
///     };
///     // tolua_end
///         
///   A draw script could access the functionality provided by the plugin by calling Sushi.LoadPlugin(), and then issuing a method
///    call on the returned plugin object.  For example:
/// 
///      plug = Sushi.LoadPlugin( "myPlugin" )  // loads myPlugin.dll
///      plug:Foo()                             // calls myPlugin::Foo()
///      plug:Bar()                             // calls myPlugin::Bar()
///
///   
//=================================================================================================================================
// tolua_begin
class SuPluginManager
{
public:
// tolua_end

   /// Destructor
   virtual SU_DLL ~SuPluginManager() {};

   /// Singleton access - pointer
   static SU_DLL SuPluginManager* GetPtr();   // tolua_export 

   /// Singleton access - reference
   static SU_DLL SuPluginManager& GetRef();

   /// Destroys the plugin manager.  This explicitly unloads any loaded plugins
   static SU_DLL void ShutDown();

   /// Loads a sushi plugin
   SU_DLL SuPlugin* LoadPlugin( const SuString& rPluginName );   // tolua_export

   /// Unloads a sushi plugin
   SU_DLL void UnloadPlugin( SuPlugin* pPlugin ); // tolua_export

   /// Adds a directory to the plugin search path
   SU_DLL void AddSearchPath( const SuString& rPath );          // tolua_export


protected:

   /// Constructor is protected to restrict instantiation to singleton access methods
   SuPluginManager() {};

   /// Helper method used to search for plugins in the plugin search path
   bool GetPluginPath( const SuString& rPluginName, SuString& rPluginPath );

   /// Performs platform-specific plugin loading
   virtual SuPlugin* DoPluginLoad( const SuString& rPluginName );

   /// Called when the plugin manager is instantiated to perform platform-specific initialization.  
   virtual void Initialize() {  }

   /// Performs platform-specific plugin cleanup
   virtual void OnPluginUnload( SuPlugin* pPlugin );

private:

   SuSearchPath m_searchPath;

   typedef SuHashMap< SuString, SuPlugin*, SuStringHasher, SuHashComparer<SuString> > SuPluginMap;
   SuPluginMap m_loadedPlugins;

   static SuPluginManager* ms_pInstance;  ///< Singleton instance pointer

// tolua_begin
};
// tolua_end

#endif // _SU_PLUGINMANAGER_H_
