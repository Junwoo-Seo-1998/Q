/*
	File Name: SimpleActionSystem.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 
	DigiPen (USA) Corporation, all rights reserved.
*/
#include "SimpleActionSystem.h"

#include "Engine/Component/SimpleActionComponent.h"

namespace q_engine
{
	void SimpleActionSystem::Update(double deltaTime)
	{
		auto& simple_script_comps = m_controlled_state->GetComponents<SimpleActionComponent>();
		for (auto& simple_script : simple_script_comps)
		{

			simple_script.OnUpdate(static_cast<float>(deltaTime));
			if (simple_script.IsDead())
			{
				m_controlled_state->DeleteComponentsLater(simple_script.GetEntityID());
				m_controlled_state->DeleteEntityLater(simple_script.GetEntityTypeID(), simple_script.GetEntityID());
			}
		}
	}
}
