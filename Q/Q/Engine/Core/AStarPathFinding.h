/*
	File Name: AStarPathFinding.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <list>
#include <glm/glm.hpp>
#include "../GameState.h"
#include "Engine/EngineComponent.h"

namespace q_engine
{
	struct PathNode
	{
		vec2Int mPosition;
		int mTimeFromStart;
		int mTimeToEnd;
		int mScore;
		std::list<PathNode>::iterator mParentNode;

		PathNode(vec2Int position, int timeFromStart, int timeToEnd, int score, std::list<PathNode>::iterator parentNode) :mPosition(position),
		mTimeFromStart(timeFromStart), mTimeToEnd(timeToEnd), mScore(score), mParentNode(parentNode) {}
		
		bool operator==(const PathNode& node) const;
		bool operator>(const PathNode& node) const;
		bool operator<(const PathNode& node) const;
	};

	
	class AStarPathFinding : public EngineComponent<AStarPathFinding>
	{
		std::list<PathNode> mOpenList;
		std::list<PathNode> mClosedList;
		
		int ComputeTTEScore(vec2Int from, vec2Int to);
		void DrawPath(Level* level, std::list<glm::vec2>& pathList, vec2Int start);
	public:
		void UpdatePath(Level* level, std::list<glm::vec2>& pathList, glm::vec2 from, glm::vec2 to);
	};
}
