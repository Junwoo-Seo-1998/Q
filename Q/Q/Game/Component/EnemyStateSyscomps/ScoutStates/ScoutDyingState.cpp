/*
	File Name: ScoutDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScoutDyingState.h"

#include "ScoutAttackingState.h"

#include "ScoutChasingState.h"
#include "../EnemyStateHelper.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"

using namespace q_engine;

void ScoutDyingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("ScoutDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(false);


	constexpr float slowMotionChance = 0.2f;
	if (Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, 1.f) < slowMotionChance)
	{
		Engine::ApplySlowMotion(0.3f, 0.2f);
	}
}


void ScoutDyingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();
	}
}

void ScoutDyingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* triggerEvent)
{

}

void ScoutDyingState::OnRayEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::RayTriggerEvent* triggerEvent)
{

}

void ScoutDyingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}
