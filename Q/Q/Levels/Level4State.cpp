/*
	File Name: Level4State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun choi

	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Level4State.h"
#include "LevelHeaders.h"


void Level4State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().ResumeSoundBG(0);
	SetLevel(q_engine::MapLevel::Level4);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Level 4 State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Level4State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Level4State::Unload()
{
	using namespace q_engine;
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	
	Engine::GetEngineComponent<SoundComponent>().StopSoundAll();
	inputManager.RemoveBinding("ParticleTester");
	inputManager.RemoveCallback("ParticleTester");

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Test Map State UnLoaded", SeverityLevel::INFORMATIONAL);
}
