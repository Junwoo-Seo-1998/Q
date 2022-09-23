/*
	File Name: EventManager.inl
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include "EventManager.h"

namespace q_engine
{
	template <typename T, typename... Args>
	void EventManager::AddEvent(Args&&... args)
	{
		static_assert(std::is_base_of_v<IEvent, T>, "T is not Event when Add Event");
		
		mEventArrays[mEndQueue] = std::make_shared<T>(std::forward<Args>(args)...);

		mEndQueue++;

		if (mEndQueue >= MAX_PENDING)
		{
			//Call logger to queue number reset to 0
			mEndQueue = 0;
		}

		if (mEndQueue == mBeginQueue)
		{
			//Call logger error (interrupt queue)
			mBeginQueue++;
			if (mBeginQueue >= MAX_PENDING)
			{
				//Call logger to begin queue number reset to 0
				mBeginQueue = 0;
			}
		}
	}

	inline void EventManager::ResolveEvents()
	{
		for (unsigned i = mBeginQueue; i != mEndQueue; ++i)
		{
			if (i >= MAX_PENDING)
			{
				i = static_cast<unsigned>(-1);
				continue;
			}

			mEventArrays[i]->ResolveEvent();
			mEventArrays[i]->NotifyAll();
		}

		mBeginQueue = mEndQueue;
	}
}