/*
	File Name: EntityComponentMover.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <tuple>
#include "../Game/Entity/PhysicsTestEntities.h"
#include "EntityManager.h"
#include "ComponentManager.h"

namespace q_engine
{
	template <typename... Ents>
	class IEntityComponentMover
	{
		template <typename T, typename... Comps>
		void MoveComponent(std::tuple<Comps...>, ContiguousHashMap<T>& entityList, ComponentManager& from, ComponentManager& target);
		
	public:
		using Entitytypes = std::tuple<Ents...>;

		void MoveEntities(EntityManager& from, EntityManager& target);
		void MoveComponents(EntityManager& TargetEntityList, ComponentManager& from, ComponentManager& target);
	};


	
	class EntityComponentMover : public IEntityComponentMover<PlayerEntity>
	{
	public:
	};

}

#include "EntityComponentMover.inl"
