/*
	File Name: FlameThrowerChasingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FlameThrowerChasingState.h"

#include "../EnemyStateHelper.h"
#include "FlameThrowerMissingState.h"
#include "FlameThrowerSearchingState.h"
#include "FlameThrowerAttackingState.h"
#include "FlameThrowerDyingState.h"
#include "FlameThrowerHurtingState.h"
#include "../../../EObjectZDepth.h"
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
#include "Component/HealthComponent.h"


std::string IEnemyState<FlameThrowerChasingState>::StateName = "FlameThrowerChasingState";
using namespace q_engine;

void FlameThrowerChasingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("FlamethrowerBotWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.Run();
	enemyState.SetLookingPlayer(true);
}

void FlameThrowerChasingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& gravityComp = controlledState->GetComponents<q_engine::GravityComponent>()[enemyID];
	
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	if (abs(relativePosition).x < abs(enemyBox.Size().x) / 2 && abs(relativePosition).y < abs(enemyBox.Size().y) / 2)
		enemyState.SetCurrentState<FlameThrowerMissingState>(controlledState);
	else if (relativePosition.x > abs(enemyBox.Size().x) / 2)
	{
		enemyState.MoveRight();
	}
	else if (relativePosition.x < -abs(enemyBox.Size().x) / 2)
	{
		enemyState.MoveLeft();
	}

	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (isCliffWall(controlledState, enemyID) && gravityComp.IsOnGround())
	{
		  if(!animation.IsAnimationID("FlamethrowerBotIdleAnim"_hashID)) 
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

	enemyState.UpdateLookingDirection(atan2(relativePosition.y, relativePosition.x));

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

void FlameThrowerChasingState::Unload([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}

void FlameThrowerChasingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	int tempHP = enemyHP.GetCurHealth();

	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
}

void FlameThrowerChasingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
	else
	{
		enemyState.SetCurrentState<FlameThrowerSearchingState>(controlledState);
	}
}

void FlameThrowerChasingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<FlameThrowerAttackingState>(controlledState);
}
