_AMD_LIBRARY_NAME = "TressFX"
_AMD_LIBRARY_NAME_ALL_CAPS = string.upper(_AMD_LIBRARY_NAME)

-- Set _AMD_LIBRARY_NAME before including amd_premake_util.lua
dofile ("../../premake/amd_premake_util.lua")

_AMD_LIBRARY_PROJECT_NAME = "AMD_" .. _AMD_LIBRARY_NAME

workspace ("AMD_" .. _AMD_LIBRARY_NAME)
   -- We are not supporting DLL builds at this time.
   configurations { "Lib_Debug", "Lib_Release" }
   platforms { "Win32", "x64" }
   location "../build"
   filename ("AMD_" .. _AMD_LIBRARY_NAME .. _AMD_VS_SUFFIX)
   startproject ("AMD_" .. _AMD_LIBRARY_NAME)

   filter "platforms:Win32"
      system "Windows"
      architecture "x86"

   filter "platforms:x64"
      system "Windows"
      architecture "x64"


project (_AMD_LIBRARY_PROJECT_NAME)
   configurations { "DLL_Debug", "DLL_Release", "Lib_Debug", "Lib_Release", "DLL_Release_MT" }
   language "C++"
   location "../build"
   filename ("AMD_" .. _AMD_LIBRARY_NAME .. _AMD_VS_SUFFIX)
   uuid "252C0AF0-91E1-82E5-1AD6-7CBC868A79E9"
   targetdir "../lib/%{_AMD_LIBRARY_DIR_LAYOUT}"
   objdir "../build/%{_AMD_LIBRARY_DIR_LAYOUT}"
   characterset "Unicode"
   --warnings "Extra"
   --exceptionhandling "Off"
   --rtti "Off"

   -- Specify WindowsTargetPlatformVersion here for VS2015
   --windowstarget (_AMD_WIN_SDK_VERSION)

   files { "../inc/**.h", "../src/**.h", "../src/**.cpp", "../src/Shaders/**.hlsl" }
   includedirs { "../inc" }

  -- kind "StaticLib"

   filter "configurations:DLL_*"
      kind "SharedLib"
      defines { "_USRDLL", "AMD_%{_AMD_LIBRARY_NAME_ALL_CAPS}_COMPILE_DYNAMIC_LIB=1" }
      -- Copy DLL and import library to the lib directory
      postbuildcommands { amdLibPostbuildCommands() }
      postbuildmessage "Copying build output to lib directory..."
   
   filter "configurations:Lib_*"
      kind "StaticLib"
      defines { "_LIB", "AMD_%{_AMD_LIBRARY_NAME_ALL_CAPS}_COMPILE_DYNAMIC_LIB=0" }
   
   filter "configurations:*_Debug"
      defines { "WIN32", "_DEBUG", "_WINDOWS", "_WIN32_WINNT=0x0601", "_SCL_SECURE_NO_WARNINGS" }
      flags { "FatalWarnings" }
      symbols "On"
      -- add "d" to the end of the library name for debug builds
      targetsuffix "d"
   
   filter "configurations:*_Release"
      defines { "WIN32", "NDEBUG", "_WINDOWS", "_WIN32_WINNT=0x0601", "_SCL_SECURE_NO_WARNINGS" }
      flags { "FatalWarnings" }
      symbols "Off"
      optimize "On"
   
   filter "configurations:DLL_Release_MT"
      defines { "WIN32", "NDEBUG", "_WINDOWS", "_WIN32_WINNT=0x0601", "_SCL_SECURE_NO_WARNINGS" }
      flags { "FatalWarnings" }
      -- link against the static runtime to avoid introducing a dependency
      -- on the particular version of Visual Studio used to build the DLLs
      flags { "StaticRuntime" }
      symbols "Off"
      optimize "On"
   
   filter "action:vs*"
      -- specify exception handling model for Visual Studio to avoid
      -- "'noexcept' used with no exception handling mode specified" 
      -- warning in vs2015
      buildoptions { "/EHsc" }
   
   filter "platforms:Win32"
      targetname "%{_AMD_LIBRARY_PREFIX}%{_AMD_LIBRARY_NAME}_x86"
   
   filter "platforms:x64"
      targetname "%{_AMD_LIBRARY_PREFIX}%{_AMD_LIBRARY_NAME}_x64"
   