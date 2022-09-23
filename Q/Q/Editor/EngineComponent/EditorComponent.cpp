/*
	File Name: EditorComponent.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#include "EditorComponent.h"

#include <iostream>



#include "Editor/EditorState/UIEditorScene.h"
#include "imgui.h"
#include "Editor/SceneSerializer.h"
#include "Engine/Loader/Loader.h"
#include "Engine/FileDialogs.h"
#include "Engine/Graphics/Drawing.h"
#include "Engine/Graphics/Window.h"

namespace q_engine
{
	EditorComponent::EditorComponent()
	{
		//m_ImGuiLayer.reset(new ImGuiLayer());

		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		auto [x, y] = get_window_size();
		fbSpec.Width = x;
		fbSpec.Height = y;
		m_Framebuffer.reset(new Framebuffer(fbSpec));
		Scene.reset(new EditorScene());
		m_SceneHierarchyPanel.SetContext(Scene);
	}

	EditorComponent::~EditorComponent()
	{
		
	}

	void EditorComponent::Load()
	{
		Scene->Load();
	}

	
	void EditorComponent::Update(double deltaTime)
	{
		Scene->Update(deltaTime);
		Scene->DeleteEntities();
		m_SceneHierarchyPanel.OnUpdate();
		//draw between here
		{

			//update viewport before drawing framebuffer
			auto spec = m_Framebuffer->GetSpecification();
			
			if ((spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y)
				&& m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f)
			{
				m_Framebuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
			}
			
			m_Framebuffer->Bind();

			q_engine::clear_background({ 0.13f, 0.33f,0.33f,1.f });
			Scene->Draw(deltaTime);
			m_Framebuffer->Unbind();
		}
		
		OnImGuiRender();
	}

	void EditorComponent::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen = true;
		static bool opt_padding = false;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->GetWorkPos());
			ImGui::SetNextWindowSize(viewport->GetWorkSize());
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}
		else
		{
			dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
		// and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		if (!opt_padding)
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		if (!opt_padding)
			ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 370.0f;
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				if (ImGui::BeginMenu("New", "Ctrl+N"))
				{
					if(ImGui::MenuItem("Game"))
					{
						NewScene();
						return;
					}
					if(ImGui::MenuItem("UI"))
					{
						NewScene(SceneHierarchyPanel::PanelType::UI);
					}
				    ImGui::EndMenu();
				}


				if (ImGui::MenuItem("Open...", "Ctrl+O"))
				{
					OpenScene();
				}

				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
				{
					SaveSceneAs();
				}

				if (ImGui::MenuItem("Exit"))
				{
					//Application::Get().Close();
				}
				ImGui::EndMenu();
			}

			if(ImGui::BeginMenu("Properties"))
			{
				if(ImGui::BeginMenu("Mode"))
				{
					if (ImGui::MenuItem("Game"))
					{
						m_SceneHierarchyPanel.SetType(SceneHierarchyPanel::PanelType::Game);
					}
					if (ImGui::MenuItem("UI"))
					{
						m_SceneHierarchyPanel.SetType(SceneHierarchyPanel::PanelType::UI);
					}
					ImGui::EndMenu();
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		m_SceneHierarchyPanel.OnImGuiRender();



		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{ 0,0 });
		ImGui::Begin("Viewport");
		m_ViewportFocused = ImGui::IsWindowFocused();
		m_ViewportHovered = ImGui::IsWindowHovered();

		//Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);

		//update viewport to resolve viewport problem in Viewport panel
		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

		//JE_WARN("Viewport Size: {0}, {1}", viewportPanelSize.x, viewportPanelSize.y);
		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID(0);
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2{ 0,1 }, ImVec2{ 1,0 });

		////
		//Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
		//if (selectedEntity && m_GizmoType != -1)
		//{
		//	ImGuizmo::SetOrthographic(false);
		//	ImGuizmo::SetDrawlist();
		//	float windowWidth = (float)ImGui::GetWindowWidth();
		//	float windowHeight = (float)ImGui::GetWindowHeight();
		//	ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);
		//
		//	//runtime camera from entity
		//	//auto cameraEntity = m_ActiveScene->GetPrimaryCameraEntity();
		//	//const auto& camera = cameraEntity.GetComponent<CameraComponent>();
		//	//const glm::mat4& cameraProjection = camera.Camera.GetProjection();
		//	//glm::mat4 cameraView = glm::inverse(cameraEntity.GetComponent<TransformComponent>().GetTransform());
		//
		//	//editor camera
		//	const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();
		//	glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();
		//
		//
		//
		//	auto& tc = selectedEntity.GetComponent<TransformComponent>();
		//	glm::mat4 transform = tc.GetTransform();
		//
		//	//snapping
		//	bool snap = Input::IsKeyPressed(Key::LeftControl);
		//	float snapValue = 0.5f;// snap 0.5 for translation and scale
		//	if (m_GizmoType == ImGuizmo::OPERATION::ROTATE)
		//		snapValue = 45.0f;
		//
		//	float snapValues[3] = { snapValue,snapValue,snapValue };
		//
		//	ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
		//		(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform), nullptr, snap ? snapValues : nullptr);
		//
		//	if (ImGuizmo::IsUsing())
		//	{
		//		glm::vec3 translation, rotation, scale;
		//		Math::DecomposeTransform(transform, translation, rotation, scale);
		//		glm::vec3 deltaRotation = rotation - tc.Rotation;
		//		tc.Translation = translation;
		//		tc.Rotation += deltaRotation;
		//		tc.Scale = scale;
		//
		//	}
		//
		//}

		ImGui::End();
		ImGui::PopStyleVar();
		
	}


	void EditorComponent::Unload()
	{
		Scene->Unload();
	}

	void EditorComponent::NewScene(SceneHierarchyPanel::PanelType type)
	{
		FramebufferSpecification fbSpec;
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		auto [x, y] = get_window_size();
		fbSpec.Width = x;
		fbSpec.Height = y;
		m_Framebuffer.reset(new Framebuffer(fbSpec));

		if(type == SceneHierarchyPanel::PanelType::UI)
		{
			Scene.reset(new UIEditorScene);
		}
		else
		{
			Scene.reset(new EditorScene());
		}
		m_SceneHierarchyPanel.SetContext(Scene);

		Scene->Unload();
		Scene->Load();
	}

	void EditorComponent::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Q-Engine Scene (*.json)\0*.json\0");
		std::cout << filepath << std::endl;
		if (!filepath.empty())
		{
			NewScene();

			Loader loader(Scene.get());
			loader.LoadScene(filepath);
		}
	}

	void EditorComponent::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Q-Engine Scene (*.json)\0*.json\0");

		if (!filepath.empty())
		{
			SceneSerializer serializer(Scene.get());
			serializer.Serialize(filepath);
		}
	}
}
