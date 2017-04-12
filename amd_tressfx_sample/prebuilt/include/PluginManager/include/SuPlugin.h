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

#ifndef _SU_PLUGIN_H_
#define _SU_PLUGIN_H_

struct lua_State;

//=================================================================================================================================
/// \ingroup PluginManager
/// \brief A base class for a sushi plugin
/// 
//=================================================================================================================================
// tolua_begin
class SuPlugin
{
public:

   //=================================================================================================================================
   /// \brief Returns the name of the subclass of this plugin.  
   ///
   /// In order to access plugin subclass methods from script, it is necessary to have tolua cast the plugin to its actual type
   ///  after the plugin's script bindings have been registered.  This is done by the lua function 'Sushi.LoadPlugin'
   ///  In order to make this work, each plugin subclass must provide its class name so that we know what type to cast it to.
   ///
   /// \return A pointer to a string containing this plugin's subclass name.  The return type is const char* rather than SuString
   ///          to avoid requiring dependencies on SuString where they are not strictly necessary.  Using const char* also
   ///          makes it simpler to pass the returned string to tolua.cast()
   //=================================================================================================================================
   virtual const char* GetPluginClassName() = 0;

// tolua_end

   virtual SU_DLL ~SuPlugin() {};

   //=================================================================================================================================
   /// \brief Method that is called after the plugin has been loaded.  
   ///  The implementation of this method should use the specified lua state to register plugin-specific tolua bindings
   //=================================================================================================================================
   virtual bool Initialize( lua_State* pLua ) = 0;

   //=================================================================================================================================
   /// \brief Method that is called to unload the plugin
   ///
   ///   This method will be called by the plugin manager when the specified plugin is being unloaded.  This signals the plugin to
   ///   release any resources that it holds and close itself down.  The treatment of the memory occupied by the SuPlugin instance
   ///   itself is left up to the plugin implementation.  After calling Unload, the plugin manager will treat the SuPlugin object as
   ///   having been destructed, (meaning that it will simply discard its pointer to it).
   ///   
   ///   If the plugin implementation allocated the SuPlugin object on the heap for some reason, then the implementation must ensure 
   ///   that the object is deleted, either by self-deleting in the SuPlugin::Unload() implementation, or by deallocating in response to
   ///   the DLL_PROCESS_DETACH message in a DLLMain
   ///
   //=================================================================================================================================
   virtual void Unload( ) = 0;

// tolua_begin
};
// tolua_end


#endif // _SU_PLUGIN_H_
