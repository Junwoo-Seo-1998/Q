/*
	File Name: OffsetComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <glm/vec2.hpp>
#include <glm/matrix.hpp>
#include "../ECS/Component.h"
#include "Engine/Types.h"
namespace sol
{
	class state;
}

namespace q_engine
{

	class OffsetComponent : public Component<OffsetComponent>
	{
	public:
		OffsetComponent();
		OffsetComponent(glm::vec2 OffsetPosition, float rotate = 0.f, float scale = 1.f);
		OffsetComponent(glm::vec2 OffsetPosition, IDType ObjectMappedEntityID);
		OffsetComponent(glm::vec2 OffsetPosition, glm::vec2 HotspotPosition, float rotate = 0.f, float scale = 1.f);
		void SetHotSpotPosition(glm::vec2 HotspotPosition);
		float GetOffSetPositionX() const { return mOffsetPosition.x; }
		float GetOffSetPositionY() const { return mOffsetPosition.y; }
		glm::vec2 GetHotSpotPosition() const { return mHotSpotPosition; }
		glm::mat3 GetOffSetMatrix() const;
		glm::mat3 GetHotSpotMatrix() const;
		glm::mat3 GetRotationAndScaleMatrix() const;
		void SetFlipped(bool flip);
		bool GetIsFlipped() const;
		void SetOffsetPosition(glm::vec2 position);
		void SetObjectMappingEntityID(IDType EntityID);
		IDType GetObjectMappingEntityID() const;
		void ResetHotSpotPosition();
		virtual Json::Value Serialize() override;
		virtual void Deserialize(Json::Value& jsonValue) override;
	private:
		glm::vec2 mOffsetPosition{ 0,0 };
		bool mIsFlipped = false;
		glm::mat3 mOffsetMatrix{ 1,0,0,0,1,0,0,0,1 };
		glm::mat3 mHotSpotMatrix{ 1,0,0,0,1,0,0,0,1 };
		glm::vec2 mHotSpotPosition{ 0,0 };
		IDType mObjectMappedEntityID = INVALID_ID;

		float mRotation = 0.f;
		float mScale = 1.f;
	};
	void RegisterOffsetComponent(sol::state& state);

}
