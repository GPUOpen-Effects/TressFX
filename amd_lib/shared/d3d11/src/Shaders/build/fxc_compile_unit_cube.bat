fxc.exe /nologo /T vs_5_0 /E vsUnitCube      /O1 /Fh ..\inc\VS_UNIT_CUBE.inc       /Vn VS_UNIT_CUBE_Data       ..\AMD_UnitCube.hlsl
fxc.exe /nologo /T ps_5_0 /E psUnitCube      /O1 /Fh ..\inc\PS_UNIT_CUBE.inc       /Vn PS_UNIT_CUBE_Data       ..\AMD_UnitCube.hlsl

fxc.exe /nologo /T vs_5_0 /E vsClipSpaceCube /O1 /Fh ..\inc\VS_CLIP_SPACE_CUBE.inc /Vn VS_CLIP_SPACE_CUBE_Data ..\AMD_UnitCube.hlsl