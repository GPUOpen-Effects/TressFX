--=================================================================================================================================
--
--
-- Main draw script
--=================================================================================================================================
-- $Id: //depot/3darg/Demos/Effects/TressFXRelease/amd_tressfx_sample/bin/Scripts/Normal.lua#1 $ 
-- 
-- Last check-in:  $DateTime: 2017/03/28 18:02:41 $ 
-- Last edited by: $Author: khillesl $
--=================================================================================================================================
-- (C) AMD, Inc. 2013 All rights reserved.
--=================================================================================================================================

function ComputePluginSuffix()
	local suffix = ""
	if Sushi.Config.API == "D3D11" then
		suffix = "D3D11"
	elseif Sushi.Config.API == "D3D12" then
		suffix = "D3D12"
	end

	return suffix

end

-- setup the main render target dimensions
MainRTWidth = Sushi.WM:GetWindowWidth()
MainRTHeight = Sushi.WM:GetWindowHeight()
defaultCameraNearPlane = 10.0
defaultCameraFarPlane = 2500.0
freezeSimulation = false
simulationFrozen = false

-- Load SushiSample plugin
SuTressFXPlugin = Sushi.LoadPlugin( "SuTressFXPlugin"..ComputePluginSuffix() )

if Sushi.IsDebugMode() then
    include( "Scripts/Debug.lua" )
end
include( "Scripts/SSAO.lua" )
include( "Scripts/ArtistSettings.lua" )
include( "Scripts/IBL.lua")
include( "Scripts/Shadows.lua")
include( "Scripts/LegacyLights.lua" )
include( "Scripts/Lighting.lua" )
include( "Scripts/Hair.lua" )
include( "Scripts/GUI.lua")


m_DrawShadows = true;
m_IsShuttingDown = false;

HairModel = "Hair"  -- this just sets the name of the scene, which is unfortunately called "Hair"


bSSAO = not Sushi.Config.disableSSAO 
if not bSSAO then
    print("bSSAO disabled")
end



--=================================================================================================================================
-- Initialization, this is called at engine start up before anything is loaded
--=================================================================================================================================
function Initialize ()

	-- Background Color
	vClearColor = SuVector4( 0.0, 0.0, 0.0, 1 )
	--vClearColor = SuVector4(0, 0, 0, 1)

	-- Load a font 
	Font = SuFont:CreateCustomFont( "defaultfont.suf2", 15 )
	tolua.takeownership(Font)
	
	-- Default clear colors
	Sushi.RM:SetClearColor( vClearColor )
	Sushi.RM:SetClearDepth( 1.0 )
	Sushi.RM:SetClearStencil( 0 )

    fAmbientScale = 0
    fGlobalSpecularScale = 1
    fLightScale = 1

	SetupBuffers()

    LineRenderer = SuBatchLineRenderer()

end






function SetupBuffers()
    -- backbuffer render target
    tBackBuffer = Sushi.CreateRenderableTexture{ Name = "tBackBuffer",
                                                 Width  = MainRTWidth,
                                                 Height = MainRTHeight,
                                                 Format = SU_FORMAT_R8G8B8A8_UNORM_SRGB,
                                                 MultiSampleTaps = 1,
                                                 ViewFormat = SU_FORMAT_R8G8B8A8_UNORM_SRGB,
                                                 ClearColor = vClearColor }
    tBackBufferView = Sushi.CreateRenderableView{ Resource = tBackBuffer }
    tBackBuffer:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )

	tDepthBuffer = Sushi.CreateDepthBuffer{ Name   = "tDepthBuffer", 
                                            Width  = MainRTWidth,
                                            Height = MainRTHeight,
                                            Format = SU_FORMAT_R32_TYPELESS,
                                            MultiSampleTaps = 1,
                                            Fetchable = false,
                                            ViewFormat = SU_FORMAT_D32_FLOAT,
                                            ClearDepth = 1.0,
                                            ClearStencil = 0 }
    tDepthBufferView = Sushi.CreateDepthStencilView{ Resource = tDepthBuffer, Format = SU_FORMAT_D32_FLOAT }                                            
    
    	-- Render target for drawing normals, used by SSAO
    -- "tNormals" and "zNormals" referenced by qSSAO.sufx
	tNormals = Sushi.CreateRenderableTexture{ Name   = "tNormals", 
                                             Width  = MainRTWidth,
                                             Height = MainRTHeight,
                                             Format = SU_FORMAT_R10G10B10A2_UNORM,
                                             MultiSampleTaps = 1,
											 ViewFormat = SU_FORMAT_R10G10B10A2_UNORM,
											 ClearColor = SuVector4(0, 0, 0, 0) }

	tNormalsView = Sushi.CreateRenderableView{ Resource = tNormals }

	zNormals = Sushi.CreateDepthBuffer{ Name   = "zNormals", 
                                        Width  = MainRTWidth,
                                        Height = MainRTHeight,
                                        Format = SU_FORMAT_R32_TYPELESS,
                                        MultiSampleTaps = 1,
										Fetchable = true,
										ViewFormat = SU_FORMAT_D32_FLOAT,
										ClearDepth = 1.0,
										ClearStencil = 0 }

	zNormalsView = Sushi.CreateDepthStencilView{ Resource = zNormals, Format = SU_FORMAT_D32_FLOAT }

	tNormals:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_NON_PIXEL_SHADER_RESOURCE);
	zNormals:Transition(SuGPUResource.STATE_DEPTH_STENCIL_WRITE, SuGPUResource.STATE_NON_PIXEL_SHADER_RESOURCE);
    
	-- Script variable in iUpsampleAO.shl
	tDepthSS = zNormals

