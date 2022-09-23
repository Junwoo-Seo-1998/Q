/*
	File Name: BossAssassinAttack3State.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossAssassinAttack3State.h"


#include "BossAssassinHelperFunctions.h"
#include "BossAssassinChangePatternState.h"
#include "BossAssassinDyingState.h"
#include "EObjectZDepth.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"
#include "Component/BossAssassinComponent.h"
#include "Component/HealthComponent.h"
#include "Component/BossMagicianComponent.h"
#include "Game/Component/EnemyWeaponComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Entity/WeaponEntity.h"

constexpr float BulletFreq = 0.8f;
constexpr int BulletNum = 6;
constexpr int BulletMAXNum = 10;
constexpr float BulletTorque = PI;
constexpr float bulletSpeed = 1000;

using namespace q_engine;

void BossAssassinAttack3State::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("AssassinWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(1);
	enemyState.SetLookingPlayer(true);
	enemyState.SetMoveByInput(true);
}

void BossAssassinAttack3State::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	bossComp.UpdateTime(deltaTime);

	if (bossComp.GetTime() >= BulletFreq)
	{
		ShootBullets(controlledState, enemyID, BulletTorque * static_cast<float>(deltaTime) * bossComp.GetIndicatorNum());
		bossComp.IncreaseIndicatorNum();
		bossComp.ResetTime();
		if (bossComp.GetIndicatorNum() > BulletMAXNum)
		{
			bossComp.ResetIndicatorNum();
			enemyState.SetCurrentState<BossAssassinChangePatternState>(controlledState);
		}
	}
	BossAssassinMove(controlledState, enemyID, deltaTime);

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<BossAssassinDyingState>(controlledState);
	}
}

void BossAssassinAttack3State::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
}

void BossAssassinAttack3State::ShootBullets(q_engine::GameState* controlledState, q_engine::IDType enemyID, float offset)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 enemyPosition = enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();




	for (int i = 0; i < BulletNum; ++i)
	{
		float angle = ((PI * 2) / static_cast<float>(BulletNum)) * static_cast<float>(i) + offset;
		glm::vec2 bulletVelocity = glm::vec2(cos(angle), sin(angle)) * bulletSpeed;

		auto bullet = controlledState->AddEntity<EnemyWeaponEntity>();
		controlledState->AddComponent<TransformComponent>(bullet, enemyPosition.x, enemyBox.CenterY(), 0.f, 1.f, 1.f);
		controlledState->AddComponent<TextureComponent>(bullet, "PyoChang"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		controlledState->AddComponent<VelocityComponent>(bullet, bulletVelocity);
		controlledState->AddComponent<EnemyWeaponComponent>(bullet, enemyID, 10, true, false);
		controlledState->AddComponent<WidthHeightComponent>(bullet, 25.f, 25.f);
		controlledState->AddComponent<RectCollisionComponent>(bullet, glm::vec2{ -100.f,200.f }, 11.f, 11.f, false, true);
		controlledState->AddComponent<OffsetComponent>(bullet, glm::vec2{ 28.f ,0.f });

		controlledState->GetComponents<q_engine::VelocityComponent>()[bullet->GetEntityID()].SetAngularVelocity(PI * 3);
	}
}
