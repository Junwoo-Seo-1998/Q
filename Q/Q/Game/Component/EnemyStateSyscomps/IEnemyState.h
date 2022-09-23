/*
	File Name: IEnemyState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>
#include <glm/vec2.hpp>


#include "../Engine/ECS/IDGenerator.h"
#include "Engine/Types.h"
#include "Engine/Core/RandomMachine.h"

namespace q_engine
{
	class IEvent;
	class GameState;
	class TriggerEvent;
	class RayTriggerEvent;
}


template<typename T>
class IEnemyState
{
public:

	inline static q_engine::IDType StateID = q_engine::IDGenerator::GenerateTypeID();
	static std::string StateName; //for Debugging purpose
	
public:
	
	static q_engine::IDType GetStateID() { return StateID; }
	
	static void Load(q_engine::GameState* , q_engine::IDType) {}
	static void Update(q_engine::GameState* , q_engine::IDType,  double) {}
	static void Unload(q_engine::GameState* , q_engine::IDType) {}

	static void OnTriggerEvent(q_engine::GameState*, q_engine::IDType, q_engine::TriggerEvent*) {}
	static void OnRayEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*) {}
	static void OnAttackEvent(q_engine::GameState*, q_engine::IDType) {}
};
