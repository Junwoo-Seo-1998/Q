/*
    File Name: Animation.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include "Texture.h"
#include "Engine/Types.h"


namespace q_engine
{
	
	class [[nodiscard]] Animation
	{
		friend class AssetManager;
	public:
		Animation(const std::filesystem::path& animationFilePath);

		[[nodiscard]] constexpr const std::vector<TextureID>& GetTextureIDs() const noexcept { return mTextureIDs; }

		[[nodiscard]] constexpr bool GetInitialIsLooping() const noexcept { return mInitialIsLooping; }
		[[nodiscard]] constexpr float GetInitialPlayRate() const noexcept { return mInitialPlayRate; }

		[[nodiscard]] constexpr int GetLoopPoint() const noexcept { return mLoopPoint; }

		[[nodiscard]] constexpr AnimationID GetAnimationID() const noexcept { return mAnimationID; }
		[[nodiscard]] std::string GetAnimationName() const{ return mAnimationName; }

		[[nodiscard]] constexpr const std::vector<float>& GetCacheAccumulatedTimePoints() const noexcept { return cacheAccumulatedTimePoints; }



	private:
		bool mInitialIsLooping;
		float mInitialPlayRate;

		int mLoopPoint = 0;

		std::vector<TextureID> mTextureIDs;
		
		AnimationID mAnimationID = INVALID_ID;

		std::vector<float> cacheAccumulatedTimePoints;

		std::string mAnimationName;
	};
}
