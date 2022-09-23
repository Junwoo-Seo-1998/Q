/*
	File Name: FXAnimLuaScriptsComponent.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/Component.h"
#include "Engine/Component/InnerLuaScriptComponent.h"

class FXAnimLuaScriptsComponent final : public q_engine::Component<FXAnimLuaScriptsComponent>
{
public:
	struct EntInfo
	{
		q_engine::IDType typeID;
		q_engine::IDType entID;
	};
	FXAnimLuaScriptsComponent() = default;

	void Update(double dt);
	void Unload() override;
	void OnComponentCreation() override;
	
	void LoadPlayerAnims();
	void LoadEnemyAnims();
	void RunScript(std::string mName);
private:
	void LoadCommonAnims();
	std::pair<EntInfo, InnerLuaScriptComponent> RunInnerScriptFunc(const std::string& scriptPath) const;
	std::unordered_map<std::string, std::string> mInitializers;
	//first script is running condition, second one is destroying condition
	std::vector<std::pair<EntInfo, InnerLuaScriptComponent>> mRunningScripts;
};

void RegisterFXAnimLuaScriptsComponent(sol::state& state);