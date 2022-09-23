/*
    File Name: ButtonComponent.h
    Project Name: Q
    Author(s):
        Primary: Yoonki kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <utility>


#include "Engine/ECS/Component.h"
#include "Engine/GameState.h"
#include "System/UI/ButtonManagingSystem.h"

class ButtonComponent final : public q_engine::Component<ButtonComponent>
{
public:
    ButtonComponent() = delete;
    ButtonComponent(q_engine::IDType entID, ButtonManagingSystem* manager, std::string script) : behaviorScript(std::move(script))
    {
        manager->Register(entID);
    }

    void SetScript(std::string&& newScript) { behaviorScript = newScript; }
    [[nodiscard]] std::string GetScript() const { return behaviorScript; }
private:
    std::string behaviorScript;
};
