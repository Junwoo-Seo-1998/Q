/*
    File Name: GameOverState.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma    
#include "../Engine/GameState.h"
#include "Engine/Core/Event.h"
#include "MainMenuState.h"

class GameOverState : public q_engine::UIState
{
public:
    void Load() override;
    void Update(double deltaTime) override;
    void Unload() override;

private:
    size_t m_timerID = q_engine::INVALID_ID;
};

