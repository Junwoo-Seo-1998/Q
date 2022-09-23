/*
	File Name: SceneHierarchyPanel.h
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <memory>
#include <stack>

#include <functional>
#include "Editor/EditorState/EditorScene.h"
#include "Engine/ECS/Entity.h"
#include <imgui.h>
#include "Engine/GameState.h"
class SceneHierarchyPanel
{
public:
	enum class PanelType
	{
		Game, UI
	};

	SceneHierarchyPanel() = default;
	SceneHierarchyPanel(const std::shared_ptr<q_engine::GameState>& context);

	void SetContext(const std::shared_ptr<q_engine::GameState>& context);
	void OnUpdate();
	void OnImGuiRender();

	void ReloadMap(q_engine::IDType baseMapEnttID);
	q_engine::BaseEntity* GetSelectedEntity() const { return mSelectedEntity; }

	void SetType(PanelType type)
	{
		mType = type;
	}
private:
	template<typename T, typename UIFunction>
	void DrawComponent(const std::string& name, q_engine::BaseEntity* entity, UIFunction uiFunction);

	void DrawComponentPanel(q_engine::BaseEntity* entity);
	
	void DrawEntityNode(q_engine::BaseEntity* entity);
	void DrawComponents(q_engine::BaseEntity* entity);
	
	void AddEntityCreationMenu(const std::string& menuName, std::function<void()> creationFunc);
	void AddEntityCreationMenus();

	template<typename Component, typename... Args>
	void AddComponentMenu(const std::string& menuName, Args... args);
	void AddComponentMenus();
private:
	std::shared_ptr<q_engine::GameState> mScene;
	q_engine::BaseEntity* mSelectedEntity;
	std::stack<q_engine::BaseEntity*> m_ToDelete;
	PanelType mType = PanelType::UI;
};

template<typename T, typename UIFunction>
void SceneHierarchyPanel::DrawComponent(const std::string& name, q_engine::BaseEntity* entity, UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed
		| ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
	if (entity->HasComponent<T>())
	{
		auto& component = entity->GetComponent<T>();
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx((void*)typeid(T).hash_code(), treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();


		ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
		if (ImGui::Button("+", ImVec2{ lineHeight,lineHeight }))
		{
			ImGui::OpenPopup("Component Settings");
		}

		bool removeComponent = false;
		if (ImGui::BeginPopup("Component Settings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				removeComponent = true;
			ImGui::EndPopup();
		}

		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}

		if (removeComponent)
			entity->DeleteComponent<T>();
	}
}

template <typename Component, typename... Args>
void SceneHierarchyPanel::AddComponentMenu(const std::string& menuName, Args... args)
{
	static_assert(std::is_constructible_v<Component, Args...>, "Cannot Make Component with provided arguments");
	if (ImGui::MenuItem(menuName.c_str()))
	{
		mScene->AddComponent<Component>(mSelectedEntity, std::forward<Args>(args)...);
		ImGui::CloseCurrentPopup();
	}
}

