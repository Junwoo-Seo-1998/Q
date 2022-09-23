/*
	File Name: TileMapComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TileMapComponent.h"

namespace q_engine
{
	TileMapComponent::TileMapComponent(const std::string& filename, const glm::vec2& offset)
		:mFileName(filename), mOffset(offset)
	{
		
	}

	void TileMapComponent::AddTile(IDType tileID)
	{
		mTiles.push(tileID);
	}

	std::queue<IDType>& TileMapComponent::GetTiles()
	{
		return mTiles;
	}

	std::string& TileMapComponent::GetFileName()
	{
		return mFileName;
	}

	glm::vec2& TileMapComponent::GetOffset()
	{
		return mOffset;
	}

	Json::Value TileMapComponent::Serialize()
	{
		Json::Value value;

		value["FileName"] = mFileName;
		value["Offset"].append(mOffset.x);
		value["Offset"].append(mOffset.y);
		
		return value;
	}

	void TileMapComponent::Deserialize(Json::Value& jsonValue)
	{
		mFileName = jsonValue["FileName"].asString();
		
		mOffset.x = jsonValue["Offset"][0].asFloat();
		mOffset.y = jsonValue["Offset"][1].asFloat();
	}
}

