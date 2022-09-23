/*
	File Name: RandomMachine.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "RandomMachine.h"
#include <type_traits>

namespace q_engine
{
	RandomMachine::RandomMachine() : machine(randomdevice()) {}
	RandomMachine::RandomMachine(const RandomMachine& other) { machine = other.machine; }
	RandomMachine& RandomMachine::operator=(const RandomMachine& other)
	{
		machine = other.machine;
		return *this;
	}

}