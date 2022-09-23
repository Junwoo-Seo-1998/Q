/*
	File Name: EngineComponent.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

namespace q_engine
{
	class IEngineComponent
	{
	public:
		virtual void Load() {}
		virtual void Update([[maybe_unused]] double deltaTime) {}
		virtual void Unload() {}
		virtual ~IEngineComponent() = default;
	};

	class Engine;

	template <typename T>
	class EngineComponent : public IEngineComponent
	{
		static int componentID;
	public:
		EngineComponent() = default;
		~EngineComponent() = default;

		static int GetComponentID() { return componentID; }

		friend Engine;
	};

	template <typename T>
	int EngineComponent<T>::componentID = 0;
}
