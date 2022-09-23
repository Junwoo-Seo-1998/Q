/*
	File Name: PlayerHealthBarUpdateDrawSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>


#include "../../../Engine/Graphics/DrawObject.h"
#include "../Engine/ECS/System.h"

class PlayerHealthBarUpdateDrawSystem final : public q_engine::System<PlayerHealthBarUpdateDrawSystem>
{
public:
	PlayerHealthBarUpdateDrawSystem(q_engine::GameState* state) : System<PlayerHealthBarUpdateDrawSystem>(state) {}

	void Load() override;
	void Update(double deltaTime) override;

	float GetPercentage()
	{
		return mPlayerHealthUIPercentage < 0.1f ? 0.f : mPlayerHealthUIPercentage;
	}
private:
	float mPlayerHealthUIPercentage = 1.f;
};
