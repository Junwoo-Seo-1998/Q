/*
	File Name: BombExplodeSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BombExplodeSystem.h"


#include "../EObjectZDepth.h"
#include "../../Engine/System/TimerManagerSystem.h"
#include "../../Engine/Event/TriggerEvent.h"
#include "../../Engine/Component/AnimationComponent.h"
#include "../../Engine/Component/WidthHeightComponent.h"
#include "../../Engine/Component/CameraShakeComponent.h"
#include "../../Engine/Component/CollisionComponent.h"
#include "../../Engine/Component/GravityComponent.h"
#include "../../Engine/Component/SoundComponent.h"
#include "../../Engine/Component/VelocityComponent.h"
#include "../../Engine/Core/StringHash.h"

#include "../Entity/BombEntity.h"
#include "../Entity/MapEntity.h"
#include "../Entity/PhysicsTestEntities.h"
#include "Engine/Graphics/Particle.h"

void BombExplodeSystem::OnEvent(q_engine::IEvent* event)
{
	using namespace q_engine;
	
	if (TriggerEvent* triggerEvent = dynamic_cast<TriggerEvent*>(event); triggerEvent)
	{
		const IDType triggerEntityTypeID = triggerEvent->mTriggerEntityTypeID;
		const IDType collideEntityTypeID = triggerEvent->mCollideWithEntityTypeID;
		
		if (triggerEntityTypeID == q_engine::GetTypeID<BombEntity>() &&
			(collideEntityTypeID == q_engine::GetTypeID<MapEntity>() || collideEntityTypeID == q_engine::GetTypeID<EnemyEntity>()))
		{
			const IDType bombID = triggerEvent->mTriggerEntityID;
			if (bombID == mCollidedEntityID)
			{
				return;
			}

			mCollidedEntityID = bombID;
			
			GameState* const currentLevel = m_controlled_state;
			
			AnimationComponent& animation = currentLevel->GetComponents<AnimationComponent>()[bombID];
			animation.SetAnimation("BombExplodeAnim"_hashID, static_cast<int>(EObjectZDepth::ITEM));
			
			TimerID timerID = TimerManagerSystem::IssueTimer(
				animation.GetLength(), true,
				[=]()
				{
					currentLevel->DeleteEntityComponentsLater(triggerEntityTypeID, bombID);
					TimerManagerSystem::YieldTimer(timerID);
				}
			);
			auto& particleSystem = m_controlled_state->GetUpdateSystem<q_engine::ParticleSystem>();
			TransformComponent& transform = currentLevel->GetComponents<TransformComponent>()[bombID];
			WidthHeightComponent& width_height = currentLevel->GetComponents<WidthHeightComponent>()[bombID];
			glm::vec2 pos = { transform.GetPositionX(),transform.GetPositionY() };
			glm::vec2 half_width_height = { width_height.GetWidthHeight().x/2.f,width_height.GetWidthHeight().y/2.f };
			particleSystem.Find("Boom")->Emit(100, pos+half_width_height, glm::vec2{ 0.f,50.f }, glm::vec2{-100.f,100.f},
				glm::vec2{ 100.f, 200.f }, glm::vec2{ 3.f, 5.f }, glm::vec2{3.f,6.f}, 300.f);

			currentLevel->DeleteComponent<RectCollisionComponent>(bombID);
			currentLevel->DeleteComponent<GravityComponent>(bombID);
			currentLevel->DeleteComponent<VelocityComponent>(bombID);

			
			Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("bomb-explosion",false);
			
			currentLevel->GetComponents<CameraShakeComponent>().Front().AddTraumaLevel(0.5f);
		}
	}
}
