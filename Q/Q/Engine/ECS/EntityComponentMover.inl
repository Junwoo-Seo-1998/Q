/*
	File Name: EntityComponentMover.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

namespace q_engine
{
	template <typename ... Ents>
	void q_engine::IEntityComponentMover<Ents...>::MoveEntities(EntityManager& from, EntityManager& target)
	{
		if constexpr (sizeof...(Ents) == 0)
		{
			return;
		}
		else
		{
			using dummy = int[];
			(void)dummy
			{
				(target.AddEntities<Ents>(std::reinterpret_pointer_cast<ContiguousHashMap<Ents>>(from.mEntityArrays[q_engine::TypeIDGenerator<Ents>::GetValue()])), 0)...
			};
		}
	}

	template <typename... Ents>
	template <typename T, typename... Comps>
	void IEntityComponentMover<Ents...>::MoveComponent(std::tuple<Comps...>, ContiguousHashMap<T>& targetEntityList, ComponentManager& from, ComponentManager& target)
	{
		if constexpr (sizeof...(Comps) == 0)
		{
			return;
		}
		else
		{
			for (T& idx : targetEntityList)
			{
				using dummy = int[];
				(void)dummy
				{
					(target.AddComponent<Comps>(&idx, from.GetComponents<Comps>()[idx.GetEntityID()]), 0)...
				};
			}
		}

	}

	template <typename... Ents>
	void q_engine::IEntityComponentMover<Ents...>::MoveComponents(EntityManager& targetEntityList, ComponentManager& from, ComponentManager& target)
	{
		if constexpr (sizeof...(Ents) == 0)
		{
			return;
		}
		else
		{
			using dummy = int[];
			(void)dummy
			{
				(MoveComponent(Ents::ComponentList(), targetEntityList.GetEntities<Ents>(), from, target), 0)...
			};
		}
	}
}
