/*
	File Name: AStarPathFinding.cpp
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "AStarPathFinding.h"

#include <iostream>
#include <windows.h>

#define ACTIVE_PATH_DEBUG false;

constexpr int Diagonal_Cost = 14;
constexpr int RectAngular_Cost = 10;

void gotoxy(int x, int y)

{
	COORD pos = { static_cast<short>(x),static_cast<short>(y) };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

namespace q_engine
{
	int AStarPathFinding::ComputeTTEScore(vec2Int from, vec2Int to)
	{
		int temp = (std::abs(to.x - from.x) + std::abs(to.y - from.y)) * RectAngular_Cost;
		return temp;
	}

	void AStarPathFinding::DrawPath(Level* level, std::list<glm::vec2>& pathList, vec2Int start)
	{
		auto MapSize = level->GetMapData().mTileSize;
		auto MapNum = level->GetMapData().mTileNum;
		pathList.clear();
		PathNode& node = mClosedList.back();
		while (node.mPosition != start)
		{
#if ACTIVE_PATH_DEBUG
			gotoxy(node.mPosition.x, MapNum.y - node.mPosition.y - 1);
			std::cout << '.';
#endif
			pathList.push_front(glm::vec2(node.mPosition.x * MapSize.x, (node.mPosition.y + 1) * MapSize.y));
			node = *node.mParentNode;
		}
	}

	void AStarPathFinding::UpdatePath(Level* level, std::list<glm::vec2>& pathList, glm::vec2 from, glm::vec2 to)
	{
		mOpenList.clear();
		mClosedList.clear();

		
		vec2Int MapSize = level->GetMapData().mTileNum;
		vec2Int start = level->GetTileIndexByCoord(from);
		vec2Int end = level->GetTileIndexByCoord(to);
		if (level->GetTileByIndex(end) != TileType::Null)
			return;

		int startTTE = ComputeTTEScore(start, end);
		mOpenList.emplace_back( start, 0, startTTE, startTTE, mOpenList.end() );


#if ACTIVE_PATH_DEBUG
		gotoxy(0, 0);
		for (int i = MapSize.y - 1; i >= 0; --i)
		{
			for (int j = 0; j < MapSize.x; ++j)
			{
				std::cout << static_cast<int>(level->GetMapData().mTiles[MapSize.x * i + j]);
			}
			std::cout << std::endl;
		}
		gotoxy(start.x, MapSize.y - start.y - 1);
		std::cout << 's';

		gotoxy(end.x, MapSize.y - end.y - 1);
		std::cout << 'e';
#endif
		
		while(!mOpenList.empty())
		{
			auto idx = std::min_element(mOpenList.begin(),mOpenList.end());
			auto check = *idx;
			mOpenList.erase(idx);
			mClosedList.push_back(check);
			auto parentNode = (--mClosedList.end());
			
		
#if ACTIVE_PATH_DEBUG
			gotoxy(check.mPosition.x, MapSize.y - check.mPosition.y - 1);
			std::cout << 'x';
#endif
			
			if(check.mPosition == end)
			{
				DrawPath(level, pathList, start);
				return;
			}

			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					if (i == 0 && j == 0)
						continue;

					vec2Int pos = check.mPosition + vec2Int{i, j};

					if (pos.x < 0 || MapSize.x <= pos.x ||
						pos.y < 0 || MapSize.y <= pos.y)
						continue;
					
					if (level->GetTileByIndex(pos) != TileType::Null)
						continue;
					
					if (std::find_if(mClosedList.begin(), mClosedList.end(), [check , &pos](const PathNode& node)
						{
							return node.mPosition == pos;
						}) != mClosedList.end())
					{
						continue;
					}

					int TTEScore = ComputeTTEScore(check.mPosition, end);
					int TFSScore;
					if (i == 0 || j == 0)
						TFSScore = check.mTimeFromStart + RectAngular_Cost;
					else
						TFSScore = check.mTimeFromStart + Diagonal_Cost;


					
					auto overlapNode = std::find_if(mOpenList.begin(), mOpenList.end(),
						[&pos, &TFSScore](const PathNode& node)
						{
							return node.mPosition == pos;
						});
					if (overlapNode != mOpenList.end())
					{
						if(overlapNode->mTimeFromStart > TFSScore)
						{
							mOpenList.erase(overlapNode);
							mOpenList.emplace_back(PathNode(pos, TFSScore, TTEScore, TFSScore + TTEScore, parentNode));
						}
					}
					else
					{
						mOpenList.emplace_back(PathNode(pos, TFSScore, TTEScore, TFSScore + TTEScore, parentNode));
					}

#if ACTIVE_PATH_DEBUG
					gotoxy(pos.x, MapSize.y - pos.y - 1);
					std::cout << 'p';
#endif
				}
			}

		}
	}
	bool PathNode::operator==(const PathNode& node) const
	{
		return mPosition == node.mPosition;
	}

	bool PathNode::operator>(const PathNode& node) const
	{
		return mScore > node.mScore;
	}

	bool PathNode::operator<(const PathNode& node) const
	{
		return mScore < node.mScore;
	}
}

