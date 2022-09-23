/*
    File Name: AnimationUpdateDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "AnimationUpdateDrawSystem.h"

#include "../Component/AnimationComponent.h"
#include "../Component/CameraShakeComponent.h"
#include "../Component/TransformComponent.h"
#include "../Entity/CameraEntity.h"
#include "../GameStateManager.h"
#include "../Profiler.h"
#include "../../Game/Entity/WeaponEntity.h"
#include "../Component/OffsetComponent.h"


namespace q_engine
{
	void AnimationUpdateDrawSystem::Update(const double deltaTime)
	{
		SCOPE_PROFILING("AnimationUpdateDrawSystem::Update")

		const auto& currentState = m_controlled_state;
        auto& animationComponents = currentState->GetComponents<AnimationComponent>();
        auto& transformComponents = currentState->GetComponents<TransformComponent>();
		auto& offsetComponents = currentState->GetComponents<OffsetComponent>();
		
		const IDType camID = currentState->GetEntities<CameraEntity>().Front().GetEntityID();
		const TransformComponent& camTransform = transformComponents[camID];
		const CameraShakeComponent& shakeComponent = currentState->GetComponents<CameraShakeComponent>()[camID];

		const glm::mat3 world2NDC = CameraEntity::CalculateWorldToNDCTransform(
			camTransform.GetPosition() + shakeComponent.GetPositionOffset(),
			camTransform.GetRotationRadian() + shakeComponent.GetAngleOffset(),
			camTransform.GetScale()
		);
		for (auto& animation : animationComponents)
		{
			animation.Update(static_cast<float>(deltaTime));
		}
		for (auto& animation : animationComponents)
		{
			const auto entityID = animation.GetEntityID();
			const TransformComponent& transform = transformComponents[entityID];

			const OffsetComponent& offset = offsetComponents[entityID];

			animation.Draw((world2NDC * transform.GetOrCalculateTransformMatrix() * offset.GetOffSetMatrix() * offset.GetHotSpotMatrix() * offset.GetRotationAndScaleMatrix()));
			
		}
	}
}
