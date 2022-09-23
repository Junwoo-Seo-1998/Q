/*
    File Name: TextDrawSystem.h
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
	class TextDrawSystem final : public System<TextDrawSystem>
	{
    public:
        TextDrawSystem() = delete;
        TextDrawSystem(GameState* state) : System<q_engine::TextDrawSystem>(state) {}
        void Update(double deltaTime) override;
	};
}
