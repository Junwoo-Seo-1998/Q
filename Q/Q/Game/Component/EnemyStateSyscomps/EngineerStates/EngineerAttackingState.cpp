/*
	File Name: EngineerAttackingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EngineerAttackingState.h"



#include "EngineerDyingState.h"
#include "EngineerRunningState.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/RaycastComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Entity/WeaponEntity.h"
#include "Engine/Event/RayTriggerEvent.h"

using namespace q_engine;

void EngineerAttackingState::ShootBullet(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 enemyPosition = enemyTransform.GetPosition();
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	float bulletSpeed = 500;
	glm::vec2 bulletVelocity = glm::normalize(relativePosition) * bulletSpeed;

	auto bullet = controlledState->AddEntity<EnemyWeaponEntity>();
	controlledState->AddComponent<q_engine::TransformComponent>(bullet, enemyPosition.x, enemyBox.CenterY(), 0.f, 2.f, 2.f);
	controlledState->AddComponent<q_engine::AnimationComponent>(bullet, "BulletAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	controlledState->AddComponent<q_engine::VelocityComponent>(bullet, bulletVelocity);
	controlledState->AddComponent<q_engine::WidthHeightComponent>(bullet, 11.f, 11.f);
	controlledState->AddComponent<q_engine::RectCollisionComponent>(bullet, glm::vec2{ -100.f,200.f }, 11.f, 11.f, false, true);
	controlledState->AddComponent<EnemyWeaponComponent>(bullet, enemyID, 10, true, false);
	controlledState->AddComponent<q_engine::OffsetComponent>(bullet, glm::vec2{ 28.f ,0.f });
}

void EngineerAttackingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("EngineerAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	enemyState.SetSpeedMultiplier(0.f);
	
	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(0.5f);
	timer.SetOnTimeOut([enemyID, controlledState, &enemyState, &timer]()
		{
			if (controlledState->GetComponents<EnemyStateComponent>().isExist(enemyID))
			{
				ShootBullet(controlledState, enemyID);

				timer.ResetAndResume(0.3f);
				timer.SetOnTimeOut([enemyID, controlledState, &enemyState, &timer]()
					{
						if (controlledState->GetComponents<EnemyStateComponent>().isExist(enemyID))
						{
							ShootBullet(controlledState, enemyID);
						}
					}
				);
			}
		}
	);

}

void EngineerAttackingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		if(isCliffWall(controlledState, enemyID))
		{
			enemyState.SetCurrentState<EngineerAttackingState>(controlledState);
		}
		else
		{
			enemyState.SetCurrentState<EngineerRunningState>(controlledState);
		}
	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<EngineerDyingState>(controlledState);
	}
}

void EngineerAttackingState::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void EngineerAttackingState::OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* rayEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[rayEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, rayEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
}

void EngineerAttackingState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
