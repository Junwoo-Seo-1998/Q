/*
	File Name: BossMagicianDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossMagicianDyingState.h"



#include "BossMagicianChangePatternState.h"
#include "EObjectZDepth.h"
#include "Component/DropWeaponComponent.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/StateMoveComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Entity/ItemEntity.h"
#include "Entity/MapEntity.h"
#include "Levels/Level5State.h"

#include "Engine/Engine.h"

void BossMagicianDyingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("ScoutDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	BossMagicianChangePatternState::isStart = false;

	constexpr float slowMotionChance = 1.f;
	if (Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, 1.f) < slowMotionChance)
	{
		Engine::ApplySlowMotion(2.f, 0.2f);
	}
}

void BossMagicianDyingState::Update(q_engine::GameState* controlledState, q_engine::IDType enemyID, double deltaTime)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	auto& enemyVelocity = controlledState->GetComponents<q_engine::VelocityComponent>()[enemyID];
	enemyVelocity.SetVelocity(glm::vec2{ 0 });
	
	if (animation.IsAnimationEnded())
	{
		auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();
		
		auto portal = controlledState->AddEntity<q_engine::PortalEntity>();
		controlledState->AddComponent<q_engine::AnimationComponent>(portal, "PortalAnim"_hashID,  static_cast<int>(EObjectZDepth::IN_GAME_PLAY_MAP_1));
		controlledState->AddComponent<q_engine::TransformComponent>(portal, glm::vec2(1250, 1300), 0.f, glm::vec2{ 1.5, 1.5 });
		controlledState->AddComponent<q_engine::WidthHeightComponent>(portal, 20.f, 200.f);
		controlledState->AddComponent<q_engine::RectCollisionComponent>(portal, enemyTransform.GetPosition(), 48.f, 48.f, true, true);
		controlledState->AddComponent<q_engine::OffsetComponent>(portal);
		controlledState->AddComponent<q_engine::VelocityComponent>(portal);
		controlledState->AddComponent<q_engine::GravityComponent>(portal);
		
		std::shared_ptr<q_engine::Level> newLevel5 = std::make_shared<Level5State>();
		controlledState->AddComponent<q_engine::StateMoveComponent>(portal, newLevel5);

		ItemEntity* const droppedItem = controlledState->AddEntity<ItemEntity>();
		controlledState->AddComponent<DropWeaponComponent>(droppedItem, "ShotGun");
		controlledState->AddComponent<TransformComponent>(droppedItem, enemyTransform.GetPosition() + glm::vec2{0,50});
		controlledState->AddComponent<q_engine::VelocityComponent>(droppedItem);
		controlledState->AddComponent<q_engine::GravityComponent>(droppedItem);
		controlledState->AddComponent<TextureComponent>(droppedItem, "ShotGunIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
		controlledState->AddComponent<OffsetComponent>(droppedItem);
		controlledState->AddComponent<WidthHeightComponent>(droppedItem, 32.f, 32.f);
		controlledState->AddComponent<RectCollisionComponent>(droppedItem, glm::vec2{ -200, 200 }, 32.f, 32.f, false, true);
	}
}
