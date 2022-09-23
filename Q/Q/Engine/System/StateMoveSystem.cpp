/*
	File Name: StateMoveSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "StateMoveSystem.h"
#include "../Event/TriggerEvent.h"
#include "../../Game/Entity/MapEntity.h"
#include "../../Engine/Core/EventManager.h"
#include "Levels/GameEndingState.h"
#include "Levels/Level3State.h"
#include "Levels/Level2State.h"
#include "Levels/UITestState.h"
#include "Levels/Level1State.h"

class MapTestState;

namespace q_engine
{
	void StateMoveSystem::OnEvent(IEvent* eventName)
	{
		if (dynamic_cast<TriggerEvent*>(eventName) != nullptr)
		{
			q_engine::TriggerEvent* triggerEvent = static_cast<q_engine::TriggerEvent*>(eventName);
			
			if (triggerEvent->mTriggerEntityTypeID != q_engine::GetTypeID<PortalEntity>() || triggerEvent->mCollideWithEntityTypeID != q_engine::GetTypeID<PlayerEntity>())
				return;


			auto& gameStateManager = Engine::GetEngineComponent<GameStateManager>();


			const bool end_cond = m_controlled_state->GetComponents<NewPlayerInventorySlots>().Front().GetCubeAmount() >= NewPlayerInventorySlots::GetMaxCubeAmount();
			if(!end_cond)
			{
				StateMoveComponent& moveComp = m_controlled_state->GetComponents<StateMoveComponent>()[triggerEvent->mTriggerEntityID];


				gameStateManager.MoveEntityComponents(gameStateManager.GetPlayState(), moveComp.GetToLevel());

				
				gameStateManager.ClearStates();
				gameStateManager.AddState(GameStateType::GAME, moveComp.GetToLevel());
				gameStateManager.AddState(GameStateType::UI1, std::make_shared<UITestState>());

			}
			else
			{
				gameStateManager.ClearAddingQue();
				gameStateManager.ClearDeletingQue();
				gameStateManager.ClearStates();
				gameStateManager.AddState(GameStateType::OTHER_MENU, std::make_shared<GameEndingState>());
			}

		}
	}
}
