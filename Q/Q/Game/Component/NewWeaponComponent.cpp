/*
	File Name: NewWeaponComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "NewWeaponComponent.h"



#include "ProjectileScriptComponent.h"
#include "Engine/Component/CollisionComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Entity/ProjectileEntity.h"

namespace q_engine {
	class BaseEntity;
}

NewWeaponComponent::NewWeaponComponent()
{
	
}


void NewWeaponComponent::OnComponentCreation()
{
	weapon_scripts.emplace("Dagger", InnerLuaScriptComponent());
	weapon_scripts["Dagger"].InitLua("resource/lua/Weapon/Dagger.lua", GetEntityID());
	weapon_scripts.emplace("Gun", InnerLuaScriptComponent());
	weapon_scripts["Gun"].InitLua("resource/lua/Weapon/Gun.lua", GetEntityID());
	weapon_scripts.emplace("ShotGun", InnerLuaScriptComponent());
	weapon_scripts["ShotGun"].InitLua("resource/lua/Weapon/ShotGun.lua", GetEntityID());
	weapon_scripts.emplace("Rocket", InnerLuaScriptComponent());
	weapon_scripts["Rocket"].InitLua("resource/lua/Weapon/Rocket.lua", GetEntityID());
}

void NewWeaponComponent::Update(const float dt)
{
	weapon_scripts[current_weapon].OnUpdate(dt);
}

bool NewWeaponComponent::GetIsAttacking()
{
	return weapon_scripts[current_weapon].GetLuaData()->get<bool>("IsAttacking");
}

const std::string& NewWeaponComponent::GetCurrentWeapon() const
{
	return current_weapon;
}

void NewWeaponComponent::ChangeCurrentWeapon(const std::string& weapon)
{
	if(!GetIsAttacking())
	    current_weapon = weapon;
}


bool NewWeaponComponent::IsRangedWeapon()
{
	std::string weaponType = weapon_scripts[current_weapon].GetLuaData()->get<std::string>("WeaponType");
	if(weaponType=="Ranged")
	{
		return true;
	}
	return false;
}

int NewWeaponComponent::GetMaxAmmo()
{
	return weapon_scripts[current_weapon].GetLuaData()->get<int>("Max_ammo");
}

int NewWeaponComponent::GetCurrentAmmo()
{
	return weapon_scripts[current_weapon].GetLuaData()->get<int>("Current_ammo");
}

int NewWeaponComponent::GetMeleeDamage()
{
	if(IsRangedWeapon()==true)
	{
		static_assert(true, "only for melee weapon");
	}
	
	return weapon_scripts[current_weapon].GetLuaData()->get<int>("MeleeDamage");
}
