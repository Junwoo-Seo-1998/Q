/*
	File Name: TurretSummoningState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../IEnemyState.h"
#include "Engine/Types.h"

class TurretSummoningState : public IEnemyState<TurretSummoningState>
{
public:
	static void Load(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double);
	static void Unload(q_engine::GameState*, q_engine::IDType) {}
};
