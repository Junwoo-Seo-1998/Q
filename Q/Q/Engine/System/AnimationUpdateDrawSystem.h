/*
    File Name: AnimationUpdateDrawSystem.h
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
	class AnimationUpdateDrawSystem final : public System<AnimationUpdateDrawSystem>
	{
    public:
        AnimationUpdateDrawSystem() = delete;
        AnimationUpdateDrawSystem(GameState* state) : System<q_engine::AnimationUpdateDrawSystem>(state) {}
        void Update(double deltaTime) override;
	};
}
