/*
	File Name: Editor-Main.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "Engine/Engine.h"
#include "Engine/Graphics/Drawing.h"
#include "Engine/Graphics/Window.h"
#include "Engine/ImGui/ImGuiHelper.h"

int main(void)
{
	q_engine::create_window(1600, 900, "Q-Editor", q_engine::EWindowMode::WINDOWED);
	ImGuiHelper ImGui;
	ImGui.Setup(q_engine::q_impl::p_window);
	q_engine::Engine::Init();

	q_engine::Engine::Load();
	while (!q_engine::is_window_closed() && !q_engine::Engine::IsGameEnded())
	{
		q_engine::clear_background({ 0.33f, 0.33f,0.33f,1.f });
		ImGui.StartRender();
		q_engine::update_window();

		q_engine::Engine::Update();

		ImGui.EndRender();
		q_engine::swap_buffers();
	}

	q_engine::Engine::Unload();
	q_engine::Engine::CleanUp();
	ImGui.CleanUp();
	return 0;
}