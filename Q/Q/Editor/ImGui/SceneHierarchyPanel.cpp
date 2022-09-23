/*
	File Name: SceneHierarchyPanel.cpp
	Project Name: Q
	Author(s):
		Primary: Junwoo seo
		Secondary:
	All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "SceneHierarchyPanel.h"
#include "Engine/AssetManager.h"
#include <iostream>


#include "EObjectZDepth.h"
#include "Engine/GameState.h"
#include "imgui_internal.h"
#include "Component/EnemyStateComponent.h"
#include "Component/EnemyWeaponComponent.h"
#include "Component/HealthComponent.h"

#include "Engine/Component/AnimationComponent.h"
#include "Engine/Component/CameraShakeComponent.h"
#include "Engine/Component/CollisionComponent.h"
#include "Engine/Component/ColorComponent.h"
#include "Engine/Component/DragAccelComponent.h"
#include "Engine/Component/FollowComponent.h"
#include "Engine/Component/GravityComponent.h"
#include "Engine/Component/LifeTimeComponent.h"
#include "Engine/Component/LuaScriptComponent.h"
#include "Engine/Component/ObjectStateComponent.h"
#include "Engine/Component/OffsetComponent.h"
#include "Engine/Component/RadiusComponent.h"
#include "Engine/Component/RaycastComponent.h"
#include "Engine/Component/TextComponent.h"
#include "Engine/Component/TextureBoxComponent.h"
#include "Engine/Component/TextureComponent.h"
#include "Engine/Component/TileMapComponent.h"
#include "Engine/Component/TransformComponent.h"
#include "Engine/Component/UserInputComponent.h"
#include "Engine/Component/VelocityComponent.h"
#include "Engine/Component/WidthHeightComponent.h"

#include "Engine/Component/TagComponent.h"
#include "Engine/Entity/CameraEntity.h"
#include "Engine/Scripting/ScriptManager.h"
#include "../Game/Component/UIComponents/ButtonComponent.h"
#include "Engine/Engine.h"

#include "Entity/MapEntity.h"
#include "Entity/PhysicsTestEntities.h"
#include "Entity/WeaponEntity.h"
#include "glm/glm.hpp"
#include "System/UI/ButtonManagingSystem.h"

#include "Engine/GameStateManager.h"

//add menu for making Enitiy
void SceneHierarchyPanel::AddEntityCreationMenus()
{
	using namespace q_engine;
	AddEntityCreationMenu("BaseEntity",[&]()
	{
		auto base = mScene->AddEntity<BaseEntity>("BaseEntity");
	});

	AddEntityCreationMenu("CameraEntity", [&]()
	{
		auto camera = mScene->AddEntity<CameraEntity>("CameraEntity");
		mScene->AddComponent<q_engine::TransformComponent>(camera);
		mScene->AddComponent<q_engine::CameraShakeComponent>(camera);
	});

	AddEntityCreationMenu("Texture", [&]()
		{
			auto texture = mScene->AddEntity<BaseEntity>("Texture");
			mScene->AddComponent<TransformComponent>(texture, glm::vec2{ 0,0 });
			mScene->AddComponent<OffsetComponent>(texture);
			mScene->AddComponent<TextureComponent>(texture, q_engine::get_hash_id("TestPng"), 0, false);
		});

	AddEntityCreationMenu("Animation", [&]()
		{
			auto animation = mScene->AddEntity<BaseEntity>("Animation");
			mScene->AddComponent<TransformComponent>(animation, glm::vec2{ 0,0 });
			mScene->AddComponent<OffsetComponent>(animation);
			mScene->AddComponent<AnimationComponent>(animation);
		});

	switch(mType)
	{
	case PanelType::Game:
	    {
		AddEntityCreationMenu("GroundEntity", [&]()
			{
				auto ground = mScene->AddEntity<GroundEntity>("Ground");
				mScene->AddComponent<TransformComponent>(ground, glm::vec2{ 0,0 });
				mScene->AddComponent<WidthHeightComponent>(ground, 100.f, 100.f);
				mScene->AddComponent<StaticRectCollisionComponent>(ground, glm::vec2{ 600,-100 }, 100.f, 100.f);
				mScene->AddComponent<TextureComponent>(ground, q_engine::get_hash_id("TestPng"), 0, false);
				mScene->AddComponent<OffsetComponent>(ground);
			});
		AddEntityCreationMenu("EnemyAndWeapon", [&]()
			{
				auto	enemy = mScene->AddEntity<EnemyEntity>("Enemy");
				auto	enemyWeapon = mScene->AddEntity<EnemyWeaponEntity>("EnemyWeapon");
				mScene->AddComponent<TransformComponent>(enemy, glm::vec2{ 0,0 }, 0.f, glm::vec2{ 2.0f,2.0f });
				mScene->AddComponent<AnimationComponent>(enemy, "WarriorMissingAnim"_hashID, static_cast<int>(EObjectZDepth::IN_GAME_PLAY_ENEMY_OBJ));
				mScene->AddComponent<VelocityComponent>(enemy, glm::vec2{ 100, 0 });
				mScene->AddComponent<EnemyStateComponent>(enemy, EnemyStateComponent::EnemyType::Warrior, enemyWeapon->GetEntityID(), glm::vec2{ 150,300 }, glm::vec2{ 1400,5000 });
				mScene->AddComponent<HealthComponent>(enemy, 50);
				mScene->AddComponent<GravityComponent>(enemy);
				mScene->AddComponent<WidthHeightComponent>(enemy, 56.f, 70.f);
				mScene->AddComponent<RectCollisionComponent>(enemy, glm::vec2{ -100.f,200.f }, 56.f, 70.f);
				mScene->AddComponent<DragAccelComponent>(enemy);
				mScene->AddComponent<RaycastComponent>(enemy, 1, 10.f);
				mScene->AddComponent<ObjectStateComponent>(enemy);
				mScene->AddComponent<OffsetComponent>(enemy, glm::vec2{ 28.f ,0.f });

				mScene->AddComponent<TransformComponent>(enemyWeapon, glm::vec2{ 110, 40 });
				mScene->AddComponent<WidthHeightComponent>(enemyWeapon, 30.f, 40.f);
				mScene->AddComponent<RectCollisionComponent>(enemyWeapon, glm::vec2{ 110, 40 }, 20.f, 10.f, false, true);
				mScene->AddComponent<EnemyWeaponComponent>(enemyWeapon, enemy->GetEntityID(), glm::vec2{ 30,10 }, 10);
				mScene->AddComponent<OffsetComponent>(enemyWeapon);
			});

		break;
	    }
	case PanelType::UI:
	    {
		AddEntityCreationMenu("Button", [&]()
			{
				auto	button = mScene->AddEntity<BaseEntity>("Button");
				mScene->AddComponent<TransformComponent>(button, glm::vec2{ 0, 0 });
				mScene->AddComponent<WidthHeightComponent>(button, glm::vec2{ 50, 100 });
				mScene->AddComponent<OffsetComponent>(button);
				auto texture = reinterpret_cast<TextureComponent*>(mScene->AddComponent<TextureComponent>(button, q_engine::get_hash_id("TestPng"), 0, false));
		        mScene->AddComponent<TextureBoxComponent>(button);
				mScene->AddComponent<LuaScriptComponent>(button, "resource/lua/UItest.lua");
				mScene->AddComponent<ButtonComponent>(button, button->GetEntityID(), &mScene->GetUpdateSystem<ButtonManagingSystem>(), "resource/lua/EditorCameraMove.lua");
				LuaScriptComponent& lua = mScene->GetComponents<LuaScriptComponent>()[button->GetEntityID()];

				lua.OnInit();
			});
		break;
	    }
	}
	
}

void SceneHierarchyPanel::AddComponentMenus()
{
	switch(mType)
	{
	case PanelType::Game:
	    {
		AddComponentMenu<q_engine::CameraShakeComponent>("CameraShakeComponent");
		AddComponentMenu<q_engine::CircleCollisionComponent>("CircleCollisionComponent", glm::vec2{ 0.f }, 1.f);
		AddComponentMenu<q_engine::DragAccelComponent>("DragAccelComponent");
		AddComponentMenu<q_engine::GravityComponent>("GravityComponent");
		AddComponentMenu<q_engine::ObjectStateComponent>("ObjectStateComponent");
		AddComponentMenu<q_engine::CircleComponent>("CircleComponent");
		AddComponentMenu<q_engine::RaycastComponent>("RaycastComponent");
		AddComponentMenu<q_engine::TileMapComponent>("TileMapComponent", "default.json", glm::vec2{ 0.f });
		AddComponentMenu<q_engine::UserInputComponent>("UserInputComponent");
		AddComponentMenu<q_engine::VelocityComponent>("VelocityComponent");
		break;
	    }
	case PanelType::UI:
	    {
		break;
	    }
	}
	AddComponentMenu<q_engine::AnimationComponent>("AnimationComponent");

	//Common components
	AddComponentMenu<q_engine::RectCollisionComponent>("RectCollisionComponent");
	AddComponentMenu<q_engine::StaticRectCollisionComponent>("StaticRectCollisionComponent");
	AddComponentMenu<q_engine::ColorComponent>("ColorComponent");
	AddComponentMenu<q_engine::FollowComponent>("FollowComponent");
	AddComponentMenu<q_engine::LifeTimeComponent>("LifeTimeComponent");
	AddComponentMenu<q_engine::OffsetComponent>("OffsetComponent");
	AddComponentMenu<q_engine::TextComponent>("TextComponent");
	AddComponentMenu<q_engine::TextureBoxComponent>("TextureBoxComponent");
	AddComponentMenu<q_engine::TextureComponent>("TextureComponent",q_engine::get_hash_id("TestPng"));
	AddComponentMenu<q_engine::TransformComponent>("TransformComponent");
	AddComponentMenu<q_engine::WidthHeightComponent>("WidthHeightComponent");
		
}

void SceneHierarchyPanel::DrawComponentPanel(q_engine::BaseEntity* entity)
{
	
	DrawComponent<q_engine::AnimationComponent>("Animation", entity, [](auto& component)
	{
		if (ImGui::BeginCombo("##Animation", component.GetAnimationName().c_str()))
		{
			auto& animations = q_engine::AssetManager::GetAnimations();
			for (auto& [id, animation] : animations)
			{
				if (ImGui::Selectable(animation.GetAnimationName().c_str()))
				{
					component.SetAnimation(id);
				}
			}
			ImGui::EndCombo();
		}
		if (ImGui::Button("Play Animation"))
			component.ResetAndPlay();
		
	});

	DrawComponent<q_engine::CameraShakeComponent>("CameraShake", entity, [](auto& component)
		{});
	DrawComponent<q_engine::OffsetComponent>("Offset", entity, [](auto& component)
		{
			bool isflipped = component.GetIsFlipped();
			if (ImGui::Checkbox("Flip", &isflipped))
			{
				component.SetFlipped(isflipped);
			}

			glm::vec2 offset_pos = { component.GetOffSetPositionX(), component.GetOffSetPositionY() };
			ImGui::DragFloat2("Offset", &offset_pos[0]);
			component.SetOffsetPosition(offset_pos);

		});

	DrawComponent<q_engine::RectCollisionComponent>("RectCollision", entity, [](auto& component)
		{});

	DrawComponent<q_engine::StaticRectCollisionComponent>("StaticRectCollision", entity, [](auto& component)
		{});

	DrawComponent<q_engine::ColorComponent>("Color", entity, [](auto& component)
		{});
	DrawComponent<q_engine::CircleCollisionComponent>("CircleCollision", entity, [](auto& component)
		{});

    DrawComponent<q_engine::TextComponent>("Text", entity, [](auto& component)
			{});

	DrawComponent<q_engine::TextureBoxComponent>("TextureBox", entity, [](auto& component)
		{});

	DrawComponent<q_engine::TextureComponent>("Texture", entity, [](auto& component)
		{
			if (ImGui::BeginCombo("##Texture", component.GetTextureName().c_str()))
			{
				auto& textures = q_engine::AssetManager::GetTextures();
				for (auto& [id, texture] : textures)
				{
					std::string textureName = texture.GetTextureName();
					if(textureName.find("Original") >= textureName.length())
					{
						if (ImGui::Selectable(texture.GetTextureName().c_str()))
						{
							component.SetTexture(id);
						}
					}
				}
				ImGui::EndCombo();
			}
		});

	DrawComponent<q_engine::TransformComponent>("Transform", entity, [](auto& component)
		{
			glm::vec3 pos = { component.GetPositionX(),component.GetPositionY(),0.f };
			DrawVec3Control("Translation", pos);
			component.SetPosition({ pos.x,pos.y });
			float rotation = glm::degrees(component.GetRotationRadian());
			ImGui::DragFloat("Rotation", &rotation);
			component.SetRotationRadian(glm::radians(rotation));
			glm::vec3 scale = { component.GetScaleX(), component.GetScaleY(),0.f };
			DrawVec3Control("Scale", scale, 1.f);
			component.SetScale({ scale.x,scale.y });
		});

	DrawComponent<q_engine::WidthHeightComponent>("WidthHeight", entity, [](auto& component)
		{
			float width = component.GetWidth<float>();
			float height = component.GetHeight<float>();
			ImGui::DragFloat("Width", &width, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::DragFloat("Height", &height, 0.1f, 0.0f, 0.0f, "%.2f");
			component.SetWidth(width);
			component.SetHeight(height);

		});


	DrawComponent<q_engine::DragAccelComponent>("DragAccel", entity, [](auto& component)
		{});

	DrawComponent<q_engine::FollowComponent>("Follow", entity, [](auto& component)
		{});

	DrawComponent<q_engine::GravityComponent>("Gravity", entity, [](auto& component)
		{});

	DrawComponent<q_engine::LifeTimeComponent>("LifeTime", entity, [](auto& component)
		{});

	DrawComponent<q_engine::ObjectStateComponent>("ObjectState", entity, [](auto& component)
		{});

	DrawComponent<q_engine::CircleComponent>("Circle", entity, [](auto& component)
		{});

	DrawComponent<q_engine::RaycastComponent>("Raycast", entity, [](auto& component)
		{});

	DrawComponent<q_engine::TileMapComponent>("TileMap", entity, [&](auto& component)
		{
			char buffer[256];
			memset(buffer, 0, sizeof(buffer));
			strcpy_s(buffer, sizeof(buffer), component.GetFileName().c_str());
			ImGui::DragFloat2("Offset", &component.GetOffset().x, 0.1f, 0.0f, 0.0f, "%.2f");
			ImGui::Text("FileName");


			ImGui::NextColumn();


			if (ImGui::InputText("##FileName", buffer, sizeof(buffer)))
			{
				component.GetFileName() = std::string(buffer);
			}
			if (ImGui::Button("Reload"))
			{
				//reload
				ReloadMap(mSelectedEntity->GetEntityID());
			}
		});

	DrawComponent<q_engine::UserInputComponent>("UserInput", entity, [](auto& component)
		{});

	DrawComponent<q_engine::VelocityComponent>("Velocity", entity, [](auto& component)
		{});

	DrawComponent<EnemyStateComponent>("EnemyState", entity, [](auto& component)
		{			});


	DrawComponent<EnemyWeaponComponent>("EnemyWeapon", entity, [](auto& component)
		{			});

	DrawComponent<HealthComponent>("Health", entity, [](auto& component)
		{			});

	DrawComponent<q_engine::LuaScriptComponent>("LuaScript", entity, [](auto& component)
	{
			//strcpy_s(buffer, sizeof(buffer), component.GetFileName().c_str());

			ImGui::Text("FileName");
			ImGui::NextColumn();
			ImGui::Text(component.GetFileName().c_str());
			static char buffer[256];

			ImGui::InputText("##FileName", buffer, sizeof(buffer));
			ImGui::SameLine();
			bool clear = ImGui::Button("Clear");
			ImGui::SameLine();
			if (clear)
			{
				memset(buffer, 0, sizeof(buffer));
			}
			if (ImGui::Button("Reload"))
			{
				//reload
				component.LoadLuaScript(std::string(buffer));
				memset(buffer, 0, sizeof(buffer));
			}
	});
	
}


SceneHierarchyPanel::SceneHierarchyPanel(const std::shared_ptr<q_engine::GameState>& context)
{
	SetContext(context);
}

void SceneHierarchyPanel::SetContext(const std::shared_ptr<q_engine::GameState>& context)
{
	mScene = context;
	mSelectedEntity = nullptr;
	while (!m_ToDelete.empty())
	{
		m_ToDelete.pop();
	}
}

void SceneHierarchyPanel::OnUpdate()
{
	while (!m_ToDelete.empty())
	{
		auto entt_to_reomve = m_ToDelete.top();
		m_ToDelete.pop();
		mScene->DeleteEntityComponents(entt_to_reomve->GetEntityID());
	}
}

void SceneHierarchyPanel::OnImGuiRender()
{
	ImGui::Begin("SceneHierarchy");

	auto& entt_pool = mScene->GetAllEntities();
	for (auto& [entt_id,entt]:entt_pool)
	{
		DrawEntityNode(&entt);
	}

	if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
		mSelectedEntity = nullptr;

	//right click on blank space
	if (ImGui::BeginPopupContextWindow(0, 1, false))
	{
		//add menus
		AddEntityCreationMenus();
		
		ImGui::EndPopup();
		
	}
	ImGui::End();

	ImGui::Begin("Properties");
	if (mSelectedEntity)
	{
		DrawComponents(mSelectedEntity);
	}
	ImGui::End();
}

void SceneHierarchyPanel::ReloadMap(q_engine::IDType baseMapEnttID)
{
	if(mType == PanelType::Game)
	{
		reinterpret_cast<EditorScene*>(mScene.get())->ReLoadTileMap(baseMapEnttID);
	}

}



void SceneHierarchyPanel::DrawEntityNode(q_engine::BaseEntity* entity)
{
	if (entity->GetRuntimeTypeID() == q_engine::GetTypeID<q_engine::MapEntity>())
		return;
	if (entity->GetRuntimeTypeID() == q_engine::GetTypeID<q_engine::BackGroundEntity>())
		return;
	if (entity->GetRuntimeTypeID() == q_engine::GetTypeID<q_engine::PortalEntity>())
		return;
	
	auto& tag = entity->GetComponent<TagComponent>();// in editor every entt have tag component
	ImGuiTreeNodeFlags flags = ((mSelectedEntity == entity) ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_SpanAvailWidth;
	std::string tagged = tag.Tag.c_str();
	tagged += std::to_string(entity->GetEntityID());
	bool opened = ImGui::TreeNodeEx(reinterpret_cast<void*>(entity->GetEntityID()), flags, tagged.c_str());

	if (ImGui::IsItemClicked())
	{
		mSelectedEntity = entity;
	}
	
	bool entityDeleted = false;
	if (ImGui::BeginPopupContextItem())
	{
		if (ImGui::MenuItem("Delete Entity"))
			entityDeleted = true;
		ImGui::EndPopup();
	}

	if (opened)
	{
		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
		bool opened = ImGui::TreeNodeEx((void*)1234, flags, "test");
		if (opened)
		{
			ImGui::TreePop();
		}
		ImGui::TreePop();
	}

	if (entityDeleted)
	{
		m_ToDelete.push(entity);
		if (mSelectedEntity == entity)
			mSelectedEntity = nullptr;
	}
}


//to draw vec3 control
static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f)
{
	ImGuiIO& io = ImGui::GetIO();
	auto boldFont = io.Fonts->Fonts[0];

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0,0 });

	float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
	ImVec2 buttonSize = { lineHeight + 3.0f,lineHeight };

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f,0.1f,0.15f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f,0.2f,0.2f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f,0.1f,0.15f ,1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("X", buttonSize))
		values.x = resetValue;
	ImGui::PopFont();

	ImGui::PopStyleColor(3);


	ImGui::SameLine();
	ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f,0.7f,0.2f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f,0.8f,0.2f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f,0.7f,0.2f ,1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Y", buttonSize))
		values.y = resetValue;
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();
	ImGui::SameLine();


	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f,0.25f,0.8f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f,0.35f,0.9f ,1.0f });
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f,0.25f,0.8f ,1.0f });

	ImGui::PushFont(boldFont);
	if (ImGui::Button("Z", buttonSize))
		values.z = resetValue;
	ImGui::PopStyleColor(3);
	ImGui::PopFont();

	ImGui::SameLine();
	ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
	ImGui::PopItemWidth();

	ImGui::PopStyleVar();

	ImGui::Columns(1);

	ImGui::PopID();
}




void SceneHierarchyPanel::DrawComponents(q_engine::BaseEntity* entity)
{
	if (entity->HasComponent<TagComponent>())
	{
		auto& tag = entity->GetComponent<TagComponent>().Tag;
		std::string tagged = tag.c_str();
		tagged += std::to_string(entity->GetEntityID());

		
		char buffer[256];
		memset(buffer, 0, sizeof(buffer));
		strcpy_s(buffer, sizeof(buffer), tagged.c_str());
		if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
		{
			tag = std::string(buffer);
		}
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(-1);
	if (ImGui::Button("Add Component"))
		ImGui::OpenPopup("Add Component");

	if (ImGui::BeginPopup("Add Component"))
	{
		AddComponentMenus();
		ImGui::EndPopup();
	}
	ImGui::PopItemWidth();


	DrawComponentPanel(entity);
}

void SceneHierarchyPanel::AddEntityCreationMenu(const std::string& menuName, std::function<void()> creationFunc)
{
	if(ImGui::MenuItem(menuName.c_str()))
	{
		creationFunc();
	}
}

