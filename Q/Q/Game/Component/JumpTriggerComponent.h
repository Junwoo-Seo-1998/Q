/*
    File Name: JumpTriggerComponent.h
    Project Name: Q
    Author(s):
        Primary: Minjae Kyung
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../../Engine/ECS/Component.h"
#include "JumpTriggerComponent.h"

#include <glm/vec2.hpp>

class JumpTriggerComponent : public q_engine::Component<JumpTriggerComponent>
{
	glm::vec2 mRequireVelocity;
    bool mIsToUpper;

public:
	JumpTriggerComponent(glm::vec2 requireVelocity, bool isToUpperDirection) : mRequireVelocity(requireVelocity), mIsToUpper(isToUpperDirection){}

	const glm::vec2& GetRequireVelocity() const { return mRequireVelocity; }
    bool isToUpper() const { return mIsToUpper; }
};
