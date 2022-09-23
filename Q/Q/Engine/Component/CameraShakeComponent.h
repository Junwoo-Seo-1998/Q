/*
    File Name: CameraShakeComponent.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"

#include <algorithm>

#include <glm/gtc/constants.hpp>
#include <glm/vec2.hpp>

#include <perlin_noise/PerlinNoise.hpp>



namespace q_engine
{
	class CameraShakeComponent final : public Component<CameraShakeComponent>
	{
	public:
		constexpr void AddTraumaLevel(float traumaLevel) noexcept { mTraumaLevel = std::clamp(mTraumaLevel + traumaLevel, 0.f, 1.f); }

		[[nodiscard]] constexpr float GetTraumaLevel() const noexcept { return mTraumaLevel; }
		[[nodiscard]] constexpr float GetTraumaLevelSquared() const noexcept { return mTraumaLevel * mTraumaLevel; }

		[[nodiscard]] float GetAngleOffset() const noexcept;
		[[nodiscard]] glm::vec2 GetPositionOffset() const noexcept;

		constexpr void UpdateTime(const float dt) noexcept { mTimeSeed += dt; }


	public:
		static constexpr float DECREASE_RATE = 0.75f;
		
	private:
		float mTraumaLevel = 0.f;
		float mTimeSeed = 0.f;

		siv::BasicPerlinNoise<float> mAngleNoise{ std::random_device{} };
		siv::BasicPerlinNoise<float> mXNoise{ std::random_device{} };
		siv::BasicPerlinNoise<float> mYNoise{ std::random_device{} };

		static constexpr float MAX_ANGLE_OFFSET = glm::pi<float>() / 4.f;
		static constexpr float MAX_POSITION_OFFSET = 200.f;
	};
}
