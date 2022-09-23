/*
	File Name: EnemySettingState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "IEnemyState.h"
#include "EnemyStateHelper.h"
#include "Engine/Types.h"
class EnemySettingState : public IEnemyState<EnemySettingState>
{
public:
	EnemySettingState([[maybe_unused]] q_engine::GameState* controlledState) { StateName = "EnemyNullState"; }
	
	static void Load(q_engine::GameState* , q_engine::IDType ) {}
	static void Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double);
	static void Unload(q_engine::GameState* , q_engine::IDType ) {}

	static void OnTriggerEvent(q_engine::GameState*, q_engine::IDType, q_engine::TriggerEvent*) {}
	static void OnRayEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*) {}
	static void OnAttackEvent(q_engine::GameState*, q_engine::IDType) {}
};