--=================================================================================================================================
--
-- Author: Jeremy Shopf
--         3D Application Research Group
--         AMD, Inc.
--
-- Script for calculating SSAO
--=================================================================================================================================
-- $Id: //depot/3darg/Demos/Effects/TressFXRelease/amd_tressfx_sample/bin/Scripts/SSAO.lua#1 $ 
-- 
-- Last check-in:  $DateTime: 2017/03/28 18:02:41 $ 
-- Last edited by: $Author: khillesl $
--=================================================================================================================================
-- (C) AMD, Inc. 2009 All rights reserved.
--=================================================================================================================================

SSAO = {}

SSAO.bDownsampled = false

function Initialize()

   fBlurRadius = 16.0
   fFalloff = .1
   nSample = 0

   SSAO.fBlurRadius = 19.0 --3.0
   SSAO.fOcclusionScale = 1.0;
   SSAO.fEpsilon = .4;
   SSAO.fDistFalloff = 150;
   SSAO.fDepthFalloff = 40;
   SSAO.fAngleFalloff = .3;
   SSAO.fSampleRadius = 0.05;   -- In view-space
   SSAO.fSharpness = 4.0;



   -- Low Res SSAO buffer + low res stencil buffer
   if( SSAO.bDownsampled ) then
      LOWRES_WIDTH = MainRTWidth/2
      LOWRES_HEIGHT = MainRTHeight/2
   else
      LOWRES_WIDTH = MainRTWidth
      LOWRES_HEIGHT = MainRTHeight
   end

   tSSAOPing = Sushi.CreateRenderableTexture{ Name   = "tSSAOPing", 
                                              Width  = LOWRES_WIDTH,
                                              Height = LOWRES_HEIGHT,
                                              Format = SU_FORMAT_R8_UNORM,
                                              MipLevels = 1,
                                              MultiSampleTaps = 1,
											  ViewFormat = SU_FORMAT_R8_UNORM,
											  ClearColor = SuVector4(1,1,1,1) }

   tSSAOPingView = Sushi.CreateRenderableView{ Resource = tSSAOPing }
   
   tSSAOPing:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE);

   tSSAOPong = Sushi.CreateRenderableTexture{ Name   = "tSSAOPong", 
                                              Width  = LOWRES_WIDTH,
                                              Height = LOWRES_HEIGHT,
                                              Format = SU_FORMAT_R8_UNORM,
                                              MipLevels = 1,
                                              MultiSampleTaps = 1,
											  ViewFormat = SU_FORMAT_R8_UNORM,
											  ClearColor = SuVector4(1,1,1,1) }

   tSSAOPongView = Sushi.CreateRenderableView{ Resource = tSSAOPong }
   
   tAOIn = tSSAOPong
   tAOView = tSSAOPingView

   tSSAOStencil = Sushi.CreateDepthBuffer{ Name = "tSSAOStencil",
                                           Width  = LOWRES_WIDTH,
                                           Height = LOWRES_HEIGHT,
                                           MultiSampleTaps = 1,
                                           Format = SU_FORMAT_D24_UNORM_S8_UINT }


   tSSAOStencilView = Sushi.CreateDepthStencilView{ Resource = tSSAOStencil }

    -- Sample positions used by SSAO.
    ssaoSamplePositions = {}
    ssaoSamplePositions[0] = SuVector4( 0.980084, 0.007705,  0.198433 , 0.0)
    ssaoSamplePositions[1] = SuVector4( 0.197163, 0.529858,  0.824850 , 0.0)
    ssaoSamplePositions[2] = SuVector4( -0.858993, -0.508074, 0.063189, 0.0 )
    ssaoSamplePositions[3] = SuVector4( -0.189187, 0.259394, 0.947060 , 0.0)
    ssaoSamplePositions[4] = SuVector4( 0.315754, -0.641066, 0.699524 , 0.0)
    ssaoSamplePositions[5] = SuVector4( 0.196800, 0.380950,  0.903409 , 0.0)
    ssaoSamplePositions[6] = SuVector4( -0.245416, -0.968485, 0.042508, 0.0 )
    ssaoSamplePositions[7] = SuVector4( -0.015660, 0.044376, 0.998892 , 0.0)
    ssaoSamplePositions[8] = SuVector4( 0.764593, 0.494733,  0.413083 , 0.0)
    ssaoSamplePositions[9] = SuVector4( 0.299423, 0.397818,  0.867229 , 0.0)
    ssaoSamplePositions[10] = SuVector4( 0.982921, -0.070992, 0.169786 , 0.0)
    ssaoSamplePositions[11] = SuVector4( 0.010751, 0.009980,  0.999892 , 0.0)
    ssaoSamplePositions[12] = SuVector4( 0.925851, 0.051894,  0.374309 , 0.0)
    ssaoSamplePositions[13] = SuVector4( -0.955866, -0.192712,0.221773 , 0.0)
    ssaoSamplePositions[14] = SuVector4( -0.757403, -0.563215,0.330347 , 0.0)
    ssaoSamplePositions[15] = SuVector4( 0.437878, 0.753691,  0.490114 , 0.0)
    ssaoSamplePositions[16] = SuVector4( -0.851617, 0.272048, 0.448039 , 0.0)
    ssaoSamplePositions[17] = SuVector4( 0.883385, 0.330883,  0.331885 , 0.0)
    ssaoSamplePositions[18] = SuVector4( 0.120773, -0.568454, 0.813802 , 0.0)
    ssaoSamplePositions[19] = SuVector4( -0.806270, -0.101252, 0.582818, 0.0 )
    ssaoSamplePositions[20] = SuVector4( 0.538524, -0.531982, 0.653443 , 0.0)
    ssaoSamplePositions[21] = SuVector4( 0.222431, -0.063260, -0.972894, 0.0 )
    ssaoSamplePositions[22] = SuVector4( -0.419263, -0.105835, 0.901675, 0.0 )
    ssaoSamplePositions[23] = SuVector4( -0.654742, 0.159015, 0.738937 , 0.0)
    ssaoSamplePositions[24] = SuVector4( 0.396645, -0.465955, 0.790923 , 0.0)
    ssaoSamplePositions[25] = SuVector4( 0.087380, -0.463499, 0.881778 , 0.0)
    ssaoSamplePositions[26] = SuVector4( 0.000959, -0.000019, 1.000000 , 0.0)
    ssaoSamplePositions[27] = SuVector4( -0.721241, -0.608197, 0.331523, 0.0 )
    ssaoSamplePositions[28] = SuVector4( -0.081757, 0.799798, 0.594675 , 0.0)
    ssaoSamplePositions[29] = SuVector4( 0.039988, -0.062886, 0.997219 , 0.0)
    ssaoSamplePositions[30] = SuVector4( -0.203967, 0.201760, 0.957962 , 0.0)
    ssaoSamplePositions[31] = SuVector4( -0.077584, -0.157630, 0.984446, 0.0 )

