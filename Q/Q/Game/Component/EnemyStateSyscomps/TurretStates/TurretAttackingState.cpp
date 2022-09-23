/*
	File Name: TurretAttackingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TurretAttackingState.h"


#include "EObjectZDepth.h"
#include "TurretDyingState.h"
#include "TurretMissingState.h"
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/RaycastComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Event/RayTriggerEvent.h"
#include "Entity/WeaponEntity.h"

constexpr float FireRate = 0.5;

using namespace q_engine;

void TurretAttackingState::ShootBullet(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 enemyPosition = enemyTransform.GetPosition();
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	glm::vec2 enemyBox = controlledState->GetComponents<q_engine::WidthHeightComponent>()[enemyID].GetWidthHeight();
	
	float bulletSpeed = 500;
	glm::vec2 bulletVelocity = glm::normalize(relativePosition) * bulletSpeed;

	auto bullet = controlledState->AddEntity<EnemyWeaponEntity>();
	controlledState->AddComponent<q_engine::TransformComponent>(bullet, enemyPosition.x + enemyBox.x / 2, enemyPosition.y + enemyBox.y/4, 0.f, 2.f, 2.f);
	controlledState->AddComponent<q_engine::AnimationComponent>(bullet, "BulletAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	controlledState->AddComponent<q_engine::VelocityComponent>(bullet, bulletVelocity);
	controlledState->AddComponent<q_engine::WidthHeightComponent>(bullet, 11.f, 11.f);
	controlledState->AddComponent<q_engine::RectCollisionComponent>(bullet, glm::vec2{ -100.f,200.f }, 11.f, 11.f, false, true);
	controlledState->AddComponent<EnemyWeaponComponent>(bullet, enemyID, 10, true, false);
	controlledState->AddComponent<q_engine::OffsetComponent>(bullet, glm::vec2{ 28.f ,0.f });
}

void TurretAttackingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("BigBoyAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(true);

}

void TurretAttackingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{

	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if(animation.IsAnimationEnded())
	{
		animation.SetAnimation("BigBoyAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		ShootBullet(controlledState, enemyID);
	}

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyPos = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 playerPos = enemyState.GetLastSeenPlayerPosition();
	glm::vec2 relativePos = playerPos - enemyPos.GetPosition();


	if (relativePos.x > 0)
	{
		enemyState.MoveRight();
	}
	else
	{
		enemyState.MoveLeft();
	}
	
	
	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<TurretDyingState>(controlledState);
	}
}

void TurretAttackingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
}

void TurretAttackingState::OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (!isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<TurretMissingState>(controlledState);
	}
}

void TurretAttackingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
