/*
	File Name: CollisionDebugDrawSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.2
*/
#include "CollisionDebugDrawSystem.h"
#include "../Graphics/DrawObject.h"
#include "../Engine/Core/StringHash.h"

#include "CollisionSystem.h"
#include "../../Game/Entity/PhysicsTestEntities.h"
#include "../Engine.h"
#include "../GameStateManager.h"
#include "../../Game/Entity/MapEntity.h"
#include "../../Game/Entity/WeaponEntity.h"
#include "../Component/CollisionComponent.h"

#include "../Component/RadiusComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/WidthHeightComponent.h"
#include "../ECS/InputManager.h"
#include "../Entity/CameraEntity.h"
#include "../../Game/Entity/JumpTriggerEntity.h"
#include "Entity/BulletEntity.h"
#include "Entity/ItemEntity.h"
#include "Entity/ProjectileEntity.h"

void q_engine::CollisionDebugDrawSystem::Load()
{
	Binding* bind = (new Binding("DebugCollisionBox"));
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::N));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("DebugCollisionBox", &CollisionDebugDrawSystem::OnOffCollisionBox, this);
}

void q_engine::CollisionDebugDrawSystem::Unload()
{
	Engine::GetEngineComponent<InputManager>().RemoveBinding("DebugCollisionBox");
	Engine::GetEngineComponent<InputManager>().RemoveCallback("DebugCollisionBox");
}

void q_engine::CollisionDebugDrawSystem::Update([[maybe_unused]] double deltaTime)
{
#ifdef _DEBUG
	if (mDrawCollisionBox == true)
	{
		DrawCollideBox();
		DrawCollideCircle();
		DrawRayPoints();
	}
#endif
	

	
}

