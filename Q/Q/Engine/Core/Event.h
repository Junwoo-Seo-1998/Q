/*
	File Name: Event.h
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary: Minjae Kyung
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine.h"
#include "../GameState.h"
#include "../GameStateManager.h"
#include "IEvent.h"



namespace q_engine
{
	template <typename T, typename... ListeningSystems_>
	class Event : public IEvent
	{
		using ListeningComponents = std::tuple<ListeningSystems_...>;
		void NotifyAll() override final;
		

		inline static int typeID = TypeIDGenerator<T>::GetValue();

		friend class EventManager;

	};
}

#include "Event.inl"
