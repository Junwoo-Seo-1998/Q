/*
	File Name: TutorialFlowSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Sieun Choi
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "TutorialFlowSystem.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/GameState.h"
#include "Entity/PhysicsTestEntities.h"
#include "Engine/Core/StringHash.h"

void TutorialFlowSystem::Load()
{
	CurrScene = TutorialScene::WALK;
}

void TutorialFlowSystem::Update(double deltaTime)
{
	auto PlayerId = m_controlled_state->GetEntities<PlayerEntity>().Front().GetEntityID();
	auto ContextID = m_controlled_state->GetEntities<TutorialEntity>().Front().GetEntityID();
	auto EnemeyID = m_controlled_state->GetEntities<EnemyEntity>().Front().GetEntityID();
	

	switch(CurrScene)
	{
	case TutorialScene::WALK:
		{
			auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();

			if(PlayerPos.x > 330.0f)
			{
				CurrScene = TutorialScene::JUMP;
			}
			break;
		}
	case TutorialScene::JUMP:
		{
			auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("JumpKey"));

			if (PlayerPos.y > 480.0f)
			{
				CurrScene = TutorialScene::PICKITEM;
			}
			break;
		}
	case TutorialScene::PICKITEM:
		{
			auto Weapon = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].GetSlotWeaponName(1);
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("PickKey"));

			if (Weapon == "Gun")
			{
				CurrScene = TutorialScene::SWAPWEAPON;
			}
			break;
		}
	case TutorialScene::SWAPWEAPON:
		{
			auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();
			auto Weapon = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].GetCurrentWeapon();
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("SwapWeapon"));

			if (PlayerPos.y >= 900.0f && Weapon == "Gun")
			{
				CurrScene = TutorialScene::SLIDE;
			}
			break;
		}
	case TutorialScene::SLIDE:
	{
		auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();
		auto Weapon = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].GetCurrentWeapon();
		m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("MoveSlide"));
		if (PlayerPos.y >= 900.0f && PlayerPos.x <= 1100.0f && Weapon == "Gun")
		{
			if (m_controlled_state->GetComponents<HealthComponent>().Size() > 1)
			{
				CurrScene = TutorialScene::ATTACK;
			}
			else
			{
				CurrScene = TutorialScene::PICKBOMB;
			}
		}
		break;
	}
	case TutorialScene::ATTACK:
		{
			auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();
			bool dead = m_controlled_state->GetComponents<HealthComponent>()[EnemeyID].IsDead();
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("Attack"));

			if (dead)
			{
				CurrScene = TutorialScene::PICKBOMB;
			}
			break;
		}
	case TutorialScene::PICKBOMB:
		{
			auto Item = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].GetCurrentItem();
			auto exist = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].IsItemSlotEmpty(0);
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("PickBomb"));
			
			if (Item != "Bomb" && !exist)
			{
				CurrScene = TutorialScene::SWAPITEM;
			}
			else if(Item == "Bomb")
			{
				CurrScene = TutorialScene::USEITEM;
			}
			break;
		}
	case TutorialScene::SWAPITEM:
		{
			auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("SwapItem"));
			auto Item = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].GetCurrentItem();
			if(Item == "Bomb")
			{
				CurrScene = TutorialScene::USEITEM;
			}
			break;
		}
	case TutorialScene::USEITEM:
		{
			auto PlayerPos = m_controlled_state->GetComponents<q_engine::TransformComponent>()[PlayerId].GetPosition();
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("UseItem"));
			if (m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].GetCurrentItemCount() < 3)
			{
				m_controlled_state->GetComponents<NewPlayerInventorySlots>()[PlayerId].SetCurrentItemCount(10);
			}
			
			if (PlayerPos.y <= 900.0f)
			{
				CurrScene = TutorialScene::GETPORTAL;
			}
			break;
		}
	case TutorialScene::GETPORTAL:
		{
			m_controlled_state->GetComponents<q_engine::TextureComponent>()[ContextID].SetTexture(q_engine::get_hash_id("GetPortal"));
			break;
		}
	}
	
}