end

--=================================================================================================================================
-- This function gets called after all assets (objects, lights, cameras, etc) are loaded and before the demo starts
--=================================================================================================================================
function OnFinishedLoading()

	-- Set initial world stransform.  This turns on a 
	-- bunch of stuff int the animated model so it 
	-- just works for now, as well.

	RatBoy_body = Sushi.GetObject("RatBoy_body")
	RatBoy_left_hand = Sushi.GetObject("RatBoy_left_hand")
	RatBoy_right_hand = Sushi.GetObject("RatBoy_right_hand")
	RatBoy_tail = Sushi.GetObject("RatBoy_tail")

	local identity = SuMatrix4.IDENTITY
	RatBoy_body:SetWorldTransform(identity)
    RatBoy_left_hand:SetWorldTransform(identity)
    RatBoy_right_hand:SetWorldTransform(identity)
    RatBoy_tail:SetWorldTransform(identity)

    SuTressFXPlugin:LoadHairAsset()
    local nTressFXObjects = SuTressFXPlugin:GetNumTressFXObjects()
    for i=0,nTressFXObjects-1 do
        Sushi.RegisterPluginObject( SuTressFXPlugin:GetTressFXObject(i), {Properties = {"TressFX"} }    )
    end

    SuTressFXPlugin:OnResize(MainRTWidth, MainRTHeight)

	Sushi.SetSceneCamera( Sushi.GetCamera("HairCamera") )

	SetupLights()

	if Sushi.Config.DemoLength then
		Sushi.SetDemoLength(Sushi.Config.DemoLength)
	end

	Shadows.AddCam(Sushi.GetCamera("RatboyKeyShadowCam"))

    --SetupEnvs()

    --SkyDome = Sushi.GetObject("SkyDome")

    SSAO.OnFinishedLoading()

    finalCopy = SuQuad:CreateQuad("qResolve.sufx")
    tolua.takeownership(finalCopy)
    
    Sushi.AtExit(Shutdown)

end

function PrintTimerStats()
    local results = Sushi.RM:GetGPUTimerResults();
    local nTimers = results:GetNumTimers()
    local s = "\nGPU TimerStats\n"
    s = s .."---------------\n"
    for i=0,nTimers-1 do
        local name = results:GetTimerName(i)
        local ave = results:GetAvgFrameTotal(i)
        s = s .. string.format("%-25s    Frame Ave: %f ms\n", name, ave*1000)
    end
    print(s)

end


--=================================================================================================================================
-- Shutdown, a chance to release things explicitly (like unbinding to avoid "resource destroyed while view reference" warnings)
--=================================================================================================================================
function Shutdown()
    SuTressFXPlugin:Shutdown()
    SSAO.Shutdown()
    if Sushi.Config.bPrintTimerStats then 
        PrintTimerStats()
    end
end

