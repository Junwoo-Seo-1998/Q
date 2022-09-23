/*
	File Name: NewitemComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "NewItemComponent.h"

#include "Engine/Engine.h"
#include "Engine/GameStateManager.h"

#include "../Game/System/UI/PlayerItemUIUpdateDrawSystem.h"

NewItemComponent::NewItemComponent()
{
}

void NewItemComponent::OnComponentCreation()
{
	item_scripts.emplace("Bomb", InnerLuaScriptComponent());
	item_scripts["Bomb"].InitLua("resource/lua/Item/Bomb.lua", GetEntityID());

	item_scripts.emplace("Potion", InnerLuaScriptComponent());
	item_scripts["Potion"].InitLua("resource/lua/Item/Potion.lua", GetEntityID());

	item_scripts.emplace("PoisonBomb", InnerLuaScriptComponent());
	item_scripts["PoisonBomb"].InitLua("resource/lua/Item/PoisonBomb.lua", GetEntityID());
}

void NewItemComponent::Update(const float dt)
{
	if(current_item.empty())
		return;

	item_scripts[current_item].OnUpdate(dt);
}

const std::string& NewItemComponent::GetCurrentItem() const
{
	return current_item;
}


void NewItemComponent::ChangeCurrentItem(const std::string& str)
{
	current_item = str;
}

void NewItemComponent::NotifyItemUIUsed()
{
	auto& UI_system = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetUIState()->GetDrawSystem<PlayeritemUIUpdateDrawSystem>();
	UI_system.ResolveItemUsedEvent(current_item);
}
