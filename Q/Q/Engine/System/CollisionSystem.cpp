/*
	File Name: CollisionSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CollisionSystem.h"



#include <future>
#include <iostream>
#include <glm/glm.hpp>
#include "../Engine.h"
#include "../GameStateManager.h"
#include "../../Engine/Core/EventManager.h"
#include "../Component/GravityComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/VelocityComponent.h"
#include "../Component/WidthHeightComponent.h"

#include "../Debugger/Debugger.h"

#include "../ECS/InputManager.h"

#include "../Engine/Component/RaycastComponent.h"
#include "../Engine/Component/ObjectStateComponent.h"

#include "../Engine/Core/CanCollideData.h"
#include "../Engine/Entity/CameraEntity.h"
#include "../Engine/Graphics/Window.h"
#include "../Event/TriggerEvent.h"
#include "../Engine/Component/OffsetComponent.h"

#include "JsonParser/MapJsonParser.h"
#include "../Engine/Core/Rect.h"
#include "../Engine/GameState.h"
#include "../Engine/Core/ThreadPool.h"
namespace q_engine
{

	void CollisionSystem::Load()
	{
		const auto& CurrentLevel = m_controlled_state;

		Rect initQuadrect { {0.f,0.f},{7680.f,7680.f} } ;
		mQuadtree.SetInitRect(initQuadrect);
		mQuadtree.SetGameState(CurrentLevel);
		auto& StaticRectComponents = CurrentLevel->GetComponents<StaticRectCollisionComponent>();

 		for(auto & StaticRectComponent : StaticRectComponents)
		{
			auto& WidthheightComponent = CurrentLevel->GetComponents<WidthHeightComponent>()[StaticRectComponent.GetEntityID()];
			float RectWidth = WidthheightComponent.GetWidth<float>();
			float RectHeight = WidthheightComponent.GetHeight<float>();
 			
			StaticRectComponent.SetColliderBoxSize(RectWidth, RectHeight);
 			
			mQuadtree.Add(StaticRectComponent.GetEntityID());
		}
		q_engine::Level* level = reinterpret_cast<q_engine::Level*>(m_controlled_state);
		float heightpoint = level->GetMapData().mOffset.y;
		float initrectstart = level->GetMapData().mOffset.x;
		float TileSizeX = level->GetMapData().mTileSize.x;
		float TileSizeY = level->GetMapData().mTileSize.y;
		for(int i = 0; i< level->GetMapData().mTileNum.y; i++)
		{
			bool BeforeTileexist = false;
			float rectwidth = 0.f;
			float rectstartpoint = initrectstart;
			float startindexposition = initrectstart;
			for(int j =0; j < level->GetMapData().mTileNum.x; j++)
			{
				if(level->GetTileByIndex(j,i) != TileType::Null && level->GetTileByIndex(j, i) != TileType::Ladder)
				{
					if(BeforeTileexist == false)
					{
						rectstartpoint = startindexposition;
					}
					rectwidth += TileSizeX;
					BeforeTileexist = true;
				}
				else if((level->GetTileByIndex(j, i) == TileType::Null || level->GetTileByIndex(j, i) == TileType::Ladder) && BeforeTileexist == true)
				{
					RayRectTile makeRect;
					makeRect.SetColliderBox(glm::vec2{ rectstartpoint, heightpoint }, rectwidth, level->GetMapData().mTileSize.y);
					mRayRectTile.push_back(makeRect);
					BeforeTileexist = false;
					rectwidth = 0.f;
				}else if(BeforeTileexist == false)
				{
					rectstartpoint = startindexposition;
				}
				startindexposition += TileSizeX;
			}
			heightpoint += TileSizeY;
		}
		
	}
	
	void CollisionSystem::Update(double dt) noexcept
	{
		
		const auto& CurrentLevel = m_controlled_state;
	

		auto& CircleComponents = CurrentLevel->GetComponents<CircleCollisionComponent>();
		auto& RayCastComponents = CurrentLevel->GetComponents<RaycastComponent>();
		auto& TransFormComponents = CurrentLevel->GetComponents<TransformComponent>();
		
		for (auto& raycast : RayCastComponents)
		{
			std::vector <std::pair<glm::vec2, bool>> &RayInterSectData = raycast.GetIntersectPoints();
			for (std::pair<glm::vec2, bool>& rayindex : RayInterSectData)
			{
				rayindex.second = false;
			}
		}
		for(auto& RemoveCollision : GetDeleteStaticCollisionData())
		{
			if(CurrentLevel->HasComponent<StaticRectCollisionComponent>(RemoveCollision))
			{
				mQuadtree.Remove(RemoveCollision);
				m_controlled_state->DeleteEntityComponents(RemoveCollision);
			}
		}
		GetDeleteStaticCollisionData().clear();

		RectRectUpdate(dt);
		QuadTreeUpdate(dt);

		CircleRectUpdate(dt);
		CircleCircleUpdate(dt);


		Engine::GetEngineComponent<ThreadPool>().WakeUpThreads(ThreadPool::GetMaxThreads());
		for (int num = 0; num < ThreadPool::GetMaxThreads(); ++num)
		{
			Engine::GetEngineComponent<ThreadPool>().GiveThreadWork([this, num]()
				{
					RayCastingUpdate(num);
				});
		}
		Engine::GetEngineComponent<ThreadPool>().WaitAll();

		for (auto& circleCollision : CircleComponents)
		{
			auto [x, y] = get_mouse_position();
			auto [w, h] = get_window_size();
			auto& transformcomp = TransFormComponents[circleCollision.GetEntityID()];
			transformcomp.SetPosition({ static_cast<float>(x) - w / 2.f,h - static_cast<float>(y) - h / 2.f });
			circleCollision.UpdatePosition(transformcomp.GetPosition());
		}



		
		glm::vec2 playerposition{0.f,0.f};
		float playerwidth = 0;
		float playerheight = 0;
		for(auto& player : TransFormComponents)
		{
			
			if(player.GetEntityTypeID() == q_engine::GetTypeID<PlayerEntity>())
			{
				auto playerEntityID = player.GetEntityID();
				WidthHeightComponent& widthHeightPlayer = CurrentLevel->GetComponents<WidthHeightComponent>()[playerEntityID];
				playerposition = player.GetPosition();
				playerwidth =widthHeightPlayer.GetWidth<float>();
				playerheight= widthHeightPlayer.GetHeight<float>();
			}
			
		}

		
		for(RaycastComponent& RaycastCollision : RayCastComponents)
		{
			const IDType entityTypeID = RaycastCollision.GetEntityTypeID();
			if (entityTypeID == q_engine::GetTypeID<EnemyEntity>())
			{
				RaycastCollision.UpdateTargetPosition(playerposition + glm::vec2{ playerwidth / 2,playerheight / 2 });
			}
			else if (entityTypeID == q_engine::GetTypeID<PlayerEntity>())
			{
				const IDType playerEntityID = RaycastCollision.GetEntityID();
			
				auto [mouseX, mouseY] = get_mouse_position();
				const IDType camID = CurrentLevel->GetEntities<CameraEntity>().Front().GetEntityID();
				const TransformComponent& camTransform = CurrentLevel->GetComponents<TransformComponent>()[camID];
				
				
				const glm::vec2 campositiontotarget = CameraEntity::CalculateWindowPositionToWorld(mouseX, mouseY, camTransform.GetPosition(), camTransform.GetRotationRadian(), camTransform.GetScale());
				RaycastCollision.UpdateTargetPosition(
					campositiontotarget
				);
			}
		}
		
	}
	
	bool CollisionSystem::PointRectCollisionCheck(glm::vec2 point ,Rect BOBject)
	{
		return point.x >= BOBject.Left() && point.x <= BOBject.Right() && point.y <= BOBject.Top() && point.y >= BOBject.Bottom();
	}
	bool CollisionSystem::AABBCollisionCheck(Rect Aobject, Rect Bobject) const
	{
		if (Aobject.Left() <= Bobject.Right() && Aobject.Right() >= Bobject.Left() && Aobject.Bottom() <= Bobject.Top() && Aobject.Top() >= Bobject.Bottom())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	
	bool CollisionSystem::CircleCircleCollisionCheck(CircleCollisionComponent& Acomponent, CircleCollisionComponent& Bcomponent)
	{

		const float X_square = (Acomponent.GetPosition().x - Bcomponent.GetPosition().x) * (Acomponent.GetPosition().x - Bcomponent.GetPosition().x);
		const float Y_square = (Acomponent.GetPosition().y - Bcomponent.GetPosition().y) * (Acomponent.GetPosition().y - Bcomponent.GetPosition().y);
		const float Radius_square = Acomponent.GetRadius() + Bcomponent.GetRadius();
		if (X_square + Y_square <= Radius_square)
		{
			return true;
		}
			return false;
	}


	float CollisionSystem::Crossproduct(const glm::vec2& a, const glm::vec2& b)
	{
		return ((a.x * b.y) - (b.x * a.y));
	}

	glm::vec2 CollisionSystem::SetVectorByAngle(const float& angle)
	{
		glm::vec2 Result;
		Result.x = cos(angle);
		Result.y = sin(angle);
		return Result;
	}

	float CollisionSystem::GetAngle(const glm::vec2& vec)
	{
		return atan2(vec.y, vec.x);
	}

	float CollisionSystem::abs(const glm::vec2& vec)
	{
		return sqrt((vec.x * vec.x) + (vec.y * vec.y));
	}

	glm::vec2 CollisionSystem::Normalize(const glm::vec2& vec)
	{
		float magnitude = abs(vec);
		return glm::vec2{ vec.x / magnitude, vec.y / magnitude };
	}



	float CollisionSystem::GetCorrectValue() const
	{
		return mCorrectionValue;
	}

	bool CollisionSystem::CircleRectCollisionCheck(const CircleCollisionComponent& Bcircle , const RectCollisionComponent& ARect)
	{
		const float RectRight = ARect.GetColliderBox().Right();
		const float RectLeft = ARect.GetColliderBox().Left();
		const float RectTop = ARect.GetColliderBox().Top();
		const float RectBottom = ARect.GetColliderBox().Bottom();
		const glm::vec2 RectTopLeft = { RectLeft,RectTop};
		const glm::vec2 RectTopRight = { RectRight,RectTop};
		const glm::vec2 RectBottomLeft = { RectLeft,RectBottom};
		const glm::vec2 RectBottomRight = { RectRight,RectBottom};

		const float Radius = Bcircle.GetRadius();
		const float MaxRangeX = RectRight + Radius;
		const float MinRangeX = RectLeft - Radius;
		const float MaxRangeY = RectTop + Radius;
		const float MinRangeY = RectBottom - Radius;
		const float RadiusSquare = Radius * Radius;
		const glm::vec2 CirclePos = Bcircle.GetPosition();

		if((RectLeft <= CirclePos.x && CirclePos.x <= RectRight) || (RectTop <= CirclePos.y && CirclePos.y <= RectBottom))
		{
			if ((MinRangeX < CirclePos.x && CirclePos.x < MaxRangeX) && (MinRangeY < CirclePos.y && CirclePos.y < MaxRangeY))
			{
				return true;
			}
		}
		else
		{
			if(DistanceSquare(CirclePos, RectTopLeft) <= RadiusSquare || DistanceSquare(CirclePos,RectBottomRight) <= RadiusSquare
				|| DistanceSquare(CirclePos,RectTopRight) <= RadiusSquare || DistanceSquare(CirclePos,RectBottomLeft) <= RadiusSquare)
			{
				return true;
			}
		}
		return false;
	}

	bool CollisionSystem::DoesIntersect(glm::vec2& a1, glm::vec2& a2, glm::vec2& b1, glm::vec2& b2) const
	{
		glm::vec2 p = { a1};
		glm::vec2 r = { a2 - a1 };
		glm::vec2 q = { b1 };
		glm::vec2 s = { b2 - b1 };

		float t = Crossproduct((q-p), s) / Crossproduct(r, s);
		float u = Crossproduct((q - p), r) / Crossproduct(r, s);
		return (0.0f <= t && t <= 1.0f) && (0.0f <= u);
	}

	glm::vec2 CollisionSystem::GetIntersect(glm::vec2 a1, glm::vec2 a2, glm::vec2 b1, glm::vec2 b2) const
	{
		glm::vec2 p = a1;
		glm::vec2 r = a2 - a1;
		glm::vec2 q = b1;
		glm::vec2 s = b2 - b1;

		glm::vec2 subtract_QP = q - p;
		float cross_RS = Crossproduct(r, s);
		
		float t = Crossproduct(subtract_QP, s) / cross_RS;
		float u = Crossproduct(subtract_QP, r) / cross_RS;

		glm::vec2 cross;
		if (0.0f < t && t <= 1.0f && 0.0f <= u) {

			cross.x = a1.x + t * (a2.x - a1.x);
			cross.y = a1.y + t * (a2.y - a1.y);
			return cross;
		}
		return glm::vec2(0.f, 0.f);

		
	}

	void CollisionSystem::RayCastingUpdate(int threadNum)
	{		
		const auto& CurrentLevel = m_controlled_state;
		auto RayCastContainer = CurrentLevel->GetComponents<RaycastComponent>().GetPartContainer(ThreadPool::GetMaxThreads(), threadNum);
		
		auto& RectContainer = CurrentLevel->GetComponents<RectCollisionComponent>();
		auto& compTransContainer = m_controlled_state->GetComponents<TransformComponent>();
		auto& compWidthHeightContainer = m_controlled_state->GetComponents<WidthHeightComponent>();

		for(auto& RayobjectA : RayCastContainer)
		{
			auto compEntityID = RayobjectA.GetEntityID();
			auto& RayVectorData = RayobjectA.GetShootRayVectorData();
			auto& RayDistanceData = RayobjectA.GetDistanceData();
			auto& TransComp = compTransContainer[compEntityID];
			auto& compWidthHeight = compWidthHeightContainer[compEntityID];
			glm::vec2 RayPoint = { TransComp.GetPosition().x + compWidthHeight.GetWidth<float>() / 2,TransComp.GetPosition().y + compWidthHeight.GetHeight<float>() / 2 };
			glm::vec2 InclineVector = { RayobjectA.GetTargetPosition() - RayPoint };

			

			float base_line_angle = GetAngle(InclineVector);
			
				int lineindex = RayobjectA.GetRayOrderindex();
				float inclineAngle = base_line_angle + glm::radians(RayobjectA.GetRayAngle() * lineindex);
				RayVectorData.at(lineindex) = SetVectorByAngle(inclineAngle);
				RayDistanceData.at(lineindex) = std::numeric_limits<float>::max();
				glm::vec2 RayVectorInverse = { 1 / RayVectorData.at(lineindex).x,1 / RayVectorData.at(lineindex).y };

			
				for (auto& collisionboxB : RectContainer)
				{
					if (RayobjectA.GetEntityID() == collisionboxB.GetEntityID())
					{
						continue;
					}
					auto CompAEntityTypeID = RayobjectA.GetEntityTypeID();
					auto CompBEntityTypeID = collisionboxB.GetEntityTypeID();

					if (CompAEntityTypeID == CompBEntityTypeID)
					{
						continue;
					}
					if(CompBEntityTypeID == q_engine::GetTypeID<WeaponEntity>())
					{
						continue;
					}
					

					RayCastingCheck(RayobjectA, collisionboxB, CurrentLevel, lineindex, RayVectorInverse);
				}
			
				for(auto& collisionboxB : mRayRectTile)
				{
					RayCastingCheck(RayobjectA, collisionboxB, CurrentLevel, lineindex, RayVectorInverse);
				}
				RayobjectA.RayIndexChecking();
			
		}

	}


	void CollisionSystem::RayCastingCheck(RaycastComponent& RayCastComponentA,
	                                               RectCollisionComponent& componentB , GameState* CurrentState,int Rayindex, glm::vec2& directioninverse )
	{
		
		auto compEntityID = RayCastComponentA.GetEntityID();
		TransformComponent& compTransComp = CurrentState->GetComponents<TransformComponent>()[compEntityID];
		auto& compWidthHeight = CurrentState->GetComponents<WidthHeightComponent>()[compEntityID];
		glm::vec2 RayPoint = { compTransComp.GetPosition().x + compWidthHeight.GetWidth<float>() / 2,compTransComp.GetPosition().y + compWidthHeight.GetHeight<float> () / 2 };
		auto& RayInterSectData = RayCastComponentA.GetIntersectPoints();
		auto& RayVectorData = RayCastComponentA.GetShootRayVectorData();
		auto& RayDistanceData = RayCastComponentA.GetDistanceData();
		auto& RayColliTypeIDEntityIDdata = RayCastComponentA.GetRayCollisionEntityTypeEntityID();
		Rect componentBRect = componentB.GetColliderBox();


		if(componentB.IsTrigger())
		{
			return;
		}
		
	
			float tx1 = (componentBRect.Left() - RayPoint.x) * directioninverse.x;
			float tx2 = (componentBRect.Right() - RayPoint.x) * directioninverse.x;

			float t_near = std::min(tx1, tx2);
			float t_far = std::max(tx1, tx2);
	
			float ty1 = (componentBRect.Bottom() - RayPoint.y) * directioninverse.y;
			float ty2 = (componentBRect.Top() - RayPoint.y) * directioninverse.y;

			t_near = std::max(t_near, std::min(ty1, ty2));
			t_far = std::min(t_far, std::max(ty1, ty2));
		
		
		if(t_far >= t_near)
		{
			glm::vec2 equationvalue{ t_near,t_far };
			glm::vec2 pt = { RayPoint.x + RayVectorData.at(0).x * equationvalue.x, RayPoint.y + RayVectorData.at(0).y * equationvalue.x };
			
				float d = glm::distance(RayPoint, pt);
				if (d < RayDistanceData.at(Rayindex)) 
				{
					RayDistanceData.at(Rayindex) = d;
					RayInterSectData.at(Rayindex).first = pt;
					RayInterSectData.at(Rayindex).second = true;
					RayColliTypeIDEntityIDdata.at(Rayindex).first = componentB.GetEntityTypeID();
					RayColliTypeIDEntityIDdata.at(Rayindex).second = componentB.GetEntityID();
				}
		}

	}
	
	void CollisionSystem::RayCastingCheck(RaycastComponent& RayCastComponentA,
		StaticRectCollisionComponent& componentB, GameState* CurrentState, int Rayindex , glm::vec2& directioninverse)
	{

		auto compEntityID = RayCastComponentA.GetEntityID();
		TransformComponent& compTransComp = CurrentState->GetComponents<TransformComponent>()[compEntityID];
		auto& compWidthHeight = CurrentState->GetComponents<WidthHeightComponent>()[compEntityID];
		glm::vec2 RayPoint = { compTransComp.GetPosition().x + compWidthHeight.GetWidth<float>() / 2,compTransComp.GetPosition().y + compWidthHeight.GetHeight<float>() / 2 };
		auto& RayInterSectData = RayCastComponentA.GetIntersectPoints();
		auto& RayVectorData = RayCastComponentA.GetShootRayVectorData();
		auto& RayDistanceData = RayCastComponentA.GetDistanceData();
		auto& RayColliTypeIDEntityIDdata = RayCastComponentA.GetRayCollisionEntityTypeEntityID();
		glm::vec2 targetposition = RayPoint + RayVectorData.at(Rayindex);


		Rect componentBRect = componentB.GetColliderBox();

		if (componentB.IsTrigger())
		{
			return;
		}
		if(componentB.IsLadder())
		{
			return;
		}
		float tx1 = (componentBRect.Left() - RayPoint.x) * directioninverse.x;
		float tx2 = (componentBRect.Right() - RayPoint.x) * directioninverse.x;

		float t_near = std::min(tx1, tx2);
		float t_far = std::max(tx1, tx2);

		float ty1 = (componentBRect.Bottom() - RayPoint.y) * directioninverse.y;
		float ty2 = (componentBRect.Top() - RayPoint.y) * directioninverse.y;

		t_near = std::max(t_near, std::min(ty1, ty2));
		t_far = std::min(t_far, std::max(ty1, ty2));
		
		if (t_far >= std::max(0.f,t_near))
		{
			glm::vec2 equationvalue{ t_near,t_far };
			glm::vec2 pt = { RayPoint.x + RayVectorData.at(Rayindex).x * equationvalue.x, RayPoint.y + RayVectorData.at(Rayindex).y * equationvalue.x };

			float d = glm::distance(RayPoint, pt);
			if (d < RayDistanceData.at(Rayindex)) 
			{
				RayDistanceData.at(Rayindex) = d;
				RayInterSectData.at(Rayindex).first = pt;
				RayInterSectData.at(Rayindex).second = true;
				RayColliTypeIDEntityIDdata.at(Rayindex).first = componentB.GetEntityTypeID();
				RayColliTypeIDEntityIDdata.at(Rayindex).second = componentB.GetEntityID();
			}
		}

	}

	void CollisionSystem::RayCastingCheck(RaycastComponent& RayCastComponentA, RayRectTile& componentB,
		GameState* CurrentState, int Rayindex, glm::vec2& directioninverse)
	{

		auto compEntityID = RayCastComponentA.GetEntityID();
		TransformComponent& compTransComp = CurrentState->GetComponents<TransformComponent>()[compEntityID];
		auto& compWidthHeight = CurrentState->GetComponents<WidthHeightComponent>()[compEntityID];
		glm::vec2 RayPoint = { compTransComp.GetPosition().x + compWidthHeight.GetWidth<float>() / 2,compTransComp.GetPosition().y + compWidthHeight.GetHeight<float>() / 2 };
		auto& RayInterSectData = RayCastComponentA.GetIntersectPoints();
		auto& RayVectorData = RayCastComponentA.GetShootRayVectorData();
		auto& RayDistanceData = RayCastComponentA.GetDistanceData();
		auto& RayColliTypeIDEntityIDdata = RayCastComponentA.GetRayCollisionEntityTypeEntityID();


		Rect componentBRect = componentB.GetColliderBox();

		//if (componentB.IsTrigger())
		//{
		//	return;
		//}
		float tx1 = (componentBRect.Left() - RayPoint.x) * directioninverse.x;
		float tx2 = (componentBRect.Right() - RayPoint.x) * directioninverse.x;

		float t_near = std::min(tx1, tx2);
		float t_far = std::max(tx1, tx2);

		float ty1 = (componentBRect.Bottom() - RayPoint.y) * directioninverse.y;
		float ty2 = (componentBRect.Top() - RayPoint.y) * directioninverse.y;

		t_near = std::max(t_near, std::min(ty1, ty2));
		t_far = std::min(t_far, std::max(ty1, ty2));

		if (t_far >= std::max(0.f, t_near))
		{
			glm::vec2 equationvalue{ t_near,t_far };
			glm::vec2 pt = { RayPoint.x + RayVectorData.at(Rayindex).x * equationvalue.x, RayPoint.y + RayVectorData.at(Rayindex).y * equationvalue.x };

			float d = glm::distance(RayPoint, pt);
			if (d < RayDistanceData.at(Rayindex))
			{
				
				RayDistanceData.at(Rayindex) = d;
				RayInterSectData.at(Rayindex).first = pt;
				RayInterSectData.at(Rayindex).second = true;
				RayColliTypeIDEntityIDdata.at(Rayindex).first = q_engine::GetTypeID<MapEntity>();
				RayColliTypeIDEntityIDdata.at(Rayindex).second = m_controlled_state->GetComponents<StaticRectCollisionComponent>().Front().GetEntityID();
			}
			//}
		}

		
	}


	float CollisionSystem::DistanceSquare(const glm::vec2& Avalue,const glm::vec2& Bvalue)
	{
		float Xvalue = Avalue.x - Bvalue.x;
		Xvalue *= Xvalue;
		float Yvalue = Avalue.y - Bvalue.y;
		Yvalue *= Yvalue;

		return { Xvalue + Yvalue };
	}

	std::vector<IDType>& CollisionSystem::GetDeleteStaticCollisionData()
	{
		return mDeleteStaticCollisionData;
	}



#include "../Game/Entity/JumpTriggerEntity.h"
	void CollisionSystem::RectRectUpdate([[maybe_unused]]double dt)
	{
		const auto& CurrentLevel = m_controlled_state;
		auto& RectContainer = CurrentLevel->GetComponents<RectCollisionComponent>();

		const auto& canCollideData = CanCollideData::GetCanCollideData();

		for (auto& componentA : RectContainer)
		{
			for (auto& componentB : RectContainer)
			{
				if (componentB.IsTrigger())
				{
					continue;
				}
				
				if (componentA.GetEntityID() == componentB.GetEntityID())
				{
					continue;
				}
				auto CompAEntityTypeID = componentA.GetEntityTypeID();
				auto CompBEntityTypeID = componentB.GetEntityTypeID();
				
				if (CompAEntityTypeID == CompBEntityTypeID)
				{
					continue;
				}

				
				if (canCollideData.count(CompAEntityTypeID) && canCollideData.at(CompAEntityTypeID).count(CompBEntityTypeID))
				{
					
						if (AABBCollisionCheck(componentA.GetColliderBox(), componentB.GetColliderBox()))
						{

							
							if (componentA.GetIsAdaptPhysic() == true)
							{
								componentA.AddDynamicResolveEntityID(componentB.GetEntityID());
							}
							if (componentA.IsTrigger())
							{
								Engine::GetEngineComponent<EventManager>().AddEvent<TriggerEvent>(componentA.GetEntityTypeID(), componentA.GetEntityID(), componentB.GetEntityTypeID(), componentB.GetEntityID());
							}
							if (componentB.IsTrigger())
							{
								Engine::GetEngineComponent<EventManager>().AddEvent<TriggerEvent>(componentB.GetEntityTypeID(), componentB.GetEntityID(), componentA.GetEntityTypeID(), componentA.GetEntityID());
							}
						}
					
				}
		
			}
			
		}
		
	}


	void CollisionSystem::ObjectMovingCollisionCheck(RectCollisionComponent& componentA,
		RectCollisionComponent& componentB ,GameState* gamestate)
	{
		auto compAEntityID = componentA.GetEntityID();
		Rect AColliderBox = componentA.GetColliderBox();
		if (PointRectCollisionCheck({ AColliderBox.Left(), AColliderBox.Bottom() - 10.f }, componentB.GetColliderBox())
			|| PointRectCollisionCheck({ AColliderBox.Right(), AColliderBox.Bottom() - 10.f }, componentB.GetColliderBox())
			|| PointRectCollisionCheck({ AColliderBox.Left() + AColliderBox.Size().x / 2.f, AColliderBox.Bottom() - 10.f }, componentB.GetColliderBox()))
		{
			auto& compAGravity = gamestate->GetComponents<GravityComponent>()[compAEntityID];
			compAGravity.SetIsOnGround(true);
		}

		if (componentA.GetEntityTypeID() == q_engine::GetTypeID<PlayerEntity>())
		{
			if (PointRectCollisionCheck({ componentA.GetColliderBox().Left(), componentA.GetColliderBox().Top() + 20.f }, componentB.GetColliderBox())
				|| PointRectCollisionCheck({ componentA.GetColliderBox().Right()-2.f, componentA.GetColliderBox().Top() + 20.f }, componentB.GetColliderBox()))
			{
				auto& compAObjectState = gamestate->GetComponents<ObjectStateComponent>()[compAEntityID];
				compAObjectState.SetIsGroundOntheHead(true);
			}
		}


	}
	void CollisionSystem::ObjectMovingCollisionCheck(RectCollisionComponent& componentA,
		StaticRectCollisionComponent& componentB, GameState* gamestate)
	{
		auto compAEntityID = componentA.GetEntityID();
		Rect AColliderBox = componentA.GetColliderBox();


		if (componentA.GetEntityTypeID() == q_engine::GetTypeID<PlayerEntity>())
		{
			if (PointRectCollisionCheck({ componentA.GetColliderBox().Left(), componentA.GetColliderBox().Top() + 20.f }, componentB.GetColliderBox())
				|| PointRectCollisionCheck({ componentA.GetColliderBox().Right() - 2.f, componentA.GetColliderBox().Top() + 20.f }, componentB.GetColliderBox()))
			{
				auto& compAObjectState = gamestate->GetComponents<ObjectStateComponent>()[compAEntityID];
				compAObjectState.SetIsGroundOntheHead(true);
			}
		}


	}
	
	void CollisionSystem::QuadTreeUpdate([[maybe_unused]]double dt)
	{
		const auto& CurrentLevel = m_controlled_state;
		auto& RectComponents = CurrentLevel->GetComponents<RectCollisionComponent>();
		auto& VelocityComponents = CurrentLevel->GetComponents<VelocityComponent>();
		for(RectCollisionComponent & rectcomponent : RectComponents)
		{			
		
			/*	if (rectcomponent.GetEntityTypeID() != q_engine::GetTypeID<EnemyWeaponEntity>())
				{*/
				auto staticCollisionHits = mQuadtree.query(rectcomponent.GetColliderBox());
					for (auto& staticCollison : staticCollisionHits)
					{
						auto& componentb = CurrentLevel->GetComponents<StaticRectCollisionComponent>()[staticCollison];
						if (AABBCollisionCheck(rectcomponent.GetColliderBox(), componentb.GetColliderBox()))
						{
							if(rectcomponent.GetEntityTypeID() == q_engine::GetTypeID<BombEntity>())
							{
								Debugger::GetLogger().AddMessage("check!");
							}
							rectcomponent.AddResolveEntityID(componentb.GetEntityID());
							if (rectcomponent.IsTrigger())
							{
								Engine::GetEngineComponent<EventManager>().AddEvent<TriggerEvent>(rectcomponent.GetEntityTypeID(), rectcomponent.GetEntityID(), componentb.GetEntityTypeID(), componentb.GetEntityID());
							}
							if(componentb.IsTrigger())
							{
								Engine::GetEngineComponent<EventManager>().AddEvent<TriggerEvent>( componentb.GetEntityTypeID(), componentb.GetEntityID(),rectcomponent.GetEntityTypeID(), rectcomponent.GetEntityID());
							}
							if (componentb.IsLadder())
							{
								Engine::GetEngineComponent<EventManager>().AddEvent<TriggerEvent>(componentb.GetEntityTypeID(), componentb.GetEntityID(), rectcomponent.GetEntityTypeID(), rectcomponent.GetEntityID());
							}
				
						}
						else
						{
							auto& compAGravity = CurrentLevel->GetComponents<GravityComponent>()[rectcomponent.GetEntityID()];
							compAGravity.SetIsOnGround(false);
							ObjectMovingCollisionCheck(rectcomponent, componentb, CurrentLevel);
						}
					}
				
		}



		
	}

	void CollisionSystem::CircleRectUpdate([[maybe_unused]]double dt)
	{
		const auto& CurrentLevel = m_controlled_state;
		auto& RectContainer = CurrentLevel->GetComponents<RectCollisionComponent>();
		auto& CircleContainer = CurrentLevel->GetComponents<CircleCollisionComponent>();
		const auto& canCollideData = CanCollideData::GetCanCollideData();
		for (auto& componentA : CircleContainer)
		{
			for (auto& componentB : RectContainer)
			{
				if (componentA.GetEntityID() == componentB.GetEntityID())
				{
					continue;
				}
				auto CompAEntityTypeID = componentA.GetEntityTypeID();
				auto CompBEntityTypeID = componentB.GetEntityTypeID();
				if (CompAEntityTypeID == CompBEntityTypeID)
				{
					continue;
				}
				if (canCollideData.count(CompAEntityTypeID) && canCollideData.at(CompAEntityTypeID).count(CompBEntityTypeID))
				{
					if (CircleRectCollisionCheck(componentA, componentB))
					{
						Debugger::GetLogger().AddMessage("circle Rect collision!");
					}
				}
			}
		}
	}

	void CollisionSystem::CircleCircleUpdate([[maybe_unused]] double dt)
	{
		const auto& CurrentLevel = m_controlled_state;
		auto& CircleContainer = CurrentLevel->GetComponents<CircleCollisionComponent>();
		for (auto& componentA : CircleContainer)
		{
			for (auto& componentB : CircleContainer)
			{
				if (componentA.GetEntityID() == componentB.GetEntityID())
				{
					continue;
				}
				auto CompAEntityTypeID = componentA.GetEntityTypeID();
				auto CompBEntityTypeID = componentB.GetEntityTypeID();
				if (CompAEntityTypeID == CompBEntityTypeID)
				{
					continue;
				}
				const auto& canCollideData = CanCollideData::GetCanCollideData();
				if (canCollideData.count(CompAEntityTypeID) && canCollideData.at(CompAEntityTypeID).count(CompBEntityTypeID))
				{
#ifdef _DEBUG
					if (CircleCircleCollisionCheck(componentA, componentB))
					{
						Debugger::GetLogger().AddMessage("circle circle collision!");
					}
#endif
				}

			}
		}
		
	}
}
