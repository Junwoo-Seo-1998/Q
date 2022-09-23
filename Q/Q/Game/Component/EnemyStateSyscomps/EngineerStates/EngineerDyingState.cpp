/*
	File Name: EngineerDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EngineerDyingState.h"


#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Engine.h"
using namespace q_engine;

void EngineerDyingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("EngineerDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(false);
}


void EngineerDyingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();
	}
}

void EngineerDyingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID,
	[[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
}

void EngineerDyingState::OnRayEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*)
{
}

void EngineerDyingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
