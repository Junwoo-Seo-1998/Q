/*
    File Name: UITestState.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/GameState.h"
class UITestState : public q_engine::UIState
{
    void Load() override;
    void Update(double deltaTime) override;
    void Unload() override;
};