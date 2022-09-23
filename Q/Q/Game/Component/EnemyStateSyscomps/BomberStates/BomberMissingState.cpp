/*
	File Name: BomberMissingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BomberMissingState.h"



#include "BomberChasingState.h"
#include "BomberDyingState.h"
#include "BomberExplodingState.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Event/RayTriggerEvent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/RaycastComponent.h"

void BomberMissingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WardroidWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.ResetSpeedMultiplier();
	enemyState.SetLookingPlayer(false);
}

void BomberMissingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();
	EnemyStateComponent& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if(isCliffWall(controlledState, enemyID))
	{
		enemyState.ReverseMove();
	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<BomberDyingState>(controlledState);
	}
}

void BomberMissingState::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void BomberMissingState::OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<BomberChasingState>(controlledState);
	}
}

void BomberMissingState::OnAttackEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<BomberExplodingState>(controlledState);
}
