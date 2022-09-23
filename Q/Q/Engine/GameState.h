/*
	File Name: GameState.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary: Minjae Kyung, Junwoo Seo
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Debugger/Debugger.h"
#include "ECS/ComponentManager.h"
#include "ECS/SystemManager.h"
#include "ECS/EntityManager.h"
#include "Component/TagComponent.h"
#include "Engine/Types.h"
#include "GameStateTypes.h"

namespace q_engine
{
	class IGameState
	{
	public:
		virtual void Load() = 0;
		virtual void Update(double deltaTime) = 0;
		virtual void Draw(double deltaTime) = 0;
		virtual void Unload() = 0;
		virtual void DeleteEntities() = 0;
	};

	class GameState : public IGameState
	{
		friend class GameStateManager;
	private:
		EntityManager mEntityManager;
		SystemManager mUpdateSystemManager;
		SystemManager mDrawSystemManager;
		ComponentManager mComponentManager;

		void SetStateType(GameStateType type) { mType = type; }

	protected:
		bool m_is_transparent = true;
		GameStateType mType = GameStateType::COUNT;
	public:
		GameStateType GetStateType() const { return mType; }

		template<typename T>
		[[nodiscard]] T* AddEntity([[maybe_unused]] const std::string& name="")
		{
			T *entt=mEntityManager.AddEntity<T>(this);
#ifdef QEDITOR
			AddComponent<TagComponent>(entt, name); //to identify name in editor
#endif
			return entt;
			
		} // you SHOULD add args separately

		std::unordered_map<IDType, BaseEntity>& GetAllEntities() { return mEntityManager.GetAllEntities(); }
		
		template<typename T>
		ContiguousHashMap<T>& GetEntities() { return mEntityManager.GetEntities<T>(); }

		template<typename T, typename Ent, typename... Args>
		IComponent* AddComponent(Ent* entity, Args... args)
		{
			static_assert(std::is_constructible_v<T, Args...>, "Cannot Make Component with provided arguments");
		    return mComponentManager.AddComponent<T>(entity, std::forward<Args>(args)...);

		}
		template<typename T, typename... Args>
		IComponent* AddComponent(IDType entt_type, IDType entt_ID, Args... args)
		{
			static_assert(std::is_constructible_v<T, Args...>, "Cannot Make Component with provided arguments");
			return mComponentManager.AddComponent<T>(entt_type, entt_ID, std::forward<Args>(args)...);

		}
		
		template<typename T>
		bool IsEntityTypeExist() { return mEntityManager.IsEntityTypeExist<T>(); }
		
		template<typename T>
		bool HasComponent(IDType entityID) { return mComponentManager.HasComponent<T>(entityID); }

		bool HasComponent(IDType typeID, IDType entityID) { return mComponentManager.HasComponent(typeID, entityID); };
		

		std::unordered_multimap<IDType, IDType>& GetAllComponentInfo() { return mComponentManager.GetAllComponentInfo(); }
		
		template<typename T>
		ContiguousHashMap<T>& GetComponents() { return mComponentManager.GetComponents<T>(); }

		template<typename T>
		T& GetComponentWithID(IDType entityID) { return mComponentManager.GetComponents<T>()[entityID]; }
		
		
		void DeleteEntityComponents(IDType entityID)
		{
			mComponentManager.DeleteComponents(entityID);
			mEntityManager.DeleteEntity(entityID);
		}
		

		void DeleteEntityLater(IDType typeID,IDType entityID)
		{
			mEntityManager.DeleteEntityLater(typeID, entityID);
		}
		void DeleteComponentsLater(IDType entityID)
		{
			mComponentManager.DeleteComponentsLater(entityID);
		}
		void DeleteComponentLater(IDType typeID , IDType entityID)
		{
			mComponentManager.DeleteComponentLater(typeID,entityID);
		}
		void DeleteEntityComponentsLater(IDType typeID, IDType entityID)
		{
			DeleteEntityLater(typeID, entityID);
			DeleteComponentsLater(entityID);
		}
		template<typename T>
		void DeleteComponent(IDType entityID);
		
		template<typename T, typename... Args>
		void AddUpdateSystem(Args... args) { mUpdateSystemManager.AddSystem<T>(this, args...); }
		template<typename T, typename... Args>
		void AddDrawSystem(Args... args) { mDrawSystemManager.AddSystem<T>(this, args...); }

		template<typename T>
		T& GetUpdateSystem() { return mUpdateSystemManager.GetSystem<T>(); }

		template<typename T>
		T& GetDrawSystem() { return mDrawSystemManager.GetSystem<T>(); }

		void Load() override
		{
		    mUpdateSystemManager.Load();
			mDrawSystemManager.Load();
		}

		void Update(const double deltaTime) override { mUpdateSystemManager.Update(deltaTime); };
		void Draw(const double deltaTime) override { mDrawSystemManager.Update(deltaTime); }
		void Unload() override
		{
#ifndef QEDITOR
			Debugger::GetEntityDebuggerManager().Clear();
#endif

			mEntityManager.Unload();
			mComponentManager.Unload();
		    mUpdateSystemManager.Unload();
			mDrawSystemManager.Unload();
		}
		bool IsTransParent() const { return m_is_transparent; }

		void DeleteEntities() override
		{
			mComponentManager.ClearComponentToDelete();
			mEntityManager.ClearEntityToDelete();
		}

	};

	template <typename T>
	void GameState::DeleteComponent(IDType entityID)
	{
		mComponentManager.DeleteComponent<T>(entityID);
	}

	enum class TileType : char
	{
		Null,
		Normal,
		OneWay,
		Breakable,
		Ladder,
	};
	

	enum class MapLevel
	{
		Start = -1,
		Tutorial,
		Level1,
		Level2,
		Level3,
		Level4,
		Level5,
		Level6,
		Level7,
		BossMap,
		BossMap2,
		Count
	};

	using vec2Int = glm::vec<2, int>;
	
	class Level : public GameState
	{
	public:
		struct MapData
		{
			std::vector<TileType> mTiles;
			vec2Int mOffset;
			vec2Int mTileSize;
			vec2Int mTileNum;
		};
	protected:
		MapData mMapData;
		MapLevel mLevelNum;
	public:
		void Load() override;
		void SetLevel(MapLevel levelNum);
		MapLevel GetLevelNum()const;
		TileType GetTileByCoord(float x, float y);
		TileType GetTileByCoord(glm::vec2 coord);
		TileType GetTileByIndex(int x, int y);
		TileType GetTileByIndex(vec2Int index);
		glm::vec<2, int> GetTileIndexByCoord(glm::vec2 coord);

		const MapData& GetMapData();
	};



	class UIState : public GameState
	{
	};
}
