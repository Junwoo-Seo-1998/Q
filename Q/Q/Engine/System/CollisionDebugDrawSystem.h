/*
	File Name: CollisionDebugDrawSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.2
*/
#pragma once
#include "../Engine/ECS/System.h"
#include "../Graphics/DrawObject.h"
#include "../Engine/Core/StringHash.h"

namespace  q_engine
{
	class CollisionDebugDrawSystem : public System<CollisionDebugDrawSystem>
	{
	public:
		CollisionDebugDrawSystem() = delete;
		CollisionDebugDrawSystem(GameState* state) : System<q_engine::CollisionDebugDrawSystem>(state) {};
		void Load() override;
		void Unload() override;
		void Update(double deltaTime) override;
		void DrawCollideBox();
		void DrawRayPoints();
		void DrawCollideCircle();
		void OnOffCollisionBox();

	private:
		bool mDrawCollisionBox = false;
		DrawObject mDrawing = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {255, 0, 0, 255}, ZDepth(3.f) };
		DrawObject mDrawingPlayer = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 255, 0, 255}, ZDepth(3.f) };
		DrawObject mDrawingEnemy = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {255, 255, 0, 255}, ZDepth(3.f) };
	
		DrawObject DrawingRay = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 255,255 , 255} };
		DrawObject DrawingRayUpdate = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 0,0 , 255} };
		DrawObject DrawingJumpTrigger = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 255,255 , 255} };
		DrawObject DrawingPortalTrigger = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 0,0 , 255} };
		DrawObject DrawingEnemyWeaponTrigger = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {186, 125,0 , 255} };
		DrawObject DrawingBulletTrigger = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {255, 125,185 , 255} };
		DrawObject DrawingPlayerBullet = { "HollowSquareMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 255,128 , 255} };
	};
}
