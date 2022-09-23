/*
	File Name: TankerDefendingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TankerDefendingState.h"

#include "TankerSearchingState.h"
#include "TankerAttackingState.h"
#include "TankerDyingState.h"
#include "TankerHurtingState.h"
#include "../EnemyStateHelper.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"

using namespace q_engine;

void TankerDefendingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("TankerDefendingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0);
	enemyState.SetLookingPlayer(true);

	auto& enemyHealth = controlledState->GetComponents<HealthComponent>()[enemyID];
	enemyHealth.SetInvincible(bool(true));
}

void TankerDefendingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	enemyState.UpdateLookingDirection(atan2(relativePosition.y, relativePosition.x));

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];

	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<TankerDyingState>(controlledState);
	}
}

void TankerDefendingState::Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyHealth = controlledState->GetComponents<HealthComponent>()[enemyID];
	enemyHealth.SetInvincible(bool(false));
}

void TankerDefendingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
	
}

void TankerDefendingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
	else
	{
		enemyState.SetCurrentState<TankerSearchingState>(controlledState);
	}
}

void TankerDefendingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<TankerAttackingState>(controlledState);
}
