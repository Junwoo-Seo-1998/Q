/*
	File Name: RegisterComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "RegisterComponent.h"


#include "Component/HealthComponent.h"
#include "Component/NewItemComponent.h"
#include "Component/NewPlayerInventorySlots.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/CollisionComponent.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/SoundComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "../Game/Component/DamageComponent.h"
#include "Engine/Core/Timer.h"


namespace q_engine
{
	void RegisterComponent(sol::state& state)
	{
		RegisterTransformComponent(state);
		RegisterAnimationComponent(state);
		RegisterTextureComponent(state);
		RegisterOffsetComponent(state);
		RegisterNewItemComponent(state);
		RegisterVelocityComponent(state);
		RegisterSoundComponent(state);
		RegisterStaticRectCollisionComponent(state);
		RegisterHealthComponent(state);
		RegisterWidthHeightComponent(state);
		RegisterMovingStateComponent(state);
		RegisterNewPlayerInventorySlotsComponent(state);
		RegisterDamageComponent(state);
	}
}
