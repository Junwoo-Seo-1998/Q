/*
    File Name: UIItemComponent.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>

#include "../Engine/AssetManager.h"

#include "../../../Engine/Core/StringHash.h"
#include "../../../Engine/ECS/Component.h"
#include "../../../Engine/Graphics/Texture.h"
#include "../Engine/Core/StringHash.h"

class UIItemTextureComponent final : public q_engine::Component<UIItemTextureComponent>
{
    friend class PlayerHealthBarUpdateDrawSystem;
    friend class PlayerWeaponUIUpdateDrawSystem;
    friend class PlayeritemUIUpdateDrawSystem;
public:
    UIItemTextureComponent()
    {
        mTexIcons[""] = q_engine::get_hash_id("Empty");
    }
    void RegisterTexture(std::string name, q_engine::IDType texID)
    {
        mTexIcons[name] = texID;
    }

private:
    q_engine::IDType GetTexture(std::string name)
    {
        return mTexIcons[name];
    };
    std::map<std::string, q_engine::IDType> mTexIcons;
};