/*
	File Name: GamePlayLogicUpdateSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GamePlayLogicUpdateSystem.h"

#include <iostream>



#include "../../Engine/Core/EventManager.h"
#include "../../Game/Entity/PhysicsTestEntities.h"
#include "../Component/HealthComponent.h"
#include "../Engine/GameState.h"
#include "../Engine/Engine.h"
#include "../Engine/GameStateManager.h"
#include "../Levels/GameOverState.h"
#include "Component/UIComponents/UIItemComponents.h"
#include "Engine/Component/AnimationComponent.h"
#include "Engine/Core/Timer.h"
#include "Engine/System/PlayerMovingSystem.h"
#include "Engine/System/TimerManagerSystem.h"
#include "Entity/MapEntity.h"
#include "Entity/UIEntities/UserFocusEntity.h"
#include "Levels/GameEndingState.h"
#include "UI/PlayerHealthBarUpdateSystem.h"
#include "Levels/Level3State.h"
#include "Engine/Graphics/PostProcessor.h"

void GamePlayLogicUpdateSystem::Load()
{
	is_PostProcessHandled = false;
}

void GamePlayLogicUpdateSystem::Update(double deltaTime)
{
	PlayerEntity& player = m_controlled_state->GetEntities<PlayerEntity>().Front();
	q_engine::GameStateManager& state_manager = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>();
	q_engine::IDType playerID = player.GetEntityID();
	NewPlayerInventorySlots& playerInventory = m_controlled_state->GetComponents<NewPlayerInventorySlots>()[playerID];
	if(on_end_cond == false)
	{
		if (playerInventory.GetCubeAmount() >= NewPlayerInventorySlots::GetMaxCubeAmount())
		{
			auto& portals = m_controlled_state->GetEntities<q_engine::PortalEntity>();
			const unsigned portalID = m_controlled_state->GetEntities<q_engine::PortalEntity>().Front().GetEntityID();
			m_controlled_state->GetComponents<q_engine::AnimationComponent>()[portalID].SetAnimation(q_engine::get_hash_id("EndPortalAnim"));
			state_manager.GetUIState()->GetComponents<UIItemTextureComponent>()[state_manager.GetUIState()->GetEntities<ItemBoxEntity>().Front().GetEntityID()].
		    RegisterTexture("Cube", q_engine::get_hash_id("FullCubeIcon"));
			q_engine::Engine::GetEngineComponent<q_engine::SoundComponent>().PlaySoundEffect("portal-open", true);
			on_end_cond = true;
		}
	}

	auto player_health = m_controlled_state->GetComponents<HealthComponent>().find(playerID);
	if(player_health->GetCurHealth() <= 0)
	{
		auto pPlayState = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState();
		const std::string curPlayerStateAsString = pPlayState->GetUpdateSystem<q_engine::MovingSystem>().GetCurrentMovingState();
		
		if (curPlayerStateAsString == "IsDead" && is_PostProcessHandled == false)
		{
			is_PostProcessHandled = true;
			q_engine::Engine::ApplySlowMotion(2.f, 0.3f);
			q_engine::Engine::GetPostProcessor()->SetMode(Inversion);
			pPlayState->GetUpdateSystem<q_engine::TimerManagerSystem>().IssueTimer(pPlayState->GetComponents<q_engine::AnimationComponent>()[q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayerID()].GetLength(), true, 
				[]()
				{
					q_engine::Engine::StopSlowMotion();
					q_engine::Engine::GetPostProcessor()->SetMode(Normal);
					q_engine::GameStateManager& state_manager = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>();

					state_manager.ClearStates();
					state_manager.AddState(GameStateType::OTHER_MENU, std::make_shared<GameOverState>());					
				});
		}

	}
}
