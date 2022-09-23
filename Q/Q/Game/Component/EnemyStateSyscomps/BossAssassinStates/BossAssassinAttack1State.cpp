/*
	File Name: BossAssassinAttack1State.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossAssassinAttack1State.h"


#include "BossAssassinChangePatternState.h"
#include "BossAssassinDyingState.h"
#include "BossAssassinHelperFunctions.h"

#include "EObjectZDepth.h"
#include "Component/BossAssassinComponent.h"
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Engine.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Entity/WeaponEntity.h"


void BossAssassinAttack1State::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("AssassinAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(true);
	enemyState.SetMoveByInput(true);
	
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	bossComp.ResetTime();
	bossComp.ResetIndicatorNum();
}

void BossAssassinAttack1State::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	auto& enemyPosition = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	
	bossComp.UpdateTime(deltaTime);
	
	if (bossComp.GetTime() > 1.6f)
	{
		enemyState.SetCurrentState<BossAssassinChangePatternState>(controlledState);
	}
	else if (bossComp.GetTime() > 1.3f)
	{
		std::lock_guard<std::mutex> lock(q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().GetMutex());
		controlledState->DeleteEntityComponentsLater(q_engine::GetTypeID<EnemyWeaponEntity>(), bossComp.GetWeaponID());
		bossComp.SetShooting(false);
	}
	else if (bossComp.GetTime() > 1.1f)
	{
		if (!bossComp.IsShooting())
		{
			auto enemyWeapon = controlledState->AddEntity<EnemyWeaponEntity>();

			controlledState->AddComponent<q_engine::TransformComponent>(enemyWeapon, enemyPosition.GetPosition());
			controlledState->AddComponent<q_engine::WidthHeightComponent>(enemyWeapon, 80.f, 100.f);
			controlledState->AddComponent<q_engine::RectCollisionComponent>(enemyWeapon, enemyPosition.GetPosition(), 80.f, 100.f, false, true);
			controlledState->AddComponent<EnemyWeaponComponent>(enemyWeapon, enemyID, glm::vec2{ 20,20 }, 10);
			controlledState->AddComponent<q_engine::OffsetComponent>(enemyWeapon);

			bossComp.SetWeaponID(enemyWeapon->GetEntityID());
			bossComp.SetShooting(true);

			controlledState->GetComponents<EnemyWeaponComponent>()[enemyWeapon->GetEntityID()].SetAttacking(true);
		}
	}
	else if (bossComp.GetTime() > 1.0f)
	{
		std::lock_guard<std::mutex> lock(q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().GetMutex());
		controlledState->DeleteEntityComponentsLater(q_engine::GetTypeID<EnemyWeaponEntity>(), bossComp.GetWeaponID());
		bossComp.SetShooting(false);
	}
	else if(bossComp.GetTime() > 0.8f)
	{
		if(!bossComp.IsShooting())
		{
			auto enemyWeapon = controlledState->AddEntity<EnemyWeaponEntity>();

			controlledState->AddComponent<q_engine::TransformComponent>(enemyWeapon, enemyPosition.GetPosition());
			controlledState->AddComponent<q_engine::WidthHeightComponent>(enemyWeapon, 80.f, 100.f);
			controlledState->AddComponent<q_engine::RectCollisionComponent>(enemyWeapon, enemyPosition.GetPosition(), 80.f, 100.f, false, true);
			controlledState->AddComponent<EnemyWeaponComponent>(enemyWeapon, enemyID, glm::vec2{ 20,20 }, 10);
			controlledState->AddComponent<q_engine::OffsetComponent>(enemyWeapon);

			bossComp.SetWeaponID(enemyWeapon->GetEntityID());
			bossComp.SetShooting(true);
			
			controlledState->GetComponents<EnemyWeaponComponent>()[enemyWeapon->GetEntityID()].SetAttacking(true);
		}
	}
	else if(bossComp.GetTime() > 0.4f)
	{
		auto& playerPosition = controlledState->GetComponents<q_engine::TransformComponent>()[bossComp.GetPlayerID()];
		auto& playerVelocity = controlledState->GetComponents<q_engine::VelocityComponent>()[bossComp.GetPlayerID()];
		
		if(bossComp.GetIndicatorNum() == 0)
		{
			bossComp.IncreaseIndicatorNum();
			if(playerVelocity.GetVelocityX() > 0)
			{
				enemyState.MoveRight();
				enemyPosition.SetPosition(playerPosition.GetPosition() - glm::vec2{ 60, 0 });
			}
			else
			{
				enemyState.MoveLeft();
				enemyPosition.SetPosition(playerPosition.GetPosition() + glm::vec2{ 60, 0 });
			}
		}

	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<BossAssassinDyingState>(controlledState);
	}
}

void BossAssassinAttack1State::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* triggerEvent)
{
}
