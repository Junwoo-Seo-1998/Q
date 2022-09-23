/*
	File Name: Boss1State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Boss1State.h"
#include "LevelHeaders.h"

void Boss1State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().PlaySoundBG("boss_bgm");
	SetLevel(q_engine::MapLevel::BossMap);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Boss Map Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Boss1State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Boss1State::Unload()
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
