/*
	File Name: EnemyStateHelper.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>
#include "../../../Engine/GameState.h"
#include "Engine/Types.h"


namespace q_engine {
	class RayTriggerEvent;
	class TriggerEvent;
}

class EnemyStateComponent;

bool isCliffWall(q_engine::GameState* controlledState, q_engine::IDType enemyID);

void BasicEnemyTriggerEvent(q_engine::GameState*, q_engine::IDType, q_engine::TriggerEvent*);
void EnemyJumpTriggerEvent(q_engine::GameState*, q_engine::IDType, q_engine::TriggerEvent*);
bool isPlayerOnSight(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*);
void BasicEnemyRayTriggerEvent(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*);

inline bool isBigger(const glm::vec2& a, const glm::vec2& b) { return a.x >= b.x && a.y >= b.y; }