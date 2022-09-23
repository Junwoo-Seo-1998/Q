/*
	File Name: FlameThrowerSearchingState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../IEnemyState.h"
#include "Engine/Types.h"
class FlameThrowerSearchingState : public IEnemyState<FlameThrowerSearchingState>
{
public:
	FlameThrowerSearchingState() { StateName = "FlameThrowerSearchingState"; }
	
	static void Load(q_engine::GameState* controlledState, q_engine::IDType enemyID);
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime);
	static void Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID);

	static void OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* eventObj);
	static void OnRayEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*);
	static void OnAttackEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID);
};
