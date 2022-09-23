/*
	File Name: RandomMachine.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../EngineComponent.h"
#include <random>
#include <type_traits>

namespace q_engine
{
	class RandomMachine : public EngineComponent<RandomMachine>
	{
		std::random_device randomdevice;
		std::mt19937 machine;

	public:
		RandomMachine();
		RandomMachine(const RandomMachine& other);
		RandomMachine& operator=(const RandomMachine& other);
		~RandomMachine() = default;


		template <typename T>
		std::enable_if_t<std::is_floating_point_v<T>, T> GetRandomValue(T min, T max);
		template <typename T>
		std::enable_if_t<std::is_integral_v<T>, T> GetRandomValue(T min, T max);
	};
}

#include "RandomMachine.inl"