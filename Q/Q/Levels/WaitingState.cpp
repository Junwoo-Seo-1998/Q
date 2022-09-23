/*
    File Name: WaitingState.cpp
    Project Name: Q
    Author(s):
        Primary: Sieun choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "WaitingState.h"
#include "Engine/Component/CameraShakeComponent.h"
#include "Engine/Component/ColorComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/TextureBoxComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/ECS/InputManager.h"
#include "Engine/Entity/CameraEntity.h"
#include "Engine/Graphics/Window.h"
#include "Engine/System/TextureBoxUpdateSystem.h"
#include "Engine/System/TextureDrawSystem.h"
#include "Entity/UIEntities.h"
#include "MainMenuState.h"


void WaitingState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();

    const CameraEntity* const camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const TextureBoxEntity* const WaitingState = AddEntity<TextureBoxEntity>();
    AddComponent<TextureComponent>(WaitingState, "Waitstate"_hashID);
    AddComponent<OffsetComponent>(WaitingState);
    AddComponent<TransformComponent>(WaitingState, glm::vec2{ });
    AddComponent<WidthHeightComponent>(WaitingState, glm::vec2{ winsize.first, winsize.second });
    AddComponent<ColorComponent>(WaitingState, Color{ 0.f, 255.f, 0.f, 0.f });
    AddComponent<TextureBoxComponent>(WaitingState);

    InputManager& inputManager = Engine::GetEngineComponent<InputManager>();

    Binding* Skip = new Binding("skip");
    Skip->BindInput(InputType::KEY_PRESS, Key::SPACE);
    inputManager.AddBinding(Skip);
    inputManager.AddCallback("skip", [&]()
    {
        Engine::GetEngineComponent<GameStateManager>().ClearStates();
        Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::MAIN_MENU_BUTTON, std::make_shared<MainMenuState>());
    }
    );

    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();

}

void WaitingState::Update(double deltaTime)
{
    using namespace q_engine;
    GameState::Update(deltaTime);
}

void WaitingState::Unload()
{
    using namespace q_engine;
    GameState::Unload();
    InputManager& inputManager = Engine::GetEngineComponent<InputManager>();

    inputManager.RemoveBinding("skip");
    inputManager.RemoveCallback("skip");

}
