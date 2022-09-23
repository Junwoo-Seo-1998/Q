/*
    File Name: HealthComponent.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include "sol/sol.hpp"

#include "../Engine/ECS/Component.h"
#include "Engine/Types.h"
#include "Component/DamageComponent.h"
class HealthComponent final : public q_engine::Component<HealthComponent>
{
public:
    using health_type = int;
    HealthComponent();// to be used by editor loading.
    ~HealthComponent();
    HealthComponent(const health_type& max_health);
    HealthComponent(const health_type& max_health, const health_type& init_health);

    [[nodiscard]] health_type GetMaxHealth() const;
    [[nodiscard]] health_type GetCurHealth() const;

    void ChangeCurHealth(health_type changing);
	
    [[nodiscard]] float AsPercentage() const;
    bool IsDead() const { return m_cur_health <= 0; }
    bool IsInvincible() const;
    void SetInvincible(float time);
    void SetInvincible(bool state);
    bool IsDamaged() const { return mIsDamaged; }
    void SetDamaged(bool isDamaged) { mIsDamaged = isDamaged; }
    virtual Json::Value Serialize() override;
    virtual void Deserialize(Json::Value& jsonValue) override;
private:
    friend class EntityDebugger;

    health_type m_max_health = 0;
    health_type m_cur_health = 0;

    q_engine::TimerID mInvincibleTimerID;
    bool mIsDamaged = false;
};

inline void RegisterHealthComponent(sol::state& state)
{
    sol::usertype<HealthComponent> healthComponent = state.new_usertype<HealthComponent>("HealthComponent");

    healthComponent.set_function("IsDead", &HealthComponent::IsDead);
    healthComponent.set_function("GetCurHealth", &HealthComponent::GetCurHealth);
    healthComponent.set_function("ChangeCurHealth", &HealthComponent::ChangeCurHealth);
    healthComponent.set_function("GetMaxHealth", &HealthComponent::GetMaxHealth);

}