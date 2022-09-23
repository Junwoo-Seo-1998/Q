/*
	File Name: FlameThrowerMissingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FlameThrowerMissingState.h"

#include "../EnemyStateHelper.h"
#include "FlameThrowerAttackingState.h"
#include "FlameThrowerChasingState.h"
#include "FlameThrowerDyingState.h"
#include "FlameThrowerHurtingState.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"

using namespace q_engine;

std::string IEnemyState<FlameThrowerMissingState>::StateName = "FlameThrowerMissingState";

void FlameThrowerMissingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("FlamethrowerBotWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.ResetSpeedMultiplier();
	enemyState.SetLookingPlayer(false);
}

void FlameThrowerMissingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isCliffWall(controlledState, enemyID))
		enemyState.ReverseMove();

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<FlameThrowerDyingState>(controlledState);
	}
	else if (enemyHP.IsDamaged())
	{
		enemyState.SetCurrentState<FlameThrowerHurtingState>(controlledState);
	}
}

void FlameThrowerMissingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void FlameThrowerMissingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<FlameThrowerChasingState>(controlledState);
	}
}

void FlameThrowerMissingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<FlameThrowerAttackingState>(controlledState);
}
