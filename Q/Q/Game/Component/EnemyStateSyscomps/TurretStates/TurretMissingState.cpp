/*
	File Name: TurretMissingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TurretMissingState.h"


#include "EObjectZDepth.h"
#include "TurretAttackingState.h"
#include "TurretDyingState.h"
#include "Component/EnemyStateComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/RaycastComponent.h"
#include "Engine/Event/RayTriggerEvent.h"

void TurretMissingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("BigBoyWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.ResetSpeedMultiplier();
	enemyState.SetLookingPlayer(false);
}

void TurretMissingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isCliffWall(controlledState, enemyID))
		enemyState.ReverseMove();

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<TurretDyingState>(controlledState);
	}
}

void TurretMissingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* triggerEvent)
{
}

void TurretMissingState::OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<TurretAttackingState>(controlledState);
	}
}

void TurretMissingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
