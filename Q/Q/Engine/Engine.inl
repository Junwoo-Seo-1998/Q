/*
	File Name: Engine.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include  "Engine.h"
#include <memory>

namespace q_engine
{
	template <typename T>
		void Engine::AddEngineComponent()
	{
		static_assert(std::is_base_of<IEngineComponent, T>::value, "T is not component");
		if (T::GetComponentID() == 0)
		{
			++LastEngineCompID;
			T::componentID = LastEngineCompID;
		}
		engineComponents.insert({ T::GetComponentID(), std::make_shared<T>() });
	}

	template <typename T>
	T& Engine::GetEngineComponent()
	{
		static_assert(std::is_base_of<IEngineComponent, T>::value, "T is not component");
		return *(std::static_pointer_cast<T>(engineComponents[T::GetComponentID()]));
	}
}