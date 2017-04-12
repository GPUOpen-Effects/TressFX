

LegacyLights = {}

function Initialize()

   Sushi.ObjectIniEnv["LightSet"] = Sushi.Internal.MakeLoadLightSetFunc
   Sushi.Internal._gLightVisualizer = SuLightVisualizer() 
   Sushi.Internal._gLightVisualizer.bEnabled = false

   Sushi.LM = SuLightManager:GetPtr();
   tolua.releaseownership( Sushi.LM );


   Sushi.BindKey( CTRL + SuKeyEvent.KEY_L,      Sushi.ToggleLightVisualization, "Toggle light visualization", false, true  )

   Sushi.BindKey( SuKeyEvent.KEY_F5, function()
                                       Sushi.ReloadLightSets();
                                      end, "Reload lightsets.", false, true );


end



function LegacyLights.Update()

   if SuTressFXPlugin then

      Sushi.UpdateLights()

   end

end



--------------------------------------------------------------------------------
-- From Legacy Engine Code



--=================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Toggles rendering of light sources
--=================================================================================================================================
function Sushi.ToggleLightVisualization()

   Sushi.Internal._gLightVisualizer.bEnabled = not Sushi.Internal._gLightVisualizer.bEnabled
   if Sushi.Internal._gLightVisualizer.bEnabled then
      print( "Light Visualization enabled" )
   else
      print( "Light Visualization disabled" )
   end
end

--=================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Disables rendering of light sources
--=================================================================================================================================
function Sushi.DisableLightVisualization()
   
   if Sushi.Internal._gLightVisualizer.bEnabled then
      print( "Light Visualization disabled" )
      Sushi.Internal._gLightVisualizer.bEnabled = false;
   end

end

--=================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Renders all light sources using the internal SuLightVisualizer
--=================================================================================================================================
function Sushi.Internal.DrawLights()

   if Sushi.Internal._gLightVisualizer and
      Sushi.Internal._gLightVisualizer.bEnabled then
      
      for _, light in SuLights( nil ) do
         Sushi.Internal._gLightVisualizer:DrawLight( light )
      end
   end
end


-- ================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Loads a Sushi light set
-- This function's parameter is a table containing named arguments, which are described below
--
-- \param FileName   The light set file name.  Required.
-- \param Properties A table containing a set of string properties to assign to the object.  Optional. 
--
-- \return           A pointer to the created object
-- ================================================================================================================================
function Sushi.LoadLightSet( args )
   if not args.Properties then args.Properties = {} end
   Sushi.LM:LoadLightSet( args.FileName, table.getn( args.Properties ), args.Properties )
end

-- ================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Reloads Sushi lights
-- ================================================================================================================================
function Sushi.ReloadLightSets( )
   SuLogManager:ResetSushiErrorLog()
   Sushi.LM:ReloadLightSets()
end

-- ================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Gets lights with a property
-- Finds all the lights with a given property
--
-- \param strProperty A property string
--
-- \return           A table of lights
-- ================================================================================================================================
function Sushi.GetLightsWithProperty( strProperty )
   
   local Lights = {}

   if not strProperty then return Lights end
   
   local nNumLights = Sushi.LM:GetNumLights()
   for i = 1,nNumLights do
      local light = Sushi.LM:GetLightByIndex( i-1 )
      if light:HasProperty( strProperty ) then table.insert( Lights, light ) end
   end

   return Lights

end

--[[
-- This is a stateful iterator for light manager lights.
-- The stateless version of this should be a little more efficient.
function SuLights( lights )

   if lights then return pairs( lights ) end
   
   local i = 0
   local n = LM:GetNumLights()
   
   return function()
      if i == n then return nil, nil end
      local light = LM:GetLightByIndex( i )
      i = i + 1
      return light:GetName(), light
   end
end
]]


--=================================================================================================================================
-- \ingroup LuaHelpers
-- \brief A function which iterates over a set of lights.
--=================================================================================================================================

function SuLights( lights )
   if lights then return pairs( lights ) end   
   local lm = Sushi.LM;
   
   local iterator = function( n, i )
      if i == n then return nil, nil end
      local light = lm:GetLightByIndex( i )
      return i + 1, light
   end
   
   return iterator, lm:GetNumLights(), 0
end

--=================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Updates all lights in the light manager according to their controller
-- 
--  The lights will be updated using Sushi.demoTimer as the time source.
-- 
--=================================================================================================================================
function Sushi.UpdateLights(  )

   local t = Sushi.demoTimer:GetTime();
   
   -- Update the lights
   for _, light in SuLights( nil ) do
      light:Update( t );
   end
end


--=================================================================================================================================
-- \ingroup LuaHelpers
-- \brief Creates a function that loads a light set
--
-- This function is used during object ini parsing
--=================================================================================================================================
function Sushi.Internal.MakeLoadLightSetFunc( args )
   local f = function()
      Sushi.LoadLightSet( args )
   end
   table.insert( Sushi.Internal.AssetLoadFunctions, f )
end
