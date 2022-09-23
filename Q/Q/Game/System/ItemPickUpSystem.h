/*
	File Name: ItemPickUpSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"


class ItemPickUpSystem final : public q_engine::System<ItemPickUpSystem>
{
public:
	ItemPickUpSystem(q_engine::GameState* state) : System<ItemPickUpSystem>(state) {}
	void Load() override;
	void Unload() override;
	void Update(double deltaTime) override;

	void OnEvent(q_engine::IEvent* event) override;

	constexpr void TryPickUp() noexcept { mbPickUp = true; }
	void NotTryPickUp() noexcept { mbPickUp = false; }

private:
	bool mbPickUp = false;
};
