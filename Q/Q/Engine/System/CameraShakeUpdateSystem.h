/*
    File Name: CameraShakeUpdateSystem.h
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
	class CameraShakeUpdateSystem final : public System<CameraShakeUpdateSystem>
	{
	public:
        CameraShakeUpdateSystem() = delete;
        CameraShakeUpdateSystem(GameState* state) : System<q_engine::CameraShakeUpdateSystem>(state) {}
		void Update(double deltaTime) override;
	};
}
