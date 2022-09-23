/*
	File Name: FlameThrowerSearchingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FlameThrowerSearchingState.h"


#include "FlameThrowerMissingState.h"
#include "../EnemyStateHelper.h"
#include "../../../../Engine/GameState.h"
#include "../../../../Engine/Component/ObjectStateComponent.h"
#include "../../../../Engine/Component/VelocityComponent.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/CollisionComponent.h"
#include "../../../../Engine/Component/GravityComponent.h"
#include "../../../../Engine/Component/TransformComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../../../EObjectZDepth.h"
#include "../../JumpTriggerComponent.h"
#include "../../EnemyStateComponent.h"
#include "../../../Entity/JumpTriggerEntity.h"
#include "../../../../Engine/Core/StringHash.h"
#include "FlameThrowerMissingState.h"
#include "FlameThrowerchasingState.h"
#include "FlameThrowerAttackingState.h"
#include "FlameThrowerDyingState.h"
#include "FlameThrowerHurtingState.h"
#include "Component/HealthComponent.h"
using namespace q_engine;

std::string IEnemyState<FlameThrowerSearchingState>::StateName = "FlameThrowerSearchingState";

void FlameThrowerSearchingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("FlamethrowerBotWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	EnemyStateComponent& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(3.f);
	timer.SetOnTimeOut([enemyID,controlledState]()
		{
			if(controlledState->GetComponents<EnemyStateComponent>().isExist(enemyID))
			{
				auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
				if (enemyState.GetCurrentStateID() == FlameThrowerSearchingState::GetStateID())
					enemyState.SetCurrentState<FlameThrowerMissingState>(controlledState);
			}
		}
	);

	enemyState.Run();
	enemyState.SetLookingPlayer(true);
}

void FlameThrowerSearchingState::Update(q_engine::GameState* controlledState, IDType enemyID, [[maybe_unused]] double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& gravityComp = controlledState->GetComponents<q_engine::GravityComponent>()[enemyID];
	
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	if (abs(relativePosition).x < abs(enemyBox.Size().x) / 2 && abs(relativePosition).y < abs(enemyBox.Size().y) / 2)
		enemyState.SetCurrentState<FlameThrowerMissingState>(controlledState);
	else if (relativePosition.x > abs(enemyBox.Size().x) / 2)
		enemyState.MoveRight();
	else if (relativePosition.x < -abs(enemyBox.Size().x) / 2)
		enemyState.MoveLeft();

	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (isCliffWall(controlledState, enemyID) && gravityComp.IsOnGround())
	{
		if (!animation.IsAnimationID("FlamethrowerBotIdleAnim"_hashID))
		{
			animation.ChangeAnimation("FlamethrowerBotIdleAnim"_hashID);
		}
		enemyState.SetSpeedMultiplier(0.f);
	}
	else
	{
		if (!animation.IsAnimationID("FlamethrowerBotWalkingAnim"_hashID))
		{
			animation.ChangeAnimation("FlamethrowerBotWalkingAnim"_hashID);
		}
		enemyState.ResetSpeedMultiplier();
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

void FlameThrowerSearchingState::Unload([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}

void FlameThrowerSearchingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void FlameThrowerSearchingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<FlameThrowerChasingState>(controlledState);
	}

}

void FlameThrowerSearchingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<FlameThrowerAttackingState>(controlledState);
}

