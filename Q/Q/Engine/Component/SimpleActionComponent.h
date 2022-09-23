#pragma once
#include <string>
#include "Engine/ECS/Component.h"
#include "Engine/Component/InnerLuaScriptComponent.h"
class SimpleActionComponent final : public q_engine::Component<SimpleActionComponent>
{
public:
	SimpleActionComponent(const std::string& script_path);
	virtual void OnComponentCreation() override;
	void OnUpdate(float dt);
	bool IsDead();

	InnerLuaScriptComponent& GetScript()
	{
		return simple_script;
	}
private:
	std::string script_path;
	InnerLuaScriptComponent simple_script;
};