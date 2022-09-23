/*
	File Name: NewPlayerInventorySlots.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary: Hoseob jeong
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/Component.h"
#include "sol/sol.hpp"
class NewPlayerInventorySlots final : public q_engine::Component<NewPlayerInventorySlots>
{
	friend class PlayerWeaponUIUpdateDrawSystem;
	friend class PlayeritemUIUpdateDrawSystem;
public:
	NewPlayerInventorySlots();
	void ClearSlots();
	
	void ChangeToPrevWeapon();
	void ChangeToNextWeapon();
	void ChangeToPrevItem();
	void ChangeToNextItem();
	bool IsWeaponSlotEmpty(int slot_num);
	bool IsItemSlotEmpty(int slot_num);
	bool IsCubeSlotEmpty();
	void SetSlotWeaponName(int i, std::string weaponname);
	void SetSlotItemName(int i, std::string itemname, int count=1);
	void SetSlotCubeName(const std::string& cubename, int count = 1);
	const std::string& GetSlotItemName(int idx) const;
	const std::string& GetSlotWeaponName(int idx) const;
	const std::string& GetCubeName() const;
	int GetCurrentItemIndex() const;
	int GetCurrentItemLeft() const;
	int GetCurrentWeaponIndex() const;
	int GetCubeAmount() const;
	const std::string& GetCurrentWeapon() const;
	const std::string& GetCurrentItem() const;
	void ItemUsed();
	int GetCurrentItemCount() const;
	int GetSlotItemCount() const;
	void SetCurrentItemCount(int howmany);

	static int GetMaxCubeAmount()
	{
		return maxCubeAmount;
	}
private:
	
	std::string weaponSlot[2];
	int weapon_cur = 0;
	std::string ItemSlot[2];
	int itemCount[2] = { 0, };
	int item_cur = 0;
	
	std::string CubeSlot;
	int cubecount = 0;
	static int maxCubeAmount;
};

void RegisterNewPlayerInventorySlotsComponent(sol::state& state);