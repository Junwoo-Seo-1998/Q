/*
    File Name: MapEntity.h
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../../Engine/Core/Rect.h"
#include "../Engine/ECS/Entity.h"

namespace q_engine {
    // Stores common tile data.
    struct TileInfo
    {
        TileInfo() : tileID(INVALID_ID)
        {
        }

        TileInfo(size_t textureID, unsigned int tileID, Rect textureRect,std::string name)
            : textureID(textureID), tileID(tileID), textureRect(textureRect),layername(name) { }
    	
        TileInfo(std::string name) : tileID(INVALID_ID),layername(name)
        {
        }
        unsigned int tileID;
        size_t textureID = 0;
        //texture position
        Rect textureRect = { {0,0},{0,0 } };
        std::string layername;
        /*glm::vec2 property_value;*/
    };

    struct Tile
    {

        std::shared_ptr<TileInfo> properties;
        float x = 0;
        float y = 0;
        unsigned int tileId = 0;
        /*bool visible = true;*/
        
    };

	class MapEntity : public Entity<MapEntity>
	{
		
	};
	class BackGroundEntity : public Entity<BackGroundEntity>
	{
		
	};
	class PortalEntity : public Entity<PortalEntity>
	{
		
	};
	class TestEntity : public Entity<TestEntity>
	{
		
	};
	class DamageTileEntity : public Entity<DamageTileEntity>
	{
		
	};
}
