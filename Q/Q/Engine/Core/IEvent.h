/*
	File Name: IEvent.h
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary: Minjae Kyung
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/IDGenerator.h"


namespace q_engine
{
	class IEvent
	{
	protected:
		virtual void ResolveEvent() {}
	public:
		virtual ~IEvent() = default;
		
		virtual void NotifyAll() = 0;

		friend class EventManager;
	};
}
