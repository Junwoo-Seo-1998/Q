/*
	File Name: ScoutStateHelper.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScoutChasingState.h"

#include "ScoutMissingState.h"
#include "ScoutSearchingState.h"
#include "ScoutAttackingState.h"
#include "ScoutDyingState.h"
#include "ScoutHurtingState.h"
#include "../EnemyStateHelper.h"
#include "../Engine/GameState.h"
#include "../../../EObjectZDepth.h"
#include "../../../../Engine/Component/AnimationComponent.h"
#include "../../../../Engine/Component/RaycastComponent.h"
#include "../../../../Engine/Event/RayTriggerEvent.h"
#include "../../../../Engine/Event/TriggerEvent.h"
#include "../Engine/Core/StringHash.h"
#include "Component/HealthComponent.h"
#include "ScoutStateHelper.h"


constexpr float scoutDistanceMultiplyer = 8;

void ScoutMovementUpdate(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();

	if(isCliffWall(controlledState,enemyID))
	{
		enemyState.SetSpeedMultiplier(0.f);
	}
	else
	{
		enemyState.SetSpeedMultiplier(1.f);
		if (abs(relativePosition).x < abs(enemyBox.Size().x) / 2 && abs(relativePosition).y < abs(enemyBox.Size().y) / 2)
			enemyState.SetCurrentState<ScoutMissingState>(controlledState);
		else if (relativePosition.x > abs(enemyBox.Size().x) * scoutDistanceMultiplyer)
			enemyState.MoveRight();
		else if (relativePosition.x <= abs(enemyBox.Size().x) * scoutDistanceMultiplyer - 10 &&
			relativePosition.x > abs(enemyBox.Size().x) / 2)
			enemyState.SetSpeedMultiplier(0.f);
		else if (relativePosition.x < -abs(enemyBox.Size().x) * scoutDistanceMultiplyer)
			enemyState.MoveLeft();
		else if (relativePosition.x >= -abs(enemyBox.Size().x) * scoutDistanceMultiplyer + 10 &&
			relativePosition.x < -abs(enemyBox.Size().x) / 2)
			enemyState.SetSpeedMultiplier(0.f);
	}

	
	enemyState.UpdateLookingDirection(atan2(relativePosition.y, relativePosition.x));
}
