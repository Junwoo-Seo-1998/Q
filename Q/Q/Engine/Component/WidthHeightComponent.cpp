/*
	File Name: WidthHeightComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/


#include"WidthHeightComponent.h"

q_engine::WidthHeightComponent::WidthHeightComponent()
{
}

q_engine::WidthHeightComponent::WidthHeightComponent(float Width, float Height, bool UI_relative) : mWidth(static_cast<float>(Width)), mHeight(static_cast<float>(Height)), m_is_UI_relative(UI_relative)
{
}

q_engine::WidthHeightComponent::WidthHeightComponent(glm::vec2 WidthHeight, bool UI_relative) : WidthHeightComponent(WidthHeight.x, WidthHeight.y, UI_relative)
{

	
}

void q_engine::WidthHeightComponent::SetWidth(float Width)
{
	mWidth = Width;
}

void q_engine::WidthHeightComponent::SetHeight(float Height)
{
	mHeight = Height;
}

void q_engine::WidthHeightComponent::SetWidthHeight(float Width, float Height)
{
	mWidth = Width;
	mHeight = Height;
}

void q_engine::WidthHeightComponent::SetWidthHeight(glm::vec2 WidthHeight)
{
	mWidth = WidthHeight.x;
	mHeight = WidthHeight.y;
}

glm::vec2 q_engine::WidthHeightComponent::GetWidthHeight() const
{
	return glm::vec2{ mWidth,mHeight };
}

Json::Value q_engine::WidthHeightComponent::Serialize()
{
	Json::Value value;
	value["Width"] = mWidth;
	value["Height"] = mHeight;
	return value;
}

void q_engine::WidthHeightComponent::Deserialize(Json::Value& jsonValue)
{
	mWidth = jsonValue["Width"].asFloat();
	mHeight=jsonValue["Height"].asFloat();
}

void q_engine::RegisterWidthHeightComponent(sol::state& state)
{
	sol::usertype<WidthHeightComponent> widthHeightComponent = state.new_usertype<WidthHeightComponent>("WidthHeightComponent");

	widthHeightComponent.set_function("SetWidthHeight", sol::resolve<void(glm::vec2)>(&WidthHeightComponent::SetWidthHeight));
	widthHeightComponent.set_function("GetWidthHeight", &WidthHeightComponent::GetWidthHeight);
	widthHeightComponent.set_function("GetWidth", &WidthHeightComponent::GetWidth<float>);
	widthHeightComponent.set_function("GetHeight", &WidthHeightComponent::GetHeight<float>);
	widthHeightComponent.set_function("GetMiddlePos", &WidthHeightComponent::GetMiddlePos);
}
