/*
	File Name: LuaColliderDispatcherSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include "../Engine/ECS/System.h"

namespace  q_engine
{
	class LuaColliderDispatcherSystem final : public System<LuaColliderDispatcherSystem>
	{
	public:
		LuaColliderDispatcherSystem() = delete;
		LuaColliderDispatcherSystem(GameState* state) : System<LuaColliderDispatcherSystem>(state) {}
		void OnEvent(IEvent* eventName) override;
	private:
		

	};

	
	
}