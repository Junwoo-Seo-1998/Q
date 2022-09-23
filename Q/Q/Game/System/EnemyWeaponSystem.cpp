/*
	File Name: EnemyWeaponSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary: Hoseob Jeong
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnemyWeaponSystem.h"


#include "../../Engine/Event/TriggerEvent.h"
#include "../../Engine/Component/TransformComponent.h"
#include "../../Engine/Component/VelocityComponent.h"
#include "../Engine/GameState.h"

#include "../Entity/WeaponEntity.h"
#include "../Entity/MapEntity.h"
#include "../Game/Component/EnemyWeaponComponent.h"
#include "../Game/Component/HealthComponent.h"
#include "../Game/Component/DamageComponent.h"
#include "PlayTestTrace.h"

void EnemyWeaponSystem::Update([[maybe_unused]] double deltaTime)
{
	using namespace q_engine;
	auto& ObjectStateComponents = m_controlled_state->GetComponents<ObjectStateComponent>();
	auto& WidthHeightComponents = m_controlled_state->GetComponents<WidthHeightComponent>();
	auto& enemyWeaponComponents = m_controlled_state->GetComponents<EnemyWeaponComponent>();
	auto& TransformComponents = m_controlled_state->GetComponents<TransformComponent>();

	for(auto& idx : enemyWeaponComponents)
	{
		if (idx.IsFollowingOwner())
		{
			IDType enemyWeaponID = idx.GetEntityID();
			IDType enemyID = idx.GetOwnerEntityID();

			auto& enemyObjectState = ObjectStateComponents[enemyID];
			auto enemyWeaponOffset = enemyWeaponComponents[enemyWeaponID].GetWeaponOffset();
			float enemyWidth= WidthHeightComponents[enemyID].GetWidth<float>();
			float enemyWeaponWidth = WidthHeightComponents[enemyWeaponID].GetWidth<float>();

			
			glm::vec2 offset;
			if (enemyObjectState.GetIsFlipped())
				offset = glm::vec2{ -(enemyWeaponWidth - (enemyWidth - enemyWeaponOffset.x))  ,  enemyWeaponOffset.y };
			else
				offset = enemyWeaponOffset;

			TransformComponents[enemyWeaponID].SetPosition(TransformComponents[enemyID].GetPosition() + offset);
		}
	}
}
 
void EnemyWeaponSystem::OnEvent(q_engine::IEvent* eventName)
{
	if (dynamic_cast<q_engine::TriggerEvent*>(eventName) != nullptr)
	{
		q_engine::TriggerEvent* triggerEvent = reinterpret_cast<q_engine::TriggerEvent*>(eventName);
		
		if (!m_controlled_state->GetEntities<EnemyWeaponEntity>().isExist(triggerEvent->mTriggerEntityID))
			return;
		
		if(triggerEvent->mCollideWithEntityTypeID == q_engine::GetTypeID<PlayerEntity>())
		{
			IDType enemyID = triggerEvent->mTriggerEntityID;
			IDType playerID = triggerEvent->mCollideWithEntityID;
			EnemyWeaponComponent& enemyWeapon = m_controlled_state->GetComponents<EnemyWeaponComponent>()[enemyID];



			auto& weaponTransform = m_controlled_state->GetComponents<q_engine::TransformComponent>()[enemyID];

			
			if(enemyWeapon.IsAttacking())
			{
				PlayerEntity* playerPtr = &m_controlled_state->GetEntities<PlayerEntity>().Front();
				enemyWeapon.SetAttacking(false);

				if (m_controlled_state->HasComponent<DamageComponent>(playerID))
				{
					if (enemyWeapon.GetDamageType() == DIRECTDAMAGE)
					{
						m_controlled_state->GetComponents<DamageComponent>()[playerID].AddDamage(enemyWeapon.GetAttackDamage(), weaponTransform.GetPosition());
					}
					else
					{
						m_controlled_state->GetComponents<DamageComponent>()[playerID].AddDamage(enemyWeapon.GetAttackDamage(), 4.f, enemyWeapon.GetDamageType());
					}
				}
				else
				{
					if(enemyWeapon.GetDamageType() == DIRECTDAMAGE)
					{
						m_controlled_state->AddComponent<DamageComponent>(playerPtr, enemyWeapon.GetAttackDamage(), weaponTransform.GetPosition());
					}
					else
					{
						m_controlled_state->AddComponent<DamageComponent>(playerPtr, enemyWeapon.GetAttackDamage(), 4.f, enemyWeapon.GetDamageType());
					}
				}

				if(m_controlled_state->GetEntities<EnemyEntity>().isExist(enemyWeapon.GetOwnerEntityID()))
				{
					EnemyStateComponent::EnemyType triggerEnemyType = m_controlled_state->GetComponents<EnemyStateComponent>()[enemyWeapon.GetOwnerEntityID()].GetEnemyType();

					if (enemyWeapon.IsBullet())
						m_controlled_state->DeleteEntityComponentsLater(q_engine::GetTypeID<EnemyWeaponEntity>(), enemyID);


					std::string hit_enemy_type_as_name;

					switch (triggerEnemyType)
					{
					case EnemyStateComponent::EnemyType::Warrior:
					{
						hit_enemy_type_as_name = "Warrior";
						break;
					}
					case EnemyStateComponent::EnemyType::BossMagician:
					{
						hit_enemy_type_as_name = "BossMagician";
						break;
					}
					case EnemyStateComponent::EnemyType::Tanker:
					{
						hit_enemy_type_as_name = "Tanker";
						break;
					}
					case EnemyStateComponent::EnemyType::Scout:
					{
						hit_enemy_type_as_name = "Scout";
						break;
					}
					}
					PlayTestTrace::LogPlayerHitByMessage(hit_enemy_type_as_name);
				}
			}
		}
		else if (triggerEvent->mCollideWithEntityTypeID == q_engine::GetTypeID<MapEntity>())
		{
			EnemyWeaponComponent& enemyWeapon = m_controlled_state->GetComponents<EnemyWeaponComponent>()[triggerEvent->mTriggerEntityID];
			if (enemyWeapon.IsBullet())
				m_controlled_state->DeleteEntityComponentsLater(q_engine::GetTypeID<EnemyWeaponEntity>(), triggerEvent->mTriggerEntityID);
		}
	}
}

