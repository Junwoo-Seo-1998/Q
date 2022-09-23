/*
    File Name: SplashState.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/GameState.h"
class SplashState : public q_engine::UIState
{
public:
    void Load() override;
    void Update(double deltaTime) override;
    void Unload() override;

private:
    q_engine::TimerID m_timerID = q_engine::INVALID_ID;
};