/*
	File Name: ProjectileSystem.h
	Project Name: Q
	Author(s):
		Primary: junwoo.seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include "Engine/ECS/System.h"

namespace  q_engine
{
	class ProjectileSystem final : public System<ProjectileSystem>
	{
	public:
		ProjectileSystem(q_engine::GameState* state);
		virtual void Load() override;
		virtual void Update(double deltaTime) override;
		virtual void Unload() override;
	protected:
		virtual void OnEvent(IEvent* eventName) override;
	public:

	};
}
