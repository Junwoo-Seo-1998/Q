/*
	File Name: DamageSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "DamageSystem.h"
#include "../Engine/ECS/ComponentManager.h"
#include "Component/DamageComponent.h"
#include "../Engine/GameState.h"
#include "Component/EnemyStateComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/SoundComponent.h"
#include "Engine/Core/RandomMachine.h"
#include "Engine/Engine.h"
#include "Entity/PhysicsTestEntities.h"
#include "PlayTestTrace.h"
#include "../Engine/GameStateManager.h"
#include "Component/FXAnimLuaScriptsComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Graphics/PostProcessor.h"
constexpr glm::vec2 playerNuckBack(300.f,200.f);

void DamageSystem::Load()
{

	
}

void DamageSystem::OnEvent([[maybe_unused]] q_engine::IEvent* eventName)
{

	
}

void DamageSystem::Update(double deltaTime)
{

	const auto& CurrentLevel = m_controlled_state;

	auto& DamageComponents = CurrentLevel->GetComponents<DamageComponent>();
	auto& HealthComponents = CurrentLevel->GetComponents<HealthComponent>();
	auto& FXComponents = CurrentLevel->GetComponents<FXAnimLuaScriptsComponent>();

	auto& SoundComponent = q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>();
	auto& Random = q_engine::Engine::GetEngineComponent<q_engine::RandomMachine>();
	for (HealthComponent& health : HealthComponents)
	{
		health.SetDamaged(false);
	}
	
	for(DamageComponent& component : DamageComponents)
	{
		q_engine::IDType entityID = component.GetEntityID();
		q_engine::IDType entityTypeID = component.GetEntityTypeID();
	    q_engine::IDType compID = component.GetTypeID();


		if (CurrentLevel->HasComponent<HealthComponent>(entityID))
		{
			HealthComponent& health = HealthComponents[entityID];

			for (auto& damage:component.GetDamageList())
			{
				damage.TimerUpdate(deltaTime);
			}
			
			if (health.IsDead() || (health.IsInvincible() && (component.GetDamageList().front().GetBulletType() == DamageType::DIRECTDAMAGE)))
			{
				CurrentLevel->DeleteComponentLater(compID, entityID);
				component.SetIsComponentDeleted(true);
				continue;
			}


			health.SetDamaged(true);
			if(entityTypeID == q_engine::GetTypeID<PlayerEntity>())
			{
				if (component.GetDamageList().front().GetBulletType() == DamageType::DIRECTDAMAGE)
				{
					FXComponents[entityID].RunScript("Shield");
					health.SetInvincible(3.f);

					const int playerHP = health.AsPercentage() * 100;
					
					if (playerHP <= 30)
						q_engine::Engine::GetPostProcessor()->SetMode(Redout, 0.5f);
					else
						q_engine::Engine::GetPostProcessor()->SetMode(Blur, 0.5f);
				}
				else 
				{
					const int playerHP = health.AsPercentage() * 100;
					if (playerHP <= 30)
						q_engine::Engine::GetPostProcessor()->SetMode(Redout, 0.5f);
					else
						q_engine::Engine::GetPostProcessor()->SetMode(Greenout, 0.5f);
				}


			}
			if((entityTypeID == q_engine::GetTypeID<PlayerEntity>()) ||
				(entityTypeID == q_engine::GetTypeID<EnemyEntity>()))
			{
				static const std::string defaultSFX = "EnemyHit";

				const int idx = Random.GetRandomValue<int>(1, 4);

				SoundComponent.PlaySoundEffect(std::string{ defaultSFX + std::to_string(idx) }, false);
			}

			const int lastFrameHP = health.GetCurHealth();

			for (auto& damage : component.GetDamageList())
			{

				if (damage.IsInCategory(DamageType::DIRECTDAMAGE))
				{
					health.ChangeCurHealth(-damage.GetDamageValue());
				}
				else if (damage.GetBulletType() >= DamageType::DOTDAMGAE)
				{
					if (damage.GetRemainTime() < damage.GetRemainDamageCount())
					{
						if(entityTypeID == q_engine::GetTypeID<PlayerEntity>())
						{
							health.ChangeCurHealth(static_cast<int>(-damage.GetDamageValue() * 0.5));
						}
						else
						{
							health.ChangeCurHealth(-damage.GetDamageValue());
						}

						int count = damage.GetRemainDamageCount();
						if (count > 0)
						{
							damage.SetRemainDamageCount(count - 0.5);
						}
					}

				}

				if (damage.GetBulletType() == DamageType::DIRECTDAMAGE)
				{
					if (entityTypeID == q_engine::GetTypeID<PlayerEntity>())
					{
						float enemyXPos = damage.GetDamagedPosition().x;
						float playerXPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[entityID].GetPositionX();
						if (enemyXPos - playerXPos > 0)
						{
							m_controlled_state->GetComponents<q_engine::VelocityComponent>()[entityID].SetVelocity({ -playerNuckBack.x, playerNuckBack.y });
						}
						else
						{
							m_controlled_state->GetComponents<q_engine::VelocityComponent>()[entityID].SetVelocity(playerNuckBack);
						}
					}
					FXComponents[entityID].RunScript("HitBy");
				}
				else
				{

					switch (damage.GetBulletType())
					{
					case DamageType::POISONDAMAGE:
					{
						FXComponents[entityID].RunScript("Poisoned");
						break;
					}
					default:
					{
						FXComponents[entityID].RunScript("Flamed");
						break;
					}
					}
				}
			}
			if(health.IsDead() && lastFrameHP > 0)
			{
				static const std::string defaultSFX{ "Explode" };

				SoundComponent.PlaySoundEffect(defaultSFX + std::to_string(Random.GetRandomValue(1, 5)), false);

				FXComponents[entityID].RunScript("DeadExplode");
				FXComponents[entityID].RunScript("DeadExplode");
				FXComponents[entityID].RunScript("DeadExplode");
				FXComponents[entityID].RunScript("DeadExplode");
			}
		}

		auto& damages = component.GetDamageList();
		for (auto iter=damages.begin(); iter!=damages.end();)
		{
			if (iter->IsTimerDone())
			{
				iter = damages.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		if(damages.empty())
		{
			if(component.GetIsComponentDeleted() == false)
			{
				CurrentLevel->DeleteComponentLater(compID, entityID);
			}
		}
	}

	
}


void DamageSystem::Unload()
{
	
}
