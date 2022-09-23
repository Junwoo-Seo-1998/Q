/*
	File Name: EditorComponent.h
	Project Name: Q
	Author(s):
		Primary: Junwoo Seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/

#pragma once
#include <memory>
#include "Editor/ImGui/ImGuiLayer.h"
#include "Engine/Graphics/Framebuffer.h"
#include "Engine/EngineComponent.h"
#include <glm/glm.hpp>
#include "Editor/EditorState/EditorScene.h"
#include "Editor/ImGui/SceneHierarchyPanel.h"

namespace q_engine
{
	class EditorComponent :public EngineComponent<EditorComponent>
	{
		friend class Engine;
	public:
		EditorComponent();
		~EditorComponent() override;
		
		virtual void Load() override;
		
		virtual void Update(double deltaTime) override;

		void OnImGuiRender();
		
		virtual void Unload() override;

		void NewScene(SceneHierarchyPanel::PanelType type = SceneHierarchyPanel::PanelType::Game);
		void OpenScene();
		void SaveSceneAs();


	private:
		[[nodiscard]] glm::vec2 GetViewportSize() const
		{
			return m_ViewportSize;
		}
		
		std::shared_ptr<Framebuffer> m_Framebuffer;
		bool m_ViewportFocused = false, m_ViewportHovered = false;
		glm::vec2 m_ViewportSize = { 0.f,0.f };

		std::shared_ptr<GameState> Scene;
		//panels
		SceneHierarchyPanel m_SceneHierarchyPanel;
	};
}
