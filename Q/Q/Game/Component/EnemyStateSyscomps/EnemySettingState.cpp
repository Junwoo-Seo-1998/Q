/*
	File Name: EnemySettingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnemySettingState.h"
#include "WarriorStates/WarriorMissingState.h"
#include "TankerStates/TankerMissingState.h"
#include "ScoutStates/ScoutMissingState.h"
#include "../EnemyStateComponent.h"
#include "BomberStates/BomberMissingState.h"
#include "BossAssassinStates/BossAssassinChangePatternState.h"
#include "BossMagicianStates/BossMagicianChangePatternState.h"
#include "EngineerStates/EngineerMissingState.h"
#include "FlameThrowerStates/FlameThrowerMissingState.h"
#include "TurretStates/TurretMissingState.h"
#include "TurretStates/TurretSummoningState.h"


void EnemySettingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	switch (enemyState.GetEnemyType())
	{
	case EnemyStateComponent::EnemyType::Warrior:
		enemyState.SetCurrentState<WarriorMissingState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::Tanker:
		enemyState.SetCurrentState<TankerMissingState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::Scout:
		enemyState.SetCurrentState<ScoutMissingState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::Engineer:
		enemyState.SetCurrentState<EngineerMissingState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::Turret:
		enemyState.SetCurrentState<TurretSummoningState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::Bomber:
		enemyState.SetCurrentState<BomberMissingState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::FlameThrower:
		enemyState.SetCurrentState<FlameThrowerMissingState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::BossMagician:
		enemyState.SetCurrentState<BossMagicianChangePatternState>(controlledState);
		break;
	case EnemyStateComponent::EnemyType::BossAssassin:
		enemyState.SetCurrentState<BossAssassinChangePatternState>(controlledState);
		break;
	}
	
}
