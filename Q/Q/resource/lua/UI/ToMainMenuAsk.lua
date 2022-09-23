function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    GameStateManager:RemoveState("IN_GAME_MENU");
    GameStateManager:AddState(Utils.GetHashID("BackToMainMenuCheck"));

    SoundComponent = Utils.GetSoundComponent()
    SoundComponent:PauseAll()

end

function OnUpdate(dt)

end

function OnCleanUp()
end