@echo off

set startdir=%cd%
cd "%~dp0"

cd ..\

echo.
echo --- amd_tressfx ---
echo.
cd amd_tressfx\premake
call :createvsfiles
echo ""


echo.
echo -- amd_tressfx_sample --
echo.
cd ..\..\amd_tressfx_sample\premake
call :createvsfilesforallapis


cd "%startdir%"

goto :EOF

::--------------------------
:: SUBROUTINES
::--------------------------



:: run premake for vs2015 and vs2017
:createvsfiles
..\..\premake\premake5.exe vs2015
..\..\premake\premake5.exe vs2017
goto :EOF


:createvsfilesforallapis
..\..\premake\premake5.exe --gfxapi=d3d11 vs2015
..\..\premake\premake5.exe --gfxapi=d3d11 vs2017
..\..\premake\premake5.exe --gfxapi=d3d12 vs2015
..\..\premake\premake5.exe --gfxapi=d3d12 vs2017
goto :EOF