function DebugDraw()

    if tressfxDrawColMeshCheckbox:GetIsChecked() then        
        SuTressFXPlugin:DrawCollisionMesh() 
    end

    --if SDFGridCheckbox:GetIsChecked() then
    --    SuTressFXPlugin:DrawSDFGrid() 
    --end

    if marchingCubesCheckbox:GetIsChecked() then
        SuTressFXPlugin:DrawSDF()
    end
end

function DrawSSAO()
    local ssaoTimer = Sushi.RM:CreateGPUTimer("SSAO")
    local ssaoDrawTimer = Sushi.RM:CreateGPUTimer("SSAO - Draw")
    local ssaoCalcTimer = Sushi.RM:CreateGPUTimer("SSAO - Calc")

    Sushi.RM:StartGPUTimer(ssaoTimer)

	tNormals:Transition(SuGPUResource.STATE_NON_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_RENDER_TARGET);
	zNormals:Transition(SuGPUResource.STATE_NON_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_DEPTH_STENCIL_WRITE);
    --------------------
    -- DRAW normals
    -------------------
    -- Render out normals for AO. 

    -- A 1 in the alpha channel indicates that that pixel should get AO
	Sushi.RM:SetRenderTargets( 1, { tNormalsView }, zNormalsView )
	Sushi.RM:ViewportAndScissor( 0, 0, MainRTWidth, MainRTHeight )
	Sushi.RM:SetClearColor( SuVector4(0, 0, 0, 0) )
	Sushi.RM:SetClearDepth( 1.0 )
	Sushi.RM:SetClearStencil( 0 )
	Sushi.RM:Clear( true, true, true )

    Sushi.RM:StartGPUTimer(ssaoDrawTimer)
	Sushi.DrawObjects{ DrawProperty = HairModel, TechniqueProperties = {"Normals"}  }
	Sushi.RM:StopGPUTimer(ssaoDrawTimer)

	tNormals:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE);
	zNormals:Transition(SuGPUResource.STATE_DEPTH_STENCIL_WRITE, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE);

	-- Compute SSAO using normals and depth
    Sushi.RM:StartGPUTimer(ssaoCalcTimer)
	if( bSSAO ) then
		SSAO.ComputeCharacterAO()
	else
		SSAO.ClearAO()
	end
    Sushi.RM:StopGPUTimer(ssaoCalcTimer)

    tNormals:Transition(SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_NON_PIXEL_SHADER_RESOURCE);
    zNormals:Transition(SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_NON_PIXEL_SHADER_RESOURCE);
    Sushi.RM:StopGPUTimer(ssaoTimer)
end

function DrawScene()
    if drawModelCheckbox:GetIsChecked() then
        Sushi.DrawObjects{ DrawProperty = HairModel } -- Drawing the model
    end

    if drawFurCheckbox:GetIsChecked() then
	    SuTressFXPlugin:DrawHair()
	end
end

function UpdateCamera(nearZ, farZ)
    local camera = Sushi.CM:GetActiveCamera()
    local aspectRatio = camera:GetAspectRatio();
	local fov = camera:GetFOV();
	camera:SetFrustum(fov, aspectRatio, nearZ, farZ)
end

