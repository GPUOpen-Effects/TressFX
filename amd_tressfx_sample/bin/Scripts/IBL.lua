






function TextureResourceName(objectName, textureName, objectDirectory, materialName)
	objectDirectory = objectDirectory or "Objects"
	materialName = materialName or "material"
    -- if loaded manually, there is a "./" at the start.
    -- if loaded through objects.ini, there is not.
	local name = objectDirectory.."/"..objectName..".suo2::"..materialName.."."..textureName
	--print("name=",name)
	return name
end



EnvViews = {}
EnvViews.Current = {}

function AddEnv(objectName, materialName)
	EnvViews[objectName] = {}
	local cosCubeName = TextureResourceName(objectName, "tIBLCosCube", "Objects", materialName)
	local ggxCubeName = TextureResourceName(objectName, "tIBLGGXCube", "Objects", materialName)
	EnvViews[objectName].CosCube = Sushi.GRM:GetNamedResource( cosCubeName ):get()
	EnvViews[objectName].GGXCube = Sushi.GRM:GetNamedResource( ggxCubeName ):get()
end

function SetEnv(objectName)

	EnvViews.Current.CosCube = EnvViews[objectName].CosCube
	EnvViews.Current.GGXCube = EnvViews[objectName].GGXCube
	
end


function SetupEnvs()

	--IBLObject = Sushi.LoadObject{ Object = "Objects/BasicSky.suo2", Name = "IBL" }
	AddEnv("BasicSky", "Background")
    SetEnv("BasicSky")

	--SunnyIBLObject = Sushi.LoadObject{ Object = "Objects/SunnyBackground.suo2", Name = "SunnyIBL" }
	--AddEnv("SunnyBackground", "SunnyBackground")

	--SetEnv("IBL")

end




