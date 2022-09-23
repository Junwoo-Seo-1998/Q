/*
	File Name: CamLockUpdateSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../Engine/Engine.h"
#include "../../Engine/Entity/CameraEntity.h"
#include "../../Engine/GameStateManager.h"
#include "../../Engine/Graphics/Window.h"
#include "../Engine/ECS/System.h"

//this update system must be added after following system
class CamLockUpdateSystem final : public q_engine::System<CamLockUpdateSystem>
{
public:
	CamLockUpdateSystem(q_engine::GameState* state) : System<CamLockUpdateSystem>(state) {}

	void Update(double /*deltaTime*/) override;
};
