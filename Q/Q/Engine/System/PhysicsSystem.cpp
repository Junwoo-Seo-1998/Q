/*
	File Name: PhysicsSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "PhysicsSystem.h"

#include "../Engine/GameStateManager.h"
#include "../Engine/Component/CollisionComponent.h"

#include "../Component/GravityComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/VelocityComponent.h"
#include "../Component/WidthHeightComponent.h"
#include "../Component/OffsetComponent.h"
#include "../Engine/Profiler.h"
void q_engine::PhysicsSystem::Load()
{

}

void q_engine::PhysicsSystem::Update(double dt)
{
	SCOPE_PROFILING("PhysicsSystem::Update")
	const auto& currentState = m_controlled_state;
	auto& mVelocity_Container = currentState->GetComponents<VelocityComponent>();
	auto& mTransform_Container = currentState->GetComponents<TransformComponent>();
	auto& mGravity_Container = currentState->GetComponents<GravityComponent>();
	auto& mRectCollision_Container = currentState->GetComponents<RectCollisionComponent>();
	for(auto & gravity_component : mGravity_Container)
	{ 
		// Gravity has entity update 
		const auto  GetEntityID = gravity_component.GetEntityID();
		GravityComponent& GravityComp = mGravity_Container[GetEntityID];
		VelocityComponent& Velocitycomponent = mVelocity_Container[GetEntityID];
		if(GravityComp.GetIsOnground() == false)
		{
			Velocitycomponent.AddVelocity({ 0.f, -GravityComp.GetGravity() * dt });
		}

		  if (Velocitycomponent.GetVelocityX() != 0.f || Velocitycomponent.GetVelocityY() != 0.f)
			{
				GravityComp.SetIsOnGround(false);
			}
		
		
	}

	
	for(auto & velocity_component : mVelocity_Container)
	{
		auto GetEntityID = velocity_component.GetEntityID();
		auto &transform_component = mTransform_Container[GetEntityID];
		auto &Velocity_component = mVelocity_Container[GetEntityID];
		transform_component.AddPosition({ Velocity_component.GetVelocity().x * dt,Velocity_component.GetVelocity().y * dt });
		transform_component.AddRotationRadian(Velocity_component.GetAngularVelocity() * dt);
	}
	

	for (auto& collisionBox : mRectCollision_Container)
	{
		auto& collisoncomp = mRectCollision_Container[collisionBox.GetEntityID()];
		auto& transformcomp = currentState->GetComponents<TransformComponent>()[collisoncomp.GetEntityID()];
		auto& Widthheightcomp = currentState->GetComponents<WidthHeightComponent>()[collisoncomp.GetEntityID()];
		float widthcomp = Widthheightcomp.GetWidth<float>();
		float heightcomp = Widthheightcomp.GetHeight<float>();
		collisionBox.SetColliderBoxSize(widthcomp, heightcomp);
		collisionBox.UpdateOldPosition({ collisoncomp.GetColliderBox().Left(), collisoncomp.GetColliderBox().Bottom() });
		collisionBox.UpdatePosition(transformcomp.GetPosition());

	}



	
}
