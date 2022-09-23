/*
    File Name: TimerManagerSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TimerManagerSystem.h"

#include <iostream>


namespace q_engine
{
	void TimerManagerSystem::Update(double deltaTime)
	{
		const float castDeltaTime = static_cast<float>(deltaTime);

		for (auto& [_, timer] : timers)
		{
			timer.Update(castDeltaTime);
		}

		for (const TimerID timerID : delayedDeleteQue)
		{
			YieldTimer(timerID);
		}
		delayedDeleteQue.clear();

	}


	void TimerManagerSystem::Unload()
	{
		timers.clear();
	}


	TimerID TimerManagerSystem::IssueTimer(const float amount, const bool doTick, std::function<void()>&& onTimeOut, const bool bPauseOnTimeOut)
	{
		static TimerID counter = 0;
		timers[counter] = Timer{ amount, doTick, std::move(onTimeOut), bPauseOnTimeOut };
		return counter++;
	}


	Timer& TimerManagerSystem::GetTimer(const TimerID timerID)
	{
		return timers.at(timerID);
	}

	bool TimerManagerSystem::HasTimer(TimerID timerID)
	{
		return timers.find(timerID) != timers.end();
	}


	void TimerManagerSystem::YieldTimer(const TimerID timerID)
	{
		if (timers.count(timerID))
		{
			timers.erase(timerID);
		}
	}
	void TimerManagerSystem::DelayedYieldTier(TimerID timerID)
	{
		delayedDeleteQue.push_back(timerID);
	}
}
