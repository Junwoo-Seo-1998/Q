/*
	File Name: RegisterDataType.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "RegisterDataType.h"

#include "Engine/GameStateManager.h"
#include "Engine/Graphics/GraphicsMisc.h"
namespace q_engine
{
	void RegisterDataType(sol::state& state)
	{
		RegisterZDepthType(state);
		RegisterGameStateManager(state);
	
	}
}
