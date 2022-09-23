/*
	File Name: BossAssassinHelperFunctions.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/Types.h"

namespace q_engine {
	class GameState;
}

void BossAssassinMove(q_engine::GameState* controlledState, q_engine::IDType enemyID, double dt);
