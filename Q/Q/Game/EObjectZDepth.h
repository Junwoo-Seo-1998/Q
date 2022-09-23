/*
    File Name: EObjectZDepth.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary: Joonho Hwang
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

//Lower number draw above
enum class EObjectZDepth
{
    TEXT = -10000,

    IN_GAME_MENU_START = -5000,
    IN_GAME_MENU_TEXT,
    IN_GAME_MENU_BOX,
    IN_GAME_MENU_BG,
    IN_GAME_MENU_END = -4500,

    IN_GAME_UI_START,
    IN_GAME_UI_TEXT,
    IN_GAME_UI_FIRST_OBJ,
    IN_GAME_UI_SECOND_OBJ,
    IN_GAME_UI_THIRD_OBJ,
    IN_GAME_UI_FOURTH_OBJ,
    IN_GAME_UI_BG,
    IN_GAME_UI_END = -4000,

    IN_GAME_PLAY_START,
    IN_GAME_PLAY_EFFECT,
	ITEM,
    WEAPON,
    IN_GAME_PLAY_PLAYER_OBJ,
    IN_GAME_PLAY_PLAYER,

    IN_GAME_PLAY_ENEMY_OBJ,
    IN_GAME_PLAY_ENEMY,
    IN_GAME_PLAY_MAP_2,
    IN_GAME_PLAY_MAP_1,
    IN_GAME_PLAY_CANISTER,
    IN_GAME_PLAY_BG,
    IN_GAME_PLAY_END = -3500,

    MAIN_MENU_START,
    MAIN_MENU_TEXT,
    MAIN_MENU_FIRST_OBJ,
    MAIN_MENU_SECOND_OBJ,
    MAIN_MENU_BG,
    MAIN_MENU_END = -3000,
};
