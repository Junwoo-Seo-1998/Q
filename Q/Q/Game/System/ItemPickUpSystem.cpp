/*
	File Name: ItemPickUpSystem.cpp
	Project Name: Q
	Author(s):
		Primary:  Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ItemPickUpSystem.h"



#include "EObjectZDepth.h"
#include "../../Engine/Component/TextureComponent.h"
#include "../../Engine/Event/TriggerEvent.h"
#include "../Entity/ItemEntity.h"
#include "../Entity/PhysicsTestEntities.h"
#include "../Entity/WeaponEntity.h"
#include "../Game/Component/NewWeaponComponent.h"
#include "../Game/Component/NewPlayerInventorySlots.h"
#include "Component/DropWeaponComponent.h"
#include "Engine/ECS/InputManager.h"
#include "Component/NewItemComponent.h"
#include "Component/DropItemComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/WidthHeightComponent.h"
#include "PlayTestTrace.h"

void ItemPickUpSystem::Load()
{
	q_engine::Binding* const pickUpItem = new q_engine::Binding("pickUpItem");
	pickUpItem->BindInput(InputType::KEY_PRESS, Key::F);
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().AddBinding(pickUpItem);
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().AddCallback("pickUpItem", &ItemPickUpSystem::TryPickUp, this);

	
}

void ItemPickUpSystem::Unload()
{
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().RemoveBinding("pickUpItem");
	q_engine::Engine::GetEngineComponent<q_engine::InputManager>().RemoveCallback("pickUpItem");
}


void ItemPickUpSystem::Update(double)
{
	
}


void ItemPickUpSystem::OnEvent(q_engine::IEvent* event)
{
	using namespace q_engine;

	if (TriggerEvent* triggerEvent = dynamic_cast<TriggerEvent*>(event); triggerEvent)
	{
		
		if (!mbPickUp || triggerEvent->mTriggerEntityTypeID != q_engine::GetTypeID<ItemEntity>()
			|| triggerEvent->mCollideWithEntityTypeID != q_engine::GetTypeID<PlayerEntity>())
		{
			mbPickUp = false;
			return;
		}

		mbPickUp = false;
		GameState* const currentLevel = m_controlled_state;

		auto& playerInventorySlot = currentLevel->GetComponents<NewPlayerInventorySlots>()[triggerEvent->mCollideWithEntityID];
		auto& playerEntity = currentLevel->GetEntities<PlayerEntity>().Front();
		if(currentLevel->HasComponent<DropItemComponent>(triggerEvent->mTriggerEntityID))
		{
			auto& dropitemcomponent = currentLevel->GetComponents<DropItemComponent>()[triggerEvent->mTriggerEntityID];
			const int slotsize = 2;

			for (int i = 0; i < slotsize; i++)
			{
				if (dropitemcomponent.GetDropItemName() == playerInventorySlot.GetSlotItemName(i))
				{
					playerInventorySlot.SetSlotItemName(i, dropitemcomponent.GetDropItemName(),dropitemcomponent.GetCount());
					currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
					Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);
					return;
				}
			}
			
			for(int i = 0; i < slotsize; i++)
			{
				if(playerInventorySlot.IsItemSlotEmpty(i))
				{
					playerInventorySlot.SetSlotItemName(i, dropitemcomponent.GetDropItemName(),dropitemcomponent.GetCount());
					currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
					Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);

					PlayTestTrace::LogPickUpMessage(dropitemcomponent.GetDropItemName(), "");
					return;
				}
			}
			// item is full check same value


			// Chanage the current weapon
			TransformComponent& playertransform = currentLevel->GetComponents<TransformComponent>()[playerEntity.GetEntityID()];

			//save it
			std::string willdroppeditem = playerInventorySlot.GetCurrentItem();
			int willdroppeditemcount = playerInventorySlot.GetCurrentItemLeft();

			//change the item
			playerInventorySlot.SetSlotItemName(playerInventorySlot.GetCurrentItemIndex(), dropitemcomponent.GetDropItemName(),dropitemcomponent.GetCount()- willdroppeditemcount);
			currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
			Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);

			PlayTestTrace::LogPickUpMessage(dropitemcomponent.GetDropItemName(), willdroppeditem);

			const IDType willdroppeditemhash = q_engine::get_hash_id(willdroppeditem + "Icon");
			
			//make dropped item
			ItemEntity* const droppedItem = currentLevel->AddEntity<ItemEntity>();

			currentLevel->AddComponent<DropItemComponent>(droppedItem, willdroppeditem,willdroppeditemcount);
			currentLevel->AddComponent<TransformComponent>(droppedItem, playertransform.GetPosition());
			currentLevel->AddComponent<TextureComponent>(droppedItem, willdroppeditemhash, static_cast<int>(EObjectZDepth::ITEM));
			currentLevel->AddComponent<OffsetComponent>(droppedItem);
			currentLevel->AddComponent<WidthHeightComponent>(droppedItem, 32.f, 32.f);
			currentLevel->AddComponent<GravityComponent>(droppedItem);
			currentLevel->AddComponent<VelocityComponent>(droppedItem);
			currentLevel->AddComponent<RectCollisionComponent>(droppedItem, playertransform.GetPosition(), 32.f, 32.f, false, true);
			
		}
		else
		{
			auto& dropweaponcomponent = currentLevel->GetComponents<DropWeaponComponent>()[triggerEvent->mTriggerEntityID];

			const int slotsize = 2;

			for (int i = 0; i < slotsize; i++)
			{
				if (dropweaponcomponent.GetDropWeaponName() == playerInventorySlot.GetSlotWeaponName(i))
				{
					return;
				}
			}

			for (int i = 0; i < slotsize; i++)
			{
				if (playerInventorySlot.IsWeaponSlotEmpty(i))
				{
					playerInventorySlot.SetSlotWeaponName(i, dropweaponcomponent.GetDropWeaponName());
					currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
					Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);
					PlayTestTrace::LogPickUpMessage(dropweaponcomponent.GetDropWeaponName(), "");
					return;
				}
			}


			TransformComponent& playertransform = currentLevel->GetComponents<TransformComponent>()[playerEntity.GetEntityID()];

			//save it
			std::string willdroppedweapon = playerInventorySlot.GetCurrentWeapon();

			//change the item
			playerInventorySlot.SetSlotWeaponName(playerInventorySlot.GetCurrentWeaponIndex(), dropweaponcomponent.GetDropWeaponName());
			currentLevel->DeleteEntityComponents(triggerEvent->mTriggerEntityID);
			Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("item_pickup", false);

			const IDType willdroppedweaponhash = q_engine::get_hash_id(willdroppedweapon + "Icon");
			PlayTestTrace::LogPickUpMessage(dropweaponcomponent.GetDropWeaponName(), willdroppedweapon);
			//make dropped item
			ItemEntity* const droppedItem = currentLevel->AddEntity<ItemEntity>();

			currentLevel->AddComponent<DropWeaponComponent>(droppedItem, willdroppedweapon);
			currentLevel->AddComponent<TransformComponent>(droppedItem, playertransform.GetPosition());
			currentLevel->AddComponent<TextureComponent>(droppedItem, willdroppedweaponhash, static_cast<int>(EObjectZDepth::ITEM));
			currentLevel->AddComponent<OffsetComponent>(droppedItem);
			currentLevel->AddComponent<WidthHeightComponent>(droppedItem, 32.f, 32.f);
			currentLevel->AddComponent<GravityComponent>(droppedItem);
			currentLevel->AddComponent<VelocityComponent>(droppedItem);
			currentLevel->AddComponent<RectCollisionComponent>(droppedItem, playertransform.GetPosition(), 32.f, 32.f, false, true);
			
		}
	}
}
