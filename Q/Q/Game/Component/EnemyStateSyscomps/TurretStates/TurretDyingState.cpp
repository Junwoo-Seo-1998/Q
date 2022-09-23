/*
	File Name: TurretDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TurretDyingState.h"

#include "Engine/Engine.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Component/AnimationComponent.h"

using namespace q_engine;

void TurretDyingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("BigBoyDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(false);


	constexpr float slowMotionChance = 1.f;
	if (Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, 1.f) < slowMotionChance)
	{
		Engine::ApplySlowMotion(0.5f, 0.2f);
	}
}


void TurretDyingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();
	}
}

void TurretDyingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
}

void TurretDyingState::OnRayEvent([[maybe_unused]] q_engine::GameState*, [[maybe_unused]] q_engine::IDType, [[maybe_unused]] q_engine::RayTriggerEvent*)
{
}

void TurretDyingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
