
--------------------------------
--  BASIC FUNCTION DESCRIPTION
--  You can specify what objects get drawn into which shadow maps.
--  This is done by specifying a property for a camera, and then only 
--  objects that have the same property are drawn into the camera.
--  If the camera has no properties, or jsut the property "dynamic", 
--  then only objects with the property "scene" are drawn.
--
--  You can also specify static vs dynamic maps.
--  If you tag a shadow map as "dynamic", and draw with the "dynamic"
--  argument, then the draw will only update "dynamic" shadow maps.
--
--  To add Basic Shadows
--  1  Include this file.
--  2  Call Shadows.AddCam(camera) for each shadow camera. 
--     Property of this camera must match property of objects to draw into it.
--  3  Add "DepthCopy" technique, meaning you return depth in the pixel shader.
--  4  Call RenderShadowMaps in your draw call.
--  5  use iShadows.shl, modulating like this:
--     light *= ComputeIndexedShadow(float3 vPosWS, float nSMIndex);
--                  





nShadowMapWidth = 4096
nShadowMapHeight = 4096

MaxShadowMaps = 4
ShadowMatrixSize = 16*4
ShadowParamsSize = 16

bDebugShadows = false
bShadowsEnable = true


Shadows = {}

-- NOTE: THIS DOES NOT SWITCH HAIR TECHNIQUE.
if true then
    Shadows.DrawTechnique = "DepthOnly"
    Shadows.DepthOnly = true
    Shadows.DSName = "ShadowMap"
else
    Shadows.DrawTechnique = "DepthCopy"
    Shadows.DepthOnly = false
    Shadows.DSName = "ShadowMapDepth"
end

function ShadowPrint(t)
    if bDebugShadows then
        print(t)
    end
end

function Initialize()

    -- Create shadow map (depth texure)
    ShadowDepth = Sushi.CreateDepthBuffer{ Name = Shadows.DSName,
                                          Width = nShadowMapWidth,
                                          Height = nShadowMapHeight,
                                          MultiSampleTaps = 1,
                                          Format = SU_FORMAT_R32_TYPELESS, 
                                          ArraySize = MaxShadowMaps,
                                          Fetchable = true,
                                          ViewFormat = SU_FORMAT_D32_FLOAT,
                                          ClearDepth = 1.,
                                          ClearStencil = 0 }


    ShadowDepth:Transition( SuGPUResource.STATE_DEPTH_STENCIL_WRITE, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE );

    -- Create shadow depth stencil view
    ShadowDSViewArray = {}
    for i=1,MaxShadowMaps do
        ShadowDSViewArray[i] = Sushi.CreateDepthStencilView{ Resource = ShadowDepth, Format = SU_FORMAT_D32_FLOAT, ArraySize = 1, FirstArrayElement = i-1  }
    end
    
    ShadowDSView = Sushi.CreateDepthStencilView{ Resource = ShadowDepth, Format = SU_FORMAT_D32_FLOAT, ArraySize = 1, FirstArrayElement = 0  }

    if not Shadows.DepthOnly then
        ShadowMaps = Sushi.CreateRenderableTexture{ Name = "ShadowMap",
                                          Width = nShadowMapWidth,
                                          Height = nShadowMapHeight,
                                          MultiSampleTaps = 1,
                                          Format = SU_FORMAT_R32_FLOAT, 
                                          ArraySize = MaxShadowMaps,
                                          Fetchable = true,
                                          ViewFormat = SU_FORMAT_R32_FLOAT,
                                          ClearColor = SuVector4(1.0,1.0,1.0,1.0) }
        ShadowMaps:Transition( SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )

        --ShadowMapsView = Sushi.CreateRenderableView{ Resource = ShadowMaps }
        ShadowMapsViews = {}
        for i = 1,MaxShadowMaps do
            ShadowMapsViews[i] = Sushi.CreateRenderableView{ Resource = ShadowMaps,  FirstArrayElement = i-1}
        end
    end


    -- Shadow View-Projection Matrix
    Shadow_VP_Matrix = SuMatrix4()

    Shadows.MatrixBuffer = Sushi.CreateStructuredBuffer { Name="ShadowMatrices",
                                                         StructSize = ShadowMatrixSize,
                                                         StructCount = MaxShadowMaps,
                                                         Fetchable = true,
                                                         UnorderedAccess = false, 
                                                         NumMaxStagingBuffers = 4 }
    Shadows.MatrixCPUBuffer = SuMemoryBuffer:Allocate(MaxShadowMaps * ShadowMatrixSize);
    tolua.takeownership(Shadows.MatrixCPUBuffer)

    Shadows.Params = Sushi.CreateStructuredBuffer { Name="ShadowParams",
                                                         StructSize = ShadowParamsSize,
                                                         StructCount = MaxShadowMaps,
                                                         Fetchable = true,
                                                         UnorderedAccess = false, 
                                                         NumMaxStagingBuffers = 4 }
    Shadows.ParamsCPUBuffer = SuMemoryBuffer:Allocate(MaxShadowMaps * ShadowParamsSize);
    tolua.takeownership(Shadows.ParamsCPUBuffer)

    Shadows.MatrixBuffer:Transition( SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )
    Shadows.Params:Transition( SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )

      -- Shadow View-Projection Matrix
   Shadow_VP_Matrix = SuMatrix4()

