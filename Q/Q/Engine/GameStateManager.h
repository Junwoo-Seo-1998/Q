/*
	File Name: GameStateManager.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary: Minjae Kyung
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once

#include <sol/usertype.hpp>

#include "../Levels/InGameMenuState.h"
#include "../Engine/Engine.h"
#include "Component/SoundComponent.h"
#include "Component/TextureComponent.h"
#include "Core/EventManager.h"
#include "EngineComponent.h"
#include "GameState.h"
#include "ECS/EntityComponentMover.h"
#include "GameStateTypes.h"

namespace q_engine
{
	class GameStateManager : public EngineComponent<GameStateManager>
	{
	private:
		using GameStateCont = std::map<GameStateType, std::shared_ptr<GameState>>;
		GameStateCont mRunningStates;
		std::vector<std::pair<GameStateType, std::shared_ptr<GameState>>> mAddingQue;
		std::vector<GameStateType> mDeletingQue;
		EntityComponentMover mEntityComponentMover;
		glm::vec2 mMapSize = { -1,-1 };
		glm::vec2 mUILayerSize;

		mutable bool mGameLocationDirty = true;
		mutable bool mUILocationDirty = true;

		GameStateType mUpdateStatus = GameStateType::COUNT;

		unsigned mPlayedLevel = 1;

		bool mIsGameEnded = false;
	public:
		GameStateManager() = default;

		void Load() override;
		void Update(double deltaTime) override;
		void RemoveLater();
		void RemoveFirstState() {
			if (mRunningStates.empty())
			{
				return;
			}
			RemoveState(mRunningStates.begin()->first);
		};
		void Unload() override;

		GameStateType GetUpdateStatus() const { return mUpdateStatus; }
		void SetMapSize(glm::vec2 size){mMapSize = size;}
		void SetUILayerSize(glm::vec2 size){mUILayerSize = size;}
        [[nodiscard]] glm::vec2 GetMapSize() const{return mMapSize;}
		[[nodiscard]] glm::vec2 GetUILayerSize() const{	return mUILayerSize;}

		[[nodiscard]] IDType GetPlayerID() const;
		//Don't call this function in normal situation
		void ClearAddingQue()
		{
			mAddingQue.clear();
		}
		void ClearDeletingQue()
		{
			mDeletingQue.clear();
		}
		void AddState(GameStateType type, std::shared_ptr<GameState> pState);
		void RemoveState(GameStateType type);
		bool HasState(GameStateType type)
		{
			return mRunningStates.find(type) != mRunningStates.end();
		};
		void RemoveState(std::string&& type_as_string);

		void AddState(IDType stateID);
		void ClearStates()
		{
			q_engine::Engine::GetEngineComponent<q_engine::EventManager>().ClearEvent();
			for(GameStateCont::iterator itr = mRunningStates.begin(); itr != mRunningStates.end(); ++itr)
			{
				mDeletingQue.emplace_back(itr->first);
			}
		}
		void ReloadState()
		{
			auto itr = mRunningStates.begin();
			auto end = mRunningStates.end();
			unsigned old_level = mPlayedLevel;
			while(itr != end)
			{
				AddState(itr->first, itr->second);
				mDeletingQue.emplace_back(itr->first);
				++itr;
			}
			mPlayedLevel = old_level;
		}

		unsigned GetNumPlayedLevel() const
		{
			return mPlayedLevel;
		}

		auto& GetAllStates()
		{
			return mRunningStates;
		}
		[[nodiscard]] std::shared_ptr<GameState> GetPlayState() const;
		[[nodiscard]] std::shared_ptr<GameState> GetUIState() const;
		[[nodiscard]] bool IsGameEnded() const { return mIsGameEnded; }

		void MoveEntityComponents(std::shared_ptr<GameState> from, std::shared_ptr<GameState> target);

		void MakeGameEnded()
		{
			mIsGameEnded = true;
		}
	};


}

inline void RegisterGameStateManager(sol::state& state)
{
	sol::usertype<q_engine::GameStateManager> gameStateManager = state.new_usertype<q_engine::GameStateManager>("GameStateManager");
	gameStateManager.set_function("ClearStates", &q_engine::GameStateManager::ClearStates);
	gameStateManager.set_function("AddState", sol::resolve<void(unsigned int)>(&q_engine::GameStateManager::AddState));
	gameStateManager.set_function("RemoveState", sol::resolve<void(std::string&&)>(&q_engine::GameStateManager::RemoveState));

	gameStateManager.set_function("GetPlayerID", &q_engine::GameStateManager::GetPlayerID);

	gameStateManager.set_function("MakeGameEnded", &q_engine::GameStateManager::MakeGameEnded);
	gameStateManager.set_function("RemoveFirstState", &q_engine::GameStateManager::RemoveFirstState);
}