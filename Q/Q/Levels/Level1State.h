/*
    File Name: Level1State.h
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
        Secondary: Yoonki Kim
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/GameState.h"
class Level1State : public q_engine::Level
{
    void Load() override;
    void Update(double deltaTime) override;
    void Unload() override;
};