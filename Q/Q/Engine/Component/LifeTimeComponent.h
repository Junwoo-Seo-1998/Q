#pragma once
/*
	File Name: LifeTimeComponent.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../ECS/Component.h"

namespace q_engine
{
	class LifeTimeComponent : public q_engine::Component<LifeTimeComponent>
	{
	public:
		LifeTimeComponent() = default;
		explicit LifeTimeComponent(float lifeTime) : mMaxLifeTime(lifeTime), mLifeTime(mMaxLifeTime) {}

		void SetLifeTme(float lifeTime)
		{
			mMaxLifeTime = lifeTime;
			mLifeTime = mMaxLifeTime;
		}

		void Update(double dt)
		{
			mLifeTime -= static_cast<float>(dt);
		}

		[[nodiscard]] double GetLifeTime() const
		{
			return mLifeTime;
		}

		[[nodiscard]] double GetLifeTimeAsPercentage() const
		{
			const double temp = mLifeTime / mMaxLifeTime;
			return temp > 0.f ? temp : 0.f;
		}

        [[nodiscard]] bool IsDead() const
        {
			return mLifeTime < 0.f;
		}

	private:
		float mMaxLifeTime = -1.f;
		float mLifeTime = -1.f;

	};



}
