/*
    File Name: Timer.h
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <functional>
#include <sol/sol.hpp>


namespace q_engine
{
	class Timer
	{
	public:
		Timer(float amount = 0.f, bool doTick = true, std::function<void()>&& onTimeOut = []() {}, bool bPauseOnTimeOut = true);


		// Should be called every frame.
		void Update(float deltaTime);

		constexpr void SetPaused(bool isPaused = true) { mDoTick = !isPaused; }
		constexpr void Pause() noexcept { SetPaused(true); }
		constexpr void Resume() noexcept { SetPaused(false); }
		constexpr void TogglePause() noexcept { SetPaused(!IsPaused()); }

		constexpr void Reset() noexcept { mRemainingTime = mOriginalTime; }
		constexpr void Reset(float newAmount) noexcept { mOriginalTime = newAmount; mRemainingTime = newAmount; }

		constexpr void ResetAndPause() noexcept { Reset(); Pause(); }
		constexpr void ResetAndPause(float newAmount) noexcept { Reset(newAmount); Pause(); }
		constexpr void ResetAndResume(float newAmount) noexcept { Reset(newAmount); Resume(); }
		constexpr void ResetAndResume() noexcept { Reset(mOriginalTime); Resume(); }

		constexpr void AddTime(float amount) noexcept { mRemainingTime += amount; }

		[[nodiscard]] constexpr float GetRemainingTime() const noexcept { return mRemainingTime; }
		[[nodiscard]] constexpr float GetOriginalTime() const noexcept { return mOriginalTime; }

		[[nodiscard]] constexpr bool IsPaused() const noexcept { return !mDoTick; }

		void SetOnTimeOut(std::function<void()>&& onTimeOut) { mOnTimeOut = std::move(onTimeOut); }

		bool IsOnTimeOut() const { return mRemainingTime <= 0; }

		void SetTimeOut() { mRemainingTime = 0.f; }
	private:
		void onTimeOut();



	private:
		float mOriginalTime;
		float mRemainingTime;

		std::function<void()> mOnTimeOut;

		bool mDoTick;
		bool mbPauseOnTimeOut;
	};

	inline void RegisterTimerType(sol::state& state)
	{
		sol::usertype<Timer> timer = state.new_usertype<Timer>("Timer");
		timer.set_function("SetTimeOut", &Timer::SetTimeOut);
		timer.set_function("IsOnTimeOut", &Timer::IsOnTimeOut);
		timer.set_function("ResetAndResume", sol::resolve<void (void)>(&Timer::ResetAndResume));
		timer.set_function("IsPaused", &Timer::IsPaused);
	}
}
