/*
	File Name: BossAssassinAttack2State.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Component/EnemyStateSyscomps/IEnemyState.h"

class BossAssassinAttack2State : public IEnemyState<BossAssassinAttack2State>
{
public:
	BossAssassinAttack2State() { StateName = "BossAssassinAttack2State"; }

	static void Load(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime);
	static void Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID);

};
