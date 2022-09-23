/*
	File Name: ProjectileScriptComponent.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>
#include "Engine/ECS/Component.h"
#include "Engine/Component/InnerLuaScriptComponent.h"
class ProjectileScriptComponent final : public q_engine::Component<ProjectileScriptComponent>
{
public:
	ProjectileScriptComponent(const std::string& script_path);
	virtual void OnComponentCreation() override;
	void OnUpdate(float dt);
	void OnCollideWith(q_engine::IDType typeID, q_engine::IDType entt_id);
	bool IsDead();
private:
	std::string script_path;
	InnerLuaScriptComponent projectile_script;
	std::shared_ptr<sol::protected_function> mOnCollideWith;
};
