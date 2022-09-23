/*
    File Name: WeaponEntity.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../Engine/ECS/Entity.h"



class WeaponEntity final : public q_engine::Entity<WeaponEntity>
{
};

class EnemyWeaponEntity final : public q_engine::Entity<EnemyWeaponEntity>
{
};