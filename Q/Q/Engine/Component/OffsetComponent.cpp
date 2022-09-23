/*
	File Name: OffsetComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "OffsetComponent.h"

//#include <glm/ext/matrix_transform.inl>


#include "Engine/Engine.h"
#include "Engine/GameStateManager.h"
#include "sol/sol.hpp"
#include "WidthHeightComponent.h"

q_engine::OffsetComponent::OffsetComponent()
{
}

q_engine::OffsetComponent::OffsetComponent(glm::vec2 OffsetPosition, float rotation, float scale): mOffsetPosition(OffsetPosition), mRotation(rotation), mScale(scale),
                                                                      mOffsetMatrix{
	                                                                      1, 0, 0, 0, 1, 0, -mOffsetPosition.x,
	                                                                      -mOffsetPosition.y, 1
                                                                      }
{
}

q_engine::OffsetComponent::OffsetComponent(glm::vec2 OffsetPosition, IDType ObjectMappedEntityID) :mOffsetPosition(OffsetPosition),
mOffsetMatrix{
	1, 0, 0, 0, 1, 0, -mOffsetPosition.x,
	-mOffsetPosition.y, 1
}, mObjectMappedEntityID(ObjectMappedEntityID)
{
	
}

q_engine::OffsetComponent::OffsetComponent(glm::vec2 OffsetPosition, glm::vec2 HotspotPosition, float rotate, float scale) : mOffsetPosition(OffsetPosition), mRotation(rotate), mScale(scale),
mOffsetMatrix{
	1, 0, 0, 0, 1, 0, -mOffsetPosition.x,
	-mOffsetPosition.y, 1}, mHotSpotPosition(HotspotPosition), mHotSpotMatrix{1,0,0,0,1,0,HotspotPosition.x * 0.5f,HotspotPosition.y * 0.5f,1}
{
	
}

void q_engine::OffsetComponent::SetHotSpotPosition(glm::vec2 HotspotPosition)
{
	mHotSpotPosition = HotspotPosition;
	mHotSpotMatrix = { 1, 0, 0, 0, 1, 0, -mHotSpotPosition.x * 0.5f, -mHotSpotPosition.y * 0.5f, 1 };
}

glm::mat3 q_engine::OffsetComponent::GetOffSetMatrix() const
{
	if (mIsFlipped == false)
	{
		return {1, 0, 0, 0, 1, 0, 0, 0, 1};
	}
	return mOffsetMatrix;
}

glm::mat3 q_engine::OffsetComponent::GetHotSpotMatrix() const
{
	if (mIsFlipped == false)
		return mHotSpotMatrix;
	else
		return { 1,0,0,0,1,0,-mHotSpotPosition.x*0.5f,mHotSpotPosition.y * 0.5f,1 };
}

glm::mat3 q_engine::OffsetComponent::GetRotationAndScaleMatrix() const
{
#ifdef QEDITOR
	return glm::mat3{ 1.f };
#endif
	auto playState = Engine::GetEngineComponent<GameStateManager>().GetPlayState();


	glm::vec2 transform = glm::vec2{};

	if (playState->HasComponent(q_engine::GetTypeID<WidthHeightComponent>(), this->GetEntityID()))
	{
		const glm::vec2 ownerEntSize = playState->GetComponents<WidthHeightComponent>()[this->GetEntityID()].GetWidthHeight();
		transform = ownerEntSize * 0.5f;
	}

	const float COS = cos(mRotation);
	const float SIN = sin(mRotation);
	return glm::mat3{ 1, 0, 0, 0, 1, 0, transform.x, transform.y, 1 } *glm::mat3{ mScale, 0, 0, 0, mScale, 0, 0, 0, 1 } * glm::mat3{ COS, SIN, 0, -SIN, COS, 0, 0, 0, 1 } * glm::mat3{1, 0, 0, 0, 1, 0, -transform.x, -transform.y, 1};
}

void q_engine::OffsetComponent::SetFlipped(bool flip)
{
	mIsFlipped = flip;
}

bool q_engine::OffsetComponent::GetIsFlipped() const
{
	return mIsFlipped;
}

void q_engine::OffsetComponent::SetOffsetPosition(glm::vec2 position)
{
	mOffsetPosition = position;
	mOffsetMatrix = {1, 0, 0, 0, 1, 0, -position.x,
					-position.y, 1
	};
}

void q_engine::OffsetComponent::SetObjectMappingEntityID(IDType EntityID)
{
	mObjectMappedEntityID = EntityID;
}

q_engine::IDType q_engine::OffsetComponent::GetObjectMappingEntityID() const
{
	return mObjectMappedEntityID;
}

void q_engine::OffsetComponent::ResetHotSpotPosition()
{
	mHotSpotPosition = { 0,0 };
	mHotSpotMatrix = { 1,0,0,0,1,0,0,0,1 };
}

Json::Value q_engine::OffsetComponent::Serialize()
{
	Json::Value value;
	return value;
}

void q_engine::OffsetComponent::Deserialize([[maybe_unused]]Json::Value& jsonValue)
{
}

void q_engine::RegisterOffsetComponent(sol::state& state)
{
	sol::usertype<OffsetComponent> offsetComponent = state.new_usertype<OffsetComponent>("TextureComponent");
	offsetComponent.set_function("GetIsFlipped", &OffsetComponent::GetIsFlipped);
	offsetComponent.set_function("SetFlipped",&OffsetComponent::SetFlipped);
	offsetComponent.set_function("SetHotSpotPosition", &OffsetComponent::SetHotSpotPosition);
	offsetComponent.set_function("GetHotSpotPosition", &OffsetComponent::GetHotSpotPosition);
}
