/*
    File Name: UIEntities.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Entity.h"
class MainLogoEntity final : public q_engine::Entity<MainLogoEntity> {};

class TextureBoxEntity final : public q_engine::Entity<TextureBoxEntity> {};

class TextBoxEntity final : public q_engine::Entity<TextBoxEntity> {};