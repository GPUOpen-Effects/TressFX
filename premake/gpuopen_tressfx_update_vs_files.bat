@echo off

set startdir_tressfx_update=%cd%
cd "%~dp0"

call gpuopen_fx_update_vs_files.bat AMD_TressFX

echo --- AMD_TressFX_Viewer ---
cd ..\AMD_TressFX_Viewer\premake
call :createvsfiles
cd ..\..\

cd "%startdir_tressfx_update%"
pause

goto :EOF

::--------------------------
:: SUBROUTINES
::--------------------------

:: run premake for vs2012, vs2013, and vs2015
:createvsfiles
..\..\premake\premake5.exe vs2012
..\..\premake\premake5.exe vs2013
..\..\premake\premake5.exe vs2015
goto :EOF
