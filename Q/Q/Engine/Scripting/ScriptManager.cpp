/*
	File Name: ScriptManager.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary: Yoonki Kim , Hoseob jeong
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ScriptManager.h"

#include <iostream>

#include "Engine/GameState.h"
#include "RegisterMath.h"
#include "RegisterComponent.h"
#include "RegisterEnum.h"
#include "RegisterDataType.h"
#include "Component/DamageComponent.h"
#include "Component/NewItemComponent.h"

#include "Engine/ECS/InputManager.h"
#include "sol/sol.hpp"
#include "Engine/Core/StringHash.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/CameraShakeComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Entity/MapEntity.h"
#include "Entity/ProjectileEntity.h"

#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Game/Component/ProjectileScriptComponent.h"
#include "Engine/Component/CollisionComponent.h"
#include "Engine/Component/FollowComponent.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Core/RandomMachine.h"
#include "Engine/Graphics/Particle.h"
#include "EObjectZDepth.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/SimpleActionComponent.h"
#include "Engine/System/TimerManagerSystem.h"
#include "Entity/SimpleEntity.h"
#include "PlayTestTrace.h"

#include "Engine/AssetManager.h"
namespace q_engine
{
	ScriptManager::ScriptManager()
		:m_State(nullptr), m_CurrentScene(nullptr)
	{
	}

	ScriptManager& ScriptManager::Get()
	{
		static ScriptManager singleton;
		return singleton;
	}

	void ScriptManager::Init()
	{
		m_State.open_libraries(sol::lib::base, sol::lib::package, sol::lib::math, sol::lib::table);

		
		RegisterTimerType(m_State);

		RegisterUtils(m_State);
		RegisterEnum(m_State);
		RegisterDataType(m_State);
		RegisterTypeID(m_State);
		RegisterMath(m_State);
		RegisterInput(m_State);
		RegisterScene(m_State);
		RegisterComponent(m_State);
		RegisterCreateObject(m_State);


	}

	sol::state& ScriptManager::GetState()
	{
		return m_State;
	}

	void ScriptManager::SetCurrentScene(GameState* scene)
	{
		m_CurrentScene = scene;
		m_State["CurrentScene"] = scene;
		std::cout <<"Set:"<< m_State.get<GameState*>("CurrentScene") << std::endl;
	}

	void ScriptManager::RegisterUtils(sol::state& state)
	{
		auto utils = state["Utils"].get_or_create<sol::table>();
		utils.set_function("GetHashID", &get_hash_id);
		utils.set_function("GetGameStateManager", &Engine::GetEngineComponent<GameStateManager>);
		utils.set_function("GetSoundComponent", &Engine::GetEngineComponent<SoundComponent>);
		utils.set_function("ShakeMainCamera", sol::overload([this](float traumaLevel, float maxTraumaLevel) -> void
			{
				if (m_CurrentScene->GetComponents<q_engine::CameraShakeComponent>().Front().GetTraumaLevel() >= maxTraumaLevel)
				{
					return;
				}
				m_CurrentScene->GetComponents<q_engine::CameraShakeComponent>().Front().AddTraumaLevel(traumaLevel);
			},
			[this](float traumaLevel) -> void
			{
				m_CurrentScene->GetComponents<q_engine::CameraShakeComponent>().Front().AddTraumaLevel(traumaLevel);
			}));
		utils.set_function("GetIntRandomValue", [](int min, int max)
			{
				return Engine::GetEngineComponent<RandomMachine>().GetRandomValue(min, max);
			});
		utils.set_function("GetFloatRandomValue", [](float min, float max)
			{
				return Engine::GetEngineComponent<RandomMachine>().GetRandomValue(min, max);
			});

		utils.set_function("EmitParticle", 
			[](std::string particleEmitterName, unsigned num_particle, glm::vec2 emit_pos, glm::vec2 emit_vel, glm::vec2 minVel, glm::vec2 maxVel, glm::vec2 size, glm::vec2 lifeTime, float gravityConstant)
			{
				auto emitter = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetUpdateSystem<ParticleSystem>().Find(std::move(particleEmitterName));
				emitter->Emit(num_particle, emit_pos, emit_vel, minVel, maxVel, size, lifeTime, gravityConstant);
			});


		auto timer = state["TimerManager"].get_or_create<sol::table>();

		timer.set_function("IssueTimer",
			[](float time)
		{
				return TimerManagerSystem::IssueTimer(time);
		});

		timer.set_function("GetTimer",
			[](TimerID ID)-> Timer&
			{
				return TimerManagerSystem::GetTimer(ID);
			});

		timer.set_function("YieldTimer",
			[](TimerID ID)			
			{
				TimerManagerSystem::YieldTimer(ID);
			});

		auto aseetManager = state["AssetManager"].get_or_create<sol::table>();

		aseetManager.set_function("GetAnimation", [](IDType animationID) ->Animation&
			{
				return AssetManager::GetAnimation(animationID);
			});

		aseetManager.set_function("GetAnimationSize", [](IDType animationID) -> glm::vec2
			{
			    Texture& texture = AssetManager::GetTexture(AssetManager::GetAnimation(animationID).GetTextureIDs().front());
				return { texture.GetWidth(), texture.GetHeight() };
			});



		auto TestTraceBack = state["TestTraceBack"].get_or_create<sol::table>();

		TestTraceBack.set_function("LogWeaponUseMessage", &PlayTestTrace::LogWeaponUseMessage);
		TestTraceBack.set_function("LogItemUseMessage", &PlayTestTrace::LogItemUseMessage);
		TestTraceBack.set_function("LogOtherTypeMessage", &PlayTestTrace::LogOtherTypeMessage);
	}

	void ScriptManager::RegisterScene(sol::state& state)
	{
		sol::usertype<GameState> game_state = state.new_usertype<GameState>("State");
		
		game_state.set_function("HasComponent", sol::resolve <bool(IDType, IDType)>(&GameState::HasComponent));
		
		game_state.set_function("GetTransformComponent", sol::resolve<TransformComponent& (IDType)>(&GameState::GetComponentWithID<TransformComponent>));
		game_state.set_function("GetAnimationComponent", sol::resolve<AnimationComponent& (IDType)>(&GameState::GetComponentWithID<AnimationComponent>));
		game_state.set_function("GetTextureComponent", sol::resolve<TextureComponent& (IDType)>(&GameState::GetComponentWithID<TextureComponent>));
		game_state.set_function("GetOffsetComponent", sol::resolve<OffsetComponent& (IDType)>(&GameState::GetComponentWithID<OffsetComponent>));
		game_state.set_function("GetNewItemComponent", sol::resolve<NewItemComponent& (IDType)>(&GameState::GetComponentWithID<NewItemComponent>));
		game_state.set_function("GetVelocityComponent", sol::resolve<VelocityComponent& (IDType)>(&GameState::GetComponentWithID<VelocityComponent>));
		game_state.set_function("GetStaticRectComponent", sol::resolve<StaticRectCollisionComponent& (IDType)>(&GameState::GetComponentWithID<StaticRectCollisionComponent>));
		game_state.set_function("GetHealthComponent", sol::resolve<HealthComponent& (IDType)>(&GameState::GetComponentWithID<HealthComponent>));
		game_state.set_function("GetWidthHeightComponent", sol::resolve<WidthHeightComponent& (IDType)>(&GameState::GetComponentWithID<WidthHeightComponent>));
		game_state.set_function("GetObjectStateComponent", sol::resolve<ObjectStateComponent& (IDType)>(&GameState::GetComponentWithID<ObjectStateComponent>));
		game_state.set_function("GetNewPlayerInventorySlots", sol::resolve<NewPlayerInventorySlots& (IDType)>(&GameState::GetComponentWithID<NewPlayerInventorySlots>));
		game_state.set_function("GetDamageComponent", sol::resolve<DamageComponent& (IDType)>(&GameState::GetComponentWithID<DamageComponent>));
		
		game_state.set_function("AddDamageComponent", sol::resolve <IComponent* (IDType, IDType, int, double, DamageType)>(&GameState::AddComponent<DamageComponent, int, double, DamageType>));
		game_state.set_function("AddTagComponent", sol::resolve <IComponent* (IDType, IDType,std::string)>(&GameState::AddComponent<TagComponent, std::string>));
	}

	void ScriptManager::RegisterInput(sol::state& state)
	{
		auto input = state["Input"].get_or_create<sol::table>();

		input.set_function("IsKeyDown", [](Key key) -> bool {
			return InputManager::IsKeyDown(key);
			});

		input.set_function("IsKeyPressed", [](Key key) -> bool {
			return InputManager::IsKeyPressed(key);
		});

		std::initializer_list<std::pair<sol::string_view, Key>> keyItems =
		{
			{"A", Key::A},
			{"B", Key::B},
			{"C", Key::C},
			{"D", Key::D},
			{"E", Key::E},
			{"F", Key::F},
			{"H", Key::G},
			{"G", Key::H},
			{"I", Key::I},
			{"J", Key::J},
			{"K", Key::K},
			{"L", Key::L},
			{"M", Key::M},
			{"N", Key::N},
			{"O", Key::O},
			{"P", Key::P},
			{"Q", Key::Q},
			{"R", Key::R},
			{"S", Key::S},
			{"T", Key::T},
			{"U", Key::U},
			{"V", Key::V},
			{"W", Key::W},
			{"X", Key::X},
			{"Y", Key::Y},
			{"Z", Key::Z},
			{"SPACE",Key::SPACE},
		};

		state.new_enum<Key, false>("Key", keyItems);
	}

	void ScriptManager::RegisterEntity(sol::state& state)
	{
	}

	void ScriptManager::RegisterTypeID(sol::state& state)
	{
		std::initializer_list<std::pair<sol::string_view, q_engine::IDType>> typeID =
		{
			//entt
			{"MapEntity", GetTypeID<MapEntity>()},
			{"EnemyEntity", GetTypeID<EnemyEntity>()},
			{"ProjectileEntity", GetTypeID<ProjectileEntity>()},

			//comp
			{"DamageComponent",DamageComponent::GetTypeID()},
			
		};
		state.new_enum<q_engine::IDType, false>("TypeID", typeID);
	}

	void ScriptManager::RegisterCreateObject(sol::state& state)
	{
		auto create = state["Create"].get_or_create<sol::table>();

		create.set_function("Projectile", [&](glm::vec2 start_pos,glm::vec2 velo, glm::vec2 size, const std::string& script_file) -> void {
		
			GameState* current_state = m_State.get<GameState*>("CurrentScene");
			BaseEntity* projectile = current_state->AddEntity<ProjectileEntity>();
			
			current_state->AddComponent<TransformComponent>(projectile, start_pos,0.f,size);
			current_state->AddComponent<VelocityComponent>(projectile, velo);
			current_state->AddComponent<WidthHeightComponent>(projectile, size, false);
			current_state->AddComponent<OffsetComponent>(projectile, glm::vec2{ size.x / 2.f,0});
			current_state->AddComponent<TextureComponent>(projectile, get_hash_id("TestPng"), static_cast<int>(EObjectZDepth::IN_GAME_PLAY_CANISTER));
			current_state->AddComponent<RectCollisionComponent>(projectile, start_pos, size.x, size.y, true, true);
			current_state->AddComponent<ProjectileScriptComponent>(projectile, script_file);
		});

		create.set_function("ProjectileWithGravity", [&](glm::vec2 start_pos, glm::vec2 velo, glm::vec2 size, const std::string& script_file) -> void {
			
			GameState* current_state = m_State.get<GameState*>("CurrentScene");
			BaseEntity* projectile = current_state->AddEntity<ProjectileEntity>();

			current_state->AddComponent<TransformComponent>(projectile, start_pos, 0.f, size);
			current_state->AddComponent<VelocityComponent>(projectile, velo);
			current_state->AddComponent<WidthHeightComponent>(projectile, size, false);
			current_state->AddComponent<OffsetComponent>(projectile, glm::vec2{ size.x / 2.f,0 });
			current_state->AddComponent<TextureComponent>(projectile, get_hash_id("TestPng"));
			current_state->AddComponent<RectCollisionComponent>(projectile, start_pos, size.x, size.y, true, true);
			current_state->AddComponent<ProjectileScriptComponent>(projectile, script_file);
			current_state->AddComponent<GravityComponent>(projectile);
			
			});


		create.set_function("ProjectileAnimation", [&](glm::vec2 start_pos, glm::vec2 velo, glm::vec2 size, const std::string& script_file) -> void {
		
			GameState* current_state = m_State.get<GameState*>("CurrentScene");
			BaseEntity* projectile = current_state->AddEntity<ProjectileEntity>();

			current_state->AddComponent<TransformComponent>(projectile, start_pos, 0.f, size);
			current_state->AddComponent<VelocityComponent>(projectile, velo);
			current_state->AddComponent<WidthHeightComponent>(projectile, size, false);
			current_state->AddComponent<OffsetComponent>(projectile, glm::vec2{ size.x / 2.f,0 });
			current_state->AddComponent<AnimationComponent>(projectile, get_hash_id("TestAnimation"), static_cast<int>(EObjectZDepth::IN_GAME_PLAY_CANISTER));
			current_state->AddComponent<RectCollisionComponent>(projectile, start_pos, size.x, size.y, true, true);
			current_state->AddComponent<ProjectileScriptComponent>(projectile, script_file);
			});

		create.set_function("ProjectileAnimationWithGravity", [&](glm::vec2 start_pos, glm::vec2 velo, glm::vec2 size, const std::string& script_file) -> void {
	
			GameState* current_state = m_State.get<GameState*>("CurrentScene");
			BaseEntity* projectile = current_state->AddEntity<ProjectileEntity>();

			current_state->AddComponent<TransformComponent>(projectile, start_pos, 0.f, size);
			current_state->AddComponent<VelocityComponent>(projectile, velo);
			current_state->AddComponent<WidthHeightComponent>(projectile, size, false);
			current_state->AddComponent<OffsetComponent>(projectile, glm::vec2{ size.x / 2.f,0 });
			current_state->AddComponent<AnimationComponent>(projectile, get_hash_id("TestAnimation"));
			current_state->AddComponent<RectCollisionComponent>(projectile, start_pos, size.x, size.y, true, true);
			current_state->AddComponent<ProjectileScriptComponent>(projectile, script_file);
			current_state->AddComponent<GravityComponent>(projectile);
			current_state->AddComponent<TagComponent>(projectile, "Bomb");
			});

		create.set_function("TextureWithScript", [&](glm::vec2 start_pos, glm::vec2 velo, glm::vec2 size, const std::string& script_file) -> void {
			GameState* current_state = m_State.get<GameState*>("CurrentScene");
			BaseEntity* texture = current_state->AddEntity<SimpleEntity>();

			current_state->AddComponent<TransformComponent>(texture, start_pos, 0.f, size);
			current_state->AddComponent<VelocityComponent>(texture, velo);
			current_state->AddComponent<WidthHeightComponent>(texture, size, false);
			current_state->AddComponent<OffsetComponent>(texture, glm::vec2{ size.x / 2.f,0 });
			current_state->AddComponent<TextureComponent>(texture, get_hash_id("TestPng"));
			current_state->AddComponent<SimpleActionComponent>(texture, script_file);
			});


		create.set_function("AnimationWithScript", [&](glm::vec2 start_pos, glm::vec2 velo, glm::vec2 size, const std::string& script_file) -> void {
			GameState* current_state = m_State.get<GameState*>("CurrentScene");
			BaseEntity* animation = current_state->AddEntity<SimpleEntity>();

			current_state->AddComponent<TransformComponent>(animation, start_pos, 0.f, size);
			current_state->AddComponent<VelocityComponent>(animation, velo);
			current_state->AddComponent<WidthHeightComponent>(animation, size, false);
			current_state->AddComponent<OffsetComponent>(animation, glm::vec2{ size.x / 2.f,0 });
			current_state->AddComponent<AnimationComponent>(animation, get_hash_id("TestAnimation"));
			current_state->AddComponent<SimpleActionComponent>(animation, script_file);

			});

		create.set_function("FollowAnimation", [&](IDType animID, IDType entToFollow, glm::vec2 offset, glm::vec2 scale, float rotation) {
			using EntInfo = std::pair<IDType, IDType>;
				GameState* current_state = m_State.get<GameState*>("CurrentScene");
				BaseEntity* animation = current_state->AddEntity<SimpleEntity>();

				const glm::vec2 followEntMiddle = current_state->GetComponents<WidthHeightComponent>()[entToFollow].GetMiddlePos();

				Texture& animTexture = AssetManager::GetTexture(AssetManager::GetAnimation(animID).GetTextureIDs().front());

				const glm::vec2 mySize = { animTexture.GetWidth(), animTexture.GetHeight() };;
				const glm::vec2 myAnimMiddle = mySize* glm::vec2{0.5, 0.5,};

				const glm::vec2 myOff = followEntMiddle/* - myAnimMiddle*/;

				current_state->AddComponent<FollowComponent>(animation, entToFollow, 1.f);
				current_state->AddComponent<TransformComponent>(animation, glm::vec2{});
				current_state->AddComponent<WidthHeightComponent>(animation, mySize);
				current_state->AddComponent<OffsetComponent>(animation, glm::vec2{}, offset, rotation, scale.x);
				current_state->AddComponent<AnimationComponent>(animation, animID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_EFFECT));

				return sol::as_table(std::array<unsigned, 2>{animation->GetRuntimeTypeID(), animation->GetEntityID()});
			});
	}

}
