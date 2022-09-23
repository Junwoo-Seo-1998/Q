/*
	File Name: EngineerRunningState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EngineerRunningState.h"



#include "EngineerDyingState.h"
#include "EngineerHurtingState.h"
#include "EngineerSummoningState.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"

void EngineerRunningState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("EngineerWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyPos = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 playerPos = enemyState.GetLastSeenPlayerPosition();
	glm::vec2 relativePos = playerPos - enemyPos.GetPosition();
	
	enemyState.Run();

	if (relativePos.x > 0)
	{
		enemyState.MoveLeft();
	}
	else
	{
		enemyState.MoveRight();
	}
	enemyState.SetLookingPlayer(true);
}

void EngineerRunningState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	
	if (isCliffWall(controlledState, enemyID))
	{
		enemyState.SetCurrentState<EngineerSummoningState>(controlledState);
	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<EngineerDyingState>(controlledState);
	}
	else if (enemyHP.IsDamaged())
	{
		enemyState.SetCurrentState<EngineerHurtingState>(controlledState);
	}
}

void EngineerRunningState::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* eventObj)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, eventObj);
}

void EngineerRunningState::OnRayEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*)
{
}

void EngineerRunningState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
