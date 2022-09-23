/*
File Name: EntityPhysicsDebugger.h
Project Name: Q
Author(s):
    Primary: Yoonki Kim
    Secondary:
All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once

#include <map>

#include "../ImGui/ImGuiHelper.h"
#include <vector>

#include "../Component/CollisionComponent.h"
#include "../Engine/Core/ContiguousHashMap.h"
#include "../Component/TransformComponent.h"
#include "../ECS/Entity.h"

namespace q_engine
{
    class EntityDebuggerManager;

    class EntityDebugger : public ImGuiRenderable
    {
        friend EntityDebuggerManager;
        enum class DebuggingComponentTypes
        {
            Position, Velocity, Health
        };

    public:
        EntityDebugger() = delete;
        EntityDebugger(IDType entityID, std::string& type_name)
            : tracking_object_ID(entityID), is_valid(true), m_typename(std::move(type_name)) {}

        virtual void ImGuiRender(bool& is_open) override;

        void ComponentRender(DebuggingComponentTypes type);

        bool IsValid() { return is_valid; }
    private:
        IDType tracking_object_ID = INVALID_ID;
        bool is_valid = false;
        std::string m_typename;
    };

    class EntityDebuggerManager : public ImGuiRenderable
    {
        friend EntityDebugger;
    public:
        void Init();

        void RegisterTrackableEntityType(IDType entity_type_ID);

        virtual void ImGuiRender(bool& is_open) override;
        void Load();
        void Clear();
    private:
        std::map<IDType, std::string> trackable_entites_type;

        std::vector<EntityDebugger> tracking_entities;
        std::vector<IDType> deleting_que;
    };
}


