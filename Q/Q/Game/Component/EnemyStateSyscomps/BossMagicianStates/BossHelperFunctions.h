/*
	File Name: BossHelperFunctions.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>

#include "Engine/Types.h"

namespace q_engine {
	class TriggerEvent;
	class GameState;
}

bool isNear(glm::vec2 pos, glm::vec2 path);

void BoseMagicianMove(q_engine::GameState* controlledState, q_engine::IDType enemyID, double dt);
void BoseMagicianBasicTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent);