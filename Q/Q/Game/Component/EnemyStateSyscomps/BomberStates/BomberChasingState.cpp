/*
	File Name: BomberChasingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BomberChasingState.h"



#include "BomberDyingState.h"
#include "BomberExplodingState.h"
#include "BomberMissingState.h"
#include "BomberSearchingState.h"
#include "EObjectZDepth.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Core/AStarPathFinding.h"
#include "Engine/Engine.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Entity/PhysicsTestEntities.h"

constexpr float NEAR_VALUE = 10.f;

bool isNear(glm::vec2 pos, glm::vec2 path)
{

	glm::vec2 delta = glm::abs(path - pos);

	return delta.x < NEAR_VALUE&& delta.y < NEAR_VALUE;

}

void BomberChasingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WardroidWakeAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(1.5);
	enemyState.SetLookingPlayer(true);
	enemyState.SetMoveByInput(false);
	q_engine::Engine::GetEngineComponent<q_engine::AStarPathFinding>().UpdatePath(reinterpret_cast<q_engine::Level*>(controlledState)
		, enemyState.GetPathToPlayer(), enemyTransform.GetPosition(), enemyState.GetLastSeenPlayerPosition());

	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(0.1f);
	timer.SetOnTimeOut([enemyID, controlledState]()
		{
			if (controlledState->GetEntities<EnemyEntity>().isExist(enemyID))
			{
				auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
				auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
				if (enemyState.GetCurrentStateID() == BomberChasingState::GetStateID())
				{
					std::lock_guard<std::mutex> lock(q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().GetMutex());
					q_engine::Engine::GetEngineComponent<q_engine::AStarPathFinding>().UpdatePath(reinterpret_cast<q_engine::Level*>(controlledState)
						, enemyState.GetPathToPlayer(), enemyTransform.GetPosition(), enemyState.GetLastSeenPlayerPosition());
				}

			}
		}
	);
}

void BomberChasingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyObjectState = controlledState->GetComponents<q_engine::ObjectStateComponent>()[enemyID];
	auto& enemyOffsetState = controlledState->GetComponents<q_engine::OffsetComponent>()[enemyID];
	auto& enemyVelocity = controlledState->GetComponents<q_engine::VelocityComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());

	if(timer.GetRemainingTime() <= 0.f)
	{

		timer.ResetAndResume(0.1f);

		std::lock_guard<std::mutex> lock(q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().GetMutex());
		q_engine::Engine::GetEngineComponent<q_engine::AStarPathFinding>().UpdatePath(reinterpret_cast<q_engine::Level*>(controlledState)
			, enemyState.GetPathToPlayer(), enemyTransform.GetPosition(), enemyState.GetLastSeenPlayerPosition());

	}

	if (enemyState.GetPathToPlayer().empty())
	{
		return;
	}

	if (isNear(enemyTransform.GetPosition(), enemyState.GetPathToPlayer().front()))
	{
		enemyState.GetPathToPlayer().pop_front();
		if (enemyState.GetPathToPlayer().empty())
		{
			return;
		}
	}
	glm::vec2 direction = glm::normalize(enemyState.GetPathToPlayer().front() - enemyTransform.GetPosition());
	glm::vec2 velocity = enemyState.GetMaxVelocity().x * direction;
	if(direction.y > 0)
	{
		velocity.y *= 2;
	}
	enemyVelocity.SetVelocity(velocity);
	
	if (enemyVelocity.GetVelocityX() < 0)
	{
		enemyObjectState.setIsFlipped(true);
		enemyOffsetState.SetFlipped(true);
	}
	else
	{
		enemyObjectState.setIsFlipped(false);
		enemyOffsetState.SetFlipped(false);
	}

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<BomberDyingState>(controlledState);
	}
}

void BomberChasingState::Unload(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetMoveByInput(false);
	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndPause();
}

void BomberChasingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* eventObj)
{
}

void BomberChasingState::OnRayEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* eventObj)
{
	if(!isPlayerOnSight(controlledState, enemyID, eventObj))
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<BomberSearchingState>(controlledState);
	}
}

void BomberChasingState::OnAttackEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<BomberExplodingState>(controlledState);
}
