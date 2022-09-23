/*
	File Name: PhysicsSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/


#pragma once
#include "../ECS/System.h"


namespace q_engine
{
	class PhysicsSystem :public System<PhysicsSystem>
	{
	public:
		PhysicsSystem() = delete;
		PhysicsSystem(GameState* state) : System<q_engine::PhysicsSystem>(state) {};
		void Load() override;
		void Update(double dt) override;
		void Unload() override {}

		
	private:

	};

}
