/*
	File Name: CollisionComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/


#include "../Engine/Component/CollisionComponent.h"


q_engine::CircleCollisionComponent::CircleCollisionComponent(const glm::vec2& position, float Radius): mCircle{position, Radius} {}

float q_engine::CircleCollisionComponent::GetRadius() const
{
	return mCircle.mRadius;
}

void q_engine::CircleCollisionComponent::UpdatePosition(const glm::vec2& position)
{
	mCircle.mPosition = position;
}

glm::vec2 q_engine::CircleCollisionComponent::GetPosition() const
{
	return mCircle.mPosition;
}

q_engine::CircleCollisionComponent::Circle q_engine::CircleCollisionComponent::GetCircle() const
{
	return mCircle;
}

bool q_engine::CircleCollisionComponent::GetIsAdaptPhysic() const
{
	return IsAdaptPhysic;
}

void q_engine::CircleCollisionComponent::OnoffPhysics()
{
	IsAdaptPhysic = !IsAdaptPhysic;
}

std::vector<q_engine::IDType>& q_engine::CircleCollisionComponent::GetResolvingEntityData()
{
	return mResolvingEntityData;
}

void q_engine::CircleCollisionComponent::AddResolveEntityID(IDType EntityID)
{
	mResolvingEntityData.push_back(EntityID);
}

void q_engine::RectCollisionComponent::SetColliderBox(glm::vec2& position, float width, float height)
{
	mColliderBox = {{position}, {position.x + width, position.y + height}};
	mWidth = width;
	mHeight = height;
}

void q_engine::RectCollisionComponent::SetOldColliderBox(glm::vec2& position, float width, float height)
{

	mOldColliderBox = { position, {position.x + width, position.y + height} };
	mWidth = width;
	mHeight = height;
}

void q_engine::RectCollisionComponent::UpdateOldPosition(const glm::vec2& position)
{
	mOldColliderBox = { position, {position.x + mWidth, position.y + mHeight} };
	mOldWidth = mWidth;
	mOldHeight = mHeight;
}

void q_engine::RectCollisionComponent::UpdatePosition(const glm::vec2& position)
{
	mColliderBox = {position, {position.x + mWidth, position.y + mHeight}};
}

void q_engine::RectCollisionComponent::SetColliderBoxSize(float width, float height)
{
	float PosX = mColliderBox.Left();
	float PosY = mColliderBox.Bottom();
	mColliderBox = { {PosX,PosY},{ PosX + width, PosY + height} };
	mWidth = width;
	mHeight = height;
}

void q_engine::RectCollisionComponent::SetColliderPositionX(float posX)
{
	float OldY = mColliderBox.Bottom();
	mColliderBox = { {posX, OldY}, {posX + mWidth, OldY + mHeight} };
}

void q_engine::RectCollisionComponent::SetColliderPositionY(float posY)
{
	float OldX = mColliderBox.Left();
	mColliderBox = { {OldX, posY}, {OldX + mWidth, posY + mHeight} };
}

Rect q_engine::RectCollisionComponent::GetColliderBox() const
{
	return mColliderBox;
}

Rect q_engine::RectCollisionComponent::GetOldColliderBox() const
{
	return mOldColliderBox;
}

bool q_engine::RectCollisionComponent::GetIsAdaptPhysic() const
{
	return mIsAdaptPhysic;
}

void q_engine::RectCollisionComponent::OnoffPhysics()
{
	mIsAdaptPhysic = !mIsAdaptPhysic;
}

std::vector<std::pair<glm::vec2, glm::vec2>> q_engine::RectCollisionComponent::GetCollisionBoxline() const
{
	std::vector<std::pair<glm::vec2, glm::vec2>> LinesData;
	glm::vec2 StartPoint = mColliderBox.bottomLeft;
	glm::vec2 EndPoint ={mColliderBox.Left(), mColliderBox.Top()};
	LinesData.push_back({ StartPoint, EndPoint });

	StartPoint = { mColliderBox.Right(), mColliderBox.Top() };
	EndPoint = { mColliderBox.Right(), mColliderBox.Bottom() };
	LinesData.push_back({ StartPoint, EndPoint });
	StartPoint = mColliderBox.bottomLeft;
	EndPoint = { mColliderBox.Right(),mColliderBox.Bottom() };
	LinesData.push_back({ StartPoint, EndPoint });
	StartPoint = { mColliderBox.Left(),mColliderBox.Top() };
	EndPoint = { mColliderBox.Right(),mColliderBox.Top() };
	LinesData.push_back({ StartPoint, EndPoint });
	
	return LinesData;
}

std::vector<q_engine::IDType>& q_engine::RectCollisionComponent::GetResolvingEntityData()
{
	return mResolvingEntityData;
}

std::vector<q_engine::IDType>& q_engine::RectCollisionComponent::GetDynamicResolvingEntityData()
{
	return mDynamicResolvingEntityData;
}

void q_engine::RectCollisionComponent::AddResolveEntityID(IDType EntityID)
{
	mResolvingEntityData.push_back(EntityID);
}

void q_engine::RectCollisionComponent::AddDynamicResolveEntityID(IDType EntityID)
{
	mDynamicResolvingEntityData.push_back(EntityID);
}

Json::Value q_engine::RectCollisionComponent::Serialize()
{
	Json::Value value;
	value["Position"].append(mColliderBox.bottomLeft.x);
	value["Position"].append(mColliderBox.bottomLeft.y);
	value["Width"] = mWidth;
	value["Height"] = mHeight;
	value["IsDynamic"] = mIsDynamic;
	value["IsAdaptPhysics"] = mIsAdaptPhysic;
	value["IsTrigger"] = mIsTrigger;
	return value;
}

void q_engine::RectCollisionComponent::Deserialize(Json::Value& jsonValue)
{
	glm::vec2 position = { jsonValue["Position"][0].asFloat(),jsonValue["Position"][1].asFloat() };
	mWidth = jsonValue["Width"].asFloat();
	mHeight = jsonValue["Height"].asFloat();
	mIsDynamic = jsonValue["IsDynamic"].asBool();
	mIsAdaptPhysic = jsonValue["IsAdaptPhysics"].asBool();
	mIsTrigger = jsonValue["IsTrigger"].asBool();

	SetColliderBox(position, mWidth, mHeight);
	SetOldColliderBox(position, mWidth, mHeight);
}

Rect q_engine::StaticRectCollisionComponent::GetColliderBox() const
{
	return mColliderBox;
}

void q_engine::StaticRectCollisionComponent::SetIsTrigger(bool IsTrigger)
{
	mIsTrigger = IsTrigger;
}

 void q_engine::StaticRectCollisionComponent::SetColliderBoxSize(float width, float height)
{
	 float PosX = mColliderBox.Left();
	 float PosY = mColliderBox.Bottom();
	 mColliderBox = { {PosX,PosY},{ PosX + width, PosY + height} };
	mWidth = width;
	mHeight = height;
}

bool q_engine::StaticRectCollisionComponent::IsTrigger() const
{
	return mIsTrigger;
}

void q_engine::StaticRectCollisionComponent::SetTileOneWay(bool IsTileOneWay)
{
	mIsTileOneWay = IsTileOneWay;
}

bool q_engine::StaticRectCollisionComponent::IsTileOneWay() const
{
	return mIsTileOneWay;
}

bool q_engine::StaticRectCollisionComponent::IsCanDestroyed() const
{
	return mIsCanDestroyed;
}

bool q_engine::StaticRectCollisionComponent::IsLadder() const
{
	return mIsLadder;
}

void q_engine::StaticRectCollisionComponent::SetCanDestroyed(bool IscanDestroyed)
{
	mIsCanDestroyed = IscanDestroyed;
}

std::vector<std::pair<glm::vec2, glm::vec2>> q_engine::StaticRectCollisionComponent::GetCollisionBoxline() const
{
	std::vector<std::pair<glm::vec2, glm::vec2>> LinesData;
	glm::vec2 StartPoint = mColliderBox.bottomLeft;
	glm::vec2 EndPoint = { mColliderBox.Left(), mColliderBox.Top() };
	LinesData.push_back({ StartPoint, EndPoint });

	StartPoint = { mColliderBox.Right(), mColliderBox.Top() };
	EndPoint = { mColliderBox.Right(), mColliderBox.Bottom() };
	LinesData.push_back({ StartPoint, EndPoint });
	StartPoint = mColliderBox.bottomLeft;
	EndPoint = { mColliderBox.Right(),mColliderBox.Bottom() };
	LinesData.push_back({ StartPoint, EndPoint });
	StartPoint = { mColliderBox.Left(),mColliderBox.Top() };
	EndPoint = { mColliderBox.Right(),mColliderBox.Top() };
	LinesData.push_back({ StartPoint, EndPoint });

	return LinesData;
}

void q_engine::RegisterStaticRectCollisionComponent(sol::state& state)
{
	sol::usertype<q_engine::StaticRectCollisionComponent> StaticRectCollisionComponent = state.new_usertype<q_engine::StaticRectCollisionComponent>("StaticRectCollisionComponent");
	StaticRectCollisionComponent.set_function("IsLadder", &q_engine::StaticRectCollisionComponent::IsLadder);
}


