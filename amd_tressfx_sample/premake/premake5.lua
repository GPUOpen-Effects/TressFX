
-- Builds project files for the sample for either DX11 or DX12.

newoption {
   trigger     = "gfxapi",
   value       = "API",
   description = "Choose a particular 3D API for rendering",
   allowed = {
      { "d3d11",  "D3D11" },
      { "d3d12",  "D3D12" }
   }
}
if not _OPTIONS["gfxapi"] then
	_OPTIONS["gfxapi"] = "d3d12"
end

API_SUFFIX = "D3D12"

if _OPTIONS["gfxapi"] == "d3d11" then
	API_SUFFIX = "D3D11"
end

PLUGIN_NAME = "SuTressFXPlugin"

_AMD_LIBRARY_NAME = "TressFX"
_AMD_LIBRARY_NAME_ALL_CAPS = string.upper(_AMD_LIBRARY_NAME)
_AMD_SAMPLE_NAME = _AMD_LIBRARY_NAME.."_Sample"
_AMD_D3D_VERSION = _OPTIONS["gfxapi"]


-- Set _AMD_LIBRARY_NAME and _AMD_D3D_VERSION before including amd_premake_util.lua
dofile ("../../premake/amd_premake_util.lua")
dofile( "../prebuilt/include/PrebuiltSushiSettings.lua" )

_AMD_LONG_DIR_LAYOUT = "%{_AMD_ACTION_ALL_CAPS}/%{_AMD_D3D_VERSION}/%{cfg.platform}/%{cfg.buildcfg}"
-- For build outputs that have suffixes to avoid name clashes.
_AMD_SHORT_DIR_LAYOUT = "%{_AMD_ACTION_ALL_CAPS}/%{cfg.platform}"
_AMD_SAMPLE_DLL_NAME = "AMD_" .. _AMD_SAMPLE_NAME .. API_SUFFIX .. _AMD_VS_SUFFIX
_AMD_SAMPLE_DLL_TARGETDIR = '../build/'.._AMD_SHORT_DIR_LAYOUT
_AMD_SAMPLE_DLL_PROJECTNAME = PLUGIN_NAME..API_SUFFIX

PLUGIN_DIR = "../"
PLUGIN_SRC_DIR = PLUGIN_DIR.."src/"
SUSHI_ROOT = "../prebuilt/"
SUSHI_INCLUDE = "../prebuilt/include/"
SUSHI_BIN = "../prebuilt/bin/VS2015/%{cfg.platform}/"
EFFECT_ROOT = "..\\..\\"
WORKING_DIR = EFFECT_ROOT.."amd_tressfx_sample\\bin\\"
SUFFIX = API_SUFFIX

TRESSFX_DIR = "../../amd_tressfx/"

workspace ("AMD_" .. _AMD_SAMPLE_NAME .. API_SUFFIX)
	filename(_AMD_SAMPLE_DLL_NAME)
	configurations { "Debug", "Release" }
	platforms { "x64" }
	--linkoptions { "/LTCG" }
	symbols "On"
	architecture "x86_64"
	flags { "MultiProcessorCompile" }
	systemversion (_AMD_WIN_SDK_VERSION)
	characterset "MBCS"
	location ("../build")
	startproject ("AMD_" .. _AMD_LIBRARY_NAME)

	defines { 
		"WIN32", 
		"WINDOWS", 
		"_WINDOWS",
		"_CRT_NONSTDC_NO_DEPRECATE",
		"_CRT_SECURE_NO_DEPRECATE",
		"_CRT_NONSTDC_NO_WARNINGS"
	}
  
	objdir ('../build/'.._AMD_LONG_DIR_LAYOUT)
	targetdir ( _AMD_SAMPLE_DLL_TARGETDIR )

	filter "kind:StaticLib"
		defines "_LIB"

	filter "configurations:Debug" 
		defines { "_DEBUG" }

	filter "configurations:Release"
		defines { "NDEBUG" }
		optimize "Speed"
	  
	filter "options:gfxapi=d3d11"
		defines { "SU_3D_API_D3D11" }
		libdirs { D3D11_SDK_DIR.."/Lib/x64" }
	
	filter "options:gfxapi=d3d12"
		defines { "SU_3D_API_D3D12" }
		libdirs { D3D12_SDK_DIR.."/Lib/x64" }

externalproject ("AMD_" .. _AMD_LIBRARY_NAME)
   kind "StaticLib"
   language "C++"
   location "../../amd_tressfx/build"
   filename ("AMD_" .. _AMD_LIBRARY_NAME .. _AMD_VS_SUFFIX)
   uuid "252C0AF0-91E1-82E5-1AD6-7CBC868A79E9"
   configmap {
      ["Debug"] = "Lib_Debug",
      ["Release"] = "Lib_Release" }

	project (PLUGIN_NAME..API_SUFFIX)
		filename('%{prj.name}'.._AMD_VS_SUFFIX)
		location ("../build")
		kind ("SharedLib")
		filter {}
		language "C++"
		vpaths { 
			["Demo/Lights"] = "../src/Lights/**" 
			,["Demo/Lua Binding"] = {"../src/**Binding*"}
		}
		
	    includedirs { TRESSFX_DIR.."/inc", "."}
		links { "AMD_%{_AMD_LIBRARY_NAME}" }
		
		amdSushiSampleBuildConfig()

		files { PLUGIN_SRC_DIR.."**.c", PLUGIN_SRC_DIR.."**.cpp", PLUGIN_SRC_DIR.."**.h", PLUGIN_SRC_DIR..PLUGIN_NAME.."ToluaBinding.cpp", PLUGIN_SRC_DIR..PLUGIN_NAME.."ToluaBinding.h" }
		includedirs { PLUGIN_SRC_DIR, PLUGIN_DIR, PLUGIN_SRC_DIR.."/Lights" }
		postbuildmessage ('Copying '..PLUGIN_NAME..' to Rel')
		
	filter "configurations:Debug" 
		links { SUSHI_CORE_LIB.."_d", TOLUA_LIB.."_d", LUA_LIB.."_d" }
		targetsuffix("_d")
		postbuildcommands { '{COPY} %{cfg.buildtarget.abspath} '..WORKING_DIR, amdSushiSamplePostbuildCommands(SUSHI_BIN, SUFFIX.."_d")  }

	filter "configurations:Release" 
		links { SUSHI_CORE_LIB, TOLUA_LIB, LUA_LIB }
		postbuildcommands { '{COPY} %{cfg.buildtarget.abspath} '..WORKING_DIR, amdSushiSamplePostbuildCommands(SUSHI_BIN, SUFFIX)  }
