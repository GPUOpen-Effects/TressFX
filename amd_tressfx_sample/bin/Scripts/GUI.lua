sdfIsoLevel  = 0
sdfCollisionMargin = 0


function Initialize()
    local right = Sushi.WM:GetWindowWidth()
    local GUIWidth = 300
    local GUIHeight = 600
    SliderWidth = 150
    
    window = Sushi.GUI:CreateGUIWindow ("TressFX", SuVector2(GUIWidth, GUIHeight), SuVector2(right-GUIWidth,0))

	window:CreateText("General Settings")
    window:CreateSeparator();
    shotCreditsWindowCheckBox = window:CreateCheckBox("Credits Window", false)
    shutDownButton = window:CreateButton("Shutdown", SuVector2(60,15), false)
    
    shortCutButton = window:CreateButton("Toggle Short Cut", SuVector2(0,0))
    Sushi.BindButton(shortCutButton, function() SuTressFXPlugin:ToggleShortCut() end)
    
    window:CreateSpace(2)

    window:CreateText("TressFX Settings")
    window:CreateSpace()
    drawFurCheckbox = window:CreateCheckBox("Draw fur", true)
    drawModelCheckbox = window:CreateCheckBox("Draw model", true)
    hairShadowCheckbox = window:CreateCheckBox("Hair shadow", true)

    SimulationControls()
    
    Parameters()

    perfWindow = Sushi.GUI:CreateGUIWindow("Perf", SuVector2(300, 250), SuVector2(0, 0))
    perfWindow:SetFontScale(1.2)
    perfText = perfWindow:CreateText("Performance")

    ShutDownWindow()
	Credits()
    
    Sushi.BindButton(shutDownButton, ShutDownSequence)

    ShutDownWindow:SetVisible(false)
    
    -- This gets set to false right away by HideGUI has to start as true though for code simplicity
    isVisible = true
    
    isControlsWindowVisible = true
    inFreeFly = false
    inArcBall = false
    
    ControlsWindow()
    freeFlyControlsWindow()
    arcBallControlsWindow()
    
    Sushi.BindKey( SuKeyEvent.KEY_ESCAPE,function() ShutDownSequence() end, "Begin Shut Down Sequence", true, true)
    Sushi.BindKey( SuKeyEvent.KEY_G, function() ToggleGUI() end, "HideGUI", true, true)
    Sushi.BindKey( SuKeyEvent.KEY_SPACE, function() freezeSimulation = not freezeSimulation end, "Freeze Simulation", true, true)
    
    Sushi.BindKey( SuKeyEvent.KEY_F, 
    function()
        if (not m_IsShuttingDown) then
            Sushi.ToggleFPSCamera() 
            inFreeFly = not inFreeFly
            if(inArcBall) then 
                inArcBall = false 
            end
        end
    end, "Start/Stop flying around manually like a lunatic" , true, true )
    
    Sushi.BindKey( SuKeyEvent.KEY_C, 
    function() 
        Sushi.ToggleArcBallCamera()
        inArcBall = not inArcBall
        if(inFreeFly) then 
            inFreeFly = false 
        end
    end, "Start/Stop Arc-Ballin' it", true, true )
    
    ToggleGUI()
end

function ControlsWindow()
    local right = Sushi.WM:GetWindowWidth()
    local bottom = Sushi.WM:GetWindowHeight()
    local width = 375
    local height = 60
    
    controlsWindow = Sushi.GUI:CreateGUIWindow ("Controls", SuVector2(width, height), SuVector2(right-width,bottom - height))
    controlsWindow:CreateText("Press 'G' to enable / disable the GUI")
    controlsWindow:CreateText("Press 'C' to enable / disable the Arc-ball Camera")
    controlsWindow:CreateText("Press 'F' to enable / disable the Free Fly Camera")
    
    
end

function freeFlyControlsWindow()
    local right = Sushi.WM:GetWindowWidth()
    local bottom = Sushi.WM:GetWindowHeight()
    local width = 375
    local height = 60
    
    fpsControlsWindow = Sushi.GUI:CreateGUIWindow ("FPSControls", SuVector2(width, height), SuVector2(right/2-width/2,bottom - height))
    fpsControlsWindow:CreateText("Use 'W,A,S,D' to move Forward, Backward, Left & Right")
    fpsControlsWindow:CreateText("Use 'Q' and 'E' to move Up and Down")
    fpsControlsWindow:CreateText("Use the mouse to look around. Hold control to regain curser")
    
