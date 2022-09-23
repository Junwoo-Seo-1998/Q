/*
	File Name: LambdaEvent.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/Core/Event.h"
#include "Engine/System/LambdaEventSystem.h"

namespace q_engine
{
	class LambdaEvent final : public Event<LambdaEvent, LambdaEventSystem>
	{
	public:
		std::function<void(void)> mFunction;
	public:
		LambdaEvent(std::function<void(void)> func) : mFunction(func) {}
	};
}