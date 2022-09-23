/*
	File Name: NewWeaponComponent.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/Component.h"
#include "Engine/Component/InnerLuaScriptComponent.h"
class NewWeaponComponent final : public q_engine::Component<NewWeaponComponent>
{
public:
	NewWeaponComponent();

	virtual void OnComponentCreation() override;
	void Update(const float dt);
	bool GetIsAttacking();
	const std::string& GetCurrentWeapon() const;
	void ChangeCurrentWeapon(const std::string& weapon);

	bool IsRangedWeapon();
	int GetMaxAmmo();
	int GetCurrentAmmo();
	int GetMeleeDamage();
private:
	std::unordered_map<std::string, InnerLuaScriptComponent> weapon_scripts;
	std::string current_weapon = "Dagger";
};
