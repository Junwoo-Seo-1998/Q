/*
	File Name: ScoutAttackingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScoutAttackingState.h"

#include "ScoutChasingState.h"
#include "ScoutDyingState.h"
#include "ScoutHurtingState.h"
#include "ScoutStateHelper.h"
#include "../EnemyStateHelper.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Entity/BulletEntity.h"
#include <Game/Component/EnemyWeaponComponent.h>
#include <Game/Entity/WeaponEntity.h>

using namespace q_engine;

void ScoutAttackingState::ShootingBullet(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 enemyPosition = enemyTransform.GetPosition();
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();
	
	float bulletSpeed = 500;
	glm::vec2 bulletVelocity = glm::normalize(relativePosition) * bulletSpeed;

	auto bullet = controlledState->AddEntity<EnemyWeaponEntity>();
	controlledState->AddComponent<TransformComponent>(bullet, enemyPosition.x, enemyBox.CenterY(), 0.f, 2.f, 2.f);
	controlledState->AddComponent<AnimationComponent>(bullet, "BulletAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	controlledState->AddComponent<VelocityComponent>(bullet, bulletVelocity);
	controlledState->AddComponent<WidthHeightComponent>(bullet, 11.f, 11.f);
	controlledState->AddComponent<RectCollisionComponent>(bullet, glm::vec2{ -100.f,200.f }, 11.f, 11.f, false, true);
	controlledState->AddComponent<EnemyWeaponComponent>(bullet, enemyID, 10, true, false);
	controlledState->AddComponent<OffsetComponent>(bullet, glm::vec2{ 28.f ,0.f });
}

void ScoutAttackingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("ScoutAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.5);
	enemyState.SetLookingPlayer(true);
	ShootingBullet(controlledState, enemyID);
}

void ScoutAttackingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<ScoutChasingState>(controlledState);
	}


	ScoutMovementUpdate(controlledState, enemyID);

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<ScoutDyingState>(controlledState);
	}
	if (enemyHP.IsDamaged())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<ScoutHurtingState>(controlledState);
	}
}

void ScoutAttackingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void ScoutAttackingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

void ScoutAttackingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}
