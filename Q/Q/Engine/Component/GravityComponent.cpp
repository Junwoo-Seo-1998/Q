/*
	File Name: GravityComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/


#include "GravityComponent.h"

q_engine::GravityComponent::GravityComponent(double gravity): mGravity(gravity)
{
}

void q_engine::GravityComponent::SetGravity(double gravity)
{
	mGravity = gravity;
}

double q_engine::GravityComponent::GetGravity() const
{
	return mGravity;
}

void q_engine::GravityComponent::SetIsOnGround(bool isOnGround)
{
	mIsonGround = isOnGround;
}

bool q_engine::GravityComponent::IsOnGround() const
{
	return mIsonGround;
}

bool q_engine::GravityComponent::GetIsOnground() const
{
	return mIsonGround;
}

Json::Value q_engine::GravityComponent::Serialize()
{
	Json::Value value;
	value["Gravity"] = mGravity;
	return  value;
}

void q_engine::GravityComponent::Deserialize(Json::Value& jsonValue)
{
	mGravity = jsonValue["Gravity"].asDouble();
}
