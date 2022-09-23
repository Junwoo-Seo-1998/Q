/*
	File Name: GravityComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"

namespace q_engine
{
	class GravityComponent : public q_engine::Component<GravityComponent>
	{
	public:
		GravityComponent() = default;
		explicit GravityComponent(double gravity);
		[[nodiscard]] void SetGravity(double gravity);

		[[nodiscard]] double GetGravity() const;

		void SetIsOnGround(bool isOnGround);

		[[nodiscard]] bool IsOnGround() const;
		bool GetIsOnground() const;
		virtual Json::Value Serialize() override;
		virtual void Deserialize(Json::Value& jsonValue) override;
	private:
		double mGravity{ 2000.0 };
		bool mIsonGround = false;
	};



}
