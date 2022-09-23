/*
    File Name: TextureDrawSystem.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/System.h"


namespace q_engine
{
    class TextureDrawSystem final : public System<TextureDrawSystem>
    {
    public:
        TextureDrawSystem() = delete;
        TextureDrawSystem(GameState* state) : System<q_engine::TextureDrawSystem>(state) {}
        void Update(double deltaTime) override;
    };
}
