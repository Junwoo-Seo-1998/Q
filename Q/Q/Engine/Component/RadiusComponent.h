/*
	File Name: RadiusComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Component.h"
namespace q_engine
{
	class CircleComponent : public Component<CircleComponent>
	{
	public:
		CircleComponent() = default;
		CircleComponent(float Radius) : mRadius(Radius) {}
		float GetRadius() const
		{
			return mRadius;
		}
	private:
		float mRadius;
	};
	
}