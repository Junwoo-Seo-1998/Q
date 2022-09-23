/*
    File Name: MapGeneration.cpp
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "MapGeneration.h"
#include <algorithm>

#include "Levels/Level1State.h"


namespace q_engine
{
     int MapGeneration::times = 0;
     bool MapGeneration::tutorialFinished = false;
     bool MapGeneration::restart = false;
	
    MapGeneration::MapGeneration()
    {
    }

    void MapGeneration::Initialize()
    {
        presets.insert(std::make_pair(MapLevel::Tutorial, "resource/json/Howtoplay.json"));
        presets.insert(std::make_pair(MapLevel::Level1, "resource/json/Level1.json"));
        presets.insert(std::make_pair(MapLevel::Level2, "resource/json/Level2.json"));
        presets.insert(std::make_pair(MapLevel::Level3, "resource/json/Level3.json"));
        presets.insert(std::make_pair(MapLevel::Level4, "resource/json/Level4.json"));
        presets.insert(std::make_pair(MapLevel::Level5, "resource/json/Level5.json"));
        presets.insert(std::make_pair(MapLevel::Level6, "resource/json/Level6.json"));
        presets.insert(std::make_pair(MapLevel::Level7, "resource/json/Level7.json"));
        presets.insert(std::make_pair(MapLevel::BossMap, "resource/json/BossMap1.json"));
        presets.insert(std::make_pair(MapLevel::BossMap2, "resource/json/BossMap2.json"));

        maporder.resize(presets.size());
        deadtimes.resize(presets.size());
    }
    void MapGeneration::AddBossMap()
    {
        std::fill(maporder.begin(), maporder.end(), MapLevel::Start);
        maporder[static_cast<int>(MapLevel::Tutorial)] = MapLevel::Tutorial;
        maporder[static_cast<int>(MapLevel::BossMap)] = MapLevel::BossMap;
        maporder[static_cast<int>(MapLevel::BossMap2)] = MapLevel::BossMap2;

    }
    void MapGeneration::Generation()
    {
        AddBossMap();
        ++times;
        std::random_device rd;
        std::mt19937 generator(rd());
        std::uniform_int_distribution<int>dis(static_cast<int>(MapLevel::Level1), static_cast<int>(MapLevel::Level7));
        int size = static_cast<int>(MapLevel::BossMap);
        for (int i = static_cast<int>(MapLevel::Level1); i < size; i++)
        {
            int value = dis(generator);
            if (i > 0)
            {
                while (!dup_check(value))
                {
                    value = dis(generator);
                }
            }
            maporder[i] = static_cast<MapLevel>(value);
        }
    	
    }

    void MapGeneration::ReGeneration()
    {
        maporder.clear();
        maporder.resize(presets.size());
        Generation();
    }


    MapLevel& MapGeneration::GetMap(int index)
    {
        return maporder[index];
    }

    std::string& MapGeneration::GetFile(int num)
    {

        return presets.find(GetMap(num))->second;
    }

     int& MapGeneration::GetTimes()
     {
         return times;
     }

     bool MapGeneration::Empty()
    {
        return maporder.empty();
    }

    MapGeneration::~MapGeneration()
    {
        maporder.clear();
        presets.clear();
    }

    bool MapGeneration::dup_check(int num)
    {
		for(int i = static_cast<int>(MapLevel::Level1); i < static_cast<int>(MapLevel::BossMap);++i)
		{
			if(static_cast<int>(maporder[i]) == num)
			{
                return false;
			}
		}

        return true;
    }


}