end



------------------------------------------------------
-- Top level calls for shadows.
--
-- If the mode is "dynamic", then only shadow 
-- cameras labelled "dynamic" will be processed.
------------------------------------------------------

function TransitionToShadowWrite()
    ShadowDepth:Transition( SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_DEPTH_STENCIL_WRITE );
    if ShadowMaps then
        ShadowMaps:Transition( SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_RENDER_TARGET)
    end
end

function TransitionToShadowRead()
    if ShadowMaps then
        ShadowMaps:Transition( SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )
    end
    ShadowDepth:Transition( SuGPUResource.STATE_DEPTH_STENCIL_WRITE, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE );

    UpdateShadowParams()
end

function RenderSceneShadowMaps(mode, scene)
    ShadowPrint("==========Starting render of scene shadows")
    if mode then ShadowPrint("mode= "..mode) end
    if scene then ShadowPrint("scene= "..scene) end

    Sushi.SetRenderingStrategy(ShadowRenderingStrategy)


    for i = 1,Shadows.nCams do
    local cam = Shadows.CamArray[i]
        if not scene or cam:HasProperty(scene) then
            if mode == "dynamic" then
                ShadowPrint("in dynamic branch")
                if cam:HasProperty("dynamic") and not cam:HasProperty("TressFX") then
                    ShadowPrint("Mode is dynamic and cam "..i.." is dynamic not TressFX")
                    DrawShadowObjects(i)
                end
            elseif mode == "TressFX" then
                ShadowPrint("in TressFX branch")
                if cam:HasProperty("TressFX") then
                    ShadowPrint("Mode is TressFX and cam "..i.." is TressFX camera")
                    DrawShadowObjects(i)
                end
            else
                ShadowPrint("in generic branch")
                if not cam:HasProperty("TressFX") then
                    DrawShadowObjects(i)
                end
            end
        end
    end
    
    ShadowPrint("==== Done Rendering Scene Shadows")
end

function RenderShadowMaps(mode, scene)
    ShadowPrint("==========Starting render of shadows")
    if mode then ShadowPrint("mode= "..mode) end
    if scene then ShadowPrint("scene= "..scene) end
    --ShadowDepth:Transition( SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_DEPTH_STENCIL_WRITE );
    --if ShadowMaps then
    --    ShadowMaps:Transition( SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_RENDER_TARGET)
    --end

    Sushi.SetRenderingStrategy(ShadowRenderingStrategy)


    for i = 1,Shadows.nCams do
    local cam = Shadows.CamArray[i]
        if not scene or cam:HasProperty(scene) then
            if mode == "dynamic" then
                ShadowPrint("in dynamic branch")
                if cam:HasProperty("dynamic") and not cam:HasProperty("TressFX") then
                    ShadowPrint("Mode is dynamic and cam "..i.." is dynamic not TressFX")
                    DrawShadowObjects(i)
                end
            elseif mode == "TressFX" then
                ShadowPrint("in TressFX branch")
                if cam:HasProperty("TressFX") then
                    ShadowPrint("Mode is TressFX and cam "..i.." is TressFX camera")
                    DrawShadowObjects(i)
                end
            else
                ShadowPrint("in generic branch")
                if not cam:HasProperty("TressFX") then
                    DrawShadowObjects(i)
                end
            end
        end
    end
    
   -- if ShadowMaps then
   --     ShadowMaps:Transition( SuGPUResource.STATE_RENDER_TARGET, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )
   -- end
   -- ShadowDepth:Transition( SuGPUResource.STATE_DEPTH_STENCIL_WRITE, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE );
   --
   -- UpdateShadowParams()
    ShadowPrint("==== Done Rendering Shadows")
