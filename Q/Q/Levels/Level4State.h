/*
    File Name: Level4State.h
    Project Name: Q
    Author(s):
        Primary: Sieun choi

    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/GameState.h"

class Level4State : public q_engine::Level
{
	void Load() override;
    void Update(double deltaTime) override;
	void Unload() override;
};