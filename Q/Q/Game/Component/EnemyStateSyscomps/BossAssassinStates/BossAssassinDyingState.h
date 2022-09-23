/*
	File Name: BossAssassinDyingState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Component/EnemyStateSyscomps/IEnemyState.h"

class BossAssassinDyingState : public IEnemyState<BossAssassinDyingState>
{
private:

	static void ShootBullets(q_engine::GameState* controlledState, q_engine::IDType enemyID, int num, float offset);
public:
	BossAssassinDyingState() { StateName = "BossAssassinDyingState"; }

	static void Load(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime);
	static void Unload([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID) {};
	static void OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent);
};
