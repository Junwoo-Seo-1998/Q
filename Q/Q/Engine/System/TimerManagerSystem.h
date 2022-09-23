/*
    File Name: TimerManagerSystem.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/System.h"

#include "../Core/Timer.h"
#include "Engine/Types.h"

#include <sol/sol.hpp>

namespace q_engine
{
	
	class TimerManagerSystem final : public System<TimerManagerSystem>
	{
	public:
		TimerManagerSystem(GameState* state) : System<TimerManagerSystem>(state) {}
		void Update(double deltaTime) override;
		void Unload() override;


		[[nodiscard]] static TimerID IssueTimer(float amount, bool doTick = true, std::function<void()>&& onTimeOut = []() {}, bool bPauseOnTimeOut = true);
		static Timer& GetTimer(TimerID timerID);
		static bool HasTimer(TimerID timerID);
		static void YieldTimer(TimerID timerID);
		static void DelayedYieldTier(TimerID timerID);
	private:
		inline static std::unordered_map<TimerID, Timer> timers;
		inline static std::list<TimerID> delayedDeleteQue;
	};

    inline void RegisterTimerManagerSystem(sol::state& state)
	{
		sol::usertype<TimerManagerSystem> timerManager = state.new_usertype<TimerManagerSystem>("TimerManagerSystem");

		timerManager.set_function("IssueTimer", &TimerManagerSystem::IssueTimer);
		timerManager.set_function("GetTimer", &TimerManagerSystem::GetTimer);
		timerManager.set_function("YieldTimer", &TimerManagerSystem::YieldTimer);
	}
}
