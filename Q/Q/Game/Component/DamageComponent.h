/*
	File Name: DamageComponent.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>

#include "../Engine/ECS/Component.h"
#include "sol/sol.hpp"

#include "Engine/BitShift.h"


enum DamageType
{
	NONE=BIT(0),
	//category 
	DIRECTDAMAGE=BIT(1),
	DOTDAMGAE=BIT(2),
	//category

	//types
	POISONDAMAGE=BIT(3) | DOTDAMGAE,  //dot damage category
	//types

};
void RegisterBulletTypeEnum(sol::state& state);
void RegisterDamageComponent(sol::state& state);


class Damage
{
public:
	Damage() = delete;
	
	Damage(int damage_value, double set_time, DamageType bullet_type);
	Damage(int damage_value, const glm::vec2& damagedPos);

	double GetSettingTime() const;
	int GetDamageValue() const;
	glm::vec2 GetDamagedPosition();
	void TimerUpdate(double dt);
	bool IsTimerDone() const;
	void ResetRemainTime();
	DamageType GetBulletType() const;
	double GetRemainTime() const;
	double GetInterval() const;
	double GetRemainDamageCount() const;
	void SetRemainDamageCount(int a);
	
	bool IsInCategory(DamageType category) const;

private:
	int mDamageValue;
	double mSettingTime = 0;
	double mRemainTime = 0;
	double mRemainDamageCounttime = 0;
	DamageType mBulletType = DIRECTDAMAGE;
	glm::vec2 mDamagedPosition;
};


class DamageComponent  : public q_engine::Component<DamageComponent>
{
public:
	DamageComponent(int damage_value, double set_time,DamageType bullet_type);
	DamageComponent(int damage_value, const glm::vec2& damagedPos, DamageType type = DamageType::DIRECTDAMAGE);

	void AddDamage(int damage_value, double set_time, DamageType bullet_type);
	void AddDamage(int damage_value, const glm::vec2& damagedPos);
	void SetIsComponentDeleted(bool deleted);
	bool GetIsComponentDeleted() const;
	
	std::list<Damage>::iterator begin();
	std::list<Damage>::iterator end();
	std::list<Damage>& GetDamageList();
private:
	bool mIsDeleted = false;
	std::list<Damage> mDamages;
};

