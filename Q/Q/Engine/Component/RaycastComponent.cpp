/*
	File Name: RaycastComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "RaycastComponent.h"

namespace q_engine
{
	RaycastComponent::RaycastComponent(glm::vec2 TargetPosition): mTargetPosition(TargetPosition)
	{
		mIntersectPoints.resize(mRaySize);
		mShootRayVectorData.resize(mRaySize);
		mDistanceData.resize(mRaySize);
		mRayCollisionEntityTypeEntityID.resize(mRaySize);
	}

	RaycastComponent::RaycastComponent(glm::vec2 TargetPosition, int Raysize): mTargetPosition(TargetPosition),
	                                                                           mRaySize(Raysize)
	{
		mIntersectPoints.resize(mRaySize);
		mShootRayVectorData.resize(mRaySize);
		mDistanceData.resize(mRaySize);
		mRayCollisionEntityTypeEntityID.resize(mRaySize);
	}

	RaycastComponent::RaycastComponent( int Raysize, float angle) : 
		mRaySize(Raysize), mAngle(angle)
	{
		mIntersectPoints.resize(mRaySize);
		mShootRayVectorData.resize(mRaySize);
		mDistanceData.resize(mRaySize);
		mRayCollisionEntityTypeEntityID.resize(mRaySize);
	}

	RaycastComponent::RaycastComponent(int Raysize): mRaySize(Raysize)
	{
		mIntersectPoints.resize(mRaySize);
		mShootRayVectorData.resize(mRaySize);
		mDistanceData.resize(mRaySize);
		mRayCollisionEntityTypeEntityID.resize(mRaySize);
	}

	RaycastComponent::~RaycastComponent()
	{
	}

	void RaycastComponent::UpdateTargetPosition(glm::vec2 TargetPosition)
	{
		mTargetPosition = TargetPosition;
	}
	glm::vec2 RaycastComponent::GetTargetPosition()
	{
		return mTargetPosition;
	}

	int RaycastComponent::GetRaySize()
	{
		return mRaySize;
	}

	std::vector<std::pair<glm::vec2, bool>>& RaycastComponent::GetIntersectPoints()
	{
		return mIntersectPoints;
	}

	std::vector<glm::vec2>& RaycastComponent::GetShootRayVectorData()
	{
		return mShootRayVectorData;
	}

	std::vector<float>& RaycastComponent::GetDistanceData()
	{
		return mDistanceData;
	}

	std::vector<std::pair<IDType, IDType>>& RaycastComponent::GetRayCollisionEntityTypeEntityID()
	{
		return mRayCollisionEntityTypeEntityID;
	}

	int RaycastComponent::GetRayOrderindex()
	{
		return mRayOrderindex;
	}

	void RaycastComponent::RayIndexChecking()
	{
		if (mRayOrderindex < mRaySize-1)
		mRayOrderindex++;
		else
			mRayOrderindex = 0;
	}

	float RaycastComponent::GetRayAngle()
	{
		return mAngle;
	}

	Json::Value RaycastComponent::Serialize()
	{
		Json::Value value;
		value["TargetPosition"].append(mTargetPosition.x);
		value["TargetPosition"].append(mTargetPosition.y);
		value["RaySize"] = mRaySize;
		value["Angle"] = mAngle;
		return value;
	}

	void RaycastComponent::Deserialize(Json::Value& jsonValue)
	{
		mTargetPosition.x = jsonValue["TargetPosition"][0].asFloat();
		mTargetPosition.y = jsonValue["TargetPosition"][1].asFloat();
		mRaySize = jsonValue["RaySize"].asInt();
		mAngle = jsonValue["Angle"].asFloat();

		mIntersectPoints.resize(mRaySize);
		mShootRayVectorData.resize(mRaySize);
		mDistanceData.resize(mRaySize);
		mRayCollisionEntityTypeEntityID.resize(mRaySize);
	}
}