end

function SSAO.OnFinishedLoading()

   local camera = Sushi.CM:GetActiveCamera()

   if( camera == nil ) then
      print( "No camera?!" )
   end

   local fTmp = 0
   local fNear = 0
   local fFar = 0

   fTmp, fTmp, fTmp, fTmp, fNear, fFar = camera:GetFrustum( fTmp, fTmp, fTmp, fTmp, fNear, fFar )

   fFarMinusNear = fFar - fNear

   --print( "fFar : " .. fFar .. " fNear: " .. fNear .. " fFarMinusNear = " .. fFarMinusNear )

   fFOV = camera:GetFOV() * math.pi / 180;   -- horizontal fov

   -- HBAO Constants ===========
   HBAO = {}
   HBAO.g_fR = 0.5
   HBAO.g_fSqrR = HBAO.g_fR*HBAO.g_fR
   HBAO.g_fInvR = 1.0 / HBAO.g_fR
   HBAO.g_fAttenuation = 1.0

   HBAO.g_fNumDir = 8
   HBAO.g_fNumSteps = 6
   HBAO.g_fContrast = 1.8
   HBAO.g_fAngleBias = 30
   HBAO.g_fTanAngleBias = 1.0/30.0
   HBAO.g_vFocalLen = SuVector2( 1.0 / math.tan(fFOV*.5), 1.0 / math.tan(fFOV*.5) *  Sushi.WM:GetWindowWidth() / Sushi.WM:GetWindowHeight() );

   --print( "Focal Length: " .. HBAO.g_vFocalLen.x );
   --==========================

   fLeft = 0
   fRight = 0
   fBottom = 0
   fTop = 0
   fNear = 0
   fFar = 0

   fLeft, fRight, fBottom, fTop, fNear, fFar = camera:GetFrustum( fLeft, fRight, fBottom, fTop, fNear, fFar )
   fViewWidth = (math.abs(fLeft) + math.abs(fRight))/2.0


   RandomTexture = Sushi.LoadObject{ Object = "Objects/SSAOTextures.suo2", Name = "SSAOTextures" }

   SSAOQuad = SuQuad:CreateQuad( "qSSAO.sufx" )
   tolua.takeownership(SSAOQuad)

end

function SSAO.Shutdown()
    SSAOQuad = nil
    local effectPtr = Sushi.EM:LoadEffect("qSSAO.sufx")
    tolua.takeownership(effectPtr)
    if effectPtr then
        local p = effectPtr:get()
        p:BindTexture("tRandom", SuCountedPtr_SuGPUSamplingResourceView_:Null())
    end
end

function SSAO.CompositeSSAO()

   SSAOQuad:Draw( "HorizonBasedSSAO" )

end

