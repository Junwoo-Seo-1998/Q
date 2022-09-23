/*
	File Name: LuaScripComponent.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../ECS/Component.h"
#include <string>
#include <memory>
#include "sol/sol.hpp"
namespace  q_engine
{
	class LuaScriptComponent :public Component<LuaScriptComponent>
	{
	public:
		LuaScriptComponent();
		LuaScriptComponent(const std::string& lua_file);
		~LuaScriptComponent();

		void Init();
		void OnInit();
		void OnUpdate(float dt);
		void LoadLuaScript(const std::string& lua_file);
		void SwitchToOtherLuaScript(const std::string& lua_file);
		std::string GetFileName() const { return mFileName; }
		std::shared_ptr<sol::environment> GetLuaData();
	private:
		std::string mFileName;
		std::shared_ptr<sol::environment> mEnv;
		std::shared_ptr<sol::protected_function> mOnInit;
		std::shared_ptr<sol::protected_function> mOnUpdate;
	};
}