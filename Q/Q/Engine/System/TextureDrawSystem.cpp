/*
    File Name: TextureDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TextureDrawSystem.h"

#include "../Component/CameraShakeComponent.h"
#include "../Component/TextureComponent.h"
#include "../Component/TransformComponent.h"
#include "../Entity/CameraEntity.h"
#include "../GameStateManager.h"
#include "../../Game/Entity/PhysicsTestEntities.h"
#include "../Component/OffsetComponent.h"
#include "../Debugger/Debugger.h"

#include "../Engine/Profiler.h"
namespace q_engine
{
	void TextureDrawSystem::Update([[maybe_unused]] double deltaTime)
	{
		SCOPE_PROFILING("TextureDrawSystem::Update")
		
		const auto& currentState = m_controlled_state;
        auto& textureComponents = currentState->GetComponents<TextureComponent>();
        auto& transformComponents = currentState->GetComponents<TransformComponent>();
		auto& OffsetComponents = currentState->GetComponents<OffsetComponent>();

		
		const IDType camID = currentState->GetEntities<CameraEntity>().Front().GetEntityID();
		const TransformComponent& camTransform = transformComponents[camID];
		const CameraShakeComponent& shakeComponent = currentState->GetComponents<CameraShakeComponent>()[camID];

		const glm::vec2 shakePositionOffset = shakeComponent.GetPositionOffset();
		const float shakeAngleOffset = shakeComponent.GetAngleOffset();
		
		const glm::mat3 world2NDC = CameraEntity::CalculateWorldToNDCTransform(
			camTransform.GetPosition() + shakePositionOffset,
			camTransform.GetRotationRadian() + shakeAngleOffset,
			camTransform.GetScale()
		);
		const glm::mat3 window2NDC = CameraEntity::CalculateWindowToNDCTransform(shakePositionOffset, shakeAngleOffset);
		for (auto& texture : textureComponents)
		{
			const auto entityID = texture.GetEntityID();
			const TransformComponent& transform = transformComponents[entityID];
			const OffsetComponent& offset = OffsetComponents[entityID];

			
			texture.Draw((texture.IsFixedOnScreen() ? window2NDC : world2NDC)  * transform.GetOrCalculateTransformMatrix()* offset.GetOffSetMatrix() * offset.GetHotSpotMatrix());
			
		}
	}
}
