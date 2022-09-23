/*
	File Name: LuaColliderDispatcherSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "LuaColliderDispatcherSystem.h"
#include "../Engine/Event/TriggerEvent.h"
#include "../Game/Entity/ProjectileEntity.h"
#include "Component/ProjectileScriptComponent.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "Entity/MapEntity.h"

namespace q_engine
{
	void LuaColliderDispatcherSystem::OnEvent(IEvent* eventName)
	{
		if(TriggerEvent* triggerEvent = dynamic_cast<TriggerEvent*>(eventName); triggerEvent)
		{
			
			const IDType TriggerEntityTypeID = triggerEvent->mTriggerEntityTypeID;
			const IDType CollideWithEntityTypeID = triggerEvent->mCollideWithEntityTypeID;
			
			
			
			if(TriggerEntityTypeID == q_engine::GetTypeID<ProjectileEntity>())
			{
				if(CollideWithEntityTypeID == q_engine::GetTypeID<EnemyEntity>() || CollideWithEntityTypeID == q_engine::GetTypeID<MapEntity>())
				{
					const IDType TriggerEntityID = triggerEvent->mTriggerEntityID;
					const IDType CollidewithEntityID = triggerEvent->mCollideWithEntityID;
					auto& lua_comp = m_controlled_state->GetComponents<ProjectileScriptComponent>()[TriggerEntityID];
					lua_comp.OnCollideWith(CollideWithEntityTypeID, CollidewithEntityID);
				}


				
			}
			
		}

		
	}
}
