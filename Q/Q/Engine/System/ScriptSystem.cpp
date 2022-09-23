/*
	File Name: ScriptSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScriptSystem.h"
#include "Engine/GameState.h"
#include "Engine/Scripting/ScriptManager.h"
#include "Engine/Component/LuaScriptComponent.h"
namespace q_engine
{
	void ScriptSystem::Load()
	{
		const auto& currentState = m_controlled_state;
		ScriptManager::Get().SetCurrentScene(currentState);
		auto& script_comps = currentState->GetComponents<LuaScriptComponent>();
		for (auto& lua : script_comps)
		{
			lua.OnInit();
		}
	}

	void ScriptSystem::Update(double deltaTime)
	{
		const auto& currentState = m_controlled_state;
		auto& script_comps = currentState->GetComponents<LuaScriptComponent>();
		for(auto& lua:script_comps)
		{
			lua.OnUpdate(static_cast<float>(deltaTime));
		}
	}

	void ScriptSystem::Unload()
	{
	}
}
