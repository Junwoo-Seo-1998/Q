function OnInit()
    GameStateManager = Utils.GetGameStateManager()
    GameStateManager:ClearStates()
    GameStateManager:AddState(Utils.GetHashID("MainMenuState"))

    SoundComponent = Utils.GetSoundComponent()
    SoundComponent:PauseAll()

end

function OnUpdate(dt)

end

function OnCleanUp()

end