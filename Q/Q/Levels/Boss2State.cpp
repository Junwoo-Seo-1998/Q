/*
	File Name: Boss2State.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Boss2State.h"
#include "LevelHeaders.h"

void Boss2State::Load()
{
	Engine::GetEngineComponent<SoundComponent>().PlaySoundBG("bgm2");
	SetLevel(q_engine::MapLevel::BossMap2);
	Level::Load();
	q_engine::Debugger::GetLogger().AddMessage("Boss Map 2 Loaded", q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("HP / " + std::to_string(GetComponents<HealthComponent>()[Engine::GetEngineComponent<GameStateManager>().GetPlayerID()].GetCurHealth()), q_engine::SeverityLevel::INFORMATIONAL);
	q_engine::Debugger::GetLogger().AddMessage("Time / " + std::to_string(Engine::GetElapsedTime()), q_engine::SeverityLevel::INFORMATIONAL);
}

void Boss2State::Update(double deltaTime)
{
	GameState::Update(deltaTime);
}


void Boss2State::Unload()
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
	Debugger::GetLogger().AddMessage("Boss Map 2 UnLoaded", SeverityLevel::INFORMATIONAL);
}
