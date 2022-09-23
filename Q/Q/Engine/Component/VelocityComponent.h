/*
	File Name: VelocityComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"
#include <glm/vec2.hpp>

namespace sol
{
	class state;
}
namespace q_engine
{
	class VelocityComponent : public Component<VelocityComponent>
	{
		friend class EntityDebugger;
	public:
		VelocityComponent() = default;
		VelocityComponent(glm::vec2 vel) : mVelocity(vel) {}
		
		glm::vec2 GetVelocity() const;
		void SetVelocity(glm::vec2 velocity);
		void SetVelocityX(float velocityX);
		void SetVelocityY(float velocityY);
		void SetAngularVelocity(float radian);
		float GetAngularVelocity() const;
		void AddAngularVelocity(float radian);
		void AddVelocity(glm::vec2 velocity);
		void AddVelocityX(float velocityX);
		void AddVelocityY(float velocityY);
		void SubtractVelocityX(float velocityX);
		void SubtractVelocityY(float velocityY);
		float GetVelocityX() const;
		float GetVelocityY() const;
		void UpdateVelocity(float x, float y);
		
		[[nodiscard]] bool IsWindowRelative() const { return is_window_relative; };
	private:
		glm::vec2 mVelocity{ 0,0 };
		float angular_velocity = 0.f;
		bool is_window_relative = false;
	};

	void RegisterVelocityComponent(sol::state& state);
}
