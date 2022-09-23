/*
    File Name: PlayTestTrace.h
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#pragma once
#include <string>

#include "Component/EnemyStateComponent.h"

namespace PlayTestTrace
{
    void LogDeadMessage(std::string cause_of_death);
    void LogWeaponUseMessage(std::string weapon);
    void LogItemUseMessage(std::string item);
    void LogPlayerHitByMessage(std::string cause_of_hit);
    void LogOtherTypeMessage(std::string message);


    void LogPickUpMessage(std::string get, std::string throw_away);
    void LogWeaponSwapMessage(std::string main, std::string second);
    void LogItemSwapMessage(std::string main, std::string second);

    void EnemyDeadMessage(EnemyStateComponent::EnemyType enemyType, std::string cause_of_death);
}
