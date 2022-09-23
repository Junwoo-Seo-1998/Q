/*
    File Name: Timer.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Timer.h"



namespace q_engine
{
	Timer::Timer(const float amount, const bool doTick, std::function<void()>&& onTimeOut, const bool bPauseOnTimeOut)
		: mOriginalTime(amount), mRemainingTime(amount), mOnTimeOut(std::move(onTimeOut)),
		mDoTick(doTick), mbPauseOnTimeOut(bPauseOnTimeOut)
	{}


	void Timer::Update(const float deltaTime)
	{
		if (!mDoTick)
		{
			return;
		}

		mRemainingTime -= deltaTime;
		if (mRemainingTime <= 0)
		{
			onTimeOut();
		}
	}


	void Timer::onTimeOut()
	{
		if (mbPauseOnTimeOut)
		{
			Pause();
		}

		mOnTimeOut();
	}
}
