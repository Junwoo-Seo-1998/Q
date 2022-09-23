function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    --[GameStateManager:ClearStates()
    GameStateManager:RemoveState("IN_GAME_MENU");
    GameStateManager:AddState(Utils.GetHashID("InGameMenu"));
end

function OnUpdate(dt)

end

function OnCleanUp()
end