/*
	File Name: CollisionComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Component.h"
#include "../Engine/Core/Rect.h"
#include <vector>
#include <sol/state.hpp>

namespace q_engine
{


	class CircleCollisionComponent : public Component<CircleCollisionComponent>
	{
	public:
		CircleCollisionComponent(const glm::vec2& position, float Radius);

		float GetRadius() const;
		void UpdatePosition(const glm::vec2& position);
		glm::vec2 GetPosition() const;

		struct Circle
		{
			glm::vec2 mPosition;
			float mRadius;
		};

		Circle GetCircle() const;
		bool GetIsAdaptPhysic() const;
		void OnoffPhysics();

		std::vector<IDType>& GetResolvingEntityData();
		void AddResolveEntityID(IDType EntityID);
	private:
		Circle mCircle;
		bool IsAdaptPhysic = false;
		std::vector<IDType>mResolvingEntityData;
	};

	
	class RectCollisionComponent : public Component<RectCollisionComponent>
	{
	public:

		RectCollisionComponent() = default;
		
		RectCollisionComponent(const glm::vec2& positions, float width, float height, bool isAdaptPhysics = true, bool isTrigger = false )
		: mColliderBox({positions}, {positions.x + width, positions.y+height}),
			mWidth(width), mHeight(height), mOldColliderBox({ positions }, { positions.x + width, positions.y + height }),
			mIsAdaptPhysic(isAdaptPhysics), mIsTrigger(isTrigger)
		{}
	/*	RectCollisionComponent(const glm::vec2& positions, float width, float height,bool isAdaptPhysics = true, bool isTrigger = false, bool isDynamic = false)
			: mColliderBox({ positions }, { positions.x + width, positions.y + height }),
			mWidth(width), mHeight(height), mOldColliderBox({ positions }, { positions.x + width, positions.y + height }),
			mIsAdaptPhysic(isAdaptPhysics), mIsTrigger(isTrigger), mIsDynamic(isDynamic)
		{}*/

		void SetColliderBox(glm::vec2& position, float width, float height);
		void SetOldColliderBox(glm::vec2& position, float width, float height);
		void UpdateOldPosition(const glm::vec2& position);
		void UpdatePosition(const glm::vec2& position);
		void SetColliderBoxSize(float width, float height);
		void SetColliderPositionX(float posX);
		void SetColliderPositionY(float posY);
		constexpr void SetIsTrigger(bool isTrigger) noexcept { mIsTrigger = isTrigger; }
		
		Rect GetColliderBox() const;
		Rect GetOldColliderBox() const;

		bool GetIsAdaptPhysic() const;
		void OnoffPhysics();
		std::vector<std::pair<glm::vec2, glm::vec2>> GetCollisionBoxline() const;

		[[nodiscard]] constexpr bool IsTrigger() const noexcept { return mIsTrigger; }

		std::vector<IDType>& GetResolvingEntityData();
		std::vector<IDType>& GetDynamicResolvingEntityData();
		void AddResolveEntityID(IDType EntityID);
		void AddDynamicResolveEntityID(IDType EntityID);
		bool GetIsDynamic() const { return mIsDynamic; }
		virtual Json::Value Serialize() override;
		virtual void Deserialize(Json::Value& jsonValue) override;;
		
	private:
		Rect mColliderBox{ {0,0},{0,0} };
		float mWidth = 0;
		float mHeight = 0;
		float mOldWidth = 0;
		float mOldHeight = 0;
		Rect mOldColliderBox{ {0,0},{0,0} };
		bool mIsAdaptPhysic = true;
		bool mIsDynamic = false;
		std::vector<IDType>mResolvingEntityData;
		std::vector<IDType>mDynamicResolvingEntityData;
		bool mIsTrigger = false;
	};

	
	class StaticRectCollisionComponent : public Component<StaticRectCollisionComponent>
	{
	public:
		StaticRectCollisionComponent() = default;
		StaticRectCollisionComponent(const glm::vec2& position, float width, float height, bool IsTrigger = false, bool IsTileOneWay = false, bool IsCanDestroyed = false, bool IsLadder = false)
		: mColliderBox({ position }, { position.x + width, position.y + height }), mWidth(width), mHeight(height), mIsTrigger(IsTrigger), mIsTileOneWay(IsTileOneWay), mIsCanDestroyed(IsCanDestroyed), mIsLadder(IsLadder)
		{
			
		}
		
		Rect GetColliderBox() const;
		void SetIsTrigger(bool IsTrigger);
		void SetColliderBoxSize(float width, float height);
		bool IsTrigger() const;
		void SetTileOneWay(bool IsTileOneWay);
		bool IsTileOneWay() const;
		bool IsCanDestroyed() const;
		bool IsLadder() const;
		void SetCanDestroyed(bool IscanDestroyed);
		
		std::vector<std::pair<glm::vec2, glm::vec2>> GetCollisionBoxline() const;
	private:
		Rect mColliderBox{ {0,0},{0,0} };
		float mWidth = 0;
		float mHeight = 0;
		bool mIsTrigger = false;
		bool mIsTileOneWay = false;
		bool mIsCanDestroyed = false;
		bool mIsLadder = false;
		std::vector<std::pair<glm::vec2, glm::vec2>> mRectEdgeLine;
	};

	void RegisterStaticRectCollisionComponent(sol::state& state);
	
}
