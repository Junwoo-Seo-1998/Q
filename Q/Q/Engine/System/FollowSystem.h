/*
    File Name: FollowSystem.h
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
	class FollowSystem final : public System<FollowSystem>
	{
	public:
        FollowSystem() = delete;
        FollowSystem(GameState* state) : System<q_engine::FollowSystem>(state) {}
		void Update(double deltaTime) override;
	};
}
