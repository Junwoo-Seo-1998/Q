/*
	File Name: BossAssassinComponent.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Component.h"
#include "Engine/GameState.h"

class BossAssassinComponent : public q_engine::Component<BossAssassinComponent>
{
	float mCurrentTime = 0.f;
	
	int mIndicatorNum = 0;
	bool mIsShooting = false;
	
	q_engine::IDType mWeaponID = q_engine::INVALID_ID;
	q_engine::IDType mPlayerID = q_engine::INVALID_ID;

public:
	BossAssassinComponent(q_engine::IDType playerID) : mPlayerID(playerID) {}

	constexpr static float SPEED = 250.f;

	float GetTime() const { return mCurrentTime; }
	void UpdateTime(double dt) { mCurrentTime += static_cast<float>(dt); };
	void ResetTime() { mCurrentTime = 0.f; }
	

	bool IsShooting() const { return mIsShooting; }
	void SetShooting(bool isShooting) { mIsShooting = isShooting; }

	void ResetIndicatorNum() { mIndicatorNum = 0; }
	void IncreaseIndicatorNum() { ++mIndicatorNum; }
	void DecreaseIndicatorNum() { --mIndicatorNum; }
	int GetIndicatorNum() const { return mIndicatorNum; }

	q_engine::IDType GetPlayerID() const { return mPlayerID; }
	void SetWeaponID(q_engine::IDType weaponID) { mWeaponID = weaponID; }
	q_engine::IDType GetWeaponID() const { return mWeaponID; }
};
