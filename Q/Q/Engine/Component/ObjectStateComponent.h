/*
	File Name: ObjectStateComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <sol/state.hpp>


#include "WidthHeightComponent.h"
#include "../ECS/Component.h"



namespace q_engine
{
	enum class MovingState
	{
		IDLE,
		RUNNING,
		FALLING,
		SKIDDING,
		JUMPING,
		SLIDEING,
		DASHING,
		CRAWLING,
		BEEING,
		CLIMBING,
		CLIMBINGUP,
		CLIMBINGDOWN,
		ISDEAD,
	};
	void RegisterMovingStateEnum(sol::state& state);


	
	class ObjectStateComponent : public Component<ObjectStateComponent>
	{
	public:
		ObjectStateComponent() = default;
		ObjectStateComponent(float heightSize) : mObjectHeightSize(heightSize) {}
		ObjectStateComponent(float WidthSize, float heightSize) : mObjectHeightSize(heightSize), mObjectWidthSize(WidthSize) {}
		void setIsFlipped(bool isflipped);
		bool GetIsFlipped() const;

		MovingState GetPlayerMovingState() const;
		void ChangeMovingState(MovingState state);
		float GetObjectHeightSize() const;
		void SetIsGroundOntheHead(bool status);
		bool GetIsGroundOntheHead() const;
		float GetObjectWidthSize() const;
		

	private:
		MovingState mMovingState = MovingState::IDLE;
		bool misFlipped = false;
		float mObjectHeightSize = 139.f;
		bool mIsGroundOntheHead = false;
		float mObjectWidthSize = 35.f;
	};

	void RegisterMovingStateComponent(sol::state& state);


}
