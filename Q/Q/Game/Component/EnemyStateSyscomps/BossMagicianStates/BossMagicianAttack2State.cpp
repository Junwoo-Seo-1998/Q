/*
	File Name: BossMagicianAttack2State.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossMagicianAttack2State.h"


#include "BossMagicianChangePatternState.h"
#include "EObjectZDepth.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"
#include "Component/BossMagicianComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Component/DragAccelComponent.h"
#include "Entity/BulletEntity.h"
#include "Entity/WeaponEntity.h"
#include "BossHelperFunctions.h"
#include "BossMagicianDyingState.h"
#include "Component/FXAnimLuaScriptsComponent.h"
#include "Component/NewWeaponComponent.h"
#include "Engine/Core/AStarPathFinding.h"
#include "Engine/Event/LambdaEvent.h"
#include "Engine/Graphics/Particle.h"
#include "Entity/BombEntity.h"

constexpr float SpawnFreq = 2.f;
constexpr int SpawnNum = 2;

using namespace q_engine;

void BossMagicianAttack2State::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
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
	
	//TimerManagerSystem::GetTimer(enemyState.GetTimerID()).ResetAndResume(10.f);
	//TimerManagerSystem::GetTimer(enemyState.GetTimerID()).SetOnTimeOut([&enemyState, controlledState]()
	//	{
	//		enemyState.SetCurrentState<BossMagicianChangePatternState>(controlledState);
	//	});

}

void BossMagicianAttack2State::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (animation.IsAnimationID("ScoutHurtingAnim"_hashID) && animation.IsAnimationEnded())
	{
		animation.SetAnimation("ScoutAttackingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	}
	
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossMagicianComponent>()[enemyID];
	bossComp.UpdateTime(deltaTime);

	if (bossComp.GetTime() >= SpawnFreq)
	{
		SpawnWarriors(controlledState, enemyID);
		bossComp.IncreaseIndicatorNum();
		bossComp.ResetTime();
		if(bossComp.GetIndicatorNum() > SpawnNum)
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

void BossMagicianAttack2State::OnTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BoseMagicianBasicTriggerEvent(controlledState, enemyID, triggerEvent);
}

void BossMagicianAttack2State::SpawnWarriors(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	Engine::GetEngineComponent<EventManager>().AddEvent<q_engine::LambdaEvent>([&, controlledState, enemyID]()
		{
			auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
			glm::vec2 enemyPosition = enemyTransform.GetPosition() + glm::vec2(0, 20);

			auto enemy = controlledState->AddEntity<EnemyEntity>();
			auto enemyWeapon = controlledState->AddEntity<EnemyWeaponEntity>();
			controlledState->AddComponent<FXAnimLuaScriptsComponent>(enemy);
			controlledState->AddComponent<TransformComponent>(enemy, enemyPosition, 0.f, glm::vec2{ 2 });
			controlledState->AddComponent<AnimationComponent>(enemy, "WarriorMissingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
			controlledState->AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
			controlledState->AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Warrior, enemyWeapon->GetEntityID(), glm::vec2{ 100,300 }, glm::vec2{ 0,500 });
			controlledState->AddComponent<HealthComponent>(enemy, 200);
			controlledState->AddComponent<GravityComponent>(enemy);
			controlledState->AddComponent<WidthHeightComponent>(enemy, 56.f, 70.f);
			controlledState->AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
			controlledState->AddComponent<DragAccelComponent>(enemy);
			controlledState->AddComponent<RaycastComponent>(enemy, 1, 10.f);
			controlledState->AddComponent<ObjectStateComponent>(enemy);
			controlledState->AddComponent<OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });

			controlledState->AddComponent<TransformComponent>(enemyWeapon, enemyPosition + glm::vec2{ 110, 40 });
			controlledState->AddComponent<WidthHeightComponent>(enemyWeapon, 30.f, 40.f);
			controlledState->AddComponent<RectCollisionComponent>(enemyWeapon, enemyPosition + glm::vec2{ 110, 40 }, 20.f, 10.f, false, true);
			controlledState->AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 30,10 }, 10);
			controlledState->AddComponent<OffsetComponent>(enemyWeapon);
		});
}

