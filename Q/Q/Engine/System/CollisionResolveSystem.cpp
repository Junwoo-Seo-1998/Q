/*
	File Name: CollisionResolveSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "CollisionResolveSystem.h"


#include "../GameStateManager.h"
#include "../../Game/Entity/PhysicsTestEntities.h"
#include "../../Game/Entity/WeaponEntity.h"
#include "../Component/CollisionComponent.h"
#include "../Component/GravityComponent.h"
#include "../Component/ObjectStateComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/VelocityComponent.h"
#include "../../Engine/Core/EventManager.h"
#include "../Core/ContiguousHashMap.h"
#include "../Core/Rect.h"
#include "../Debugger/Debugger.h"
#include "../Event/RayTriggerEvent.h"
#include "glm/glm.hpp"
#include "../Engine/Engine.h"
#include "Component/HealthComponent.h"
#include "Engine/Event/TriggerEvent.h"
#include "Entity/BombEntity.h"
#include "Engine/System/CollisionSystem.h"
#include "Entity/ItemEntity.h"
#include "Entity/MapEntity.h"
#include "Entity/ProjectileEntity.h"

namespace q_engine
{
	void CollisionResolveSystem::Load()
	{
		
	}

	

	void CollisionResolveSystem::Update([[maybe_unused]] double dt) noexcept
	{
		FUNCTION_PROFILING();
		auto& RectCollisionContainer = m_controlled_state->GetComponents<RectCollisionComponent>();
		auto& StaticCollsionContainer = m_controlled_state->GetComponents<StaticRectCollisionComponent>();
		
		for(RectCollisionComponent& ComponentA : RectCollisionContainer)
		{
			if(ComponentA.GetDynamicResolvingEntityData().size() ==0)
			{
				continue;
			}
			ComponentA.GetDynamicResolvingEntityData().clear();
		}

		auto& RayCollisionContainer = m_controlled_state->GetComponents<RaycastComponent>();

		
		for(RaycastComponent& ComponentA : RayCollisionContainer)
		{
			const int RayShootingsize = ComponentA.GetRaySize();

			for(int collisionindex =0; collisionindex < RayShootingsize; collisionindex++)
			{
				if(ComponentA.GetIntersectPoints().at(collisionindex).second == true)
				{
					RayResolveUpdate(ComponentA, collisionindex);
				}
			}
		}
		// new
		for (RectCollisionComponent& ComponentA : RectCollisionContainer)
		{
			if (ComponentA.GetResolvingEntityData().size() == 0)
			{
				continue;
			}
			for (IDType ResolvedID : ComponentA.GetResolvingEntityData())
			{
				StaticRectCollisionComponent& ComponentB = StaticCollsionContainer[ResolvedID];
				RectStaticResolveUpdate(ComponentA, ComponentB);
			}
			ComponentA.GetResolvingEntityData().clear();
		}
		
	
	
		
		
	}

	void CollisionResolveSystem::Unload()
	{
		
	}

	void CollisionResolveSystem::RectResolveUpdate(RectCollisionComponent& componentA, RectCollisionComponent& componentB)
	{
		const int ResolveEntityType = componentA.GetEntityTypeID();
		const int Player = q_engine::GetTypeID<PlayerEntity>();
		const int Enemy = q_engine::GetTypeID<EnemyEntity>();
		const int Weapon = q_engine::GetTypeID<WeaponEntity>();


		
		if(ResolveEntityType == Player || ResolveEntityType == Enemy)
		{
			Rect RectA = componentA.GetColliderBox();
			Rect OldRectA = componentA.GetOldColliderBox();
			Rect RectB = componentB.GetColliderBox();
			Rect OldRectB = componentB.GetOldColliderBox();
			auto compAEntityID = componentA.GetEntityID();
			auto& compATransform = m_controlled_state->GetComponents<TransformComponent>()[compAEntityID];
			auto& compAVelocity = m_controlled_state->GetComponents<VelocityComponent>()[compAEntityID];
			auto& compAGravity = m_controlled_state->GetComponents<GravityComponent>()[compAEntityID];

			if (RectA.Bottom() <= RectB.Top() && OldRectA.Bottom() >= OldRectB.Top())
			{
				componentA.SetColliderPositionY(RectB.Top());
				compATransform.SetPositionY(RectB.Top());
				compAVelocity.SetVelocityY(0.f);
				compAGravity.SetIsOnGround(true);
			}
			else if (RectA.Top() > RectB.Bottom() && OldRectA.Top() <= OldRectB.Bottom())
			{
				componentA.SetColliderPositionY(RectB.Bottom() - RectA.Size().y);
				compATransform.SetPositionY(RectB.Bottom() - RectA.Size().y);
				compAVelocity.SetVelocityY(0.f);
			}
			else if (RectA.Left() < RectB.Right() && OldRectA.Left() >= OldRectB.Right())
			{
				componentA.SetColliderPositionX(RectB.Right() + GetCorrectValue());
				compATransform.SetPositionX(RectB.Right() + GetCorrectValue());
				compAVelocity.SetVelocityX(0.f);
			}
			else if (RectA.Right() > RectB.Left() && OldRectA.Right() <= OldRectB.Left())
			{
				componentA.SetColliderPositionX(RectB.Left() - RectA.Size().x - GetCorrectValue());
				compATransform.SetPositionX(RectB.Left() - RectA.Size().x - GetCorrectValue());
				compAVelocity.SetVelocityX(0.f);
			}else
			{
				compAGravity.SetIsOnGround(false);
			}

			compATransform.SetPosition(componentA.GetColliderBox().Left(), componentA.GetColliderBox().Bottom());

		}
		else if(ResolveEntityType == Weapon)
		{

			

			
		}
	
	
		
	}
	void CollisionResolveSystem::RectStaticResolveUpdate(RectCollisionComponent& componentA, StaticRectCollisionComponent& componentB)
	{
		const IDType ResolveEntityType = componentA.GetEntityTypeID();
		const IDType Player = q_engine::GetTypeID<PlayerEntity>();
		const IDType Enemy = q_engine::GetTypeID<EnemyEntity>();
		const IDType Weapon = q_engine::GetTypeID<WeaponEntity>();
		const IDType Projectile = q_engine::GetTypeID<ProjectileEntity>();
		const IDType Portaltile = q_engine::GetTypeID<PortalEntity>();
		const IDType Item = q_engine::GetTypeID<ItemEntity>();
		const IDType Cube = q_engine::GetTypeID<CubeEntity>();
		if (ResolveEntityType == Projectile)
		{
			Rect RectA = componentA.GetColliderBox();
			Rect OldRectA = componentA.GetOldColliderBox();
			Rect RectB = componentB.GetColliderBox();
			Rect OldRectB = componentB.GetColliderBox();
			auto compAEntityID = componentA.GetEntityID();
			auto& compATransform = m_controlled_state->GetComponents<TransformComponent>()[compAEntityID];
			auto& compAVelocity = m_controlled_state->GetComponents<VelocityComponent>()[compAEntityID];

				if(!componentB.IsLadder())
				if (RectA.Bottom() <= RectB.Top() && OldRectA.Bottom() >= OldRectB.Top())
				{
					componentA.SetColliderPositionY(RectB.Top());
					compATransform.SetPositionY(RectB.Top()); 
				}
				else if (RectA.Top() > RectB.Bottom() && OldRectA.Top() <= OldRectB.Bottom())
				{
						componentA.SetColliderPositionY(RectB.Bottom() - RectA.Size().y);
						compATransform.SetPositionY(RectB.Bottom() - RectA.Size().y);
						compAVelocity.SetVelocityX(0.f);
				}
				else if (RectA.Left() < RectB.Right() && OldRectA.Left() >= OldRectB.Right())
				{
						componentA.SetColliderPositionX(RectB.Right() + GetCorrectValue());
						compATransform.SetPositionX(RectB.Right() + GetCorrectValue());
				}
				else if (RectA.Right() > RectB.Left() && OldRectA.Right() <= OldRectB.Left())
				{
						componentA.SetColliderPositionX(RectB.Left() - RectA.Size().x - GetCorrectValue());
						compATransform.SetPositionX(RectB.Left() - RectA.Size().x - GetCorrectValue());
						compAVelocity.SetVelocityX(0.f);
				}
			
			
			compATransform.SetPosition(componentA.GetColliderBox().Left(), componentA.GetColliderBox().Bottom());

			
			

			
		}


		
		if (ResolveEntityType == Player || ResolveEntityType == Enemy || ResolveEntityType == Portaltile || ResolveEntityType == Item || ResolveEntityType == Cube)
		{
			Rect RectA = componentA.GetColliderBox();
			Rect OldRectA = componentA.GetOldColliderBox();
			Rect RectB = componentB.GetColliderBox();
			Rect OldRectB = componentB.GetColliderBox();
			auto compAEntityID = componentA.GetEntityID();
			auto& compATransform = m_controlled_state->GetComponents<TransformComponent>()[compAEntityID];
			auto& compAVelocity = m_controlled_state->GetComponents<VelocityComponent>()[compAEntityID];
			
			if (!m_controlled_state->GetComponents<GravityComponent>().isExist(compAEntityID))
				return;
			auto& compAGravity = m_controlled_state->GetComponents<GravityComponent>()[compAEntityID];
			
			if(componentB.IsLadder())
			{
				if(RectA.Bottom() <= RectB.Top() && OldRectA.Bottom() >= OldRectB.Top())
				{
				
					compATransform.SetPositionY(RectB.Top());
				}
			}
			else
			if (RectA.Bottom() <= RectB.Top() && OldRectA.Bottom() >= OldRectB.Top())
			{
				componentA.SetColliderPositionY(RectB.Top());
				compATransform.SetPositionY(RectB.Top());
				compAVelocity.SetVelocityY(0.f);
				compAGravity.SetIsOnGround(true);

			}
			else if (RectA.Top() > RectB.Bottom() && OldRectA.Top() <= OldRectB.Bottom())
			{
				if (componentB.IsTileOneWay() == false)
				{
					componentA.SetColliderPositionY(RectB.Bottom() - RectA.Size().y);
					compATransform.SetPositionY(RectB.Bottom() - RectA.Size().y);
					compAVelocity.SetVelocityY(0.f);
			
				}
			}
			else if (RectA.Left() < RectB.Right() && OldRectA.Left() >= OldRectB.Right())
			{
				if (componentB.IsTileOneWay() == false)
				{
					componentA.SetColliderPositionX(RectB.Right() + GetCorrectValue());
					compATransform.SetPositionX(RectB.Right() + GetCorrectValue());
	
					compAVelocity.SetVelocityX(0.f);
				}
			}
			else if (RectA.Right() > RectB.Left() && OldRectA.Right() <= OldRectB.Left())
			{
				if (componentB.IsTileOneWay() == false)
				{
					componentA.SetColliderPositionX(RectB.Left() - RectA.Size().x - GetCorrectValue());
					compATransform.SetPositionX(RectB.Left() - RectA.Size().x - GetCorrectValue());
			
					compAVelocity.SetVelocityX(0.f);
				}
			}
			else
			{
				compAGravity.SetIsOnGround(false);
			}
			if(compAGravity.GetIsOnground() == true)
			{
			}
			if(ResolveEntityType == Player)
			{
				if (componentB.IsCanDestroyed() == true)
				{
					auto& objectstate = m_controlled_state->GetComponents<ObjectStateComponent>()[componentA.GetEntityID()];
					if(objectstate.GetPlayerMovingState() == MovingState::BEEING)
					{
						if (RectA.Bottom() <= RectB.Top() && OldRectA.Bottom() >= OldRectB.Top())
						{
							m_controlled_state->GetUpdateSystem<CollisionSystem>().GetDeleteStaticCollisionData().push_back(componentB.GetEntityID());
							compAGravity.SetIsOnGround(false);
						}
					}
				}
				
			}


			compATransform.SetPosition(componentA.GetColliderBox().Left(), componentA.GetColliderBox().Bottom());

		}
		else if (ResolveEntityType == Projectile)
		{
			if(m_controlled_state->HasComponent<TagComponent>(componentA.GetEntityID()))
			if(componentB.IsCanDestroyed() == true)
			{
				m_controlled_state->GetUpdateSystem<CollisionSystem>().GetDeleteStaticCollisionData().push_back(componentB.GetEntityID());
			}
		}
		
		if (ResolveEntityType == Item || ResolveEntityType == Cube || ResolveEntityType == Player)
		{
			if (componentB.IsCanDestroyed() == true)
			{
				Rect RectA = componentA.GetColliderBox();
				Rect OldRectA = componentA.GetOldColliderBox();
				Rect RectB = componentB.GetColliderBox();
				Rect OldRectB = componentB.GetColliderBox();
				auto compAEntityID = componentA.GetEntityID();
				if (RectA.Bottom() <= RectB.Top() && OldRectA.Bottom() >= OldRectB.Top())
				{
					for (auto BreakableTile : m_controlled_state->GetUpdateSystem<CollisionSystem>().GetDeleteStaticCollisionData())
					{
						if (BreakableTile == componentB.GetEntityID())
						{
							auto& compAGravity = m_controlled_state->GetComponents<GravityComponent>()[compAEntityID];
							compAGravity.SetIsOnGround(false);
						}
					}
				}
			}

		}
		


	}



#include "../Game/Component/EnemyStateComponent.h"
	
	void CollisionResolveSystem::RayResolveUpdate(RaycastComponent& componentA, int RayIndex)
	{
		IDType CollisionEntityTypeID = componentA.GetRayCollisionEntityTypeEntityID().at(RayIndex).first;
		IDType CollisionEntityID = componentA.GetRayCollisionEntityTypeEntityID().at(RayIndex).second;
		IDType ownerRayEntityID = componentA.GetEntityID();
		IDType ownerRayEntityTypeID = componentA.GetEntityTypeID();
		;
		if (ownerRayEntityTypeID == q_engine::GetTypeID<EnemyEntity>())
		{

			Engine::GetEngineComponent<EventManager>().AddEvent<RayTriggerEvent>(EntityInfo{ ownerRayEntityTypeID,ownerRayEntityID }, 
				EntityInfo{ CollisionEntityTypeID, CollisionEntityID }, RayIndex);
		}
		
	}

	float CollisionResolveSystem::GetCorrectValue() const
	{
		return mCorrectionValue;
	}


	
}
