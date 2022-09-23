/*
	File Name: ScenceSerializer.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "jsoncpp/json.h"
#include "Engine/Types.h"
namespace q_engine
{
	class GameState;
	class SceneSerializer
	{
	public:
		SceneSerializer(GameState* scene);
		//text
		void Serialize(const std::string& filepath);


		
	private:
		void RegisterEntityTypeFunction(Json::Value& jsonData);
		void RegisterComponentTypeFunction(Json::Value& jsonData);
		void ComponentSerializeFunction(Json::Value& jsonData, GameState* scene, IDType enttID);
		GameState* scene;
	};
}
