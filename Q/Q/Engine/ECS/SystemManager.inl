/*
	File Name: SystemManager.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Profiler.h"

namespace q_engine
{
	template <typename T, typename... Args>
	void SystemManager::AddSystem(Args... args)
	{
		if(mSystems.find(T::typeID) != mSystems.end())
		{
			// Send Logger
		}

		mSystems.Add(T::typeID, std::make_shared<T>(args...));
	}

	template <typename T>
	T& SystemManager::GetSystem()
	{
		return *(std::reinterpret_pointer_cast<T>(mSystems[T::GetTypeID()]));
	}

	inline void SystemManager::Load()
	{
		for (const auto& idx : mSystems)
		{
			idx->Load();
		}
	}

	inline void SystemManager::Update(double deltaTime)
	{
		FUNCTION_PROFILING();
		for (const auto& idx : mSystems)
		{
			idx->Update(deltaTime);
		}
	}

	inline void SystemManager::Unload()
	{
		for (const auto& idx : mSystems)
		{
			idx->Unload();
		}
		mSystems.Clear();
	}
}
