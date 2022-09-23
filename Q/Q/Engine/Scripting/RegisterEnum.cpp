/*
	File Name: RegisterEnum.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "RegisterEnum.h"

#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Graphics/GraphicsMisc.h"
#include "Game/Component/DamageComponent.h"
namespace q_engine
{
	void RegisterEnum(sol::state& state)
	{
		RegisterEFrameOfReferenceEnum(state);
		RegisterBulletTypeEnum(state);
		RegisterMovingStateEnum(state);
	}
}
