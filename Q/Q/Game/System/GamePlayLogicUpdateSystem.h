/*
	File Name: GamePlayLogicUpdateSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"
class GamePlayLogicUpdateSystem final : public q_engine::System<GamePlayLogicUpdateSystem>
{
public:
	GamePlayLogicUpdateSystem() = delete;
	GamePlayLogicUpdateSystem(q_engine::GameState* state) : System<GamePlayLogicUpdateSystem>(state), on_end_cond(false), is_PostProcessHandled(false) {}
	virtual void Load() override;
	virtual void Update(double deltaTime) override;

private:
	bool on_end_cond;
	bool is_PostProcessHandled;
};