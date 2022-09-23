/*
	File Name: DropWeaponComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Component.h"
#include <string>
namespace q_engine
{
	class DropWeaponComponent final : public Component<DropWeaponComponent>
	{
	public:
		DropWeaponComponent() = default;
		DropWeaponComponent(std::string DropWeaponname) : mWeaponName(DropWeaponname) {}
		std::string GetDropWeaponName() const { return mWeaponName; }

	private:
		std::string mWeaponName;

	};
}
