/*
	File Name: NewitemComponent.h
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

class NewItemComponent final : public q_engine::Component<NewItemComponent>
{
	friend class PlayeritemUIUpdateDrawSystem;
public:
	NewItemComponent();

	virtual void OnComponentCreation() override;
	void Update(const float dt);
	const std::string& GetCurrentItem() const;
	void ChangeCurrentItem(const std::string& str);

	//only used in lua
	void NotifyItemUIUsed();
private:
	std::unordered_map<std::string, InnerLuaScriptComponent> item_scripts;
	std::string current_item = "";
};

inline void RegisterNewItemComponent(sol::state& state)
{
	sol::usertype<NewItemComponent> newItemComponent = state.new_usertype<NewItemComponent>("NewItemComponent");
	newItemComponent.set_function("NotifyItemUIUsed", &NewItemComponent::NotifyItemUIUsed);
}