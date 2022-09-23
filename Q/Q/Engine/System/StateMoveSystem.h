/*
	File Name: StateMoveSystem.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../ECS/System.h"
#include "Engine/Core/Event.h"
#include "Engine/GameState.h"
#include "../Component/StateMoveComponent.h"

namespace q_engine
{
	class StateMoveSystem : public System<StateMoveSystem>
	{
	public:
		StateMoveSystem(GameState* state) : System<StateMoveSystem>(state) {}
		void OnEvent(IEvent* event) override;
	};
}
