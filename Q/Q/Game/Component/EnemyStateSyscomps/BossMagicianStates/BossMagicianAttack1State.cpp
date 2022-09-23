/*
	File Name: BossMagicianAttack1State.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossMagicianAttack1State.h"



#include "BossHelperFunctions.h"
#include "BossMagicianChangePatternState.h"
#include "BossMagicianDyingState.h"
#include "EObjectZDepth.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"
#include "Component/BossMagicianComponent.h"
#include "Component/NewWeaponComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Core/AStarPathFinding.h"
#include "Engine/Graphics/Particle.h"
#include "Entity/BombEntity.h"
#include "Entity/BulletEntity.h"
#include "Entity/WeaponEntity.h"
#include "Game/Component/EnemyWeaponComponent.h"

constexpr float BulletFreq = 0.3f;
constexpr int BulletNum = 2;
constexpr int BulletMAXNum = 5;



using namespace q_engine;

void BossMagicianAttack1State::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("ScoutAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.5);
	enemyState.SetLookingPlayer(true);

	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossMagicianComponent>()[enemyID];

	auto playerID = bossComp.GetPlayerID();
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[playerID];

	q_engine::Engine::GetEngineComponent<q_engine::AStarPathFinding>().UpdatePath(reinterpret_cast<q_engine::Level*>(controlledState)
		, bossComp.GetPathToPlayer(), enemyTransform.GetPosition(), playerTransform.GetPosition());
	
	//q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	//timer.ResetAndResume(10.f);
	//timer.SetOnTimeOut([&enemyState, controlledState]()
	//	{
	//		enemyState.SetCurrentState<BossMagicianChangePatternState>(controlledState);
	//	});
}

void BossMagicianAttack1State::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossMagicianComponent>()[enemyID];
	bossComp.UpdateTime(deltaTime);

	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if(animation.IsAnimationID("ScoutHurtingAnim"_hashID) && animation.IsAnimationEnded())
	{
		animation.SetAnimation("ScoutAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	}

	if (bossComp.GetTime() >= BulletFreq)
	{
		ShootBullets(controlledState, enemyID, BulletNum * bossComp.GetIndicatorNum(), bossComp.GetTime() * 20.f);
		bossComp.IncreaseIndicatorNum();
		bossComp.ResetTime();
		if(bossComp.GetIndicatorNum() > BulletMAXNum)
		{
			bossComp.ResetIndicatorNum();
			enemyState.SetCurrentState<BossMagicianChangePatternState>(controlledState);
		}
	}
	BoseMagicianMove(controlledState, enemyID, deltaTime);

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<BossMagicianDyingState>(controlledState);
	}
}

void BossMagicianAttack1State::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BoseMagicianBasicTriggerEvent(controlledState, enemyID, triggerEvent);
}

void BossMagicianAttack1State::ShootBullets(q_engine::GameState* controlledState, q_engine::IDType enemyID, int num, float offset)
{
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	glm::vec2 enemyPosition = enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	float bulletSpeed = 500;

	
	for (int i = 0; i < num; ++i)
	{
		float angle = ((PI*2) / static_cast<float>(num)) * static_cast<float>(i) + offset;
		glm::vec2 bulletVelocity = glm::vec2(cos(angle), sin(angle)) * bulletSpeed;

		auto bullet = controlledState->AddEntity<EnemyWeaponEntity>();
		controlledState->AddComponent<TransformComponent>(bullet, enemyPosition.x, enemyBox.CenterY(), 0.f, 2.f, 2.f);
		controlledState->AddComponent<AnimationComponent>(bullet, "BulletAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		controlledState->AddComponent<VelocityComponent>(bullet, bulletVelocity);
		controlledState->AddComponent<EnemyWeaponComponent>(bullet, enemyID, 10, true, false); 
		controlledState->AddComponent<WidthHeightComponent>(bullet, 11.f, 11.f);
		controlledState->AddComponent<RectCollisionComponent>(bullet, glm::vec2{ -100.f,200.f }, 11.f, 11.f, false, true);
		controlledState->AddComponent<OffsetComponent>(bullet, glm::vec2{ 28.f ,0.f });
	}
}
