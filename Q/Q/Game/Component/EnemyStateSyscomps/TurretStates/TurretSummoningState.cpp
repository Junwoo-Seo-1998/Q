/*
	File Name: TurretSummoningState.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TurretSummoningState.h"

#include "TurretDyingState.h"

#include "Engine/Engine.h"
#include "EObjectZDepth.h"
#include "TurretMissingState.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Component/AnimationComponent.h"

using namespace q_engine;

void TurretSummoningState::Load(q_engine::GameState* controlledState, IDType enemyID)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("SummonBigBoyAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	enemyState.SetLookingPlayer(false);
}


void TurretSummoningState::Update(q_engine::GameState* controlledState, IDType enemyID, double)
{
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	if (!animation.IsPlaying())
	{
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.SetCurrentState<TurretMissingState>(controlledState);
	}
}
