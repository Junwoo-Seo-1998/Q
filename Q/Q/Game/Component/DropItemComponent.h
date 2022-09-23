/*
	File Name: DropItemComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include "../Engine/ECS/Component.h"

class DropItemComponent : public q_engine::Component<DropItemComponent>
{

public:
	DropItemComponent() = default;
	DropItemComponent(std::string DropItemname,int count =1) : mItemName(DropItemname), mCount(count) {}
	std::string GetDropItemName() const { return mItemName; }
	int GetCount() const { return mCount; }
	void SetCount(int i) { mCount = i; }
private:
	std::string mItemName;
	int mCount = 1;

};