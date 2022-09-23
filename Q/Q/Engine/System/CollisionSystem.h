/*
	File Name: CollisionSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../GameState.h"
#include "../Component/CollisionComponent.h"
#include "../Core/Rect.h"
#include "glm/glm.hpp"
#include "../ECS/System.h"
#include "../Component/RaycastComponent.h"
#include "../Engine/Core/Quadtree.h"
namespace q_engine
{
	 auto getRect = [](IDType collisionID, GameState* state) {return state->GetComponents<StaticRectCollisionComponent>()[collisionID].GetColliderBox(); };
	
	class Circle;
	class CollisionSystem  : public System<CollisionSystem>
	{
		struct RayRectTile;
	
	public:
		
		CollisionSystem(GameState* state) : System<q_engine::CollisionSystem>(state)
		{
		
		};
		
		void Load() override;
		void Update(double dt) noexcept override;
		
		bool AABBCollisionCheck( Rect Aobject,Rect Bobject) const;
		bool CircleCircleCollisionCheck(CircleCollisionComponent& Acomponent, CircleCollisionComponent& Bcomponent);
		static bool PointRectCollisionCheck(glm::vec2 point, Rect BOBject) ;
		bool CircleRectCollisionCheck(const CircleCollisionComponent& Bcircle, const RectCollisionComponent& ARect);

		// this for gunObject and AI
		bool DoesIntersect(glm::vec2& a1, glm::vec2& a2, glm::vec2& b1, glm::vec2& b2) const;
		glm::vec2 GetIntersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2) const;
		void RayCastingUpdate(int threadNum);
		void RayCastingCheck(RaycastComponent& RayCastComponentA, RectCollisionComponent& componentB, GameState* CurrentState, int Rayindex, glm::vec2& directioninverse);
		void RayCastingCheck(RaycastComponent& RayCastComponentA, StaticRectCollisionComponent& componentB, GameState* CurrentState, int Rayindex, glm::vec2& directioninverse);
		void RayCastingCheck(RaycastComponent& RayCastComponentA, RayRectTile& componentB, GameState* CurrentState, int Rayindex, glm::vec2& directioninverse);
		void RectRectUpdate([[maybe_unused]]double dt);
		void CircleRectUpdate([[maybe_unused]] double dt);
		void CircleCircleUpdate([[maybe_unused]] double dt);
		void ObjectMovingCollisionCheck(RectCollisionComponent& componentA, RectCollisionComponent& componentB, GameState* gamestate);
		void ObjectMovingCollisionCheck(RectCollisionComponent& componentA, StaticRectCollisionComponent& componentB,
		                                GameState* gamestate);

		void QuadTreeUpdate([[maybe_unused]] double dt);
		// math function
		static float Crossproduct(const glm::vec2& a, const glm::vec2& b);
		static glm::vec2 SetVectorByAngle(const float& angle);
		static float GetAngle(const glm::vec2& vec);
		static float abs(const glm::vec2& vec);
		static glm::vec2 Normalize(const glm::vec2& vec);

		
		void Unload() override {};
		
		float GetCorrectValue() const;


		float DistanceSquare(const glm::vec2& Avalue, const glm::vec2& Bvalue);
		std::vector<IDType>& GetDeleteStaticCollisionData();
		
	private:
		Rect a{ {0.f,0.f},{2000.f,2000.f} };
		Quadtree<IDType, decltype(getRect)> mQuadtree = Quadtree<IDType, decltype(getRect)>( a, getRect);
		float mCorrectionValue = 1.1f;
		IDType RaycollisionTypeID = INVALID_ID;
		IDType RaycollisionEntityID = INVALID_ID;
		std::vector<IDType> mDeleteStaticCollisionData;
		struct RayRectTile
		{
			Rect mRect;
			float mwidth;
			float mheight;
			void SetColliderBox(glm::vec2 position, float width, float height)
			{
				mRect = { {position}, {position.x + width, position.y + height} };
				mwidth = width;
				mheight = height;
			}
			Rect GetColliderBox() const
			{
				return mRect;
			}
		};
		std::vector<RayRectTile> mRayRectTile;
	};
}