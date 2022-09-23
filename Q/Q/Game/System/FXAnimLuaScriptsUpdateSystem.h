/*
	File Name: FXAnimLuaScriptsUpdateSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary: Hoseob Jeong
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"
#include "../Engine/GameState.h"
#include "../Component/FXAnimLuaScriptsComponent.h"
#include "Game/Component/FXAnimLuaScriptsComponent.h"
class FXAnimLuaScriptsUpdateSystem final : public q_engine::System<FXAnimLuaScriptsUpdateSystem>
{
public:
	FXAnimLuaScriptsUpdateSystem() = delete;
	FXAnimLuaScriptsUpdateSystem(q_engine::GameState* state) : System<FXAnimLuaScriptsUpdateSystem>(state) {};
	void Update(double deltaTime) override;

	void Adopt(FXAnimLuaScriptsComponent::EntInfo&& first, InnerLuaScriptComponent&& second);
private:
	std::vector<std::pair<FXAnimLuaScriptsComponent::EntInfo, InnerLuaScriptComponent>> mOrphanScripts;
};


