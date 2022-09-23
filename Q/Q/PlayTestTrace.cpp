/*
    File Name: PlayTestTrace.cpp
    Project Name: Q
    Author(s):
        Primary: Yoonki Kim
        Secondary:
    All content (C) 2021 DigiPen (USA) Corporation, all rights reserved.
*/
#include "PlayTestTrace.h"


#include "Engine/Debugger/Debugger.h"
#include "Engine/Engine.h"
#include "Engine/GameStateManager.h"

void PlayTestTrace::LogDeadMessage(std::string cause_of_death)
{
    static unsigned death_counter = 1;
    if(cause_of_death == "Reset")
    {
        death_counter = 0;
        return;
    }

    std::string logMessage = "Player's " + std::to_string(death_counter) + " Dead " + cause_of_death + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime());
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);

}

void PlayTestTrace::LogWeaponUseMessage(std::string weapon)
{
    std::string logMessage = "Player used weapon " + weapon + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime());
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayTestTrace::LogItemUseMessage(std::string item)
{
    std::string logMessage = "Player used item " + item + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime());
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayTestTrace::LogPlayerHitByMessage(std::string cause_of_hit)
{
    static int before_health = 0;
    auto playState = q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayState();
    if (playState->GetComponents<HealthComponent>()[q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayerID()].IsDead())
    {
        return;
    }
    int playerHealth = playState->GetComponents<HealthComponent>()[q_engine::Engine::GetEngineComponent<q_engine::GameStateManager>().GetPlayerID()].GetCurHealth();
    if (before_health == playerHealth)
    {
        return;
    }
    std::string logMessage = "Player Hit By " + cause_of_hit + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime()) + '\n' + '\t';
    logMessage += "Now player health is " + std::to_string(playerHealth);
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
    before_health = playerHealth;
}

void PlayTestTrace::LogOtherTypeMessage(std::string message)
{
    q_engine::Debugger::GetLogger().AddMessage(message, q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayTestTrace::LogPickUpMessage(std::string get, std::string throw_away)
{
    std::string logMessage = "Player picked " + get + " and throw " + throw_away + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime()) + '\n';
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayTestTrace::LogWeaponSwapMessage(std::string main, std::string second)
{
    std::string logMessage = "Player swap weapon " + main + " to " + second + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime()) + '\n';
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayTestTrace::LogItemSwapMessage(std::string main, std::string second)
{
    std::string logMessage = "Player swap item " + main + " to " + second + " at Time " + std::to_string(q_engine::Engine::GetElapsedTime()) + '\n';
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
}

void PlayTestTrace::EnemyDeadMessage(EnemyStateComponent::EnemyType enemyType, std::string cause_of_death)
{
    std::string enemyString;

    switch(enemyType)
    {
    case EnemyStateComponent::EnemyType::BossMagician:
        {
        enemyString = "BossMagician";
        break;
        }
    case EnemyStateComponent::EnemyType::Warrior:
    {
        enemyString = "Warrior";
        break;
    }
    case EnemyStateComponent::EnemyType::Scout:
    {
        enemyString = "Scout";
        break;
    }
    case EnemyStateComponent::EnemyType::Tanker:
    {
        enemyString = "Tanker";
        break;
    }


    }


    std::string logMessage = "Enemy " + enemyString + " dead by " + cause_of_death;
    q_engine::Debugger::GetLogger().AddMessage(logMessage, q_engine::SeverityLevel::INFORMATIONAL);
}
