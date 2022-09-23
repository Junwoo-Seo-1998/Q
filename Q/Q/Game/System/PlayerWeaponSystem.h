/*
	File Name: PlayerWeaponSystem.h
	Project Name: Q
	Author(s):
		Primary: junwoo.seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/System.h"
#include "Engine/GameState.h"

class PlayerWeaponSystem final : public q_engine::System<PlayerWeaponSystem>
{
public:
	PlayerWeaponSystem(q_engine::GameState* state);
	virtual void Load() override;
	virtual void Update(double deltaTime) override;
	virtual void Unload() override;
protected:
	virtual void OnEvent(q_engine::IEvent* eventName) override;
};