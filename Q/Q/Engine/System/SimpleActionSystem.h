/*
    File Name: SimpleActionSystem.h
    Project Name: Q
    Author(s):
        Primary: Junwoo seo
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/System.h"
#include "Engine/GameState.h"

namespace q_engine
{
    class SimpleActionSystem final : public q_engine::System<SimpleActionSystem>
    {
    public:
        SimpleActionSystem() = delete;
        SimpleActionSystem(q_engine::GameState* state) : System<SimpleActionSystem>(state) {}
        void Update(double deltaTime) override;
    };
}