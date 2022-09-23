function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    --[GameStateManager:ClearStates()
    GameStateManager:RemoveState("IN_GAME_MENU");
    SoundComponent = Utils.GetSoundComponent()
    SoundComponent:ResumeAll()
end

function OnUpdate(dt)

end

function OnCleanUp()
end