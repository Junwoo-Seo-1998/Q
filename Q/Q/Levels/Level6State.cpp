/*
	File Name: Level6State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun choi

	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Level6State.h"
#include "LevelHeaders.h"


void Level6State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().ResumeAll();
	SetLevel(q_engine::MapLevel::Level6);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Level 6 State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Level6State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Level6State::Unload()
{
	using namespace q_engine;
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	Engine::GetEngineComponent<SoundComponent>().StopSoundAll();

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Level 6 State UnLoaded", SeverityLevel::INFORMATIONAL);
}
