dofile ("../../premake/amd_premake_util.lua")

workspace "AMD_LIB"
   configurations { "Debug", "Release" }
   platforms { "Win32", "x64" }
   location "../build"
   filename ("AMD_LIB" .. _AMD_VS_SUFFIX)
   startproject "AMD_LIB"

   filter "platforms:Win32"
      system "Windows"
      architecture "x86"

   filter "platforms:x64"
      system "Windows"
      architecture "x64"

project "AMD_LIB"
   kind "StaticLib"
   language "C++"
   location "../build"
   filename ("AMD_LIB" .. _AMD_VS_SUFFIX)
   uuid "0D2AEA47-7909-69E3-8221-F4B9EE7FCF44"
   targetdir "../lib"
   objdir "../build/%{_AMD_LIBRARY_DIR_LAYOUT}"
   warnings "Extra"
   floatingpoint "Fast"

   -- Specify WindowsTargetPlatformVersion here for VS2015
   windowstarget (_AMD_WIN_SDK_VERSION)

   files { "../inc/**.h", "../src/**.h", "../src/**.cpp", "../src/**.inl", "../src/**.hlsl", "../../AGS_LIB/inc/*.h" }
   includedirs { "../inc", "../../AGS_LIB/inc" }

   filter "configurations:Debug"
      defines { "WIN32", "_DEBUG", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "Symbols", "FatalWarnings", "Unicode" }
      -- add "d" to the end of the library name for debug builds
      targetsuffix "d"

   filter "configurations:Release"
      defines { "WIN32", "NDEBUG", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "FatalWarnings", "Unicode" }
      optimize "On"

   filter "platforms:Win32"
      targetname "amd_lib_x86%{_AMD_VS_SUFFIX}"

   filter "platforms:x64"
      targetname "amd_lib_x64%{_AMD_VS_SUFFIX}"
