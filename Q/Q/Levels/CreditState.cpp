/*
    File Name: CreditState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CreditState.h"

#include "../Game/EObjectZDepth.h"
#include "../Engine/Graphics/Window.h"
#include "../Engine/Engine.h"
#include "MainMenuState.h"
#include "Engine/ECS/InputManager.h"

//entities
#include "../Game/Entity/UIEntities.h"
#include "../Engine/Entity/CameraEntity.h"

//components
#include "../Engine/Component/CameraShakeComponent.h"
#include "../Engine/Component/ColorComponent.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Engine/Component/TextComponent.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Component/TextureComponent.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/WidthHeightComponent.h"
#include "../Engine/System/TextDrawSystem.h"
#include "../Engine/System/TextureBoxUpdateSystem.h"
#include "../Engine/System/TextureDrawSystem.h"
#include "../Game/System/UI/ButtonManagingSystem.h"
#include "../Game/Entity/UITestEntities.h"
#include "Component/UIComponents/ButtonComponent.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "Engine/ECS/InputManager.h"
#include "Engine/System/ScriptSystem.h"

inline static bool need_add_or_swap_texture = true;

void CreditState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();

    need_add_or_swap_texture = true;

    AddUpdateSystem<ScriptSystem>();
    AddUpdateSystem<ButtonManagingSystem>();

    const CameraEntity* const camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const TextureBoxEntity* const credit_texture_entity = AddEntity<TextureBoxEntity>();
    AddComponent<TextureComponent>(credit_texture_entity, "TitleImage"_hashID);
    AddComponent<OffsetComponent>(credit_texture_entity);
    AddComponent<TransformComponent>(credit_texture_entity, glm::vec2{  });
    AddComponent<WidthHeightComponent>(credit_texture_entity, glm::vec2{ winsize.first, winsize.second });
    AddComponent<ColorComponent>(credit_texture_entity, Color{ 0.f, 255.f, 0.f, 0.f });
    AddComponent<TextureBoxComponent>(credit_texture_entity);


  /*  glm::vec2 menubox_size = { winsize.first / 5.f, winsize.second / 10.f };

    auto MenuBox = AddEntity<TextureBoxEntity>();
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<TransformComponent>(MenuBox, glm::vec2{ (winsize.first - menubox_size.x) * 0.5f, menubox_size.y });
    AddComponent<WidthHeightComponent>(MenuBox, menubox_size, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    const glm::vec2 menu_box_size = { winsize.first / 4, winsize.second / 12 };
    const float text_size = menu_box_size.y / 2.f;

    const UITextEntity* UIText = AddEntity<UITextEntity>();
    const std::string return_string = "Return";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, return_string, text_size, Color{ 50, 50, 50, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menubox_size.y + text_size * 0.5f } - glm::vec2{ (return_string.size() / 2.f) * text_size * 0.8f, 0.f });*/

	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	Binding* Skip = new Binding("NextOrder");
    Skip->BindInput(InputType::KEY_PRESS, Key::SPACE);
	inputManager.AddBinding(Skip);
    inputManager.AddCallback("NextOrder", [&]()
    {
    	if(order == SIX)
    	{
            Engine::GetEngineComponent<GameStateManager>().ClearStates();
            Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::MAIN_MENU_BUTTON, std::make_shared<MainMenuState>());
    	}
        else {
            need_add_or_swap_texture = true;
            ++order;
        }
    });

    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddDrawSystem<TextDrawSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddUpdateSystem<TextureBoxUpdateSystem>();

    GameState::Load();
}

void CreditState::Update(double )
{
    using namespace q_engine;
	
    auto ContextID = GetEntities<TextureBoxEntity>().Front().GetEntityID();
    if (need_add_or_swap_texture) {
        switch (order)
        {
        case TWO:
        {
            GetComponents<TextureComponent>()[ContextID].SetTexture("Credit2"_hashID);

            break;
        }
        case THREE:
        {
            GetComponents<TextureComponent>()[ContextID].SetTexture("Credit3"_hashID);

            break;
        }
        case FOUR:
        {
            GetComponents<TextureComponent>()[ContextID].SetTexture("Credit4"_hashID);

            break;
        }
        case FIVE:
        {
            GetComponents<TextureComponent>()[ContextID].SetTexture("Credit5"_hashID);

            break;
        }
        case SIX:
        {

            GetComponents<TextureComponent>()[ContextID].SetTexture("Credit6"_hashID);

            const auto winsize = get_window_size();
            glm::vec2 menubox_size = { winsize.first / 2.5f, winsize.second / 10.f };
            auto MenuBox = AddEntity<UIBoxEntity>();
            AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
            AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToMainMenu.lua");
            AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
            AddComponent<TransformComponent>(MenuBox, glm::vec2{ (winsize.first - menubox_size.x) * 0.5f, menubox_size.y + winsize.second * 0.05 });
            AddComponent<WidthHeightComponent>(MenuBox, menubox_size, true);
            AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
            AddComponent<TextureBoxComponent>(MenuBox);
            AddComponent<OffsetComponent>(MenuBox);

            const glm::vec2 menu_box_size = { winsize.first / 4, winsize.second / 12 };
            const float text_size = menu_box_size.y / 2.f;

            const UITextEntity* UIText = AddEntity<UITextEntity>();
            const std::string return_string = "Return";
            AddComponent<TextComponent>(UIText, "MenuFont"_hashID, return_string, text_size, Color{ 50, 50, 50, 255 });
            AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menubox_size.y + winsize.second * 0.05f + text_size * 0.7f } - glm::vec2{ (return_string.size() / 2.f) * text_size * 0.8f, 0.f });

            break;
        }
        }
        need_add_or_swap_texture = false;
    }

}

void CreditState::Unload()
{
    using namespace q_engine;
    GameState::Unload();
    InputManager& inputManager = Engine::GetEngineComponent<InputManager>();

    inputManager.RemoveBinding("NextOrder");
    inputManager.RemoveCallback("NextOrder");

}
