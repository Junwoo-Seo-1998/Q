/*
    File Name: PhysicsTestEntities.h
    Project Name: Q
    Author(s):
        Primary: Hoseob Jeong
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Entity.h"
#include "Component/HealthComponent.h"
#include "Component/NewPlayerInventorySlots.h"

class BallEntity : public q_engine::Entity<BallEntity>
{
};

class GroundEntity : public q_engine::Entity<GroundEntity>
{
};


class PlayerEntity : public q_engine::Entity<PlayerEntity>
{
public:
    using MovingComps = std::tuple < HealthComponent, NewPlayerInventorySlots>;
    static MovingComps ComponentList() { return MovingComps(); }
};

class EnemyEntity : public q_engine::Entity<EnemyEntity>
{
	
};