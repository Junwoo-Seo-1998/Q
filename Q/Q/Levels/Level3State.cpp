/*
	File Name: Level3State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Level3State.h"
#include "LevelHeaders.h"


void Level3State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().ResumeSoundBG(0);
	SetLevel(q_engine::MapLevel::Level3);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Level 3 State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Level3State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Level3State::Unload()
{
	using namespace q_engine;
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	Engine::GetEngineComponent<SoundComponent>().PauseSoundBG(0);
	Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	inputManager.RemoveBinding("ParticleTester");
	inputManager.RemoveCallback("ParticleTester");

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Test Map State UnLoaded", SeverityLevel::INFORMATIONAL);
}
