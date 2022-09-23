/*
	File Name: PlayerWeaponSystem.cpp
	Project Name: Q
	Author(s):
		Primary: junwoo.seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerWeaponSystem.h"
#include "Engine/GameStateManager.h"
#include "Component/NewWeaponComponent.h"
#include "Engine/GameState.h"

PlayerWeaponSystem::PlayerWeaponSystem(q_engine::GameState* state)
	:q_engine::System<PlayerWeaponSystem>(state)
{
}

void PlayerWeaponSystem::Load()	
{
}

void PlayerWeaponSystem::Update(double deltaTime)
{
	const auto& currentState = m_controlled_state;
	auto& player_weapons = currentState->GetComponents<NewWeaponComponent>();
	for (auto& player_weapon:player_weapons)
	{
		player_weapon.Update(static_cast<float>(deltaTime));
		
	}
}

void PlayerWeaponSystem::Unload()
{
}

void PlayerWeaponSystem::OnEvent([[maybe_unused]] q_engine::IEvent* eventName)
{
}
