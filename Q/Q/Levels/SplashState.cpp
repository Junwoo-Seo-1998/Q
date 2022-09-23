/*
    File Name: SplashState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "SplashState.h"

#include "../Engine/Graphics/Window.h"
#include "../Engine/Engine.h"

//entities
#include "../Game/Entity/UIEntities.h"
#include "../Engine/Entity/CameraEntity.h"

//components
#include "../Engine/Component/CameraShakeComponent.h"
#include "../Engine/Component/ColorComponent.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Engine/Component/SoundComponent.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Component/TextureComponent.h"
#include "../Engine/System/TimerManagerSystem.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/WidthHeightComponent.h"
#include "../Engine/GameStateManager.h"
#include "../Engine/System/TextureBoxUpdateSystem.h"
#include "../Engine/System/TextureDrawSystem.h"
#include "WaitingState.h"
#include "../Engine/ECS/InputManager.h"


void SplashState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();

    const CameraEntity* const camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const TextureBoxEntity* const LogoEntity = AddEntity<TextureBoxEntity>();
    AddComponent<TextureComponent>(LogoEntity, "digipen_logo"_hashID);
    AddComponent<OffsetComponent>(LogoEntity);
    AddComponent<TransformComponent>(LogoEntity, glm::vec2{  });
    AddComponent<WidthHeightComponent>(LogoEntity, glm::vec2{ winsize.first, winsize.second });
    AddComponent<ColorComponent>(LogoEntity, Color{ 0.f, 255.f, 0.f, 0.f });
    AddComponent<TextureBoxComponent>(LogoEntity);

    AddUpdateSystem<TimerManagerSystem>();

    m_timerID = TimerManagerSystem::IssueTimer(3.f, true, [&]() mutable
        {
            auto& main_logo_entity = GetEntities<TextureBoxEntity>().Front();
            ContiguousHashMapIterator<TextureComponent> logo_texture = GetComponents<TextureComponent>().find(main_logo_entity.GetEntityID());
            logo_texture->SetTexture("fmod_logo"_hashID);
            TimerManagerSystem::GetTimer(m_timerID).ResetAndResume(2.f);
            TimerManagerSystem::GetTimer(m_timerID).SetOnTimeOut([&]() mutable
                {
                    GetComponents<TextureComponent>().find(main_logo_entity.GetEntityID())->SetTexture("TitleImage"_hashID);
                    TimerManagerSystem::GetTimer(m_timerID).ResetAndResume(2.f);
                    TimerManagerSystem::GetTimer(m_timerID).SetOnTimeOut([&]()
                        {
                            Engine::GetEngineComponent<GameStateManager>().ClearStates();
                            Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::MAIN_MENU_BUTTON, std::make_shared<WaitingState>());
                        });
                });
        });


    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();

    Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("splash-bgm",false);
}

void SplashState::Update(double deltaTime)
{
    using namespace q_engine;
    using q_engine::InputManager;
    if (TimerManagerSystem::HasTimer(m_timerID)) {
        if (InputManager::IsKeyPressed(Key::SPACE) || InputManager::IsKeyPressed(Key::ENTER) || InputManager::IsKeyPressed(Key::ESC)) {
            TimerManagerSystem::GetTimer(m_timerID).SetTimeOut();
        }
    }
    GameState::Update(deltaTime);
}

void SplashState::Unload()
{
    GameState::Unload();

}
