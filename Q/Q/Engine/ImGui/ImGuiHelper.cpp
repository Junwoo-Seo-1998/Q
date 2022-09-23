/*
	File Name: ImGuiHelper.cpp
	Project Name: Q
	Author(s):
		Primary: Yoonki Kim, Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ImGuiHelper.h"
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include "../Graphics/Window.h"
static void SetDarkThemeColors()
{
	auto& colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f,0.11f,1.0f };

	//headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };

	//buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };

	//frame
	colors[ImGuiCol_FrameBg] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };
	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f,0.305f,0.31f,1.0f };
	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };


	//tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f,0.3805f,0.381f,1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f,0.2805f,0.281f,1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f,0.205f,0.21f,1.0f };

	//title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f,0.1505f,0.151f,1.0f };

}

void ImGuiHelper::Setup(gsl::not_null<GLFWwindow*> p_window)
{
	IMGUI_CHECKVERSION();

	ImGui::CreateContext();
	io = ImGui::GetIO();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //enable keyboard ctrl
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //enable gamepad
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;//enable docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;//enable multi viewport /platform windows
	//when viewports are enabled we tweak window rounding /windowbg so platform windwos can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;
	}
	ImGui_ImplGlfw_InitForOpenGL(p_window, true);
	ImGui_ImplOpenGL3_Init("#version 410");
	ImGui::StyleColorsDark();


	SetDarkThemeColors();
}

void ImGuiHelper::StartRender()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	static bool show_demo = true;

	if (show_demo)
		ImGui::ShowDemoWindow();
}

void ImGuiHelper::EndRender()
{
	ImGuiIO& io = ImGui::GetIO();
	auto [x, y] = q_engine::get_window_size();

	io.DisplaySize = ImVec2((float)x, (float)y);

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);

	}
}

void ImGuiHelper::CleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}
