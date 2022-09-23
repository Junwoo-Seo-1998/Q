/*
	File Name: DeathitemDropComponent.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/Component.h"

class DeathItemDropComponent : public q_engine::Component<DeathItemDropComponent>
{
public:
	enum class ItemType
	{
		Potion,
		Bomb,
		PoisonBomb,
		ItemNum
	};
};