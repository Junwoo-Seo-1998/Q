/*
	File Name: WarriorHurtingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "WarriorHurtingState.h"

#include "WarriorAttackingState.h"

#include "../EnemyStateHelper.h"
#include "WarriorChasingState.h"
#include "WarriorDyingState.h"
#include "../../../../Engine/GameState.h"
#include "../../EnemyStateComponent.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/CollisionComponent.h"
#include "../../../../Engine/Component/GravityComponent.h"
#include "../../../../Engine/Component/ObjectStateComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Component/TransformComponent.h"
#include "../../../../Engine/Component/VelocityComponent.h"
#include "../../../../Engine/Core/Rect.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../Entity/PhysicsTestEntities.h"
#include "../../../../Engine/Core/StringHash.h"
#include "../../../EObjectZDepth.h"
#include "Component/EnemyWeaponComponent.h"


std::string IEnemyState<WarriorHurtingState>::StateName = "WarriorAttackingState";
using namespace q_engine;

void WarriorHurtingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WarriorHurtingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.);
	enemyState.SetLookingPlayer(true);

	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);
}

void WarriorHurtingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (!animation.IsPlaying())
		enemyState.SetCurrentState<WarriorChasingState>(controlledState);

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<WarriorDyingState>(controlledState);
	}
}

void WarriorHurtingState::Unload(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(true);
}

void WarriorHurtingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* triggerEvent)
{
}

void WarriorHurtingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

