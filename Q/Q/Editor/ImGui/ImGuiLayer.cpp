/*
	File Name: ImGuiLayer.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "ImGuiLayer.h"


#include "imgui/imgui.h"
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>
#include <GLFW/glfw3.h>

#include "Engine/Graphics/Window.h"

ImGuiLayer::ImGuiLayer()
{
	//setup imgui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; //enable keyboard ctrl
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad; //enable gamepad
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;//enable docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;//enable multi viewport /platform windows
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoAutoMerge = true;

	///io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Bold.ttf", 18.f);

	//io.FontDefault = io.Fonts->AddFontFromFileTTF("assets/fonts/opensans/OpenSans-Regular.ttf", 18.f);

	//setup style
	ImGui::StyleColorsDark();


	//when viewports are enabled we tweak window rounding /windowbg so platform windwos can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.f;
		style.Colors[ImGuiCol_WindowBg].w = 1.f;
	}

	SetDarkThemeColors();

	
	GLFWwindow* window = q_engine::q_impl::p_window;


	ImGui_ImplGlfw_InitForOpenGL(window, true);

	ImGui_ImplOpenGL3_Init("#version 410");
}

ImGuiLayer::~ImGuiLayer()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiLayer::Begin()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void ImGuiLayer::End()
{
	ImGuiIO& io = ImGui::GetIO();
	auto [x, y] = q_engine::get_window_size();
	io.DisplaySize = ImVec2((float)x, (float)y);

	//rendering
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

void ImGuiLayer::SetDarkThemeColors()
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
