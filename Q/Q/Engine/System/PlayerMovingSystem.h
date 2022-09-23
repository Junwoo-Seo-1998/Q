/*
	File Name: PlayerMovingSystem.h
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include "../ECS/System.h"
#include "../Engine/Component/ObjectStateComponent.h"
#include "../Engine/Component/TransformComponent.h"
#include "../Engine/Component/VelocityComponent.h"
#include "../Engine/Component/GravityComponent.h"
#include "Engine/Core/Rect.h"

//IDLE,
//RUNNING,
//FALLING,
//SKIDDING,
//JUMPING,
//SLIDING

namespace q_engine{
	class DragAccelComponent;
	
class MovingSystem final : public System<MovingSystem>
{
public:
	MovingSystem() = delete;
	MovingSystem(GameState* state) : System<MovingSystem>(state) {}
	void Load() override;
	void Update(double dt) override;
	void Unload() override;
	void MovingJump();
	void MovingRight();
	void MovingLeft();
	void MovingDown();
	void NotMovingLeft();
	void NotMovingRight();
	void NotMovingJump();
	void NotMovingDown();
	void MovingSlidingLeft();
	void NotMovingSlidingLeft();
	void MovingSlidingRight();
	void NotMovingSlidingRight();
	void InteractionKeyPress();
	void NotInteractionKeyPress();
	void MovingSystem::OnEvent(IEvent* eventName) override;
	bool IsOnLadder() const;
	void SetIsOnLadder(bool isladder);

	std::string GetCurrentMovingState() const
    {
		return mState != nullptr ? mState->GetMovingStateString() : "InvalidStateCall";
	}
	
	class StateSubSystem
	{public:
		virtual ~StateSubSystem() = default;
		virtual void Init(IDType EntityID,VelocityComponent& velocity_comp,MovingSystem* moving_system);
		virtual void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt);
		virtual void CheckState(IDType EntityID, MovingSystem* moving_system);
		virtual void SetGameState(GameState* state)
		{
			mGamestate = state;
		}
		
		GameState* GetCurrentstate()
		{
			return mGamestate;
		}
		virtual std::string GetMovingStateString() = 0;

		const std::vector<std::string>& GetWeaponName() const
		{
			return mWeaponName;
		}
	private:
		GameState* mGamestate =nullptr;
		std::vector<std::string> mWeaponName = {"Gun", "Dagger"};
	};


private:

	class State_Idle : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Idle"; }
	};
	class State_Falling : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Falling"; }
	};
	class State_Skidding : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Skidding"; }
	};
	class State_Running : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID , MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Running"; }
	};
	class State_Sliding : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Sliding"; }
	};
	class State_Crawling : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Crawling"; }
	};
	class State_Dashing : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt) override;
		void CheckState(IDType EntityID,  MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Dashing"; }
	};
	class State_Jumping : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID,  MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Jumping"; }
	};
	class State_Beeing : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Beeing"; }
	};
	class State_Climbing : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "Climbing"; }
	};
	class State_ClimbingDown : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "ClimbingDown"; }
	};
	class State_ClimbingUp : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "ClimbingUp"; }
	};
	class State_IsDead : public StateSubSystem
	{
	public:
		void Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system) override;
		void Update(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system, double dt)override;
		void CheckState(IDType EntityID, MovingSystem* moving_system) override;
		std::string GetMovingStateString() override { return "IsDead"; }
	};
	
	void UpdateXvelocity(ObjectStateComponent& ObjectState_comp, VelocityComponent& Velocity_comp, DragAccelComponent& DragAccel_comp, GravityComponent& Gravity_comp, double dt);
	StateSubSystem* mState = nullptr;

	State_Idle mState_Idle;
	State_Crawling mState_Crawling;
	State_Dashing mState_Dashing;
	State_Falling mState_Falling;
	State_Skidding mState_Skidding;
	State_Running mState_Running;
	State_Sliding mState_Sliding;
	State_Jumping mState_Jumping;
	State_Beeing mState_Beeing;
	State_Climbing mState_Climbing;
	State_ClimbingDown mState_ClimbingDown;
	State_ClimbingUp mState_ClimbingUp;
	State_IsDead mState_IsDead;
	
	bool mIsJumpingPressed = false;
	bool mIsMovingRightPressed = false;
	bool mIsMovingLeftPressed = false;
	bool mIsMovingDownPressed = false;
	bool mIsMovingSlideLeftPressed = false;
	bool mIsMovingSlideRightPressed = false;
	bool mIsPlayerInteractPressed = false;
	bool mIsDiveAttackDone = false;
	
	bool mIsNotJumping = false;
	bool mIsJumping = false;
	float mPlayerCrawlHeightSize = 50.f;
	float mPlayerCrawlWidthSize = 100.f;
	bool mInit = true;
	bool mWasAttack = false;
	bool mIsStateChange = false;
	TimerID mDashTimerID = INVALID_ID;
	MovingState mMovingState = MovingState::IDLE;
	MovingState mPreviousMovingState = MovingState::IDLE;
	float mIdlePlayerHeightabs = 0.f;
	float mIdlePlayerWidthabs = 0.f;
	bool mIsAnimationChange = false;
	AnimationID mCurrentAnimation = INVALID_ID;
	bool mIsonLadder = false;
	std::string mPreviousWeapon;
	Rect mLadderPosition;
};
}