end

-------------------------------------------------------
-- Draw objects for shadow map i. Figures out which objects 
-- are drawn to shadow map i.
-- 
-- Each shadow camera has a list of objects tags it should render.
-- It takes the properties, and finds all objects that has that property.
-- For example, if a camera has properties "ground" and "structure",
-- it will render all objects with the "ground" property, as well 
-- as objects with the "structure" property.
-- If the list is empty, it will just render everything labeled "scene".
-- It calls RenderShadowMap with each object list. 
--
-- Could probably be optimized by concatenating lists rather than
-- calling RenderShadowMap for each list. That would also eliminate
-- duplicates because an object appears on more than one list.
------------------------------------------------------

function DrawShadowObjects(i)
    local cam = Shadows.CamArray[i]
    ShadowPrint("Finding list for shadow cam "..Sushi.CM:GetCameraName(cam)) 
    local nLists = cam:GetNumProperties()
    if nLists == 0 or (nLists == 1 and cam:HasProperty("dynamic")) then
        ShadowPrint("no list")
        RenderShadowMap(Sushi.GetObjects {DrawProperty = "Scene"}, i)
    elseif cam:HasProperty("TressFX") then
        objList = {"TressFX"}
        RenderShadowMap(objList, i, true)
    else
        ShadowPrint("nLists = "..nLists)
        local bClear = true
        for iList = 1, nLists do
            local objListName = cam:GetProperty(iList-1)
            ShadowPrint("objListName= "..objListName)
            local objList = Sushi.GetObjects {DrawProperty = objListName, TechniqueProperties = { Shadows.DrawTechnique} }
            ShadowPrint("list size = "..#objList)
            if objList then
                --local bClear = (iList ~= 1)
                RenderShadowMap(objList, i, bClear)
                bClear = false
            end
        end
    end
end







-------------------------------------------------------
-- Render a set of objects into shadow map slice i
------------------------------------------------------

function RenderShadowMap(objects, i, bClear)
    --ShadowPrint("bClear= "..bClear)
    if bClear then ShadowPrint("clearing") else ShadowPrint("not clearing") end
    local flipScaleBiasMat = SuMatrix4( 0.5, 0.0, 0.0, 0.5,
                                       0.0,-0.5, 0.0, 0.5,
                                       0.0, 0.0, 1.0, 0.0,
                                       0.0, 0.0, 0.0, 1.0 );
    local camera = Shadows.CamArray[i] 

    if camera == nil then
        WarnOnce( "Custom ShadowCamera is nil" )
    else
        bShadow_Visible = camera:IsVisible()
        if not bShadow_Visible then
            WarnOnce("shadow cam not visible")
        end

        local fov = camera:GetFOV()
        local nearPlane = camera:GetNearPlaneDistance()
        local farPlane = camera:GetFarPlaneDistance()
        
        local cameraPos = camera:GetPosition()
        
        camera:SetFrustum(fov,1.0,nearPlane,farPlane)
        --camera:SetFrustum(fov,1.0,10, farPlane)
        if bShadow_Visible == true  and #objects > 0 then
            Sushi.PushOverrideCamera( camera )
            
            Shadow_VP_Matrix:MakeFromMatrix( flipScaleBiasMat * camera:GetViewProjectionMatrix() )
            ShadowPrint("rendering shadow "..i)
            SetShadowMatrix(i, Shadow_VP_Matrix)
            if ShadowMaps and ShadowMapsViews[i] == nil then
                WarnOnce("view is nil.")
            end

            local tressfxShadowClear = Sushi.RM:CreateGPUTimer("tressfx shadow clear")
            if objects[1] == "TressFX" then
                Sushi.RM:StartGPUTimer(tressfxShadowClear)
            end

            if Shadows.DepthOnly then
                Sushi.RM:SetRenderTargets( 0, {}, ShadowDSViewArray[i] )
            else
                Sushi.RM:SetRenderTargets( 1, {ShadowMapsViews[i]}, ShadowDSView )
            end
            Sushi.RM:SetClearDepth( 1.0 );
            Sushi.RM:SetClearColor( SuVector4(1.0,1.0,1.0,1.0) )
            Sushi.RM:ViewportAndScissor( 0, 0, nShadowMapWidth, nShadowMapHeight )
            if bClear then
                Sushi.RM:Clear(not Shadows.DepthOnly,true,false,0);
            end

            if objects[1] == "TressFX" then
                Sushi.RM:StopGPUTimer(tressfxShadowClear)
            end

            ShadowMapIndex = i-1
            
            ShadowPrint(string.format("drawing for camera %d %s",i,Sushi.CM:GetCameraName(camera)) )
            if objects[1] == "TressFX" then
                if hairShadowCheckbox:GetIsChecked() then
                    ShadowPrint("Drawing TressFX Shadow")
                    SuTressFXPlugin:DrawHairShadows()
                end
            else
                --Sushi.DrawObjects{ ObjectList = objects, TechniqueProperties = {"DepthCopy"} }
                Sushi.DrawObjects{ ObjectList = objects, TechniqueProperties = {Shadows.DrawTechnique} }
            end

         Sushi.PopOverrideCamera()
      end
   end

end

-------------------------------------------------------
-- Callback to render each object into the shadow map.
------------------------------------------------------
function ShadowRenderingStrategy( args )
    
    local objects = args.Objects;
    
    -- Sushi2-style light-object interaction
    for _, obj in pairs( objects ) do
    
        ShadowPrint("Shadow drawing ".. obj:GetName())
        
        if args.Technique ~= nil then
            obj:Draw( args.Technique )
        else
            if args.TechniqueProperties == nil then
                obj:Draw()
            else
                obj:Draw( table.getn( args.TechniqueProperties ), args.TechniqueProperties )
            end
        end
    end -- for all objects
    
end


--------------------------
-- Shadow Camera lists
--------------------------

Shadows.nCams = 0
Shadows.CamIndicies = {} -- List of static cameras indexed by cam pointer
Shadows.CamArray = {} -- Indexed list of cameras.

function Shadows.AddCam(cam)
    if Shadows.CamIndicies[cam] == nil then
        --print("adding shadow cam", Sushi.CM:GetCameraName(cam))
        Shadows.nCams = Shadows.nCams + 1
        Shadows.CamIndicies[cam] = Shadows.nCams
        Shadows.CamArray[Shadows.nCams] = cam
        local fRadius = GetRadius(cam)
        --print("radius ", fRadius)
        local fDappleFlag = GetDappleFlag(cam)
        --print("Dapple = ", fDappleFlag)
        Shadows.ParamsCPUBuffer:SetFloat32( (Shadows.nCams - 1)*ShadowParamsSize, 0, fRadius )
        Shadows.ParamsCPUBuffer:SetFloat32( (Shadows.nCams - 1)*ShadowParamsSize, 1, fDappleFlag )
        Shadows.ParamsCPUBuffer:SetFloat32( (Shadows.nCams - 1)*ShadowParamsSize, 2, cam:GetNearPlaneDistance() )
        Shadows.ParamsCPUBuffer:SetFloat32( (Shadows.nCams - 1)*ShadowParamsSize, 3, cam:GetFarPlaneDistance() )
        ShadowPrint("adding shadow cam")
        --PrintCamera(cam)
    end
    return Shadows.CamIndicies[cam] -- return zero-based
end




function GetRadius(cam)
    local nLists = cam:GetNumProperties()
    --print("num props = ",nLists)
    for iList = 1, nLists do
        local objListName = cam:GetProperty(iList-1)
        local iStart,_ = string.find(objListName, "radius")
        if iStart then
            return tonumber(string.sub(objListName,iStart+6))
         end
    end

    return 5
end

function GetDappleFlag(cam)
    local nLists = cam:GetNumProperties()
    --print("num props = ",nLists)
        for iList = 1, nLists do
            local objListName = cam:GetProperty(iList-1)
            local iStart,_ = string.find(objListName, "dappler")
            if iStart then
                return 1
            end
        end

       return 0
end



function SetShadowMatrix(iOneBased, matrix)
    for column = 0,3 do
        for row=0,3 do
            Shadows.MatrixCPUBuffer:SetFloat32( (iOneBased-1)*ShadowMatrixSize, row+column*4, matrix:Get(row,column) )
        end
    end
end

function UpdateShadowParams()


    local shadowParamUpdate = Sushi.RM:CreateGPUTimer("shadow param update")
    Sushi.RM:StartGPUTimer(shadowParamUpdate)

     --print("updating ",Shadows.nCams, "matrices")
    Shadows.MatrixBuffer:Transition( SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_COPY_DEST )
    Shadows.Params:Transition( SuGPUResource.STATE_PIXEL_SHADER_RESOURCE, SuGPUResource.STATE_COPY_DEST )

    Shadows.MatrixBuffer:UpdateBufferRange( Shadows.MatrixCPUBuffer, 0, Shadows.nCams )
    Shadows.Params:UpdateBufferRange( Shadows.ParamsCPUBuffer, 0, Shadows.nCams )

    Shadows.MatrixBuffer:Transition( SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )
    Shadows.Params:Transition( SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE )

    Sushi.RM:StopGPUTimer(shadowParamUpdate)

end


------------------------------------------------------
-- Debug draw of frustums.
------------------------------------------------------
function DrawShadowFrustums()


    if not LineRenderer then
        LineRenderer = SuBatchLineRenderer()
    end

   --local ShadowCamera = Sushi.GetCamera( "ShadowCamera" )

    ShadowCams = Shadows.CamArray
   --ShadowCams = { Sushi.GetCamera( "DirectionalShadowCam" ) }

    for i,cam in ipairs(ShadowCams) do
        local params = cam:GetFrustumParams()
        local left = params.x
        local right = params.y
        local bottom = params.z
        local top = params.w

        local near = cam:GetNearPlaneDistance()
        local far = cam:GetFarPlaneDistance()    
      
      
        LineRenderer:SetColor( SuVector4(0,0.5,1,1) )

        local ray0 = cam:GetPickRay(0,0)
        LineRenderer:DrawLine(ray0:GetPosition(0), ray0:GetPosition(far-near) )

        local ray1 = cam:GetPickRay(1,0)
        LineRenderer:DrawLine(ray1:GetPosition(0), ray1:GetPosition(far-near) )
        
        local ray2 = cam:GetPickRay(1,1)
        LineRenderer:DrawLine(ray2:GetPosition(0), ray2:GetPosition(far-near) )
        
        local ray3 = cam:GetPickRay(0,1)
        LineRenderer:DrawLine(ray3:GetPosition(0), ray3:GetPosition(far-near) )
        
        
        LineRenderer:DrawLine(ray0:GetPosition(0), ray1:GetPosition(0) )
        LineRenderer:DrawLine(ray1:GetPosition(0), ray2:GetPosition(0) )
        LineRenderer:DrawLine(ray2:GetPosition(0), ray3:GetPosition(0) )
        LineRenderer:DrawLine(ray3:GetPosition(0), ray0:GetPosition(0) )
        
        LineRenderer:DrawLine(ray0:GetPosition(far-near), ray1:GetPosition(far-near) )
        LineRenderer:DrawLine(ray1:GetPosition(far-near), ray2:GetPosition(far-near) )
        LineRenderer:DrawLine(ray2:GetPosition(far-near), ray3:GetPosition(far-near) )
        LineRenderer:DrawLine(ray3:GetPosition(far-near), ray0:GetPosition(far-near) )

        LineRenderer:Flush()

        LineRenderer:SetColor( SuVector4(1,1,1,1) )
        
    end
end



