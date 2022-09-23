/*
    File Name: GameEndingState.h
    Project Name: Q
    Author(s):
        Primary: Sieun choi
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/GameState.h"
#include "Engine/Core/Event.h"
#include "MainMenuState.h"

class GameEndingState : public q_engine::UIState
{
public:
    void Load() override;
    void Update(double deltaTime) override;
    void Unload() override;

private:
    size_t m_timerID = q_engine::INVALID_ID;
};

