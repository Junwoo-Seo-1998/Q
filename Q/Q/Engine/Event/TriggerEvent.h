/*
	File Name: TriggerEvent.h
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../Game/System/BombExplodeSystem.h"
#include "../../Game/System/EnemyStateSystem.h"
#include "../../Game/System/EnemyWeaponSystem.h"
#include "../../Game/System/ItemPickUpSystem.h"
#include "../Engine/System/StateMoveSystem.h"
#include "../Core/Event.h"
#include "Engine/System/CollisionResolveSystem.h"
#include "Engine/System/PlayerMovingSystem.h"
#include "Engine/Types.h"
#include "../../Game/System/LuaColliderDispatcherSystem.h"
#include "System/CubePickUpSystem.h"

namespace q_engine
{

	class TriggerEvent final : public Event<TriggerEvent, EnemyStateSystem, BombExplodeSystem, ItemPickUpSystem, StateMoveSystem, EnemyWeaponSystem, MovingSystem, LuaColliderDispatcherSystem, CubePickUpSystem>
	{
	public:
		TriggerEvent(IDType triggerEntityTypeID, IDType triggerEntityID, IDType collideWithEntityTypeID, IDType collideWithEntityID);


	public:
		IDType mTriggerEntityTypeID;
		IDType mTriggerEntityID;
		IDType mCollideWithEntityTypeID;
		IDType mCollideWithEntityID;
	};
}
