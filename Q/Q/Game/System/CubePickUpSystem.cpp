/*
	File Name: CubePickUpSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "CubePickUpSystem.h"

#include "PlayTestTrace.h"
#include "Component/DropCubeComponent.h"
#include "Engine/ECS/InputManager.h"
#include "Engine/Event/TriggerEvent.h"
#include "Entity/ItemEntity.h"

void CubePickUpSystem::Load()
{
	q_engine::Binding* const pickUpItem = new q_engine::Binding("pickUpCube");
	pickUpItem->BindInput(InputType::KEY_PRESS, Key::F);
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().AddBinding(pickUpItem);
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().AddCallback("pickUpCube", &CubePickUpSystem::TryPickUp, this);
}

void CubePickUpSystem::Unload()
{
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().RemoveBinding("pickUpCube");
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().RemoveCallback("pickUpCube");
}

void CubePickUpSystem::Update([[maybe_unused]] double deltaTime)
{


	
}

void CubePickUpSystem::OnEvent(q_engine::IEvent* event)
{
	using namespace q_engine;

	if (TriggerEvent* triggerEvent = dynamic_cast<TriggerEvent*>(event); triggerEvent)
	{
		if (!mbPickUp || triggerEvent->mTriggerEntityTypeID != q_engine::GetTypeID<CubeEntity>()
			|| triggerEvent->mCollideWithEntityTypeID != q_engine::GetTypeID<PlayerEntity>())
		{
			mbPickUp = false;
			return;
		}

		mbPickUp = false;
		GameState* const currentLevel = m_controlled_state;

		auto& playerInventorySlot = currentLevel->GetComponents<NewPlayerInventorySlots>()[triggerEvent->mCollideWithEntityID];

		if (currentLevel->HasComponent<DropCubeComponent>(triggerEvent->mTriggerEntityID))
		{
			auto& dropcubecomponent = currentLevel->GetComponents<DropCubeComponent>()[triggerEvent->mTriggerEntityID];
	
				if (dropcubecomponent.GetDropCubeName() == playerInventorySlot.GetCubeName())
				{
					playerInventorySlot.SetSlotCubeName( dropcubecomponent.GetDropCubeName(), dropcubecomponent.GetCount());
					currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
					Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);
					return;
				}
			

			
				if (playerInventorySlot.IsCubeSlotEmpty())
				{
					playerInventorySlot.SetSlotCubeName( dropcubecomponent.GetDropCubeName(), dropcubecomponent.GetCount());
					currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
					Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);
					PlayTestTrace::LogPickUpMessage(dropcubecomponent.GetDropCubeName(), "");
					return;
				}
			

		}
	}
}
