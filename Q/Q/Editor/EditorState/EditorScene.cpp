/*
	File Name: EditorScene.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EditorScene.h"

#include <iostream>


#include "EObjectZDepth.h"
#include "Engine/Component/CameraShakeComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Entity/CameraEntity.h"
#include "Entity/PhysicsTestEntities.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "glm/glm.hpp"
#include "Engine/Core/StringHash.h"
#include "Engine/System/TextureDrawSystem.h"
#include "Editor/Graphics/EditorDrawSystem.h"
#include "Engine/System/AnimationUpdateDrawSystem.h"
#include "Engine/Component/TileMapComponent.h"


#include "JsonParser/MapJsonParser.h"
#include "Engine/Loader/Loader.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "../SceneSerializer.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/System/ScriptSystem.h"
void EditorScene::Load()
{
	//main cam for editor
	auto camera = AddEntity<q_engine::CameraEntity>("EditorCamera");
	AddComponent<q_engine::TransformComponent>(camera,glm::vec2{48,0});
	AddComponent<q_engine::CameraShakeComponent>(camera);
	AddComponent<q_engine::LuaScriptComponent>(camera, "resource/lua/EditorCameraMove.lua");
	q_engine::LuaScriptComponent lua=GetComponents<q_engine::LuaScriptComponent>()[camera->GetEntityID()];

	//for testing
	camera = AddEntity<q_engine::CameraEntity>("Camera");
	AddComponent<q_engine::TransformComponent>(camera, glm::vec2{ 48,0 });
	AddComponent<q_engine::CameraShakeComponent>(camera);
	//AddComponent<q_engine::LuaScriptComponent>(camera, "resource/lua/test2.lua");

	auto enemy = AddEntity<q_engine::BaseEntity>("Enemy");
	AddComponent<q_engine::TransformComponent>(enemy, glm::vec2{ 0,0 }, 0.f, glm::vec2{ 2.0f,2.0f });
	AddComponent<q_engine::OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });
	AddComponent<q_engine::AnimationComponent>(enemy, q_engine::get_hash_id("WarriorMissingAnim"), static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	AddComponent<q_engine::LuaScriptComponent>(enemy, "resource/lua/test2.lua");

	
	q_engine::TileMapParser map_parser;
	std::string path = "resource/json/RealPlayableMap3.json";
	glm::vec2 offset = { 48.f,0.f };

	auto Map_Base=AddEntity<q_engine::BaseEntity>("BaseMap");
	AddComponent<q_engine::TileMapComponent>(Map_Base, "resource/json/Level1.json", offset);

	auto& tilecomp = GetComponents<q_engine::TileMapComponent>()[Map_Base->GetEntityID()];
	q_engine::Loader loader(this);


	ReLoadAllTileMaps();
	
	AddDrawSystem<q_engine::TextureDrawSystem>();
	AddDrawSystem<q_engine::AnimationUpdateDrawSystem>();
	//AddDrawSystem<EditorDrawSystem>();
	AddUpdateSystem<q_engine::ScriptSystem>();
	GameState::Load();
	//--------------fortesting
	//q_engine::SceneSerializer writer(this);
	//writer.Serialize("test.json");
	//loader.LoadScene("test.json");
}

void EditorScene::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}

void EditorScene::Unload()
{
	GameState::Unload();
}

void EditorScene::ReLoadTileMap(q_engine::IDType baseMapEntt)
{
	auto& tilecomp = GetComponents<q_engine::TileMapComponent>()[baseMapEntt];
	q_engine::Loader loader(this);
	loader.LoadTileMap(&tilecomp);
}

void EditorScene::ReLoadAllTileMaps()
{
	auto& tilecomp = GetComponents<q_engine::TileMapComponent>();
	for(auto iter=tilecomp.begin(); iter!=tilecomp.end(); ++iter)
	{
		ReLoadTileMap(iter->GetEntityID());
	}
}




