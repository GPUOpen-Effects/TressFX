-- amd_premake_util.lua
-- utility code shared by AMD build scripts

-- _ACTION is a premake global variable and for our usage will be vs2012, vs2013, etc.
-- Strip "vs" from this string to make a suffix for solution and project files.
_AMD_VS_SUFFIX = "_" .. string.gsub(_ACTION, "vs", "")

-- _ACTION is a premake global variable and for our usage will be vs2012, vs2013, etc.
-- Make an allcaps version. We use this for some directory names.
_AMD_ACTION_ALL_CAPS = string.upper(_ACTION)

-- Specify the DLL prefix here
_AMD_LIBRARY_PREFIX = "GPUOpen_"

-- Specify build output directory structure here: e.g. VS2012\x64\DLL_Debug
_AMD_SAMPLE_DIR_LAYOUT  = "Desktop%{_AMD_VS_SUFFIX}/%{cfg.platform}/%{cfg.buildcfg}"
_AMD_LIBRARY_DIR_LAYOUT = "%{_AMD_ACTION_ALL_CAPS}/%{cfg.platform}/%{cfg.buildcfg}"
_AMD_LIBRARY_DIR_LAYOUT_BACKSLASH = "%{_AMD_ACTION_ALL_CAPS}\\%{cfg.platform}\\%{cfg.buildcfg}"

-- Some projects have a "minimal dependencies" build. Give them a different output
-- directory structure to avoid collisions with the full-dependencies version.
_AMD_SAMPLE_DIR_LAYOUT_MINIMAL  = "Desktop%{_AMD_VS_SUFFIX}/minimal/%{cfg.platform}/%{cfg.buildcfg}"
_AMD_LIBRARY_DIR_LAYOUT_MINIMAL = "%{_AMD_ACTION_ALL_CAPS}/minimal/%{cfg.platform}/%{cfg.buildcfg}"

-- Specify WindowsTargetPlatformVersion here for VS2015 and VS2017
_AMD_WIN_SDK_VERSION = "8.1"

_AMD_COPY_WIN_10_SDK_REDIST_TO_BIN = "if not exist \"..\\bin\\d3dcompiler_47.dll\" if exist \"$(ProgramFiles)\\Windows Kits\\10\\Redist\\D3D\\x64\\d3dcompiler_47.dll\" xcopy \"$(ProgramFiles)\\Windows Kits\\10\\Redist\\D3D\\x64\\d3dcompiler_47.dll\" \"..\\bin\" /H /R /Y > nul"

-- touch the import lib so that the sample build will know the library was updated
_AMD_TOUCH_IMPORT_LIB = "copy /b \"..\\lib\\%{_AMD_LIBRARY_DIR_LAYOUT_BACKSLASH}\\$(TargetName).lib\" +,, \"..\\lib\\%{_AMD_LIBRARY_DIR_LAYOUT_BACKSLASH}\\\" > nul"

-- these are for copying the updated import lib and dll into the shared location
_AMD_COPY_DLL        = "xcopy \"..\\lib\\%{_AMD_LIBRARY_DIR_LAYOUT_BACKSLASH}\\$(TargetName).dll\"  \"..\\lib\" /H /R /Y > nul"
_AMD_COPY_IMPORT_LIB = "xcopy \"..\\lib\\%{_AMD_LIBRARY_DIR_LAYOUT_BACKSLASH}\\$(TargetName).lib\"  \"..\\lib\" /H /R /Y > nul"

-- post-build commands for samples
function amdSamplePostbuildCommands()
   local commands = {}
   
   table.insert(commands, _AMD_COPY_WIN_10_SDK_REDIST_TO_BIN)
 
   -- copy the library DLLs to the local bin directory
   table.insert(commands, "if exist \"..\\..\\AMD_%{_AMD_LIBRARY_NAME}\\lib\\%{_AMD_LIBRARY_PREFIX}%{_AMD_LIBRARY_NAME}_x64d.dll\" xcopy \"..\\..\\AMD_%{_AMD_LIBRARY_NAME}\\lib\\%{_AMD_LIBRARY_PREFIX}%{_AMD_LIBRARY_NAME}_x64d.dll\"  \"..\\bin\" /H /R /Y > nul")
   table.insert(commands, "if exist \"..\\..\\AMD_%{_AMD_LIBRARY_NAME}\\lib\\%{_AMD_LIBRARY_PREFIX}%{_AMD_LIBRARY_NAME}_x64.dll\"  xcopy \"..\\..\\AMD_%{_AMD_LIBRARY_NAME}\\lib\\%{_AMD_LIBRARY_PREFIX}%{_AMD_LIBRARY_NAME}_x64.dll\"   \"..\\bin\" /H /R /Y > nul")
   
   return commands
end

-- post-build commands for libraries
function amdLibPostbuildCommands()
   local commands = {}
   -- touch the import library so that the sample build will know the library was updated
   table.insert(commands, _AMD_TOUCH_IMPORT_LIB)
   -- copy the DLL and import library into the lib directory
   table.insert(commands, _AMD_COPY_DLL)
   table.insert(commands, _AMD_COPY_IMPORT_LIB)
   return commands
end
