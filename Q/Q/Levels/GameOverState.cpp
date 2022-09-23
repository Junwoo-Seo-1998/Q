/*
    File Name: GameOverState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GameOverState.h"

#include "../Engine/Graphics/Window.h"
#include "../Engine/Engine.h"
#include "MainMenuState.h"
//entities
#include "../Game/Entity/UIEntities.h"
#include "../Engine/Entity/CameraEntity.h"

//components
#include "../Engine/Core/EventManager.h"
#include "../Engine/Component/CameraShakeComponent.h"
#include "../Engine/Component/ColorComponent.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Engine/Component/SoundComponent.h"
#include "../Engine/Component/TextComponent.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Component/TextureComponent.h"
#include "../Engine/System/TimerManagerSystem.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/WidthHeightComponent.h"
#include "../Engine/System/TextureBoxUpdateSystem.h"
#include "../Engine/System/TextureDrawSystem.h"
#include "Event/ToMainMenuEvent.h"


void GameOverState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();



    const CameraEntity* const camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const TextureBoxEntity* const gameover_box = AddEntity<TextureBoxEntity>();
    AddComponent<TextureComponent>(gameover_box, "GameOver"_hashID);
    AddComponent<OffsetComponent>(gameover_box);
    AddComponent<TransformComponent>(gameover_box, glm::vec2{  });
    AddComponent<WidthHeightComponent>(gameover_box, glm::vec2{winsize.first , winsize.second });
    AddComponent<ColorComponent>(gameover_box, Color{0.f, 0.f, 0.f, 1.f });
    AddComponent<TextureBoxComponent>(gameover_box);

    AddUpdateSystem<TimerManagerSystem>();

    m_timerID = TimerManagerSystem::IssueTimer(5.f, true, [&]() mutable
        {
            Engine::StopSlowMotion();
            Engine::GetEngineComponent<EventManager>().AddEvent<ToMainMenuEvent>();
        });
  
    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();

    GameState::Load();
    //Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("splash-bgm",false);

    q_engine::Debugger::GetLogger().AddMessage("***Player Died****", q_engine::SeverityLevel::INFORMATIONAL);
    q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void GameOverState::Update(double deltaTime)
{
    using namespace q_engine;
    GameState::Update(deltaTime);
    //Engine::GetEngineComponent<SoundComponent>().StopSoundAll(0);
}

void GameOverState::Unload()
{
    q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().StopSoundAll();
    q_engine::MapGeneration::ReGeneration();
    GameState::Unload();
}
