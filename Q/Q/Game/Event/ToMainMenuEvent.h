/*
    File Name: ToMainMenuEvent.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/Component/SoundComponent.h"
#include "Engine/Core/Event.h"
#include "Engine/Engine.h"
#include "Levels/MainMenuState.h"

class ToMainMenuEvent final : public q_engine::Event<ToMainMenuEvent>
{
public:
    void ResolveEvent() override
    {
        q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().StopSoundBG(0);
        auto& game_state_manager = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>();
        game_state_manager.ClearStates();
        game_state_manager.AddState(GameStateType::MAIN_MENU_BUTTON, std::make_shared<MainMenuState>());
    }
};
