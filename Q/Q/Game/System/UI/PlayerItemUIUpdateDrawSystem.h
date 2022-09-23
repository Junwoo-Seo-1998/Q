/*
	File Name: PlayeritemUIUpdateDrawSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../../Engine/Graphics/DrawObject.h"
#include "../Engine/ECS/System.h"
#include "Engine/Core/Timer.h"
#include "Engine/Engine.h"
#include "Engine/System/TimerManagerSystem.h"
#include "Engine/Types.h"
class PlayeritemUIUpdateDrawSystem final : public q_engine::System<PlayeritemUIUpdateDrawSystem>
{
	friend class NotifyUIItemUsedEvent;
	friend class NewItemComponent;
public:
	PlayeritemUIUpdateDrawSystem(q_engine::GameState* state);

	void Load() override;
	void Update(double deltaTime) override;
	void Unload() override;

private:

private:
	q_engine::IDType mFirstItemEntityID = q_engine::INVALID_ID;
	q_engine::IDType mSecondItemEntityID = q_engine::INVALID_ID;

	using ItemUseInteraction = std::pair<bool, q_engine::TimerID>;
	ItemUseInteraction mFirstItemUseInteractionTimer;
	ItemUseInteraction mSecondItemUseInteractionTimer;
	void ResolveItemInteractionEventFromItemEntity(q_engine::IDType entityID);

	void ResolveItemUsedEvent(const std::string& item_name);

	void UpdateItemAmountText();
};
