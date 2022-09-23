/*
    File Name: UITestState.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "UITestState.h"


#include "../Engine/Component/CameraShakeComponent.h"
#include "../Engine/Engine.h"
#include "../Engine/Component/ColorComponent.h"
#include "../Engine/Component/TextComponent.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/WidthHeightComponent.h"
#include "../Engine/Debugger/Debugger.h"
#include "../Engine/Graphics/Window.h"
#include "../Game/Entity/UITestEntities.h"
#include "../Engine/Core/StringHash.h"
#include "../Engine/GameStateManager.h"
#include "../Engine/System/TextDrawSystem.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Component/TextureComponent.h"
#include "../Engine/Entity/CameraEntity.h"
#include "../Engine/System/TextureBoxUpdateSystem.h"
#include "../Engine/System/TextureDrawSystem.h"
#include "../Game/Entity/UIEntities.h"
#include "../Game/System/StarBGUpdateDrawSystem.h"
#include "Component/UIComponents/UIItemComponents.h"
#include "Engine/System/TimerManagerSystem.h"
#include "Entity/UIEntities/InGameEntities.h"
#include "EObjectZDepth.h"
#include "System/UI/PlayerHealthBarUpdateSystem.h"
#include "System/UI/PlayerWeaponUIUpdateDrawSystem.h"
#include "System/UI/PlayerItemUIUpdateDrawSystem.h"

void UITestState::Load()
{
    using namespace q_engine;

    const auto winsize = get_window_size();

    auto camera = AddEntity<CameraEntity>();
    AddComponent<TransformComponent>(camera, glm::vec2{ winsize.first / 2.f, winsize.second / 2.f });
    AddComponent<CameraShakeComponent>(camera);

    //Add Systems
    AddUpdateSystem<TextureBoxUpdateSystem>();
    AddDrawSystem<TextDrawSystem>();
    AddDrawSystem<TextureDrawSystem>();
    AddUpdateSystem<TimerManagerSystem>();

    const glm::vec2 UI_layer_box_size = { winsize.first * 1.f, winsize.second / 5.f };
    {
        const glm::vec2 healthBoxSize = glm::vec2{ winsize.first / 3.5f, winsize.second / 13.f };
        const glm::vec2 healthBoxPos = glm::vec2{ winsize.first / 15.f, (UI_layer_box_size.y - healthBoxSize.y) / 2.f };

        HealthBarUIEntity* healthbar = AddEntity<HealthBarUIEntity>();
        AddComponent<TransformComponent>(healthbar, healthBoxPos);
        AddComponent<WidthHeightComponent>(healthbar, healthBoxSize);
        auto pHealthBarTextureComponent = static_cast<UIItemTextureComponent*>(AddComponent<UIItemTextureComponent>(healthbar));
        pHealthBarTextureComponent->RegisterTexture("Layer", "PlayerHealthBarLayer"_hashID);
        pHealthBarTextureComponent->RegisterTexture("Bar", "PlayerHealthBar"_hashID);
        pHealthBarTextureComponent->RegisterTexture("Empty", "Black"_hashID);
        AddDrawSystem<PlayerHealthBarUpdateDrawSystem>();
    }

    {
        //Adding Weapon UI
        const glm::vec2 weaponUIBoxSize = glm::vec2{ winsize.second / 10.f, winsize.second / 10.f };
        const glm::vec2 weaponUIBoxPos = glm::vec2{ winsize.first * 3.f / 4.f, (UI_layer_box_size.y - weaponUIBoxSize.y) / 2.f };

        WeaponBoxEntity* weaponUI = AddEntity<WeaponBoxEntity>();
        AddComponent<TransformComponent>(weaponUI, weaponUIBoxPos);
        AddComponent<WidthHeightComponent>(weaponUI, weaponUIBoxSize);
        auto pUIItemTextureComponent = static_cast<UIItemTextureComponent*>(AddComponent<UIItemTextureComponent>(weaponUI));
        pUIItemTextureComponent->RegisterTexture("Box", "Box"_hashID);
        pUIItemTextureComponent->RegisterTexture("Dagger", "DaggerUIIcon"_hashID);
        pUIItemTextureComponent->RegisterTexture("Gun", "DefaultGunIcon"_hashID);
        pUIItemTextureComponent->RegisterTexture("ShotGun", "ShotGunUIIcon"_hashID);
        pUIItemTextureComponent->RegisterTexture("Rocket", "RocketUIIcon"_hashID);

        AddDrawSystem<PlayerWeaponUIUpdateDrawSystem>();
    
        //Adding Item UI
        const glm::vec2 itemBoxSize = weaponUIBoxSize * 0.7f;
        glm::vec2 itemBoxPos = weaponUIBoxPos - glm::vec2{ itemBoxSize.x * 3.f, 0 };
        itemBoxPos.y = (UI_layer_box_size.y - itemBoxSize.y) / 2.f;
        ItemBoxEntity* itemUI = AddEntity<ItemBoxEntity>();

        AddComponent<TransformComponent>(itemUI, itemBoxPos);
        AddComponent<WidthHeightComponent>(itemUI, itemBoxSize);
        pUIItemTextureComponent = static_cast<UIItemTextureComponent*>(AddComponent<UIItemTextureComponent>(itemUI));
        pUIItemTextureComponent->RegisterTexture("Box", "Box"_hashID);
        pUIItemTextureComponent->RegisterTexture("Bomb", "BombUIIcon"_hashID);
        pUIItemTextureComponent->RegisterTexture("PoisonBomb", "PoisonBombUIIcon"_hashID);
        pUIItemTextureComponent->RegisterTexture("Potion", "PotionUIIcon"_hashID);
        pUIItemTextureComponent->RegisterTexture("Cube", "NormalCubeIcon"_hashID);

        AddDrawSystem<PlayeritemUIUpdateDrawSystem>();


        const UITextEntity* text = AddEntity<UITextEntity>();
        AddComponent<TagComponent>(text, "MajorItemText");
        AddComponent<TextComponent>(text, "MenuFont"_hashID, std::string{}, 15.f, Color{0.15f});
        AddComponent<TransformComponent>(text, itemBoxPos - glm::vec2{ 0.f, 20.f });

        text = AddEntity<UITextEntity>();
        AddComponent<TagComponent>(text, "MinorItemText");
        AddComponent<TextComponent>(text, "MenuFont"_hashID, std::string{}, 15.f * 0.7f, Color{ 0.15f });
        AddComponent<TransformComponent>(text, itemBoxPos + glm::vec2{ itemBoxSize.x * 1.3f, -20.f });

        text = AddEntity<UITextEntity>();
        AddComponent<TagComponent>(text, "CubeAmountText");
        AddComponent<TextComponent>(text, "MenuFont"_hashID, std::string{}, 15.f * 0.7f, Color{ 0.15f });
        AddComponent<TransformComponent>(text, itemBoxPos + glm::vec2{ -itemBoxSize.x * 4.05f, -20.f });
    }

    AddDrawSystem<StarBGUpdateDrawSystem>(std::initializer_list<StarBGUpdateDrawSystem::StarInitializer>{
        { Rect{ glm::vec2{0.f, 0.f}, glm::vec2{winsize.first, winsize.second} }, 200}
    });


    const TextureBoxEntity* first_item_texture_box = AddEntity<TextureBoxEntity>();
    AddComponent<TransformComponent>(first_item_texture_box, glm::vec2{0.f, 0.f});
    AddComponent<TextureComponent>(first_item_texture_box, "InGameUIBackLayer"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_UI_BG));
    AddComponent<OffsetComponent>(first_item_texture_box);
    AddComponent<WidthHeightComponent>(first_item_texture_box, static_cast<float>(winsize.first), winsize.second * 0.2f, true);
    AddComponent<ColorComponent>(first_item_texture_box, Color{ 255, 255, 255, 160 });
    AddComponent<TextureBoxComponent>(first_item_texture_box);

    Engine::GetEngineComponent<GameStateManager>().SetUILayerSize(UI_layer_box_size);

    GameState::Load();
}

void UITestState::Update(double deltaTime)
{
    GameState::Update(deltaTime);
}

void UITestState::Unload()
{
    GameState::Unload();
}
