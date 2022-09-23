/*
    File Name: HealthComponent.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "HealthComponent.h"
#include "../../Engine/Engine.h"
#include "../../Engine/Component/CameraShakeComponent.h"
#include "../../Engine/GameStateManager.h"
#include "../../Engine/System/TimerManagerSystem.h"


HealthComponent::HealthComponent()
{
}

HealthComponent::~HealthComponent()
{
	if(mInvincibleTimerID != -1)
	{
		q_engine::TimerManagerSystem::YieldTimer(mInvincibleTimerID);
		mInvincibleTimerID = q_engine::INVALID_ID;
	}
}

HealthComponent::HealthComponent(const health_type& max_health): HealthComponent(max_health, max_health)
{

}

HealthComponent::HealthComponent(const health_type& max_health, const health_type& init_health):
	m_max_health(max_health), m_cur_health(init_health)
{
	mInvincibleTimerID = q_engine::TimerManagerSystem::IssueTimer(0.f, false);
}

HealthComponent::health_type HealthComponent::GetMaxHealth() const
{
	return m_max_health;
}

HealthComponent::health_type HealthComponent::GetCurHealth() const
{
	return m_cur_health;
}

void HealthComponent::ChangeCurHealth(health_type changing)
{
	if (m_cur_health > 0)
	{
		m_cur_health += changing;
		if(m_cur_health < 0)
		{
			m_cur_health = 0;
		}
		if (m_cur_health > m_max_health)
			m_cur_health = m_max_health;
	}
}

float HealthComponent::AsPercentage() const
{
	return static_cast<float>(m_cur_health) / m_max_health;
}

bool HealthComponent::IsInvincible() const
{
	if(q_engine::TimerManagerSystem::HasTimer(mInvincibleTimerID))
	{
		q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(mInvincibleTimerID);
		return !(timer.IsOnTimeOut());
	}
	return false;
}

void HealthComponent::SetInvincible(float time)
{
	if(time <= 0.f)
	{
		throw std::runtime_error("time should be more than zero");
	}
	if (!q_engine::TimerManagerSystem::HasTimer(mInvincibleTimerID))
	{
		mInvincibleTimerID = q_engine::TimerManagerSystem::IssueTimer(0.f, false);
	}
	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(mInvincibleTimerID);
	const float cur_timer_maxTime = timer.GetOriginalTime();

	if(time > cur_timer_maxTime)
	{
		timer.ResetAndResume(time);
	}
	else
	{
		float curTimerRemaningTime = timer.GetRemainingTime();
		if (curTimerRemaningTime < 0.f)
		{
			curTimerRemaningTime = 0.f;
		}
		const float targetTime = std::clamp(curTimerRemaningTime + time, 0.f, cur_timer_maxTime);
		timer.ResetAndResume(targetTime);
	}
}

void HealthComponent::SetInvincible(bool state)
{
	q_engine::Timer& timer = q_engine::TimerManagerSystem::GetTimer(mInvincibleTimerID);

	if(timer.GetOriginalTime() == 0.f)
	{
		timer.Reset(1.f);
	}

	if(timer.GetRemainingTime() <= 0.f)
	{
		timer.Reset();
	}
	timer.SetPaused(state);
}

Json::Value HealthComponent::Serialize()
{
	Json::Value value;
	value["cur_health"] = m_cur_health;
	value["max_health"] = m_max_health;
	return value;
}

void HealthComponent::Deserialize(Json::Value& jsonValue)
{
	m_cur_health = jsonValue["cur_health"].asInt();
	m_max_health = jsonValue["max_health"].asInt();
}
