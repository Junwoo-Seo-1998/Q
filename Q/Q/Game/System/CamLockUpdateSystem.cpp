/*
	File Name: CamLockUpdateSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "CamLockUpdateSystem.h"

void CamLockUpdateSystem::Update(double)
{


	using namespace q_engine;
	const glm::vec2 halfWinSize = glm::vec2{ get_window_size().first, get_window_size().second } *0.5f;
	auto& gameStateManager = Engine::GetEngineComponent<GameStateManager>();
	const glm::vec2 mapSize = gameStateManager.GetMapSize();

	const glm::vec2 UILayerSize = gameStateManager.GetUILayerSize() - glm::vec2{0, 48.f};

	assert(m_controlled_state->GetEntities<CameraEntity>().Size() != 0);

	CameraEntity& camEntity = m_controlled_state->GetEntities<CameraEntity>().Front();

	auto& camTransform = m_controlled_state->GetComponents<TransformComponent>()[camEntity.GetEntityID()];

	const glm::vec2 camPos = camTransform.GetPosition();

	if (camPos.x < halfWinSize.x)
	{
		camTransform.SetPositionX(halfWinSize.x);
	}
	else if(camPos.x + halfWinSize.x > mapSize.x)
	{
		camTransform.SetPositionX(mapSize.x - halfWinSize.x);
	}

	if (camPos.y + UILayerSize.y < halfWinSize.y)
	{
		camTransform.SetPositionY(halfWinSize.y - UILayerSize.y);
	}
	else if (camPos.y + UILayerSize.y + halfWinSize.y > mapSize.y)
	{
		camTransform.SetPositionY(mapSize.y - halfWinSize.y - UILayerSize.y);
	}
}

