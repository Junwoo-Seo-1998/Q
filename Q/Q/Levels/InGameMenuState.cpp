/*
    File Name: InGameMenuState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "InGameMenuState.h"

#include "../Engine/Component/CameraShakeComponent.h"
//#include "../Game/Component/ButtonComponent.h"
#include "../Engine/ECS/InputManager.h"
#include "../Engine/Graphics/Window.h"
#include "../Engine/Engine.h"
#include "../Engine/GameStateManager.h"

#include "../Game/Entity/UITestEntities.h"

#include "../Engine/Component/ColorComponent.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Engine/Component/TextComponent.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Component/TextureComponent.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/WidthHeightComponent.h"
#include "../Engine/Entity/CameraEntity.h"
#include "../Engine/System/TextDrawSystem.h"
#include "../Engine/System/TextureBoxUpdateSystem.h"
#include "../Engine/System/TextureDrawSystem.h"
#include "../Game/EObjectZDepth.h"

#include "Engine/Component/LuaScriptComponent.h"
#include "Engine/Scripting/ScriptManager.h"
#include "Engine/System/ScriptSystem.h"
#include "Entity/UIEntities.h"
#include "System/UI/ButtonManagingSystem.h"
#include "Game/Component/UIComponents/ButtonComponent.h"
#include "Engine/Graphics/PostProcessor.h"

void InGameMenuState::Load()
{
    using namespace q_engine;
    m_is_transparent = false;

    const auto winsize = q_engine::get_window_size();

    AddUpdateSystem<ScriptSystem>();
    AddUpdateSystem<ButtonManagingSystem>();

    auto camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const glm::vec2 menu_box_size = { winsize.first / 2, winsize.second / 10 };
    const glm::vec2 menu_box_interval{ 0, 2 * menu_box_size.y };

    const float text_size = menu_box_size.y / 2.5f;

    glm::vec2 menu_box_pos = glm::vec2{ (winsize.first - menu_box_size.x) / 2, menu_box_size.y * 2.6 };

    auto MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");

    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToMainMenuAsk.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    const UITextEntity* UIText = AddEntity<UITextEntity>();
    const std::string to_main_string = "Title Screen";

    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, to_main_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (to_main_string.size() / 2.f) * text_size * 0.65f, 0.f }
    + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/InGameOption.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    const std::string happy_string = "OPTION";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, happy_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } 
    - glm::vec2{ (happy_string.size() / 2.f) * text_size * 0.65f, 0.f } + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ResumeGame.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    const std::string resume_string = "Resume";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, resume_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } -
        glm::vec2{ (resume_string.size() / 2.f) * text_size * 0.65f, 0.f } + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    const UIBoxEntity* UIBox = AddEntity<UIBoxEntity>();
    AddComponent<TransformComponent>(UIBox, glm::vec2{ 0, 0 });
    AddComponent<WidthHeightComponent>(UIBox, static_cast<float>(winsize.first), static_cast<float>(winsize.second), true);
    AddComponent<ColorComponent>(UIBox, Color{ 0, 0, 0, 160 });

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

    GameState::Load();
    Engine::GetPostProcessor()->SetMode(PostProcessorMode::MenuBlur);

    q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PauseAll();

    Debugger::GetLogger().AddMessage("InGameMenuState Loaded", SeverityLevel::INFORMATIONAL);
    q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void InGameMenuState::Update(double deltaTime)
{
    GameState::Update(deltaTime);
}

void InGameMenuState::Unload()
{
    q_engine::Engine::GetPostProcessor()->SetMode(Normal);
    q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().ResumeAll();
    GameState::Unload();
    q_engine::ScriptManager::Get().SetCurrentScene(q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState().get());
    q_engine::Debugger::GetLogger().AddMessage("InGameMenuState Unloaded", q_engine::SeverityLevel::INFORMATIONAL);
    q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(q_engine::Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
    q_engine::InputManager::CallAllReleaseCallbacks();

}

void PlayerGoBackMainMenuAskState::Load()
{
    using namespace q_engine;
    m_is_transparent = false;

    const auto winsize = q_engine::get_window_size();

    AddUpdateSystem<ScriptSystem>();
    AddUpdateSystem<ButtonManagingSystem>();

    auto camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    const glm::vec2 menu_box_size = { winsize.first / 2, winsize.second / 10 };
    const glm::vec2 menu_box_interval{ 0, 2 * menu_box_size.y };

    const float text_size = menu_box_size.y / 2.5f;

    glm::vec2 menu_box_pos = glm::vec2{ (winsize.first - menu_box_size.x) / 2, menu_box_size.y * 2.6 };

    auto MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");

    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToMainMenu.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    const UITextEntity* UIText = AddEntity<UITextEntity>();
    const std::string to_main_string = "Yes";

    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, to_main_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (to_main_string.size() / 2.f) * text_size * 0.65f, 0.f }
    + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToInGameMenu2.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    std::string happy_string = "No";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, happy_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f }
    - glm::vec2{ (happy_string.size() / 2.f) * text_size * 0.65f, 0.f } + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    UIText = AddEntity<UITextEntity>();
    happy_string = "Are you sure go back to main menu?";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, happy_string, text_size * 0.8f, Color{ 255, 255, 255, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 1.9f, menu_box_pos.y + text_size * 0.5f }
    - glm::vec2{ (happy_string.size() / 2.f) * text_size * 0.65f, 0.f } + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    glm::vec2 targetSize = { winsize.first * 0.6, winsize.second * 0.6 };
    const TextureBoxEntity* first_item_texture_box = AddEntity<TextureBoxEntity>();
    AddComponent<TransformComponent>(first_item_texture_box, glm::vec2{ (winsize.first - targetSize.x) * 0.5f, (winsize.second - targetSize.y) * 0.5f });
    AddComponent<TextureComponent>(first_item_texture_box, "InGameMenuBackLayer"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_MENU_START));
    AddComponent<OffsetComponent>(first_item_texture_box);
    AddComponent<WidthHeightComponent>(first_item_texture_box, targetSize, true);
    AddComponent<ColorComponent>(first_item_texture_box, Color{ 255, 255, 255, 160 });
    AddComponent<TextureBoxComponent>(first_item_texture_box);

    Engine::GetPostProcessor()->SetMode(PostProcessorMode::MenuBlur);

    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddDrawSystem<TextDrawSystem>();

    q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PauseAll();

    GameState::Load();
    Debugger::GetLogger().AddMessage("InGameMenuState Loaded", SeverityLevel::INFORMATIONAL);
    q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayerGoBackMainMenuAskState::Unload()
{
    q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().ResumeAll();
}
