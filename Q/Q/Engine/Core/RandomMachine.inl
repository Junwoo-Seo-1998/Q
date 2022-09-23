/*
	File Name: RandomMachine.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "RandomMachine.h"


namespace q_engine
{
	template <typename T>
	std::enable_if_t<std::is_floating_point_v<T>, T> RandomMachine::GetRandomValue(T min, T max)
	{
		std::uniform_real_distribution<T> distribution(min, max);
		return distribution(machine);
	}
	template <typename T>
	std::enable_if_t<std::is_integral_v<T>, T> RandomMachine::GetRandomValue(T min, T max)
	{
		std::uniform_int_distribution<T> distribution(min, max);
		return distribution(machine);
	}
}
