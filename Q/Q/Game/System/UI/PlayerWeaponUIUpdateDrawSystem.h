/*
	File Name: PlayerWeaponUIUpdateDrawSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>


#include "../../../Engine/Graphics/DrawObject.h"
#include "../Engine/ECS/System.h"
#include "Engine/Core/Timer.h"
#include "Engine/Graphics/TextObject.h"
#include "EObjectZDepth.h"
#include "Engine/Core/StringHash.h"

class PlayerWeaponUIUpdateDrawSystem final : public q_engine::System<PlayerWeaponUIUpdateDrawSystem>
{
	friend class NotifyUIItemUsedEvent;
public:
	PlayerWeaponUIUpdateDrawSystem(q_engine::GameState* state) : System<PlayerWeaponUIUpdateDrawSystem>(state)
	{
	}

	void Load() override;
	void Update(double deltaTime) override;

private:

	q_engine::TextObject mFirstItemText{ q_engine::get_hash_id("MenuFont"), "", static_cast<int>(EObjectZDepth::TEXT) };
	q_engine::TextObject mSecondItemText{ q_engine::get_hash_id("MenuFont"), "", static_cast<int>(EObjectZDepth::TEXT) };
};
