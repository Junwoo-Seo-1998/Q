/*
	File Name: EnemyStateSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "EnemyStateSystem.h"

#include <future>

#include "EObjectZDepth.h"
#include "../../Engine/Event/TriggerEvent.h"
#include "../../Engine/Event/RayTriggerEvent.h"
#include "../../Engine/Graphics/Particle.h"
#include "../Component/EnemyWeaponComponent.h"
#include "../Component/HealthComponent.h"
#include "../Component/DeathItemDropComponent.h"
#include "../Engine/Component/GravityComponent.h"
#include "../Engine/Component/ObjectStateComponent.h"
#include "../Engine/Component/OffsetComponent.h"
#include "../Game/Component/EnemyStateComponent.h"
#include "../Engine/GameState.h"
#include "../Engine/Core/ThreadPool.h"
#include "../Engine/Engine.h"
#include "../Entity/PhysicsTestEntities.h"
#include "../Entity/WeaponEntity.h"
#include "../Engine/System/TimerManagerSystem.h"
#include "Component/DropItemComponent.h"
#include "Component/FXAnimLuaScriptsComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Entity/ItemEntity.h"
#include "FXAnimLuaScriptsUpdateSystem.h"


void EnemyStateSystem::EnemyInputCheck(EnemyStateComponent& enemyState)
{
	IDType enemyID = enemyState.GetEntityID();

	if (!m_controlled_state->HasComponent<GravityComponent>(enemyID) || !enemyState.isMoveByInput())
		return;
	
	auto& enemyVelocity = m_controlled_state->GetComponents<q_engine::VelocityComponent>()[enemyID];
	auto& enemyObjectState = m_controlled_state->GetComponents<q_engine::ObjectStateComponent>()[enemyID];
	auto& enemyOffsetState = m_controlled_state->GetComponents<q_engine::OffsetComponent>()[enemyID];
	auto& gravityComp = m_controlled_state->GetComponents<q_engine::GravityComponent>()[enemyID];



	if (gravityComp.IsOnGround())
	{
		enemyVelocity.SetVelocityX(0);

		if (enemyState.isJumping())
		{
			enemyVelocity.SetVelocity(enemyState.GetJumpVelocity());
			enemyState.Land();
		}
		else
		{
			if (!enemyState.isMovingRight() && !enemyState.isMovingLeft())
			{
				enemyVelocity.SetVelocityX(0);
			}
			else
			{
				if (enemyState.isMovingLeft())
				{
					enemyVelocity.SetVelocityX(-enemyState.GetMaxVelocity().x);
					enemyObjectState.setIsFlipped(true);
					enemyOffsetState.SetFlipped(true);
				}
				if (enemyState.isMovingRight())
				{
					enemyVelocity.SetVelocityX(enemyState.GetMaxVelocity().x);
					enemyObjectState.setIsFlipped(false);
					enemyOffsetState.SetFlipped(false);
				}
			}
		}
	}
	else
	{
		static const float JumpMultiply = 1.2f;
		if (enemyState.isMovingLeft())
		{
			enemyVelocity.SetVelocityX(-enemyState.GetMaxVelocity().x * JumpMultiply); 
			enemyObjectState.setIsFlipped(true);
			enemyOffsetState.SetFlipped(true);
		}
		if (enemyState.isMovingRight())
		{
			enemyVelocity.SetVelocityX(enemyState.GetMaxVelocity().x * JumpMultiply);
			enemyObjectState.setIsFlipped(false);
			enemyOffsetState.SetFlipped(false);
		}
	}
	if (!enemyState.isLookingPlayer())
	{
		if (enemyState.isMovingRight())
			enemyState.UpdateLookingDirection(0.f);
		else if (enemyState.isMovingLeft())
			enemyState.UpdateLookingDirection(PI);
	}
}


void EnemyStateSystem::ThreadedUpdate(double deltaTime, int threadNum, std::vector<IDType>& deadEnemies)
{
	auto enemyStateComponents = m_controlled_state->GetComponents<EnemyStateComponent>().GetPartContainer(q_engine::ThreadPool::GetMaxThreads(), threadNum);
	
	for (auto& idx : enemyStateComponents)
	{		
		idx.UpdateCurrentState(m_controlled_state, deltaTime);
		
		idx.isUpdated = true;
		
		if (idx.isDead())
		{
			std::lock_guard<std::mutex> lockGuard(q_engine::ThreadPool::GetMutex());

			deadEnemies.push_back(idx.GetEntityID());
			deadEnemies.push_back(idx.GetWeaponID());
		}
		else
		{

			EnemyInputCheck(idx);
		}
	}
}

constexpr int RandomPercentReverse = 2;

void EnemyStateSystem::DropItem(IDType enemyID)
{
	bool notDroppingItem = Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0, RandomPercentReverse - 1);
	if(notDroppingItem)
	{
		return;
	}
	
	
	DeathItemDropComponent::ItemType itemType = static_cast<DeathItemDropComponent::ItemType>(Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0, static_cast<int>(DeathItemDropComponent::ItemType::ItemNum) - 1));
	glm::vec2 enemyPos = m_controlled_state->GetComponents<TransformComponent>()[enemyID].GetPosition();
	glm::vec2 enemySize = m_controlled_state->GetComponents<WidthHeightComponent>()[enemyID].GetWidthHeight();
	enemyPos += glm::vec2(0, enemySize.y);

	ItemEntity* const droppedItem = m_controlled_state->AddEntity<ItemEntity>();
	
	switch(itemType)
	{
	case DeathItemDropComponent::ItemType::Potion:
		m_controlled_state->AddComponent<DropItemComponent>(droppedItem, "Potion");
		m_controlled_state->AddComponent<TransformComponent>(droppedItem, enemyPos);
		m_controlled_state->AddComponent<TextureComponent>(droppedItem, "PotionIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
		m_controlled_state->AddComponent<OffsetComponent>(droppedItem);
		m_controlled_state->AddComponent<WidthHeightComponent>(droppedItem, 50.f, 50.f);
		m_controlled_state->AddComponent<VelocityComponent>(droppedItem);
		m_controlled_state->AddComponent<GravityComponent>(droppedItem);
		m_controlled_state->AddComponent<RectCollisionComponent>(droppedItem, enemyPos, 50.f, 50.f, true, true);
		break;
	case DeathItemDropComponent::ItemType::Bomb:
		m_controlled_state->AddComponent<DropItemComponent>(droppedItem, "Bomb");
		m_controlled_state->AddComponent<TransformComponent>(droppedItem, enemyPos);
		m_controlled_state->AddComponent<TextureComponent>(droppedItem, "BombIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
		m_controlled_state->AddComponent<OffsetComponent>(droppedItem);
		m_controlled_state->AddComponent<WidthHeightComponent>(droppedItem, 50.f, 50.f);
		m_controlled_state->AddComponent<VelocityComponent>(droppedItem);
		m_controlled_state->AddComponent<GravityComponent>(droppedItem);
		m_controlled_state->AddComponent<RectCollisionComponent>(droppedItem, enemyPos, 50.f, 50.f, true, true);
		break;
	case DeathItemDropComponent::ItemType::PoisonBomb:
		m_controlled_state->AddComponent<DropItemComponent>(droppedItem, "PoisonBomb");
		m_controlled_state->AddComponent<TransformComponent>(droppedItem, enemyPos);
		m_controlled_state->AddComponent<TextureComponent>(droppedItem, "PoisonBombIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
		m_controlled_state->AddComponent<OffsetComponent>(droppedItem);
		m_controlled_state->AddComponent<WidthHeightComponent>(droppedItem, 50.f, 50.f);
		m_controlled_state->AddComponent<VelocityComponent>(droppedItem);
		m_controlled_state->AddComponent<GravityComponent>(droppedItem);
		m_controlled_state->AddComponent<RectCollisionComponent>(droppedItem, enemyPos, 50.f, 50.f, true, true);
		break;
	}
}

void EnemyStateSystem::Update(double deltaTime)
{
	FUNCTION_PROFILING();

	std::vector<IDType> deadEnemies;

	q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().WakeUpThreads(ThreadPool::GetMaxThreads());
	for (unsigned num = 0; num < q_engine::ThreadPool::GetMaxThreads(); ++num)
	{
		q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().GiveThreadWork([this, num = num, &deadEnemies, deltaTime]()
		{
				ThreadedUpdate(deltaTime, num, deadEnemies);
		});
	}

	q_engine::Engine::GetEngineComponent<q_engine::ThreadPool>().WaitAll();

	for(IDType idx : deadEnemies)
	{
		if(m_controlled_state->HasComponent<DeathItemDropComponent>(idx))
		{
			DropItem(idx);
		}

		m_controlled_state->DeleteEntityComponentsLater(q_engine::GetTypeID<EnemyEntity>(),idx);
	}

}

void EnemyStateSystem::OnEvent(q_engine::IEvent* eventObj)
{	
	if (dynamic_cast<q_engine::TriggerEvent*>(eventObj) != nullptr)
	{
		q_engine::TriggerEvent* triggerEvent = reinterpret_cast<q_engine::TriggerEvent*>(eventObj);
		if(m_controlled_state->GetComponents<EnemyStateComponent>().isExist(triggerEvent->mCollideWithEntityID))
			m_controlled_state->GetComponents<EnemyStateComponent>()[triggerEvent->mCollideWithEntityID].CurrentStateOnTriggerEvent(m_controlled_state, triggerEvent);
		
		else if (m_controlled_state->GetComponents<EnemyWeaponComponent>().isExist(triggerEvent->mTriggerEntityID) &&
			m_controlled_state->GetComponents<HealthComponent>().isExist(triggerEvent->mCollideWithEntityID))
		{
			auto& enemyWeapon = m_controlled_state->GetComponents<EnemyWeaponComponent>()[triggerEvent->mTriggerEntityID];
			auto& playerHealth = m_controlled_state->GetComponents<HealthComponent>()[triggerEvent->mCollideWithEntityID];

			
			if(!playerHealth.IsDead() && m_controlled_state->HasComponent<EnemyStateComponent>(enemyWeapon.GetOwnerEntityID()))
				m_controlled_state->GetComponents<EnemyStateComponent>()[enemyWeapon.GetOwnerEntityID()].CurrentStateOnAttackEvent(m_controlled_state);
		}
			
	}
	else if (dynamic_cast<q_engine::RayTriggerEvent*>(eventObj) != nullptr)
	{
		q_engine::RayTriggerEvent* triggerEvent = reinterpret_cast<q_engine::RayTriggerEvent*>(eventObj);
		if (m_controlled_state->GetComponents<EnemyStateComponent>().isExist(triggerEvent->rayOwner.entityID))
			m_controlled_state->GetComponents<EnemyStateComponent>()[triggerEvent->rayOwner.entityID].CurrentStateOnRayEvent(m_controlled_state, triggerEvent);
	}
}
