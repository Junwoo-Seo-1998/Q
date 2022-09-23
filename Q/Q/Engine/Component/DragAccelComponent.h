/*
	File Name: DragAccelComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Component.h"

namespace q_engine
{
class DragAccelComponent : public Component<DragAccelComponent>
{
public:
	DragAccelComponent() = default;
	float GetDrag() const;

	float GetAccel() const;

	float GetJumpVelocity() const;
	float GetMaxVelocity() const;
	float GetSlideVelocity() const;
	float GetMaxSlideVelocity() const;
	float GetCrawlingVelocity() const;
	float GetMaxCrawlingVelocity() const;
private:
	static const float mDrag;
	static const float mAccel;
	static const float mJumpvelocity;
	static const float mMaxvelocity;
	static const float mSlidevelocity;
	static const float mMaxSlidevelocity;
	static const float mCrawlVelocity;
	static const float mCrawlMaxVelocity;
};



}