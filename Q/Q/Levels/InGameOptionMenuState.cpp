/*
    File Name: OptionMenuState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "InGameOptionMenuState.h"
#include "Level4State.h"
#include "../Engine/AssetManager.h"
#include "../Engine/Component/ColorComponent.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/WidthHeightComponent.h"
#include "../Game/Component/UIComponents/ButtonComponent.h"
#include "../Engine/Component/CameraShakeComponent.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Engine/Component/TextComponent.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Component/TextureComponent.h"
#include "../Engine/ECS/InputManager.h"
#include "../Engine/Graphics/Window.h"
#include "../Game/Entity/UITestEntities.h"
#include "../Engine/Entity/CameraEntity.h"
#include "../Engine/GameStateManager.h"
#include "../Engine/System/TextDrawSystem.h"
#include "../Engine/System/TextureBoxUpdateSystem.h"
#include "../Engine/System/TextureDrawSystem.h"
#include "../Game/Entity/UIEntities.h"
#include "../Game/EObjectZDepth.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "Engine/Graphics/PostProcessor.h"
#include "Engine/System/ScriptSystem.h"
#include "Entity/MapEntity.h"
#include "Entity/UIEntities/UserFocusEntity.h"
#include "System/StarBGUpdateDrawSystem.h"

void InGameOptionMenuState::Load()
{
    using namespace q_engine;
    m_is_transparent = false;

    AddUpdateSystem<ScriptSystem>();
    AddUpdateSystem<ButtonManagingSystem>();
    AddDrawSystem<TextDrawSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddUpdateSystem<TextureBoxUpdateSystem>();

    const auto winsize = get_window_size();

    auto camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    auto userFocus = AddEntity<UserFocusEntity>();
    AddComponent<TransformComponent>(userFocus, glm::vec2{ 0.f, 0.f });

    const glm::vec2 menu_box_size = { winsize.first / 4 * 0.8, winsize.second / 12 * 0.8 };
    const glm::vec2 menu_box_interval{ 0, 2.0 * menu_box_size.y };

    glm::vec2 menu_box_pos = glm::vec2{ (winsize.first - menu_box_size.x) / 2, menu_box_size.y * 5.9 };
    const float text_size = menu_box_size.y / 2.5f;


    menu_box_pos -= menu_box_interval;
    auto MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ResumeGame2.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    const UITextEntity* UIText = AddEntity<UITextEntity>();
    const std::string exit_string = "BACK";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, exit_string, text_size, Color{ 0, 0, 0, 255 }, ETextHorizontalAlign::LEFT);
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (exit_string.size() / 2.f) * text_size * 0.65f, 0.f } 
        + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 }
    );


    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonCheckBox.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    std::string instruction_string = "FULLSCREEN";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, instruction_string, text_size, Color{ 0, 0, 0, 255 }, ETextHorizontalAlign::LEFT);
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (instruction_string.size() / 2.f) * text_size * 0.8f, 0.f } + glm::vec2{ menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonSlider.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "SFX");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    instruction_string = "SFX";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, instruction_string, text_size, Color{ 0, 0, 0, 255 }, ETextHorizontalAlign::LEFT);
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (instruction_string.size() / 2.f) * text_size * 0.8f, 0.f } + glm::vec2{ 0.0, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonSlider.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "BG");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    instruction_string = "BG";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, instruction_string, text_size, Color{ 0, 0, 0, 255 }, ETextHorizontalAlign::LEFT);
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (instruction_string.size() / 2.f) * text_size * 0.8f, 0.f } + glm::vec2{ 0.0, menu_box_size.y * 0.05 });


    glm::vec2 targetSize = { winsize.first * 0.6, winsize.second * 0.6 };
    const TextureBoxEntity* first_item_texture_box = AddEntity<TextureBoxEntity>();
    AddComponent<TransformComponent>(first_item_texture_box, glm::vec2{ (winsize.first - targetSize.x) * 0.5f, (winsize.second - targetSize.y) * 0.5f });
    AddComponent<TextureComponent>(first_item_texture_box, "InGameMenuBackLayer"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<OffsetComponent>(first_item_texture_box);
    AddComponent<WidthHeightComponent>(first_item_texture_box, targetSize, true);
    AddComponent<ColorComponent>(first_item_texture_box, Color{ 255, 255, 255, 160 });
    AddComponent<TextureBoxComponent>(first_item_texture_box);

    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddDrawSystem<TextDrawSystem>();

    q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PauseAll();
    GameState::Load();
    Engine::GetPostProcessor()->SetMode(PostProcessorMode::MenuBlur);
    Debugger::GetLogger().AddMessage("MainMenuState Loaded", SeverityLevel::INFORMATIONAL);
}

void InGameOptionMenuState::Unload()
{
    Engine::GetPostProcessor()->SetMode(PostProcessorMode::Normal);
}
