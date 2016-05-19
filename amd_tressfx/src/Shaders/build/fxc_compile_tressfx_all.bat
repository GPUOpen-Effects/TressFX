@echo off

REM Set this to point to the Windows SDK version where fxc is located
REM Recommend Windows SDK 8.1 or higher.
set windows_sdk=8.1

REM Batch file for compiling the TressFX library shaders
echo --- Compiling shaders for TressFX ---

set startdir=%cd%
cd "%~dp0"

set fxc_exe=%ProgramFiles(x86)%\Windows Kits\%windows_sdk%\bin\x86\fxc.exe

REM If fxc.exe exists in the same directory as the batch file, it will be used instead
if exist .\fxc.exe set fxc_exe=.\fxc.exe

echo --- Using "%fxc_exe%" ---

REM TressFXRender Vertex Shaders
"%fxc_exe%" /nologo /T vs_5_0 /E VS_RenderHair /O1 /Fh ..\inc\VS_RenderHair.inc /Vn VS_RenderHair_Data ..\TressFXRender.hlsl
"%fxc_exe%" /nologo /T vs_5_0 /E VS_RenderHair_AA /O1 /Fh ..\inc\VS_RenderHair_AA.inc /Vn VS_RenderHair_AA_Data ..\TressFXRender.hlsl
"%fxc_exe%" /nologo /T vs_5_0 /E VS_GenerateHairSM /O1 /Fh ..\inc\VS_GenerateHairSM.inc /Vn VS_GenerateHairSM_Data ..\TressFXRender.hlsl
"%fxc_exe%" /nologo /T vs_5_0 /E VS_RenderHair_StrandCopies /O1 /Fh ..\inc\VS_RenderHair_StrandCopies.inc /Vn VS_RenderHair_StrandCopies_Data ..\TressFXRender.hlsl
"%fxc_exe%" /nologo /T vs_5_0 /E VS_RenderHair_AA_StrandCopies /O1 /Fh ..\inc\VS_RenderHair_AA_StrandCopies.inc /Vn VS_RenderHair_AA_StrandCopies_Data ..\TressFXRender.hlsl
"%fxc_exe%" /nologo /T vs_5_0 /E VS_ScreenQuad /O1 /Fh ..\inc\VS_ScreenQuad.inc /Vn VS_ScreenQuad_Data ..\TressFXRender.hlsl

REM TressFXRender Pixel Shaders
"%fxc_exe%" /nologo /T ps_5_0 /E PS_ABuffer_Hair /O1 /Fh ..\inc\PS_ABuffer_Hair.inc /Vn PS_ABuffer_Hair_Data  ..\TressFXRender.hlsl 
"%fxc_exe%" /nologo /T ps_5_0 /E PS_KBuffer_Hair /O1 /Fh ..\inc\PS_KBuffer_Hair.inc /Vn PS_KBuffer_Hair_Data  ..\TressFXRender.hlsl 
"%fxc_exe%" /nologo /T ps_5_0 /E PS_Depth_Hair /O1 /Fh ..\inc\PS_Depth_Hair.inc /Vn PS_Depth_Hair_Data  ..\TressFXRender.hlsl 
"%fxc_exe%" /nologo /T ps_5_0 /E PS_FillColors_Hair /O1 /Fh ..\inc\PS_FillColors_Hair.inc /Vn PS_FillColors_Hair_Data  ..\TressFXRender.hlsl 
"%fxc_exe%" /nologo /T ps_5_0 /E PS_ResolveDepth_Hair /O1 /Fh ..\inc\PS_ResolveDepth_Hair.inc /Vn PS_ResolveDepth_Hair_Data  ..\TressFXRender.hlsl
"%fxc_exe%" /nologo /T ps_5_0 /E PS_ResolveColor_Hair /O1 /Fh ..\inc\PS_ResolveColor_Hair.inc /Vn PS_ResolveColor_Hair_Data  ..\TressFXRender.hlsl 

REM TressFXSimulation Compute Shaders
"%fxc_exe%" /nologo /T cs_5_0 /E IntegrationAndGlobalShapeConstraints /O1 /Fh ..\inc\IntegrationAndGlobalShapeConstraints.inc /Vn IntegrationAndGlobalShapeConstraints_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E ApplyHairTransformGlobally /O1 /Fh ..\inc\ApplyHairTransformGlobally.inc /Vn ApplyHairTransformGlobally_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E ComputeTangents /O1 /Fh ..\inc\ComputeTangents.inc /Vn ComputeTangents_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E LocalShapeConstraints /O1 /Fh ..\inc\LocalShapeConstraints.inc /Vn LocalShapeConstraints_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E LocalShapeConstraintsWithIteration /O1 /Fh ..\inc\LocalShapeConstraintsWithIteration.inc /Vn LocalShapeConstraintsWithIteration_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E LengthConstriantsWindAndCollision /O1 /Fh ..\inc\LengthConstriantsWindAndCollision.inc /Vn LengthConstriantsWindAndCollision_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E UpdateFollowHairVertices /O1 /Fh ..\inc\UpdateFollowHairVertices.inc /Vn UpdateFollowHairVertices_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E PrepareFollowHairBeforeTurningIntoGuide /O1 /Fh ..\inc\PrepareFollowHairBeforeTurningIntoGuide.inc /Vn PrepareFollowHairBeforeTurningIntoGuide_Data  ..\TressFXSimulation.hlsl 
"%fxc_exe%" /nologo /T cs_5_0 /E GenerateTransforms /O1 /Fh ..\inc\GenerateTransforms.inc /Vn GenerateTransforms_Data  ..\TressFXSimulation.hlsl 

cd "%startdir%"
pause
