/*
	File Name: EntityManager.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung, Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include <memory>
#include <deque>
#include <queue>

#include "../Core/ContiguousHashMap.h"
#include "../Engine/ECS/Entity.h"
#include "IDGenerator.h"
#include "Engine/Types.h"
namespace q_engine
{
	class EntityManager
	{
	private:
		std::unordered_map< IDType, std::shared_ptr<IContiguousHashMap>> mEntityArrays;
		std::unordered_map<IDType, BaseEntity> mAllEntities;

		std::queue<std::tuple<IDType, IDType>> mEntitiesToBeDeleted;
	public:
		           //for iterating.
		           std::unordered_map<IDType, BaseEntity>::iterator begin() { return mAllEntities.begin(); }
		std::unordered_map<IDType, BaseEntity>::iterator end() { return mAllEntities.end(); }
		
		std::unordered_map<IDType, BaseEntity> ::const_iterator begin() const { return mAllEntities.begin(); }
		std::unordered_map<IDType, BaseEntity>::const_iterator end()	const { return mAllEntities.end(); }
		
	public:
		std::unordered_map<IDType, BaseEntity>& GetAllEntities() { return mAllEntities; }

		template<class Ent>
		ContiguousHashMap<Ent>& GetEntities();

		template<class Ent>
		[[nodiscard]] Ent* AddEntity(GameState* game_state);

		template<class Ent>
		void AddEntities(std::shared_ptr< ContiguousHashMap<Ent>> entities);
		
		template<class Ent>
		void DeleteEntity(IDType entityID);
		
		void DeleteEntity(IDType entityID);

		void DeleteEntity(IDType typeID, IDType entityID);

		//to make entity deleted end of frame
		void DeleteEntityLater(IDType typeID, IDType entityID);
		void ClearEntityToDelete();

		template<class Ent>
		bool IsEntityTypeExist();
		
		inline void Unload();

		template <typename... Args>
		friend class IEntityComponentMover;
	};
}

#include "EntityManager.inl"
