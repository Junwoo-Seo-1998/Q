/*
    File Name: MainMenuState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary: 
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "MainMenuState.h"
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
#include "Engine/System/ScriptSystem.h"
#include "Entity/MapEntity.h"
#include "Entity/UIEntities/UserFocusEntity.h"
#include "System/StarBGUpdateDrawSystem.h"
#include "Engine/Graphics/PostProcessor.h"


void MainMenuState::Load()
{
    using namespace q_engine;

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

    const glm::vec2 menu_box_size = { winsize.first / 4, winsize.second / 12 };
    const glm::vec2 menu_box_interval{ 0, 1.5 * menu_box_size.y };

    glm::vec2 menu_box_pos = glm::vec2{ (winsize.first - menu_box_size.x) / 2, menu_box_size.y * 2.7 };
    const float text_size = menu_box_size.y / 2.5f;

    menu_box_pos -= menu_box_interval;
    auto MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(),"resource/lua/UI/QuitCheck.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    const UITextEntity*  UIText = AddEntity<UITextEntity>();
    const std::string exit_string = "Exit";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, exit_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (exit_string.size() / 2.f) * text_size * 0.65f, 0.f }
        + glm::vec2{ -menu_box_size.x * 0.007, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToHowToPlayState.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    const std::string instruction_string = "HowToPlay";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, instruction_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (instruction_string.size() / 2.f) * text_size * 0.8f, 0.f }
    + glm::vec2{ -menu_box_size.x * 0.012, menu_box_size.y * 0.05 });


    menu_box_pos += menu_box_interval;

    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToCredit.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    const std::string credit_string = "Credit";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, credit_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (credit_string.size() / 2.f) * text_size * 0.65f, 0.f }
    + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    menu_box_pos += menu_box_interval;

    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToOptionState.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<ColorComponent>(MenuBox, Color{ 0, 200, 0, 150 });
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    const std::string option_string = "Option";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, option_string, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (option_string.size() / 2.f) * text_size * 0.65f, 0.f }
    + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });


    menu_box_pos += menu_box_interval;
    MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToStoryState.lua");
    AddComponent<TextureComponent>(MenuBox, "MenuBox"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<TransformComponent>(MenuBox, menu_box_pos);
    AddComponent<WidthHeightComponent>(MenuBox, menu_box_size.x, menu_box_size.y, true);
    AddComponent<TextureBoxComponent>(MenuBox);
    AddComponent<OffsetComponent>(MenuBox);

    UIText = AddEntity<UITextEntity>();
    const std::string play_string = "Play";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, std::string{ "Play" }, text_size, Color{ 0, 0, 0, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menu_box_pos.y + text_size * 0.5f } - glm::vec2{ (play_string.size() / 2.f) * text_size * 0.65f, 0.f } 
    + glm::vec2{ 0.0, menu_box_size.y * 0.05 });

    Texture& logo_tex = AssetManager::GetTexture("Q"_hashID);
    const float original_size_ratio = static_cast<float>(logo_tex.GetHeight()) / logo_tex.GetWidth(); 
    glm::vec2 main_logo_size = { winsize.first / 10.f , 1.f };
    main_logo_size.y = main_logo_size.x * original_size_ratio;

    const MainLogoEntity* game_logo = AddEntity<MainLogoEntity>();
    AddComponent<TransformComponent>(game_logo, glm::vec2{winsize.first / 2.f, winsize.second - main_logo_size.y * 1.2f} + glm::vec2{ -main_logo_size.x / 2.f, 0.f }
    + glm::vec2{ -menu_box_size.x * 0.005, menu_box_size.y * 0.05 });
    AddComponent<TextureComponent>(game_logo, logo_tex.GetTextureID(), static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
    AddComponent<WidthHeightComponent>(game_logo, main_logo_size.x, main_logo_size.y, true);
    AddComponent<OffsetComponent>(game_logo);
    AddComponent<TextureBoxComponent>(game_logo);
    


    AddUpdateSystem<StarBGUpdateDrawSystem>(std::initializer_list<StarBGUpdateDrawSystem::StarInitializer>{
        { Rect{ glm::vec2{0.f, 0.f}, glm::vec2{winsize.first, winsize.second} }, 200}
    });

    //Sound
    Engine::GetEngineComponent<SoundComponent>().ResumeAll();
    Engine::GetEngineComponent<SoundComponent>().PlaySoundBG("MyVeryOwnDeadShip");
	
    GameState::Load();

    Engine::GetPostProcessor()->SetMode(PostProcessorMode::Normal);

    Debugger::GetLogger().AddMessage("MainMenuState Loaded", SeverityLevel::INFORMATIONAL);
}

void MainMenuState::Update(double deltaTime)
{
    using namespace q_engine;
    GameState::Update(deltaTime);
}

void MainMenuState::Unload()
{
    Engine::GetEngineComponent<SoundComponent>().StopSoundBG(2);
    GameState::Unload();
    q_engine::Debugger::GetLogger().AddMessage("MainMenuState Unloaded", q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayerShutdownAskState::Load()
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

    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/EndGame.lua");
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
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToMainMenu2.lua");
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
    happy_string = "Are you sure want to quit?";
    AddComponent<TextComponent>(UIText, "MenuFont"_hashID, happy_string, text_size, Color{ 255, 255, 255, 255 });
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.1f, menu_box_pos.y + text_size * 0.5f }
    - glm::vec2{ (happy_string.size() / 2.f) * text_size * 0.65f, 0.f } + glm::vec2{ -menu_box_size.x * 0.02, menu_box_size.y * 0.05 });

    glm::vec2 targetSize = { winsize.first, winsize.second };
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
    Debugger::GetLogger().AddMessage("InGameMenuState Loaded", SeverityLevel::INFORMATIONAL);
    q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayerShutdownAskState::Unload()
{
}
