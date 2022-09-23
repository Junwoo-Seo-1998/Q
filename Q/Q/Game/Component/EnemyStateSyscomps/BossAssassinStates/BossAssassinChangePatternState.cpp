/*
	File Name: BossAssassinChangePatternState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Component/EnemyStateSyscomps/BossAssassinStates/BossAssassinChangePatternState.h"

#include "Component/EnemyStateComponent.h"
#include "Engine/Engine.h"
#include "BossAssassinAttack1State.h"
#include "BossAssassinAttack2State.h"
#include "BossAssassinAttack3State.h"
#include "BossAssassinAttack4State.h"
#include "BossAssassinDyingState.h"
#include "BossAssassinHelperFunctions.h"
#include "EObjectZDepth.h"
#include "Component/BossAssassinComponent.h"
#include "Component/HealthComponent.h"
#include "Engine/Component/AnimationComponent.h"

constexpr int MAX_STATE_NUM = 4;

void BossAssassinChangePatternState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("AssassinWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));


	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(1);
	enemyState.SetLookingPlayer(true);
	enemyState.SetMoveByInput(true);

	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	bossComp.ResetTime();
	bossComp.ResetIndicatorNum();
}

void BossAssassinChangePatternState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	int StateNum = q_engine::Engine::GetEngineComponent<q_engine::RandomMachine>().GetRandomValue(0, MAX_STATE_NUM - 1);
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	bossComp.UpdateTime(deltaTime);
	if(bossComp.GetTime() < 1.f)
	{
		BossAssassinMove(controlledState, enemyID, deltaTime);
		auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
		if (enemyHP.IsDead())
		{
			enemyState.SetCurrentState<BossAssassinDyingState>(controlledState);
		}
		return;
	}

	
	switch (StateNum)
	{
	case 0:
		enemyState.SetCurrentState<BossAssassinAttack1State>(controlledState);
		break;
	case 1:
		enemyState.SetCurrentState<BossAssassinAttack2State>(controlledState);
		break;
	case 2:
		enemyState.SetCurrentState<BossAssassinAttack3State>(controlledState);
		break;
	case 3:
		enemyState.SetCurrentState<BossAssassinAttack4State>(controlledState);
		break;
	default:
		throw std::logic_error("Boss State DNE");
		break;
	}
}