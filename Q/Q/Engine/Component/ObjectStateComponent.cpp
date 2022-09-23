/*
	File Name: ObjectStateComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/



#include "ObjectStateComponent.h"


namespace  q_engine
{
	void RegisterMovingStateEnum(sol::state& state)
	{

		std::initializer_list<std::pair<sol::string_view, MovingState>>	MovingStateEnum =
		{
			{"CLIMBING", MovingState::CLIMBING},
			{"CLIMBINGUP", MovingState::CLIMBINGUP},
			{"CLIMBINGDOWN",MovingState::CLIMBINGDOWN},
			{"ISDEAD", MovingState::ISDEAD},
		};

			state.new_enum<MovingState, false>("MovingState", MovingStateEnum);
	}

	void ObjectStateComponent::setIsFlipped(bool isflipped)
	{
		misFlipped = isflipped;
	}

	bool ObjectStateComponent::GetIsFlipped() const
	{
		return misFlipped;
	}

	MovingState ObjectStateComponent::GetPlayerMovingState() const
	{
		return mMovingState;
	}

	void ObjectStateComponent::ChangeMovingState(MovingState state)
	{
		mMovingState = state;
	}

	float ObjectStateComponent::GetObjectHeightSize() const
	{
		return mObjectHeightSize;
	}

	void ObjectStateComponent::SetIsGroundOntheHead(bool status)
	{
		mIsGroundOntheHead = status;
	}

	bool ObjectStateComponent::GetIsGroundOntheHead() const
	{
		return mIsGroundOntheHead;
	}

	float ObjectStateComponent::GetObjectWidthSize() const
	{
		return mObjectWidthSize;
	}

	void RegisterMovingStateComponent(sol::state& state)
	{
		sol::usertype<ObjectStateComponent> objectstatecomponent = state.new_usertype<ObjectStateComponent>("MovingStateComponent");
		objectstatecomponent.set_function("GetPlayerMovingState", &ObjectStateComponent::GetPlayerMovingState);
	}
}
