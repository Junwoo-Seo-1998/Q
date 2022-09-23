/*
	File Name: BossAssassinAttack4State.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossAssassinAttack4State.h"


#include "BossAssassinHelperFunctions.h"
#include "BossAssassinChangePatternState.h"
#include "BossAssassinDyingState.h"
#include "EObjectZDepth.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"
#include "Component/BossAssassinComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Core/AStarPathFinding.h"
#include "Entity/WeaponEntity.h"
#include "Game/Component/EnemyWeaponComponent.h"

constexpr float BulletFreq = 0.3f;
constexpr int BulletNum = 1;
constexpr int BulletMAXNum = 10;
constexpr float bulletSpeed = 1500;

using namespace q_engine;

void BossAssassinAttack4State::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("AssassinWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(1);
	enemyState.SetLookingPlayer(true);
	enemyState.SetMoveByInput(true);
}

void BossAssassinAttack4State::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	bossComp.UpdateTime(deltaTime);

	if (bossComp.GetTime() >= BulletFreq)
	{
		ShootBullets(controlledState, enemyID);
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

void BossAssassinAttack4State::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* triggerEvent)
{

}

void BossAssassinAttack4State::ShootBullets(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 enemyPosition = enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();




	for (int i = 0; i < BulletNum; ++i)
	{
		float angle = Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, PI);
		glm::vec2 bulletVelocity = glm::vec2(cos(angle), sin(angle)) * bulletSpeed;

		auto bullet = controlledState->AddEntity<EnemyWeaponEntity>();
		controlledState->AddComponent<TransformComponent>(bullet, enemyPosition.x, enemyBox.CenterY(), 0.f, 1.f, 1.f);
		controlledState->AddComponent<TextureComponent>(bullet, "PyoChang"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		controlledState->AddComponent<VelocityComponent>(bullet, bulletVelocity);
		controlledState->AddComponent<EnemyWeaponComponent>(bullet, enemyID, 10, true, false);
		controlledState->AddComponent<WidthHeightComponent>(bullet, 25.f, 25.f);
		controlledState->AddComponent<RectCollisionComponent>(bullet, glm::vec2{ -100.f,200.f }, 11.f, 11.f, false, true);
		controlledState->AddComponent<OffsetComponent>(bullet, glm::vec2{ 28.f ,0.f });

		controlledState->GetComponents<q_engine::VelocityComponent>()[bullet->GetEntityID()].SetAngularVelocity(PI*3);
	}
}
