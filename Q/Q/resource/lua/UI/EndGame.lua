function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    GameStateManager:ClearStates()
    GameStateManager:MakeGameEnded()
end

function OnUpdate(dt)

end

function OnCleanUp()

end