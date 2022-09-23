/*
	File Name: EnemyStateHelper.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EnemyStateHelper.h"

#include "../Engine/GameState.h"
#include "../Engine/Component/VelocityComponent.h"
#include "../EnemyStateComponent.h"
#include "../HealthComponent.h"
#include "../JumpTriggerComponent.h"

#include "../../../Engine/Component/CollisionComponent.h"
#include "../../../Engine/Component/GravityComponent.h"
#include "../../../Engine/Component/TransformComponent.h"
#include "../../../Engine/Component/ObjectStateComponent.h"
#include "../../../Engine/Component/RaycastComponent.h"

#include "../../../Engine/Event/RayTriggerEvent.h"
#include "../../../Engine/Event/TriggerEvent.h"

#include "../../Entity/MapEntity.h"
#include "../../Entity/BombEntity.h"
#include "../../Entity/JumpTriggerEntity.h"
#include "../../Entity/WeaponEntity.h"
#include "Component/FXAnimLuaScriptsComponent.h"
#include "Component/NewWeaponComponent.h"
#include "Engine/Graphics/Particle.h"
#include "PlayTestTrace.h"


bool temp_helper_isInBox(q_engine::StaticRectCollisionComponent rect, glm::vec2 point)
{
	Rect box = rect.GetColliderBox();
	return box.Left() < point.x && point.x < box.Right() && box.Bottom() < point.y&& point.y < box.Top();
}

//temp code
bool isCliffWall(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	Rect enemyBox = controlledState->GetComponents<q_engine::RectCollisionComponent>()[enemyID].GetColliderBox();
	EnemyStateComponent& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	glm::vec2 enemyCliffCheckPoint = enemyState.isMovingRight() ? glm::vec2{ enemyBox.Right() + 2.f, enemyBox.Bottom() - 1.f } : glm::vec2{ enemyBox.Left() - 2.f, enemyBox.Bottom() - 1.f };
	glm::vec2 enemyWallCheckPoint1 = enemyState.isMovingRight() ? glm::vec2{ enemyBox.Right() + 2.f, enemyBox.Bottom() + 1.f } : glm::vec2{ enemyBox.Left() - 2.f,enemyBox.Bottom() + 1.f };
	glm::vec2 enemyWallCheckPoint2 = enemyState.isMovingRight() ? glm::vec2{ enemyBox.Right() + 2.f, enemyBox.Top() - 1.f } : glm::vec2{ enemyBox.Left() - 2.f,enemyBox.Top() - 1.f };

	q_engine::Level* level = reinterpret_cast<q_engine::Level*>(controlledState);
	
	bool isCliff = level->GetTileByCoord(enemyCliffCheckPoint) == q_engine::TileType::Null;
	bool isWall = level->GetTileByCoord(enemyWallCheckPoint1) != q_engine::TileType::Null ||
		level->GetTileByCoord(enemyWallCheckPoint2) != q_engine::TileType::Null;


	return isCliff || isWall;
}

glm::vec2 temp_NuckBack(200, 600);

void BasicEnemyTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	q_engine::IDType CollidedID = triggerEvent->mTriggerEntityID;

	if (controlledState->GetComponents<HealthComponent>()[enemyID].IsDead())
	{
		return;
	}

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	auto& enemyVelocity = controlledState->GetComponents<q_engine::VelocityComponent>()[enemyID];

	using namespace q_engine;

	if (triggerEvent->mTriggerEntityTypeID == q_engine::GetTypeID<BombEntity>())
	{
		auto& BombTransform = controlledState->GetComponents<q_engine::TransformComponent>()[CollidedID];

		glm::vec2 relativeTransform = BombTransform.GetPosition() - enemyTransform.GetPosition();
		if (relativeTransform.x > 0)
			enemyState.Jump(2.f * glm::vec2{ -temp_NuckBack.x,temp_NuckBack.y });
		else
			enemyState.Jump(2.f * temp_NuckBack);

		auto& enemyHP = controlledState->GetComponents<HealthComponent>()[enemyID];
		enemyHP.ChangeCurHealth(-BombEntity::DAMAGE);

		static const std::string sound_paths[4] = { "enemy_hit1","enemy_hit2" ,"enemy_hit3" ,"enemy_hit_dead" };
		unsigned sound_idx = q_engine::Engine::GetEngineComponent<q_engine::RandomMachine>().GetRandomValue(0, 2);
		if (enemyHP.GetCurHealth() <= 0)
		{
			sound_idx = 3;
		}
		q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PlaySoundEffect(sound_paths[sound_idx], false);

		Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetUpdateSystem<ParticleSystem>().Find("Test")->Emit(50, enemyTransform.GetPosition(), enemyVelocity.GetVelocity(),
			glm::vec2{ -50, 50 }, glm::vec2{ 50, 200 }, glm::vec2{ 3.f, 5.f }, glm::vec2{ 2.f, 3.f }, 200.f);
	}
	else if (triggerEvent->mTriggerEntityTypeID == q_engine::GetTypeID<DamageTileEntity>()) 
	{
		enemyState.KillThis();

		auto& FXComponents = controlledState->GetComponents<FXAnimLuaScriptsComponent>();
		
		static const std::string defaultSFX{ "Explode" };

		q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PlaySoundEffect(defaultSFX + std::to_string(Engine::GetEngineComponent<RandomMachine>().GetRandomValue(1, 5)), false);



		FXComponents[enemyID].RunScript("DeadExplode");
		FXComponents[enemyID].RunScript("DeadExplode");
		FXComponents[enemyID].RunScript("DeadExplode");
		FXComponents[enemyID].RunScript("DeadExplode");
	}
}

void EnemyJumpTriggerEvent(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::TriggerEvent* triggerEvent)
{
	if (triggerEvent->mTriggerEntityTypeID != q_engine::GetTypeID<JumpTriggerEntity>())
		return;

	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];

	auto& JumpTrigger = controlledState->GetComponents<JumpTriggerComponent>()[triggerEvent->mCollideWithEntityID];


	glm::vec2 relativePosition = enemyState.GetLastSeenPlayerPosition() - enemyTransform.GetPosition();


	if (isBigger(enemyState.GetMaxJumpVelocity(), JumpTrigger.GetRequireVelocity()))
	{
		if (JumpTrigger.isToUpper() && relativePosition.y > 0)
			enemyState.Jump(JumpTrigger.GetRequireVelocity());
		else if (!JumpTrigger.isToUpper() && relativePosition.y <= 0)
			enemyState.Jump(JumpTrigger.GetRequireVelocity());
	}
}

double cross(const glm::vec2& thisPtr, const glm::vec2& other)
{
	return thisPtr.x * other.y - thisPtr.y * other.x;
}

bool isPlayerOnSight(q_engine::GameState* controlledState, q_engine::IDType enemyID, q_engine::RayTriggerEvent* triggerEvent)
{
	auto& RayComponent = controlledState->GetComponents<q_engine::RaycastComponent>()[enemyID];
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];

	if (triggerEvent->rayColided.typeID == q_engine::GetTypeID<PlayerEntity>() 
		&& RayComponent.GetDistanceData().at(triggerEvent->rayIndex) < enemyState.GetSightDistance())
	{
		
		auto& playerTransform = controlledState->GetComponents<q_engine::TransformComponent>()[triggerEvent->rayColided.entityID];
		auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
		
		if(controlledState->GetComponents<HealthComponent>()[triggerEvent->rayColided.entityID].IsDead())
		{
			return false;
		}

		glm::vec2 relativePosition = playerTransform.GetPosition() - enemyTransform.GetPosition();

		if(enemyState.isLookingPlayer())
		{
			enemyState.UpdateLookingDirection(atan2(relativePosition.y, relativePosition.x));
		}
		else if(enemyState.isMovingLeft())
		{
			enemyState.UpdateLookingDirection(PI);
		}
		else if (enemyState.isMovingRight())
		{
			enemyState.UpdateLookingDirection(0.f);
		}
		
		glm::vec2 upFov = glm::vec2(cos(enemyState.GetLookingDirection() + enemyState.GetSightFOV()), sin(enemyState.GetLookingDirection() + enemyState.GetSightFOV()));
		glm::vec2 downFov = glm::vec2(cos(enemyState.GetLookingDirection() - enemyState.GetSightFOV()), sin(enemyState.GetLookingDirection() - enemyState.GetSightFOV()));

		if (cross(relativePosition, upFov) >= 0 &&
			cross(relativePosition, downFov) <= 0)
		{
			enemyState.UpdateLastSeenPlayerPosition(playerTransform.GetPosition());
			return true;
		}
	}
	return false;
}
