/*
	File Name: DragAccelComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "DragAccelComponent.h"
const float q_engine::DragAccelComponent::mDrag = 550.f;
const float q_engine::DragAccelComponent::mAccel = 350.f;
const float q_engine::DragAccelComponent::mJumpvelocity = 1150.f;
const float q_engine::DragAccelComponent::mMaxvelocity = 320.f;
const float q_engine::DragAccelComponent::mSlidevelocity = 150.f;
const float q_engine::DragAccelComponent::mMaxSlidevelocity = 550.f;
const float q_engine::DragAccelComponent::mCrawlVelocity = 180.f;
const float q_engine::DragAccelComponent::mCrawlMaxVelocity = 210.f;

float q_engine::DragAccelComponent::GetDrag() const
{
	return mDrag;
}

float q_engine::DragAccelComponent::GetAccel() const
{
	return mAccel;
}

float q_engine::DragAccelComponent::GetJumpVelocity() const
{
	return mJumpvelocity;
}

float q_engine::DragAccelComponent::GetMaxVelocity() const
{
	return mMaxvelocity;
}

float q_engine::DragAccelComponent::GetSlideVelocity() const
{
	return mSlidevelocity;
}

float q_engine::DragAccelComponent::GetMaxSlideVelocity() const
{
	return mMaxSlidevelocity; 
}

float q_engine::DragAccelComponent::GetCrawlingVelocity() const
{
	return mCrawlVelocity;
}

float q_engine::DragAccelComponent::GetMaxCrawlingVelocity() const
{
	return mCrawlMaxVelocity;
}
