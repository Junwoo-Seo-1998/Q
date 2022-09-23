/*
	File Name: EnemyStateComponent.h
	Project Name: Q
	Author(s):
		Primary: Minjae Kyung
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <glm/vec2.hpp>
#include <functional>

#include "../../Engine/System/TimerManagerSystem.h"
#include "../Engine/ECS/Component.h"
#include "EnemyStateSyscomps/IEnemyState.h"
#include "../Engine/Core/ThreadPool.h"
#include "EnemyStateSyscomps/EnemySettingState.h"
#include "../Engine/Loader/Loader.h"
#include "Engine/Types.h"
#define PI 3.1415926535f



class EnemyStateComponent : public q_engine::Component<EnemyStateComponent>
{
public:


	enum class EnemyType
	{
		Warrior,
		Tanker,
		Scout,
		Engineer,
		Turret,
		FlameThrower,
		Bomber,
		BossMagician,
		BossAssassin
	};
private:
	EnemyType mEnemyType;

	q_engine::TimerID mTimerID = q_engine::INVALID_ID;
	unsigned mSearchingTime = 5;
	
	glm::vec2 mMaxVelocity = { -1,-1 };
	float mSpeedMultiplier = 1;
	inline static const float mRunningMultiplier = 1.5;

	glm::vec2 mLastSeenPlayerPos = {0,0 };
	glm::vec2 mMaxJumpVelocity = { -1,-1 };
	glm::vec2 mJumpVelocity = { -1,-1 };

	bool mIsJumping = false;
	
	float mSightDistance =1.f;
	float mSightFOV = -1.f;
	float mLookingDirection = 0.f;
	bool mIsLookingPlayer = false;

	bool mIsLeftPressed = false, mIsRightPressed = true;
	bool mIsMoveByInput = true;

	int mWeaponID = -1;
	q_engine::IDType mEnemyStateID = EnemySettingState::GetStateID();

	bool mIsDead = false;

	std::list<glm::vec2> mPathToPlayer;
	
	enum class StateStatus
	{
		Load,
		Update,
		UnLoad,
	};

	std::function<void(q_engine::GameState*, q_engine::IDType)> mLoadState = EnemySettingState::Load;
	std::function<void(q_engine::GameState*, q_engine::IDType, double)> mUpdateState = EnemySettingState::Update;
	std::function<void(q_engine::GameState*, q_engine::IDType)> mUnloadState = EnemySettingState::Unload;
	std::function<void(q_engine::GameState*, q_engine::IDType, q_engine::TriggerEvent*)> mStateOnTriggerEvent = EnemySettingState::OnTriggerEvent;
	std::function<void(q_engine::GameState*, q_engine::IDType, q_engine::RayTriggerEvent*)> mStateOnRayEvent = EnemySettingState::OnRayEvent;
	std::function<void(q_engine::GameState*, q_engine::IDType)> mStateOnAttackEvent = EnemySettingState::OnAttackEvent;
	
public:
	EnemyStateComponent() = default;

	EnemyStateComponent(EnemyType enemyType, q_engine::IDType weaponID,
		const glm::vec2& maxVelocity, const glm::vec2& maxJumpVelocity,
		float sightDistance = 500, float sightFOV = PI * 1.f / 4.f)
	 : mEnemyType(enemyType), mWeaponID(weaponID), mMaxVelocity(maxVelocity), mMaxJumpVelocity(maxJumpVelocity),
		mSightDistance(sightDistance), mSightFOV(sightFOV)
	{
		mTimerID = q_engine::TimerManagerSystem::IssueTimer(0.f, false);
		q_engine::TimerManagerSystem::GetTimer(mTimerID).ResetAndPause(0.f);
	}
	~EnemyStateComponent()
	{
		q_engine::TimerManagerSystem::YieldTimer(mTimerID);	    
	}

	[[nodiscard]] EnemyType GetEnemyType() const { return mEnemyType; }

	[[nodiscard]] q_engine::IDType GetWeaponID() const { return mWeaponID; }
	void SetWeaponID(q_engine::IDType weaponID) { mWeaponID = weaponID; }
	
	[[nodiscard]] const glm::vec2 GetMaxVelocity() const { return mMaxVelocity * mSpeedMultiplier; }

	void ResetSpeedMultiplier() { mSpeedMultiplier = 1; }
	void SetSpeedMultiplier(float multiplier) { mSpeedMultiplier = multiplier; }
	void Run() { mSpeedMultiplier = mRunningMultiplier; }

	const glm::vec2& GetLastSeenPlayerPosition() const { return mLastSeenPlayerPos; }
	void UpdateLastSeenPlayerPosition(glm::vec2 pos) { mLastSeenPlayerPos = pos; }

	bool isLookingPlayer() { return mIsLookingPlayer; }
	void SetLookingPlayer(bool looking) { mIsLookingPlayer = looking; }
	void UpdateLookingDirection(float direction) { mLookingDirection = direction; }
	float GetLookingDirection() const { return mLookingDirection; }
	float GetSightDistance() const { return mSightDistance; }
	float GetSightFOV() const { return mSightFOV; }
	[[nodiscard]] q_engine::IDType GetCurrentStateID() const { return mEnemyStateID; }

	[[nodiscard]] bool isJumping() const { return mIsJumping; }
	void Jump(glm::vec2 jumpVelocity) { mJumpVelocity = jumpVelocity; mIsJumping = true; }
	void Land() { mJumpVelocity = glm::vec2(0,0); mIsJumping = false; }
	[[nodiscard]] const glm::vec2& GetJumpVelocity() const { return mJumpVelocity; }
	[[nodiscard]] const glm::vec2& GetMaxJumpVelocity() const { return mMaxJumpVelocity; }

	std::list<glm::vec2>& GetPathToPlayer() { return mPathToPlayer; }

	void SetMoveByInput(bool moveBy) { mIsMoveByInput = moveBy; }
	bool isMoveByInput() const { return mIsMoveByInput; }
	void MoveRight() 
	{ 
		mIsLeftPressed = false; mIsRightPressed = true;
	}
	void MoveLeft() 
	{ 
		mIsLeftPressed = true; mIsRightPressed = false; 
	}
	void StopMove() 
	{ 
		mIsLeftPressed = false; mIsRightPressed = false; 
	}
	void ReverseMove() 
	{ 
		mIsLeftPressed = !mIsLeftPressed; mIsRightPressed = !mIsRightPressed; 
	}
	bool isMovingRight() { return mIsRightPressed; }
	bool isMovingLeft() { return mIsLeftPressed; }

	q_engine::TimerID GetTimerID() { return mTimerID; }

	bool isDead() { return mIsDead; }
	void KillThis() { mIsDead = true; }
	
	void LoadCurrentState(q_engine::GameState* controlledState) { mLoadState(controlledState, GetEntityID()); }
	void UpdateCurrentState(q_engine::GameState* controlledState, double dt) { mUpdateState(controlledState, GetEntityID(), dt); }
	void UnloadCurrentState(q_engine::GameState* controlledState) { mUnloadState(controlledState, GetEntityID()); }
	void CurrentStateOnTriggerEvent(q_engine::GameState* controlledState, q_engine::TriggerEvent* eventObj) { mStateOnTriggerEvent(controlledState, GetEntityID(), eventObj); }
	void CurrentStateOnRayEvent(q_engine::GameState* controlledState, q_engine::RayTriggerEvent* eventObj) { mStateOnRayEvent(controlledState, GetEntityID(), eventObj); }
	void CurrentStateOnAttackEvent(q_engine::GameState* controlledState) { mStateOnAttackEvent(controlledState, GetEntityID()); }

	template<typename T>
	void SetCurrentState(q_engine::GameState* controlledState)//Warning to use
	{
		std::lock_guard lock(q_engine::ThreadPool::GetMutex());
		
		mUnloadState(controlledState, GetEntityID());
		
		mEnemyStateID = T::GetStateID();

		mLoadState = T::Load;
		mUpdateState = T::Update;
		mUnloadState = T::Unload;
		mStateOnTriggerEvent = T::OnTriggerEvent;
		mStateOnRayEvent = T::OnRayEvent;
		mStateOnAttackEvent = T::OnAttackEvent;
		mLoadState(controlledState, GetEntityID());
	}

	bool isUpdated = false;
	
	inline Json::Value Serialize();
	inline void Deserialize(Json::Value& jsonValue);
};


inline Json::Value EnemyStateComponent::Serialize()
{
	Json::Value value;
	value["WeaponID"] = mWeaponID;
	value["MaxVelocity"].append(mMaxVelocity.x);
	value["MaxVelocity"].append(mMaxVelocity.y);
	value["MaxJumpVelocity"].append(mJumpVelocity.x);
	value["MaxJumpVelocity"].append(mJumpVelocity.y);
	return value;
}

inline void EnemyStateComponent::Deserialize(Json::Value& jsonValue)
{
	//load saved weapon entt id
	mWeaponID = q_engine::Loader::GetLoadedIDWithSavedID(jsonValue["WeaponID"].asInt());
	mMaxVelocity.x = jsonValue["MaxVelocity"][0].asFloat();
	mMaxVelocity.y = jsonValue["MaxVelocity"][1].asFloat();
	mJumpVelocity.x=jsonValue["JumpVelocity"][0].asFloat();
	mJumpVelocity.y=jsonValue["JumpVelocity"][1].asFloat();
}



