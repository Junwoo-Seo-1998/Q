/*
    File Name: TextureBoxComponent.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>
#include "../ECS/Component.h"
namespace q_engine
{
    //kind of tag component
    class TextureBoxComponent : public Component<TextureBoxComponent>
    {
    public:
        TextureBoxComponent() = default;


        [[nodiscard]] glm::vec2 GetOffset() const { return m_offset; }
        [[nodiscard]] glm::vec2 GetScale() const { return m_scale; }
        void SetOffset(glm::vec2 new_offset) { m_offset = new_offset; }
        void SetScale(glm::vec2 new_scale ) { m_scale = new_scale; }

    private:
        glm::vec2 m_offset = {};
        glm::vec2 m_scale = {1.f, 1.f}; 
    };
}
