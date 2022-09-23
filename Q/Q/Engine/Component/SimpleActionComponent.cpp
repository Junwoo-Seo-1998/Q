#include "SimpleActionComponent.h"

SimpleActionComponent::SimpleActionComponent(const std::string& script_path)
	:script_path(script_path)
{
}

void SimpleActionComponent::OnComponentCreation()
{
	simple_script.InitLua(script_path, GetEntityID());
}

void SimpleActionComponent::OnUpdate(float dt)
{
	simple_script.OnUpdate(dt);
}


bool SimpleActionComponent::IsDead()
{
	return simple_script.GetLuaData()->get<bool>("is_dead");
}
