fxc.exe /nologo /T vs_5_0 /E vsFullscreen        /O1 /Fh ..\inc\VS_FULLSCREEN.inc          /Vn VS_FULLSCREEN_Data          ..\AMD_FullscreenPass.hlsl
fxc.exe /nologo /T vs_5_0 /E vsScreenQuad        /O1 /Fh ..\inc\VS_SCREENQUAD.inc          /Vn VS_SCREENQUAD_Data          ..\AMD_FullscreenPass.hlsl

fxc.exe /nologo /T vs_5_0 /E vsFullscreenIndexRT /O1 /Fh ..\inc\VS_FULLSCREEN_INDEX_RT.inc /Vn VS_FULLSCREEN_INDEX_RT_Data ..\AMD_FullscreenPass.hlsl
fxc.exe /nologo /T gs_5_0 /E gsFullscreenIndexRT /O1 /Fh ..\inc\GS_FULLSCREEN_INDEX_RT.inc /Vn GS_FULLSCREEN_INDEX_RT_Data ..\AMD_FullscreenPass.hlsl
fxc.exe /nologo /T ps_5_0 /E psFullscreen        /O1 /Fh ..\inc\PS_FULLSCREEN.inc          /Vn PS_FULLSCREEN_Data          ..\AMD_FullscreenPass.hlsl