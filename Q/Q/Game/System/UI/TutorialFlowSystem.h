#pragma once
/*
	File Name: TutorialFlowSystem.h
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"
#include "Engine/ECS/Entity.h"

enum class TutorialScene
{
	WALK,
	JUMP,
	PICKITEM,
	SWAPWEAPON,
	SLIDE,
	ATTACK,
	PICKBOMB,
	USEITEM,
	SWAPITEM,
	GETPORTAL
};

class TutorialEntity : public q_engine::Entity<TutorialEntity>
{
	
};

class TutorialFlowSystem final : public q_engine::System<TutorialFlowSystem>
{
public:
	TutorialFlowSystem(q_engine::GameState* state) : System<TutorialFlowSystem>(state) {}
	void Load() override;
	void Update(double deltaTime) override;
private:
	TutorialScene CurrScene;
};