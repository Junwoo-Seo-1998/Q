function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    GameStateManager:ClearStates()
    GameStateManager:AddState(Utils.GetHashID("QuitCheck"));
end

function OnUpdate(dt)

end

function OnCleanUp()
end