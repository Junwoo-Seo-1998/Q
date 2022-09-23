#include "EntityManager.h"
/*
	File Name: EntityManager.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung, Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once


namespace q_engine
{
	template<class Ent>
	ContiguousHashMap<Ent>& EntityManager::GetEntities()
	{
		//static_assert(std::is_base_of_v<IComponent, T>, "T is not Component");
		return *(std::reinterpret_pointer_cast<ContiguousHashMap<Ent>>(mEntityArrays[q_engine::GetTypeID<Ent>()]));
	}

	template<class Ent>
	Ent* EntityManager::AddEntity(GameState* game_state)
	{
		if (mEntityArrays.find(q_engine::GetTypeID<Ent>()) == mEntityArrays.end())
		{
			mEntityArrays.insert({ q_engine::GetTypeID<Ent>(),
				std::reinterpret_pointer_cast<IContiguousHashMap>(std::make_shared<ContiguousHashMap<Ent>>()) });
		}
		Ent temp;
		IDType entt_id= IDGenerator::GenerateEntityID();
		temp.mEntityID = entt_id;
		temp.mTypeID= TypeIDGenerator<Ent>::GetValue();
		temp.mScene = game_state;
		auto iter = std::reinterpret_pointer_cast<ContiguousHashMap<Ent>>(mEntityArrays[q_engine::GetTypeID<Ent>()])
			->Add(entt_id, temp);
		mAllEntities[entt_id] = temp;//for tracking all entts.
		return &(*iter);
	}

	template<class Ent>
	void EntityManager::AddEntities(std::shared_ptr<ContiguousHashMap<Ent>> entities)
	{
		std::shared_ptr< ContiguousHashMap<Ent> > entitiesArr(entities);

		for(auto iter=entitiesArr->begin(); iter!=entitiesArr->end(); ++iter )
		{
			mAllEntities[iter->GetEntityID()]=*iter ;//for tracking all entts.
		}

		
		mEntityArrays.insert({ q_engine::GetTypeID<Ent>(),
				std::reinterpret_pointer_cast<IContiguousHashMap>(entitiesArr) });
	}
	
	template<class Ent>
	void EntityManager::DeleteEntity(IDType entityID)
	{
		mAllEntities.erase(entityID);
		std::reinterpret_pointer_cast<ContiguousHashMap<Ent>>(mEntityArrays[q_engine::GetTypeID<Ent>()])
			->Remove(entityID);
	}

	inline void EntityManager::DeleteEntity(IDType entityID)
	{
		for (auto& idx : mEntityArrays)
		{
			if(idx.second->isExist(entityID))
			{
				mAllEntities.erase(entityID);
				idx.second->Delete(entityID);
			}
		}
	}

	inline void EntityManager::DeleteEntity(IDType typeID, IDType entityID)
	{
		mAllEntities.erase(entityID);
		mEntityArrays[typeID]->Delete(entityID);
	}

	inline void EntityManager::DeleteEntityLater(IDType typeID, IDType entityID)
	{
		mEntitiesToBeDeleted.push(std::tuple<IDType, IDType>{ typeID, entityID });
	}

	inline void EntityManager::ClearEntityToDelete()
	{
		while(!mEntitiesToBeDeleted.empty())
		{
			auto [typeID, entityID] = mEntitiesToBeDeleted.front();
			mEntitiesToBeDeleted.pop();
			DeleteEntity(typeID, entityID);
		}
	}

	template<class Ent>
	inline bool EntityManager::IsEntityTypeExist()
	{
		return mEntityArrays.find(q_engine::GetTypeID<Ent>()) != mEntityArrays.end();
	}

	inline void EntityManager::Unload()
	{
		mAllEntities.clear();
		mEntityArrays.clear();
	}
}