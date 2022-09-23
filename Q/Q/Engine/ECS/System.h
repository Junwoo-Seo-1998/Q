/*
	File Name: System.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "IDGenerator.h"
#include "Engine/Types.h"
namespace q_engine
{
	class GameState;
	class ISystem
	{
		virtual void Load() {}
		virtual void Update([[maybe_unused]] double deltaTime) {}
		virtual void Unload() {}

	protected:
		GameState* m_controlled_state;

	public:
		ISystem() = delete;
		ISystem(GameState* state) : m_controlled_state(state) {}
		virtual ~ISystem() { m_controlled_state = nullptr; }
		
		friend class SystemManager;
	};

	class IEvent;
	
	template <typename T>
	class System : public ISystem
	{
	public:
		System() = delete;
	protected:
		inline static IDType typeID = TypeIDGenerator<T>::GetValue();
		
		System(GameState* state) : ISystem(state) {}

		virtual void OnEvent([[maybe_unused]] IEvent* eventName) {}
	public:
		static IDType GetTypeID() { return typeID; }

		friend class SystemManager;
	};
}