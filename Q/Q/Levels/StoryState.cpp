/*
    File Name: StoryState.cpp
    Project Name: Q
    Author(s):
        Primary: Sieun choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "StoryState.h"
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
#include "Level1State.h"
#include "MainMenuState.h"
#include "TutorialState.h"
#include "UITestState.h"


void StoryState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();

    const CameraEntity* const camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const TextureBoxEntity* const WaitingState = AddEntity<TextureBoxEntity>();
    AddComponent<TextureComponent>(WaitingState, "CubePiece"_hashID);
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

            if (!MapGeneration::tutorialFinished)
            {
                Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::GAME, std::make_shared<TutorialState>());
            }
            else
            {
                Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::GAME, std::make_shared <Level1State>());
            }
            Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::UI1, std::make_shared<UITestState>());
        }
    );

    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();

}

void StoryState::Update(double deltaTime)
{
    using namespace q_engine;
    GameState::Update(deltaTime);
}

void StoryState::Unload()
{
    using namespace q_engine;
    GameState::Unload();
    InputManager& inputManager = Engine::GetEngineComponent<InputManager>();

    inputManager.RemoveBinding("skip");
    inputManager.RemoveCallback("skip");

}
