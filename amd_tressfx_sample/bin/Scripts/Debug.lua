
Debug = {}
Debug.MaxBinCount = 512


if USE_NETWORK_PLUGIN then
   debugPlugin = Sushi.LoadPlugin( "SuNetworkDebugPlugin"..ComputePluginSuffix() )
   
   
   if debugPlugin == nil then 
      error("Debug plugin load failed")
   else
      debugPlugin:SetNumMsgPerFrame(100)
      --debugPlugin:StartWebServer("../html");
      debugPlugin:StartTCPIPServer()
      print("Debug plugin loaded.")
   end
   
end

function Initialize()
	DebugBuffer = {}
	DebugBuffer.StructCount = 10 + Debug.MaxBinCount
	DebugBuffer.Buffer = Sushi.CreateStructuredBuffer { Name="DebugBuffer", 
							   StructSize = 4, 
							   StructCount = DebugBuffer.StructCount,
							   Fetchable = true, 
							   UnorderedAccess = true, 
							   NumMaxStagingBuffers = 10
							   }

	if not DebugBuffer.Buffer then
		error( "Unable to create Debug Buffer" )
	end
	
	DebugBuffer.ClearBuffer = SuMemoryBuffer:Allocate( DebugBuffer.StructCount * 4 )
	tolua.takeownership(DebugBuffer.ClearBuffer)
	for i = 0,DebugBuffer.StructCount-1 do
		DebugBuffer.ClearBuffer:SetUINT32(0, i, 0)
	end

end

function Debug.Clear()

    DebugBuffer.Buffer:Transition( SuGPUResource.STATE_UNORDERED_ACCESS, SuGPUResource.STATE_COPY_DEST )
	DebugBuffer.Buffer:UpdateBufferRange( DebugBuffer.ClearBuffer, 0, DebugBuffer.StructCount )
    DebugBuffer.Buffer:Transition( SuGPUResource.STATE_COPY_DEST, SuGPUResource.STATE_UNORDERED_ACCESS )

end

function Debug.DebugValues()

    local readbackPtr = DebugBuffer.Buffer:ReadbackBuffer()
    local readback = readbackPtr:get()

	local PixelCount = readback:GetUINT32(0,0)
	print("pixels = ", PixelCount)
	
	local FragmentCount = readback:GetUINT32(0,1)
	print("fragment count=",FragmentCount)

	local TailFragmentCount = readback:GetUINT32(0,2)
	print("tail fragment count=",TailFragmentCount)

	local MaxFragmentCount = readback:GetUINT32(0,3)
	print("max fragment count=", MaxFragmentCount)

	local EarlyRejects = readback:GetUINT32(0,4)
	print("Early Rejects=", EarlyRejects)

	local binOffset = 10

--	if DisplayModes[iDisplayMode]=="CountFragments" then
--		for i = 1, math.min(Debug.MaxBinCount, MaxFragmentCount) do
--			local binCount = readback:GetUINT32(0,binOffset + i - 1)
--			print("bin",i,binCount)
--		end
--	end

end


function KarlPrint(a)
    if Sushi.Config.Karl then
        print(a)
    end
end

