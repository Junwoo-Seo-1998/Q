/*
	File Name: ScriptManger.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "sol/sol.hpp"
namespace q_engine
{
	class GameState;

	class ScriptManager
	{
	public:
		ScriptManager();
		
		static ScriptManager& Get();

		void Init();
		sol::state& GetState();
		void SetCurrentScene(GameState* scene);
	private:
		void RegisterUtils(sol::state& state);
		void RegisterScene(sol::state& state);
		void RegisterInput(sol::state& state);
		void RegisterEntity(sol::state& state);
		void RegisterTypeID(sol::state& state);
		void RegisterCreateObject(sol::state& state);
		sol::state m_State;
		GameState* m_CurrentScene;
	};
}
