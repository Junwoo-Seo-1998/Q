/*
	File Name: WarriorAttackingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "WarriorAttackingState.h"

#include "../EnemyStateHelper.h"
#include "WarriorChasingState.h"
#include "WarriorDyingState.h"
#include "WarriorHurtingState.h"
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
#include "Component/HealthComponent.h"
#include "Game/Component/EnemyWeaponComponent.h"


std::string IEnemyState<WarriorAttackingState>::StateName = "WarriorAttackingState";
using namespace q_engine;

void WarriorAttackingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WarriorAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.);
	enemyState.SetLookingPlayer(true);
	auto& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(true);
}

void WarriorAttackingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
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
	else if (enemyHP.IsDamaged())
	{
		enemyState.SetCurrentState<WarriorHurtingState>(controlledState);
	}
}

void WarriorAttackingState::Unload(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);
}

void WarriorAttackingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void WarriorAttackingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

