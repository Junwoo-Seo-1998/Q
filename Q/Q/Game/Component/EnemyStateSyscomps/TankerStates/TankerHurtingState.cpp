/*
	File Name: TankerHurtingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TankerHurtingState.h"

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

void TankerHurtingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("TankerHurtingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0);
	enemyState.SetLookingPlayer(true);

	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);
	
	//auto& enemyHealth = controlledState->GetComponents<HealthComponent>()[enemyID];
	//enemyHealth.SetInvincible(false);
}

void TankerHurtingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<TankerDefendingState>(controlledState);
	}
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	enemyState.UpdateLookingDirection(atan2(relativePosition.y, relativePosition.x));
}

void TankerHurtingState::Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(true);
}

void TankerHurtingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{

}

void TankerHurtingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

void TankerHurtingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}
