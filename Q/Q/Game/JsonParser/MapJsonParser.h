#pragma once
/*
    File Name: MapJsonParser.h
    Project Name: Q
    Author(s):
        Primary: Sieun Choi
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include <glm/vec2.hpp>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>
#include "../Entity/MapEntity.h"
#include <jsoncpp/json.h>


namespace q_engine {
	
    struct TileSheetData
    {
        size_t textureId = 0;
        glm::vec2 imageSize = {0.f,0.f};
        int columns  = 0;
        int rows = 0;
        std::string imagepath;
        glm::vec2 tileSize = {0.f,0.f};
        int margin = 0;
        int spacing = 0;
    };
	struct TileObjectData
	{
        TileObjectData(glm::vec2 position,Rect rect, glm::vec2 property_value,bool Trigger,glm::vec2 size)
            : textureRect(rect), pos(position),properties(property_value),on(Trigger),objectSize(size) { }
        TileObjectData(glm::vec2 position, Rect rect,glm::vec2 size)
            : textureRect(rect), pos(position),objectSize(size) { }
		
        Rect textureRect = { {0.f,0.f},{0.f,0.f } };
        glm::vec2 pos = { 0.f,0.f };
        glm::vec2 properties = { 0.f,0.f };
        bool on = false;
        glm::vec2 objectSize = { 0.f,0.f };
		
	};
using Layer = std::vector<std::shared_ptr<Tile>>;

// Stores layer names with layer.
using MapTiles =
std::map<std::string, std::shared_ptr<Layer>>;

// Stores the different tile types that can be used.
using TileSet =
std::unordered_map<unsigned int, std::shared_ptr<TileInfo>>;

    class TileMapParser
    {
    public:
        TileMapParser() = default;

        Layer Parse(const std::string& file, glm::vec2 offset);

        [[nodiscard]] const glm::vec2& getImageSize() const;
        [[nodiscard]] const int getTileCount() const;
        [[nodiscard]] const glm::vec2 getTileSize() const;
        [[nodiscard]] const Layer& GetEntireTileInfo()const;
        [[nodiscard]] const TileSheetData& getTileSheetData()const;
        [[nodiscard]]  std::vector<int>& getTileIds();
        [[nodiscard]] const TileSet& getTileset() const;
        [[nodiscard]] const float getTileScale() const;
    	[[nodiscard]] std::map<std::string, std::vector<std::shared_ptr<TileObjectData>>>& GetObjectlayersInfo();
        [[nodiscard]] const glm::vec2& GetStartPos();
        [[nodiscard]] const glm::vec2& GetOffset();
        [[nodiscard]] const glm::vec2 GetMapSize();
        [[nodiscard]] const glm::vec<2, int>& GetMapTileNum();
    	
    private:
    	
        std::shared_ptr<TileSheetData> BuildTileSheetData(Json::Value rootNode);
        std::shared_ptr<MapTiles> BuildMapTiles(Json::Value rootNode);
        std::pair<std::string, std::shared_ptr<Layer>>
            BuildLayer(Json::ValueIterator node, std::shared_ptr<TileSheetData> tileSheetData
            );
        std::shared_ptr<TileObjectData> MakeObjectLayer(Json::ValueIterator node, std::shared_ptr<TileSheetData> tileSheetData);
    	
        TileSheetData tileSheetData;
        Layer layer;
        TileSet tileset;
        glm::vec2 offset = { 0.f,0.f };
        std::vector<int>tileIds;
        const float tileScale = 3.;
        int tileCount = 0;
        glm::vec2 tileSize = {0.f,0.f};
        glm::vec2 StartPosition = { 0.f,0.f };
        glm::vec<2, int>MapSize = { 0,0 };
        glm::vec<2,int> MapTileNum = { 0,0 };
        std::map<std::string, std::vector<std::shared_ptr<TileObjectData>>>ObjectLayers;
    	
    };




}