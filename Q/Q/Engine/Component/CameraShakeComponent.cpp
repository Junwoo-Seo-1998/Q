/*
    File Name: CameraShakeComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CameraShakeComponent.h"



namespace q_engine
{
	float CameraShakeComponent::GetAngleOffset() const noexcept
	{ 
        return MAX_ANGLE_OFFSET * GetTraumaLevelSquared() * mAngleNoise.noise2D(234.19832f, mTimeSeed * 10.f);
	}


	glm::vec2 CameraShakeComponent::GetPositionOffset() const noexcept
	{
		return { MAX_POSITION_OFFSET * GetTraumaLevelSquared() * mXNoise.noise2D(1283.38478f, mTimeSeed * 10.f),
			MAX_POSITION_OFFSET * GetTraumaLevelSquared() * mYNoise.noise2D(98.123f, mTimeSeed * 10.f)
		};
	}
}
