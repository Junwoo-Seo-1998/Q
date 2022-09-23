/*
	File Name: RayTriggerEvent.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../Game/System/EnemyStateSystem.h"
#include "../Core/Event.h"
#include "Engine/Types.h"

namespace q_engine
{
	class RayTriggerEvent final : public Event<RayTriggerEvent, EnemyStateSystem>
	{
	public:
		EntityInfo rayOwner;
		EntityInfo rayColided;
		int rayIndex;

		RayTriggerEvent(EntityInfo rayOwner, EntityInfo rayColided, int rayIndex) : rayOwner(rayOwner), rayColided(rayColided), rayIndex(rayIndex) {}
	};
}
