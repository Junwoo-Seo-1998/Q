/*
	File Name: StateMoveComponent.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"
#include "../GameState.h"
namespace q_engine
{
	class StateMoveComponent : public Component<StateMoveComponent>
	{
		std::shared_ptr<Level> mToLevel;

	public:
		StateMoveComponent(std::shared_ptr<Level> toLevel) : mToLevel(toLevel) {}
		std::shared_ptr<Level> GetToLevel() { return mToLevel; }
		
	};
}
