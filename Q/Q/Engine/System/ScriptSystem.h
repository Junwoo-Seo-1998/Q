/*
	File Name: ScriptSystem.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/System.h"

namespace q_engine
{
	class ScriptSystem final : public System<ScriptSystem>
	{
	public:
		ScriptSystem(GameState* state) : System<ScriptSystem>(state) {}
		virtual void Load() override;
		virtual void Update(double deltaTime) override;
		virtual void Unload() override;
	};
}
