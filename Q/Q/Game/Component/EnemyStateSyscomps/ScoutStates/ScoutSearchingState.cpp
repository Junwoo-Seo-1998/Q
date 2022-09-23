/*
	File Name: ScoutSearchingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScoutSearchingState.h"

#include "ScoutMissingState.h"
#include "ScoutChasingState.h"
#include "ScoutDyingState.h"
#include "ScoutHurtingState.h"
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

void ScoutSearchingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("ScoutChasingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.ResetSpeedMultiplier();
	enemyState.SetLookingPlayer(true);

	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(3.f);
	timer.SetOnTimeOut([enemyID, controlledState]()
		{
			if (controlledState->GetComponents<EnemyStateComponent>().isExist(enemyID))
			{
				auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
				if (enemyState.GetCurrentStateID() == ScoutSearchingState::GetStateID())
					enemyState.SetCurrentState<ScoutMissingState>(controlledState);
			}
		}
	);
}

void ScoutSearchingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	if (abs(relativePosition).x < abs(enemyBox.Size().x) / 2 && abs(relativePosition).y < abs(enemyBox.Size().y) / 2)
		enemyState.SetCurrentState<ScoutMissingState>(controlledState);
	else if (relativePosition.x > abs(enemyBox.Size().x))
		enemyState.MoveRight();
	else if (relativePosition.x < -abs(enemyBox.Size().x))
		enemyState.MoveLeft();

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<ScoutDyingState>(controlledState);
	}
	else if (enemyHP.IsDamaged())
	{
		enemyState.SetCurrentState<ScoutHurtingState>(controlledState);
	}
}

void ScoutSearchingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void ScoutSearchingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<ScoutChasingState>(controlledState);
	}
}

void ScoutSearchingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
}
