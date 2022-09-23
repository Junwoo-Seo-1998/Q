/*
    File Name: TextureBoxUpdateSystem.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"

namespace q_engine
{
    class TextureBoxUpdateSystem final : public q_engine::System<TextureBoxUpdateSystem>
    {
    public:
        TextureBoxUpdateSystem() = delete;
        TextureBoxUpdateSystem(GameState* state) : System<TextureBoxUpdateSystem>(state) {}

        void Update(double deltaTime) override;
    };
}
