/*
	File Name: GameState.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun choi
		Secondary: Junwoo seo, Hoseob jeong, Minjae kyung, yoonki kim
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GameState.h"

#include <iostream>

#include "Engine.h"
#include "../Engine/Component/TextureBoxComponent.h"
#include "../Engine/Graphics/Particle.h"
#include "../Levels/LevelHeaders.h"
#include "Component/BossAssassinComponent.h"
#include "Component/BossMagicianComponent.h"
#include "Component/DeathItemDropComponent.h"
#include "Component/DropCubeComponent.h"
#include "Component/DropItemComponent.h"
#include "Component/DropWeaponComponent.h"
#include "Component/FXAnimLuaScriptsComponent.h"
#include "Component/NewItemComponent.h"
#include "Component/NewPlayerInventorySlots.h"
#include "Component/NewWeaponComponent.h"
#include "Levels/Boss1State.h"
#include "Levels/Boss2State.h"
#include "Levels/TutorialState.h"
#include "Levels/Level1State.h"
#include "Levels/Level2State.h"
#include "Levels/Level3State.h"
#include "Levels/Level4State.h"
#include "Levels/Level5State.h"
#include "Levels/Level6State.h"
#include "Levels/Level7State.h"
#include "Levels/MainMenuState.h"
#include "Scripting/ScriptManager.h"
#include "System/NewPlayerInventorySystem.h"
#include "System/PlayerWeaponSystem.h"
#include "System/ProjectileSystem.h"

namespace q_engine
{
	void Level::Load()
	{
		//mLevelNum = MapLevel::BossMap;
		using namespace q_engine;
		Engine::GetEngineComponent<EventManager>().ClearEvent();
		ScriptManager::Get().SetCurrentScene(this);
		//camera Entity

		auto camera = AddEntity<CameraEntity>();
		AddComponent<TransformComponent>(camera);
		AddComponent<CameraShakeComponent>(camera);
		std::cout << this << std::endl;
		//Map Entity AND Portal Entity
		glm::vec2 offset = { 48.f,0.f };
		TileMapParser map_parser;
		std::string path = MapGeneration::GetFile(static_cast<int>(GetLevelNum()));;
		Layer layer{ map_parser.Parse(path,offset) };
		MapEntity* Map = AddEntity<MapEntity>();
		PortalEntity* Last = nullptr;
		BackGroundEntity* back = AddEntity<BackGroundEntity>();
		DamageTileEntity* damage = AddEntity<DamageTileEntity>();
		glm::vec2 tile_size = map_parser.getTileSize();
		glm::vec<2, int> tile_num = map_parser.GetMapTileNum();
		const float tile_scale = map_parser.getTileScale();
		glm::vec2 imageSize = map_parser.getImageSize();
		glm::vec2 player_start_pos = map_parser.GetStartPos();
		auto objects_layer = map_parser.GetObjectlayersInfo();

		q_engine::Debugger::GetLogger().AddMessage("Using map / " + path, q_engine::SeverityLevel::INFORMATIONAL);

		//Player Entity
		PlayerEntity* player;

		if (!IsEntityTypeExist<PlayerEntity>())
		{
			player = AddEntity<PlayerEntity>();
			AddComponent<HealthComponent>(player, 200);
			AddComponent<NewPlayerInventorySlots>(player);
		}
		else
		{
			player = &GetEntities<PlayerEntity>().Front();
		}

		AddComponent<FollowComponent>(camera, player->GetEntityID(), 4.f);
		AddComponent<FXAnimLuaScriptsComponent>(player);
		AddComponent<TransformComponent>(player, player_start_pos.x, player_start_pos.y, 0.f, 1.5f, 1.7f);
		AddComponent<VelocityComponent>(player);
		AddComponent<GravityComponent>(player);
		AddComponent<WidthHeightComponent>(player, 35.f, 73.f);
		AddComponent<RectCollisionComponent>(player, glm::vec2{ player_start_pos.x, player_start_pos.y }, 35.f, 73.f);
		AddComponent<UserInputComponent>(player);
		AddComponent<DragAccelComponent>(player);
		AddComponent<AnimationComponent>(player, "NewPlayerIdleDaggerAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_PLAYER_OBJ));
		AddComponent<RaycastComponent>(player, 1);
		AddComponent<ObjectStateComponent>(player, 35.f, 73.f);

		AddComponent<OffsetComponent>(player, glm::vec2{ 25,0 });

		//new item
		AddComponent<NewItemComponent>(player);

		/*Enemy & enemyWeapon*/

		/*How to get the enemy position and Trigger property value*/

		auto enemies = objects_layer.find("Warrior")->second;

		int enemySize = static_cast<int>(enemies.size());

		for (int i = 0; i < enemySize; i++)
		{
			auto enemy = AddEntity<EnemyEntity>();
			auto enemyWeapon = AddEntity<EnemyWeaponEntity>();
			AddComponent<FXAnimLuaScriptsComponent>(enemy);
			AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 2.f, 2.f);
			AddComponent<AnimationComponent>(enemy, "WarriorMissingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
			AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
			AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Warrior, enemyWeapon->GetEntityID(), glm::vec2{ 100,300 }, glm::vec2{ 0,600 });
			AddComponent<DeathItemDropComponent>(enemy);
			AddComponent<HealthComponent>(enemy, 200);
			AddComponent<GravityComponent>(enemy);
			AddComponent<WidthHeightComponent>(enemy, 50.f, 70.f);
			AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
			AddComponent<DragAccelComponent>(enemy);
			AddComponent<RaycastComponent>(enemy, 1, 10.f);
			AddComponent<ObjectStateComponent>(enemy);
			AddComponent<OffsetComponent>(enemy, glm::vec2{ 25.f ,0.f });

			AddComponent<TransformComponent>(enemyWeapon, enemies[i]->pos + glm::vec2{ 110, 40 });
			AddComponent<WidthHeightComponent>(enemyWeapon, 20.f, 40.f);
			AddComponent<RectCollisionComponent>(enemyWeapon, enemies[i]->pos + glm::vec2{ 110, 40 }, 20.f, 10.f, false, true);
			AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 30,10 }, 10);
			AddComponent<OffsetComponent>(enemyWeapon);
		}

		enemies = objects_layer.find("Tanker")->second;
		enemySize = static_cast<int>(enemies.size());

		for (int i = 0; i < enemySize; i++)
		{
			auto	enemy = AddEntity<EnemyEntity>();
			auto	enemyWeapon = AddEntity<EnemyWeaponEntity>();

			AddComponent<FXAnimLuaScriptsComponent>(enemy);
			AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 2.5f, 2.5f);
			AddComponent<AnimationComponent>(enemy, "TankerMissingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
			AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
			AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Tanker, enemyWeapon->GetEntityID(), glm::vec2{ 60,300 }, glm::vec2{ 0,500 },
				600.f, PI * 0.25f);
			AddComponent<DeathItemDropComponent>(enemy);
			AddComponent<HealthComponent>(enemy, 400);
			AddComponent<GravityComponent>(enemy);
			AddComponent<WidthHeightComponent>(enemy, 52.f, 75.f);
			AddComponent<RectCollisionComponent>(enemy, glm::vec2{ 48.f,64.f }, 48.f, 60.f);
			AddComponent<DragAccelComponent>(enemy);
			AddComponent<RaycastComponent>(enemy, 1, 10.f);
			AddComponent<ObjectStateComponent>(enemy);
			AddComponent<OffsetComponent>(enemy, glm::vec2{ 64.f ,0.f }, glm::vec2{ -40.f,0.f });

			AddComponent<TransformComponent>(enemyWeapon, enemies[i]->pos + glm::vec2{ 110, 40 });
			AddComponent<WidthHeightComponent>(enemyWeapon, 52.f, 48.f);
			AddComponent<RectCollisionComponent>(enemyWeapon, enemies[i]->pos + glm::vec2{ 110, 40 }, 20.f, 10.f, false, true);
			AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 0,0 }, 1);
			AddComponent<OffsetComponent>(enemyWeapon);
		}

		enemies = objects_layer.find("Scout")->second;
		enemySize = static_cast<int>(enemies.size());

		for (int i = 0; i < enemySize; i++)
		{
			auto	enemy = AddEntity<EnemyEntity>();
			auto	enemyWeapon = AddEntity<EnemyWeaponEntity>();

			glm::vec2 enemyPos1(1000, 200);
			AddComponent<FXAnimLuaScriptsComponent>(enemy);
			AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 2.f, 2.f);
			AddComponent<AnimationComponent>(enemy, "ScoutMissingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
			AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
			AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Scout, enemyWeapon->GetEntityID(), glm::vec2{ 150,300 }, glm::vec2{ 0,500 },
				500.f, PI * 4.f / 5.f);
			AddComponent<DeathItemDropComponent>(enemy);
			AddComponent<HealthComponent>(enemy, 200);
			AddComponent<GravityComponent>(enemy);
			AddComponent<WidthHeightComponent>(enemy, 50.f, 70.f);
			AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 50.f, 70.f);
			AddComponent<DragAccelComponent>(enemy);
			AddComponent<RaycastComponent>(enemy, 1, 10.f);
			AddComponent<ObjectStateComponent>(enemy);
			AddComponent<OffsetComponent>(enemy, glm::vec2{ 32.f ,0.f }, glm::vec2{ -10.f ,0.f });
		}

		
		enemies = objects_layer.find("Engineer")->second;
		enemySize = static_cast<int>(enemies.size());

		for (int i = 0; i < enemySize; i++)
		{
			auto enemy = AddEntity<EnemyEntity>();
			auto enemyWeapon = AddEntity<EnemyWeaponEntity>();
			AddComponent<FXAnimLuaScriptsComponent>(enemy);
			AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 2.f, 2.f);
			AddComponent<AnimationComponent>(enemy, "EngineerWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
			AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
			AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Engineer, enemyWeapon->GetEntityID(), glm::vec2{ 100,300 }, glm::vec2{ 0,600 });
			AddComponent<DeathItemDropComponent>(enemy);
			AddComponent<HealthComponent>(enemy, 200);
			AddComponent<GravityComponent>(enemy);
			AddComponent<WidthHeightComponent>(enemy, 56.f, 70.f);
			AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
			AddComponent<DragAccelComponent>(enemy);
			AddComponent<RaycastComponent>(enemy, 1, 10.f);
			AddComponent<ObjectStateComponent>(enemy);
			AddComponent<OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });
		}

		enemies = objects_layer.find("Bomber")->second;
		enemySize = static_cast<int>(enemies.size());

		for (int i = 0; i < enemySize; i++)
		{
		auto enemy = AddEntity<EnemyEntity>();
		auto enemyWeapon = AddEntity<EnemyWeaponEntity>();
		AddComponent<FXAnimLuaScriptsComponent>(enemy);
		AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 2.f, 2.f);
		AddComponent<AnimationComponent>(enemy, "WardroidWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
		AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Bomber, enemyWeapon->GetEntityID(), glm::vec2{ 100,300 }, glm::vec2{ 0,600 });
		AddComponent<DeathItemDropComponent>(enemy);
		AddComponent<HealthComponent>(enemy, 200);
		AddComponent<GravityComponent>(enemy);
		AddComponent<WidthHeightComponent>(enemy, 56.f, 70.f);
		AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
		AddComponent<DragAccelComponent>(enemy);
		AddComponent<RaycastComponent>(enemy, 1, 10.f);
		AddComponent<ObjectStateComponent>(enemy);
		AddComponent<OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });

		AddComponent<TransformComponent>(enemyWeapon, enemies[i]->pos);
		AddComponent<WidthHeightComponent>(enemyWeapon, 56.f, 70.f);
		AddComponent<RectCollisionComponent>(enemyWeapon, enemies[i]->pos, 20.f, 10.f, false, true);
		AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 0,0 }, 2, DamageType::POISONDAMAGE);
		AddComponent<OffsetComponent>(enemyWeapon);
		}

		enemies = objects_layer.find("FlameThrower")->second;
		enemySize = static_cast<int>(enemies.size());

		for (int i = 0; i < enemySize; i++)
		{
		auto enemy = AddEntity<EnemyEntity>();
		auto enemyWeapon = AddEntity<EnemyWeaponEntity>();
		AddComponent<FXAnimLuaScriptsComponent>(enemy);
		AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 3.f, 3.f);
		AddComponent<AnimationComponent>(enemy, "FlamethrowerBotWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
		AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
		AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::FlameThrower, enemyWeapon->GetEntityID(), glm::vec2{ 100,300 }, glm::vec2{ 0,600 });
	 	AddComponent<DeathItemDropComponent>(enemy);
		AddComponent<HealthComponent>(enemy, 200);
		AddComponent<GravityComponent>(enemy);
		AddComponent<WidthHeightComponent>(enemy, 56.f, 70.f);
		AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
		AddComponent<DragAccelComponent>(enemy);
		AddComponent<RaycastComponent>(enemy, 1, 10.f);
		AddComponent<ObjectStateComponent>(enemy);
		AddComponent<OffsetComponent>(enemy, glm::vec2{ 20.f ,0.f }, glm::vec2{ 0, 0 });

		AddComponent<TransformComponent>(enemyWeapon, enemies[i]->pos + glm::vec2{ 110, 40 });
		AddComponent<WidthHeightComponent>(enemyWeapon, 150.f, 40.f);
		AddComponent<RectCollisionComponent>(enemyWeapon, enemies[i]->pos + glm::vec2{ 110, 40 }, 20.f, 10.f, false, true);
		AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 30,10 }, 2, DamageType::DOTDAMGAE);
		AddComponent<OffsetComponent>(enemyWeapon);
		}

		if (objects_layer.find("BossMagician") != objects_layer.end())
		{
			enemies = objects_layer.find("BossMagician")->second;
			enemySize = static_cast<int>(enemies.size());

			for (int i = 0; i < enemySize; i++)
			{
				auto	enemy = AddEntity<EnemyEntity>();
				auto	enemyWeapon = AddEntity<EnemyWeaponEntity>();
				AddComponent<FXAnimLuaScriptsComponent>(enemy);
				AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 4.f, 4.f);
				AddComponent<AnimationComponent>(enemy, "ScoutMissingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
				AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
				AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::BossMagician, enemyWeapon->GetEntityID(), glm::vec2{ 60,300 }, glm::vec2{ 0,500 },
					1000.f, PI * 4.f / 5.f);
				AddComponent<BossMagicianComponent>(enemy, player->GetEntityID());
				AddComponent<HealthComponent>(enemy, 10000);
				AddComponent<WidthHeightComponent>(enemy, 50.f, 140.f);
				AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 50.f, 140.f);
				AddComponent<DragAccelComponent>(enemy);
				AddComponent<RaycastComponent>(enemy, 1, 10.f);
				AddComponent<ObjectStateComponent>(enemy);
				AddComponent<OffsetComponent>(enemy, glm::vec2{ 0.f ,0.f });

				AddComponent<TransformComponent>(enemyWeapon, enemies[i]->pos);
				AddComponent<WidthHeightComponent>(enemyWeapon, 50.f, 140.f);
				AddComponent<RectCollisionComponent>(enemyWeapon, enemies[i]->pos, 50.f, 140.f, false, true);
				AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 0,0 }, 20);
				AddComponent<OffsetComponent>(enemyWeapon);
			}
		}
		if (objects_layer.find("BossAssassin") != objects_layer.end())
		{
			enemies = objects_layer.find("BossAssassin")->second;
			enemySize = static_cast<int>(enemies.size());

			for (int i = 0; i < enemySize; i++)
			{
				auto	enemy = AddEntity<EnemyEntity>();
				auto	enemyWeapon = AddEntity<EnemyWeaponEntity>();
				AddComponent<FXAnimLuaScriptsComponent>(enemy);
				AddComponent<TransformComponent>(enemy, enemies[i]->pos.x, enemies[i]->pos.y, 0.f, 2.f, 2.f);
				AddComponent<AnimationComponent>(enemy, "AssassinWalkingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
				AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
				AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::BossAssassin, enemyWeapon->GetEntityID(), glm::vec2{ 200,300 }, glm::vec2{ 0,1000 },
					1000.f, PI * 4.f / 5.f);
				AddComponent<BossAssassinComponent>(enemy, player->GetEntityID());
				AddComponent<HealthComponent>(enemy, 10000);
				AddComponent<GravityComponent>(enemy);
				AddComponent<WidthHeightComponent>(enemy, 56.f, 70.f);
				AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
				AddComponent<DragAccelComponent>(enemy);
				AddComponent<RaycastComponent>(enemy, 1, 10.f);
				AddComponent<ObjectStateComponent>(enemy);
				AddComponent<OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });
			}
		}
		

		//Cube pieces
		if (objects_layer.find("Cube") != objects_layer.end())
		{
			auto cube = objects_layer.find("Cube")->second;

			CubeEntity* const droppedCube = AddEntity<CubeEntity>();
			AddComponent<DropCubeComponent>(droppedCube, "cube");
			AddComponent<TransformComponent>(droppedCube, cube[0]->pos, 0.0f, glm::vec2{ 0.4f, 0.4f });
			AddComponent<TextureComponent>(droppedCube, "Cube"_hashID, static_cast<int>(EObjectZDepth::ITEM));
			AddComponent<OffsetComponent>(droppedCube);
			AddComponent<WidthHeightComponent>(droppedCube, 32.f, 32.f);
			AddComponent<VelocityComponent>(droppedCube);
			AddComponent<GravityComponent>(droppedCube);
			AddComponent<RectCollisionComponent>(droppedCube, cube[0]->pos, 32.f, 32.f, true, true);
		}
		
		////////////////////////////////////////////////////////////////MAP///////////////////////////////////////////////////////////////////////////////

		const int layer_size = static_cast<int>(layer.size());

		//Map Entity
		mMapData.mOffset = { 0,48 };
		mMapData.mTileSize = tile_size * tile_scale;
		mMapData.mTiles.resize(tile_num.x * tile_num.y, TileType::Null);
		mMapData.mTileNum = tile_num;
		for (int i = 0; i < layer_size; i++)
		{

			auto texel = map_parser.getTileset().find(layer[i]->tileId)->second->textureRect;

			if (layer[i]->properties->layername == "PortalTile")
			{
				Last = AddEntity<PortalEntity>();
				float scale = 0.5f;
				AddComponent<AnimationComponent>(Last, "PortalAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_MAP_1));
				AddComponent<TransformComponent>(Last, layer[i]->x, layer[i]->y, 0.f, scale, scale);
				AddComponent<WidthHeightComponent>(Last, tile_size.x * tile_scale, tile_size.y * tile_scale);
				AddComponent<StaticRectCollisionComponent>(Last, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, true);
				AddComponent<OffsetComponent>(Last);

				int index = (mMapData.mTileNum.x * ((static_cast<int>(layer[i]->y) - mMapData.mOffset.y) / mMapData.mTileSize.y))
					+ ((static_cast<int>(layer[i]->x) - mMapData.mOffset.x) / mMapData.mTileSize.x);

				mMapData.mTiles[index] = TileType::Normal;
				
				switch (GetLevelNum())
				{
				case MapLevel::Tutorial:
				{
					std::shared_ptr<Level> newLevel1 = std::make_shared<Level1State>();
					AddComponent<StateMoveComponent>(Last, newLevel1);
					break;
				}
				case MapLevel::Level1:
				{
					std::shared_ptr<Level> newLevel2 = std::make_shared<Level2State>();
					AddComponent<StateMoveComponent>(Last, newLevel2);
					GetComponents<OffsetComponent>().find(Last->GetEntityID())->SetFlipped(true);
					break;
				}
				case MapLevel::Level2:
				{
					std::shared_ptr<Level> newLevel3 = std::make_shared<Level3State>();
					AddComponent<StateMoveComponent>(Last, newLevel3);
					break;
				}
				case MapLevel::Level3:
				{
					std::shared_ptr<Level> newLevel4 = std::make_shared<Level4State>();
					AddComponent<StateMoveComponent>(Last, newLevel4);
					break;
				}
				case MapLevel::Level4:
				{
					std::shared_ptr<Level> BossMap = std::make_shared<Boss1State>();
					AddComponent<StateMoveComponent>(Last, BossMap);
					break;
				}
				case MapLevel::Level5:
				{
					std::shared_ptr<Level> newLevel6 = std::make_shared<Level6State>();
					AddComponent<StateMoveComponent>(Last, newLevel6);
					break;
				}
				case MapLevel::Level6:
				{
					std::shared_ptr<Level> bossmap2 = std::make_shared<Boss2State>();
					AddComponent<StateMoveComponent>(Last, bossmap2);
					GetComponents<OffsetComponent>().find(Last->GetEntityID())->SetFlipped(true);
					break;
				}
				case MapLevel::Level7:
				{
					std::shared_ptr<Level> newLevel2 = std::make_shared<Level1State>();
					AddComponent<StateMoveComponent>(Last, newLevel2);
					break;
				}
				}
				Binding* test = new Binding("ToNextMap", GetStateType());
				test->BindInput(InputType::KEY_PRESS, Key::P);
				Engine::GetEngineComponent<InputManager>().RemoveBinding("ToNextMap");
				Engine::GetEngineComponent<InputManager>().RemoveCallback("ToNextMap");
				Engine::GetEngineComponent<InputManager>().Update(0.f);
				Engine::GetEngineComponent<InputManager>().AddBinding(test);
				Engine::GetEngineComponent<InputManager>().AddCallback("ToNextMap", []()
					{
						TransformComponent& playerTransform = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<TransformComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()];
						auto& stateMoves = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<StateMoveComponent>();
				        if(stateMoves.isEmpty())
				        {
							InputManager& input_manager = Engine::GetEngineComponent<InputManager>();
							input_manager.RemoveCallback("ToNextMap");
							input_manager.RemoveBinding("ToNextMap");
							return;
				        }
						TransformComponent& portalTransform = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<TransformComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<StateMoveComponent>().begin()->GetEntityID()];
						playerTransform.SetPosition(portalTransform.GetPosition());
					});

			}
			else if (layer[i]->properties->layername == "BgTile")
			{
				back = AddEntity<BackGroundEntity>();
				AddComponent<TextureComponent>(back, "NTiles"_hashID, texel.bottomLeft.x, texel.bottomLeft.y,
					texel.topRight.x, texel.topRight.y, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_MAP_1));
				AddComponent<TransformComponent>(back, layer[i]->x, layer[i]->y, 0.f, tile_scale, tile_scale);
				AddComponent<WidthHeightComponent>(back, tile_size.x * tile_scale, tile_size.y * tile_scale);
				AddComponent<OffsetComponent>(back);
			}
			else if (layer[i]->properties->layername == "XTile")
			{
				damage = AddEntity<DamageTileEntity>();
				AddComponent<TextureComponent>(damage, "NTiles"_hashID, texel.bottomLeft.x, texel.bottomLeft.y,
					texel.topRight.x, texel.topRight.y, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_MAP_1));
				AddComponent<TransformComponent>(damage, layer[i]->x, layer[i]->y, 0.f, tile_scale, tile_scale);
				AddComponent<WidthHeightComponent>(damage, tile_size.x * tile_scale, tile_size.y * tile_scale);
				AddComponent<StaticRectCollisionComponent>(damage, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, true, false, false);
				AddComponent<OffsetComponent>(damage);
			}
			else
			{
				Map = AddEntity<MapEntity>();
				if (layer[i]->properties->layername != "WallTile")
				{
					AddComponent<TextureComponent>(Map, "NTiles"_hashID, texel.bottomLeft.x, texel.bottomLeft.y,
						texel.topRight.x, texel.topRight.y, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_MAP_1));
				}
				AddComponent<TransformComponent>(Map, layer[i]->x, layer[i]->y, 0.f, tile_scale, tile_scale);
				AddComponent<WidthHeightComponent>(Map, tile_size.x * tile_scale, tile_size.y * tile_scale);

				int index = (mMapData.mTileNum.x * ((static_cast<int>(layer[i]->y) - mMapData.mOffset.y) / mMapData.mTileSize.y))
					+ ((static_cast<int>(layer[i]->x) - mMapData.mOffset.x) / mMapData.mTileSize.x);


				if (layer[i]->properties->layername == "Oneway")
				{
					AddComponent<StaticRectCollisionComponent>(Map, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, false, true, false);
					mMapData.mTiles[index] = TileType::OneWay;
				}
				else if (layer[i]->properties->layername == "BreakTile")
				{
					AddComponent<StaticRectCollisionComponent>(Map, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, false, false, true, false);
					mMapData.mTiles[index] = TileType::Breakable;
				}
				else if (layer[i]->properties->layername == "Ladder")
				{
					AddComponent<StaticRectCollisionComponent>(Map, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, false, false, false, true);
					mMapData.mTiles[index] = TileType::Ladder;
				}
				else
				{
					AddComponent<StaticRectCollisionComponent>(Map, glm::vec2{ layer[i]->x, layer[i]->y }, tile_size.x, tile_size.y, false, false, false);
					mMapData.mTiles[index] = TileType::Normal;
				}


				AddComponent<OffsetComponent>(Map);
			}

		}

		//Items..

		WeaponEntity* const weapon = AddEntity<WeaponEntity>();
		AddComponent<TransformComponent>(weapon, 400.f, 200.f, 0.f, 0.5f, 0.5f);
		AddComponent<AnimationComponent>(weapon, INVALID_ID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_PLAYER_OBJ));

		AddComponent<RectCollisionComponent>(weapon, glm::vec2{ 400.f,200.f }, 100.f, 100.f, false, true);
		AddComponent<WidthHeightComponent>(weapon, 100.f, 100.f);
		AddComponent<OffsetComponent>(weapon, glm::vec2{ 80.f, 0.f }, player->GetEntityID());
		AddComponent<ObjectStateComponent>(weapon);

		//lua weapon

		AddComponent<NewWeaponComponent>(weapon);


		MapLevel MapNum = MapGeneration::GetMap(static_cast<int>(GetLevelNum()));
		if (MapNum == MapLevel::Level2 || GetLevelNum() == MapLevel::Level1
			|| MapNum == MapLevel::Level4 || GetLevelNum() == MapLevel::Tutorial)
		{
			auto items = objects_layer.find("Item")->second;

			ItemEntity* const droppedItem = AddEntity<ItemEntity>();
			AddComponent<DropWeaponComponent>(droppedItem, "Gun");
			AddComponent<TransformComponent>(droppedItem, items[0]->pos);
			AddComponent<TextureComponent>(droppedItem, "GunIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
			AddComponent<OffsetComponent>(droppedItem);
			AddComponent<WidthHeightComponent>(droppedItem, 32.f, 32.f);
			AddComponent<VelocityComponent>(droppedItem);
			AddComponent<GravityComponent>(droppedItem);
			AddComponent<RectCollisionComponent>(droppedItem, items[0]->pos, 32.f, 32.f, true, true);


			ItemEntity* const droppedItem2 = AddEntity<ItemEntity>();
			AddComponent<DropItemComponent>(droppedItem2, "Bomb");
			AddComponent<TransformComponent>(droppedItem2, items[1]->pos);
			AddComponent<TextureComponent>(droppedItem2, "BombIcon"_hashID, static_cast<int>(EObjectZDepth::ITEM));
			AddComponent<OffsetComponent>(droppedItem2);
			AddComponent<WidthHeightComponent>(droppedItem2, 32.f, 32.f);
			AddComponent<VelocityComponent>(droppedItem2);
			AddComponent<GravityComponent>(droppedItem2);
			AddComponent<RectCollisionComponent>(droppedItem2, glm::vec2{ -200, 200 }, 32.f, 32.f, true, true);
		}



		AddUpdateSystem<ParticleSystem>();



		auto emitter = std::make_shared<RectMeshParticleEmitter>("Test", 300, 3, Color{ 0.f, 1.f, 1.f, 1.f });
		GetUpdateSystem<ParticleSystem>().RegisterEmitter(emitter);
		emitter = std::make_shared<RectMeshParticleEmitter>("Boom", 500, 3, Color{ 255.f / 255.f, 249.f / 255.f, 77.f / 255.f, 1.f });
		GetUpdateSystem<ParticleSystem>().RegisterEmitter(emitter);
		emitter = std::make_shared<RectMeshParticleEmitter>("BulletFragments", 500, 1, Color{ 0.f, 1.f, 1.f, 1.f });
		GetUpdateSystem<ParticleSystem>().RegisterEmitter(emitter);
		emitter = std::make_shared<RectMeshParticleEmitter>("Heal", 100, 3, Color{ 0, 1.f, 0.f, 1.f });
		GetUpdateSystem<ParticleSystem>().RegisterEmitter(emitter);



		auto& inputManager = Engine::GetEngineComponent<InputManager>();

		Binding* bind = new Binding("ParticleTester");
		bind->BindInput(InputType::KEY_PRESS, Key::E);
		inputManager.AddBinding(bind);
		inputManager.AddCallback("ParticleTester", [&]()
			{
				Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetUpdateSystem<ParticleSystem>().Find("BulletFragments")->Emit(10, glm::vec2{ 500.f, 912.f }, glm::vec2{},
					glm::vec2{ -50, 100 }, glm::vec2{ 50, 200 }, glm::vec2{ 1.f, 2.f }, glm::vec2{ 1.f, 2.f }, 0.f);
			}
		);


		Loader loader(this);

		//test end session
		if (!Last)
		{
			Last = AddEntity<PortalEntity>();
		}



		LevelSystemBindingLoader(this, player, weapon, camera);
#ifndef QEDITOR
		Debugger::GetEntityDebuggerManager().Clear();
		Debugger::GetEntityDebuggerManager().Load();
#endif
		GameState::Load();

		q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PlaySoundEffect("portal-warp", false);
	}

	void Level::SetLevel(MapLevel levelnum)
	{
		mLevelNum = levelnum;
	}

	MapLevel Level::GetLevelNum() const
	{
		return mLevelNum;
	}

	TileType Level::GetTileByCoord(float x, float y)
	{
		int index = (mMapData.mTileNum.x * ((static_cast<int>(y) - mMapData.mOffset.y) / mMapData.mTileSize.y))
			+ ((static_cast<int>(x) - mMapData.mOffset.x) / mMapData.mTileSize.x);
		return mMapData.mTiles[index];
	}

	TileType Level::GetTileByCoord(glm::vec2 coord)
	{
		int index = (mMapData.mTileNum.x * ((static_cast<int>(coord.y) - mMapData.mOffset.y) / mMapData.mTileSize.y))
			+ ((static_cast<int>(coord.x) - mMapData.mOffset.x) / mMapData.mTileSize.x);
		if (0 <= index && index < mMapData.mTiles.size())
			return mMapData.mTiles[index];
		else
			return TileType::Normal;
	}

	TileType Level::GetTileByIndex(int x, int y)
	{
		return mMapData.mTiles[mMapData.mTileNum.x * y + x];
	}

	TileType Level::GetTileByIndex(vec2Int index)
	{
		return mMapData.mTiles[mMapData.mTileNum.x * index.y + index.x];
	}

	const Level::MapData& Level::GetMapData()
	{
		return mMapData;
	}
	
	glm::vec<2, int> Level::GetTileIndexByCoord(glm::vec2 coord)
	{
		return { ((static_cast<int>(coord.x) - mMapData.mOffset.x) / mMapData.mTileSize.x), ((static_cast<int>(coord.y) - mMapData.mOffset.y) / mMapData.mTileSize.y) };
	}
}
