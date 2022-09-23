/*
	File Name: NewPlayerInventorySlots.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary: Hoseob jeong
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "NewPlayerInventorySlots.h"

#include "Engine/Component/SoundComponent.h"
#include "Engine/Engine.h"
#include "PlayTestTrace.h"

int NewPlayerInventorySlots::maxCubeAmount = 8;

NewPlayerInventorySlots::NewPlayerInventorySlots()
{
	ClearSlots();
}

void NewPlayerInventorySlots::ClearSlots()
{
	weaponSlot[0] = "Dagger";
	weaponSlot[1] = "";

	weapon_cur = 0;
	item_cur = 0;

	ItemSlot[0] = "";
	ItemSlot[1] = "";
	itemCount[0] = 0;
	itemCount[1] = 0;

	CubeSlot = "";
	cubecount = 0;
}



void NewPlayerInventorySlots::ChangeToPrevWeapon()
{
	
	if (--weapon_cur < 0)
		weapon_cur = 2 - 1;
}

void NewPlayerInventorySlots::ChangeToNextWeapon()
{
	++weapon_cur;

	if (weapon_cur == 2)
		weapon_cur = 0;
	
	if(IsWeaponSlotEmpty(weapon_cur))
	{
		weapon_cur--;
	}
	else
	{
		PlayTestTrace::LogWeaponSwapMessage(weaponSlot[weapon_cur == 0 ? 1 : 0], weaponSlot[weapon_cur]);
		q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PlaySoundEffect("item_swap", false);
	}

}

void NewPlayerInventorySlots::ChangeToPrevItem()
{
	if (--item_cur < 0)
		item_cur = 2 - 1;
}

void NewPlayerInventorySlots::ChangeToNextItem()
{
	++item_cur;

	if (item_cur == 2)
		item_cur = 0;
	

	if (IsItemSlotEmpty(item_cur))
	{
		return;
		
	}
	else
	{
		PlayTestTrace::LogItemSwapMessage(ItemSlot[item_cur == 0 ? 1 : 0], ItemSlot[item_cur]);
		q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PlaySoundEffect("item_swap", false);
	}

}

bool NewPlayerInventorySlots::IsWeaponSlotEmpty(int slot_num)
{
	return weaponSlot[slot_num].empty();
}

bool NewPlayerInventorySlots::IsItemSlotEmpty(int slot_num)
{
	return ItemSlot[slot_num].empty();
}

bool NewPlayerInventorySlots::IsCubeSlotEmpty()
{
	return CubeSlot.empty();
}

void NewPlayerInventorySlots::SetSlotWeaponName(int i, std::string weaponname)
{
	weaponSlot[i] = weaponname;
}

void NewPlayerInventorySlots::SetSlotItemName(int i, std::string itemname, int count)
{
	ItemSlot[i] = itemname;
	itemCount[i] += count;
}

void NewPlayerInventorySlots::SetSlotCubeName(const std::string& cubename, int count)
{
	CubeSlot = cubename;
	cubecount += count;
}

const std::string& NewPlayerInventorySlots::GetSlotItemName(int idx) const
{
	return ItemSlot[idx];
}

const std::string& NewPlayerInventorySlots::GetSlotWeaponName(int idx) const
{
	return weaponSlot[idx];
}

const std::string& NewPlayerInventorySlots::GetCubeName() const
{
	
	return CubeSlot;
}

int NewPlayerInventorySlots::GetCurrentItemIndex() const
{
	return item_cur;
}

int NewPlayerInventorySlots::GetCurrentItemLeft() const
{
	return itemCount[item_cur];
}


int NewPlayerInventorySlots::GetCurrentWeaponIndex() const
{
	return weapon_cur;
}

int NewPlayerInventorySlots::GetCubeAmount() const
{
	return cubecount;
}

const std::string& NewPlayerInventorySlots::GetCurrentWeapon() const
{
	return weaponSlot[weapon_cur];
}

const std::string& NewPlayerInventorySlots::GetCurrentItem() const
{
	return ItemSlot[item_cur];
}

void NewPlayerInventorySlots::ItemUsed()
{
	itemCount[item_cur]--;
	if(itemCount[item_cur]<=0)
	{
		ItemSlot[item_cur].clear();
		itemCount[item_cur] = 0;
	}
}

int NewPlayerInventorySlots::GetCurrentItemCount() const
{
	return itemCount[item_cur];
}

int NewPlayerInventorySlots::GetSlotItemCount() const
{
	return itemCount[item_cur == 0 ? 1 : 0];
}

void NewPlayerInventorySlots::SetCurrentItemCount(int howmany)
{
	itemCount[item_cur] = howmany;
}

void RegisterNewPlayerInventorySlotsComponent(sol::state& state)
{
	sol::usertype<NewPlayerInventorySlots> newPlayerInventorySlots = state.new_usertype<NewPlayerInventorySlots>("NewPlayerInventorySlots");
	newPlayerInventorySlots.set_function("ItemUsed",&NewPlayerInventorySlots::ItemUsed);
}
