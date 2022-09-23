/*
	File Name: LuaScripComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "LuaScriptComponent.h"
#include "Engine/Scripting/ScriptManager.h"
#include <iostream>
namespace q_engine
{
	LuaScriptComponent::LuaScriptComponent()
		:mFileName("")
	{
	}

	LuaScriptComponent::LuaScriptComponent(const std::string& lua_file)
		:mFileName(lua_file)
	{
		Init();
	}

	LuaScriptComponent::~LuaScriptComponent()
	{
	}

	void LuaScriptComponent::Init()
	{
		LoadLuaScript(mFileName);
	}

	void LuaScriptComponent::OnInit()
	{
		mEnv->clear();
	    mEnv->set("MyID", GetEntityID());
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

	void LuaScriptComponent::OnUpdate(float dt)
	{

		if(mOnUpdate)
		{
			sol::protected_function_result result = mOnUpdate->call(dt);
			if(!result.valid())
			{
				//error
				sol::error error = result;
				std::cout << error.what();
			}
		}
	}

	void LuaScriptComponent::LoadLuaScript(const std::string& lua_file)
	{
		mFileName = lua_file;

		mEnv = std::make_shared<sol::environment>(ScriptManager::Get().GetState(), sol::create, ScriptManager::Get().GetState().globals());
		auto result = ScriptManager::Get().GetState().script_file(mFileName, *mEnv, sol::script_pass_on_error);
		if(!result.valid())
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
		
		ScriptManager::Get().GetState().collect_garbage();
	}

	void LuaScriptComponent::SwitchToOtherLuaScript(const std::string& lua_file)
	{
		mFileName = lua_file;
	}

	std::shared_ptr<sol::environment> LuaScriptComponent::GetLuaData()
	{
		return mEnv;
	}
}
