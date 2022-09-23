/*
	File Name: BossAssassinHelperFunctions.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossAssassinHelperFunctions.h"


#include "Component/BossAssassinComponent.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/GameState.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/VelocityComponent.h"
constexpr float offset = 100;

void BossAssassinMove(q_engine::GameState* controlledState, q_engine::IDType enemyID, double dt)
{
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& bossComp = controlledState->GetComponents<BossAssassinComponent>()[enemyID];
	auto& enemyPosition = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
	auto& playerPosition = controlledState->GetComponents<q_engine::TransformComponent>()[bossComp.GetPlayerID()];
	auto& enemyVelocity = controlledState->GetComponents<q_engine::VelocityComponent>()[enemyID];
	auto& playerGravity = controlledState->GetComponents<q_engine::GravityComponent>()[bossComp.GetPlayerID()];
	
	if(playerPosition.GetPositionY() > enemyPosition.GetPositionY() + offset && playerGravity.IsOnGround()
		&& (enemyVelocity.GetVelocityY() < 0.f || enemyState.isJumping() == false))
	{
		enemyVelocity.SetVelocity(enemyState.GetMaxJumpVelocity());
	}


	if(playerPosition.GetPositionY() < enemyPosition.GetPositionY())
	{
		
	}
	else
	{
		if (playerPosition.GetPositionX() > enemyPosition.GetPositionX())
		{
			enemyState.MoveRight();
		}
		else
		{
			enemyState.MoveLeft();
		}
	}

}