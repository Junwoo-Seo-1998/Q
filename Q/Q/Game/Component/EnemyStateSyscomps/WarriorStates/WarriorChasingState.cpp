/*
	File Name: WarriorChasingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "WarriorChasingState.h"

#include "../EnemyStateHelper.h"
#include "WarriorMissingState.h"
#include "WarriorSearchingState.h"
#include "WarriorAttackingState.h"
#include "WarriorDyingState.h"
#include "WarriorHurtingState.h"
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


std::string IEnemyState<WarriorChasingState>::StateName = "WarriorChasingState";
using namespace q_engine;

void WarriorChasingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WarriorChasingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.Run();
	enemyState.SetLookingPlayer(true);
}

void WarriorChasingState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& gravityComp = controlledState->GetComponents<q_engine::GravityComponent>()[enemyID];
	
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	if (abs(relativePosition).x < abs(enemyBox.Size().x) / 2 && abs(relativePosition).y < abs(enemyBox.Size().y) / 2)
		enemyState.SetCurrentState<WarriorMissingState>(controlledState);
	else if (relativePosition.x > abs(enemyBox.Size().x) / 2)
	{
		enemyState.MoveRight();
	}
	else if (relativePosition.x < -abs(enemyBox.Size().x) / 2)
	{
		enemyState.MoveLeft();
	}


	if (isCliffWall(controlledState, enemyID) && gravityComp.IsOnGround())
	{
		if (relativePosition.x > abs(enemyBox.Size().x) / 2)
		{
			enemyState.Jump(glm::vec2{ enemyState.GetMaxVelocity().x * 3, enemyState.GetMaxJumpVelocity().y });
		}
		else if (relativePosition.x < -abs(enemyBox.Size().x) / 2)
		{
			enemyState.Jump(glm::vec2{ -enemyState.GetMaxVelocity().x * 3, enemyState.GetMaxJumpVelocity().y });
		}
	}

	enemyState.UpdateLookingDirection(atan2(relativePosition.y, relativePosition.x));

	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];

	if (enemyHP.IsDead())
	{
		enemyState.SetCurrentState<WarriorDyingState>(controlledState);
	}
	else if (enemyHP.IsDamaged())
	{
		enemyState.SetCurrentState<WarriorHurtingState>(controlledState);
	}
}

void WarriorChasingState::Unload([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] IDType enemyID)
{
}

void WarriorChasingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	int tempHP = enemyHP.GetCurHealth();

	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
	EnemyJumpTriggerEvent(controlledState, enemyID, triggerEvent);


	if (enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<WarriorDyingState>(controlledState);
	}
	else if (tempHP > enemyHP.GetCurHealth())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<WarriorHurtingState>(controlledState);
	}
}

void WarriorChasingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
	}
	else
	{
		enemyState.SetCurrentState<WarriorSeachingState>(controlledState);
	}
}

void WarriorChasingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<WarriorAttackingState>(controlledState);
}
