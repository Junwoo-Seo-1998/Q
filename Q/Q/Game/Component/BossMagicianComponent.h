/*
	File Name: BossMagicianComponent.h
	Project Name: Q
	Author(s):
		Primary: Minjae kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "../Engine/ECS/Component.h"
#include "Engine/GameState.h"

class BossMagicianComponent : public q_engine::Component<BossMagicianComponent>
{
	float mCurrentTime = 0.f;
	float mInputCheckTime = 0.f;
	int mIndicatorNum = 0;
	bool mIsShooting = false;
	q_engine::IDType mPlayerID;
	
	std::list<glm::vec2> mPathToPlayer;
	
public:
	BossMagicianComponent(q_engine::IDType playerID) : mPlayerID(playerID) {}

	constexpr static float SPEED = 250.f;
	
	float GetTime() const { return mCurrentTime; }
	void UpdateTime(double dt) { mCurrentTime += static_cast<float>(dt); };
	void ResetTime() { mCurrentTime = 0.f; }

	float GetInputCheckTime() const { return mInputCheckTime; }
	void UpdateInputCheckTime(double dt) { mInputCheckTime += static_cast<float>(dt); };
	void ResetInputCheckTime() { mInputCheckTime = 0.f; }
	
	bool IsShooting() const { return mIsShooting; }
	void SetShooting(bool isShooting) { mIsShooting = isShooting; }

	void ResetIndicatorNum() { mIndicatorNum = 0; }
	void IncreaseIndicatorNum() { ++mIndicatorNum; }
	void DecreaseIndicatorNum() { --mIndicatorNum; }
	int GetIndicatorNum() const { return mIndicatorNum; }

	std::list<glm::vec2>& GetPathToPlayer() { return mPathToPlayer; }
	q_engine::IDType GetPlayerID() { return mPlayerID; }
};
