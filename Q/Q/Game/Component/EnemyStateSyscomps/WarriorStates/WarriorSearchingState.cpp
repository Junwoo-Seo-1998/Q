/*
	File Name: WarriorSearchingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "WarriorSearchingState.h"

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
#include "WarriorMissingState.h"
#include "WarriorchasingState.h"
#include "WarriorAttackingState.h"
#include "WarriorDyingState.h"
#include "WarriorHurtingState.h"
#include "Component/HealthComponent.h"
using namespace q_engine;

std::string IEnemyState<WarriorSeachingState>::StateName = "WarriorSearchingState";

void WarriorSeachingState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("WarriorChasingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));

	EnemyStateComponent& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(enemyState.GetTimerID());
	timer.ResetAndResume(3.f);
	timer.SetOnTimeOut([enemyID,controlledState]()
		{
			if(controlledState->GetComponents<EnemyStateComponent>().isExist(enemyID))
			{
				auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
				if (enemyState.GetCurrentStateID() == WarriorSeachingState::GetStateID())
					enemyState.SetCurrentState<WarriorMissingState>(controlledState);
			}
		}
	);

	enemyState.Run();
	enemyState.SetLookingPlayer(true);
}

void WarriorSeachingState::Update(q_engine::GameState* controlledState, IDType enemyID, [[maybe_unused]] double deltaTime)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& gravityComp = controlledState->GetComponents<q_engine::GravityComponent>()[enemyID];
	
	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	if (abs(relativePosition).x < abs(enemyBox.Size().x) / 2 && abs(relativePosition).y < abs(enemyBox.Size().y) / 2)
		enemyState.SetCurrentState<WarriorMissingState>(controlledState);
	else if (relativePosition.x > abs(enemyBox.Size().x) / 2)
		enemyState.MoveRight();
	else if (relativePosition.x < -abs(enemyBox.Size().x) / 2)
		enemyState.MoveLeft();

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

void WarriorSeachingState::Unload([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]]IDType enemyID)
{
}

void WarriorSeachingState::OnTriggerEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
	int tempHP = enemyHP.GetCurHealth();
	
	BasicEnemyTriggerEvent(controlledState, enemyID, triggerEvent);
	EnemyJumpTriggerEvent(controlledState, enemyID, triggerEvent);

	if(enemyHP.IsDead())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<WarriorDyingState>(controlledState);
	}
	else if(tempHP > enemyHP.GetCurHealth())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<WarriorHurtingState>(controlledState);
	}
}

void WarriorSeachingState::OnRayEvent(q_engine::GameState* controlledState, IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (isPlayerOnSight(controlledState, enemyID, triggerEvent))
	{
		enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
		enemyState.SetCurrentState<WarriorChasingState>(controlledState);
	}

}

void WarriorSeachingState::OnAttackEvent(q_engine::GameState* controlledState, IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetCurrentState<WarriorAttackingState>(controlledState);
}

