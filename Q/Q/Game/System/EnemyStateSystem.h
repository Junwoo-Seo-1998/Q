/*
	File Name: EnemyStateSystem.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include "../../Engine/Component/ObjectStateComponent.h"
#include "../Component/EnemyStateComponent.h"
#include "../Engine/ECS/System.h"
#include "Engine/Graphics/DrawObject.h"


namespace q_engine
{
	class VelocityComponent;
}

struct Rect;

using namespace q_engine;
class EnemyStateSystem final : public q_engine::System<EnemyStateSystem>
{
	using IDType = int;

	void ThreadedUpdate(double deltaTime, int threadNum, std::vector<IDType>& deadEnemies);
	void DropItem(IDType enemyID);
public:
	q_engine::DrawObject mDrawing = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID,
		{255, 100, 255, 255}};

	EnemyStateSystem(q_engine::GameState* state) : System<EnemyStateSystem>(state) {};

	void EnemyInputCheck(EnemyStateComponent& enemyState);

	void Update(double deltaTime) override;
	
	void OnEvent(q_engine::IEvent* event) override;
};
