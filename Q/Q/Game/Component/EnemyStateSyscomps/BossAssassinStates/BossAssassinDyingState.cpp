/*
	File Name: BossAssassinDyingState.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "BossAssassinDyingState.h"


#include "EObjectZDepth.h"
#include "Component/DropWeaponComponent.h"
#include "Component/EnemyStateComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/GameState.h"
#include "Engine/Engine.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/StateMoveComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Entity/ItemEntity.h"
#include "Entity/MapEntity.h"
#include "Levels/Level5State.h"
#include "Levels/Level7State.h"


void BossAssassinDyingState::Load(q_engine::GameState* controlledState, q_engine::IDType enemyID)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];
	animation.SetAnimation("AssassinDyingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
	auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
	enemyState.SetSpeedMultiplier(0.f);
	
	constexpr float slowMotionChance = 1.f;
	if (Engine::GetEngineComponent<RandomMachine>().GetRandomValue(0.f, 1.f) < slowMotionChance)
	{
		Engine::ApplySlowMotion(2.f, 0.2f);
	}
}

void BossAssassinDyingState::Update(q_engine::GameState * controlledState, q_engine::IDType enemyID, [[maybe_unused]] double deltaTime)
{
	using namespace q_engine;
	q_engine::AnimationComponent& animation = controlledState->GetComponents<q_engine::AnimationComponent>()[enemyID];

	if (animation.IsAnimationEnded())
	{
		auto& enemyTransform = controlledState->GetComponents<q_engine::TransformComponent>()[enemyID];
		auto& enemyState = controlledState->GetComponents<EnemyStateComponent>()[enemyID];
		enemyState.KillThis();

		auto portal = controlledState->AddEntity<q_engine::PortalEntity>();
		controlledState->AddComponent<q_engine::AnimationComponent>(portal, "PortalAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_MAP_1));
		controlledState->AddComponent<q_engine::TransformComponent>(portal, glm::vec2(1250, 1300), 0.f, glm::vec2{ 1.5, 1.5 });
		controlledState->AddComponent<q_engine::WidthHeightComponent>(portal, 20.f, 200.f);
		controlledState->AddComponent<q_engine::RectCollisionComponent>(portal, enemyTransform.GetPosition(), 48.f, 48.f, true, true);
		controlledState->AddComponent<q_engine::OffsetComponent>(portal);
		controlledState->AddComponent<q_engine::VelocityComponent>(portal);
		controlledState->AddComponent<q_engine::GravityComponent>(portal);

		std::shared_ptr<q_engine::Level> newLevel7 = std::make_shared<Level7State>();
		controlledState->AddComponent<q_engine::StateMoveComponent>(portal, newLevel7);

		ItemEntity* const droppedItem = controlledState->AddEntity<ItemEntity>();
		controlledState->AddComponent<DropWeaponComponent>(droppedItem, "Rocket");
		controlledState->AddComponent<TransformComponent>(droppedItem, enemyTransform.GetPosition() + glm::vec2{ 0,50 });
		controlledState->AddComponent<q_engine::VelocityComponent>(droppedItem);
		controlledState->AddComponent<q_engine::GravityComponent>(droppedItem);
		controlledState->AddComponent<TextureComponent>(droppedItem, "RocketIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
		controlledState->AddComponent<OffsetComponent>(droppedItem);
		controlledState->AddComponent<WidthHeightComponent>(droppedItem, 48.f, 48.f);
		controlledState->AddComponent<RectCollisionComponent>(droppedItem, glm::vec2{ -200, 200 }, 32.f, 32.f, false, true);
	}
}

void BossAssassinDyingState::OnTriggerEvent([[maybe_unused]] q_engine::GameState* controlledState, [[maybe_unused]] q_engine::IDType enemyID, [[maybe_unused]] q_engine::TriggerEvent* triggerEvent)
{
}
