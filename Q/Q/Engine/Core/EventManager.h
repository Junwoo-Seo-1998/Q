/*
	File Name: EventManager.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include <memory>
#include <array>
#include "IEvent.h"
#include "../EngineComponent.h"
#include "Engine/Types.h"
namespace q_engine
{
	class EventManager : public EngineComponent<EventManager>
	{
	private:
		static const int MAX_PENDING = 1024;

		std::array<std::shared_ptr<IEvent>, MAX_PENDING> mEventArrays;
		IDType mBeginQueue{ 0 };
		IDType mEndQueue{ 0 };

		void ResolveEvents();
	public:
		template <typename T, typename... Args>
		void AddEvent(Args&&... args);

		void Update(double) override { ResolveEvents(); }
		void ClearEvent() { mBeginQueue = mEndQueue; }
	};


}

#include "EventManager.inl"