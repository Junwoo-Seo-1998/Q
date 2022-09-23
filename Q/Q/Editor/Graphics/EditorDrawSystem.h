/*
	File Name: EditorDrawSystem.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/System.h"

class EditorDrawSystem final : public q_engine::System<EditorDrawSystem>
{
public:
    EditorDrawSystem() = delete;
    EditorDrawSystem(q_engine::GameState* state) : System<EditorDrawSystem>(state) {}
    void Update(double deltaTime) override;
};
