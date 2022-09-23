/*
	File Name: ThreadPool.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

namespace q_engine
{



	
	template<typename Func, typename ...Args>
	void ThreadPool::GiveThreadWork(Func func, Args&&... args)
	{
		{
			std::lock_guard<std::mutex> mutex(mThreadWaitMutex);
			mThreadWorks.push([func, &args...]() { func(std::forward<Args>(args)...); });
		}
		mConditionVariable.notify_one();
		return;
	}
}

