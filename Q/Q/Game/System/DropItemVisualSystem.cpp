/*
	File Name: DropItemVisualSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "DropItemVisualSystem.h"
#include "../Game/Component/DropItemComponent.h"
#include "../Game/Component/DropWeaponComponent.h"
#include "Component/DropCubeComponent.h"
#include "Engine/GameState.h"
#include "Engine/Component/OffsetComponent.h"
void DropItemVisualSystem::Load()
{
	
}

void DropItemVisualSystem::Update(double deltaTime)
{
	auto& CurrentLevel = m_controlled_state;
	auto& dropitemcomponents = CurrentLevel->GetComponents<DropItemComponent>();
	auto& dropWeaponcomponents = CurrentLevel->GetComponents<q_engine::DropWeaponComponent>();
	auto& dropCubecomponents = CurrentLevel->GetComponents<DropCubeComponent>();
	
	for(DropItemComponent& component : dropitemcomponents)
	{
		q_engine::IDType CompID = component.GetEntityID();
		q_engine::OffsetComponent& offset = CurrentLevel->GetComponents<q_engine::OffsetComponent>()[CompID];
		offset.SetHotSpotPosition(glm::vec2{ 0.f, -10 + 10 * cos(time) });
	}

	for (q_engine::DropWeaponComponent& component : dropWeaponcomponents)
	{
		q_engine::IDType CompID = component.GetEntityID();
		q_engine::OffsetComponent& offset = CurrentLevel->GetComponents<q_engine::OffsetComponent>()[CompID];
		offset.SetHotSpotPosition(glm::vec2{ 0.f,  10 * cos(time) });
	}

	for (DropCubeComponent& component : dropCubecomponents)
	{
		q_engine::IDType CompID = component.GetEntityID();
		q_engine::OffsetComponent& offset = CurrentLevel->GetComponents<q_engine::OffsetComponent>()[CompID];
		offset.SetHotSpotPosition(glm::vec2{ 0.f,  10 * cos(time*2) });
	}
	
	time += deltaTime*2;
}

void DropItemVisualSystem::Unload()
{
	
}
