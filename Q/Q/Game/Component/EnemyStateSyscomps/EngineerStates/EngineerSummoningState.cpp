/*
	File Name: EngineerSummoningState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EngineerSummoningState.h"



#include "EngineerDyingState.h"
#include "EngineerMissingState.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Component/FXAnimLuaScriptsComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/DragAccelComponent.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/RaycastComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Event/LambdaEvent.h"
#include "Entity/PhysicsTestEntities.h"

void EngineerSummoningState::SummonTurret(q_engine::GameState* controlledState, q_engine::IDType enemyID, double dt, glm::vec2 position)
{
	using namespace q_engine;
	Engine::GetEngineComponent<EventManager>().AddEvent<q_engine::LambdaEvent>([position, controlledState]()
		{
			auto enemy = controlledState->AddEntity<EnemyEntity>();
			auto enemyWeapon = controlledState->AddEntity<EnemyEntity>();

			controlledState->AddComponent<FXAnimLuaScriptsComponent>(enemy);
			controlledState->AddComponent<q_engine::TransformComponent>(enemy, position, 0.f, glm::vec2{ 2 });
			controlledState->AddComponent<q_engine::AnimationComponent>(enemy, "SummonBigBoyAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
			controlledState->AddComponent<q_engine::VelocityComponent>(enemy, glm::vec2{ 100, 0 });
			controlledState->AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Turret, enemyWeapon->GetEntityID(), glm::vec2{ 100,300 }, glm::vec2{ 0,500 });
			controlledState->AddComponent<HealthComponent>(enemy, 800);
			controlledState->AddComponent<q_engine::GravityComponent>(enemy);
			controlledState->AddComponent<q_engine::WidthHeightComponent>(enemy, 80.f, 100.f);
			controlledState->AddComponent<q_engine::RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
			controlledState->AddComponent<q_engine::DragAccelComponent>(enemy);
			controlledState->AddComponent<q_engine::RaycastComponent>(enemy, 1, 10.f);
			controlledState->AddComponent<q_engine::ObjectStateComponent>(enemy);
			controlledState->AddComponent<q_engine::OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });
		});
}

void EngineerSummoningState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("SummonAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(false);
}
constexpr glm::vec2 offset(50, 20);

void EngineerSummoningState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double dt)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<EngineerMissingState>(controlledState);

		if(enemyState.isMovingRight())
		{
			glm::vec2 position = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID].GetPosition() + glm::vec2(-offset.x, offset.y);
			SummonTurret(controlledState, enemyID, dt, position);
			enemyState.SetCurrentState<EngineerDyingState>(controlledState);
			return;
		}
		else
		{
			glm::vec2 position = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID].GetPosition() + offset.x;
			SummonTurret(controlledState, enemyID, dt, position);
			enemyState.SetCurrentState<EngineerDyingState>(controlledState);
			return;
		}

	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<EngineerDyingState>(controlledState);
	}
}

void EngineerSummoningState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
}

void EngineerSummoningState::OnRayEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*)
{
}

void EngineerSummoningState::OnAttackEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
}
