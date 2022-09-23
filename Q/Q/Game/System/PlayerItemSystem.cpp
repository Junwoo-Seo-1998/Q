/*
	File Name: PlayerItemSystem.cpp
	Project Name: Q
	Author(s):
		Primary: junwoo.seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerItemSystem.h"
#include "Engine/GameStateManager.h"
#include "Component/NewItemComponent.h"
#include "Engine/GameState.h"

PlayerItemSystem::PlayerItemSystem(q_engine::GameState* state)
	:q_engine::System<PlayerItemSystem>(state)
{
}

void PlayerItemSystem::Load()
{
}

void PlayerItemSystem::Update(double deltaTime)
{
	const auto& currentState = m_controlled_state;
	auto& player_Items = currentState->GetComponents<NewItemComponent>();
	for (auto& Item : player_Items)
	{
		Item.Update(static_cast<float>(deltaTime));
	}
}

void PlayerItemSystem::Unload()
{
}

void PlayerItemSystem::OnEvent([[maybe_unused]] q_engine::IEvent* eventName)
{
}
