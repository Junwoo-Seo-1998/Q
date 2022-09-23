/*
	File Name: LevelHeaders.inl
	Project Name: Q
	Author(s):
		Primary: Noflex
		Secondary: Minjae Kyung
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../Engine/GameStateManager.h"


#include "System/CamLockUpdateSystem.h"
#include "Engine/Types.h"
#include "System/NewPlayerInventorySystem.h"
#include "System/PlayerItemSystem.h"
#include "System/PlayerWeaponSystem.h"
#include "System/ProjectileSystem.h"
#include "../Engine/GameState.h"
#include "Engine/System/LambdaEventSystem.h"
#include "Engine/System/SimpleActionSystem.h"
#include "System/FXAnimLuaScriptsUpdateSystem.h"
#include "../Game/System/DropItemVisualSystem.h"
#include "System/CubePickUpSystem.h"

class q_engine::GameState;

inline void LevelSystemBindingLoader(q_engine::GameState* level, q_engine::Entity<PlayerEntity>* player, q_engine::Entity<WeaponEntity>* weapon, q_engine::Entity<q_engine::CameraEntity>* camera)
{
	using namespace q_engine;

	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();

	Binding* test = new Binding("UIAdd", level->GetStateType());
	test->BindInput(InputType::KEY_PRESS, Key::ESC);
	inputManager.AddBinding(test);
	inputManager.AddCallback("UIAdd", [&]()
		{
			Engine::GetEngineComponent<SoundComponent>().PauseAll();
			//Engine::GetEngineComponent<SoundComponent>().PauseSoundBG(0);
			//Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect(0);
			Engine::GetEngineComponent<GameStateManager>().AddState(GameStateType::IN_GAME_MENU, std::make_shared<InGameMenuState>());
		});

	//Systems..

	level->AddUpdateSystem<MovingSystem>();
	level->AddUpdateSystem<PhysicsSystem>();
	level->AddUpdateSystem<CollisionSystem>();
	level->AddUpdateSystem<CollisionResolveSystem>();
	level->AddUpdateSystem<GamePlayLogicUpdateSystem>();
	level->AddUpdateSystem<EnemyStateSystem>();
	level->AddUpdateSystem<FollowSystem>();
	level->AddUpdateSystem<CamLockUpdateSystem>();
	level->AddUpdateSystem<TimerManagerSystem>();
	level->AddUpdateSystem<BombExplodeSystem>();
	level->AddUpdateSystem<DropItemVisualSystem>();
	level->AddUpdateSystem<OffsetSystem>();
	level->AddUpdateSystem<ItemPickUpSystem>();  // Should be after CollisionResolveSystem
	level->AddUpdateSystem<CubePickUpSystem>();
	level->AddUpdateSystem<CameraShakeUpdateSystem>();
	level->AddUpdateSystem<EnemyWeaponSystem>();
	level->AddUpdateSystem<StateMoveSystem>();
	level->AddUpdateSystem<TextureBoxUpdateSystem>();
	level->AddUpdateSystem<NewPlayerInventorySystem>();
	level->AddUpdateSystem<PlayerWeaponSystem>();
	level->AddUpdateSystem<PlayerItemSystem>();
	level->AddUpdateSystem<ProjectileSystem>();
	level->AddUpdateSystem<LuaColliderDispatcherSystem>();
	level->AddUpdateSystem<DamageSystem>();
	level->AddUpdateSystem<SimpleActionSystem>();
	level->AddUpdateSystem<LambdaEventSystem>();
	level->AddUpdateSystem<FXAnimLuaScriptsUpdateSystem>();


	level->AddDrawSystem<TextureDrawSystem>();
	level->AddDrawSystem<CollisionDebugDrawSystem>();
	level->AddDrawSystem<AnimationUpdateDrawSystem>();



}
