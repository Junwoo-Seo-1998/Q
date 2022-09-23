/*
	File Name: ThreadPool.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ThreadPool.h"


#include <iostream>
#include <mutex>

namespace q_engine
{
	
#ifdef _DEBUG
	//const unsigned int ThreadPool::mMaxThreads = std::thread::hardware_concurrency();
	const unsigned int ThreadPool::mMaxThreads = 4;
#else
	const unsigned int ThreadPool::mMaxThreads = std::thread::hardware_concurrency();
	//const unsigned int ThreadPool::mMaxThreads = 1;
#endif
	
	std::mutex ThreadPool::mGlobalMutex;
	
	ThreadPool::ThreadPool()
	{
		for (unsigned i = 0; i < mMaxThreads; ++i)
		{
			mIsWorking[i] = false;
		}
		for (unsigned i = 0; i < mMaxThreads; ++i)
		{
			mThreads.emplace_back([this, i]() { this->ThreadLoop(i); });
		}
	}

	ThreadPool::~ThreadPool()
	{
		mIsThreadEnd = true;
		mConditionVariable.notify_all();
		for(std::thread& thrd : mThreads)
			thrd.join();
	}

	void ThreadPool::WakeUpThreads(int num)
	{
		mNumWorking = num;
	}

	void ThreadPool::WaitAll()
	{
		std::unique_lock<std::mutex> lock(mThreadWaitMutex);

		mFinishCondition.wait(lock, [&]()
			{
				return mThreadWorks.empty();
			});
		
		while (mNumWorking != 0);
		
		{
	
		}

	}

	void ThreadPool::ThreadLoop(int threadNum)
	{
		while (true) 
		{			
			std::unique_lock<std::mutex> lock(mThreadWaitMutex);
			
			mConditionVariable.wait(lock, [&]()
			{
					return !mThreadWorks.empty() || mIsThreadEnd;
			});
			
			if (mIsThreadEnd)
				return;
			
			std::function<void()> workToDo;
			
			workToDo = mThreadWorks.front();
			mThreadWorks.pop();

			
			lock.unlock();
			
			mIsWorking[threadNum] = true;
			workToDo();
			mIsWorking[threadNum] = false;

			--mNumWorking;
		
			
			mFinishCondition.notify_one();
			

		}
	}
	

}

