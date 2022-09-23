/*
    File Name: TransformComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TransformComponent.h"


#include <iostream>
#include <glm/gtc/constants.hpp>
#include "sol/sol.hpp"

namespace q_engine
{
	void TransformComponent::SetRotationRadian(const float angleRadian) noexcept
	{
		constexpr float TWO_PI = glm::two_pi<float>();

		mRotationRadian = angleRadian;

		while (mRotationRadian >= TWO_PI)
		{
			mRotationRadian -= TWO_PI;
		}
		while (mRotationRadian < 0)
		{
			mRotationRadian += TWO_PI;
		}

		mIsCacheDirty = true;
	}


	void TransformComponent::AddRotationRadian(const float angleRadian) noexcept
	{
		SetRotationRadian(mRotationRadian + angleRadian);
	}


	const glm::mat3& TransformComponent::GetOrCalculateTransformMatrix() const noexcept
	{
		if (mIsCacheDirty)
		{
			const float COS = cos(mRotationRadian);
			const float SIN = sin(mRotationRadian);
			
			mTransformMatrix = glm::mat3{ 1, 0, 0, 0, 1, 0, mPosition.x, mPosition.y, 1 }
				* glm::mat3{ COS, SIN, 0, -SIN, COS, 0, 0, 0, 1 }
				* glm::mat3{ mScale.x, 0, 0, 0, mScale.y, 0, 0, 0, 1 };
			
			mIsCacheDirty = false;
		}

		return mTransformMatrix;
	}

	Json::Value TransformComponent::Serialize()
	{
		Json::Value out;
		out["position"].append(mPosition.x);
		out["position"].append(mPosition.y);

		out["scale"].append(mScale.x);
		out["scale"].append(mScale.y);

		out["rotation"] = mRotationRadian;
		return out;
	}

	void TransformComponent::Deserialize(Json::Value& jsonValue)
	{
		mPosition.x = jsonValue["position"][0].asFloat();
		mPosition.y = jsonValue["position"][1].asFloat();

		mScale.x = jsonValue["scale"][0].asFloat();
		mScale.y = jsonValue["scale"][1].asFloat();

		mRotationRadian = jsonValue["rotation"].asFloat();
	}


	void RegisterTransformComponent(sol::state& state)
	{
		sol::usertype<TransformComponent> transformComponent = state.new_usertype<TransformComponent>("TransformComponent");
		
		transformComponent.set_function("GetPosition",&TransformComponent::GetPosition);
		transformComponent.set_function("SetPosition", sol::overload(sol::resolve<void(glm::vec2)>(&TransformComponent::SetPosition), sol::resolve<void(float, float)>(&TransformComponent::SetPosition)));
		transformComponent.set("Position", sol::property(&TransformComponent::GetPosition, sol::resolve<void(glm::vec2)>(&TransformComponent::SetPosition)));

		transformComponent.set_function("GetScale", &TransformComponent::GetScale);
		transformComponent.set_function("SetScale", sol::overload(sol::resolve<void(glm::vec2)>(&TransformComponent::SetScale), sol::resolve<void(float, float)>(&TransformComponent::SetScale)));
		transformComponent.set("Scale", sol::property(&TransformComponent::GetScale, sol::resolve<void(glm::vec2)>(&TransformComponent::SetScale)));

		transformComponent.set_function("GetRotation", &TransformComponent::GetRotationRadian);
		transformComponent.set_function("SetRotation", &TransformComponent::SetRotationRadian);
		transformComponent.set("Rotation", sol::property(&TransformComponent::GetRotationRadian, &TransformComponent::SetRotationRadian));
	}
}