function SSAO.BlurAO()
    local ssaoBlur = Sushi.RM:CreateGPUTimer("SSAO - Blur")
    Sushi.RM:StartGPUTimer(ssaoBlur)

    tAOIn = tSSAOPong
    tAOView = tSSAOPingView
    
    tSSAOPing:Transition(SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_RENDER_TARGET);
    tSSAOPong:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE);
    
    Sushi.RM:SetRenderTargets( 1, { tAOView }, SuCountedPtr_SuGPUDepthStencilResourceView_:Null() )
    Sushi.RM:ViewportAndScissor( 0, 0, LOWRES_WIDTH, LOWRES_HEIGHT )
    --Sushi.RM:SetClearColor( SuVector4(1,1,1,1) )
    --Sushi.RM:Clear( true, false, false )
    SSAOQuad:Draw( "GaussianBlurX" )
    
    tAOIn = tSSAOPing
    tAOView = tSSAOPongView
    
    tSSAOPong:Transition(SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_RENDER_TARGET);
    tSSAOPing:Transition(SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE);
    
    Sushi.RM:SetRenderTargets( 1, { tAOView }, SuCountedPtr_SuGPUDepthStencilResourceView_:Null() )
    Sushi.RM:ViewportAndScissor( 0, 0, LOWRES_WIDTH, LOWRES_HEIGHT )
    --Sushi.RM:SetClearColor( SuVector4(1,1,1,1) )
    --Sushi.RM:Clear( true, false, false )
    SSAOQuad:Draw( "GaussianBlurY" )
    
    tAOIn = tSSAOPong
    tAOView = tSSAOPingView

    Sushi.RM:StopGPUTimer(ssaoBlur)
end

function SSAO.UpdateCameraParams()
   local camera = Sushi.CM:GetActiveCamera()

   if( camera == nil ) then
      print( "No camera?!" )
   end

   local fTmp = 0
   local fNear = 0
   local fFar = 0

   fTmp, fTmp, fTmp, fTmp, fNear, fFar = camera:GetFrustum( fTmp, fTmp, fTmp, fTmp, fNear, fFar )

   fFarMinusNear = fFar - fNear

   --print( "fFar : " .. fFar .. " fNear: " .. fNear .. " fFarMinusNear = " .. fFarMinusNear )

   fFOV = camera:GetFOV() * math.pi / 180;   -- horizontal fov

   -- HBAO Constants ===========
   HBAO.g_fR = 0.3
   HBAO.g_fSqrR = HBAO.g_fR*HBAO.g_fR
   HBAO.g_fInvR = 1.0 / HBAO.g_fR
   HBAO.g_fAttenuation = .001

   HBAO.g_fNumDir = 6
   HBAO.g_fNumSteps = 8
   HBAO.g_fContrast = 1.25
   HBAO.g_fAngleBias = 25
   HBAO.g_fTanAngleBias = 1.0/25.0
   HBAO.g_vFocalLen = SuVector2( 1.0 / math.tan(fFOV*.5), 1.0 / math.tan(fFOV*.5) *  Sushi.WM:GetWindowWidth() / Sushi.WM:GetWindowHeight() );

end

function SSAO.ComputeCharacterAO()

   --SSAO.UpdateCameraParams()

   -- Set stencil on pixels covered by AOed objects (alpha channel in Normals texture)

   -- Stencil not working -- have not debugged yet
   --Sushi.RM:SetRenderTargets( 0, { }, tSSAOStencilView )
   --Sushi.RM:ViewportAndScissor( 0, 0, LOWRES_WIDTH, LOWRES_HEIGHT )
   --Sushi.RM:SetClearDepth( 1.0 )
   --Sushi.RM:SetClearStencil( 0 )
   --Sushi.RM:Clear( true, true, true )

   --SSAOQuad:Draw( "SetStencil" )

   -- Calculate SSAO single sampled, but only on stenciled pixels
   --Sushi.RM:SetRenderTargets( 1, { tSSAOPongView }, tSSAOStencilView )
   Sushi.RM:SetRenderTargets( 1, { tSSAOPongView }, SuCountedPtr_SuGPUDepthStencilResourceView_:Null() )
   Sushi.RM:ViewportAndScissor( 0, 0, LOWRES_WIDTH, LOWRES_HEIGHT )
   Sushi.RM:SetClearColor( SuVector4(1,1,1,1) )
   Sushi.RM:Clear( true, false, false )
   --SSAOQuad:Draw( "StenciledHorizonBasedSSAO" )
   SSAOQuad:Draw( "SSAO" )

   SSAO.BlurAO()

end

function SSAO.ClearAO()

   Sushi.RM:SetRenderTargets( 1, { tSSAOPongView }, SuCountedPtr_SuGPUDepthStencilResourceView_:Null() )
   Sushi.RM:ViewportAndScissor( 0, 0, LOWRES_WIDTH, LOWRES_HEIGHT )
   Sushi.RM:SetClearColor( SuVector4(1,1,1,1) )
   Sushi.RM:Clear( true, false, false )
   Sushi.RM:SetRenderTargets( 1, { tSSAOPingView }, SuCountedPtr_SuGPUDepthStencilResourceView_:Null() )
   Sushi.RM:ViewportAndScissor( 0, 0, LOWRES_WIDTH, LOWRES_HEIGHT )
   Sushi.RM:Clear( true, false, false )
   
   Sushi.RM:SetClearColor( SuVector4(0,0,0,0) )
end

-- This function is here to help visualize 
function SSAO.VisualizeSamples()

   SSAOQuad:DrawVerticesAsPoints( "RenderSamplesAsPoints", 32/4 )

end