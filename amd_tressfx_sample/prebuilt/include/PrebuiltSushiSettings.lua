WINDOWS_SDK_DIR = ""
WINDOWS_SDK_INCLUDE = ""

D3D11_SDK_DIR = WINDOWS_SDK_DIR
D3D11_SDK_INCLUDE = WINDOWS_SDK_INCLUDE
D3D12_SDK_DIR = WINDOWS_SDK_DIR
D3D12_SDK_INCLUDE = WINDOWS_SDK_INCLUDE

LUA_DIR = "Thirdparty/lua-5.1.5/"
LUA_INCLUDE = LUA_DIR.."src/"
TOLUA_EXE = "bin/tolua++.exe"  
TOLUA_DIR = "Thirdparty/toluapp-master/"
TOLUA_INCLUDE = TOLUA_DIR.."include/"


SUSHI_COMMON_DIR = "Common/"
SUSHI_COMMON_INCLUDE = SUSHI_COMMON_DIR.."include/"

SUSHI_PS_DIR = "ParticleSystem/"
SUSHI_PS_INCLUDE = SUSHI_PS_DIR.."include/"

SUSHI_RM_DIR = "RenderManager/"
SUSHI_RM_INCLUDE = SUSHI_RM_DIR.."include/"

SUSHI_GRM_DIR = "GPUResourceManager/"
SUSHI_GRM_INCLUDE = SUSHI_GRM_DIR.."include/"

SUSHI_PM_DIR = "PluginManager/"
SUSHI_PM_INCLUDE = SUSHI_PM_DIR.."include/"

SUSHI_WM_DIR = "WindowManager/"
SUSHI_WM_INCLUDE = SUSHI_WM_DIR.."include/"

SUSHI_EM_DIR = "EffectManager/"
SUSHI_EM_INCLUDE = SUSHI_EM_DIR.."include/"

SUSHI_AM_DIR = "AudioManager/"
SUSHI_AM_INCLUDE = SUSHI_AM_DIR.."include/"

SUSHI_SM_DIR = "ScriptManager/"
SUSHI_SM_INCLUDE = SUSHI_SM_DIR.."include/"

SUSHI_OM_DIR = "ObjectManager/"
SUSHI_OM_INCLUDE = SUSHI_OM_DIR.."include/"

SUSHI_GUI_DIR = "GUI/"
SUSHI_GUI_INCLUDE = SUSHI_GUI_DIR.."include/"

SUSHI_FONTS_DIR = "SushiFonts/"
SUSHI_FONTS_INCLUDE = SUSHI_FONTS_DIR.."include/"

SUSHI_ENGINE_DIR = "Engine/"
SUSHI_CORE_DIR = "SushiCore/"

TOLUA_LIB = "tolualib"..API_SUFFIX
TOLUA_EXE_PROJ = "tolua"
LUA_LIB = "lua"..API_SUFFIX

SUSHI_COMMON_LIB = "SushiCommon"
SUSHI_WM_LIB = "WindowManager"
SUSHI_RM_LIB = "RenderManager"
SUSHI_GRM_LIB = "GPUResourceManager"
SUSHI_AM_LIB = "AudioManager"
SUSHI_SM_LIB = "ScriptManager"
SUSHI_PM_LIB = "PluginManager"
SUSHI_PS_LIB = "ParticleSystem"
SUSHI_OM_LIB = "ObjectManager"
SUSHI_GUI_LIB = "GUI"
SUSHI_EM_LIB = "EffectManager"
SUSHI_FONTS_LIB = "SushiFonts"

SUSHI_CORE_LIB = "SushiCore"..API_SUFFIX

INCLUDE_AM = true
INCLUDE_SM = true
INCLUDE_PM = true
INCLUDE_PS = true
INCLUDE_OM = true  -- this one might depend on PS or visa-versa.


-- post-build commands for samples
function amdSushiSamplePostbuildCommands(sushipath, suffix)
   local commands = {}
   
   table.insert(commands, '{COPY} %{_AMD_SAMPLE_DLL_TARGETDIR}\\$(TargetName).dll '..WORKING_DIR )
   table.insert(commands, '{COPY} '..sushipath..'SushiCore'..suffix..'.dll '..WORKING_DIR )
   table.insert(commands, '{COPY} '..sushipath..'lua'..suffix..'.dll '..WORKING_DIR )
   table.insert(commands, '{COPY} '..sushipath..'Sushi'..suffix..'.exe '..WORKING_DIR )
   
   return commands
end

-- post-build commands for samples
function amdSushiSampleBuildConfig()
		includedirs {
			SUSHI_INCLUDE..SUSHI_COMMON_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_PM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_OM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_OM_DIR, 
			SUSHI_INCLUDE..SUSHI_PS_DIR, 
			SUSHI_INCLUDE..SUSHI_PS_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_WM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_EM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_SM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_RM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_GRM_INCLUDE, 
			SUSHI_INCLUDE..SUSHI_GUI_INCLUDE, 
			SUSHI_INCLUDE..TOLUA_INCLUDE, 
			SUSHI_INCLUDE..LUA_INCLUDE }
		libdirs { SUSHI_BIN }
		defines { "SUSHI_DLL_IMPORT" }
		defines { "LUA_BUILD_AS_DLL" }
 
end

