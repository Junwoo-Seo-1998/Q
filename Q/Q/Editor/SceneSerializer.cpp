/*
	File Name: ScenceSerializer.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "SceneSerializer.h"
#include "Engine/GameState.h"
//entities
#include "Engine/Entity/CameraEntity.h"
#include "Game/Entity/BombEntity.h"
#include "Game/Entity/ItemEntity.h"
#include "Game/Entity/JumpTriggerEntity.h"
#include "Game/Entity/MapEntity.h"
#include "Game/Entity/UIEntities.h"
#include "Game/Entity/UITestEntities.h"
#include "Game/Entity/WeaponEntity.h"
//entities
#include "Engine/Component/TagComponent.h"
#include "Entity/PhysicsTestEntities.h"



//comps
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"
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

namespace q_engine
{
	template<typename Entity>
	void RegisterEntityType(const std::string& enttName, Json::Value& jsonData)
	{
		int id = q_engine::GetTypeID<Entity>();
		jsonData["EntitiesID"].append(id);
		jsonData[std::to_string(id)] = enttName;
	}
	
	template<typename Component>
	void RegisterComponentType(const std::string& componentName, Json::Value& jsonData)
	{
		int id = Component::GetTypeID();
		jsonData["ComponentsID"].append(id);
		jsonData[std::to_string(id)] = componentName;
	}
	
	template<typename Component>
	void RegisterComponentSerializeFunction(Json::Value& jsonData,GameState* scene, IDType enttID)
	{
		if(!scene->HasComponent<Component>(enttID))
			return;
		IComponent* comp=&scene->GetComponents<Component>()[enttID];
		jsonData[std::to_string(Component::GetTypeID())] =comp->Serialize();
	}
	

	//Register Entity Type here
	void SceneSerializer::RegisterEntityTypeFunction(Json::Value& jsonData)
	{
		RegisterEntityType<BaseEntity>("BaseEntity", jsonData);
		RegisterEntityType<GroundEntity>("GroundEntity", jsonData);
		RegisterEntityType<BombEntity>("BombEntity", jsonData);
		RegisterEntityType<ItemEntity>("ItemEntity", jsonData);
		RegisterEntityType<JumpTriggerEntity>("JumpTriggerEntity", jsonData);
		RegisterEntityType<WeaponEntity>("WeaponEntity", jsonData);
		RegisterEntityType<CameraEntity>("CameraEntity", jsonData);
		RegisterEntityType<EnemyEntity>("EnemyEntity", jsonData);
		RegisterEntityType<EnemyWeaponEntity>("EnemyWeaponEntity", jsonData);
	}

	//Register Component Type here
	void SceneSerializer::RegisterComponentTypeFunction(Json::Value& jsonData)
	{
		RegisterComponentType<AnimationComponent>("AnimationComponent", jsonData);

		RegisterComponentType<CameraShakeComponent>("CameraShakeComponent", jsonData);

		RegisterComponentType<CircleCollisionComponent>("CircleCollisionComponent", jsonData);

		RegisterComponentType<RectCollisionComponent>("RectCollisionComponent", jsonData);


		RegisterComponentType<StaticRectCollisionComponent>("StaticRectCollisionComponent", jsonData);


		RegisterComponentType<ColorComponent>("ColorComponent", jsonData);


		RegisterComponentType<DragAccelComponent>("DragAccelComponent", jsonData);


		RegisterComponentType<FollowComponent>("FollowComponent", jsonData);


		RegisterComponentType<GravityComponent>("GravityComponent", jsonData);


		RegisterComponentType<LifeTimeComponent>("LifeTimeComponent", jsonData);


		RegisterComponentType<ObjectStateComponent>("ObjectStateComponent", jsonData);


		RegisterComponentType<OffsetComponent>("OffsetComponent", jsonData);


		RegisterComponentType<CircleComponent>("CircleComponent", jsonData);


		RegisterComponentType<RaycastComponent>("RaycastComponent", jsonData);


		RegisterComponentType<TextComponent>("TextComponent", jsonData);


		RegisterComponentType<TextureBoxComponent>("TextureBoxComponent", jsonData);


		RegisterComponentType<TextureComponent>("TextureComponent", jsonData);


		RegisterComponentType<TileMapComponent>("TileMapComponent", jsonData);

		RegisterComponentType<TransformComponent>("TransformComponent", jsonData);

		RegisterComponentType<UserInputComponent>("UserInputComponent", jsonData);


		RegisterComponentType<VelocityComponent>("VelocityComponent", jsonData);


		RegisterComponentType<WidthHeightComponent>("WidthHeightComponent", jsonData);

		RegisterComponentType<EnemyStateComponent>("EnemyStateComponent", jsonData);
		
		RegisterComponentType<EnemyWeaponComponent>("EnemyWeaponComponent", jsonData);
		
		RegisterComponentType<HealthComponent>("HealthComponent", jsonData);
	}

	//Register Component Serialize function here
	void SceneSerializer::ComponentSerializeFunction(Json::Value& jsonData, GameState* scene, IDType enttID)
	{
		
		RegisterComponentSerializeFunction<TransformComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<TileMapComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<WidthHeightComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<TextureComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<AnimationComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<VelocityComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<EnemyStateComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<HealthComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<GravityComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<RectCollisionComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<DragAccelComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<RaycastComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<ObjectStateComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<OffsetComponent>(jsonData, scene, enttID);

		RegisterComponentSerializeFunction<EnemyWeaponComponent>(jsonData, scene, enttID);
		
	}
	
	SceneSerializer::SceneSerializer(GameState* scene)
		:scene(scene)
	{
	}

	void SceneSerializer::Serialize(const std::string& filepath)
	{
		Json::Value root;
		//reg entity type
		Json::Value EntityIDList;
		RegisterEntityTypeFunction(EntityIDList);
		root["EntityType"] = EntityIDList;
		//reg comp type
		Json::Value componentList;
		RegisterComponentTypeFunction(componentList);
		root["ComponentType"] = componentList;
		

		
		Json::Value entts;
		for (auto& [id,entt]:scene->GetAllEntities())
		{
			if (entt.GetRuntimeTypeID() == GetTypeID<MapEntity>())
				continue;
			entts.append(id);
			Json::Value entt_info;
			entt_info["Tag"] = scene->GetComponents<TagComponent>()[id].Tag;
			//entt_info["Type"] = BaseEntityToString(entt);
			entt_info["Type"] = entt.GetRuntimeTypeID();
			auto& iters=scene->GetAllComponentInfo().equal_range(id);
			for (auto iter=iters.first; iter!=iters.second; ++iter )
			{
				entt_info["ComponentIDs"].append(iter->second);
			}
			ComponentSerializeFunction(entt_info, scene, id);
			
			
			root[std::to_string(id)] = entt_info;
		}
		root["Entities"] = entts;


		
		Json::StyledWriter writer;//change it fastwriter later
		std::ofstream ost(filepath);
		ost << writer.write(root);
		ost.close();
	}


}
