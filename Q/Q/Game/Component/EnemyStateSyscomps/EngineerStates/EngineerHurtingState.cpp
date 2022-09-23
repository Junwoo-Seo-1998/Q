/*
	File Name: EngineerHurtingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EngineerHurtingState.h"


#include "EngineerAttackingState.h"
#include "EngineerDyingState.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Event/RayTriggerEvent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/RaycastComponent.h"


void EngineerHurtingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("EngineerHurtingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0);
	enemyState.SetLookingPlayer(true);

}

void EngineerHurtingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<EngineerAttackingState>(controlledState);
	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<EngineerDyingState>(controlledState);
	}
	else if (enemyHP.IsDamaged())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<EngineerHurtingState>(controlledState);
	}
}

void EngineerHurtingState::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void EngineerHurtingState::OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* rayEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[rayEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, rayEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

void EngineerHurtingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
