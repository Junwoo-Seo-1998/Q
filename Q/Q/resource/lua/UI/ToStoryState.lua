function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    GameStateManager:ClearStates()
    GameStateManager:AddState(Utils.GetHashID("ToStoryState"));
    --[GameStateManager:AddState(Utils.GetHashID("UITestState"));

    SoundComponent = Utils.GetSoundComponent()

end

function OnUpdate(dt)

end

function OnCleanUp()
end