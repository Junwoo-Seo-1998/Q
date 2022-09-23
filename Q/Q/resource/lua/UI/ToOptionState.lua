function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    GameStateManager:ClearStates()
    GameStateManager:AddState(Utils.GetHashID("ToOptionState"));

end

function OnUpdate(dt)

end

function OnCleanUp()

end