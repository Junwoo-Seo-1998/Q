/*
	File Name: SystemManager.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <vector>
#include <memory>
#include "../Core/ContiguousHashMap.h"
#include "System.h"
#include "Engine/Types.h"
namespace q_engine
{
	class IEvent;
	class GameState;
	class SystemManager
	{
	private:
		ContiguousHashMap<std::shared_ptr<ISystem>> mSystems;
		IDType mLastTypeID{ 0 };
	public:
		template <typename T, typename... Args>
		void AddSystem(Args... args); // you should add proper component separately
		template <typename T>
		T& GetSystem();

		void Load();
		void Update(double deltaTime);
		void Unload();

		template <typename T>
		void NotifyEvent(IEvent* event);
	};
}

#include "SystemManager.inl"