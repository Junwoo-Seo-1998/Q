/*
	File Name: NewPlayerInventorySystem.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary: 
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "NewPlayerInventorySystem.h"

#include <iostream>


#include "Component/NewItemComponent.h"
#include "Engine/ECS/InputManager.h"

#include "Component/NewPlayerInventorySlots.h"
#include "Component/NewWeaponComponent.h"

NewPlayerInventorySystem::NewPlayerInventorySystem(q_engine::GameState* state)
	:q_engine::System<NewPlayerInventorySystem>(state)
{
}

void NewPlayerInventorySystem::Load()
{
}

void NewPlayerInventorySystem::Update([[maybe_unused]] double deltaTime)
{
	const auto& currentState = m_controlled_state;
	auto& player_weapons = currentState->GetComponents<NewWeaponComponent>();
	auto& player_Items = currentState->GetComponents<NewItemComponent>();
	auto& player_Inventory = currentState->GetComponents<NewPlayerInventorySlots>();
	if(q_engine::InputManager::IsKeyPressed(Key::Z))
	{
		std::cout << "weapon swap!"<<std::endl;
		player_Inventory.Front().ChangeToNextWeapon();
	}
	if (q_engine::InputManager::IsKeyPressed(Key::A))
	{
		std::cout << "weapon swap!" << std::endl;
		player_Inventory.Front().ChangeToNextItem();
	}

	
	
	for (auto& player_weapon : player_weapons)
	{
		player_weapon.ChangeCurrentWeapon(player_Inventory.Front().GetCurrentWeapon());

	}

	for (auto& player_Item : player_Items)
	{
		player_Item.ChangeCurrentItem(player_Inventory.Front().GetCurrentItem());

	}
}

void NewPlayerInventorySystem::Unload()
{
}

void NewPlayerInventorySystem::OnEvent([[maybe_unused]] q_engine::IEvent* eventName)
{
}
