/*
	File Name: EngineerAttackingState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../IEnemyState.h"
#include "Engine/Types.h"
class EngineerAttackingState : public IEnemyState<EngineerAttackingState>
{
	static void ShootBullet(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	
public:
	static void Load(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double);
	static void Unload(q_engine::GameState*, q_engine::IDType) {}

	static void OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent);
	static void OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* rayEvent);
	static void OnAttackEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID);
};
