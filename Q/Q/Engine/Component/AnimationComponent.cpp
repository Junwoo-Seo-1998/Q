/*
    File Name: AnimationComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "AnimationComponent.h"

#include <numeric>


#include "../AssetManager.h"
#include <sol/state.hpp>

namespace q_engine
{
	AnimationComponent::AnimationComponent()
		: mDrawObject("SquareMesh"_hashID, "DefaultTextureShaderProgram"_hashID)
	{
		mAnimationID = INVALID_ID;
		SetAnimation(INVALID_ID);
	}


	AnimationComponent::AnimationComponent(const AnimationID animationID, const ZDepth zDepth, const EFrameOfReference frameOfReference, const bool bPlayImmediately)
		: mDrawObject("SquareMesh"_hashID, "DefaultTextureShaderProgram"_hashID)
	{
		SetAnimation(animationID, zDepth, frameOfReference, bPlayImmediately);
	}


	float AnimationComponent::GetLength() const noexcept
	{
		return cacheAccumulatedTimePoints.back();
	}

	std::string AnimationComponent::GetAnimationName() const
	{
		if (mAnimationID == INVALID_ID)
			return "";
		return AssetManager::GetAnimation(mAnimationID).GetAnimationName();
	}

	bool AnimationComponent::IsAnimationID(AnimationID id)
	{
		return mAnimationID == id;
	}

	size_t AnimationComponent::GetCurrentIndex() const
	{
		return mCurrentTimePointIndex;
	}


	void AnimationComponent::SetAnimation(const AnimationID animationID, const ZDepth zDepth, const EFrameOfReference frameOfReference, const bool bPlayImmediately)
	{
		SetZDepth(zDepth);
		if (frameOfReference != EFrameOfReference::UNCHANGED)
		{
			mDrawObject.SetFrameOfReference(frameOfReference);
		}
		mIsPlaying = bPlayImmediately;

		Reset();

		if (animationID == INVALID_ID)
		{
			return;
		}
		
		Animation& animation = AssetManager::GetAnimation(animationID);
		mAnimationID = animationID;
		mIsLooping = animation.GetInitialIsLooping();
		mLoopPoint = animation.GetLoopPoint();
		mPlayRate = animation.GetInitialPlayRate();

		mTextureIDs = animation.GetTextureIDs();

		cacheAccumulatedTimePoints = animation.GetCacheAccumulatedTimePoints();

		cacheMaxTimePointIndex = cacheAccumulatedTimePoints.size() - 1;
		cacheMaxTimePoint = cacheAccumulatedTimePoints.back();

	}

	void AnimationComponent::ChangeAnimation(AnimationID animationID)
	{
		SetAnimation(animationID, mDrawObject.GetZDepth());
	}

	void AnimationComponent::SetZDepth(const ZDepth zDepth) const
	{
		mDrawObject.SetZDepth(zDepth);
	}


	TextureID AnimationComponent::getCurrentTextureID() const
	{
		return mTextureIDs.at(mCurrentTimePointIndex);
	}

	Json::Value AnimationComponent::Serialize()
	{
		Json::Value value;
		value["Animation"] = GetAnimationName();
		value["ZDepth"] = static_cast<int>(mDrawObject.GetZDepth());
		value["FrameOfReference"] = (int)mDrawObject.GetFrameOfReference();
		value["PlayImmediately"] = mIsPlaying;
		return value;
	}

	void AnimationComponent::Deserialize(Json::Value& jsonValue)
	{
		std::string name = jsonValue["Animation"].asString();
		ZDepth z_depth = ZDepth(jsonValue["ZDepth"].asInt());
		EFrameOfReference frameOfRef = (EFrameOfReference)jsonValue["FrameOfReference"].asInt();
		bool playImmediately =jsonValue["PlayImmediately"].asBool();

		SetAnimation(q_engine::get_hash_id(name), z_depth, frameOfRef, playImmediately);
	}




	void AnimationComponent::Update(const float deltaTime)
	{
		if (mAnimationID == INVALID_ID)
			return;
		if (!mIsPlaying)
		{
			return;
		}

		mCurrentPlayTime += deltaTime * mPlayRate;
		if (mCurrentPlayTime >= cacheMaxTimePoint)
		{
			if (mIsLooping)
			{
				while (mCurrentPlayTime >= cacheMaxTimePoint)
				{
					mCurrentPlayTime -= cacheMaxTimePoint;
					
					
				}
				mCurrentPlayTime += std::accumulate(cacheAccumulatedTimePoints.begin(), cacheAccumulatedTimePoints.begin() + mLoopPoint, 0.f);
				mCurrentTimePointIndex = mLoopPoint;
			}
			else
			{
				mCurrentTimePointIndex = cacheMaxTimePointIndex - 1;
				Pause();
				return;
			}
		}

		size_t nextIndex = mCurrentTimePointIndex + 1;
		while (nextIndex < cacheMaxTimePointIndex && cacheAccumulatedTimePoints.at(nextIndex) < mCurrentPlayTime)
		{
			mCurrentTimePointIndex = nextIndex++;
		}
	}


	void AnimationComponent::Draw(const glm::mat3& model2NDCTransform) const
	{
		if(mAnimationID==INVALID_ID)
			return;
		mDrawObject.SetTextureID(getCurrentTextureID());
		mDrawObject.Draw(model2NDCTransform);
	}


	void AnimationComponent::Reset()
	{
		mCurrentPlayTime = 0;
		mCurrentTimePointIndex = 0;
	}


	void AnimationComponent::ResetAndPlay()
	{
		Reset();
		Play();
	}


	void RegisterAnimationComponent(sol::state& state)
	{
		sol::usertype<AnimationComponent> animationComponent = state.new_usertype<AnimationComponent>("AnimationComponent");

		animationComponent.set_function("GetLength", &AnimationComponent::GetLength);
		animationComponent.set_function("SetAnimation", &AnimationComponent::SetAnimation);
		animationComponent.set_function("ChangeAnimation", &AnimationComponent::ChangeAnimation);
		animationComponent.set_function("GetAnimationName", &AnimationComponent::GetAnimationName);
		animationComponent.set_function("IsAnimationID", &AnimationComponent::IsAnimationID);
	}
}
