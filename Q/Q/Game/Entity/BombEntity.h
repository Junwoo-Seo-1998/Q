/*
    File Name: BombEntity.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../Engine/ECS/Entity.h"


class BombEntity final : public q_engine::Entity<BombEntity>
{
public:
    static constexpr int DAMAGE = 200;
};
