/*
	File Name: BomberDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BomberDyingState.h"


#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Engine.h"

void BomberDyingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WardroidDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(false);
	enemyState.SetMoveByInput(true);
	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);
}


void BomberDyingState::Update(q_engine::GameState * controlledState, q_engine::IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();
	}
}

void BomberDyingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
}

void BomberDyingState::OnRayEvent([[maybe_unused]] q_engine::GameState*, [[maybe_unused]] q_engine::IDType, [[maybe_unused]] q_engine::RayTriggerEvent*)
{
}

void BomberDyingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
