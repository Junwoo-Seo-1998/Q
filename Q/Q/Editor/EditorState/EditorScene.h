/*
	File Name: EditorScene.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "Engine/GameState.h"
#include "Engine/Types.h"
class EditorScene : public q_engine::Level
{
public:
	void Load() override;
	void Update(double deltaTime) override;
	void Unload() override;

public:
	void ReLoadTileMap(q_engine::IDType baseMapEntt);
	void ReLoadAllTileMaps();
};