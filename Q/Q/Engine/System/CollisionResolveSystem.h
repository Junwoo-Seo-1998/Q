/*
	File Name: CollisionResolveSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../GameState.h"

#include "../ECS/System.h"
#include "../Component/RaycastComponent.h"

namespace q_engine
{
	class RectCollisionComponent;
	class StaticRectCollisionComponent;
	class CollisionResolveSystem final : public System<CollisionResolveSystem>
	{

	public:
		CollisionResolveSystem(GameState* state) : System<q_engine::CollisionResolveSystem>(state) {};
		void Load() override;
		void Update(double dt) noexcept override;
		void Unload() override;
		void RectResolveUpdate(RectCollisionComponent& componentA, RectCollisionComponent& componentB);
		void RectStaticResolveUpdate(RectCollisionComponent& componentA, StaticRectCollisionComponent& componentB);
		void RayResolveUpdate(int RayownerTypeID ,int RayownerEntityID ,int EntityTypeID, int EntityID);
		void RayResolveUpdate(RaycastComponent& componentA, int RayIndex);
		float GetCorrectValue() const;
	private:
		float mCorrectionValue = 1.1f;
		
	};
	
}