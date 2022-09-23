#pragma once
/*
	File Name: ButtonManagingSystem.h
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <glm/vec2.hpp>
#include <vector>

#include "../Engine/ECS/System.h"

class ButtonManagingSystem final : public q_engine::System<ButtonManagingSystem>
{
	friend class ButtonComponent;
	using IDtype = unsigned int;
public:
	ButtonManagingSystem() = delete;
	ButtonManagingSystem(q_engine::GameState* state) : System<ButtonManagingSystem>(state) {}

	void Load() override;
	void Update(double deltaTime) override;
	void Unload() override;

	glm::vec2 GetFocusedButtonPos();
private:
	void Register(IDtype& id)
	{
		m_buttonEntityIDs.emplace_back(id);
	}
	std::vector<IDtype> m_buttonEntityIDs;
	int m_FocusedButtonIdx = 0;
};