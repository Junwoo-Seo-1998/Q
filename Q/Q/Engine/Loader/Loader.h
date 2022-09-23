/*
	File Name: Loader.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <functional>
#include <string>
#include <unordered_map>


#include "../ECS/Component.h"
#include "../ECS/Entity.h"
#include "Engine/Types.h"

namespace q_engine
{
	class GameState;
	class TileMapComponent;
	class Loader
	{
	public:
		Loader(GameState* scene):scene(scene) {}

		template<typename Entity>
		void RegisterEntityTypeFunction(const std::string& To_Compare, IDType enttTypeID, const std::string& enttName);
		void RegisterEntities(IDType enttTypeID, const std::string& enttName);
		
		
		template<typename Component, typename... Args>
		void RegisterComponentFunction(const std::string& To_Compare, IDType compID, const std::string& compName, Args... args);
		void RegisterComponents(IDType compID, const std::string& compName);
		
		void LoadScene(const std::string& filePath);
		void LoadTileMap(TileMapComponent* tileMapComp);


		static IDType GetLoadedIDWithSavedID(int SavedID);
	private:
		GameState* scene;
		std::unordered_map<IDType, std::function<BaseEntity* (const std::string&)>> CreateEntityFunctions;
		std::unordered_map<IDType, std::function<BaseEntity* (IDType)>> GetEntityByTypeFunctions;
		std::unordered_map<IDType, std::function<IComponent* (BaseEntity*)>> CreateComponentFunctions;
		static std::unordered_map<int, IDType> SavedIDLoadedIDMaps;
	};

	template <typename Entity>
	void Loader::RegisterEntityTypeFunction(const std::string& To_Compare, IDType enttTypeID,
		const std::string& enttName)
	{
		if(enttName==To_Compare)
		{
			CreateEntityFunctions[enttTypeID] = [&](const std::string& tag)->BaseEntity*
			{
				Entity* entt = scene->AddEntity<Entity>(tag);
				
				return entt;
			};
			GetEntityByTypeFunctions[enttTypeID] = [&](IDType entt_ID)->BaseEntity*
			{
				Entity* entt = &scene->GetEntities<Entity>()[entt_ID];
				return entt;
			};
		}
	}

	template <typename Component, typename... Args>
	void Loader::RegisterComponentFunction(const std::string& To_Compare,IDType compID, const std::string& compName, Args... args)
	{
		if (compName == To_Compare)
		{
			CreateComponentFunctions[compID] = [&](BaseEntity* entity)->IComponent*
			{
				return scene->AddComponent<Component>(entity, std::forward<Args>(args)...);
			};
		}
		
	}
}
