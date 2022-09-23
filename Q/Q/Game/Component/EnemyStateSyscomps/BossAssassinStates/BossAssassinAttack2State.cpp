/*
	File Name: BossAssassinAttack2State.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossAssassinAttack2State.h"


#include "BossAssassinChangePatternState.h"
#include "BossAssassinDyingState.h"
#include "EObjectZDepth.h"
#include "Component/BossAssassinComponent.h"
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Engine.h"
#include "Entity/WeaponEntity.h"


void BossAssassinAttack2State::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;

	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("AssassinTelePortAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	q_engine::OffsetComponent& offset = controlledState->GetComponents<q_engine::OffsetComponent>()[enemyID];
	offset.SetHotSpotPosition({ 100,0 });

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(true);
	enemyState.SetMoveByInput(true);
	
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	bossComp.ResetTime();
	bossComp.ResetIndicatorNum();
}

void BossAssassinAttack2State::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	auto& enemyPosition = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& playerPosition = controlledState->GetComponents<q_engine::TransformComponent>()[bossComp.GetPlayerID()];

	bossComp.UpdateTime(deltaTime);

	if (bossComp.GetTime() > 1.7f)
	{
		enemyState.SetCurrentState<BossAssassinChangePatternState>(controlledState);
	}
	else if (bossComp.GetTime() > 1.2f)
	{
		std::lock_guard<std::mutex> lock(q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().GetMutex());
		controlledState->DeleteEntityComponentsLater(q_engine::GetTypeID<EnemyWeaponEntity>(), bossComp.GetWeaponID());
		bossComp.SetShooting(false);
	}
	else if (bossComp.GetTime() > 1.0f)
	{
		if (!bossComp.IsShooting())
		{
			auto enemyWeapon = controlledState->AddEntity<EnemyWeaponEntity>();

			controlledState->AddComponent<q_engine::TransformComponent>(enemyWeapon, enemyPosition.GetPosition());
			controlledState->AddComponent<q_engine::WidthHeightComponent>(enemyWeapon, 100.f, 50.f);
			controlledState->AddComponent<q_engine::RectCollisionComponent>(enemyWeapon, enemyPosition.GetPosition(), 100.f, 50.f, false, true);
			controlledState->AddComponent<EnemyWeaponComponent>(enemyWeapon, enemyID, glm::vec2{ -20,0 }, 10);
			controlledState->AddComponent<q_engine::OffsetComponent>(enemyWeapon);

			bossComp.SetWeaponID(enemyWeapon->GetEntityID());
			bossComp.SetShooting(true);

			controlledState->GetComponents<EnemyWeaponComponent>()[enemyWeapon->GetEntityID()].SetAttacking(true);
		}

	}
	else if (bossComp.GetTime() > 0.5f)
	{
		if (bossComp.GetIndicatorNum() == 0)
		{
			bossComp.IncreaseIndicatorNum();
			enemyPosition.SetPosition(playerPosition.GetPosition());
		}

	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<BossAssassinDyingState>(controlledState);
	}
}

void BossAssassinAttack2State::Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	q_engine::OffsetComponent& offset = controlledState->GetComponents<q_engine::OffsetComponent>()[enemyID];
	offset.SetHotSpotPosition({ 0,0 });
}
