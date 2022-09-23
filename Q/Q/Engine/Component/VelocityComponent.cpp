/*
	File Name: VelocityComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "../Engine/Component/VelocityComponent.h"
#include "sol/sol.hpp"

glm::vec2 q_engine::VelocityComponent::GetVelocity() const
{
	return mVelocity;
}

void q_engine::VelocityComponent::SetVelocity(glm::vec2 velocity)
{
	mVelocity = velocity;
}

void q_engine::VelocityComponent::SetVelocityX(float velocityX)
{
	mVelocity.x = velocityX;
}

void q_engine::VelocityComponent::SetVelocityY(float velocityY)
{
	mVelocity.y = velocityY;
}

void q_engine::VelocityComponent::SetAngularVelocity(float radian)
{
	angular_velocity = radian;
}

float q_engine::VelocityComponent::GetAngularVelocity() const
{
	return angular_velocity;
}

void q_engine::VelocityComponent::AddAngularVelocity(float radian)
{
	angular_velocity += radian;
}

void q_engine::VelocityComponent::AddVelocity(glm::vec2 velocity)
{
	mVelocity += velocity;
}

void q_engine::VelocityComponent::AddVelocityX(float velocityX)
{
	mVelocity.x += velocityX;
}

void q_engine::VelocityComponent::AddVelocityY(float velocityY)
{
	mVelocity.y += velocityY;
}

void q_engine::VelocityComponent::SubtractVelocityX(float velocityX)
{
	mVelocity.x -= velocityX;
}

void q_engine::VelocityComponent::SubtractVelocityY(float velocityY)
{
	mVelocity.y -= velocityY;
}

float q_engine::VelocityComponent::GetVelocityX() const
{
	return mVelocity.x;
}

float q_engine::VelocityComponent::GetVelocityY() const
{
	return mVelocity.y;
}

void q_engine::VelocityComponent::UpdateVelocity(float x, float y)
{
	mVelocity += glm::vec2{ x,y };
}

void q_engine::RegisterVelocityComponent(sol::state& state)
{
	sol::usertype<VelocityComponent> velocityComponent = state.new_usertype<VelocityComponent>("VelocityComponent");

	velocityComponent.set_function("GetVelocity", &VelocityComponent::GetVelocity);
	velocityComponent.set_function("SetVelocity", &VelocityComponent::SetVelocity);
	velocityComponent.set_function("SetAngularVelocity", &VelocityComponent::SetAngularVelocity);
	velocityComponent.set_function("GetAngularVelocity", &VelocityComponent::GetAngularVelocity);
	velocityComponent.set_function("AddAngularVelocity", &VelocityComponent::AddAngularVelocity);
}
