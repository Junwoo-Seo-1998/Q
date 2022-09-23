/*
	File Name: InnerLuaScriptComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "InnerLuaScriptComponent.h"

#include <iostream>

#include "Engine/Scripting/ScriptManager.h"
#include "sol/sol.hpp"
InnerLuaScriptComponent::InnerLuaScriptComponent()
{
}

void InnerLuaScriptComponent::InitLua(const std::string& fileName, q_engine::IDType ownerEnttID)
{
	LoadLuaScript(fileName);

	mEnv->set("MyID", ownerEnttID);
	if (mOnInit)
	{
		sol::protected_function_result result = mOnInit->call();
		if (!result.valid())
		{
			//error
			sol::error error = result;
	
			std::cout << error.what();
		}
	}
}

void InnerLuaScriptComponent::OnUpdate(float dt)
{
	if (mOnUpdate)
	{
		sol::protected_function_result result = mOnUpdate->call(dt);
		if (!result.valid())
		{
			//error
			sol::error error = result;
	
			std::cout << error.what();
		}
	}
}

std::shared_ptr<sol::environment> InnerLuaScriptComponent::GetLuaData()
{
	return mEnv;
}

void InnerLuaScriptComponent::LoadLuaScript(const std::string& lua_file)
{
	mFileName = lua_file;

	mEnv = std::make_shared<sol::environment>(q_engine::ScriptManager::Get().GetState(), sol::create, q_engine::ScriptManager::Get().GetState().globals());
	auto result = q_engine::ScriptManager::Get().GetState().script_file(mFileName, *mEnv, sol::script_pass_on_error);
	if (!result.valid())
	{
		sol::error error = result;

		std::cout << error.what();
	}

	mOnInit = std::make_shared<sol::protected_function>((*mEnv)["OnInit"]);
	if (!mOnInit->valid())
		mOnInit.reset();


	mOnUpdate = std::make_shared<sol::protected_function>((*mEnv)["OnUpdate"]);
	if (!mOnUpdate->valid())
		mOnUpdate.reset();

	q_engine::ScriptManager::Get().GetState().collect_garbage();
}

