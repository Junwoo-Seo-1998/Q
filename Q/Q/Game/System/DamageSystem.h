/*
	File Name: DamageSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/ECS/System.h"

class DamageSystem final : public q_engine::System<DamageSystem>
{
public:
	DamageSystem(q_engine::GameState* state) :System<DamageSystem>(state) {}
	void Update(double deltaTime) override;
	void Load() override;
	void OnEvent(q_engine::IEvent* eventName) override;
	void Unload() override;
private:

};