end

function arcBallControlsWindow()
    local right = Sushi.WM:GetWindowWidth()
    local bottom = Sushi.WM:GetWindowHeight()
    local width = 350
    local height = 60
    
    abControlsWindow = Sushi.GUI:CreateGUIWindow ("ABControls", SuVector2(width, height), SuVector2(right/2-width/2,bottom - height))
    abControlsWindow:CreateText("Use the mouse wheel to zoom in and out.")
    abControlsWindow:CreateText("Hold left mouse button to orbit current position.")
    abControlsWindow:CreateText("Hold right mouse button to pan the camera.")
    
end

function ToggleGUI()

    if not (m_IsShuttingDown) then
        isVisible = not isVisible
        beforeCreditsChecked = shotCreditsWindowCheckBox:GetIsChecked()
        
        window:SetVisible(isVisible)
        perfWindow:SetVisible(isVisible)
        paramsWindow:SetVisible(isVisible)
        
        if(inFreeFly) then
            Sushi.WM:ShowMouse(false)
        else
            Sushi.WM:ShowMouse(isVisible)
        end
        
        if(isVisible) then
            CreditsWindow:SetVisible(beforeCreditsChecked)
            ShutDownWindow:SetVisible(false)
        else
            CreditsWindow:SetVisible(false)
            ShutDownWindow:SetVisible(false)
        end
    end
    
end

function ShutDownSequence()

    m_IsShuttingDown = true
    
    if(inFreeFly) then
        Sushi.ToggleFPSCamera() 
        inFreeFly = not inFreeFly
    end
    
    window:SetVisible(false)
    perfWindow:SetVisible(false)
    paramsWindow:SetVisible(false)
    controlsWindow:SetVisible(false)
    abControlsWindow:SetVisible(false)
    fpsControlsWindow:SetVisible(false)
    CreditsWindow:SetVisible(true)
    ShutDownWindow:SetVisible(true)
    Sushi.WM:ShowMouse(true)
    
    local waitTime = 3.0
    ShutDownTimer = Sushi.demoTimer:GetTime() + waitTime;
    
end

function ShutDownWindow()

    local right = Sushi.WM:GetWindowWidth()
    local bottom = Sushi.WM:GetWindowHeight()
    
    local windowWidth = 500;
    local windowHeight = 100;
    
    ShutDownWindow = Sushi.GUI:CreateGUIWindow("", SuVector2(windowWidth, windowHeight), SuVector2(right-right/2 - windowWidth/2,bottom - bottom/2 - windowHeight/2 - 300))
    ShutDownWindow:SetFontScale(2.0)
    
    ShutDownWindow:CreateText("Are you sure you want to shut down?")
    ShutDownWindow:CreateText("        ", false)
    yesButton = ShutDownWindow:CreateButton("Yes", SuVector2(100,30),true)
    ShutDownWindow:CreateText("  ", true)
    noButton = ShutDownWindow:CreateButton("No", SuVector2(100,30),true)
  
    Sushi.BindButton(yesButton,
    function()
            Sushi.WM:Exit()
    end)
    
    Sushi.BindButton(noButton, 
    function()
        m_IsShuttingDown = false
        CreditsWindow:SetVisible(shotCreditsWindowCheckBox:GetIsChecked())
        ShutDownWindow:SetVisible(false)
        controlsWindow:SetVisible(true)
        window:SetVisible(isVisible)
        perfWindow:SetVisible(isVisible)
        paramsWindow:SetVisible(isVisible)
        Sushi.WM:ShowMouse(isVisible)
    end)
end

