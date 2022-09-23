/*
    File Name: CameraShakeUpdateSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CameraShakeUpdateSystem.h"

#include "../Component/CameraShakeComponent.h"
#include "../GameStateManager.h"


namespace q_engine
{
	void CameraShakeUpdateSystem::Update(double deltaTime)
	{
		const float castDeltaTime = static_cast<float>(deltaTime);
		
        const auto& currentState = m_controlled_state;
        auto& cameraShakeComponents = currentState->GetComponents<CameraShakeComponent>();

		for (CameraShakeComponent& cameraShake : cameraShakeComponents)
		{
			cameraShake.AddTraumaLevel(castDeltaTime * -CameraShakeComponent::DECREASE_RATE);
			cameraShake.UpdateTime(castDeltaTime);
		}
	}
}
