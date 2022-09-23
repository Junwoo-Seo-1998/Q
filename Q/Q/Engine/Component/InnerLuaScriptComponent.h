/*
	File Name: InnerLuaScriptComponent.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

//by jun to be used inside of other component class
#pragma once
#include "Engine/Types.h"
#include "sol/sol.hpp"
#include <string>
//update initLua end should be called inside of caller components 
class InnerLuaScriptComponent
{
public:
	InnerLuaScriptComponent();
	
	void InitLua(const std::string& fileName, q_engine::IDType ownerEnttID);
	void OnUpdate(float dt);
	std::shared_ptr<sol::environment> GetLuaData();
private:
	void LoadLuaScript(const std::string& lua_file);
	
	std::string mFileName;
	std::shared_ptr<sol::environment> mEnv;
	std::shared_ptr<sol::protected_function> mOnInit;
	std::shared_ptr<sol::protected_function> mOnUpdate;
};

