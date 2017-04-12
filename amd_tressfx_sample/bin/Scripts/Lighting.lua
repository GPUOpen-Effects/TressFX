
Lighting = {}  -- Namespace I'm not really using as much as I should.

MaxLights = 20

Lighting.nShadowCams = 0    -- Local count fo shadow cams.  Should probably reconcile with Shadows.nCams

fLightScale = 1.0
fGlobalSpecularScale  = 1.0
fAmbientScale  = 1.0
fBackgroundScale = 1.0

bDebugLights = false

function LightPrint(t)
    if bDebugLights then
        print(t)
    end
end


function Initialize()
	CreateLightingResources()
	Sushi.EM:AddDefine( "SU_MAX_LIGHTS", tostring(MaxLights) )
end


function CreateLightingResources()


   ShadowsUpperBuffer = Sushi.CreateStructuredBuffer { Name="ShadowsUpperBuffer",
                                                         StructSize = 4,
                                                         StructCount = MaxLights,
                                                         Fetchable = true,
                                                         UnorderedAccess = false }
   ShadowsUpperCPUBuffer = SuMemoryBuffer:Allocate(MaxLights * 4);
   tolua.takeownership(ShadowsUpperCPUBuffer)

   ShadowsIndexBuffer = Sushi.CreateStructuredBuffer { Name="ShadowsIndexBuffer",
                                                         StructSize = 4,
                                                         StructCount = MaxLights*MaxShadowMaps,
                                                         Fetchable = true,
                                                         UnorderedAccess = false }
   ShadowsIndexCPUBuffer = SuMemoryBuffer:Allocate(MaxLights*MaxShadowMaps * 4);
   tolua.takeownership(ShadowsIndexCPUBuffer)


end



function AddLightsToObjects(lightList, objList, ignoreCull)
	if lightList and objList then
		for _,obj in pairs( objList ) do
			for _,light in pairs(lightList) do
				if ignoreCull or (light:IlluminatesBoundingBox( obj:GetBoundingBox() ) == true) then
					LightPrint("adding "..light:GetName().." to "..obj:GetName())
					Sushi.objToLightMapping:AddActiveLightToObject( obj, light )
				end
			end
		end
	else
		--print("lightList or objList empty")
	end
end
function SetupLights()
   SortLightsForObjects()
   SetupLightsAndShadows()
end

Lighting.nShadowCams = 0

-------------------------------------------------------------
-- AddExplicitShadowCams
--
-- Light has explicit list of shadow cameras. 
-------------------------------------------------------------
function AddExplicitShadowCams(light, lightIndex)
      local nProperties = light:GetNumProperties()
      LightPrint("looking at "..light:GetName().." with "..nProperties.." properties")



      for iProperty = 0, nProperties-1 do
         local property = light:GetProperty(iProperty)
         LightPrint("property = "..property)
         local sCam = Sushi.GetCamera( property )


         if sCam ~= nil then
			--print("sCam props = ",sCam:GetNumProperties())
            nShadowIndex = Shadows.AddCam(sCam) - 1
            ShadowsIndexCPUBuffer:SetUINT32(0, Lighting.nShadowCams, nShadowIndex) -- zero based so I increment after.
            LightPrint("shadow "..Lighting.nShadowCams.." set to "..nShadowIndex)
            Lighting.nShadowCams = Lighting.nShadowCams + 1
          end
      end

      ShadowsUpperCPUBuffer:SetUINT32(0, lightIndex, Lighting.nShadowCams)
      LightPrint("setting shadows upper of light index "..lightIndex.." to "..Lighting.nShadowCams)
end

Lighting.AutoShadowCamControllers = {}
Lighting.AutoShadowCams = {}

