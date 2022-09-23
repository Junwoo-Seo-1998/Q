/*
	File Name: Loader.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Loader.h"
#include "Engine/GameState.h"
#include "Engine/Entity/CameraEntity.h"



#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/CameraShakeComponent.h"
#include "Engine/Component/CollisionComponent.h"
#include "Engine/Component/ColorComponent.h"
#include "Engine/Component/DragAccelComponent.h"
#include "Engine/Component/FollowComponent.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Component/LifeTimeComponent.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/RadiusComponent.h"
#include "Engine/Component/RaycastComponent.h"
#include "Engine/Component/TextComponent.h"
#include "Engine/Component/TextureBoxComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/TileMapComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/UserInputComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"



#include "JsonParser/MapJsonParser.h"
#include "jsoncpp/json.h"
#include "Engine/ECS/Entity.h"
#include <iostream>
#include <fstream>



#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"
#include "Entity/BombEntity.h"
#include "Entity/ItemEntity.h"
#include "Entity/JumpTriggerEntity.h"
#include "Entity/PhysicsTestEntities.h"
#include "Entity/WeaponEntity.h"

namespace q_engine
{
	std::unordered_map<int, IDType> Loader::SavedIDLoadedIDMaps;
	//Reg Entities
	void Loader::RegisterEntities(IDType enttTypeID, const std::string& enttName)
	{
		if (CreateEntityFunctions.find(enttTypeID) != CreateEntityFunctions.end())
			return;
		RegisterEntityTypeFunction<BaseEntity>("BaseEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<CameraEntity>("CameraEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<GroundEntity>("GroundEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<BombEntity>("BombEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<ItemEntity>("ItemEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<JumpTriggerEntity>("JumpTriggerEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<WeaponEntity>("WeaponEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<EnemyEntity>("EnemyEntity", enttTypeID, enttName);
		RegisterEntityTypeFunction<EnemyWeaponEntity>("EnemyWeaponEntity", enttTypeID, enttName);
	}

	//Reg Component
	void Loader::RegisterComponents(IDType compID, const std::string& compName)
	{
		if (CreateComponentFunctions.find(compID) != CreateComponentFunctions.end())
			return;

		RegisterComponentFunction<AnimationComponent>("AnimationComponent", compID, compName);

		RegisterComponentFunction<CameraShakeComponent>("CameraShakeComponent", compID, compName);

		RegisterComponentFunction<CircleCollisionComponent>("CircleCollisionComponent", compID, compName, glm::vec2{ 0.f }, 1.f);

		RegisterComponentFunction<RectCollisionComponent>("RectCollisionComponent", compID, compName);


		//RegisterComponentFunction<StaticRectCollisionComponent>("StaticRectCollisionComponent", compID, compName);


		//RegisterComponentFunction<ColorComponent>("ColorComponent", compID, compName);


		RegisterComponentFunction<DragAccelComponent>("DragAccelComponent", compID, compName);


		//RegisterComponentFunction<FollowComponent>("FollowComponent", compID, compName);


		RegisterComponentFunction<GravityComponent>("GravityComponent", compID, compName);


		//RegisterComponentFunction<LifeTimeComponent>("LifeTimeComponent", compID, compName);


		RegisterComponentFunction<ObjectStateComponent>("ObjectStateComponent", compID, compName);


		RegisterComponentFunction<OffsetComponent>("OffsetComponent", compID, compName);


		//RegisterComponentFunction<CircleComponent>("CircleComponent", compID, compName);


		RegisterComponentFunction<RaycastComponent>("RaycastComponent", compID, compName);


		//RegisterComponentFunction<TextComponent>("TextComponent", compID, compName);


		//RegisterComponentFunction<TextureBoxComponent>("TextureBoxComponent", compID, compName);


		RegisterComponentFunction<TextureComponent>("TextureComponent", compID, compName);


		RegisterComponentFunction<TileMapComponent>("TileMapComponent", compID, compName);

		RegisterComponentFunction<TransformComponent>("TransformComponent", compID, compName);

		RegisterComponentFunction<UserInputComponent>("UserInputComponent", compID, compName);


		RegisterComponentFunction<VelocityComponent>("VelocityComponent", compID, compName);


		RegisterComponentFunction<WidthHeightComponent>("WidthHeightComponent", compID, compName);
		
		RegisterComponentFunction<HealthComponent>("HealthComponent", compID, compName);

		RegisterComponentFunction<EnemyStateComponent>("EnemyStateComponent", compID, compName);

		RegisterComponentFunction<EnemyWeaponComponent>("EnemyWeaponComponent", compID, compName);

	}

	void Loader::LoadScene(const std::string& filePath)
	{
		SavedIDLoadedIDMaps.clear();
		
		std::ifstream stream;
		stream.open(filePath);
		
		Json::Value root;
		stream >> root;
		stream.close();

		//register EntityType
		Json::Value EntityTypeID = root["EntityType"]["EntitiesID"];
		for (unsigned int i = 0; i < EntityTypeID.size(); ++i)
		{
			RegisterEntities(EntityTypeID[i].asInt(), root["EntityType"][EntityTypeID[i].asString()].asString());
		}


		
		//register ComponentsType
		Json::Value ComponentTypeID = root["ComponentType"]["ComponentsID"];
		for (unsigned int i = 0; i < ComponentTypeID.size(); ++i)
		{
			RegisterComponents(ComponentTypeID[i].asInt(), root["ComponentType"][ComponentTypeID[i].asString()].asString());
		}
		
		//add entities.
		Json::Value entts = root["Entities"];
		auto num_of_entts = entts.size();

		SavedIDLoadedIDMaps.reserve(num_of_entts);
		for (unsigned int i=0; i< num_of_entts; ++i)
		{
			std::string entt_id=entts[i].asString();
			int entt_TypeID = root[entt_id]["Type"].asInt();
		
			BaseEntity* entity = nullptr;
			if (CreateEntityFunctions.find(entt_TypeID) != CreateEntityFunctions.end())
			{
				entity = CreateEntityFunctions[entt_TypeID](root[entt_id]["Tag"].asString());
				SavedIDLoadedIDMaps[entts[i].asInt()] = entity->GetEntityID();
				entity->Deserialize(root[entt_id]);
			}
			else
			{
				static_assert(true, "unknown entt type!");
				//should be warning actually.
				//entity = scene->AddEntity<BaseEntity>(root[entt_id]["Tag"].asString());
			}
			
			/*
			//add comps
			Json::Value comps = root[entt_id]["ComponentIDs"];
			for (int j = 0; j < comps.size(); ++j)
			{
				int comp_id = comps[j].asInt();
				if (CreateComponentFunctions.find(comp_id) != CreateComponentFunctions.end())
				{
					IComponent* comp=CreateComponentFunctions[comp_id](entity);
					//Deserialize component
					comp->Deserialize(root[entt_id][std::to_string(comp_id)]);
				}
			}
			commmented out to avoid crash when it has dependency.
			*/ 
		}

		for(auto& [SavedID,LoadedID]:SavedIDLoadedIDMaps)
		{
			//add comps
			int entt_TypeID = root[std::to_string(SavedID)]["Type"].asInt();
			Json::Value comps = root[std::to_string(SavedID)]["ComponentIDs"];
			for (unsigned int j = 0; j < comps.size(); ++j)
			{
				int comp_id = comps[j].asInt();
				if (CreateComponentFunctions.find(comp_id) != CreateComponentFunctions.end())
				{
					IComponent* comp = CreateComponentFunctions[comp_id](GetEntityByTypeFunctions[entt_TypeID](LoadedID));
					//Deserialize component
					comp->Deserialize(root[std::to_string(SavedID)][std::to_string(comp_id)]);
				}
			}
		}


		
	}

	void Loader::LoadTileMap(TileMapComponent* tileMapComp)
	{
		auto& tiles = tileMapComp->GetTiles();
		while (!tiles.empty()) {
			scene->DeleteEntityComponents(tiles.front());
			tiles.pop();
		}
		


		q_engine::TileMapParser map_parser;
		q_engine::Layer layer{ map_parser.Parse(tileMapComp->mFileName,tileMapComp->mOffset) };
		glm::vec2 tile_size = map_parser.getTileSize();
		const float tile_scale = map_parser.getTileScale();
		glm::vec2 imageSize = map_parser.getImageSize();
		
		MapEntity* Map = scene->AddEntity<MapEntity>();
		PortalEntity* Last = scene->AddEntity<PortalEntity>();
		BackGroundEntity* back = scene->AddEntity<BackGroundEntity>();


		//q_engine::MapEntity* Map;// = AddEntity<q_engine::MapEntity>();
		for (int i = 0; i < static_cast<int>(layer.size()); i++)
		{
			
			auto texel = map_parser.getTileset().find(layer[i]->tileId)->second->textureRect;

			if (layer[i]->properties->layername == "PortalTile")
			{
				Last = scene->AddEntity<PortalEntity>();
				scene->AddComponent<TextureComponent>(Last, "NTiles"_hashID, texel.bottomLeft.x, texel.bottomLeft.y,
					texel.topRight.x, texel.topRight.y, 10000);
				scene->AddComponent<TransformComponent>(Last, layer[i]->x, layer[i]->y, 0.f, tile_scale, tile_scale);
				scene->AddComponent<WidthHeightComponent>(Last, tile_size.x * tile_scale, tile_size.y * tile_scale);
				scene->AddComponent<StaticRectCollisionComponent>(Last, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, true);
				scene->AddComponent<OffsetComponent>(Last);

			}
			else if (layer[i]->properties->layername == "BgTile")
			{
				back = scene->AddEntity<BackGroundEntity>();
				scene->AddComponent<TextureComponent>(back, "NTiles"_hashID, texel.bottomLeft.x, texel.bottomLeft.y,
					texel.topRight.x, texel.topRight.y, 10000);
				scene->AddComponent<TransformComponent>(back, layer[i]->x, layer[i]->y, 0.f, tile_scale, tile_scale);
				scene->AddComponent<WidthHeightComponent>(back, tile_size.x * tile_scale, tile_size.y * tile_scale);
				scene->AddComponent<OffsetComponent>(back);
			}
			else
			{
				Map = scene->AddEntity<MapEntity>();
				if (layer[i]->properties->layername != "WallTile")
				{
					scene->AddComponent<TextureComponent>(Map, "NTiles"_hashID, texel.bottomLeft.x, texel.bottomLeft.y,
						texel.topRight.x, texel.topRight.y, 10000);
				}
				scene->AddComponent<TransformComponent>(Map, layer[i]->x, layer[i]->y, 0.f, tile_scale, tile_scale);
				scene->AddComponent<WidthHeightComponent>(Map, tile_size.x * tile_scale, tile_size.y * tile_scale);
				scene->AddComponent<StaticRectCollisionComponent>(Map, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, false, false, true);
				scene->AddComponent<OffsetComponent>(Map);
			}
		}
	}

	IDType Loader::GetLoadedIDWithSavedID(int SavedID)
	{
		return SavedIDLoadedIDMaps[SavedID];
	}


}
