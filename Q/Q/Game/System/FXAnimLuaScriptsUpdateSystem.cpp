/*
	File Name: FXAnimLuaScriptsUpdateSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary: 
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FXAnimLuaScriptsUpdateSystem.h"



#include "Engine/Component/FollowComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Engine.h"
#include "Engine/GameStateManager.h"

void FXAnimLuaScriptsUpdateSystem::Update(double deltaTime)
{
	auto& comps = m_controlled_state->GetComponents<FXAnimLuaScriptsComponent>();

	for (auto& comp : comps)
	{
		comp.Update(deltaTime);
	}

	//Same logic with corresponding comp

	std::vector<unsigned> deleting;
	using q_engine::IDType;
	for (decltype(mOrphanScripts)::reverse_iterator itr = mOrphanScripts.rbegin(); itr != mOrphanScripts.rend(); ++itr)
	{
		auto& script = *itr;
		script.second.OnUpdate(static_cast<float>(deltaTime));
		if (script.second.GetLuaData()->get<bool>("DoDestroy"))
		{
			q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState()->
				DeleteEntityComponentsLater(script.first.typeID, script.first.entID);
			deleting.push_back(abs(std::distance(itr, mOrphanScripts.rend())) - 1);
		}
	}

	for (auto deletingPos : deleting)
	{
		mOrphanScripts.erase(mOrphanScripts.begin() + deletingPos);
	}
}

void FXAnimLuaScriptsUpdateSystem::Adopt(FXAnimLuaScriptsComponent::EntInfo&& first, InnerLuaScriptComponent&& second)
{
	mOrphanScripts.emplace_back(std::pair{ first , second });
}