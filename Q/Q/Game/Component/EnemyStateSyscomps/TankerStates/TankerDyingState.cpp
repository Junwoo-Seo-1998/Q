/*
	File Name: TankerDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TankerDyingState.h"

#include "TankerDefendingState.h"

#include "TankerSearchingState.h"
#include "TankerAttackingState.h"
#include "../EnemyStateHelper.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"
#include "Component/EnemyWeaponComponent.h"

using namespace q_engine;

void TankerDyingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("TankerDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0);
	enemyState.SetLookingPlayer(true);


	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);

	constexpr float slowMotionChance = 0.5f;
	if (Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, 1.f) < slowMotionChance)
	{
		Engine::ApplySlowMotion(0.5f, 0.2f);
	}
}

void TankerDyingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();
	}
}

void TankerDyingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{

}

void TankerDyingState::OnRayEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::RayTriggerEvent* triggerEvent)
{

}

void TankerDyingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}
