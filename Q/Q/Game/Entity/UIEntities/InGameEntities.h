/*
    File Name: InGameEntities.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../../Engine/ECS/Entity.h"

class HealthBarUIEntity final : public q_engine::Entity<HealthBarUIEntity> {};
class WeaponBoxEntity final : public q_engine::Entity<WeaponBoxEntity> {};
class ItemBoxEntity : public q_engine::Entity<ItemBoxEntity> {};