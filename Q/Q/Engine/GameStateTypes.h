/*
	File Name: GameStateManager.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Core/StringHash.h"

enum class GameStateType
{
	IN_GAME_MENU3, IN_GAME_MENU2, IN_GAME_MENU,  UI2, UI1, GAME, MAIN_MENU_BUTTON, OTHER_MENU, MAIN_MENU_BG, SPLASH_SCREEN, COUNT
};

inline std::string ConvertToString(GameStateType type)
{
	switch(type)
	{
	case GameStateType::IN_GAME_MENU3: 
	{
		return "IN_GAME_MENU3";
	}
	case GameStateType::IN_GAME_MENU2:
	{
		return "IN_GAME_MENU2";
	}
	case GameStateType::IN_GAME_MENU:
	    {
		return "IN_GAME_MENU";
	    }
	case GameStateType::UI2:
	    {
		return "UI2";
	    }
	case GameStateType::UI1:
	    {
		return "UI1";
		}
	case GameStateType::GAME:
	    {
		return "GAME";
	    }
	case GameStateType::MAIN_MENU_BUTTON:
	    {
		return "MAIN_MENU_BUTTON";
	    }
	case GameStateType::OTHER_MENU:
	    {
		return "OTHER_MENU";
		}
	case GameStateType::MAIN_MENU_BG:
	{
		return "MAIN_MENU_BG";
	}
	case GameStateType::SPLASH_SCREEN:
	{
		return "SPLASH_SCREEN";
	}
	default:
	    {
		throw "COUNT";
	    }
	}
}
inline GameStateType ConvertToType(const std::string name)
{
	const unsigned int nameID = q_engine::get_hash_id(name);

	static std::vector<unsigned> typeIDs(static_cast<size_t>(GameStateType::COUNT) + 1);
	static bool first_access = true;
	if(first_access == true)
	{
		for(int idx = 0; idx < static_cast<int>(GameStateType::COUNT); idx++)
		{
			typeIDs[idx] = q_engine::get_hash_id(ConvertToString(static_cast<GameStateType>(idx)));
		}
		first_access = false;
	}
	return static_cast<GameStateType>(std::distance(typeIDs.begin(), std::find(typeIDs.begin(), typeIDs.end(), nameID)));
}