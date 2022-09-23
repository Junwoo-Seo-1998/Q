/*
    File Name: HowToPlayState.cpp
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "HowToPlayState.h"

#include "CreditState.h"

#include "../Game/EObjectZDepth.h"
#include "../Engine/Graphics/Window.h"
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
#include "../Game/Component/UIComponents/ButtonComponent.h"
//#include "../Game/Component/ButtonComponent.h"
#include "../Game/Entity/UITestEntities.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "Engine/System/ScriptSystem.h"
#include "System/UI/ButtonManagingSystem.h"

#include "../Engine/AssetManager.h"

void HowToPlayState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();

    auto FindWindowRelativeMiddlePos = [winsize](glm::vec2 objSize) -> glm::vec2
    {
        const glm::vec2 windowSize = { winsize.first, winsize.second };
        return (windowSize - objSize) * 0.5f;
    };

    AddUpdateSystem<ScriptSystem>();
    AddUpdateSystem<ButtonManagingSystem>();

    const CameraEntity* const camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    //Overall Layout
    {
        glm::vec2 OverAllLayoutSize = { AssetManager::GetTexture("HowToPlay"_hashID).GetWidth(), AssetManager::GetTexture("HowToPlay"_hashID).GetHeight() };

        const TextureBoxEntity* const credit_texture_entity = AddEntity<TextureBoxEntity>();
        AddComponent<TextureComponent>(credit_texture_entity, "HowToPlay"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_BG));
        AddComponent<OffsetComponent>(credit_texture_entity);
        AddComponent<TransformComponent>(credit_texture_entity, glm::vec2{ 0, 0 });
        AddComponent<WidthHeightComponent>(credit_texture_entity, glm::vec2{ winsize.first, winsize.second });
        AddComponent<TextureBoxComponent>(credit_texture_entity);
    }


    //KeyBoardLayOut
    {
        glm::vec2 KeyBoardLayoutSize = { AssetManager::GetTexture("KeyBoardLayout"_hashID).GetWidth(), AssetManager::GetTexture("KeyBoardLayout"_hashID).GetHeight() };

        KeyBoardLayoutSize *= 4.f;

        const TextureBoxEntity* const credit_texture_entity = AddEntity<TextureBoxEntity>();
        AddComponent<TextureComponent>(credit_texture_entity, "KeyBoardLayout"_hashID, static_cast<int>(EObjectZDepth::MAIN_MENU_FIRST_OBJ));
        AddComponent<OffsetComponent>(credit_texture_entity);
        AddComponent<TransformComponent>(credit_texture_entity, glm::vec2{ FindWindowRelativeMiddlePos(KeyBoardLayoutSize).x, (winsize.second - (KeyBoardLayoutSize.y + winsize.second * 0.2f)) });
        AddComponent<WidthHeightComponent>(credit_texture_entity, KeyBoardLayoutSize);
        AddComponent<TextureBoxComponent>(credit_texture_entity);
    }



    glm::vec2 menubox_size = { winsize.first / 5.f, winsize.second / 10.f };

    auto MenuBox = AddEntity<UIBoxEntity>();
    AddComponent<LuaScriptComponent>(MenuBox, "resource/lua/UI/ButtonMove.lua");
    AddComponent<ButtonComponent>(MenuBox, MenuBox->GetEntityID(), &GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/UI/ToMainMenu.lua");
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
    AddComponent<TransformComponent>(UIText, glm::vec2{ winsize.first / 2.f, menubox_size.y + text_size * 0.5f } - glm::vec2{ (return_string.size() / 2.f) * text_size * 0.8f, 0.f });


    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddDrawSystem<TextDrawSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddUpdateSystem<TextureBoxUpdateSystem>();

    GameState::Load();
}
