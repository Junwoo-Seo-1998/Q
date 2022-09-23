/*
    File Name: TextureBoxUpdateSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TextureBoxUpdateSystem.h"

#include "../../Engine/Component/TextureComponent.h"
#include "../../Engine/Component/TransformComponent.h"
#include "../../Engine/Component/WidthHeightComponent.h"
#include "../Component/OffsetComponent.h"
#include "../Component/TextureBoxComponent.h"
#include "../Engine/GameState.h"

namespace q_engine
{
    void TextureBoxUpdateSystem::Update(double /*deltaTime*/)
    {
        using namespace q_engine;

        auto& size_components = m_controlled_state->GetComponents<WidthHeightComponent>();
        auto& transform_components = m_controlled_state->GetComponents<TransformComponent>();
        auto& texture_components = m_controlled_state->GetComponents<TextureComponent>();
        auto& offset_components = m_controlled_state->GetComponents<OffsetComponent>();

        auto& texture_boxes = m_controlled_state->GetComponents<TextureBoxComponent>();

        for (auto& texture_box : texture_boxes)
        {
            const IDType entityID = texture_box.GetEntityID();

            const glm::vec2 scale = texture_box.GetScale();
            const glm::vec2 offset = texture_box.GetOffset();

            const TextureComponent& my_texture = *texture_components.find(entityID);
            const WidthHeightComponent& target_size = *size_components.find(entityID);

            const glm::vec2 current_texture_size = glm::vec2{ my_texture.GetWidth(), my_texture.GetHeight() };

            const glm::vec2 texture_scale_factor = target_size.GetWidthHeight() / current_texture_size * scale;

            transform_components.find(entityID)->SetScale(texture_scale_factor);
            offset_components.find(entityID)->SetOffsetPosition(offset * texture_scale_factor);
        }
    }

}
