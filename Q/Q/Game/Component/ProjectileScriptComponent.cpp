/*
	File Name: ProjectileScriptComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ProjectileScriptComponent.h"

#include <iostream>

ProjectileScriptComponent::ProjectileScriptComponent(const std::string& script_path)
	:script_path(script_path)
{
}

void ProjectileScriptComponent::OnComponentCreation()
{
	projectile_script.InitLua(script_path, GetEntityID());
	mOnCollideWith = std::make_shared<sol::protected_function>((*projectile_script.GetLuaData())["OnCollideWith"]);
	if (!mOnCollideWith->valid())
		mOnCollideWith.reset();
}

void ProjectileScriptComponent::OnUpdate(float dt)
{
	projectile_script.OnUpdate(dt);
}

void ProjectileScriptComponent::OnCollideWith(q_engine::IDType typeID, q_engine::IDType entt_id)
{
	if (mOnCollideWith)
	{
		sol::protected_function_result result = mOnCollideWith->call(typeID, entt_id);
		if (!result.valid())
		{
			//error
			sol::error error = result;
			
			std::cout << error.what();
		}
	}
	
}

bool ProjectileScriptComponent::IsDead()
{
	return projectile_script.GetLuaData()->get<bool>("is_dead");
}
