/*
	File Name: ScoutChasingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScoutChasingState.h"

#include "ScoutMissingState.h"
#include "ScoutSearchingState.h"
#include "ScoutAttackingState.h"
#include "ScoutDyingState.h"
#include "ScoutHurtingState.h"
#include "../EnemyStateHelper.h"
#include "ScoutStateHelper.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"

using namespace q_engine;

void ScoutChasingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("ScoutChasingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.Run();
	enemyState.SetLookingPlayer(true);

	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(1.f);
	timer.SetOnTimeOut([enemyID, controlledState]()
		{
			if(controlledState->GetEntities<EnemyEntity>().isExist(enemyID))
			{
				auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
				if (enemyState.GetCurrentStateID() == ScoutChasingState::GetStateID())
					enemyState.SetCurrentState<ScoutAttackingState>(controlledState);
			}
		}
	);
	
}

void ScoutChasingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	ScoutMovementUpdate(controlledState, enemyID);

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<ScoutDyingState>(controlledState);
	}
	if (enemyHP.IsDamaged())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<ScoutHurtingState>(controlledState);
	}
}

void ScoutChasingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void ScoutChasingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
	else
	{
		enemyState.SetCurrentState<ScoutSearchingState>(controlledState);
	}
}

void ScoutChasingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}
