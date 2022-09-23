/*
	File Name: PlayerMovingSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Hoseob Jeong
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayerMovingSystem.h"

#include <iostream>

#include "../Engine/Engine.h"
#include "../Engine/GameStateManager.h"

#include "CollisionSystem.h"
#include "../../Game/EObjectZDepth.h"

#include "../Component/DragAccelComponent.h"
#include "../Component/ObjectStateComponent.h"
#include "../Component/UserInputComponent.h"
#include "../Component/VelocityComponent.h"
#include "../Component/WidthHeightComponent.h"
#include "../Component/OffsetComponent.h"
#include "../Component/AnimationComponent.h"

#include "../System/TimerManagerSystem.h"
#include "../ECS/InputManager.h"
#include "../Engine/Event/TriggerEvent.h"
#include "Component/HealthComponent.h"
#include "Entity/MapEntity.h"
#include "../Game/Component/NewWeaponComponent.h"
#include "Engine/Component/FollowComponent.h"
#include "Entity/SimpleEntity.h"
#include "PlayTestTrace.h"
#include "../Game/Component/DamageComponent.h"
#include "Entity/BombEntity.h"
#include "Entity/WeaponEntity.h"


void q_engine::MovingSystem::Load()
{
	Binding* bind = (new Binding("PlayerJumpKeyPressed"));
	bind->BindInput(InputType::KEY_DOWN, KeyCode(Key::UP_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerJumpKeyPressed", &MovingSystem::MovingJump, this);

	bind = (new Binding("PlayerJumpKeyReleased"));
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::UP_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerJumpKeyReleased", &MovingSystem::NotMovingJump, this);

	bind = (new Binding("PlayerMoveRightKeyPressed"));
	bind->BindInput(InputType::KEY_DOWN, KeyCode(Key::RIGHT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveRightKeyPressed", &MovingSystem::MovingRight, this);

	bind = { new Binding("PlayerMoveRightKeyReleased") };
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::RIGHT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveRightKeyReleased", &MovingSystem::NotMovingRight, this);

	bind = (new Binding("PlayerMoveLeftKeyPressed"));
	bind->BindInput(InputType::KEY_DOWN, KeyCode(Key::LEFT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveLeftKeyPressed", &MovingSystem::MovingLeft, this);

	bind = (new Binding("PlayerMoveLeftKeyReleased"));
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::LEFT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveLeftKeyReleased", &MovingSystem::NotMovingLeft, this);

	bind = (new Binding("PlayerMoveDownPressed"));
	bind->BindInput(InputType::KEY_DOWN, KeyCode(Key::DOWN_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveDownPressed", &MovingSystem::MovingDown, this);

	bind = (new Binding("PlayerMoveDownReleased"));
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::DOWN_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveDownReleased", &MovingSystem::NotMovingDown, this);


	bind = (new Binding("PlayerMoveSlidingLeftPressed"));
	bind->BindInput(InputType::KEY_PRESS, KeyCode(Key::DOWN_ARROW));
	bind->BindInput(InputType::KEY_DOWN, KeyCode(Key::LEFT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveSlidingLeftPressed", &MovingSystem::MovingSlidingLeft, this);

	bind = (new Binding("PlayerMoveSlidingLeftReleased"));
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::LEFT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveSlidingLeftReleased", &MovingSystem::NotMovingSlidingLeft, this);

	bind = (new Binding("PlayerMoveSlidingLeftReleased1"));
	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::DOWN_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveSlidingLeftReleased1", &MovingSystem::NotMovingSlidingLeft, this);

	bind = (new Binding("PlayerMoveSlidingRightPressed"));
	bind->BindInput(InputType::KEY_PRESS, KeyCode(Key::DOWN_ARROW));
	bind->BindInput(InputType::KEY_DOWN, KeyCode(Key::RIGHT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveSlidingRightPressed", &MovingSystem::MovingSlidingRight, this);

	bind = (new Binding("PlayerMoveSlidingRightReleased"));

	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::RIGHT_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveSlidingRightReleased", &MovingSystem::NotMovingSlidingRight, this);

	bind = (new Binding("PlayerMoveSlidingRightReleased1"));

	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::DOWN_ARROW));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerMoveSlidingRightReleased1", &MovingSystem::NotMovingSlidingRight, this);

	bind = (new Binding("PlayerInteractKeyPressed"));

	bind->BindInput(InputType::KEY_PRESS, KeyCode(Key::X));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerInteractKeyPressed", &MovingSystem::InteractionKeyPress, this);

	bind = (new Binding("PlayerInteractKeyReleased"));

	bind->BindInput(InputType::KEY_RELEASE, KeyCode(Key::X));
	Engine::GetEngineComponent<InputManager>().AddBinding(bind);
	Engine::GetEngineComponent<InputManager>().AddCallback("PlayerInteractKeyReleased", &MovingSystem::NotInteractionKeyPress, this);
	

	
	mDashTimerID = TimerManagerSystem::IssueTimer(0.f, false);
	mState_Crawling.SetGameState(m_controlled_state);
	mState_Dashing.SetGameState(m_controlled_state);
	mState_Falling.SetGameState(m_controlled_state);
	mState_Running.SetGameState(m_controlled_state);
	mState_Jumping.SetGameState(m_controlled_state);
	mState_Skidding.SetGameState(m_controlled_state);
	mState_Sliding.SetGameState(m_controlled_state);
	mState_Idle.SetGameState(m_controlled_state);
	mState_Beeing.SetGameState(m_controlled_state);
	mState_Climbing.SetGameState(m_controlled_state);
	mState_ClimbingDown.SetGameState(m_controlled_state);
	mState_ClimbingUp.SetGameState(m_controlled_state);
	mState_IsDead.SetGameState(m_controlled_state);
	mState = &mState_Falling;
}

void q_engine::MovingSystem::Update(double dt)
{
	auto& UserInputComponents = m_controlled_state->GetComponents<UserInputComponent>();
	auto& VelocityComponents = m_controlled_state->GetComponents<VelocityComponent>();;
	auto& ObjectStateComponents = m_controlled_state->GetComponents<ObjectStateComponent>();
	auto& OffsetComponents = m_controlled_state->GetComponents<OffsetComponent>();
	auto& TransFormComponents = m_controlled_state->GetComponents<TransformComponent>();
	auto& NewWeaponComponents = m_controlled_state->GetComponents<NewWeaponComponent>().Front();




	for (UserInputComponent& player : UserInputComponents)
	{
		auto GetEntityID = player.GetEntityID();

		VelocityComponent& velocity = VelocityComponents[GetEntityID];
		ObjectStateComponent& objectstate = ObjectStateComponents[GetEntityID];
		OffsetComponent& offset = OffsetComponents[GetEntityID];


	
		if (mInit == true)
		{
			TransformComponent& transform = TransFormComponents[GetEntityID];
			mIdlePlayerHeightabs = abs(transform.GetScaleY());
			mInit = false;
		}
		
		switch (objectstate.GetPlayerMovingState())
		{
		case MovingState::IDLE:
			mState = &mState_Idle;
			break;
		case MovingState::FALLING:
			mState = &mState_Falling;
			break;
		case MovingState::SLIDEING:
			mState = &mState_Sliding;
			break;
		case MovingState::RUNNING:
			mState = &mState_Running;
			break;
		case MovingState::JUMPING:
			mState = &mState_Jumping;
			break;
		case MovingState::SKIDDING:
			mState = &mState_Skidding;
			break;
		case MovingState::DASHING:
			mState = &mState_Dashing;
			break;
		case MovingState::BEEING:
			mState = &mState_Beeing;
			break;
		case MovingState::CLIMBING:
			mState = &mState_Climbing;
			break;
		case MovingState::CLIMBINGDOWN:
			mState = &mState_ClimbingDown;
			break;
		case MovingState::CLIMBINGUP:
			mState = &mState_ClimbingUp;
			break;
		case MovingState::ISDEAD:
			mState = &mState_IsDead;
			break;
		}


		if (mIsStateChange == true && objectstate.GetPlayerMovingState() != mPreviousMovingState)
		{
			mState->Init(GetEntityID, velocity, this);
			Debugger::GetLogger().AddMessage(mState->GetMovingStateString());
		}

		mState->Update(GetEntityID, velocity, this, dt);
		mPreviousMovingState = objectstate.GetPlayerMovingState();
		mPreviousWeapon = NewWeaponComponents.GetCurrentWeapon();
		mState->CheckState(GetEntityID, this);
		mIsonLadder = false;

		
		if(mIsMovingRightPressed == true)
		{
			offset.SetFlipped(false);
			objectstate.setIsFlipped(false);

		}
		else if(mIsMovingLeftPressed == true)
		{
			offset.SetFlipped(true);
			objectstate.setIsFlipped(true);

		}

		if (velocity.GetVelocityY() != 0 || velocity.GetVelocityX() != 0)
			if (objectstate.GetIsGroundOntheHead() == true)
			{
				objectstate.SetIsGroundOntheHead(false);
			}
	}

	if (NewWeaponComponents.GetIsAttacking() == true)
	{
		mWasAttack = true;
	}
	else
	{
		mWasAttack = false;
	}


}

void q_engine::MovingSystem::Unload()
{
	auto& input_manager = Engine::GetEngineComponent<InputManager>();

	input_manager.RemoveBinding("PlayerJumpKeyPressed");
	input_manager.RemoveBinding("PlayerJumpKeyReleased");
	input_manager.RemoveBinding("PlayerMoveRightKeyPressed");
	input_manager.RemoveBinding("PlayerMoveRightKeyReleased");
	input_manager.RemoveBinding("PlayerMoveLeftKeyPressed");
	input_manager.RemoveBinding("PlayerMoveLeftKeyReleased");
	input_manager.RemoveBinding("PlayerMoveDownPressed");
	input_manager.RemoveBinding("PlayerMoveDownReleased");
	input_manager.RemoveBinding("PlayerMoveSlidingLeftPressed");
	input_manager.RemoveBinding("PlayerMoveSlidingLeftReleased");
	input_manager.RemoveBinding("PlayerMoveSlidingLeftReleased1");
	input_manager.RemoveBinding("PlayerMoveSlidingRightPressed");
	input_manager.RemoveBinding("PlayerMoveSlidingRightReleased");
	input_manager.RemoveBinding("PlayerMoveSlidingRightReleased1");
	input_manager.RemoveBinding("PlayerInteractKeyReleased");
	input_manager.RemoveBinding("PlayerInteractKeyPressed");

	input_manager.RemoveCallback("PlayerJumpKeyPressed");
	input_manager.RemoveCallback("PlayerJumpKeyReleased");
	input_manager.RemoveCallback("PlayerMoveRightKeyPressed");
	input_manager.RemoveCallback("PlayerMoveRightKeyReleased");
	input_manager.RemoveCallback("PlayerMoveLeftKeyPressed");
	input_manager.RemoveCallback("PlayerMoveLeftKeyReleased");
	input_manager.RemoveCallback("PlayerMoveDownPressed");
	input_manager.RemoveCallback("PlayerMoveDownReleased");
	input_manager.RemoveCallback("PlayerMoveSlidingLeftPressed");
	input_manager.RemoveCallback("PlayerMoveSlidingLeftReleased");
	input_manager.RemoveCallback("PlayerMoveSlidingLeftReleased1");
	input_manager.RemoveCallback("PlayerMoveSlidingRightPressed");
	input_manager.RemoveCallback("PlayerMoveSlidingRightReleased");
	input_manager.RemoveCallback("PlayerMoveSlidingRightReleased1");
	input_manager.RemoveCallback("PlayerInteractKeyReleased");
	input_manager.RemoveCallback("PlayerInteractKeyPressed");
}




void q_engine::MovingSystem::MovingJump()
{
	mIsJumpingPressed = true;
}

void q_engine::MovingSystem::MovingRight()
{
	mIsMovingRightPressed = true;
}

void q_engine::MovingSystem::MovingLeft()
{
	mIsMovingLeftPressed = true;
}


void q_engine::MovingSystem::MovingDown()
{
	mIsMovingDownPressed = true;
}

void q_engine::MovingSystem::NotMovingLeft()
{
	mIsMovingLeftPressed = false;
}

void q_engine::MovingSystem::NotMovingRight()
{
	mIsMovingRightPressed = false;
}

void q_engine::MovingSystem::NotMovingJump()
{
	mIsJumpingPressed = false;
}

void q_engine::MovingSystem::NotMovingDown()
{
	mIsMovingDownPressed = false;

}

void q_engine::MovingSystem::MovingSlidingLeft()
{
	mIsMovingSlideLeftPressed = true;
}

void q_engine::MovingSystem::NotMovingSlidingLeft()
{
	mIsMovingSlideLeftPressed = false;
}

void q_engine::MovingSystem::MovingSlidingRight()
{
	mIsMovingSlideRightPressed = true;
}

void q_engine::MovingSystem::NotMovingSlidingRight()
{
	mIsMovingSlideRightPressed = false;
}

void q_engine::MovingSystem::InteractionKeyPress()
{
	mIsPlayerInteractPressed = true;
}

void q_engine::MovingSystem::NotInteractionKeyPress()
{
	mIsPlayerInteractPressed = false;
}

void q_engine::MovingSystem::OnEvent(IEvent* eventName)
{
	if (dynamic_cast<q_engine::TriggerEvent*>(eventName) != nullptr)
	{
		q_engine::TriggerEvent* triggerEvent = reinterpret_cast<q_engine::TriggerEvent*>(eventName);

		if (triggerEvent->mTriggerEntityTypeID == q_engine::GetTypeID<DamageTileEntity>())
		{
			if (triggerEvent->mCollideWithEntityTypeID == q_engine::GetTypeID<PlayerEntity>())
			{
				if(!m_controlled_state->HasComponent(q_engine::GetTypeID<DamageComponent>(),triggerEvent->mCollideWithEntityID))
				{
					m_controlled_state->AddComponent<DamageComponent>(q_engine::GetTypeID<PlayerEntity>(), triggerEvent->mCollideWithEntityID, 14, 2.0, DamageType::DOTDAMGAE);
				}
                PlayTestTrace::LogPlayerHitByMessage("DamageTile");
			}
		}
		if (triggerEvent->mTriggerEntityTypeID == q_engine::GetTypeID<MapEntity>())
		{
			if (triggerEvent->mCollideWithEntityTypeID == q_engine::GetTypeID<PlayerEntity>())
			{
				StaticRectCollisionComponent& ladder = m_controlled_state->GetComponents<StaticRectCollisionComponent>()[triggerEvent->mTriggerEntityID];
				if (ladder.IsLadder())
				{
					mLadderPosition = ladder.GetColliderBox();
					mIsonLadder = true;
				}
				
			}
		}
		if (triggerEvent->mTriggerEntityTypeID == q_engine::GetTypeID<PlayerEntity>())
		{
			if (triggerEvent->mCollideWithEntityTypeID == q_engine::GetTypeID<EnemyEntity>())
			{
				if (!m_controlled_state->HasComponent(q_engine::GetTypeID<DamageComponent>(), triggerEvent->mCollideWithEntityID))
				{
					if(mIsDiveAttackDone == false)
					{
						m_controlled_state->AddComponent<DamageComponent>(q_engine::GetTypeID<EnemyEntity>(), triggerEvent->mCollideWithEntityID, BombEntity::DAMAGE, 0.0, DamageType::DIRECTDAMAGE);
						mIsDiveAttackDone = true;
					}
				}

			}
		}

		if (triggerEvent->mTriggerEntityTypeID == q_engine::GetTypeID<WeaponEntity>())
		{
			if (triggerEvent->mCollideWithEntityTypeID == q_engine::GetTypeID<EnemyEntity>())
			{
				auto& player = m_controlled_state->GetComponents<UserInputComponent>().Front();
				IDType playerentityid = player.GetEntityID();
				auto& playerstate = m_controlled_state->GetComponents<ObjectStateComponent>()[playerentityid];
				
				auto& weaponComponent = m_controlled_state->GetComponents<NewWeaponComponent>()[triggerEvent->mTriggerEntityID];

				if(playerstate.GetPlayerMovingState() == MovingState::ISDEAD)
				{
					return;
				}

				if (!weaponComponent.GetIsAttacking() || weaponComponent.IsRangedWeapon())
					return;
				
				if (!m_controlled_state->HasComponent(q_engine::GetTypeID<DamageComponent>(), triggerEvent->mCollideWithEntityID))
				{
					m_controlled_state->AddComponent<DamageComponent>(q_engine::GetTypeID<EnemyEntity>(), triggerEvent->mCollideWithEntityID, BombEntity::DAMAGE, 0.f, DamageType::DIRECTDAMAGE);
				}
				else
				{
					m_controlled_state->GetComponents<DamageComponent>()[triggerEvent->mCollideWithEntityID].AddDamage(BombEntity::DAMAGE, 0.f, DamageType::DIRECTDAMAGE);
				}

			}
		}
	}

}


bool q_engine::MovingSystem::IsOnLadder() const
{
	return mIsonLadder;
}

void MovingSystem::SetIsOnLadder(bool isladder)
{
	mIsonLadder = isladder;
}

void q_engine::MovingSystem::State_Idle::Init(IDType EntityID, VelocityComponent& velocity_comp, MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	velocity_comp.SetVelocity({ 0,0 });
}

void q_engine::MovingSystem::State_Idle::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
}

void q_engine::MovingSystem::State_Idle::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	StateSubSystem::CheckState(EntityID, moving_system);
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];
	if (gravity.GetIsOnground() == false)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		moving_system->mIsStateChange = true;
	}
	else if (moving_system->mIsMovingRightPressed == true || moving_system->mIsMovingLeftPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::RUNNING);
		moving_system->mIsStateChange = true;
	}
	else if (moving_system->mIsJumpingPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::JUMPING);
		moving_system->mIsStateChange = true;
	}
	if (gravity.IsOnGround() == false)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		moving_system->mIsStateChange = true;
	}

	if (moving_system->IsOnLadder() && moving_system->mIsJumpingPressed == true)
	{
		transform.SetPositionX(moving_system->mLadderPosition.Left() + moving_system->mLadderPosition.Size().x / 8.f);
		gravity.SetIsOnGround(true);
		objectstate.ChangeMovingState(MovingState::CLIMBING);
		moving_system->mIsStateChange = true;
	}
}



void q_engine::MovingSystem::State_Skidding::Init([[maybe_unused]] IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	moving_system->mIsStateChange = false;

}

void q_engine::MovingSystem::State_Skidding::Update(IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp,
	[[maybe_unused]] MovingSystem* moving_system, double dt)
{
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];


	float addedGetDragAccel = dragaccel.GetDrag() + dragaccel.GetAccel();
	if (velocity.GetVelocityX() > 0.f)
	{
		velocity.AddVelocityX(-addedGetDragAccel * static_cast<float>(dt));
	}
	else if (velocity.GetVelocityX() < 0.f)
	{
		velocity.AddVelocityX(addedGetDragAccel * static_cast<float>(dt));
	}


}

void q_engine::MovingSystem::State_Skidding::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];



	if (moving_system->mIsJumpingPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::JUMPING);
		moving_system->mIsStateChange = true;
	}

	if (gravity.GetIsOnground() == true && (velocity.GetVelocityX() >= -20.f && velocity.GetVelocityX() <= 20.f))
	{
		objectstate.ChangeMovingState(MovingState::IDLE);
		moving_system->mIsStateChange = true;
	}
	
	if (moving_system->mIsMovingLeftPressed == true)
	{
		if (velocity.GetVelocityX() <= 0.f)
		{ 
			objectstate.ChangeMovingState(MovingState::RUNNING);
			moving_system->mIsStateChange = true;
		}
	}
	else if (moving_system->mIsMovingRightPressed == true)
	{
		if (velocity.GetVelocityX() >= 0.f)
		{
			objectstate.ChangeMovingState(MovingState::RUNNING);
			moving_system->mIsStateChange = true;
		}
	}

	if (gravity.IsOnGround() == false)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		moving_system->mIsStateChange = true;
	}


}

void q_engine::MovingSystem::State_Running::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	moving_system->mIsStateChange = false;
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("player-walking", true);
}

void q_engine::MovingSystem::State_Running::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];

	moving_system->mIsPlayerInteractPressed = false;
	moving_system->UpdateXvelocity(objectstate, velocity_comp, dragaccel, gravity, dt);
}

void q_engine::MovingSystem::State_Running::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];


	if (moving_system->mIsMovingLeftPressed && velocity.GetVelocityX() > 0)
	{
		objectstate.ChangeMovingState(MovingState::SKIDDING);
		moving_system->mIsStateChange = true;
		Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	}
	if (moving_system->mIsMovingRightPressed && velocity.GetVelocityX() < 0)
	{
		objectstate.ChangeMovingState(MovingState::SKIDDING);
		moving_system->mIsStateChange = true;
		Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	}
	if (velocity.GetVelocityX() == 0.f)
	{
		objectstate.ChangeMovingState(MovingState::IDLE);
		moving_system->mIsStateChange = true;
		Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	}

	if (moving_system->mIsMovingSlideLeftPressed == true || moving_system->mIsMovingSlideRightPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::SLIDEING);
		moving_system->mIsStateChange = true;
		Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	}
	if (moving_system->mIsJumpingPressed == true && objectstate.GetIsGroundOntheHead() == false)
	{
		objectstate.ChangeMovingState(MovingState::JUMPING);
		moving_system->mIsStateChange = true;
		Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	}
	if (gravity.IsOnGround() == false)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		moving_system->mIsStateChange = true;
		Engine::GetEngineComponent<SoundComponent>().StopSoundLoopEffect("player-walking");
	}

	StateSubSystem::CheckState(EntityID, moving_system);
}

void q_engine::MovingSystem::State_Sliding::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	moving_system->mIsStateChange = false;
	TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];


	moving_system->mIdlePlayerWidthabs = std::abs(transform.GetScaleX());
	widthheight.SetHeight(objectstate.GetObjectHeightSize() * 0.5f);




	if (velocity_comp.GetVelocityX() >= 0.f)
	{
		velocity_comp.SetVelocityX(dragaccel.GetMaxSlideVelocity());
	}

	if (velocity_comp.GetVelocityX() < 0.f)
	{
		velocity_comp.SetVelocityX(-dragaccel.GetMaxSlideVelocity());
	}
}

void q_engine::MovingSystem::State_Sliding::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];

	if (velocity.GetVelocityX() > 0.f)
	{
		velocity.AddVelocity({ -dragaccel.GetDrag() * dt,0.f });
	}
	if (velocity.GetVelocityX() < 0.f)
	{
		velocity.AddVelocity({ dragaccel.GetDrag() * dt, 0 });
	}
}

void q_engine::MovingSystem::State_Sliding::CheckState(IDType EntityID, MovingSystem* moving_system)
{

	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];
	if (velocity.GetVelocityX() > -dragaccel.GetSlideVelocity() && velocity.GetVelocityX() < dragaccel.GetSlideVelocity())
	{
		 if (moving_system->mIsJumpingPressed == true)
		{
			objectstate.ChangeMovingState(MovingState::JUMPING);
			moving_system->mIsStateChange = true;
			if (objectstate.GetIsGroundOntheHead() == false)
			{
				widthheight.SetHeight(objectstate.GetObjectHeightSize());
			}
		}
		else
		{
			objectstate.ChangeMovingState(MovingState::SKIDDING);
			moving_system->mIsStateChange = true;
			if (objectstate.GetIsGroundOntheHead() == false)
			{
				widthheight.SetHeight(objectstate.GetObjectHeightSize());
			}
		}


	}

	if (gravity.IsOnGround() == false)
	{
		if (objectstate.GetIsGroundOntheHead() == false)
		{
			widthheight.SetHeight(objectstate.GetObjectHeightSize());
		}
		objectstate.ChangeMovingState(MovingState::FALLING);
		moving_system->mIsStateChange = true;
	}
	StateSubSystem::CheckState(EntityID, moving_system);
}

void q_engine::MovingSystem::State_Crawling::Init(IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	moving_system->mIsStateChange = false;
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	if (moving_system->mPreviousMovingState != MovingState::SLIDEING)
	{

		moving_system->mIdlePlayerWidthabs = std::abs(transform.GetScaleX());
		if (moving_system->mIsMovingDownPressed == true)
		{
			widthheight.SetHeight(objectstate.GetObjectHeightSize() * 0.5f);
			transform.SetScaleY(moving_system->mIdlePlayerHeightabs / 2.f);
		}
	}

}

void q_engine::MovingSystem::State_Crawling::Update(IDType EntityID, VelocityComponent& velocity,
	MovingSystem* moving_system, double dt)
{
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];

	moving_system->UpdateXvelocity(objectstate, velocity, dragaccel, gravity, dt);
}

void q_engine::MovingSystem::State_Crawling::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];

	if (moving_system->mIsMovingDownPressed == false)
	{
		if (objectstate.GetIsGroundOntheHead() == false)
		{
			objectstate.ChangeMovingState(MovingState::RUNNING);
			widthheight.SetHeight(objectstate.GetObjectHeightSize());
			transform.SetScaleY(moving_system->mIdlePlayerHeightabs);
			moving_system->mIsStateChange = true;
		}
	}
	if (gravity.GetIsOnground() == false)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		widthheight.SetHeight(objectstate.GetObjectHeightSize());
		transform.SetScaleY(moving_system->mIdlePlayerHeightabs);
		moving_system->mIsStateChange = true;
	}

}

void q_engine::MovingSystem::State_Jumping::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	//player - jumping
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];
	Engine::GetEngineComponent<SoundComponent>().PlaySoundEffect("player-jumping", false);
	velocity_comp.SetVelocityY(dragaccel.GetJumpVelocity());
}

void q_engine::MovingSystem::State_Jumping::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];
	moving_system->UpdateXvelocity(objectstate, velocity_comp, dragaccel, gravity, dt);
}

void q_engine::MovingSystem::State_Jumping::CheckState(IDType EntityID, MovingSystem* moving_system)
{

	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];
	if (InputManager::IsKeyReleased(Key::UP_ARROW) || velocity.GetVelocityY() <= 0.0f)
	{
		velocity.SetVelocityY(0.f);
		objectstate.ChangeMovingState(MovingState::FALLING);
		moving_system->mIsStateChange = true;
	}
	if(moving_system->mIsJumpingPressed == true && moving_system->IsOnLadder() == true)
	{
		transform.SetPositionX(moving_system->mLadderPosition.Left() + moving_system->mLadderPosition.Size().x / 8.f);
		gravity.SetIsOnGround(true);
		velocity.SetVelocityX(0.f);
		objectstate.ChangeMovingState(MovingState::CLIMBING);
		moving_system->mIsStateChange = true;
	}
	StateSubSystem::CheckState(EntityID, moving_system);
}

void q_engine::MovingSystem::State_Beeing::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	moving_system->mIsStateChange = false;
	RectCollisionComponent& rectcollison = GetCurrentstate()->GetComponents<RectCollisionComponent>()[EntityID];
	if (moving_system->mIsMovingLeftPressed == true)
	{
		velocity_comp.SetVelocity(glm::vec2(-300.f, -1000.f));
	}
	else if (moving_system->mIsMovingRightPressed == true)
	{
		velocity_comp.SetVelocity(glm::vec2(300.f, -1000.f));
	}
	rectcollison.SetIsTrigger(true);
}

void q_engine::MovingSystem::State_Beeing::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
}

void q_engine::MovingSystem::State_Beeing::CheckState(IDType EntityID, MovingSystem* moving_system)
{

	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	RectCollisionComponent& rectcollison = GetCurrentstate()->GetComponents<RectCollisionComponent>()[EntityID];
	if (gravity.GetIsOnground() == true && velocity.GetVelocityX() == 0.f)
	{
		objectstate.ChangeMovingState(MovingState::IDLE);
		rectcollison.SetIsTrigger(false);
		moving_system->mIsStateChange = true;
		moving_system->mIsDiveAttackDone = false;
	}
	else if (gravity.GetIsOnground() == true && velocity.GetVelocityX() != 0.f)
	{
		objectstate.ChangeMovingState(MovingState::RUNNING);
		rectcollison.SetIsTrigger(false);
		moving_system->mIsStateChange = true;
		moving_system->mIsDiveAttackDone = false;
	}


	StateSubSystem::CheckState(EntityID, moving_system);
}

void q_engine::MovingSystem::State_Climbing::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	moving_system->mIsStateChange = false;
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	widthheight.SetWidth(40.f);
	
}

void q_engine::MovingSystem::State_Climbing::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);


}

void q_engine::MovingSystem::State_Climbing::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	
	if (moving_system->mIsJumpingPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::CLIMBINGUP);
		moving_system->mIsStateChange = true;
	}
	if (moving_system->mIsMovingDownPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::CLIMBINGDOWN);
		moving_system->mIsStateChange = true;
	}

	
	if (moving_system->IsOnLadder() == false)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		widthheight.SetWidth(objectstate.GetObjectWidthSize());
		moving_system->mIsPlayerInteractPressed = false;
		moving_system->mIsStateChange = true;
	}
	StateSubSystem::CheckState(EntityID, moving_system);

}

void q_engine::MovingSystem::State_ClimbingDown::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];

	gravity.SetIsOnGround(true);
	if (moving_system->mIsMovingDownPressed == true)
	{
		velocity_comp.SetVelocity(glm::vec2(0.f, -150.f));
	}
	
}

void q_engine::MovingSystem::State_ClimbingDown::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);

	gravity.SetIsOnGround(true);
	if (moving_system->mIsMovingDownPressed == true)
	{
		velocity_comp.SetVelocity(glm::vec2(0.f, -150.f));
	}
	if (moving_system->mIsJumpingPressed == false && moving_system->mIsMovingDownPressed == false)
	{
		velocity_comp.SetVelocity(glm::vec2(0.f, 0.f));
	}
	
}

void q_engine::MovingSystem::State_ClimbingDown::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];
	
	if (moving_system->mIsJumpingPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::CLIMBINGUP);
		moving_system->mIsStateChange = true;
	}
	if (moving_system->IsOnLadder() == false || moving_system->mIsPlayerInteractPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		widthheight.SetWidth(objectstate.GetObjectWidthSize());
		moving_system->mIsPlayerInteractPressed = false;
		moving_system->mIsStateChange = true;
	}
	StateSubSystem::CheckState(EntityID, moving_system);
	
}

void q_engine::MovingSystem::State_ClimbingUp::Init(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);
	if (moving_system->mIsJumpingPressed == true)
	{
		velocity_comp.SetVelocity(glm::vec2(0.f, 150.f));
	}
}

void q_engine::MovingSystem::State_ClimbingUp::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	gravity.SetIsOnGround(true);
	
	if (moving_system->mIsJumpingPressed == true)
	{
		velocity_comp.SetVelocity(glm::vec2(0.f, 150.f));
	}
	if (moving_system->mIsJumpingPressed == false && moving_system->mIsMovingDownPressed == false)
	{
		velocity_comp.SetVelocity(glm::vec2(0.f, 0.f));
	}
}

void q_engine::MovingSystem::State_ClimbingUp::CheckState(IDType EntityID, MovingSystem* moving_system)
{

	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	WidthHeightComponent& widthheight = GetCurrentstate()->GetComponents<WidthHeightComponent>()[EntityID];

	if (InputManager::IsKeyPressed(Key::DOWN_ARROW) == true)
	{
		objectstate.ChangeMovingState(MovingState::CLIMBINGDOWN);
		moving_system->mIsStateChange = true;
	}
	if (moving_system->IsOnLadder() == false || moving_system->mIsPlayerInteractPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::FALLING);
		widthheight.SetWidth(objectstate.GetObjectWidthSize());
		moving_system->mIsPlayerInteractPressed = false;
		moving_system->mIsStateChange = true;
	}
	StateSubSystem::CheckState(EntityID, moving_system);
}

void q_engine::MovingSystem::State_IsDead::Init(IDType EntityID, VelocityComponent& velocity_comp,
	[[maybe_unused]] MovingSystem* moving_system)
{
	velocity_comp.SetVelocity(glm::vec2{ 0, 0 });
	AnimationComponent& animation = GetCurrentstate()->GetComponents<AnimationComponent>()[EntityID];
	const IDType animationHash = q_engine::get_hash_id("NewPlayerIsDeadAnim");
	animation.SetAnimation(animationHash, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_PLAYER_OBJ));
	PlayTestTrace::LogDeadMessage("Check above");

}

void q_engine::MovingSystem::State_IsDead::Update([[maybe_unused]] IDType EntityID, VelocityComponent& velocity_comp,
	[[maybe_unused]] MovingSystem* moving_system, [[maybe_unused]] double dt)
{
	velocity_comp.SetVelocity(glm::vec2{ 0, 0 });
}

void q_engine::MovingSystem::State_IsDead::CheckState([[maybe_unused]] IDType EntityID, [[maybe_unused]] MovingSystem* moving_system)
{
	
}

void q_engine::MovingSystem::State_Falling::Init(IDType EntityID, VelocityComponent& velocity_comp,
                                                 MovingSystem* moving_system)
{
	StateSubSystem::Init(EntityID, velocity_comp, moving_system);


}

void q_engine::MovingSystem::State_Falling::Update(IDType EntityID, VelocityComponent& velocity_comp,
	MovingSystem* moving_system, double dt)
{
	StateSubSystem::Update(EntityID, velocity_comp, moving_system, dt);
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	DragAccelComponent& dragaccel = GetCurrentstate()->GetComponents<DragAccelComponent>()[EntityID];

	moving_system->UpdateXvelocity(objectstate, velocity_comp, dragaccel, gravity, dt);
	if (moving_system->mPreviousMovingState == MovingState::CRAWLING || moving_system->mPreviousMovingState == MovingState::SLIDEING)
	{
		TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];
		transform.SetScaleY(moving_system->mIdlePlayerHeightabs);
	}
}

void q_engine::MovingSystem::State_Falling::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	GravityComponent& gravity = GetCurrentstate()->GetComponents<GravityComponent>()[EntityID];
	TransformComponent& transform = GetCurrentstate()->GetComponents<TransformComponent>()[EntityID];
	if (gravity.GetIsOnground() == true && velocity.GetVelocityX() == 0.f)
	{
		objectstate.ChangeMovingState(MovingState::IDLE);
		moving_system->mIsStateChange = true;
	}
	else if (gravity.GetIsOnground() == true && velocity.GetVelocityX() != 0.f)
	{
		objectstate.ChangeMovingState(MovingState::RUNNING);
		moving_system->mIsStateChange = true;
	}
	else if (moving_system->mIsMovingSlideLeftPressed == true || moving_system->mIsMovingSlideRightPressed == true)
	{
		objectstate.ChangeMovingState(MovingState::BEEING);
		moving_system->mIsStateChange = true;
	}
	if (moving_system->mIsJumpingPressed == true && moving_system->IsOnLadder() == true)
	{
		transform.SetPositionX(moving_system->mLadderPosition.Left() + moving_system->mLadderPosition.Size().x / 8.f);
		gravity.SetIsOnGround(true);
		velocity.SetVelocityX(0.f);
		objectstate.ChangeMovingState(MovingState::CLIMBING);
		moving_system->mIsStateChange = true;
	}
	StateSubSystem::CheckState(EntityID, moving_system);
}

void q_engine::MovingSystem::UpdateXvelocity(ObjectStateComponent& ObjectState_comp, VelocityComponent& Velocity_comp, DragAccelComponent& DragAccel_comp, GravityComponent& Gravity_comp, double dt)
{

	if (mIsMovingLeftPressed == true)
	{
		if (ObjectState_comp.GetPlayerMovingState() != MovingState::CRAWLING)
		{
			Velocity_comp.AddVelocity({ -DragAccel_comp.GetAccel() * dt, 0.f });
			if (Velocity_comp.GetVelocityX() < -DragAccel_comp.GetMaxVelocity())
			{
				Velocity_comp.SetVelocityX(-DragAccel_comp.GetMaxVelocity());
			}
		}
		else
		{
			Velocity_comp.AddVelocity({ -DragAccel_comp.GetCrawlingVelocity() * dt, 0.f });
			if (Velocity_comp.GetVelocityX() < -DragAccel_comp.GetMaxCrawlingVelocity())
			{
				Velocity_comp.SetVelocityX(-DragAccel_comp.GetMaxCrawlingVelocity());
			}
		}
	}

	if (mIsMovingRightPressed == true)
	{
		if (ObjectState_comp.GetPlayerMovingState() != MovingState::CRAWLING)
		{
			Velocity_comp.AddVelocity({ DragAccel_comp.GetAccel() * dt, 0.f });
			if (Velocity_comp.GetVelocityX() > DragAccel_comp.GetMaxVelocity())
			{
				Velocity_comp.SetVelocityX(DragAccel_comp.GetMaxVelocity());
			}
		}
		else
		{
			Velocity_comp.AddVelocity({ DragAccel_comp.GetCrawlingVelocity() * dt, 0.f });
			if (Velocity_comp.GetVelocityX() > DragAccel_comp.GetMaxCrawlingVelocity())
			{
				Velocity_comp.SetVelocityX(DragAccel_comp.GetMaxCrawlingVelocity());
			}
		}
	}

	if (mIsMovingRightPressed == false && mIsMovingLeftPressed == false && mIsMovingSlideLeftPressed == false && mIsMovingSlideRightPressed == false)
	{
		if (Velocity_comp.GetVelocityX() > 0.f)
		{
			Velocity_comp.AddVelocity({ -DragAccel_comp.GetDrag() * dt,0.f });
		}
		if (Velocity_comp.GetVelocityX() < 0.f)
		{
			Velocity_comp.AddVelocity({ DragAccel_comp.GetDrag() * dt, 0 });
		}


		if ((Velocity_comp.GetVelocityX() > 0.f && Velocity_comp.GetVelocityX() < DragAccel_comp.GetDrag() * dt)
			|| (Velocity_comp.GetVelocityX() < 0.f && Velocity_comp.GetVelocityX() > -DragAccel_comp.GetDrag() * dt))
		{
			Velocity_comp.SetVelocityX(0);
		}


		if (Velocity_comp.GetVelocityY() != 0)
		{
			Gravity_comp.SetIsOnGround(false);
		}
	}
}

void q_engine::MovingSystem::State_Dashing::Init([[maybe_unused]] IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp,
	[[maybe_unused]] MovingSystem* moving_system)
{


}

void q_engine::MovingSystem::State_Dashing::Update([[maybe_unused]] IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp,
	[[maybe_unused]] MovingSystem* moving_system, [[maybe_unused]] double dt)
{


}

void q_engine::MovingSystem::State_Dashing::CheckState([[maybe_unused]] IDType EntityID, [[maybe_unused]] MovingSystem* moving_system)
{


}

void q_engine::MovingSystem::StateSubSystem::Init(IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp,
	MovingSystem* moving_system)
{
	moving_system->mIsStateChange = false;
	AnimationComponent& animation = GetCurrentstate()->GetComponents<AnimationComponent>()[EntityID];
	NewWeaponComponent& weapon = GetCurrentstate()->GetComponents<NewWeaponComponent>().Front();
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	
	std::string default = "Default";
	std::string current_weapon = weapon.GetCurrentWeapon();
	std::string initalize = "Initialize";
	std::string frontname = "NewPlayer";
	std::string lastname = "Anim";
	std::string Attack = "";
	std::string Start = "";

	if (weapon.GetIsAttacking() == true)
	{
		Attack = "Attack";
		std::cout << "anima :atack" << std::endl;
	}
	if (GetMovingStateString() == "Climbing" || GetMovingStateString() == "ClimbingUp" || GetMovingStateString() == "ClimbingDown" || GetMovingStateString() == "Beeing" || (GetMovingStateString() == "Sliding" && current_weapon == "Dagger"))
	{
		Attack = "";
	}
	if (GetMovingStateString() == "ClimbingUp" || GetMovingStateString() == "ClimbingDown")
	{
		if (velocity.GetVelocityY() == 0.f)
		{
			animation.Pause();
		}
		else
		{
			animation.Play();
		}
	}
	if(GetMovingStateString() == "Climbing")
	{
		Start = "Start";
	}

	const std::string animationName = frontname + GetMovingStateString() + current_weapon + Attack + Start + lastname;
	const IDType animationHash = q_engine::get_hash_id(animationName);
	animation.SetAnimation(animationHash, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_PLAYER_OBJ));
	moving_system->mCurrentAnimation = animationHash;
	moving_system->mIsAnimationChange = false;


}

void q_engine::MovingSystem::StateSubSystem::Update([[maybe_unused]] IDType EntityID, [[maybe_unused]] VelocityComponent& velocity_comp, MovingSystem* moving_system, [[maybe_unused]] double dt)
{
	NewWeaponComponent& weapon = GetCurrentstate()->GetComponents<NewWeaponComponent>().Front();
	std::string CurrentWeapon = weapon.GetCurrentWeapon();

	if(weapon.GetIsAttacking() == true && moving_system->mWasAttack == false)
	{
		moving_system->mIsAnimationChange = false;
	}
	if(moving_system->mPreviousWeapon != CurrentWeapon)
	{
		moving_system->mIsAnimationChange = false;
	}


	

	
}

void q_engine::MovingSystem::StateSubSystem::CheckState(IDType EntityID, MovingSystem* moving_system)
{
	AnimationComponent& animation = GetCurrentstate()->GetComponents<AnimationComponent>()[EntityID];
	NewWeaponComponent& weapon = GetCurrentstate()->GetComponents<NewWeaponComponent>().Front();
	VelocityComponent& velocity = GetCurrentstate()->GetComponents<VelocityComponent>()[EntityID];
	HealthComponent& health = GetCurrentstate()->GetComponents<HealthComponent>()[EntityID];
	ObjectStateComponent& objectstate = GetCurrentstate()->GetComponents<ObjectStateComponent>()[EntityID];
	std::string default = "Default";
	std::string initalize = "Initialize";
	std::string frontname = "NewPlayer";
	std::string lastname = "Anim";
	std::string Attack = "";
	std::string CurrentWeaponName = weapon.GetCurrentWeapon();
	std::string Start = "";

	
		if (weapon.GetIsAttacking() == true)
		{
			Attack = "Attack";
		}
		if (GetMovingStateString() == "Climbing" || GetMovingStateString() == "ClimbingUp" || GetMovingStateString() == "ClimbingDown" || GetMovingStateString() == "Beeing" || (GetMovingStateString() == "Sliding" && CurrentWeaponName == "Dagger"))
		{
			Attack = "";
		}
	
	if (GetMovingStateString() == "ClimbingUp" || GetMovingStateString() == "ClimbingDown")
	{
		if (velocity.GetVelocityY() == 0.f)
		{
			animation.Pause();
		}
		else
		{
			animation.Play();
		}
	}
	if (GetMovingStateString() == "Climbing")
	{
		Start = "Start";
	}

	if(moving_system->mIsAnimationChange == false)
	{
		const std::string animationName = frontname + GetMovingStateString() + CurrentWeaponName + Attack + Start + lastname;
		const IDType animationHash = q_engine::get_hash_id(animationName);
		animation.SetAnimation(animationHash, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_PLAYER_OBJ));
		moving_system->mIsAnimationChange = true;
	}

	if(health.GetCurHealth() <=0.f)
	{
		objectstate.ChangeMovingState(MovingState::ISDEAD);
		moving_system->mIsStateChange = true;
	}

}
