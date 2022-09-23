/*
	File Name: FlameThrowerAttackingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FlameThrowerAttackingState.h"

#include "../EnemyStateHelper.h"
#include "FlameThrowerChasingState.h"
#include "FlameThrowerDyingState.h"
#include "FlameThrowerHurtingState.h"
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


std::string IEnemyState<FlameThrowerAttackingState>::StateName = "WarriorAttackingState";
using namespace q_engine;

void FlameThrowerAttackingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.);
	enemyState.SetLookingPlayer(true);
	auto& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];

	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsAnimationID("FlamethrowerBotAttackingAnim"_hashID))
	{
		animation.SetAnimation("FlamethrowerBotPrepAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		enemyWeapon.SetAttacking(false);
	}
	else 
	{
		enemyWeapon.SetAttacking(true);
	}




	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(1.f);
	timer.SetOnTimeOut([enemyID, controlledState]()
		{
			if (controlledState->GetComponents<EnemyStateComponent>().isExist(enemyID))
			{
				auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
				if (enemyState.GetCurrentStateID() == FlameThrowerAttackingState::GetStateID())
				{
					auto& enemyPos = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
					auto enemyWidthHeight = controlledState->GetComponents<q_engine::WidthHeightComponent>()[enemyID].GetWidthHeight();
					auto enemyWeaponWidth = controlledState->GetComponents<q_engine::WidthHeightComponent>()[enemyState.GetWeaponID()].GetWidth<float>();

					glm::vec2 playerPos = enemyState.GetLastSeenPlayerPosition();
					glm::vec2 relativePos = playerPos - enemyPos.GetPosition() + 0.5f * enemyWidthHeight;


					if (abs(relativePos.x) > enemyWeaponWidth * 1.5)
					{
						enemyState.SetCurrentState<FlameThrowerChasingState>(controlledState);
					}
					else
					{
						enemyState.SetCurrentState<FlameThrowerAttackingState>(controlledState);
					}
				}

			}
		}
	);
}

void FlameThrowerAttackingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyPos = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 playerPos = enemyState.GetLastSeenPlayerPosition();
	glm::vec2 relativePos = playerPos - enemyPos.GetPosition();

	if (animation.IsAnimationID("FlamethrowerBotPrepAttackingAnim"_hashID))
	{
		if (animation.IsAnimationEnded())
		{
			auto& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
			animation.ChangeAnimation("FlamethrowerBotAttackingAnim"_hashID);
			enemyWeapon.SetAttacking(true);
		}
	}

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
		enemyState.SetCurrentState<FlameThrowerDyingState>(controlledState);
	}
	else if (enemyHP.IsDamaged())
	{
		enemyState.SetCurrentState<FlameThrowerHurtingState>(controlledState);
	}


}

void FlameThrowerAttackingState::Unload(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyWeapon = controlledState->GetComponents<EnemyWeaponComponent>()[enemyState.GetWeaponID()];
	enemyWeapon.SetAttacking(false);
}

void FlameThrowerAttackingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void FlameThrowerAttackingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
	else
	{
		enemyState.SetCurrentState<FlameThrowerChasingState>(controlledState);
	}
}

