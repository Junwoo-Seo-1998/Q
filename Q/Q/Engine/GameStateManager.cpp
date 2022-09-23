/*
	File Name: GameStateManager.cpp
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary: Minjae Kyung
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "GameStateManager.h"
#include "Engine.h"
#include "../Engine/ECS/InputManager.h"
#include "../Levels/SplashState.h"
#include "Levels/CreditState.h"
#include "Levels/HowToPlayState.h"
#include "Levels/Level1State.h"
#include "Levels/MainMenuState.h"
#include "Levels/OptionMenuState.h"
#include "Levels/StoryState.h"
#include "Levels/TutorialState.h"
#include "Levels/UITestState.h"
#include "Levels/InGameOptionMenuState.h"
namespace q_engine
{
	std::shared_ptr<GameState> GameStateManager::GetPlayState() const
	{
		static auto pGameState = mRunningStates.at(GameStateType::GAME);
		if(mGameLocationDirty)
		{
			pGameState = mRunningStates.at(GameStateType::GAME);
			mGameLocationDirty = false;
		}
		return pGameState;
	}

	std::shared_ptr<GameState> GameStateManager::GetUIState() const
	{
		static auto pUIState = mRunningStates.at(GameStateType::UI1);
		if (mUILocationDirty)
		{
			pUIState = mRunningStates.at(GameStateType::UI1);
			mUILocationDirty = false;
		}
		return pUIState;
	}

	void GameStateManager::MoveEntityComponents(std::shared_ptr<GameState> from, std::shared_ptr<GameState> target)
	{
		mEntityComponentMover.MoveEntities(from->mEntityManager, target->mEntityManager);
		mEntityComponentMover.MoveComponents(target->mEntityManager, from->mComponentManager, target->mComponentManager);
	}

	void GameStateManager::Load()
	{
		AddState(GameStateType::SPLASH_SCREEN, std::make_shared<SplashState>());
	}

	void GameStateManager::Update(double deltaTime)
	{
		//Update
		for (auto& state : mRunningStates)
		{
		    {
				state.second->Update(deltaTime);

				if (!state.second->IsTransParent())
				{
					mUpdateStatus = state.second->mType;
					break;
				}
				mUpdateStatus = state.second->mType;
		    }
		}

		//Draw
		for (auto& state : mRunningStates)
		{
			state.second->Draw(deltaTime);
		}

		//Delete
		for (auto stateType : mDeletingQue)
		{
			if (mRunningStates.find(stateType) != mRunningStates.end())
			{
				mRunningStates[stateType]->Unload();
				mRunningStates.erase(stateType);
			}
		}
		if (!mDeletingQue.empty())
		{
			Engine::GetEngineComponent<InputManager>().Update(deltaTime);
		}
		mDeletingQue.clear();
		for (auto& state : mAddingQue)
		{
			mGameLocationDirty = true;
			mUILocationDirty = true;
			mRunningStates.try_emplace(state.first, state.second);
			state.second->Load();
		}
		mAddingQue.clear();
	}

	void GameStateManager::RemoveLater()
	{
		for (auto& state : mRunningStates)
		{
			state.second->DeleteEntities();
		}
	}

	void GameStateManager::Unload()
	{
		
	}

    IDType GameStateManager::GetPlayerID() const
	{
		return GetPlayState()->GetEntities<PlayerEntity>().begin()->GetEntityID();
	}

	void GameStateManager::AddState(GameStateType type, std::shared_ptr<GameState> pState)
	{
		const auto typeLocation = static_cast<unsigned>(type);
		if(typeLocation < static_cast<unsigned>(GameStateType::GAME))
		{
			mGameLocationDirty = true;
		}
		if(typeLocation < static_cast<unsigned>(GameStateType::UI2))
		{
			mUILocationDirty = true;
		}
		pState->SetStateType(type);
		if(type == GameStateType::MAIN_MENU_BUTTON)
		{
			mPlayedLevel = 1;
		}
		if(type == GameStateType::GAME && HasState(GameStateType::GAME))
		{
			++mPlayedLevel;
		}
		mAddingQue.emplace_back(std::make_pair(type, pState));
	}
	void GameStateManager::RemoveState(GameStateType type)
	{
		if(mRunningStates.find(type) != mRunningStates.end())
		{
			mDeletingQue.emplace_back(type);
		}
	}
	void GameStateManager::RemoveState(std::string&& type_as_string)
	{
		RemoveState(ConvertToType(type_as_string));
	}
	void GameStateManager::AddState(IDType stateID)
	{
		constexpr IDType map_test = "MapTestState"_hashID;
		constexpr IDType ui_test = "UITestState"_hashID;
		constexpr IDType in_game_menu_2 = "InGameMenu2"_hashID;
		constexpr IDType in_game_menu_1 = "InGameMenu"_hashID;
		constexpr IDType to_credit = "CreditState"_hashID;
		constexpr IDType to_how_to_play = "HowToPlayState"_hashID;
		constexpr IDType to_main_menu = "MainMenuState"_hashID;
		constexpr IDType to_story_state = "ToStoryState"_hashID;
		constexpr IDType to_option_state = "ToOptionState"_hashID;
		constexpr IDType quit_check_state = "QuitCheck"_hashID;
		constexpr IDType return_mainmenu_check_state = "BackToMainMenuCheck"_hashID;

		switch (stateID)
		{
		case map_test:
		{
			if (!MapGeneration::tutorialFinished)
			{
				AddState(GameStateType::GAME, std::make_shared<TutorialState>());
			}
			else
			{
				AddState(GameStateType::GAME, std::make_shared<Level1State>());
			}
			break;
		}
		case ui_test:
		{
			AddState(GameStateType::UI1, std::make_shared<UITestState>());
			break;
		}
		case to_credit:
		{
			AddState(GameStateType::OTHER_MENU, std::make_shared<CreditState>());
			break;
		}
		case to_how_to_play:
		    {
			AddState(GameStateType::OTHER_MENU, std::make_shared<HowToPlayState>());
			break;
		    }
		case to_main_menu:
		    {
			AddState(GameStateType::MAIN_MENU_BUTTON, std::make_shared<MainMenuState>());
			break;
		    }
		case to_story_state:
		    {
			AddState(GameStateType::OTHER_MENU, std::make_shared<StoryState>());
			break;
		    }
		case to_option_state:
		    {
			AddState(GameStateType::MAIN_MENU_BUTTON, std::make_shared<OptionMenuState>());
			break;
		    }
		case in_game_menu_2:
		    {
			AddState(GameStateType::IN_GAME_MENU2, std::make_shared<InGameOptionMenuState>());
			break;
		    }
		case in_game_menu_1:
		    {
			AddState(GameStateType::IN_GAME_MENU, std::make_shared<InGameMenuState>());
			break;
		    }
		case quit_check_state: {
			AddState(GameStateType::IN_GAME_MENU3, std::make_shared<PlayerShutdownAskState>());
			break;
		}
		case return_mainmenu_check_state: 
		{
			AddState(GameStateType::IN_GAME_MENU3, std::make_shared<PlayerGoBackMainMenuAskState>());
			break;
		}
		default:
			throw "Logic error";
		}
	}

}
