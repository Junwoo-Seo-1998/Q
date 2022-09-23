/*
	File Name: Entity.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung, Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "IDGenerator.h"
#include <tuple>
#include "jsoncpp/json.h"

namespace q_engine
{
	struct EntityInfo
	{
		IDType typeID;
		IDType entityID;
		EntityInfo(IDType typeID, IDType entityID) : typeID(typeID), entityID(entityID) {}
	};
	
	class IEntity {};

	template<typename Type>
	IDType GetTypeID() // use this to get entity type id.
	{
		return TypeIDGenerator<Type>::GetValue();
	}
	class GameState;
	
	class BaseEntity
	{
	public:
		[[nodiscard]] IDType GetEntityID() const { return mEntityID; }
		IDType GetRuntimeTypeID() const { return mTypeID; } //used to know derived class type id.
		virtual Json::Value Serialize() { return Json::Value(); };
		virtual void Deserialize([[maybe_unused]] Json::Value& jsonValue) { };
	public:
		template <class Comp, typename ... Args>
		void AddComponent(Args... args) { mScene->AddComponent(this, std::forward<Args>(args)...); }

		template <class Comp>
		bool HasComponent() { return mScene->HasComponent<Comp>(mEntityID); }
		
		template <class Comp>
		Comp& GetComponent() { return mScene->GetComponents<Comp>()[mEntityID]; }

		template <typename Comp>
		void DeleteComponent() { mScene->DeleteComponent<Comp>(mEntityID); }
		
	private:
		IDType mEntityID = INVALID_ID;
		IDType mTypeID = INVALID_ID;
		GameState* mScene;
		friend class EntityManager;
	};

	template <typename T, typename... Components>
	class Entity : public BaseEntity
	{
	public:
		using ComponentList = std::tuple<Components...>;
	

	public:
		//virtual IDType GetRuntimeTypeID() const final { return TypeIDGenerator<T>::GetValue(); }
		
		friend class EntityManager;
	};
}