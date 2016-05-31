dofile ("../../../../premake/amd_premake_util.lua")

workspace "DXUTOpt"
   configurations { "Debug", "Release" }
   platforms { "x64" }
   filename ("DXUTOpt" .. _AMD_VS_SUFFIX)
   startproject "DXUTOpt"

   filter "platforms:x64"
      system "Windows"
      architecture "x64"

project "DXUTOpt"
   kind "StaticLib"
   language "C++"
   filename ("DXUTOpt" .. _AMD_VS_SUFFIX)
   uuid "61B333C2-C4F7-4cc1-A9BF-83F6D95588EB"
   targetdir "Bin/%{_AMD_SAMPLE_DIR_LAYOUT}"
   objdir "Bin/%{_AMD_SAMPLE_DIR_LAYOUT}"
   warnings "Extra"
   floatingpoint "Fast"
   pchheader "DXUT.h"
   pchsource "DXUTres.cpp"

   -- Specify WindowsTargetPlatformVersion here for VS2015
   windowstarget (_AMD_WIN_SDK_VERSION)

   files { "*.h", "*.cpp" }
   includedirs { "../Core" }

   filter "configurations:Debug"
      defines { "WIN32", "_DEBUG", "DEBUG", "PROFILE", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "Symbols", "FatalWarnings", "Unicode" }

   filter "configurations:Release"
      defines { "WIN32", "NDEBUG", "_WINDOWS", "_LIB", "_WIN32_WINNT=0x0601" }
      flags { "LinkTimeOptimization", "Symbols", "FatalWarnings", "Unicode" }
      optimize "On"
