dofile ("../../../../premake/amd_premake_util.lua")

workspace "AMD_LIB_Minimal"
   configurations { "Debug", "Release", "Release_MT" }
   platforms { "Win32", "x64" }
   location "../build"
   filename ("AMD_LIB_Minimal" .. _AMD_VS_SUFFIX)
   startproject "AMD_LIB_Minimal"

   filter "platforms:Win32"
      system "Windows"
      architecture "x86"

   filter "platforms:x64"
      system "Windows"
      architecture "x64"

project "AMD_LIB_Minimal"
   kind "StaticLib"
   language "C++"
   location "../build"
   filename ("AMD_LIB_Minimal" .. _AMD_VS_SUFFIX)
   uuid "0D2AEA47-7909-69E3-8221-F4B9EE7FCF44"
   targetdir "../lib"
   objdir "../build/%{_AMD_LIBRARY_DIR_LAYOUT_MINIMAL}"
   warnings "Extra"
   floatingpoint "Fast"
   exceptionhandling "Off"
   rtti "Off"

   -- Specify WindowsTargetPlatformVersion here for VS2015
   windowstarget (_AMD_WIN_SDK_VERSION)

   files { "../inc/**.h", "../src/**.h", "../src/**.cpp", "../src/**.inl", "../src/**.hlsl", "../../common/inc/*.h" }
   includedirs { "../inc", "../../common/inc" }
   defines { "AMD_LIB_MINIMAL" }

   filter "configurations:Debug"
      defines { "WIN32", "_DEBUG", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "Symbols", "FatalWarnings", "Unicode" }
      -- add "d" to the end of the library name for debug builds
      targetsuffix "d"

   filter "configurations:Release"
      defines { "WIN32", "NDEBUG", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "FatalWarnings", "Unicode" }
      optimize "On"

   filter "configurations:Release_MT"
      defines { "WIN32", "NDEBUG", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "FatalWarnings", "Unicode" }
      -- link against the static runtime to avoid introducing a dependency
      -- on the particular version of Visual Studio used to build the DLLs
      flags { "StaticRuntime" }
      -- add "mt" to the end of the library name for Release_MT builds
      targetsuffix "mt"
      optimize "On"

   filter "action:vs*"
      -- specify exception handling model for Visual Studio to avoid
      -- "'noexcept' used with no exception handling mode specified" 
      -- warning in vs2015
      buildoptions { "/EHsc" }

   filter "platforms:Win32"
      targetname "amd_lib_minimal_x86%{_AMD_VS_SUFFIX}"

   filter "platforms:x64"
      targetname "amd_lib_minimal_x64%{_AMD_VS_SUFFIX}"
