/*
	File Name: OffsetSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../GameState.h"
#include "../ECS/System.h"
namespace q_engine
{

	class OffsetSystem : public System<OffsetSystem>
	{
	public:
		OffsetSystem(GameState* state) : System<q_engine::OffsetSystem>(state) {};
		void Load() override;
		void Update(double dt) noexcept override;
		void Unload() override {};
	};
}