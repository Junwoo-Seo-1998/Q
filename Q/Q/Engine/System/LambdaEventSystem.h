/*
	File Name: LambdaEventSystem.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#pragma once
#include "../GameState.h"
#include "../ECS/System.h"
namespace q_engine
{

	class LambdaEventSystem : public System<LambdaEventSystem>
	{
	public:
		LambdaEventSystem(GameState* state) : System<q_engine::LambdaEventSystem>(state) {}
		
		void OnEvent(IEvent* eventName) override;
	};


}