function Credits()
	local right = Sushi.WM:GetWindowWidth()
    local bottom = Sushi.WM:GetWindowHeight()
    
    local windowWidth = 400;
    local windowHeight = 375;

	CreditsWindow = Sushi.GUI:CreateGUIWindow("Credits Window", SuVector2(windowWidth, windowHeight), SuVector2(right-right/2 - windowWidth/2,bottom - bottom/2 - windowHeight/2))
	CreditsWindow:SetFontScale(1.5)
    
	CreditsWindow:CreateText("        GPU Open Effects Team")
	CreditsWindow:CreateSeparator();
    CreditsWindow:CreateText("             Dongsoo Han")
	CreditsWindow:CreateText("             Eric Larsen")
    CreditsWindow:CreateText("           Karl Hillesland")
	CreditsWindow:CreateText("              Ryan Mayne")
	CreditsWindow:CreateText("             Sean Skelton")
    CreditsWindow:CreateText("              Tim Heath")
	
	CreditsWindow:CreateSpace(3)

	CreditsWindow:CreateText("                Humber")
	CreditsWindow:CreateSeparator()
    CreditsWindow:CreateText("       Cory Avery - Instructor")
    CreditsWindow:CreateText("   Jacob Cattapan - Environment")
    CreditsWindow:CreateText("        Matt Cook - Animator")
	CreditsWindow:CreateText("      Saul Garcia - Animator")
    
    CreditsWindow:CreateSpace(3)
    
    CreditsWindow:CreateText("      GPU Open Effects Management")
	CreditsWindow:CreateSeparator();
    CreditsWindow:CreateText("            Carlos Silva")
    CreditsWindow:CreateText("          Nicolas Thibieroz")
    
end

function Parameters()
    paramsWindow  = Sushi.GUI:CreateGUIWindow("Simulation Parameters Window", SuVector2(300, 635), SuVector2(0, 250))
    paramsWindow:CreateText("Parameters")
    paramsWindow:CreateSeparator()
    paramsWindow:CreateSpace(2)

    fiberRadiusSlider = paramsWindow:CreateSliderFloat("FIber Radius", SliderWidth, 0.21, 0, 1)
    fiberSpacingSlider = paramsWindow:CreateSliderFloat("Fiber Spacing", SliderWidth, 0.1,0,1)
    ratioSlider = paramsWindow:CreateSliderFloat("Fiber Ratio", SliderWidth, 0.5,0.0,1.0)

    g_MatKValue.y = 0.07
    g_MatKValue.z = 0.17
    g_MatKValue.w = 14.4
    g_fHairKs2 = 0.72
    g_fHairEx2 = 11.8
    KdSlider = paramsWindow:CreateSliderFloat("Kd", SliderWidth, g_MatKValue.y, 0, 1)
    Ks1Slider = paramsWindow:CreateSliderFloat("Ks1", SliderWidth, g_MatKValue.z, 0, 1)
    Ex1Slider = paramsWindow:CreateSliderFloat("Ex1", SliderWidth, g_MatKValue.w, 0, 20)
    Ks2Slider = paramsWindow:CreateSliderFloat("Ks2", SliderWidth, g_fHairKs2, 0, 1)
    Ex2Slider = paramsWindow:CreateSliderFloat("Ex2", SliderWidth, g_fHairEx2, 0, 20)
    
	paramsWindow:CreateSeparator()
    paramsWindow:CreateText("Mohawk")
    localConstraintsSliderMohawk = paramsWindow:CreateSliderInt("Local Constraints Iterations   - M", 100.0, 3, 0, 20) -- adding some arbitrary character at the end of name to distinguish this control from the one for Short. This is a hacky way to make it unique. 
    lengthConstraintsSliderMohawk = paramsWindow:CreateSliderInt("Length Constraints Iterations   - M", 100.0, 3, 1, 20)
	localConstraintsStiffnessSliderMohawk = paramsWindow:CreateSliderFloat("Local Constraints Stiffness    - M", 100.0, 0.9, 0.0, 1.0)
	globalConstraintsStiffnessSliderMohawk = paramsWindow:CreateSliderFloat("Global Constraints Stiffness    - M", 100.0, 0.4, 0.0, 1.0)
	globalConstraintsRangeSliderMohawk = paramsWindow:CreateSliderFloat("Global Constraints Range       - M", 100.0, 0.3, 0.0, 1.0)
    dampingSliderMohawk = paramsWindow:CreateSliderFloat("Damping                       - M", 100.0, 0.06, 0.0, 1.0)

	-- Velocity shock propagation amount value
    velShockProgationSliderMohawk = paramsWindow:CreateSliderFloat ("VSP amount                 - M", SliderWidth, 0.75, 0, 1.0);

	-- Velocity shock propagation pseudo-acceleration threshold
	vspAccelThresholdMohawk = paramsWindow:CreateSliderFloat ("VSP accel threshold                 - M", SliderWidth, 1.2, 0, 10.0);

    paramsWindow:CreateSeparator()
    paramsWindow:CreateText("Fur")
    localConstraintsSliderShort = paramsWindow:CreateSliderInt("Local Constraints Iterations", 100.0, 2, 0, 20)
    lengthConstraintsSliderShort = paramsWindow:CreateSliderInt("Length Constraints Iterations", 100.0, 2, 0, 20)
	localConstraintsStiffnessSliderShort = paramsWindow:CreateSliderFloat("Local Constraints Stiffness", 100.0, 0.8, 0.0, 1.0)
	globalConstraintsStiffnessSliderShort = paramsWindow:CreateSliderFloat("Global Constraints Stiffness", 100.0, 0.0, 0.0, 1.0)
	globalConstraintsRangeSliderShort = paramsWindow:CreateSliderFloat("Global Constraints Range", 100.0, 0.0, 0.0, 1.0)
    dampingSliderShort = paramsWindow:CreateSliderFloat("Damping", 100.0, 0.035, 0.0, 1.0)

	-- Velocity shock propagation amount value
    velShockProgationSliderShort = paramsWindow:CreateSliderFloat ("VSP amount", SliderWidth, 0.65, 0, 1.0);

	-- Velocity shock propagation pseudo-acceleration threshold
	vspAccelThresholdShort = paramsWindow:CreateSliderFloat ("VSP accel threshold", SliderWidth, 1.2, 0, 10.0);
    
	paramsWindow:CreateSeparator()
	thinTipCheckBox = paramsWindow:CreateCheckBox("Thin Tip", true)
    
    alphaSlider = paramsWindow:CreateSliderFloat("Hair Opacity", SliderWidth, 0.63, 0.0, 1.0)
    
