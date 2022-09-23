/*
	File Name: ProjectileSystem.cpp
	Project Name: Q
	Author(s):
		Primary: junwoo.seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ProjectileSystem.h"
#include "Engine/GameState.h"
#include "Game/Component/ProjectileScriptComponent.h"
namespace q_engine
{
	ProjectileSystem::ProjectileSystem(GameState* state)
		:System<ProjectileSystem>(state)
	{
	}

	void ProjectileSystem::Load()
	{
	}

	void ProjectileSystem::Update(double deltaTime)
	{
		
		auto& projectile_script_comps = m_controlled_state->GetComponents<ProjectileScriptComponent>();
		for (auto & projectile:projectile_script_comps)
		{
			projectile.OnUpdate(static_cast<float>(deltaTime));
			if(projectile.IsDead())
			{
				m_controlled_state->DeleteComponentsLater(projectile.GetEntityID());
				m_controlled_state->DeleteEntityLater(projectile.GetEntityTypeID(), projectile.GetEntityID());
			}
		}

	
	}

	void ProjectileSystem::Unload()
	{
	}

	void ProjectileSystem::OnEvent(IEvent* eventName)
	{
	}
}

