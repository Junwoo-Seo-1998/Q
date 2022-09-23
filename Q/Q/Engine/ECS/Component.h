/*
	File Name: Component.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "IDGenerator.h"
#include <string>
#include <vector>
#include "jsoncpp/json.h"

namespace q_engine
{
	class IComponent
	{
	public:
		virtual Json::Value Serialize() { return Json::Value(); }
		virtual void Deserialize(Json::Value& /*jsonValue*/) { }
		virtual void OnComponentCreation() {}
		virtual void Unload() {}
	};

	template <typename T>
	class Component : public IComponent
	{
	private:
		inline static std::string StateName;
		inline static IDType componentTypeID = TypeIDGenerator<T>::GetValue();
		IDType mEntityID = INVALID_ID;
		IDType mEntityTypeID = INVALID_ID;

	protected:
		static std::string mComponentName;

	public:
		static IDType GetTypeID() { return componentTypeID; }
		[[nodiscard]] IDType GetEntityID() const { return mEntityID; }
		[[nodiscard]] IDType GetEntityTypeID() const { return mEntityTypeID; }
		
		friend class ComponentManager;
	};
}