--=================================================================================================================================
-- Per-frame draw function
--=================================================================================================================================
function Draw()

    if not(m_IsShuttingDown) then

        renderSceneTotal = Sushi.RM:CreateGPUTimer("RENDER QUEUE TOTAL")
        Sushi.RM:StartGPUTimer(renderSceneTotal)



        PropogateGUISettings()
    
        camera = Sushi.CM:GetCameraByName("HairCamera")
        aspectRatio = camera:GetAspectRatio();
        fov = camera:GetFOV();
        camera:SetFrustum(fov, aspectRatio, 1.0, 2500.0)
    
        if Sushi.IsDebugMode() then
            Debug.Clear()
        end
    
        LegacyLights.Update()
    
        -- We can globally turn off timer from be proceeded to turn off the model's animation and this does not cause the jerky motion when it resumes. 
        Sushi.demoTimer:Pause( freezeSimulation )
    
        SuTressFXPlugin:EnableUpdatingCollisionMesh(tressfxUpdateColMeshCheckbox:GetIsChecked())
        SuTressFXPlugin:EnableSDFCollisionResponse(tressfxCollisionCheckbox:GetIsChecked())
    
        if not simulationFrozen then

            -- Simulation
            local demoTime = Sushi.demoTimer:GetTime();

	        Sushi.EM:ForceDisableOverrideTechniques( true )
	        SuTressFXPlugin:Simulate(demoTime)
	        Sushi.EM:ForceDisableOverrideTechniques( false )

        end

        TransitionToShadowWrite()

        --------------------------
        -- Scene Shadows
        local sceneShadowTimer = Sushi.RM:CreateGPUTimer("Scene Shadows")
        Sushi.RM:StartGPUTimer(sceneShadowTimer)
        RenderShadowMaps("dynamic")
        Sushi.RM:StopGPUTimer(sceneShadowTimer)


        --------------------------
        -- Wait on sim before using hair
        if not simulationFrozen then
            SuTressFXPlugin:WaitSimulateDone()
        end

        simulationFrozen = freezeSimulation

        --------------------------
        -- TressFX Shadows
        local tressfxShadowTimer = Sushi.RM:CreateGPUTimer("TRESSFX SHADOWS")
        Sushi.RM:StartGPUTimer(tressfxShadowTimer)
        RenderShadowMaps("TressFX")
        Sushi.RM:StopGPUTimer(tressfxShadowTimer)

        Sushi.SetRenderingStrategy(LightRenderingStrategy)
    
        UpdateCamera(defaultCameraNearPlane, defaultCameraFarPlane)
        DrawSSAO()
 
        TransitionToShadowRead()

        ----------------------------------
        -- Scene Draw

        local sceneDrawTimer = Sushi.RM:CreateGPUTimer("Scene Draw")
        Sushi.RM:StartGPUTimer(sceneDrawTimer)

        tBackBuffer:Transition(SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_RENDER_TARGET )
    
        -- Set the back buffer as the render target, then clear everything
        Sushi.RM:SetRenderTargets( 1, { tBackBufferView }, tDepthBufferView )
        Sushi.RM:ViewportAndScissor( 0, 0, MainRTWidth, MainRTHeight )
        Sushi.RM:SetClearColor( vClearColor )
        Sushi.RM:SetClearDepth( 1.0 )
        Sushi.RM:SetClearStencil( 0 )
        Sushi.RM:Clear( true, true, true )
    
        --SkyDome:Draw()

    
        UpdateCamera(defaultCameraNearPlane, defaultCameraFarPlane)

        DrawScene()
        Sushi.RM:StopGPUTimer(sceneDrawTimer)
        
        local resolveTimer = Sushi.RM:CreateGPUTimer("Resolve")
        Sushi.RM:StartGPUTimer(resolveTimer)

        tBackBuffer:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )
        Sushi.RM:SetRenderTargets( 1, { Sushi.GRM:GetBackBufferView() }, Sushi.GRM:GetDefaultZBufferView() )
        Sushi.RM:ViewportAndScissor( 0, 0, Sushi.WM:GetWindowWidth(), Sushi.WM:GetWindowHeight() )
        Sushi.RM:Clear( true, true, true )
    
        finalCopy:Draw("CopyFinalResults")
        Sushi.RM:StopGPUTimer(resolveTimer)
        
        -- render debug directly into the back buffer
        DebugDraw()

        Sushi.RM:StopGPUTimer(renderSceneTotal)


    else
        -- Set the back buffer as the render target, then clear everything
        Sushi.RM:SetRenderTargets( 1, { Sushi.GRM:GetBackBufferView() }, Sushi.GRM:GetDefaultZBufferView() )
        Sushi.RM:ViewportAndScissor( 0, 0, MainRTWidth, MainRTHeight )
        Sushi.RM:SetClearColor( 0.0,0.0,0.0,1.0 )
        Sushi.RM:SetClearDepth( 1.0 )
        Sushi.RM:SetClearStencil( 0 )
        Sushi.RM:Clear( true, true, true )
    end
end

-----------------------------------------------------------------------------------------------------------------------------------
-- Mechanism to only print a warning once.
-----------------------------------------------------------------------------------------------------------------------------------
Warnings = {}

function WarnOnce(warning)
      if not Warnings[warning] then
         print(warning)
         Warnings[warning] = true
      end
end
