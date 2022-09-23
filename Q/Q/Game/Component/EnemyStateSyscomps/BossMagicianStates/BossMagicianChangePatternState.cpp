/*
	File Name: BossMagicianChangePatternState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossMagicianChangePatternState.h"


#include "BossMagicianAttack1State.h"
#include "BossMagicianAttack2State.h"
#include "BossMagicianAttack3State.h"
#include "BossMagicianAttack4State.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Core/RandomMachine.h"
#include "Engine/Engine.h"
#include "Engine/GameState.h"

constexpr int MAX_STATE_NUM = 4;
bool BossMagicianChangePatternState::isStart = false;

void BossMagicianChangePatternState::Load([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID)
{
	
}

void BossMagicianChangePatternState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, [[maybe_unused]] double deltaTime)
{
	int StateNum = q_engine::Engine::GetEngineComponent<q_engine::RandomMachine>().GetRandomValue(0, MAX_STATE_NUM - 1);
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	if(isStart == false)
	{
		isStart = true;
		StateNum = 1;
	}
	//StateNum = 1;
	switch(StateNum)
	{
	case 0:
		enemyState.SetCurrentState<BossMagicianAttack1State>(controlledState);
		break;
	case 1:
		enemyState.SetCurrentState<BossMagicianAttack2State>(controlledState);
		break;
	case 2:
		enemyState.SetCurrentState<BossMagicianAttack3State>(controlledState);
		break;
	case 3:
		enemyState.SetCurrentState<BossMagicianAttack4State>(controlledState);
		break;
	default:
		throw std::logic_error("Boss State DNE");
		break;
	}
}
