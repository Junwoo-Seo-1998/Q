/*
	File Name: TutorialState.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
		Secondary: Yoonki Kim
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TutorialState.h"

#include "Component/DropItemComponent.h"
#include "LevelHeaders.h"
#include "System/UI/TutorialFlowSystem.h"

void TutorialState::Load()
{
	SetLevel(q_engine::MapLevel::Tutorial);
	TutorialHelper();
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Tutorial State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void TutorialState::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void TutorialState::Unload()
{
	using namespace q_engine;
	Engine::GetEngineComponent<SoundComponent>().StopSoundAll();
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	inputManager.RemoveBinding("ParticleTester");
	inputManager.RemoveCallback("ParticleTester");

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Test Map State UnLoaded", SeverityLevel::INFORMATIONAL);
}

void TutorialState::TutorialHelper()
{
	MapGeneration::tutorialFinished = true;
	glm::vec2 WinSize = { get_window_size().first,get_window_size().second };
	float scale = 0.9f;
	TutorialEntity* tuto = AddEntity<TutorialEntity>();
	AddComponent<TextureComponent>(tuto, "MoveKey"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY),true);
	AddComponent<TransformComponent>(tuto, -350.f, 200.f,0.f, scale, scale);
	AddComponent<WidthHeightComponent>(tuto, 600.f,150.f);
	AddComponent<OffsetComponent>(tuto);
	AddUpdateSystem<TutorialFlowSystem>();

	
}
