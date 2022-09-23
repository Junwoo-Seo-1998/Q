/*
    File Name: FollowComponent.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Core/Rect.h"
#include "../ECS/Component.h"


namespace q_engine
{
	class FollowComponent final : public Component<FollowComponent>
	{
		friend class FollowSystem;
	public:

		FollowComponent() = default;
		FollowComponent(IDType entityIDToFollow, float smoothRate = 4.f);

		[[nodiscard]] constexpr IDType GetEntityIDToFollow() const noexcept { return mEntityIDToFollow; }
		[[nodiscard]] constexpr float GetSmoothRate() const noexcept { return mSmoothRate; }

		constexpr void FollowEntity(IDType entityIDToFollow) noexcept { mEntityIDToFollow = entityIDToFollow; }
		constexpr void StopFollowing() noexcept { mEntityIDToFollow = INVALID_ID; }

		constexpr void SetSmoothRate(float smoothRate) noexcept { mSmoothRate = smoothRate; }


	private:

		IDType mEntityIDToFollow = INVALID_ID;

		float mSmoothRate = 4.f;
	};
}
