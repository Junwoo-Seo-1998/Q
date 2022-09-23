/*
    File Name: TextDrawSystem.cpp
    Project Name: Q
    Author(s):
        Primary: Joonho Hwang
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "TextDrawSystem.h"

#include "../Component/TextComponent.h"
#include "../Component/TransformComponent.h"
#include "../GameStateManager.h"
#include "../Graphics/Window.h"


namespace q_engine
{
	void TextDrawSystem::Update(double)
	{
		const auto& currentState = m_controlled_state;
		auto& textComponents = currentState->GetComponents<TextComponent>();
		auto& transformComponents = currentState->GetComponents<TransformComponent>();

		const auto win_size = glm::vec2{ get_window_size().first, get_window_size().second };

		for (auto& text : textComponents)
		{
			const auto entityID = text.GetEntityID();

			const TransformComponent& transform = transformComponents[entityID];
			text.GetTextObject().Draw(transform.GetPosition() - glm::vec2{win_size.x / 2.f, win_size.y / 2.f}, transform.GetRotationRadian(), transform.GetScale());
		}
	}
}
