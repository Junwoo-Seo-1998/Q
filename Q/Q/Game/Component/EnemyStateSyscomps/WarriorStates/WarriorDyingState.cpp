/*
	File Name: WarriorDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "WarriorDyingState.h"

#include "WarriorAttackingState.h"

#include "../EnemyStateHelper.h"
#include "WarriorChasingState.h"
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


std::string IEnemyState<WarriorDyingState>::StateName = "WarriorAttackingState";
using namespace q_engine;

void WarriorDyingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WarriorDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.);
	enemyState.SetLookingPlayer(true);
	
	EnemyWeaponComponent& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);
	
	constexpr float slowMotionChance = 0.3f;
	if (Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, 1.f) < slowMotionChance)
	{
		Engine::ApplySlowMotion(0.3f, 0.2f);
	}


}

void WarriorDyingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (!animation.IsPlaying())
		enemyState.KillThis();
}

void WarriorDyingState::Unload([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}

void WarriorDyingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
}

void WarriorDyingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

