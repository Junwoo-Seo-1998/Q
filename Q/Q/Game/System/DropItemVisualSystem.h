/*
	File Name: DropItemVisualSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"
#include "Engine/Types.h"

class DropItemVisualSystem : public q_engine::System<DropItemVisualSystem>
{
public:
	DropItemVisualSystem(q_engine::GameState* state) : System<DropItemVisualSystem>(state) {}
	void Load() override;
	void Update(double deltaTime) override;
	void Unload() override;

private:
	double time = 0.0;
	
};