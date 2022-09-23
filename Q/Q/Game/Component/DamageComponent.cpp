/*
	File Name: DamageComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "DamageComponent.h"

void RegisterBulletTypeEnum(sol::state& state)
{
	std::initializer_list<std::pair<sol::string_view, DamageType>> BulletTypeEnum =
	{
		{"DIRECTDAMAGE", DamageType::DIRECTDAMAGE},
		{"DOTDAMGAE", DamageType::DOTDAMGAE},
		{"POISONDAMAGE", DamageType::POISONDAMAGE},
	};

	state.new_enum<DamageType, false>("EBulletType", BulletTypeEnum);
}

DamageComponent::DamageComponent(int damage_value, double set_time, DamageType bullet_type)
{
	AddDamage(damage_value, set_time, bullet_type);
}

DamageComponent::DamageComponent(int damage_value, const glm::vec2& damagedPos, [[maybe_unused]] DamageType bullet_type)
{
	AddDamage(damage_value, damagedPos);
}


void DamageComponent::AddDamage(int damage_value, double set_time, DamageType bullet_type)
{
	mDamages.emplace_back(damage_value, set_time, bullet_type);
}

void DamageComponent::AddDamage(int damage_value, const glm::vec2& damagedPos)
{
	mDamages.emplace_back(damage_value, damagedPos);
}

void DamageComponent::SetIsComponentDeleted(bool deleted)
{
	mIsDeleted = deleted;
}

bool DamageComponent::GetIsComponentDeleted() const
{
	return mIsDeleted;
}


std::list<Damage>::iterator DamageComponent::begin()
{
	return mDamages.begin();
}

std::list<Damage>::iterator DamageComponent::end()
{
	return mDamages.end();
}

std::list<Damage>& DamageComponent::GetDamageList()
{
	return mDamages;
}

void RegisterDamageComponent(sol::state& state)
{
	sol::usertype<DamageComponent> damageComponent = state.new_usertype<DamageComponent>("DamageComponent");
	
	damageComponent.set_function("AddDamage", sol::resolve<void(int, double, DamageType)>(&DamageComponent::AddDamage));


}

Damage::Damage(int damage_value, double set_time, DamageType bullet_type)
	: mDamageValue(damage_value), mSettingTime(set_time), mRemainTime(set_time), mBulletType(bullet_type), mRemainDamageCounttime(set_time)
{
}

Damage::Damage(int damage_value, const glm::vec2& damagedPos)
	: mDamageValue(damage_value), mDamagedPosition(damagedPos)
{
}


double Damage::GetSettingTime() const
{
	return mSettingTime;
}

int Damage::GetDamageValue() const
{
	return mDamageValue;
}

glm::vec2 Damage::GetDamagedPosition()
{
	return mDamagedPosition;
}

void Damage::TimerUpdate(double dt)
{
	mRemainTime -= dt;
}

bool Damage::IsTimerDone() const
{
	if (mRemainTime <= 0)
	{
		return true;
	}
	return false;

}

void Damage::ResetRemainTime()
{
	mRemainTime = mSettingTime;
}

DamageType Damage::GetBulletType() const
{
	return mBulletType;
}

double Damage::GetRemainTime() const
{
	return mRemainTime;
}

double Damage::GetInterval() const
{
	return mSettingTime / 3;
}

double Damage::GetRemainDamageCount() const
{
	return mRemainDamageCounttime;
}

void Damage::SetRemainDamageCount(int a)
{
	mRemainDamageCounttime = a;
}

bool Damage::IsInCategory(DamageType category) const
{
	return mBulletType & category;
}
