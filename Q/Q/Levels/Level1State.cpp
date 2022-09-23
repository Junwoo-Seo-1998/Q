/*
	File Name: Level1State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Level1State.h"
#include "LevelHeaders.h"

void Level1State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().PlaySoundBG(0);
	SetLevel(q_engine::MapLevel::Level1);
	Level::Load();

	if(!MapGeneration::restart)
	{
		GetComponentWithID<NewPlayerInventorySlots>(Engine::GetEngineComponent<GameStateManager>().GetPlayerID()).ClearSlots();
	}
	q_engine::Debugger::GetLogger().AddMessage("Level 1 State Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Level1State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Level1State::Unload()
{
	using namespace q_engine;
	Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	Engine::GetEngineComponent<SoundComponent>().PauseSoundBG(0);
	InputManager& inputManager = Engine::GetEngineComponent<InputManager>();
	inputManager.RemoveBinding("ParticleTester");
	inputManager.RemoveCallback("ParticleTester");

	inputManager.RemoveBinding("UIAdd");
	inputManager.RemoveCallback("UIAdd");

	using namespace q_engine;
	GameState::Unload();
	Debugger::GetLogger().AddMessage("Test Map State UnLoaded", SeverityLevel::INFORMATIONAL);
}
