/*
    File Name: FollowSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "FollowSystem.h"

#include "../Component/FollowComponent.h"
#include "../Component/TransformComponent.h"
#include "../Component/WidthHeightComponent.h"
#include "../Core/Math.h"
#include "../GameStateManager.h"


namespace q_engine
{
	void FollowSystem::Update(double)
	{
        const auto& currentState = m_controlled_state;
        auto& followComponents = currentState->GetComponents<FollowComponent>();
        auto& transformComponents = currentState->GetComponents<TransformComponent>();

		for (const FollowComponent& followComponent : followComponents)
		{
            const IDType entityIDToFollow = followComponent.GetEntityIDToFollow();
			if (entityIDToFollow == IDType(-1))
			{
                continue;
			}
			
            TransformComponent& transform = transformComponents[followComponent.GetEntityID()];
            const glm::vec2 target = transformComponents[entityIDToFollow].GetPosition();

            transform.SetPosition(smooth_step(transform.GetPosition(), target, 1 / followComponent.GetSmoothRate()));
 
		}
	}
}