end


function SimulationControls()
    window:CreateSpace(2)
    window:CreateSeparator()
    window:CreateText("Debug rendering")

    -- Draw collision mesh
    tressfxDrawColMeshCheckbox = window:CreateCheckBox ("Draw collision mesh", false)

    -- Draw SDF grid
    --SDFGridCheckbox = window:CreateCheckBox ("Draw SDF grid", false)

    -- Draw marching cubes
    marchingCubesCheckbox = window:CreateCheckBox ("Draw marching cubes", false)

    -- Control iso level for SDF
    --sdfIsoLevelSlider = window:CreateSliderFloat ("Iso level for SDF drawing",SliderWidth, sdfIsoLevel, -10, 10);

    window:CreateSpace()
    window:CreateSeparator()
    window:CreateText("Animation/simulation")

    -- Update collision mesh - skinning
    tressfxUpdateColMeshCheckbox = window:CreateCheckBox("Update collision mesh", true)

    -- Update SDF
    --tressfxUpdateSDFCheckbox = window:CreateCheckBox("Update SDF", false)

    -- SDF collision
    tressfxCollisionCheckbox = window:CreateCheckBox("Run SDF collision", true)

    -- SDF collision margin
    --sdfCollisionMarginSlider = window:CreateSliderFloat ("Collision margin", SliderWidth, sdfCollisionMargin, -1, 5);
end

function FindTimer(results, timerName)
    local nTimers = results:GetNumTimers()

    for i=0,nTimers-1 do
        if results:IsTimedThisFrame(i) then
            if results:GetTimerName(i) == timerName then   
                return i
            end
        end
    end

    return -1
end

function RoundToNearestNth(num,nth)
    return math.floor(num*nth + 0.5)/nth
end


function FormatTimerString(name, seconds)
   -- print("name "..name.." seconds "..seconds)
   local ms = seconds*1000
   if ms < 0.1 then
      return string.format("%-25s: %.2f ms\n",name, RoundToNearestNth(ms,100))
   else
      return string.format("%-25s: %.1f ms\n",name, RoundToNearestNth(ms,10))
   end
end

function GetTimerString(results, timerName)
    local timerIndex = FindTimer(results, timerName)

    if (timerIndex ~= -1) then
       return FormatTimerString(results:GetTimerName(timerIndex), results:GetFrameTotal(timerIndex))
    else
        return ""
    end
end

function GetRenderTimerString(results, timerIndex)

    if not results:IsTimedThisFrame(timerIndex) then
        return ""
    end

    local renderTag = "render: "
    local tagLength = string.len(renderTag)
    local timerName = results:GetTimerName(timerIndex)
    if string.sub(string.lower(timerName), 1, tagLength) == renderTag then
        local renderTimerName = string.sub(timerName, tagLength + 1, -1)
        return FormatTimerString(renderTimerName, results:GetFrameTotal(timerIndex))
    else
        return ""
    end
