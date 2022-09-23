/*
    File Name: EntityPhysicsDebugger.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "EntityPhysicsDebugger.h"


#include "../Component/VelocityComponent.h"
#include "../Engine.h"
#include "../GameStateManager.h"
#include "../Core/StringHash.h"

namespace q_engine
{
    void EntityDebugger::ImGuiRender(bool& is_open)
    {
        ImGui::PushID(tracking_object_ID);
        ImGui::AlignTextToFramePadding();
        bool node_open = ImGui::TreeNode("Object", "%s", m_typename.c_str());
        ImGui::NextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::Text("EntityID_%u", tracking_object_ID);
        ImGui::NextColumn();

        //check for valid object
        auto& TransformCompCont = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<TransformComponent>();
        auto transform_itr = TransformCompCont.find(tracking_object_ID);
        if (transform_itr == TransformCompCont.end())
        {
            is_valid = false;
        }

        if (node_open)
        {
            ComponentRender(DebuggingComponentTypes::Position);
            ComponentRender(DebuggingComponentTypes::Velocity);
           
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    void EntityDebugger::ComponentRender(DebuggingComponentTypes type)
    {
        const std::string entID = std::to_string(tracking_object_ID);

        switch (type)
        {
        case DebuggingComponentTypes::Position:
        {
            const std::string type_s = "Position";
            bool node_open = ImGui::TreeNode(type_s.c_str(), "%s", type_s.c_str());
            const std::string hash_node_ID = entID + type_s;
            ImGui::NextColumn();
            ImGui::PushID(static_cast<int>(get_hash_id(hash_node_ID)));

            if (node_open)
            {
                auto& TransformCompCont = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<TransformComponent>();
                auto transform_itr = TransformCompCont.find(tracking_object_ID);

                ImGui::AlignTextToFramePadding();;
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);
                ImGui::PushID(0);


                static glm::vec2 input_position = transform_itr->mPosition;
                ImGui::Text("CurX");
                ImGui::NextColumn();
                ImGui::Text("%f", transform_itr->mPosition.x);

                ImGui::NextColumn();
                ImGui::Text("CurY");
                ImGui::NextColumn();
                ImGui::Text("%f", transform_itr->mPosition.y);
                ImGui::NextColumn();

                ImGui::Text("TargetX");
                ImGui::NextColumn();
                ImGui::InputFloat("", &input_position.x);
                ImGui::NextColumn();
                ImGui::PopID();
                ImGui::SetNextItemWidth(-1);
                ImGui::PushID(1);
                ImGui::Text("TargetY");
                ImGui::NextColumn();
                ImGui::InputFloat("", &input_position.y);
                ImGui::NextColumn();
                if (ImGui::Button("Sync"))
                {
                    input_position = transform_itr->mPosition;
                }
                ImGui::NextColumn();
                if (ImGui::Button("Apply"))
                {
                    transform_itr->mPosition = input_position;
                }


                ImGui::PopID();
                ImGui::TreePop();



            }
            ImGui::NextColumn();
            ImGui::PopID();
            break;
        }
        case DebuggingComponentTypes::Velocity:
        {
            const std::string type_s = "Velocity";
            bool node_open = ImGui::TreeNode(type_s.c_str(), "%s", type_s.c_str());
            const std::string hash_node_ID = entID + type_s;
            ImGui::NextColumn();
            ImGui::PushID(static_cast<int>(get_hash_id(hash_node_ID)));

            auto& vel_comp_cont = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<VelocityComponent>();
            auto vel_itr = vel_comp_cont.find(tracking_object_ID);

            if (node_open)
            {
                ImGui::AlignTextToFramePadding();
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);

                glm::vec2* velocity = &vel_itr->mVelocity;
                ImGui::Text("X");
                ImGui::NextColumn();
                ImGui::TextWrapped(std::to_string(velocity->x).c_str());
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);
                ImGui::Text("Y");
                ImGui::NextColumn();
                ImGui::TextWrapped(std::to_string(velocity->y).c_str());
                ImGui::TreePop();
            }
            ImGui::NextColumn();
            ImGui::PopID();
            break;
        }
        case DebuggingComponentTypes::Health:
        {
            const std::string type_s = "Health";
            bool node_open = ImGui::TreeNode(type_s.c_str(), "%s", type_s.c_str());
            const std::string hash_node_ID = entID + type_s;
            ImGui::NextColumn();
            ImGui::PushID(static_cast<int>(get_hash_id(hash_node_ID)));

            auto& health_comp_cont = Engine::GetEngineComponent<GameStateManager>().GetPlayState()->GetComponents<HealthComponent>();
            auto hel_itr = health_comp_cont.find(tracking_object_ID);

            if (node_open)
            {
                ImGui::AlignTextToFramePadding();
                ImGui::NextColumn();
                ImGui::SetNextItemWidth(-1);

                ImGui::Text("Health");
                ImGui::NextColumn();
                ImGui::Text("%d / %d", hel_itr->GetCurHealth(), hel_itr->GetMaxHealth());

            }
            ImGui::NextColumn();
            ImGui::PopID();
            break;
        }
        }
    }

    void EntityDebuggerManager::ImGuiRender(bool& is_open)
    {
        ImGui::SetNextWindowSize(ImVec2(430, 450), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("EntityPhysicsDebugger", &is_open))
        {
            ImGui::End();
            return;
        }
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        ImGui::Columns(2);
        ImGui::Separator();


        if(tracking_entities.empty())
        {
            ImGui::Text("Tracking Ent Que is Empty");
        }

        for (auto& ent_debug : tracking_entities)
        {
            ent_debug.ImGuiRender(is_open);
            if (ent_debug.IsValid() == false)
            {
                deleting_que.push_back(ent_debug.tracking_object_ID);
            }
        }
        for (auto& ent_ID : deleting_que)
        {
            auto delete_itr = std::find_if(tracking_entities.begin(), tracking_entities.end(), [ent_ID](EntityDebugger& elem)
                {
                    return elem.tracking_object_ID == ent_ID;
                });
            if (delete_itr != tracking_entities.end())
            {
                tracking_entities.erase(delete_itr);
            }
        }
        deleting_que.clear();

        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::PopStyleVar();
        ImGui::End();
    }

    void EntityDebuggerManager::Load()
    {
        Init();

        std::shared_ptr<GameState> playing_state = Engine::GetEngineComponent<GameStateManager>().GetPlayState();
        ContiguousHashMap<RectCollisionComponent>* m_debugging_obj_cont = &playing_state->GetComponents<RectCollisionComponent>();
        auto itr = m_debugging_obj_cont->begin();

        const auto trackable_entites_type_end_itr = trackable_entites_type.end();

        while (itr < m_debugging_obj_cont->end())
        {
            const IDType entityTypeID = itr->GetEntityTypeID();
            const auto trackable_entity_type_itr = trackable_entites_type.find(entityTypeID);

            if (trackable_entity_type_itr != trackable_entites_type_end_itr)
            {
                const IDType entityID = itr->GetEntityID();
                std::string type_name = trackable_entity_type_itr->second;
                tracking_entities.emplace_back(EntityDebugger(entityID, type_name));
            }
            ++itr;
        }
    }

    void EntityDebuggerManager::Clear()
    {
        for (auto tracking_entity : tracking_entities)
        {
            deleting_que.emplace_back(tracking_entity.tracking_object_ID);
        }
    }

    void EntityDebuggerManager::Init()
    {
        RegisterTrackableEntityType(GetTypeID<PlayerEntity>());
        RegisterTrackableEntityType(GetTypeID<EnemyEntity>());
    }

    void EntityDebuggerManager::RegisterTrackableEntityType(IDType entity_type_ID)
    {
        static const IDType playerID = GetTypeID<PlayerEntity>();
        static const IDType enemyID = GetTypeID<EnemyEntity>();

        std::string type_name;


        if (entity_type_ID == playerID)
        {
            type_name = "Player";
        }
        else if (entity_type_ID == enemyID)
        {
            type_name = "Enemy";
        }

        trackable_entites_type.emplace(entity_type_ID, type_name);
    }

}

