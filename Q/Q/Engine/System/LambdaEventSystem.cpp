/*
	File Name: LambdaEventSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "LambdaEventSystem.h"

#include "Engine/Event/LambdaEvent.h"

void q_engine::LambdaEventSystem::OnEvent(IEvent* eventName)
{
	LambdaEvent* lambda = dynamic_cast<LambdaEvent*>(eventName);
	if(lambda != nullptr)
	{
		lambda->mFunction();
	}
}
