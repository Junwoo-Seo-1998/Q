/*
	File Name: BossHelperFunctions.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossHelperFunctions.h"



#include "BossMagicianDyingState.h"
#include "EObjectZDepth.h"
#include "Component/BossMagicianComponent.h"
#include "Component/NewPlayerInventorySlots.h"
#include "Component/NewWeaponComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameState.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Core/AStarPathFinding.h"
#include "Engine/Event/TriggerEvent.h"
#include "Engine/Graphics/Particle.h"
#include "Entity/BombEntity.h"
#include "Entity/WeaponEntity.h"

void BoseMagicianMove(q_engine::GameState* controlledState, q_engine::IDType enemyID, double dt)
{
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& enemyVelocity = controlledState->GetComponents<q_engine::VelocityComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossMagicianComponent>()[enemyID];

	auto playerID = bossComp.GetPlayerID();
	auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[playerID];
	
	bossComp.UpdateInputCheckTime(dt);
	if (bossComp.GetInputCheckTime() > 0.3f)
	{
		q_engine::Engine::GetEngineComponent<q_engine::AStarPathFinding>().UpdatePath(reinterpret_cast<q_engine::Level*>(controlledState)
			, bossComp.GetPathToPlayer(), enemyTransform.GetPosition(), playerTransform.GetPosition());
		bossComp.ResetInputCheckTime();
	}
	
	if (bossComp.GetPathToPlayer().empty())
	{
		enemyVelocity.SetVelocity(glm::vec2(0));
		return;
	}
	
	if(isNear(enemyTransform.GetPosition(), bossComp.GetPathToPlayer().front()))
	{
		bossComp.GetPathToPlayer().pop_front();
		if (bossComp.GetPathToPlayer().empty())
		{
			enemyVelocity.SetVelocity(glm::vec2(0));
			return;
		}
	}
	glm::vec2 direction = glm::normalize(bossComp.GetPathToPlayer().front() - enemyTransform.GetPosition());
	glm::vec2 velocity = BossMagicianComponent::SPEED * direction;

	enemyVelocity.SetVelocity(velocity);
}

void BoseMagicianBasicTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
}