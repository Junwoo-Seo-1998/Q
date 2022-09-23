/*
	File Name: DropCubeComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>
#include "Engine/ECS/Component.h"

class DropCubeComponent : public q_engine::Component<DropCubeComponent>
{

public:
	DropCubeComponent() = default;
	DropCubeComponent(std::string DropCubeName, int count = 1) : mCubeName(DropCubeName), mCount(count) {}
	std::string GetDropCubeName() const { return mCubeName; }
	int GetCount() const { return mCount; }
	void SetCount(int i) { mCount = i; }
private:
	std::string mCubeName;
	int mCount = 1;
};
