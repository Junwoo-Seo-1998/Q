/*
    File Name: AnimationComponent.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once


#include "../ECS/Component.h"

#include "../Graphics/Animation.h"
#include "../Graphics/DrawObject.h"

namespace sol
{
	class state;
}


namespace q_engine
{
	class AnimationComponent final : public Component<AnimationComponent>
	{
	public:
		AnimationComponent();
		AnimationComponent(AnimationID animationID, ZDepth zDepth = 0, EFrameOfReference frameOfReference = EFrameOfReference::BOTTOM_LEFT, bool bPlayImmediately = true);

		[[nodiscard]] constexpr bool IsLooping() const noexcept { return mIsLooping; }
		[[nodiscard]] constexpr float GetPlayRate() const noexcept { return mPlayRate; }
		[[nodiscard]] constexpr bool IsPlaying() const noexcept { return mIsPlaying; }

		[[nodiscard]] float GetLength() const noexcept;
		[[nodiscard]] std::string GetAnimationName() const;
		[[nodiscard]] bool IsAnimationID(AnimationID id);
		[[nodiscard]] size_t GetCurrentIndex() const;
		
		void SetAnimation(AnimationID animationID, ZDepth zDepth = 0, EFrameOfReference frameOfReference = EFrameOfReference::UNCHANGED, bool bPlayImmediately = true);
		void ChangeAnimation(AnimationID animationID);

		void SetZDepth(ZDepth zDepth) const;

		constexpr void SetLooping(bool bLooping = true) noexcept { mIsLooping = bLooping; }
		constexpr void ToggleLooping() noexcept { SetLooping(!IsLooping()); }

		constexpr void SetPlayRate(float playRate) noexcept { mPlayRate = playRate; }

		constexpr void SetPlaying(bool bPlay = true) noexcept { mIsPlaying = bPlay; }
		constexpr void Play() noexcept { SetPlaying(true); }
		constexpr void Pause() noexcept { SetPlaying(false); }
		constexpr void TogglePlaying() noexcept { SetPlaying(!IsPlaying()); }

		constexpr bool IsAnimationEnded() const noexcept { return IsLooping() == false ? mCurrentPlayTime >= cacheMaxTimePoint : false; }

		void Update(float deltaTime);
		void Draw(const glm::mat3& model2NDCTransform = { 1, 0, 0, 0, 1, 0, 0, 0, 1 }) const;

		void Reset();
		void ResetAndPlay();

	private:
		[[nodiscard]] TextureID getCurrentTextureID() const;


	public:
		virtual Json::Value Serialize() override;
		virtual void Deserialize(Json::Value& jsonValue) override;
	private:
		mutable DrawObject mDrawObject;

		AnimationID mAnimationID;
		
		bool mIsLooping;
		int mLoopPoint = 0;
		float mPlayRate;
		
		std::vector<TextureID> mTextureIDs;

		bool mIsPlaying;
		float mCurrentPlayTime = 0.f;
		size_t mCurrentTimePointIndex = 0;

		std::vector<float> cacheAccumulatedTimePoints;
		size_t cacheMaxTimePointIndex;
		float cacheMaxTimePoint;
	};
	
	void RegisterAnimationComponent(sol::state& state);
	
}
