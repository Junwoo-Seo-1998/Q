/*
	File Name: TriggerEvent.cpp
	Project Name: Q
	Author(s):
		Primary: Joonho Hwang
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TriggerEvent.h"



namespace q_engine
{
	TriggerEvent::TriggerEvent(const IDType triggerEntityTypeID, const IDType triggerEntityID, const IDType collideWithEntityTypeID, const IDType collideWithEntityID)
		: mTriggerEntityTypeID(triggerEntityTypeID), mTriggerEntityID(triggerEntityID), mCollideWithEntityTypeID(collideWithEntityTypeID), mCollideWithEntityID(collideWithEntityID)
	{}
}
