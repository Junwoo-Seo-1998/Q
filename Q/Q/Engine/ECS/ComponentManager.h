/*
	File Name: ComponentManager.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:Junwoo Seo
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <memory>
#include <queue>
#include <unordered_map>
#include <vector>

#include "../Core/ContiguousHashMap.h"
#include "Engine/Types.h"

namespace q_engine
{
	class ComponentManager
	{
	private:
		std::unordered_map< IDType, std::shared_ptr<IContiguousHashMap>> mComponentArrays;
		IDType mLastTypeID{ 0 };
		std::unordered_multimap<IDType, IDType> mComponentInfo;

		std::queue<IDType> mComponentsToBeDeleted;
		std::queue<std::tuple<IDType, IDType>> mSingleComponentToBeDeleted;
	public:
		std::unordered_multimap<IDType, IDType>& GetAllComponentInfo();
		
		template<class Comp>
		ContiguousHashMap<Comp>& GetComponents();

		[[nodiscard]] std::vector<IDType> GetComponentList(IDType entityID);

		template<class Comp, class Ent, typename... Args>
		IComponent* AddComponent(Ent* entityID, Args... args);

		template<class Comp, typename... Args>
		IComponent* AddComponent(IDType entityType,IDType entityID, Args... args);


		template<typename Comp, typename... Args>
		void AddComponentForLua(IDType EntypeID, IDType entityID);
		
		
		template<class Comp>
		bool HasComponent(IDType entityID);

		bool HasComponent(IDType typeID, IDType entityID);
		
		void DeleteComponents(IDType entityID);

		template<class Comp>
		void DeleteComponent(IDType entityID);

		void DeleteComponent(IDType typeID, IDType entityID);
		
		void DeleteComponentLater(IDType typeID, IDType entityID);

		void DeleteComponentsLater(IDType entityID);
		
		void ClearComponentToDelete();
		
		void Unload();

		void MoveComponents(IDType entityTypeID, ComponentManager& target);
	};
}

#include "ComponentManager.inl"

	