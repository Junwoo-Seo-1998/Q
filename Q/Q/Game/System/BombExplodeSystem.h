/*
	File Name: BombExplodeSystem.h
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"
#include "Engine/Types.h"

class BombExplodeSystem final : public q_engine::System<BombExplodeSystem>
{
public:
	BombExplodeSystem(q_engine::GameState* state) : System<BombExplodeSystem>(state) {}


	void OnEvent(q_engine::IEvent* event) override;


private:
	q_engine::IDType mCollidedEntityID = q_engine::INVALID_ID;
};
