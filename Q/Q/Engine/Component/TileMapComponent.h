/*
    File Name: TileMapComponent.h
    Project Name: Q
    Author(s):
        Primary: Junwoo Seo
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <queue>

#include "../ECS/Component.h"

#include "../Core/StringHash.h"
#include "../Graphics/DrawObject.h"
#include <vector>
//only for editor.
namespace q_engine
{
    class TileMapComponent final : public Component<TileMapComponent>
    {
    public:
        TileMapComponent() = default;
        TileMapComponent(const std::string& filename, const glm::vec2& offset);
        void AddTile(IDType tileID);
        std::queue<IDType>& GetTiles();
        std::string& GetFileName();
        glm::vec2& GetOffset();

        virtual Json::Value Serialize() override;
        virtual void Deserialize(Json::Value& jsonValue) override;
    private:
        glm::vec2 mOffset;
        std::string mFileName;
        std::queue<IDType> mTiles;

    friend class Loader;
    };
}