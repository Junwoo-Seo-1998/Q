/*
    File Name: FollowComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FollowComponent.h"


namespace q_engine
{
	FollowComponent::FollowComponent(const IDType entityIDToFollow, const float smoothRate)
        : mEntityIDToFollow(entityIDToFollow), mSmoothRate(smoothRate)
	{}
}
