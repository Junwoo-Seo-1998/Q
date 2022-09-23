/*
	File Name: BossMagicianAttack4State.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Component/EnemyStateSyscomps/IEnemyState.h"

class BossMagicianAttack4State : public IEnemyState<BossMagicianAttack4State>
{
private:

	static void ShootBullets(q_engine::GameState* controlledState, q_engine::IDType enemyID);
public:
	BossMagicianAttack4State() { StateName = "BossMagicianAttack1State"; }

	static void Load(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime);
	static void Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID) {};

	static void OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent);
};