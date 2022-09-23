/*
    File Name: MapJsonParser.cpp
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "MapJsonParser.h"
#include <fstream>
#include <glm/vec2.hpp>
#include <iostream>
#include "../../Engine/Core/StringHash.h"
#include "../../Engine/Engine.h"
#include "../../Engine/GameStateManager.h"


namespace q_engine {


 
	Layer TileMapParser::Parse(const std::string& file, glm::vec2 offset)
    {
 
        Json::Value root;
        std::ifstream input_stream;
        input_stream.open(file);
        input_stream >> root;

		if(!input_stream.is_open())
		{
            std::cerr << "Cannot find the json file" << std::endl;
		}

        tileSize.x = root["tilewidth"].asFloat();
        tileSize.y= root["tileheight"].asFloat();
        MapTileNum.x = root["width"].asInt();
        MapTileNum.y = root["height"].asInt();
        MapSize.x = MapTileNum.x * tileSize.x;
        MapSize.y = MapTileNum.y * tileSize.y;


        this->offset = offset;
		
    	//Layer Name with layer  
        std::shared_ptr<MapTiles> tiles = BuildMapTiles(root);
    	

        std::vector<std::shared_ptr<Tile>> tileObjects;

    	//Each layer 
        for (const auto& layer : *tiles)
        {
            for (const auto& tile : *layer.second)
            {
            	
                std::shared_ptr<TileInfo>tileInfo = tile->properties;

                std::shared_ptr<Tile>tileObject = std::make_shared<Tile>();


                //position of the tile also transform for set position -> on the test level
                tileObject->x = tile->x * tileSize.x * tileScale + offset.x;
                tileObject->y = (MapTileNum.y - tile->y) * tileSize.y * tileScale + offset.y;

                tileObject->tileId = tile->tileId;
                tileObject->properties = tile->properties;

                if ( tileInfo->layername != "StartTile")
                {
                    tileObjects.emplace_back(tileObject);
                }
                if (tileInfo->layername == "StartTile")
                {
                    StartPosition = { tileObject->x  , tileObject->y };
                }
               
            }
        }
#ifndef QEDITOR
        Engine::GetEngineComponent<GameStateManager>().SetMapSize(GetMapSize());
#endif
        return tileObjects;
    }




	//Tile Sets Parser
    std::shared_ptr<TileSheetData> TileMapParser::BuildTileSheetData(Json::Value root)
    {
        //TileSheetData tileSheetData;

    	//TileSheet Info
        Json::Value tileSets = root["tilesets"];
        Json::ValueIterator it = tileSets.begin();
        Json::ValueIterator endit = tileSets.end();

        std::string info;
        while (it != endit)
        {
            //Testing the iterator
            tileSheetData.columns = (*it)["columns"].asInt();
            int firstid = (*it)["firstgid"].asInt();
            tileSheetData.tileSize.x = (*it)["tilewidth"].asFloat();
            tileSheetData.tileSize.y = (*it)["tileheight"].asFloat();
            tileCount = (*it)["tilecount"].asInt();
            tileSheetData.rows = tileCount / tileSheetData.columns;
            tileSheetData.margin = (*it)["margin"].asInt();
            tileSheetData.spacing = (*it)["spacing"].asInt();
            tileSheetData.imageSize.x = 496;
            tileSheetData.imageSize.y = 496;
            tileSheetData.textureId = get_hash_id((*it)["image"].asString());
            tileSheetData.imagepath = (*it)["image"].asString();

            ++it;

        }   
        return std::make_shared<TileSheetData>(tileSheetData);
    }

    std::shared_ptr<MapTiles> TileMapParser::BuildMapTiles(Json::Value rootNode)
    {
        std::shared_ptr<TileSheetData>tileSheetData = BuildTileSheetData(rootNode);

        std::shared_ptr<MapTiles> map = std::make_shared<MapTiles>();

        Json::Value layer_node = rootNode["layers"];
    	
        for (Json::ValueIterator iter = layer_node.begin(); iter != layer_node.end(); ++iter)
        {
        	//Now flow of the layer is platform layer -> collision layer
  
            std::pair<std::string, std::shared_ptr<Layer>> mapLayer = BuildLayer(iter, tileSheetData);
            map->emplace(mapLayer);
        }

        return map;
    }
	
    //Layer Parsing
    std::pair<std::string, std::shared_ptr<Layer>> TileMapParser::BuildLayer(Json::ValueIterator layerNode, std::shared_ptr<TileSheetData> tileSheetData)
    {
        
        /*TileSet tileSet;*/
        std::shared_ptr<Layer> layer = std::make_shared<Layer>();

        Json::ValueIterator iter = layerNode;

        const std::string layerName = (*iter)["name"].asString();
        glm::vec2 properties = { 0.f,0.f };

		//Make the ObjectLayer
		if((*iter)["draworder"].asString() == "topdown")
		{
            Json::Value ObjectsNode = (*iter)["objects"];
            std::vector<std::shared_ptr<TileObjectData>>TileObjects;
            for (Json::ValueIterator object_iter = ObjectsNode.begin(); object_iter != ObjectsNode.end(); ++object_iter)
            {
                auto objects_info = MakeObjectLayer(object_iter, tileSheetData);
                TileObjects.push_back(objects_info);
            }
            ObjectLayers.insert(std::make_pair(layerName, TileObjects));
			
            return std::make_pair(layerName, layer);
		}
		
        Json::Value dataNode = (*iter)["data"];
        
        auto mapIndices = dataNode.begin();

        int count = 0;
		
    	
        while (mapIndices != dataNode.end())
        {

            int tileId = mapIndices->asInt();
            std::shared_ptr<Tile> tile = std::make_shared<Tile>();
        	
            if (tileId != 0)
            {
                auto itr = tileset.find(tileId);
                if (itr == tileset.end())
                {
                    int textureX = tileId % tileSheetData->columns - 1;
                    int textureY = (tileId / tileSheetData->columns);
                    Rect rect;

                    if (textureX == 0)
                    {
                        float Y = getImageSize().y - (static_cast<float>(textureY) + 1  * tileSize.y);
                         rect = { {textureX * tileSize.x,Y  },{ tileSheetData->tileSize.x, tileSheetData->tileSize.y} };
                    }
                    else
                    {
                        float X = static_cast<float>(textureX * tileSize.x + textureX * tileSheetData->spacing);
                        float Y = getImageSize().y - (static_cast<float>((textureY + 1) * tileSize.y) + (textureY * tileSheetData->spacing));
                         rect = { {X, Y },{ tileSheetData->tileSize.x, tileSheetData->tileSize.y} };
                       
                    }
                    std::shared_ptr<TileInfo> tileInfo = std::make_shared<TileInfo>(tileSheetData->textureId, tileId, rect, layerName);
                    tileIds.push_back(tileId);
                	
                    itr = tileset.insert(std::make_pair(tileId, tileInfo)).first;

                    
                }
               
                // Bind properties of a tile from a set.
                tile->properties = itr->second;
                tile->x = static_cast<float>(count % MapTileNum.x - 1);
                tile->y = static_cast<float>(count / MapTileNum.x);
                tile->tileId = tileId;
                layer->emplace_back(tile);

            }

            count++;
            ++mapIndices;
        }
        
        return std::make_pair(layerName, layer);
    }


    std::shared_ptr<TileObjectData> TileMapParser::MakeObjectLayer(Json::ValueIterator objectnode, std::shared_ptr<TileSheetData> tileSheetData)
	{
       
        glm::vec2 object_pos;
        const glm::vec2 objectSize = { (*objectnode)["width"].asFloat() ,(*objectnode)["height"].asFloat() };
        glm::vec2 p_value;
        bool Trigger = false;
        object_pos.x = (*objectnode)["x"].asFloat() * tileScale + offset.x - (objectSize.x * tileScale);
        object_pos.y = ((MapTileNum.y * objectSize.y - (*objectnode)["y"].asFloat()) * tileScale + offset.y + (objectSize.y * tileScale));
		
        const int tileId = (*objectnode)["gid"].asInt();

        const int textureX = tileId % tileSheetData->columns - 1;
        const int textureY = tileId / tileSheetData->columns;

        Rect rect = { {textureX * objectSize.x, textureY * objectSize.y},
        	{ objectSize.x, objectSize.y} };

       
        Json::Value PropertyNode = (*objectnode)["properties"];
		
        for (Json::ValueIterator property_iter = PropertyNode.begin(); property_iter != PropertyNode.end(); ++property_iter)
        {
            if ((*property_iter)["name"].asString() == "Trigger")
            {
                Trigger = (*property_iter)["value"].asBool();
                ++property_iter;
                p_value.x = (*property_iter)["value"].asFloat();
                ++property_iter;
                p_value.y = (*property_iter)["value"].asFloat();
            }
        }
        
        return std::make_shared<TileObjectData>(object_pos, rect, p_value,Trigger,objectSize);
	}

    [[nodiscard]] const float TileMapParser::getTileScale() const
    {
        return tileScale;
    }

	std::map<std::string, std::vector<std::shared_ptr<TileObjectData>>>& TileMapParser::GetObjectlayersInfo()
	{
        return ObjectLayers;
	}

	const glm::vec2& TileMapParser::GetStartPos()
    {
        return StartPosition;
    }

	const glm::vec2& TileMapParser::GetOffset()
	{
        return offset;
	}
	
    const glm::vec2 TileMapParser::GetMapSize()
    {
        return { MapSize.x* tileScale, MapSize.y * tileScale };
    }

	const glm::vec<2,int>& TileMapParser::GetMapTileNum()
	{
        return MapTileNum;
	}

	[[nodiscard]] const TileSheetData& TileMapParser::getTileSheetData()const
    {
        return tileSheetData;
    }

    std::vector<int>& TileMapParser::getTileIds()
    {
        return tileIds;
    };
	const TileSet& TileMapParser::getTileset() const
	{
        return tileset;
	}

	const int TileMapParser::getTileCount() const
	{
        return tileCount;
	}
	const glm::vec2 TileMapParser::getTileSize() const
	{
        return tileSize;
	}
	const glm::vec2& TileMapParser::getImageSize() const
    {
        return tileSheetData.imageSize;
    }

	 
}

