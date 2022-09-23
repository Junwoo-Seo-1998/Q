function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    --[GameStateManager:ClearStates()
    GameStateManager:RemoveState("IN_GAME_MENU2");
    GameStateManager:AddState(Utils.GetHashID("InGameMenu"));

    SoundComponent = Utils.GetSoundComponent()
    SoundComponent:ResumeAll()
end

function OnUpdate(dt)

end

function OnCleanUp()
end