-------------------------------------------------------------
-- AddAutoShadowCams
--
-- Light needs shadow cams added automatically.
-- Explicit shadow cams will be ignored. 
-- Only works for spotlights right now.
-------------------------------------------------------------
function AddAutoShadowCams(light, lightIndex)

	if light:GetType() ~= SuLight.SPOT_LIGHT then
		return
	end

	local spotLight = tolua.cast( light, "SuSpotlight" )
	if not spotLight then
		print("failed to cast to spotlight")
	end

	Lighting.AutoShadowCams[lightIndex] = Sushi.CM:CreateCamera( light:GetName().."AutoShadowCam" )
	tolua.takeownership(Lighting.AutoShadowCams[lightIndex])
	local cam = Lighting.AutoShadowCams[lightIndex]
	cam:AddProperty("dynamic")  -- tells the shadow system to update every frame
	near,far = GetNearAndFar(light)
	cam:SetFrustum(spotlight:GetConeAngle(), 1.0, near, far)

	Lighting.AutoShadowCamControllers[lightIndex] = SuSpotlightCameraController(spotLight)
	tolua.takeownership(Lighting.AutoShadowCamControllers[lightIndex])
	local controller = Lighting.AutoShadowCamControllers[lightIndex]

	-- Not only attaches the controller to the camera, but puts in on the list
	-- to update based on demo time (because third argument is false)
	Sushi.SetCameraController( cam, controller, false )


	nShadowIndex = Shadows.AddCam(cam) - 1
    ShadowsIndexCPUBuffer:SetUINT32(0, Lighting.nShadowCams, nShadowIndex) -- zero based so I increment after.
    --print("shadow ",Lighting.nShadowCams,"set to ",nShadowIndex)
    Lighting.nShadowCams = Lighting.nShadowCams + 1


    ShadowsUpperCPUBuffer:SetUINT32(0, lightIndex, Lighting.nShadowCams)
    --print("setting shadows upper to i,value",i,Lighting.nShadowCams)
end



function GetNearAndFar(light)
   local nLists = light:GetNumProperties()
   --print("num props = ",nLists)
   near = 10
   far = 1000
     for iList = 1, nLists do
         local property = light:GetProperty(iList-1)
         local iStart,_ = string.find(property, "near")
         if iStart then
            near = tonumber(string.sub(property,iStart+4))
         end
         iStart,_ = string.find(property, "far")
         if iStart then
            far = tonumber(string.sub(property,iStart+3))
         end
      end

      return near,far
end


function SetupLightsAndShadows()



   local nLights = Sushi.LM:GetNumLights()
   if nLights > MaxLights then
		print("nLights = ",nLights)
	end

   --local nLightCams = 0


	for i = 0, nLights - 1 do
		local light = Sushi.LM:GetLightByIndex(i)
		light:SetIndex(i)

		if light:HasProperty("auto") then
			AddAutoShadowCams(light, i)
		else
			AddExplicitShadowCams(light, i)
		end
   end

   ShadowsUpperBuffer:UpdateBufferRange( ShadowsUpperCPUBuffer, 0, nLights )
   ShadowsIndexBuffer:UpdateBufferRange( ShadowsIndexCPUBuffer, 0, Lighting.nShadowCams )
   ShadowsUpperBuffer:Transition(SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE)
   ShadowsIndexBuffer:Transition(SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_PIXEL_SHADER_RESOURCE)

end

function SortLightsForObjects()
	--print("SortLightsForObjects")

	Sushi.objToLightMapping:Clear()

	-- Lights marked "Env" will light everything.
	EnvLights = Sushi.GetLightsWithProperty( "Env" )
	--print("adding env lights")
	AddLightsToObjects( EnvLights, Sushi.Internal.AllObjects )

	-- because ground is so tricky.
	AddLightsToObjects( Sushi.GetLightsWithProperty( "TressFX" ), Sushi.Internal.ObjectsWithProperty[ "TressFX" ], true)
	AddLightsToObjects( Sushi.GetLightsWithProperty( "Hair" ), Sushi.Internal.ObjectsWithProperty[ "Hair" ], true)
	AddLightsToObjects( Sushi.GetLightsWithProperty( "Scene" ), Sushi.Internal.ObjectsWithProperty[ "Scene" ], true)

	--for _,name in pairs(ArtistSettings.LinkedLightSets) do
	--	--print("adding",name)
	--	local lights = Sushi.GetLightsWithProperty( name )
	--	local objects = Sushi.Internal.ObjectsWithProperty[ name ]
	--	AddLightsToObjects(lights, objects, true)
	--end

end


function LightRenderingStrategy( args )

	--SortLightsForObjects()
   local objects = args.Objects;

   -- Sushi2-style light-object interaction
   for _, obj in pairs( objects ) do

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

