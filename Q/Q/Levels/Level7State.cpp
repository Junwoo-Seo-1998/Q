/*
	File Name: Level7State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun choi

	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Level7State.h"
#include "LevelHeaders.h"


void Level7State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().PlaySoundBG("bgm3");
	SetLevel(q_engine::MapLevel::Level7);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Level 7 State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Level7State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Level7State::Unload()
{
	using namespace q_engine;
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	Engine::GetEngineComponent<SoundComponent>().StopSoundAll();
	MapGeneration::restart = true;

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Level 7 State UnLoaded", SeverityLevel::INFORMATIONAL);
}
