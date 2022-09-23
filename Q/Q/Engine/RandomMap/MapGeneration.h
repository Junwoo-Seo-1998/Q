#pragma once
#include <map>
#include <vector>
#include <random>

#include "../Engine/GameState.h"

/*
    File Name: MapGeneration.h
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/



namespace  q_engine
{
	class MapGeneration
	{
	public:
		MapGeneration();
		static void Initialize();
		static void Generation();
		static void ReGeneration();
		static MapLevel& GetMap(int index);
		static std::string& GetFile(int key);
		static int& GetTimes();
		static bool Empty();
		~MapGeneration();
		static bool dup_check(int num);
		static void AddBossMap();

		static bool tutorialFinished;
		static bool restart;
	private:

		inline static std::vector<MapLevel>maporder;
		inline static std::map<MapLevel, std::string>presets;
		static int times;
		inline static std::vector<int>deadtimes;
		
	};
}
