/*
	File Name: ThreadPool.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>
#include <array>
#include <queue>

#include "../Engine/EngineComponent.h"

namespace q_engine
{
	constexpr int MAX_SUPPORT_THREAD = 64;
	class ThreadPool : public EngineComponent<ThreadPool>
	{
		std::vector<std::thread> mThreads;
		
		std::queue<std::function<void()>> mThreadWorks;
		std::array<std::atomic_bool, MAX_SUPPORT_THREAD> mIsWorking;
		std::mutex mThreadWaitMutex;
		std::mutex mWaitMutex;
		std::condition_variable mConditionVariable;

		std::condition_variable mFinishCondition;
		
		std::atomic<int> mNumWorking = 0;

		static std::mutex mGlobalMutex;
		static const unsigned int mMaxThreads;

		std::atomic<bool> mIsThreadEnd = false;
		
		void ThreadLoop(int threadNum);
		
	public:


		
		ThreadPool();
		~ThreadPool();
		
		static std::mutex& GetMutex() { return mGlobalMutex; }
		static const unsigned int GetMaxThreads() { return mMaxThreads; }

		void WakeUpThreads(int num);
		void WaitAll();
		

		template<typename Func, typename... Args>
		void GiveThreadWork(Func function, Args&&... Arguments);
	};
}

#include "ThreadPool.inl"