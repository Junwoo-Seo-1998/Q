/*
	File Name: EnemyWeaponSystem.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary: Hoseob Jeong
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"
#include "Engine/Types.h"
struct Rect;

class EnemyWeaponSystem final : public q_engine::System<EnemyWeaponSystem>
{

public:
	EnemyWeaponSystem(q_engine::GameState* state) : System<EnemyWeaponSystem>(state) {};
	void Update(double deltaTime) override;
	void OnEvent(q_engine::IEvent* eventName) override;
};


