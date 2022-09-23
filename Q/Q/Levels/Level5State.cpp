/*
	File Name: Level5State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun choi

	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Level5State.h"
#include "LevelHeaders.h"


void Level5State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().PlaySoundBG("bgm3");
	SetLevel(q_engine::MapLevel::Level5);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Level 5 State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Level5State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Level5State::Unload()
{
	using namespace q_engine;
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	Engine::GetEngineComponent<SoundComponent>().PauseAll();
	Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Test Map State UnLoaded", SeverityLevel::INFORMATIONAL);
}
