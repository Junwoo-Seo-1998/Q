/*
	File Name: NewPlayerInventorySystem.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/System.h"
#include "Engine/GameState.h"

class NewPlayerInventorySystem final : public q_engine::System<NewPlayerInventorySystem>
{
public:
	NewPlayerInventorySystem(q_engine::GameState* state);
	virtual void Load() override;
	virtual void Update(double deltaTime) override;
	virtual void Unload() override;
protected:
	virtual void OnEvent(q_engine::IEvent* eventName) override;
};