end

function GeneratePerfText()

    local fps = string.format("%.1f FPS", Sushi.Internal.FPSCounter.fps)
    t = string.format("%-25s\n", fps)

    local results = Sushi.RM:GetGPUTimerResults();
    t = t.."------------------------ GPU Times\n"

    -- Simulation timers
    t = t..GetTimerString(results, "Generating SDF")
    t = t..GetTimerString(results, "Simulation")
    t = t..GetTimerString(results, "Apply SDF")
    t = t..GetTimerString(results, "TRESSFX SIMULATION TOTAL")
    t = t.."------------------------\n"

    -- Rendering timers
    t = t..GetTimerString(results, "TRESSFX SHADOWS")
    t = t.."------------------------\n"
    local nTimers = results:GetNumTimers()
    for i=0,nTimers-1 do
        t = t..GetRenderTimerString(results, i)
    end
    t = t..GetTimerString(results, "TRESSFX RENDERING TOTAL")
    t = t.."------------------------\n"
    t = t..GetTimerString(results, "RENDER QUEUE TOTAL")
    t = t.."------------------------\n"


    return t
end


function PropogateGUISettings()
    fAmbientScale = 0.0
 
    bEnableShadows = true
    fGlobalSpecularScale = 1
    fLightScale = 1
    
    perfText:SetText(GeneratePerfText())

    if(isVisible) then
        CreditsWindow:SetVisible(shotCreditsWindowCheckBox:GetIsChecked())
    end
    
    fpsControlsWindow:SetVisible(inFreeFly)
    abControlsWindow:SetVisible(inArcBall)
    
    mohawkParameters = TressFXSimulationSettings()
    
    mohawkParameters.localConstraintsIterations = localConstraintsSliderMohawk:GetValue()
    mohawkParameters.damping = dampingSliderMohawk:GetValue()
    mohawkParameters.lengthConstraintsIterations = lengthConstraintsSliderMohawk:GetValue()
	mohawkParameters.localConstraintStiffness = localConstraintsStiffnessSliderMohawk:GetValue()
	mohawkParameters.globalConstraintStiffness = globalConstraintsStiffnessSliderMohawk:GetValue()
	mohawkParameters.globalConstraintsRange = globalConstraintsRangeSliderMohawk:GetValue()
    mohawkParameters.vspCoeff = velShockProgationSliderMohawk:GetValue()
	mohawkParameters.vspAccelThreshold = vspAccelThresholdMohawk:GetValue()

	g_FiberRadius = fiberRadiusSlider:GetValue()
    g_FiberSpacing = fiberSpacingSlider:GetValue()
	g_bThinTip = thinTipCheckBox:GetIsChecked()
    g_Ratio = ratioSlider:GetValue()

    g_MatKValue.y = KdSlider:GetValue()
    g_MatKValue.z = Ks1Slider:GetValue() / 100.0
    g_MatKValue.w = Ex1Slider:GetValue()
    g_fHairKs2 = Ks2Slider:GetValue() / 10.0
    g_fHairEx2 = Ex2Slider:GetValue()

    local alpha = alphaSlider:GetValue()
    
    g_cHairColor = SuVector4(1, 1, 1, alpha)

    shortHairParameters = TressFXSimulationSettings()
    
    shortHairParameters.localConstraintsIterations = localConstraintsSliderShort:GetValue()
    shortHairParameters.damping = dampingSliderShort:GetValue()
    shortHairParameters.lengthConstraintsIterations = lengthConstraintsSliderShort:GetValue()
	shortHairParameters.localConstraintStiffness = localConstraintsStiffnessSliderShort:GetValue()
	shortHairParameters.globalConstraintStiffness = globalConstraintsStiffnessSliderShort:GetValue()
	shortHairParameters.globalConstraintsRange = globalConstraintsRangeSliderShort:GetValue()
    shortHairParameters.vspCoeff = velShockProgationSliderShort:GetValue()
	shortHairParameters.vspAccelThreshold = vspAccelThresholdShort:GetValue()

    SuTressFXPlugin:UpdateSimulationParameters(mohawkParameters, shortHairParameters)
    
    -- SSAO Settings
    fSampleSize = 16
    bBlur = true
    SSAO.fSampleRadius = 5.0
    bShadowsEnable = true
    
end