void q_engine::CollisionDebugDrawSystem::DrawCollideBox()
{
    const auto& CurrentLevel = m_controlled_state;
	auto& transformComponents = CurrentLevel->GetComponents<TransformComponent>();
	auto& WidthHeightComponents = CurrentLevel->GetComponents<WidthHeightComponent>();
	auto& Container = CurrentLevel->GetComponents<RectCollisionComponent>();
	auto& StaticContainer = CurrentLevel->GetComponents<StaticRectCollisionComponent>();
	const TransformComponent& camTransform = transformComponents[CurrentLevel->GetEntities<CameraEntity>().Front().GetEntityID()];

	const glm::mat3 world2NDC = CameraEntity::CalculateWorldToNDCTransform(camTransform.GetPosition(), camTransform.GetRotationRadian(), camTransform.GetScale());
	for (auto & collisionbox : StaticContainer)
	{
		auto& transformcomp = transformComponents[collisionbox.GetEntityID()];
		auto& widthheightcomp = WidthHeightComponents[collisionbox.GetEntityID()];
		glm::mat3 WidthHeightMatrix = { widthheightcomp.GetWidth<float>(),0,0,0,widthheightcomp.GetHeight<float>(), 0,0,0,1 };
		glm::mat3 CollisionBoxMatrix = { 1,0,0,0,1,0,transformcomp.GetPositionX(),transformcomp.GetPositionY(),1 };
		
		mDrawing.Draw(world2NDC * CollisionBoxMatrix * WidthHeightMatrix);
		
		
	}
	for (auto& collisionbox : Container)
	{
		auto& transformcomp = transformComponents[collisionbox.GetEntityID()];
		auto& widthheightcomp = WidthHeightComponents[collisionbox.GetEntityID()];
		auto GettypeID = collisionbox.GetEntityTypeID();
		glm::mat3 WidthHeightMatrix = { widthheightcomp.GetWidth<float>(),0,0,0,widthheightcomp.GetHeight<float>(), 0,0,0,1 };
		glm::mat3 CollisionBoxMatrix = { 1,0,0,0,1,0,transformcomp.GetPositionX(),transformcomp.GetPositionY(),1 };
		
		if (GettypeID == q_engine::GetTypeID<GroundEntity>() || GettypeID == q_engine::GetTypeID<MapEntity>() || GettypeID == q_engine::GetTypeID<TestEntity>())
		{
			mDrawing.Draw(world2NDC * CollisionBoxMatrix * WidthHeightMatrix );
		}
		else if(GettypeID == q_engine::GetTypeID<PlayerEntity>())
		{
			mDrawingPlayer.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix});
		}
		else if (GettypeID == q_engine::GetTypeID<WeaponEntity>())
		{
			mDrawingPlayer.Draw({ world2NDC  * CollisionBoxMatrix * WidthHeightMatrix  });
		}
		else if(GettypeID == q_engine::GetTypeID<EnemyEntity>())
		{
			mDrawingEnemy.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		else if (GettypeID == q_engine::GetTypeID<JumpTriggerEntity>())
		{
			DrawingJumpTrigger.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		else if (GettypeID == q_engine::GetTypeID<PortalEntity>())
		{
			DrawingPortalTrigger.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		
		else if (GettypeID == q_engine::GetTypeID<EnemyWeaponEntity>())
		{
			DrawingEnemyWeaponTrigger.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		else if (GettypeID == q_engine::GetTypeID<BulletEntity>())
		{
			DrawingBulletTrigger.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		else if (GettypeID == q_engine::GetTypeID<ItemEntity>())
		{
			DrawingBulletTrigger.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		else if(GettypeID == q_engine::GetTypeID<ProjectileEntity>())
		{
			DrawingPlayerBullet.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
		else if(GettypeID == q_engine::GetTypeID<CubeEntity>())
		{
			DrawingPlayerBullet.Draw({ world2NDC * CollisionBoxMatrix * WidthHeightMatrix });
		}
	}

	
}

void q_engine::CollisionDebugDrawSystem::DrawRayPoints()
{
	auto& RayCastContainer = m_controlled_state->GetComponents<RaycastComponent>();
	for (auto& raycomponent : RayCastContainer)
	{
		auto& RayInterSectData = raycomponent.GetIntersectPoints();
		
		
			int index = 0;
			for (auto& rayindex : RayInterSectData)
			{
			
					const auto& currentLevel = m_controlled_state;
					auto& transformComponents = currentLevel->GetComponents<TransformComponent>();
					
	
					const TransformComponent& camTransform = transformComponents[currentLevel->GetEntities<CameraEntity>().Front().GetEntityID()];
					const glm::mat3 world2NDC = CameraEntity::CalculateWorldToNDCTransform(camTransform.GetPosition(), camTransform.GetRotationRadian(), camTransform.GetScale());

					TransformComponent a{ rayindex.first };
					glm::mat3 CircleradiusMatrix = { 10,0,0,0,10, 0,0,0,1 };
					if(rayindex.second == false)
					DrawingRay.Draw(world2NDC * a.GetOrCalculateTransformMatrix() * CircleradiusMatrix);
					else
					DrawingRayUpdate.Draw(world2NDC * a.GetOrCalculateTransformMatrix() * CircleradiusMatrix);				
					index++;
			}
	}
}

void q_engine::CollisionDebugDrawSystem::DrawCollideCircle()
{
  	const auto& CurrentLevel = m_controlled_state;
	auto& transformComponents = CurrentLevel->GetComponents<TransformComponent>();
	auto& CircleComponents = CurrentLevel->GetComponents<CircleComponent>();
	auto& Container = CurrentLevel->GetComponents<CircleCollisionComponent>();
	DrawObject Drawing = { "CircleMesh"_hashID, "DefaultColorShaderProgram"_hashID, {0, 0, 255, 255} };

	const TransformComponent& camTransform = transformComponents[CurrentLevel->GetEntities<CameraEntity>().Front().GetEntityID()];

	const glm::mat3 world2NDC = CameraEntity::CalculateWorldToNDCTransform(camTransform.GetPosition(), camTransform.GetRotationRadian(), camTransform.GetScale());
	for (auto& collisionbox : Container)
	{
		auto& transformcomp = transformComponents[collisionbox.GetEntityID()];
		auto& CircleComp = CircleComponents[collisionbox.GetEntityID()];
	
		glm::mat3 CircleradiusMatrix = { CircleComp.GetRadius(),0,0,0,CircleComp.GetRadius(), 0,0,0,1 };

		Drawing.Draw(world2NDC * transformcomp.GetOrCalculateTransformMatrix() * CircleradiusMatrix);

	}
}

void q_engine::CollisionDebugDrawSystem::OnOffCollisionBox()
{

	mDrawCollisionBox = !mDrawCollisionBox;
}
